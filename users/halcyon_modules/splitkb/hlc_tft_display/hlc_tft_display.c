// Copyright 2024 splitkb.com (support@splitkb.com)
// SPDX-License-Identifier: GPL-2.0-or-later

#include "halcyon.h"
#include "hlc_tft_display.h"

#include "hardware/structs/rosc.h"

#include "rgb_matrix.h"

// Fonts mono2
#include "graphics/fonts/Retron2000-27.qff.h"
#include "graphics/fonts/Retron2000-underline-27.qff.h"

// Numbers mono2
#include "graphics/numbers/0.qgf.h"
#include "graphics/numbers/1.qgf.h"
#include "graphics/numbers/2.qgf.h"
#include "graphics/numbers/3.qgf.h"
#include "graphics/numbers/4.qgf.h"
#include "graphics/numbers/5.qgf.h"
#include "graphics/numbers/6.qgf.h"
#include "graphics/numbers/7.qgf.h"
#include "graphics/numbers/8.qgf.h"
#include "graphics/numbers/9.qgf.h"
#include "graphics/numbers/undef.qgf.h"

static const char *caps =        "C";
static const char *num =         "N";
static const char *scroll =      "S";

static painter_font_handle_t Retron27;
static painter_font_handle_t Retron27_underline;

static uint8_t lcd_surface_fb[SURFACE_REQUIRED_BUFFER_BYTE_SIZE(135, 240, 16)];

int color_value = 0;

painter_device_t lcd;
painter_device_t lcd_surface;

led_t last_led_usb_state = {0};
//layer_state_t last_layer_state = {0};

const char* LayerName(int layer){
	switch(layer){
	case 0:
		return "Base";
	case 1:
		return "Navig";
	case 2:		
		return "Number L";
	case 3:
		return "Right";
	case 4:
		return "NUMPAD";
	case 5:
		return "Bracket";
	case 6:
		return "Number R";
	case 7:
		return "[Light]";
	case 8:
		return "Mouse";
	case 9:
		return "--Game--";
	}
	return "";
}

uint16_t encode_layers(void)
{
    uint16_t mask = 0;

    for (uint8_t i = 0; i < 10; ++i)
    {
        mask |= (uint16_t)(IS_LAYER_ON(i)) << i;
    }

    return mask;
}


int LinePoxY(int i){
	return (Retron27->line_height*(i) + 5*(i+1));	
}

