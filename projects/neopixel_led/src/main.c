#include "debug.h"
#include "lana.h"

#define NEOPIXEL_PIN PD0
#define LED_INDEX 0

static uint32_t Color(uint8_t r, uint8_t g, uint8_t b)
{
    return ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
}

uint8_t Red(uint32_t color)
{
    return (color >> 16) & 0xFF;
}

uint8_t Green(uint32_t color)
{
    return (color >> 8) & 0xFF;
}

uint8_t Blue(uint32_t color)
{
    return color & 0xFF;
}

uint32_t Wheel(uint8_t WheelPos)
{
    WheelPos = 255 - WheelPos;
    if(WheelPos < 85)
    {
        return Color(255 - WheelPos * 3, 0, WheelPos * 3);
    }
    else if(WheelPos < 170)
    {
        WheelPos -= 85;
        return Color(0, WheelPos * 3, 255 - WheelPos * 3);
    }
    else
    {
        WheelPos -= 170;
        return Color(WheelPos * 3, 255 - WheelPos * 3, 0);
    }
}

int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    Delay_Init();
    initNeopixel();

    uint8_t i = 0;
    while (1)
    {
        uint32_t color = Wheel(i);
        SetLed(LED_INDEX, Red(color), Green(color), Blue(color));
        Write();
        Delay_Ms(33);
        i++;  // 8bit value -> 255+1=0
    }
    
}
