
#ifndef SSD1322_OLED_H_
#define SSD1322_OLED_H_

/*==================[inclusions]=============================================*/

#include <stdint.h>
#include "gpio.h"
#include "pmp.h"

/*==================[macros]=================================================*/

#define SSD1322_OLED_WIDTH    128
#define SSD1322_OLED_HEIGHT   64
#define SSD1322_OLED_COLUMNS  64

// SSD1322 commands
#define SSD1322_CMD_SET_COLUMN    0x15
#define SSD1322_CMD_WRITE_RAM     0x5C
#define SSD1322_CMD_SET_ROW       0x75
#define SSD1322_CMD_SET_REMAP     0xA0
#define SSD1322_CMD_SET_START     0xA1
#define SSD1322_CMD_SET_OFFSET    0xA2
#define SSD1322_CMD_NORMAL_DISP   0xA6
#define SSD1322_CMD_SET_MUX       0xA8
#define SSD1322_CMD_SET_PHASE     0xB1
#define SSD1322_CMD_SET_CLOCK     0xB3
#define SSD1322_CMD_SET_GPIO      0xB5
#define SSD1322_CMD_SET_PRECHARGE 0xBB
#define SSD1322_CMD_SET_VCOMH     0xBE
#define SSD1322_CMD_SET_CONTRAST  0xC1
#define SSD1322_CMD_MASTER_BRIGHT 0xC7
#define SSD1322_CMD_FUNCTION_SEL  0xAB
#define SSD1322_CMD_DISPLAY_ON    0xAF
#define SSD1322_CMD_DISPLAY_OFF   0xAE

/*==================[external functions declaration]=========================*/

void SSD1322_OLED_init(void);
void SSD1322_OLED_sendCommand(uint8_t cmd);
void SSD1322_OLED_sendData(uint8_t data);
void SSD1322_OLED_setColumn(uint8_t start, uint8_t end);
void SSD1322_OLED_setRow(uint8_t start, uint8_t end);
void SSD1322_OLED_refresh(uint64_t *framebuffer, uint16_t width);
void SSD1322_OLED_clear(void);
void SSD1322_OLED_setContrast(uint8_t contrast);
void SSD1322_OLED_powerOn(void);
void SSD1322_OLED_powerOff(void);

/*==================[end of file]============================================*/
#endif

