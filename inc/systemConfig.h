// CONFIG2
#pragma config POSCMOD = NONE       // Primary Oscillator disabled
#pragma config I2C1SEL = PRI        // Use default SCL1/SDA1 pins
#pragma config IOL1WAY = ON         // IOLOCK protection
#pragma config OSCIOFNC = ON        // OSC2/CLKO as I/O
#pragma config FCKSM = CSDCMD       // Clock switching disabled
#pragma config FNOSC = FRCPLL       // Fast RC Oscillator with PLL
#pragma config SOSCSEL = SOSC       // Default Secondary Oscillator
#pragma config WUTSEL = LEG         // Legacy Wake-up Timer
#pragma config IESO = OFF           // Two-Speed Start-up disabled
// CONFIG1
#pragma config WDTPS = PS16384      // Watchdog Postscaler 1:16384
#pragma config FWPSA = PR128        // WDT Prescaler 1:128
#pragma config WINDIS = OFF         // Windowed WDT disabled
#pragma config FWDTEN = OFF         // Watchdog Timer disabled
#pragma config ICS = PGx1           // Emulator on PGC1/PGD1
#pragma config GWRP = OFF           // General Code Segment Write Protect off
#pragma config GCP = OFF            // Code protection disabled
#pragma config JTAGEN = OFF         // JTAG disabled
