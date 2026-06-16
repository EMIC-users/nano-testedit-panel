
#ifndef PIC_PMP_H_
#define PIC_PMP_H_

/*==================[inclusions]=============================================*/

#include <xc.h>
#include <stdint.h>

/*==================[macros]=================================================*/

#define PMP_DATA_8BIT   0
#define PMP_DATA_16BIT  1

/*==================[external functions declaration]=========================*/

void PMP_init(void);

void PMP_write(uint8_t data);

uint8_t PMP_read(void);

uint8_t PMP_isBusy(void);

/*==================[end of file]============================================*/
#endif

