# Dynamic MQTT Component

Tato custom komponenta pro ESPHome automaticky nastavuje MQTT topic prefix založený na MAC adrese zařízení. Řeší problém s hardcoded topic prefixy tím, že každé zařízení dostane svůj unikátní prefix.

## Účel

Bez této komponenty by všechna zařízení používala stejný topic prefix definovaný v konfiguraci, což by způsobovalo konflikty v MQTT brokeru. Tato komponenta automaticky:

- Získá MAC adresu zařízení při startu
- Vytvoří unikátní topic prefix ve formátu `esp32device/MACADRESA`
- Nastaví všechny MQTT parametry dynamicky

## Výsledný formát

Pokud má zařízení MAC adresu `AA:BB:CC:DD:EE:FF`, komponenta vytvoří:

- **Topic prefix**: `esp32device/AABBCCDDEEFF`
- **Status topic**: `esp32device/AABBCCDDEEFF/status`
- **Discovery prefix**: `esp32device/AABBCCDDEEFF/discovery`
- **Všechny entity**: `esp32device/AABBCCDDEEFF/sensor/temperature/state` atd.

## Konfigurace

### 1. Přidání external_components

```yaml
external_components:
  - source:
      type: local
      path: custom_components
```

### 2. MQTT konfigurace

```yaml
mqtt:
  id: mqtt_client  # DŮLEŽITÉ! Musí mít ID
  broker: !secret mqtt_broker
  port: !secret mqtt_port
  username: !secret mqtt_username
  password: !secret mqtt_password
  topic_prefix: "temp"  # Dočasný prefix - bude nahrazen dynamickým
  discovery: true
  discovery_prefix: "temp"  # Dočasný prefix - bude nahrazen dynamickým
  birth_message:
    topic: "temp/status"
    payload: "online"
  will_message:
    topic: "temp/status"
    payload: "offline"
```

### 3. Aktivace dynamic_mqtt komponenty

```yaml
# MUSÍ BÝT PO mqtt: sekci!
dynamic_mqtt:
  id: dynamic_mqtt_component
```

## Důležité poznámky

### Pořadí konfigurace
Komponenta `dynamic_mqtt` **MUSÍ** být definována **PO** `mqtt` sekci, protože potřebuje přístup k již vytvořenému MQTT klientovi.

### ID v MQTT sekci
MQTT konfigurace **MUSÍ** obsahovat `id: mqtt_client`, jinak komponenta nebude fungovat.

### Dočasné prefixy
Hodnoty `topic_prefix` a `discovery_prefix` v MQTT konfiguraci jsou pouze dočasné - budou automaticky nahrazeny dynamickými hodnotami založenými na MAC adrese.

## Jak komponenta funguje

1. **Setup fáze**: Při startu zařízení se spustí metoda `setup()`
2. **Získání MAC**: Pomocí `get_mac_address_raw()` získá MAC adresu
3. **Formátování**: Převede MAC na string bez dvojteček (např. `AABBCCDDEEFF`)
4. **Nastavení prefixu**: Vytvoří topic prefix `esp32device/MACADRESA`
5. **Aktualizace MQTT**: Nastaví nový prefix pro všechny MQTT zprávy
6. **Birth/Will zprávy**: Aktualizuje birth a will zprávy s novým prefixem

## Výhody

- ✅ **Žádné hardcoded MAC adresy** v konfiguraci
- ✅ **Automatická unikátnost** každého zařízení
- ✅ **Snadná správa** mnoha zařízení současně
- ✅ **Kompatibilita s ESPHome 2025.5.1**
- ✅ **Transparentní fungování** - ostatní komponenty o změně nevědí

## Příklad výstupu v logu

```
[I][dynamic_mqtt:22] Setting MQTT topic prefix to: esp32device/a0b1c2d3e4f5
```

## Technické detaily

### Závislosti
- ESPHome 2025.5.1+
- MQTT komponenta s nastaveným ID

### Soubory komponenty
- `__init__.py` - ESPHome integrace a kódová generace
- `dynamic_mqtt.h` - Header soubor s definicí třídy
- `dynamic_mqtt.cpp` - Implementace komponenty

### API kompatibilita
Komponenta používá správná API volání pro ESPHome 2025.5.1:
- `set_topic_prefix(prefix, prefix)` místo starého jednoho parametru
- `set_birth_message({...})` a `set_last_will({...})` s rvalue reference
- Správné include cesty pro ESPHome headers

## Možné rozšíření

V budoucnu lze komponentu rozšířit o:
- Konfigurovatelný prefix (místo hardcoded "esp32device")
- Možnost použít jiný identifikátor než MAC adresu
- Volitelné formátování MAC adresy (s/bez dvojteček, velká/malá písmena)
- Callback pro notifikaci o změně prefixu
