
/*==================[inclusions]=============================================*/

#include "inc/Graphics_OLED.h"
#include "inc/font_5x7.h"
#include "inc/font_7x9.h"
#include "inc/font_16x32.h"
#include "inc/streamOut.h"
#include "inc/streamIn.h"

/*==================[internal macros]=======================================*/

#define bit_set(var,bit)     ((var) |= ((uint64_t)1 << (bit)))
#define bit_clear(var,bit)   ((var) &= ~((uint64_t)1 << (bit)))

/*==================[internal data]=========================================*/

uint64_t Graphics_OLED_framebuffer[GFX_OLED_WIDTH];

static void putChar(char c);
static uint16_t putCharAvailable(void);
const streamOut_t streamOut_Graphic_OLED = {putChar, putCharAvailable};

static int16_t cursorX = 0;
static int16_t cursorY = 0;
static uint8_t currentFont = GFX_FONT_5X7;
static uint8_t fontWidth = 5;
static uint8_t fontHeight = 7;

static const uint8_t fontWidths[]  = {5, 7, 16};
static const uint8_t fontHeights[] = {7, 9, 32};
static const char    fontFirst[]   = {' ', ' ', '0'};
static const char    fontLast[]    = {'^', 'Z', '9'};


typedef struct {
    gfxProvider_t provider;  /* produces the text to draw                     */
    void*    ptr;            /* watched GLOBAL variable, passed to provider    */
    char*    mask;           /* raw picture mask, passed verbatim to provider  */
    uint8_t  type;           /* GFX_BIND_* selector, passed to provider        */
    uint8_t  x;
    uint8_t  y;
    uint8_t  font;
} gfxFncBind_t;

static gfxFncBind_t gfxFncBinds[GFX_OLED_MAX_BINDS];
static uint8_t      gfxFncBindCount = 0;

static void setFont(uint8_t fontId);   /* defined below in the text section */

/* Draw a ready-made string at (x,y) with the given font (no formatting). */
static void drawStrAt(uint8_t fontId, uint8_t x, uint8_t y, const char* s)
{
    if (s == 0) return;
    setFont(fontId);
    cursorX = x;
    cursorY = y;
    while (*s) putChar(*s++);
}

/*==================[init/poll]============================================*/

void Graphics_OLED_init(void)
{
    uint16_t i;
    for (i = 0; i < GFX_OLED_WIDTH; i++)
        Graphics_OLED_framebuffer[i] = 0;
    cursorX = 0;
    cursorY = 0;
    currentFont = GFX_FONT_5X7;
    fontWidth = fontWidths[GFX_FONT_5X7];
    fontHeight = fontHeights[GFX_FONT_5X7];
    SSD1322_OLED_init();
}

void Graphics_OLED_poll(void)
{

    // Provider-backed fields: ask each provider for its text and draw it. The
    // provider owns the formatting (it received var/type/mask at bind time).
    uint8_t kf;
    for (kf = 0; kf < gfxFncBindCount; kf++)
    {
        gfxFncBind_t* bf = &gfxFncBinds[kf];
        if (bf->provider == 0) continue;   // bind sin provider: nada que dibujar
        char* txt = bf->provider(bf->ptr, bf->type, bf->mask);
        drawStrAt(bf->font, bf->x, bf->y, txt);
    }

    // Flush framebuffer to the display hardware
    SSD1322_OLED_refresh(Graphics_OLED_framebuffer, GFX_OLED_WIDTH);
}

/*==================[pixel / clear]========================================*/

void Graphics_OLED_clear(void)
{
    uint16_t i;
    for (i = 0; i < GFX_OLED_WIDTH; i++)
        Graphics_OLED_framebuffer[i] = 0;
    gfxFncBindCount = 0;   // ...and the provider-backed fields too
}

void Graphics_OLED_pixel(uint8_t x, uint8_t y, uint8_t set)
{
    if (x >= GFX_OLED_WIDTH || y >= GFX_OLED_HEIGHT) return;
    if (set)
        bit_set(Graphics_OLED_framebuffer[x], y);
    else
        bit_clear(Graphics_OLED_framebuffer[x], y);
}

