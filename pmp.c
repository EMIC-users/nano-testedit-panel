
/*==================[inclusions]=============================================*/

#include "inc/pmp.h"

/*==================[internal functions definition]==========================*/

void PMP_init(void)
{
    // Known-good configuration from the legacy driver (Master Mode 1).
    // The previous Master-Mode-2 setup did not produce a usable write strobe
    // and the BUSY poll in PMP_write could hang the display init.
    PMAEN  = 0x0000;
    PMMODE = 0x0300;   // Master Mode 1, 8-bit data, no wait states
    PMCON  = 0x8383;   // PMPEN=1 + strobe/polarity bits (legacy values)
}

void PMP_write(uint8_t data)
{
    // Equivalent to the legacy PMPMasterWrite(): writing PMDIN1 triggers the
    // PMP write cycle and its hardware PMWR strobe. The legacy driver does NOT
    // poll BUSY here (it was unreliable on this part); the strobe (E/PMWR) must
    // NOT be toggled manually by the caller.
    PMDIN1 = data;
}

uint8_t PMP_read(void)
{
    while(PMMODEbits.BUSY);
    return (uint8_t)PMDIN1;
}

uint8_t PMP_isBusy(void)
{
    return PMMODEbits.BUSY;
}

