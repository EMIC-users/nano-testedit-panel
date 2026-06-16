

// LEDs
#define TRIS_Led1                   _TRISC9
#define PIN_Led1                    _RC9
#define LAT_Led1                    _LATC9
#define ODC_Led1                    _ODC9
#define RPOUT_Led1                  RPOR12bits.RP25R
#define RPIN_Led1                   25
#define CN_Led1                     19
#define TRIS_Led2                   _TRISC8
#define PIN_Led2                    _RC8
#define LAT_Led2                    _LATC8
#define ODC_Led2                    _ODC8
#define RPOUT_Led2                  RPOR12bits.RP24R
#define RPIN_Led2                   24
#define CN_Led2                     20

// OLED Display control pins (SSD1322 via PMP)
#define TRIS_OLED_CS	                _TRISB15
#define PORT_OLED_CS 	                _RB15
#define LAT_OLED_CS 	                _LATB15
#define ODC_OLED_CS	                _ODB15
#define PIN_OLED_CS 	                _RB15
#define RPOUT_OLED_CS	                RPOR7bits.RP15R
#define RPIN_OLED_CS	                15
#define CN_OLED_CS		                11
#define ADC_value_OLED_CS              Buffer_entradas[9] 
#define HAL_SetAnalog_OLED_CS()        {_PCFG9=0;\
                                        adc_addAnalogChannel(9);}
#define TRIS_OLED_SHDN                   _TRISC6
#define PIN_OLED_SHDN                    _RC6
#define LAT_OLED_SHDN                    _LATC6
#define ODC_OLED_SHDN                    _ODC6
#define RPOUT_OLED_SHDN                  RPOR11bits.RP22R
#define RPIN_OLED_SHDN                   22
#define CN_OLED_SHDN                     18
#define TRIS_OLED_DI                   _TRISC5
#define PIN_OLED_DI                    _RC5
#define LAT_OLED_DI                    _LATC5
#define ODC_OLED_DI                    _ODC5
#define RPOUT_OLED_DI                  RPOR10bits.RP21R
#define RPIN_OLED_DI                   21
#define CN_OLED_DI                     26
#define TRIS_OLED_RW	                _TRISB13
#define PORT_OLED_RW 	                _RB13
#define LAT_OLED_RW 	                _LATB13
#define ODC_OLED_RW	                _ODB13
#define PIN_OLED_RW 	                _RB13
#define RPOUT_OLED_RW		            RPOR6bits.RP13R
#define RPIN_OLED_RW		            13
#define CN_OLED_RW		                13
#define ADC_value_OLED_RW              Buffer_entradas[11] 
#define HAL_SetAnalog_OLED_RW()        {_PCFG11=0;\
                                        adc_addAnalogChannel(11);}

#define TRIS_OLED_E	                _TRISB14
#define PORT_OLED_E 	                _RB14
#define LAT_OLED_E 	                _LATB14
#define ODC_OLED_E	                _ODB14
#define PIN_OLED_E 	                _RB14
#define RPOUT_OLED_E	                RPOR7bits.RP14R
#define RPIN_OLED_E	                14
#define CN_OLED_E		                12
#define ADC_value_OLED_E              Buffer_entradas[10] 
#define HAL_SetAnalog_OLED_E()        {_PCFG10=0;\
                                        adc_addAnalogChannel(10);}

#define TRIS_OLED_RST                   _TRISC7
#define PIN_OLED_RST                    _RC7
#define LAT_OLED_RST                    _LATC7
#define ODC_OLED_RST                    _ODC7
#define RPOUT_OLED_RST                  RPOR11bits.RP23R
#define RPIN_OLED_RST                   23
#define CN_OLED_RST                     17

// Keyboard 3x4 (3 columns, 4 rows)
#define TRIS_KBD1_COL2	_TRISA1
#define LAT_KBD1_COL2 	_LATA1
#define ODC_KBD1_COL2	_ODA1
#define PIN_KBD1_COL2 	_RA1
#define CN_KBD1_COL2		3
#define ADC_value_KBD1_COL2              Buffer_entradas[1] 
#define HAL_SetAnalog_KBD1_COL2()        {_PCFG1=0;\
                                        adc_addAnalogChannel(1);}

