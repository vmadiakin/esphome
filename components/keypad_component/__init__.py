import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins
from esphome.const import CONF_ID

DEPENDENCIES = ['gpio']

# пространство имён
keypad_ns = cg.esphome_ns.namespace('keypad_component')
KeypadComponent = keypad_ns.class_('KeypadComponent', cg.Component)

# Конфигурация YAML
CONF_ROWS = 'row_pins'
CONF_COLS = 'col_pins'
CONF_LOCK = 'lock'

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(KeypadComponent),
    cv.Required(CONF_ROWS): cv.ensure_list(pins.gpio_input_pin_schema),
    cv.Required(CONF_COLS): cv.ensure_list(pins.gpio_input_pin_schema),
    cv.Required(CONF_LOCK): cv.use_id(cg.GPIOOutput),
}).extend(cv.COMPONENT_SCHEMA)

# регистрация компонента как yaml-платформы
async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    # строки
    var.row_pins_ = [await cg.gpio_pin_expression(x) for x in config[CONF_ROWS]]
    # столбцы
    var.col_pins_ = [await cg.gpio_pin_expression(x) for x in config[CONF_COLS]]
    # реле
    var.lock_ = await cg.get_variable(config[CONF_LOCK])
