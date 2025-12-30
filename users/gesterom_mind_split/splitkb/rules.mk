# May need to be changed when adding more pointing devices
POINTING_DEVICE_ENABLE = no
POINTING_DEVICE_DRIVER = cirque_pinnacle_spi

# May need to be changed when adding more displays
QUANTUM_PAINTER_ENABLE = yes
QUANTUM_PAINTER_DRIVERS += st7789_spi surface

BACKLIGHT_ENABLE = yes
BACKLIGHT_DRIVER = pwm

CONSOLE_ENABLE = yes

VPATH += $(USER_PATH)/splitkb/
SRC += $(USER_PATH)/splitkb/halcyon.c
HALCONFDIR += $(USER_PATH)/splitkb/halconf.h
POST_CONFIG_H += $(USER_PATH)/splitkb/config.h

include $(USER_PATH)/splitkb/hlc_tft_display/rules.mk

HLC_OPTIONS := $(HLC_TFT_DISPLAY)
