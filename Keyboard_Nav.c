
/*==================[inclusions]=============================================*/

#include "inc/Keyboard_Nav.h"
#include "inc/systemTimer.h"
#include "inc/system.h"        // FCY, required by libpic30 __delay_us
#include <libpic30.h>

/*==================[internal data]=========================================*/

#define KBD_Nav_KEYS  (KBD_Nav_COLS * KBD_Nav_ROWS)

static uint8_t  keyState[KBD_Nav_KEYS];
static uint8_t  keyPrev[KBD_Nav_KEYS];
static uint8_t  debounce[KBD_Nav_KEYS];
static uint8_t  mode = 0;
static uint32_t lastScan = 0;

// Physical index (row*cols+col) -> key code reported by onPress/onRelease.
// Filled from the key0..key11 instance parameters; unmapped entries default
// to their own index, so an instance without keymap behaves as before.
// Fixed size 12 = physical maximum of this API (4 rows x 3 columns).
static const char kbdMap[12] = {
    [0]  = KEY_AUX3,  [1]  = KEY_AUX4,  [2]  = KEY_AUX5,
    [3]  = KEY_UP,  [4]  = KEY_AUX1,  [5]  = KEY_AUX2,
    [6]  = KEY_DOWN,  [7]  = KEY_RIGHT,  [8]  = KEY_LEFT,
    [9]  = 9,  [10] = 10, [11] = 11
};

/*==================[internal functions]====================================*/

// Drive a row LOW (enable its output; LAT was pre-loaded to 0 in init).
static void driveRow(uint8_t row)
{
    switch (row)
    {
        case 0: HAL_GPIO_PinCfg(KBD2_ROW0, GPIO_OUTPUT); break;
        case 1: HAL_GPIO_PinCfg(KBD2_ROW1, GPIO_OUTPUT); break;
        case 2: HAL_GPIO_PinCfg(KBD2_ROW2, GPIO_OUTPUT); break;
        case 3: HAL_GPIO_PinCfg(KBD2_ROW2, GPIO_OUTPUT); break;
    }
}

// Release a row (hi-Z: back to input).
static void releaseRow(uint8_t row)
{
    switch (row)
    {
        case 0: HAL_GPIO_PinCfg(KBD2_ROW0, GPIO_INPUT); break;
        case 1: HAL_GPIO_PinCfg(KBD2_ROW1, GPIO_INPUT); break;
        case 2: HAL_GPIO_PinCfg(KBD2_ROW2, GPIO_INPUT); break;
        case 3: HAL_GPIO_PinCfg(KBD2_ROW2, GPIO_INPUT); break;
    }
}

// Read a column sense line (1 = released/high, 0 = pressed/low).
static uint8_t readCol(uint8_t col)
{
    switch (col)
    {
        case 0: return HAL_GPIO_PinGet(KBD2_COL0);
        case 1: return HAL_GPIO_PinGet(KBD2_COL1);
        case 2: return HAL_GPIO_PinGet(KBD2_COL2);
    }
    return 1;
}

/*==================[external functions]====================================*/

void Keyboard_Nav_init(void)
{
    uint8_t i;
    for (i = 0; i < KBD_Nav_KEYS; i++)
    {
        keyState[i] = 0;
        keyPrev[i]  = 0;
        debounce[i] = 0;
    }

    // Columns: inputs with internal pull-up (sense lines).
    HAL_GPIO_PinCfg(KBD2_COL0, GPIO_INPUT); HAL_GPIO_PinPullUp(KBD2_COL0);
    HAL_GPIO_PinCfg(KBD2_COL1, GPIO_INPUT); HAL_GPIO_PinPullUp(KBD2_COL1);
    HAL_GPIO_PinCfg(KBD2_COL2, GPIO_INPUT); HAL_GPIO_PinPullUp(KBD2_COL2);

    // Rows: pre-load LAT=0 and leave hi-Z (input). driveRow() enables output.
    HAL_GPIO_PinSet(KBD2_ROW0, GPIO_LOW); HAL_GPIO_PinCfg(KBD2_ROW0, GPIO_INPUT);
    HAL_GPIO_PinSet(KBD2_ROW1, GPIO_LOW); HAL_GPIO_PinCfg(KBD2_ROW1, GPIO_INPUT);
    HAL_GPIO_PinSet(KBD2_ROW2, GPIO_LOW); HAL_GPIO_PinCfg(KBD2_ROW2, GPIO_INPUT);
    HAL_GPIO_PinSet(KBD2_ROW2, GPIO_LOW); HAL_GPIO_PinCfg(KBD2_ROW2, GPIO_INPUT);
}

void Keyboard_Nav_setMode(uint8_t m)
{
    mode = m;
}

void Keyboard_Nav_poll(void)
{
    uint8_t row, col, idx, pressed;
    uint32_t now = getSystemMilis();

    // Throttle the scan so the debounce counter measures real time.
    if ((uint32_t)(now - lastScan) < KBD_Nav_SCAN_MS) return;
    lastScan = now;

    for (row = 0; row < KBD_Nav_ROWS; row++)
    {
        driveRow(row);
        __delay_us(15);   // let the weak pull-up settle the column lines

        for (col = 0; col < KBD_Nav_COLS; col++)
        {
            idx = row * KBD_Nav_COLS + col;
            pressed = !readCol(col);

            if (pressed != keyPrev[idx])
            {
                debounce[idx] = KBD_Nav_DEBOUNCE;
                keyPrev[idx] = pressed;
            }

            if (debounce[idx] > 0)
            {
                debounce[idx]--;
                if (debounce[idx] == 0)
                {
                    if (pressed && !keyState[idx])
                    {
                        keyState[idx] = 1;
                    }
                    else if (!pressed && keyState[idx])
                    {
                        keyState[idx] = 0;
                    }
                }
            }
        }

        releaseRow(row);
    }
}

/*==================[end of file]============================================*/

