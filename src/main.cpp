#include "mbed.h"
#include "TM1637.h"


#define COUNTDOWN_DAYS 14
#define SECONDS_PER_DAY 86400
#define TOTAL_COUNTDOWN_SECONDS (COUNTDOWN_DAYS * SECONDS_PER_DAY)
#define RESET_HOLD_TICKS 3 // Number of seconds to hold button for reset
Ticker oneSecondTimer;
volatile bool tick = false;
DigitalOut myled(LED1);
DigitalIn button(p10);

PwmOut buttonLed(p21);

//buttonLed = 0.0; // Initialize LED to off

// Use the correct pins for your hardware
TM1637_CATALEX displayDaysHours(p6, p7); // DIO, CLK
TM1637_CATALEX displayMinutesSeconds(p8, p9); // DIO, CLK
uint32_t secondsRemaining = TOTAL_COUNTDOWN_SECONDS;
uint8_t buttonHoldTicks = 0;

void OnTick() {
    tick = true;
}

int main() {
    float brightness = 0.0f;
    float step = 0.01f;
    bool increasing = true;

    displayDaysHours.cls();
    displayMinutesSeconds.cls();
    displayDaysHours.setDisplay(true);
    displayMinutesSeconds.setDisplay(true);
    displayDaysHours.printf("1234");
    displayMinutesSeconds.printf("4321");
    oneSecondTimer.attach(&OnTick, 1.0);
    displayDaysHours.setIcon(TM1637_CATALEX::COL2);
    displayMinutesSeconds.setIcon(TM1637_CATALEX::COL2);

    buttonLed.period_ms(1);

    while (true) {

        if(tick) {
            tick = false;
            if (secondsRemaining > 0) {
                secondsRemaining--;

                uint32_t days = secondsRemaining / SECONDS_PER_DAY;
                uint32_t hours = (secondsRemaining % SECONDS_PER_DAY) / 3600;
                uint32_t minutes = (secondsRemaining % 3600) / 60;
                uint32_t seconds = secondsRemaining % 60;

                displayDaysHours.printf("%02d%02d", days, hours);
                displayMinutesSeconds.printf("%02d%02d", minutes, seconds);
                displayDaysHours.setIcon(TM1637_CATALEX::COL2);
                displayMinutesSeconds.setIcon(TM1637_CATALEX::COL2);
            } else {
                displayDaysHours.printf("0000");
                displayMinutesSeconds.printf("0000");
            }
            myled = !myled; // Toggle LED to indicate a tick
            if(button.read())
            {
                buttonHoldTicks++;
                if(buttonHoldTicks >= RESET_HOLD_TICKS) {
                    secondsRemaining = TOTAL_COUNTDOWN_SECONDS;
                }
            } else {
                buttonHoldTicks = 0;
            }

            buttonLed = brightness; // Set LED brightness (0.0 to 1.0)


            if (increasing) {
                brightness += step;
                if (brightness >= 1.0f) {
                    brightness = 1.0f;
                    increasing = false;
                }
            } else {
                brightness -= step;
                if (brightness <= 0.0f) {
                    brightness = 0.0f;
                    increasing = true;
                }
            }
        }
    }
}