#define TRIS_KBD1_COL1	                _TRISA0
#define PORT_KBD1_COL1 	                _A0
#define LAT_KBD1_COL1 	                _LATA0
#define ODC_KBD1_COL1	                _ODA0
#define PIN_KBD1_COL1 	                _RA0
#define CN_KBD1_COL1		                2
#define ADC_value_KBD1_COL1              Buffer_entradas[0] 
#define HAL_SetAnalog_KBD1_COL1()        {_PCFG0=0;\
                                        adc_addAnalogChannel(0);}


#define TRIS_KBD1_COL0                   _TRISC3
#define PIN_KBD1_COL0                    _RC3
#define LAT_KBD1_COL0                    _LATC3
#define ODC_KBD1_COL0                    _ODC3
#define RPOUT_KBD1_COL0                  RPOR9bits.RP19R
#define RPIN_KBD1_COL0                   19
#define CN_KBD1_COL0                     28
#define TRIS_KBD1_ROW0                   _TRISA7
#define PIN_KBD1_ROW0                    _RA7
#define LAT_KBD1_ROW0                    _LATA7
#define ODC_KBD1_ROW0                    _ODA7
#define TRIS_KBD1_ROW1	                _TRISA4
#define LAT_KBD1_ROW1 	                _LATA4
#define ODC_KBD1_ROW1	                _ODA4
#define PIN_KBD1_ROW1 	                _RA4
#define CN_KBD1_ROW1		                1
#define TRIS_KBD1_ROW2	_TRISA3
#define LAT_KBD1_ROW2 	_LATA3
#define ODC_KBD1_ROW2	_ODA3
#define PIN_KBD1_ROW2 	_RA3
#define CN_KBD1_ROW2		29
#define TRIS_KBD1_ROW3	_TRISA2
#define LAT_KBD1_ROW3 	_LATA2
#define ODC_KBD1_ROW3	_ODA2
#define PIN_KBD1_ROW3 	_RA2
#define CN_KBD1_ROW3		30

// Keyboard 3x3 (3 columns, 3 rows)
#define TRIS_KBD2_COL0                   _TRISC2
#define PIN_KBD2_COL0                    _RC2
#define LAT_KBD2_COL0                    _LATC2
#define ODC_KBD2_COL0                    _ODC2
#define RPOUT_KBD2_COL0                  RPOR9bits.RP18R
#define RPIN_KBD2_COL0                   18
#define CN_KBD2_COL0                     10
#define ADC_value_KBD2_COL0              Buffer_entradas[8]
#define HAL_SetAnalog_KBD2_COL0()        {_PCFG8=0;\
                                        adc_addAnalogChannel(8);}
#define TRIS_KBD2_COL1                   _TRISC1
#define PIN_KBD2_COL1                    _RC1
#define LAT_KBD2_COL1                    _LATC1
#define ODC_KBD2_COL1                    _ODC1
#define RPOUT_KBD2_COL1                  RPOR8bits.RP17R
#define RPIN_KBD2_COL1                   17
#define CN_KBD2_COL1                     9
#define ADC_value_KBD2_COL1              Buffer_entradas[7]
#define HAL_SetAnalog_KBD2_COL1()        {_PCFG7=0;\
                                        adc_addAnalogChannel(7);}
#define TRIS_KBD2_COL2                   _TRISC0
#define PIN_KBD2_COL2                    _RC0
#define LAT_KBD2_COL2                    _LATC0
#define ODC_KBD2_COL2                    _ODC0
#define RPOUT_KBD2_COL2                  RPOR8bits.RP16R
#define RPIN_KBD2_COL2                   16
#define CN_KBD2_COL2                     8
#define ADC_value_KBD2_COL2              Buffer_entradas[6]
#define HAL_SetAnalog_KBD2_COL2()        {_PCFG6=0;\
                                        adc_addAnalogChannel(6);}
#define TRIS_KBD2_ROW0                   _TRISA8
#define PIN_KBD2_ROW0                    _RA8
#define LAT_KBD2_ROW0                    _LATA8
#define ODC_KBD2_ROW0                    _ODA8
#define TRIS_KBD2_ROW1                   _TRISA9
#define PIN_KBD2_ROW1                    _RA9
#define LAT_KBD2_ROW1                    _LATA9
#define ODC_KBD2_ROW1                    _ODA9
#define TRIS_KBD2_ROW2                   _TRISA10
#define PIN_KBD2_ROW2                    _RA10
#define LAT_KBD2_ROW2                    _LATA10
#define ODC_KBD2_ROW2                    _ODA10

// Board name

