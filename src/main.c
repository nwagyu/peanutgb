#include <eadk.h>
#include <stdlib.h>
#include "peanut_gb/peanut_gb.h"

const char eadk_app_name[] __attribute__((section(".rodata.eadk_app_name"))) = "Game Boy";
const uint32_t eadk_api_level  __attribute__((section(".rodata.eadk_api_level"))) = 0;

uint8_t gb_rom_read(struct gb_s * gb, const uint_fast32_t addr) {
  return eadk_external_data[addr];
}

void gb_cart_ram_write(struct gb_s *gb, const uint_fast32_t addr, const uint8_t val) {
}

uint8_t gb_cart_ram_read(struct gb_s *gb, const uint_fast32_t addr) {
  return 0xFF;
}

void gb_error(struct gb_s *gb, const enum gb_error_e gb_err, const uint16_t val) {
  //  printf("GB_ERROR %d %d %d\n", gb_err, GB_INVALID_WRITE, val);
  return;
}

eadk_color_t palette_original[4] = {0x8F80, 0x24CC, 0x4402, 0x0A40};
eadk_color_t palette_gray[4] = {eadk_color_white, 0xAD55, 0x52AA, eadk_color_black};
eadk_color_t palette_gray_negative[4] = {eadk_color_black, 0x52AA, 0xAD55, eadk_color_white};
eadk_color_t * palette = palette_original;

eadk_color_t eadk_color_from_gb_pixel(uint8_t gb_pixel) {
    uint8_t gb_color = gb_pixel & 0x3;
    return palette[gb_color];
}

static void lcd_draw_line_centered(struct gb_s * gb, const uint8_t * input_pixels, const uint_fast8_t line) {
  eadk_color_t output_pixels[LCD_WIDTH];
  for (int i=0; i<LCD_WIDTH; i++) {
    output_pixels[i] = eadk_color_from_gb_pixel(input_pixels[i]);
  }
  eadk_display_push_rect((eadk_rect_t){(EADK_SCREEN_WIDTH-LCD_WIDTH)/2, (EADK_SCREEN_HEIGHT-LCD_HEIGHT)/2+line, LCD_WIDTH, 1}, output_pixels);
}

static void lcd_draw_line_maximized(struct gb_s * gb, const uint8_t * input_pixels, const uint_fast8_t line) {
  // Nearest neighbor scaling of a 160x144 texture to a 320x240 resolution
  // Horizontally, we just double
  eadk_color_t output_pixels[2*LCD_WIDTH];
  for (int i=0; i<LCD_WIDTH; i++) {
    eadk_color_t color = eadk_color_from_gb_pixel(input_pixels[i]);
    output_pixels[2*i] = color;
    output_pixels[2*i+1] = color;
  }
  // Vertically, we want to scale by a 5/3 ratio. So we need to make 5 lines out of three:  we double two lines out of three.
  uint16_t y = (5*line)/3;
  eadk_display_push_rect((eadk_rect_t){0, y, 2*LCD_WIDTH, 1}, output_pixels);
  if (line%3 != 0) {
    eadk_display_push_rect((eadk_rect_t){0, y+1, 2*LCD_WIDTH, 1}, output_pixels);
  }
}

struct gb_s gb;

int main(int argc, char * argv[]) {
  eadk_display_push_rect_uniform(eadk_screen_rect, eadk_color_black);

  int ret = gb_init(&gb, gb_rom_read, gb_cart_ram_read, gb_cart_ram_write, gb_error, NULL);
  if (ret != GB_INIT_NO_ERROR) {
    return -1;
  }
  gb_init_lcd(&gb, lcd_draw_line_maximized);

  while (true) {
    gb_run_frame(&gb);

    eadk_keyboard_state_t kbd = eadk_keyboard_scan();
    gb.direct.joypad_bits.a = !eadk_keyboard_key_down(kbd, eadk_key_back);
    gb.direct.joypad_bits.b = !eadk_keyboard_key_down(kbd, eadk_key_ok);
    gb.direct.joypad_bits.select = !eadk_keyboard_key_down(kbd, eadk_key_shift);
    gb.direct.joypad_bits.start = !eadk_keyboard_key_down(kbd, eadk_key_backspace);
    gb.direct.joypad_bits.right = !eadk_keyboard_key_down(kbd, eadk_key_right);
    gb.direct.joypad_bits.left = !eadk_keyboard_key_down(kbd, eadk_key_left);
    gb.direct.joypad_bits.up = !eadk_keyboard_key_down(kbd, eadk_key_up);
    gb.direct.joypad_bits.down = !eadk_keyboard_key_down(kbd, eadk_key_down);

    if (eadk_keyboard_key_down(kbd, eadk_key_one)) {
      palette = palette_original;
    }
    if (eadk_keyboard_key_down(kbd, eadk_key_two)) {
      palette = palette_gray;
    }
    if (eadk_keyboard_key_down(kbd, eadk_key_three)) {
      palette = palette_gray_negative;
    }
    if (eadk_keyboard_key_down(kbd, eadk_key_plus)) {
      gb.display.lcd_draw_line = lcd_draw_line_maximized;
    }
    if (eadk_keyboard_key_down(kbd, eadk_key_minus)) {
      eadk_display_push_rect_uniform(eadk_screen_rect, eadk_color_black);
      gb.display.lcd_draw_line = lcd_draw_line_centered;
    }
  }

  return 0;
}
