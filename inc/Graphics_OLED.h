
#ifndef GRAPHICS_OLED_H_
#define GRAPHICS_OLED_H_

/*==================[inclusions]=============================================*/

#include <stdint.h>
#include "SSD1322_OLED.h"
#include "streamOut.h"

/*==================[macros]=================================================*/

#define GFX_OLED_WIDTH    128
#define GFX_OLED_HEIGHT   64

#define GFX_FONT_5X7    0
#define GFX_FONT_7X9    1
#define GFX_FONT_16X32  2

#define GFX_BIND_U8     0
#define GFX_BIND_U16    1
#define GFX_BIND_U32    2
#define GFX_BIND_I8     3
#define GFX_BIND_I16    4
#define GFX_BIND_I32    5
#define GFX_BIND_FLOAT  6
#define GFX_BIND_U64    7
#define GFX_BIND_I64    8
#define GFX_BIND_CHAR   9

/* Max number of simultaneously bound fields (static, no heap) */
#define GFX_OLED_MAX_BINDS  8

/*==================[types]==================================================*/

typedef char* (*gfxProvider_t)(void* var, uint8_t type, char* mask);

/*==================[external data declaration]==============================*/

extern uint64_t Graphics_OLED_framebuffer[GFX_OLED_WIDTH];
extern const streamOut_t streamOut_Graphic_OLED;
/*==================[init/poll]==============================================*/

void Graphics_OLED_init(void);

void Graphics_OLED_poll(void);

/*==================[public API]============================================*/

void Graphics_OLED_clear(void);






void Graphics_OLED_bindFncAt(uint8_t x, uint8_t y, uint8_t fontId, gfxProvider_t provider, void* var, uint8_t type, char* mask);



/*==================[end of file]============================================*/
#endif /* GRAPHICS_OLED_H_ */