void update_display(void) {
    static bool font_loaded = false;
	static bool first_run_led = false;
	//static bool first_run_led_matrix = false;
    static bool first_run_layer = false;	
	static uint16_t last_layer_state = 0;
	static uint16_t curent_layer_state = 0;
	
	static bool ready = false;
	if (!ready && timer_read32() < 500) {
		return; // wait 500ms after boot
	}
	ready = true;
	
    if(font_loaded == false) {
        // Load fonts
        Retron27 = qp_load_font_mem(font_Retron2000_27);
        Retron27_underline = qp_load_font_mem(font_Retron2000_underline_27);
		font_loaded = true;   
    }	
    
	if(last_led_usb_state.raw != host_keyboard_led_state().raw || first_run_led == false) {
        led_t led_usb_state = host_keyboard_led_state();
		qp_rect(lcd_surface, 0, LCD_HEIGHT - Retron27->line_height - 5 , LCD_WIDTH - 1, LCD_HEIGHT - 1, HSV_BLACK, true);
		led_usb_state.caps_lock   ? 
			qp_drawtext_recolor(lcd_surface, 5, LCD_HEIGHT - Retron27->line_height - 5, Retron27_underline, caps,   HSV_CAPS_ON,   HSV_BLACK) 
			: qp_drawtext_recolor(lcd_surface, 5, LCD_HEIGHT - Retron27->line_height - 5, Retron27, caps,   HSV_CAPS_OFF,   HSV_BLACK);
		led_usb_state.num_lock    ? 
			qp_drawtext_recolor(lcd_surface, 5 + qp_textwidth(Retron27, "C"), LCD_HEIGHT - Retron27->line_height - 5, Retron27_underline, num,    HSV_NUM_ON,    HSV_BLACK) 
			: qp_drawtext_recolor(lcd_surface, 5 + qp_textwidth(Retron27, "C"), LCD_HEIGHT - Retron27->line_height - 5, Retron27, num,    HSV_NUM_OFF,    HSV_BLACK);
		led_usb_state.scroll_lock ? 
			qp_drawtext_recolor(lcd_surface, 5 + qp_textwidth(Retron27, "CN"), LCD_HEIGHT - Retron27->line_height - 5,      Retron27_underline, scroll, HSV_SCROLL_ON, HSV_BLACK) 
			: qp_drawtext_recolor(lcd_surface, 5 + qp_textwidth(Retron27, "CN"), LCD_HEIGHT - Retron27->line_height - 5,      Retron27, scroll, HSV_SCROLL_OFF, HSV_BLACK);
		
        last_led_usb_state = led_usb_state;
        first_run_led = true;
    }
	
	//if(!first_run_led_matrix){
		//rgb_matrix_mode(43);
		//rgb_matrix_sethsv(88, 255, 128);
		//rgb_matrix_set_speed(63);
		//first_run_led_matrix = true;
	//}
	
	curent_layer_state = encode_layers();
	bool layer_change = false;
	
    if(last_layer_state != curent_layer_state || first_run_layer == false) {
		qp_rect(lcd_surface, 0, 0, LCD_WIDTH - 1, LCD_HEIGHT - Retron27->line_height - 5, HSV_BLACK, true);
		
        if (is_keyboard_left()){
			qp_drawtext_recolor(lcd_surface, 5, LinePoxY(0), Retron27_underline, "[Left]" , HSV_SCROLL_ON, HSV_BLACK);
			if (IS_LAYER_OFF(1) && IS_LAYER_OFF(2) && IS_LAYER_OFF(3) && IS_LAYER_OFF(4) && IS_LAYER_OFF(9)){
				qp_drawtext_recolor(lcd_surface, 5, LinePoxY(1), Retron27, LayerName(0) , HSV_SCROLL_OFF, HSV_BLACK);
			}
			if (IS_LAYER_ON(1)){
				qp_drawtext_recolor(lcd_surface, 5, LinePoxY(2), Retron27, LayerName(1) , HSV_SCROLL_OFF, HSV_BLACK);
			}
			if (IS_LAYER_ON(2)){
				qp_drawtext_recolor(lcd_surface, 5, LinePoxY(3), Retron27, LayerName(2) , HSV_SCROLL_OFF, HSV_BLACK);
			}
			if (IS_LAYER_ON(3)){
				qp_drawtext_recolor(lcd_surface, 5, LinePoxY(4), Retron27, LayerName(3) , HSV_SCROLL_OFF, HSV_BLACK);
			}
			if (IS_LAYER_ON(4)){
				qp_drawtext_recolor(lcd_surface, 5, LinePoxY(5), Retron27, LayerName(4) , HSV_SCROLL_OFF, HSV_BLACK);
			}
			if (IS_LAYER_ON(9)){
				qp_drawtext_recolor(lcd_surface, 5, LinePoxY(1), Retron27, LayerName(9) , HSV_SCROLL_ON, HSV_BLACK);
			}			
		}else{
			qp_drawtext_recolor(lcd_surface, 5, LinePoxY(0), Retron27_underline, "[Right]" , HSV_SCROLL_ON, HSV_BLACK);
			if (IS_LAYER_OFF(5) && IS_LAYER_OFF(6) && IS_LAYER_OFF(7) && IS_LAYER_OFF(8)){
				qp_drawtext_recolor(lcd_surface, 5, LinePoxY(1), Retron27, LayerName(0) , HSV_SCROLL_OFF, HSV_BLACK);
			}
			if (IS_LAYER_ON(5)){
				qp_drawtext_recolor(lcd_surface, 5, LinePoxY(2), Retron27, LayerName(5) , HSV_SCROLL_OFF, HSV_BLACK);
			}
			if (IS_LAYER_ON(6)){
				qp_drawtext_recolor(lcd_surface, 5, LinePoxY(3), Retron27, LayerName(6) , HSV_SCROLL_OFF, HSV_BLACK);
			}
			if (IS_LAYER_ON(8)){
				qp_drawtext_recolor(lcd_surface, 5, LinePoxY(5), Retron27, LayerName(8) , HSV_SCROLL_OFF, HSV_BLACK);
			}	 
		}
        last_layer_state = curent_layer_state;
        first_run_layer = true;
		if(IS_LAYER_ON(7))
			layer_change = true;
    }
	
	static HSV last_hsv = {};
	static uint8_t last_mode = 0;
	static uint8_t last_speed = 0;
	
	HSV hsv = rgb_matrix_get_hsv();
	uint8_t mode = rgb_matrix_get_mode();
	uint8_t speed = rgb_matrix_get_speed();
	
	if ( layer_change || (IS_LAYER_ON(7) && 
		( (hsv.h != last_hsv.h || hsv.s != last_hsv.s || hsv.v != last_hsv.v) || 
			last_mode  != mode || 
			last_speed != speed) 
		) ){
		qp_rect(lcd_surface, 0, 0, LCD_WIDTH - 1, LCD_HEIGHT - Retron27->line_height - 5, HSV_BLACK, true);		
		char buf[24];
		
		qp_drawtext_recolor(lcd_surface, 5, LinePoxY(0), Retron27, LayerName(7), HSV_SCROLL_ON, HSV_BLACK);

		snprintf(buf, sizeof(buf), "H:%3d", hsv.h);
		qp_drawtext_recolor(lcd_surface, 5, LinePoxY(1), Retron27, buf, HSV_SCROLL_OFF, HSV_BLACK);

		snprintf(buf, sizeof(buf), "S:%3d", hsv.s);
		qp_drawtext_recolor(lcd_surface, 5, LinePoxY(2), Retron27, buf, HSV_SCROLL_OFF, HSV_BLACK);

		snprintf(buf, sizeof(buf), "V:%3d", hsv.v);
		qp_drawtext_recolor(lcd_surface, 5, LinePoxY(3), Retron27, buf, HSV_SCROLL_OFF, HSV_BLACK);

		snprintf(buf, sizeof(buf), "M:%2d", mode);
		qp_drawtext_recolor(lcd_surface, 5, LinePoxY(4), Retron27, buf, HSV_SCROLL_OFF, HSV_BLACK);
		
		snprintf(buf, sizeof(buf), "SPD:%3d", speed);
		qp_drawtext_recolor(lcd_surface, 5, LinePoxY(5), Retron27, buf, HSV_SCROLL_OFF, HSV_BLACK);
		last_hsv = hsv;
		last_mode = mode;
		last_speed = speed;
	}
}

