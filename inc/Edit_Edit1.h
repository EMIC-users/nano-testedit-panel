
#ifndef EDIT_Edit1_H_
#define EDIT_Edit1_H_

#include <stdint.h>

/*==================[macros]=================================================*/

#define EDIT_TYPE_U8     0
#define EDIT_TYPE_U16    1
#define EDIT_TYPE_U32    2
#define EDIT_TYPE_I8     3
#define EDIT_TYPE_I16    4
#define EDIT_TYPE_I32    5
#define EDIT_TYPE_FLOAT  6
#define EDIT_TYPE_U64    7
#define EDIT_TYPE_I64    8
#define EDIT_TYPE_CHAR   9

#define EDIT_Edit1_MAXLEN    16    /* max mask/buffer width (excluding the '\0') */
#define EDIT_Edit1_BLINK_MS  400   /* cursor blink half-period (ms) */

/*==================[init]===================================================*/

void Edit_Edit1_init(void);

/*==================[public API]=============================================*/

char* Edit_Edit1_editableVar(void* var, uint8_t type, char* mask);

void Edit_Edit1_begin(void* var, uint8_t type, char* mask);

void Edit_Edit1_accept(void);


void Edit_Edit1_nextDigit(void);

void Edit_Edit1_prevDigit(void);

void Edit_Edit1_incDigit(void);

void Edit_Edit1_decDigit(void);

void Edit_Edit1_changeCharacter(char c);

/*==================[end of file]============================================*/
#endif /* EDIT_Edit1_H_ */