/*==================[line]=================================================*/

void Graphics_OLED_line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
    int16_t dx = (x2 > x1) ? (x2 - x1) : (x1 - x2);
    int16_t dy = (y2 > y1) ? (y2 - y1) : (y1 - y2);
    int16_t sx = (x1 < x2) ? 1 : -1;
    int16_t sy = (y1 < y2) ? 1 : -1;
    int16_t err = dx - dy;
    int16_t e2;
    int16_t cx = x1, cy = y1;

    for (;;)
    {
        if (cx >= 0 && cx < GFX_OLED_WIDTH && cy >= 0 && cy < GFX_OLED_HEIGHT)
            bit_set(Graphics_OLED_framebuffer[cx], cy);

        if (cx == x2 && cy == y2) break;
        e2 = 2 * err;
        if (e2 > -dy) { err -= dy; cx += sx; }
        if (e2 <  dx) { err += dx; cy += sy; }
    }
}

/*==================[rect / circle]=======================================*/



/*==================[text]=================================================*/

static void setFont(uint8_t fontId)
{
    if (fontId > 2) fontId = 0;
    currentFont = fontId;
    fontWidth = fontWidths[fontId];
    fontHeight = fontHeights[fontId];
}

static uint16_t putCharAvailable()
{
    return 1;
}

static void putChar(char c)
{
    uint8_t col, row;
    uint16_t offset;

    if (c < fontFirst[currentFont] || c > fontLast[currentFont]) return;
    offset = (uint16_t)(c - fontFirst[currentFont]) * fontWidth;

    for (col = 0; col < fontWidth; col++)
    {
        if (cursorX + col >= GFX_OLED_WIDTH) break;

        int32_t bitmap;
        switch (currentFont)
        {
            case GFX_FONT_7X9:   bitmap = font_7x9[offset + col];   break;
            case GFX_FONT_16X32: bitmap = font_16x32[offset + col]; break;
            default:             bitmap = font_5x7[offset + col];   break;
        }

        for (row = 0; row < fontHeight; row++)
        {
            if (cursorY + row >= GFX_OLED_HEIGHT) break;
            if (bitmap & ((int32_t)1 << row))
                bit_set(Graphics_OLED_framebuffer[cursorX + col], cursorY + row);
            else
                bit_clear(Graphics_OLED_framebuffer[cursorX + col], cursorY + row);
        }
    }
    cursorX += fontWidth + 1;
}

void Graphics_OLED_print(char* format, ...)
{
	va_list arg;
    va_start(arg, format);

	sendDataToStream(&streamOut_Graphic_OLED,format,arg);
	va_end(arg);

}

void Graphics_OLED_printAt(uint8_t x, uint8_t y, uint8_t fontId, char* format, ...)
{
    setFont(fontId);
    cursorX = x;
    cursorY = y;
    
    va_list arg;
    va_start(arg, format);

	sendDataToStream(&streamOut_Graphic_OLED,format,arg);

	va_end(arg);
}

/*==================[bindAt — reactive numeric fields]=====================*/


void Graphics_OLED_bindFncAt(uint8_t x, uint8_t y, uint8_t fontId, gfxProvider_t provider, void* var, uint8_t type, char* mask)
{
    if (gfxFncBindCount >= GFX_OLED_MAX_BINDS) return;   // table full: drop silently
    gfxFncBinds[gfxFncBindCount].provider = provider;
    gfxFncBinds[gfxFncBindCount].ptr      = var;
    gfxFncBinds[gfxFncBindCount].mask     = mask;
    gfxFncBinds[gfxFncBindCount].type     = type;
    gfxFncBinds[gfxFncBindCount].x        = x;
    gfxFncBinds[gfxFncBindCount].y        = y;
    gfxFncBinds[gfxFncBindCount].font     = fontId;
    gfxFncBindCount++;
}

/*==================[end of file]============================================*/

