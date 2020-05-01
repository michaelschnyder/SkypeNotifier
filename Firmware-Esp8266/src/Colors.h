#ifndef Colors_h
#define Colors_h

#define BGR16(color) rgb32_to_bgr16(color)
#define rgb32_to_bgr16(color) rgbc32_to_bgr16((color & 0xFF0000) >> 16, (color & 0x00FF00) >> 8, (color & 0x0000FF))
#define rgbc32_to_bgr16(red, green, blue) ((blue & 0b11111000) << 8) | ((green & 0b11111100) << 3) | (red >> 3)

#define RGB16(color) rgb32_to_rgb16(color)
#define rgb32_to_rgb16(color) rgbc32_to_rgb16((color & 0xFF0000) >> 16, (color & 0x00FF00) >> 8, (color & 0x0000FF))
#define rgbc32_to_rgb16(red, green, blue) ((red & 0b11111000) << 8) | ((green & 0b11111100) << 3) | (blue >> 3)

uint16_t BLACK     = RGB16(0x000000);
uint16_t WHITE     = RGB16(0xffffff);

uint16_t RED     = RGB16(0xff0000);
uint16_t GREEN   = RGB16(0x00ff00);
uint16_t BLUE    = RGB16(0x0000ff);

uint16_t STRONG_RED    = RGB16(0xb30000);
uint16_t STRONG_GREEN  = RGB16(0x00b300);
uint16_t STRONG_BLUE   = RGB16(0x005ab3);

uint16_t DARK_RED      = RGB16(0x8b0000);
uint16_t DARK_GREEN    = RGB16(0x006400);
uint16_t DARK_BLUE     = RGB16(0x00008b);

uint16_t ORANGE        = RGB16(0xffa500);
uint16_t STRONG_ORANGE = RGB16(0xb37400);
uint16_t DARK_ORANGE   = RGB16(0x9b6300);
 
uint16_t YELLOW        = RGB16(0xffff00);
uint16_t STRONG_YELLOW = RGB16(0xffd500);
uint16_t DARK_YELLOW   = RGB16(0xe6c000);
 
uint16_t VIOLET        = RGB16(0x9400e8);
uint16_t STRONG_VIOLET = RGB16(0x8400cf);
uint16_t DARK_VIOLET   = RGB16(0x63009b);

#endif