#include <xc.h>
#include "inc/led_Led2.h"
#include "inc/gpio.h"
#include "inc/systemTimer.h"

void LEDs_Led2_init (void)
{
	HAL_GPIO_PinCfg(Led2, GPIO_OUTPUT);
}




