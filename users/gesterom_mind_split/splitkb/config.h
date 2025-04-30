// Copyright 2024 splitkb.com (support@splitkb.com)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#define SPLIT_TRANSACTION_IDS_KB MODULE_SYNC

#define HLC_BACKLIGHT_TIMEOUT 120000

#define BACKLIGHT_PWM_DRIVER PWMD5
#define BACKLIGHT_LEVELS 10
#define BACKLIGHT_PWM_CHANNEL RP2040_PWM_CHANNEL_B

//// Keyboard redefines

// Always the same
#define BACKLIGHT_PIN GP2 //NOT CONNECTED
#define POINTING_DEVICE_CS_PIN GP2 //NOT CONNECTED
#define HLC_ENCODER_A NO_PIN
#define HLC_ENCODER_B NO_PIN

#define SPLIT_MODS_ENABLE
#define SPLIT_LED_STATE_ENABLE
#define SPLIT_LAYER_STATE_ENABLE

// Elora
#if defined(KEYBOARD_splitkb_halcyon_elora_rev2)
    #undef ENCODER_A_PINS
    #define ENCODER_A_PINS { GP22, HLC_ENCODER_A }
    #undef ENCODER_B_PINS
    #define ENCODER_B_PINS { GP18, HLC_ENCODER_B }
    #undef MATRIX_ROWS
    #define MATRIX_ROWS 12
    #define LAYOUT_elora_hlc(k0G, k0F, k0E, k0D, k0C, k0B, k6B, k6C, k6D, k6E, k6F, k6G, k1G, k1F, k1E, k1D, k1C, k1B, k7B, k7C, k7D, k7E, k7F, k7G, k2G, k2F, k2E, k2D, k2C, k2B, k8B, k8C, k8D, k8E, k8F, k8G, k3G, k3F, k3E, k3D, k3C, k3B, k4D, k3A, k9A, k10D, k9B, k9C, k9D, k9E, k9F, k9G, k4E, k4C, k4B, k4F, k4A, k10A, k10F, k10B, k10C, k10E, k5A, k5B, k5C, k5D, k5E, k11A, k11B, k11C, k11D, k11E) { \
	 {KC_NO, k0B, k0C, k0D, k0E, k0F, k0G}, \
	 {KC_NO, k1B, k1C, k1D, k1E, k1F, k1G}, \
	 {KC_NO, k2B, k2C, k2D, k2E, k2F, k2G}, \
	 {k3A, k3B, k3C, k3D, k3E, k3F, k3G}, \
	 {k4A, k4B, k4C, k4D, k4E, k4F, KC_NO}, \
     {k5A, k5B, k5C, k5D, k5E, KC_NO, KC_NO}, \
	 {KC_NO, k6B, k6C, k6D, k6E, k6F, k6G}, \
	 {KC_NO, k7B, k7C, k7D, k7E, k7F, k7G}, \
	 {KC_NO, k8B, k8C, k8D, k8E, k8F, k8G}, \
	 {k9A, k9B, k9C, k9D, k9E, k9F, k9G}, \
	 {k10A, k10B, k10C, k10D, k10E, k10F, KC_NO}, \
     {k11A, k11B, k11C, k11D, k11E, KC_NO, KC_NO} \
}
#endif
