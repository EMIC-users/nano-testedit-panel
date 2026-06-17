
#ifndef KEYCODES_H_
#define KEYCODES_H_

/*==================[macros]=================================================*/

#define KEY_NONE        0x00

/* Real ASCII codes */
#define KEY_BACKSPACE   0x08
#define KEY_TAB         0x09
#define KEY_ENTER       0x0D
#define KEY_ESC         0x1B
#define KEY_SPACE       0x20
#define KEY_COMMA       0x2C   /* ',' cannot travel as a literal in EMIC parameters */
#define KEY_DOT         0x2E

/* Extended codes (no ASCII representation) */
#define KEY_UP          0x80
#define KEY_DOWN        0x81
#define KEY_LEFT        0x82
#define KEY_RIGHT       0x83

#define KEY_AUX1        0x90
#define KEY_AUX2        0x91
#define KEY_AUX3        0x92
#define KEY_AUX4        0x93
#define KEY_AUX5        0x94

/*==================[end of file]============================================*/
#endif

