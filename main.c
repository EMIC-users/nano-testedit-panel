#include <xc.h>
#include "inc/systemConfig.h"

#include "inc/systemTimer.h"
#include "inc/led_Led1.h"
#include "inc/led_Led2.h"
#include "inc/timer_api1.h"
#include "inc/conversionFunctions.h"
#include "inc/Graphics_OLED.h"
#include "inc/Edit_Edit1.h"
#include "inc/Keyboard_Pad.h"
#include "inc/Keyboard_Nav.h"
#include "inc/EMICBus.h"
#include "inc/system.h"
#include "inc/userFncFile.h"

#include "system.c"

int main(void)
{
	initSystem();
	systemTimeInit();
	LEDs_Led1_init();
	LEDs_Led2_init();
	Graphics_OLED_init();
	Edit_Edit1_init();
	Keyboard_Pad_init();
	Keyboard_Nav_init();
	EMICBus_init();
	onReset();
	do
	{
		Graphics_OLED_poll();
		Keyboard_Pad_poll();
		Keyboard_Nav_poll();
		poll_EMICBus();
	}
	while(1);
}

