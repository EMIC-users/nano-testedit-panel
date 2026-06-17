
#ifndef PIC_GPIO_H_
#define PIC_GPIO_H_

#include "pins.h"

typedef enum
{
	GPIO_OUTPUT,
	GPIO_INPUT,
}gpio_pinConfig_t;

typedef enum
{
	GPIO_LOW,
	GPIO_HIGH
}gpio_state_t;



#define HAL_GPIO_PinCfg(name,cfg_value) TRIS_##name = cfg_value
#define HAL_GPIO_PinSet(name,value) LAT_##name = value
#define HAL_GPIO_PinToggle(name) LAT_##name = !LAT_##name
#define HAL_GPIO_PinGet(name) PIN_##name

// Enable the internal pull-up on a pin via its Change Notification (CN) bit.
// Requires the pin to have a CN_<name> define (not all pins support CN).
#define _HAL_GPIO_PU_CAT(n)  _CN##n##PUE
#define _HAL_GPIO_PU_EXP(n)  _HAL_GPIO_PU_CAT(n)
#define HAL_GPIO_PinPullUp(name) _HAL_GPIO_PU_EXP(CN_##name) = 1


#endif