// Called from halcyon.c
void module_suspend_power_down_kb(void) {
    qp_power(lcd, false);
}

// Called from halcyon.c
void module_suspend_wakeup_init_kb(void) {
    qp_power(lcd, true);
}

// Called from halcyon.c
bool module_post_init_kb(void) {
    // Turn on backlight
    backlight_enable();

    // Make the devices
    lcd = qp_st7789_make_spi_device(LCD_WIDTH, LCD_HEIGHT, LCD_CS_PIN, LCD_DC_PIN, LCD_RST_PIN, LCD_SPI_DIVISOR, LCD_SPI_MODE);
    lcd_surface = qp_make_rgb565_surface(LCD_WIDTH, LCD_HEIGHT, lcd_surface_fb);

    // Initialise the LCD
    qp_init(lcd, LCD_ROTATION);
    qp_set_viewport_offsets(lcd, LCD_OFFSET_X, LCD_OFFSET_Y);
    qp_clear(lcd);
    qp_rect(lcd, 0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1, HSV_BLACK, true);
    qp_power(lcd, true);
    qp_flush(lcd);

    // Initialise the LCD surface
    qp_init(lcd_surface, LCD_ROTATION);
    qp_rect(lcd_surface, 0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1, HSV_BLACK, true);
    qp_surface_draw(lcd_surface, lcd, 0, 0, 0);
    qp_flush(lcd);

    if(!module_post_init_user()) { return false; }

    return true;
}

// Called from halcyon.c
bool display_module_housekeeping_task_kb(bool second_display) {
    if(!display_module_housekeeping_task_user(second_display)) { return false; }
	
	update_display();

    // Move surface to lcd
	qp_surface_draw(lcd_surface, lcd, 0, 0, true);
	qp_flush(lcd);

    return true;
}
