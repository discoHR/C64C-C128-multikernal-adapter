// Firmware for C64C/C128 28-pin multi-kernal adapter
// Coded by discoHR, based on Bwack's code for multi-kernal adapter for breadbin

// CONFIG
#pragma config FOSC = INTRCIO   // Oscillator Selection bits (INTOSC oscillator: I/O function on GP4/OSC2/CLKOUT pin, I/O function on GP5/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-Up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // GP3/MCLR pin function select (GP3/MCLR pin function is digital I/O, MCLR internally tied to VDD)
#pragma config BOREN = OFF      // Brown-out Detect Enable bit (BOD disabled)
#pragma config CP = OFF         // Code Protection bit (Program Memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#define _XTAL_FREQ  4000000

#include <xc.h>
#include <pic12f629.h>

// inputs
#define SJ1         GPIObits.GP1
#define RESTORE_N   GPIObits.GP3

// outputs
#define RED_LED     GPIObits.GP0
#define A14         GPIObits.GP4
#define A15         GPIObits.GP5

// input/outputs
#define RESET_N     GPIObits.GP2

// EEPROM addresses used for configuration persistance
#define EEPROM_ADDR_KERNAL          0
#define EEPROM_ADDR_RED_INVERTED    2

#define KERNAL_MASK (SJ1 ? 3 : 1)
#define byte unsigned char

enum state_t {
    IDLE, SELECT
} state = IDLE;

byte buttonTimer, old_button;
byte kernalIndex, oldKernalIndex;
byte ignoreReset;
byte redInverted;

void DoReset(void) {
    RESET_N = 0;
    TRISIObits.TRISIO2 = 0; // pull INTRES_N low
    RED_LED ^= 1;
    __delay_ms(50);
    RED_LED ^= 1;
    __delay_ms(200);
    TRISIObits.TRISIO2 = 1; // release INTRES_N
    __delay_ms(250); // possible fix for double-reset cartridges
}

void SetKernal(byte index) {
    GPIO = (GPIO & 0x0f) | (index << 4);
    __delay_ms(20);
}

void SaveKernal(byte index) {
    // don't write unless necessary
    if (index != oldKernalIndex) {
        EEPROM_WRITE(EEPROM_ADDR_KERNAL, index);
        oldKernalIndex = index;
    }
}

void SetLED(void) {
    RED_LED = ((0 != kernalIndex) && redInverted) ^ ((buttonTimer & 2) ? 0 : 1);
}

void Init(void) {
    byte i;

    OPTION_REG = 0;
    WPUbits.WPU1 = 1;
    WPUbits.WPU2 = 1;
    WPUbits.WPU5 = 1;
    CMCON = 0x07; // digital IO
    TRISIO = 0b00101110;
    RESET_N = 1;
    RESTORE_N = 1;
    RED_LED = 0;
    __delay_ms(50);
    if (SJ1) {
        WPUbits.WPU5 = 0;
        TRISIO = 0b00001110;
    }
    kernalIndex = oldKernalIndex = EEPROM_READ(EEPROM_ADDR_KERNAL) & KERNAL_MASK;
    SetKernal(kernalIndex);
    redInverted = EEPROM_READ(EEPROM_ADDR_RED_INVERTED) & 1;
    DoReset();

    for (i = 0; i < 10; i++) {
        RED_LED = 0;
        __delay_ms(50);
        RED_LED = 1;
        __delay_ms(50);
    }

    __delay_ms(250); // ignore reset during power up
}

void main() {
    Init();

    // change the colour theme if user is holding the restore key on power-on
    if (!RESTORE_N) {
        redInverted ^= 1;
        EEPROM_WRITE(EEPROM_ADDR_RED_INVERTED, redInverted);
        // wait until restore key is released
        while (!RESTORE_N) {
            __delay_ms(100);
        }
    }

    // main loop
    for (;;) {
        SetLED();
        switch (state) {
            case IDLE:
                if (!RESTORE_N) {
                    ++buttonTimer;
                } else {
                    buttonTimer = 0;
                }
                if (buttonTimer > 15 || !RESET_N) {
                    // either the restore key was long-pressed or
                    // the reset button was short-pressed
                    state = SELECT;
                    old_button = buttonTimer = 0;
                    // ignore reset in the select state if it was the reset button
                    ignoreReset = !RESET_N;
                    RED_LED ^= 1;
                    __delay_ms(50);
                    RED_LED ^= 1;
                    __delay_ms(50);
                }
                break;

            case SELECT:
                if (!old_button && RESTORE_N && RESET_N) {
                    old_button = 1;
                } else if (old_button && (!RESTORE_N || !RESET_N)) {
                    old_button = ignoreReset = 0; // it's ok to reset after this
                    ++kernalIndex;
                    kernalIndex &= KERNAL_MASK;
                    SetKernal(kernalIndex);
                } else if (RESTORE_N && RESET_N) {
                    // both buttons released
                    ++buttonTimer;
                    if (buttonTimer > 30) {
                        // do the actual reset if needed and go back to idle state
                        old_button = 1;
                        if (!ignoreReset) {
                            DoReset();
                        }
                        SaveKernal(kernalIndex);
                        buttonTimer = 0;
                        state = IDLE;
                    }
                } else {
                    // at least one button is pressed
                    buttonTimer = 0;
                }
                break;

            default:
                // something is wrong, flash like crazy
                A14 = A15 = 0; // pull A14 and A15 low, makes red clearly visible
                RED_LED ^= 1;
                break;
        }
        __delay_ms(50);
    }
}