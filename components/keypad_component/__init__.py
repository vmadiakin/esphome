import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ID

keypad_ns = cg.esphome_ns.namespace("keypad_component")
KeypadComponent = keypad_ns.class_("KeypadComponent", cg.Component)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(KeypadComponent),
}).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
