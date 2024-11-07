#include "debug.h"
#include "lana.h"

int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    Delay_Init();
    initNeopixel();

    pinMode(PIN_PA0, OUTPUT);

    while (1) {
        digitalWrite(PIN_PA0, HIGH);
        Delay_Ms(500);
        digitalWrite(PIN_PA0, LOW);
        Delay_Ms(500);
    }
}
