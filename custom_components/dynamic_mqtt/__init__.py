# custom_components/dynamic_mqtt/__init__.py
import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import core
from esphome.const import CONF_ID
from esphome.components import mqtt

DEPENDENCIES = ['mqtt']
CODEOWNERS = ['@custom']

# Definování namespace pro custom komponentu  
dynamic_mqtt_ns = cg.esphome_ns.namespace("dynamic_mqtt")
DynamicMQTTComponent = dynamic_mqtt_ns.class_("DynamicMQTTComponent", cg.Component)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(DynamicMQTTComponent),
}).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
    # Vytvoření instance komponenty
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    
    # Získání MQTT klienta a předání do custom komponenty
    mqtt_component = await cg.get_variable(core.CORE.config["mqtt"][CONF_ID])
    cg.add(var.set_mqtt_client(mqtt_component))