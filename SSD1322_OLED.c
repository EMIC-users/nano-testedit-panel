
/*==================[inclusions]=============================================*/

#include "inc/SSD1322_OLED.h"
#include "inc/system.h"   // defines FCY, required by libpic30 __delay_us
#include <libpic30.h>

/*==================[low level read/write]===================================*/

void SSD1322_OLED_sendCommand(uint8_t cmd)
{
    HAL_GPIO_PinSet(OLED_DI, GPIO_LOW);   // command
    PMP_write(cmd);
    HAL_GPIO_PinSet(OLED_DI, GPIO_HIGH);  // back to data
}

void SSD1322_OLED_sendData(uint8_t data)
{
    PMP_write(data);                        // DI already HIGH (data)
}

void SSD1322_OLED_setColumn(uint8_t start, uint8_t end)
{
    SSD1322_OLED_sendCommand(SSD1322_CMD_SET_COLUMN);
    SSD1322_OLED_sendData(start);
    SSD1322_OLED_sendData(end);
}

void SSD1322_OLED_setRow(uint8_t start, uint8_t end)
{
    SSD1322_OLED_sendCommand(SSD1322_CMD_SET_ROW);
    SSD1322_OLED_sendData(start);
    SSD1322_OLED_sendData(end);
}

/*==================[init]===================================================*/

void SSD1322_OLED_init(void)
{
    // Control pins as outputs
    HAL_GPIO_PinCfg(OLED_CS,   GPIO_OUTPUT);
    HAL_GPIO_PinCfg(OLED_DI,   GPIO_OUTPUT);
    HAL_GPIO_PinCfg(OLED_RW,   GPIO_OUTPUT);
    HAL_GPIO_PinCfg(OLED_E,    GPIO_OUTPUT);
    HAL_GPIO_PinCfg(OLED_RST,  GPIO_OUTPUT);
    // SHDN intentionally NOT driven (as the proven legacy driver did). The board
    // sets the panel-enable level; driving SHDN here kept the panel dark.

    HAL_GPIO_PinSet(OLED_RW,   GPIO_LOW);    // write direction

    PMP_init();

    HAL_GPIO_PinSet(OLED_CS,  GPIO_LOW);     // chip select held LOW from here on

    // Reset pulse
    HAL_GPIO_PinSet(OLED_RST, GPIO_LOW);
    __delay_us(200);
    HAL_GPIO_PinSet(OLED_RST, GPIO_HIGH);
    __delay_us(200);

    // Initialization sequence (exact values from the proven legacy driver)
    SSD1322_OLED_sendCommand(0xAE);                                  // display OFF
    SSD1322_OLED_sendCommand(0xB3); SSD1322_OLED_sendData(0x91); // clock div / osc freq
    SSD1322_OLED_sendCommand(0xCA); SSD1322_OLED_sendData(0x3F); // MUX ratio (64)
    SSD1322_OLED_sendCommand(0xA2); SSD1322_OLED_sendData(0x00); // display offset
    SSD1322_OLED_sendCommand(0xAB); SSD1322_OLED_sendData(0x01); // function (internal VDD)
    SSD1322_OLED_sendCommand(0xA0); SSD1322_OLED_sendData(0x05); SSD1322_OLED_sendData(0x11); // re-map
    SSD1322_OLED_sendCommand(0xC7); SSD1322_OLED_sendData(0x0F); // master contrast current
    SSD1322_OLED_sendCommand(0xC1); SSD1322_OLED_sendData(0x9F); // contrast current
    SSD1322_OLED_sendCommand(0xB1); SSD1322_OLED_sendData(0xF2); // phase length
    SSD1322_OLED_sendCommand(0xBB); SSD1322_OLED_sendData(0x1F); // pre-charge voltage
    SSD1322_OLED_sendCommand(0xB4); SSD1322_OLED_sendData(0xA0); SSD1322_OLED_sendData(0xFD); // VSL
    SSD1322_OLED_sendCommand(0xBE); SSD1322_OLED_sendData(0x04); // VCOMH
    SSD1322_OLED_sendCommand(0xA6);                                  // normal display
    SSD1322_OLED_sendCommand(0xA1); SSD1322_OLED_sendData(0x00); // start line
    SSD1322_OLED_sendCommand(0xA8); SSD1322_OLED_sendData(0x00); SSD1322_OLED_sendData(0x40);
    SSD1322_OLED_sendCommand(0xAF);                                  // display ON

    SSD1322_OLED_clear();
}

/*==================[clear]=================================================*/

void SSD1322_OLED_clear(void)
{
    uint16_t i;
    SSD1322_OLED_setColumn(28, 91);
    SSD1322_OLED_setRow(0, 63);
    SSD1322_OLED_sendCommand(SSD1322_CMD_WRITE_RAM);
    for (i = 0; i < 64 * 64; i++)
    {
        SSD1322_OLED_sendData(0x00);
        SSD1322_OLED_sendData(0x00);
    }
}

/*==================[refresh]===============================================*/

void SSD1322_OLED_refresh(uint64_t *framebuffer, uint16_t width)
{
    uint8_t j;
    uint16_t k;
    uint64_t dato1, dato2;

    // Single addressing window, then stream the whole framebuffer (proven on
    // hardware). Each pair of framebuffer columns maps to one SSD1322 column.
    SSD1322_OLED_setColumn(28, 91);
    SSD1322_OLED_setRow(0, 63);
    SSD1322_OLED_sendCommand(SSD1322_CMD_WRITE_RAM);

    for (k = 0; k < width; k += 2)
    {
        dato1 = framebuffer[k];
        dato2 = framebuffer[k + 1];

        // 64 rows, two bytes per row (one per source column bit)
        for (j = 64; j > 0; j--)
        {
            SSD1322_OLED_sendData((dato1 & 1) ? 0xFF : 0x00);
            SSD1322_OLED_sendData((dato2 & 1) ? 0xFF : 0x00);
            dato1 >>= 1;
            dato2 >>= 1;
        }
    }
}

/*==================[misc]==================================================*/

void SSD1322_OLED_setContrast(uint8_t contrast)
{
    SSD1322_OLED_sendCommand(SSD1322_CMD_SET_CONTRAST);
    SSD1322_OLED_sendData(contrast);
}

void SSD1322_OLED_powerOn(void)
{
    HAL_GPIO_PinSet(OLED_SHDN, GPIO_HIGH);
    SSD1322_OLED_sendCommand(SSD1322_CMD_DISPLAY_ON);
}

void SSD1322_OLED_powerOff(void)
{
    SSD1322_OLED_sendCommand(SSD1322_CMD_DISPLAY_OFF);
    HAL_GPIO_PinSet(OLED_SHDN, GPIO_LOW);
}

/*==================[end of file]============================================*/

