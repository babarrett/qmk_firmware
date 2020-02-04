# MCU name
MCU = atmega32u4
BOOTLOADER = atmel-dfu

CUSTOM_MATRIX			= yes
STENO_ENABLE			= yes
VIRTSER_ENABLE		= no
NKRO_ENABLE				= yes

DEBOUNCE_TYPE			= eager_pr

# Lets try and save some space eh?
MOUSEKEY_ENABLE   = no
EXTRAKEY_ENABLE   = no
CONSOLE_ENABLE		= yes
COMMAND_ENABLE		= no

SRC							+= matrix.c
QUANTUM_LIB_SRC += i2c_master.c
OPT_DEFS				+= -DONLYQWERTY -DDEBUG_MATRIX -save-temps -c
LTO_ENABLE = yes
