#include "lana.h"
#include "stdlib.h"

#define N 1
uint8_t rgbArray[3 * N]; // Each color is 3 bytes

void LED_SendBit(uint8_t bit)
{
    if (bit) {
    //// Send a 1 bit
        GPIOD->BSHR = 1 << 0; // put pin C4 high and wait for 800nS
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");
        GPIOD->BCR = 1 << 0; // put pin C4 low and exit, 400nS is taken up by other functions
        return;
        }
//    else {
        // Send a 0 bit
        GPIOD->BSHR = 1 << 0; // put pin C4 high and wait for 400nS
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");
        GPIOD->BCR = 1 << 0; // put pin C4 low and wait for 400nS, 400nS is taken up by other functions
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
            __asm__("nop");__asm__("nop");__asm__("nop");__asm__("nop");
//    }
}

// Send a single colour for a single LED
//WS2812B LEDs want 24 bits per led in the string
void LED_SendColour(uint8_t red, uint8_t green, uint8_t blue)
{
    // Send the green component first (MSB)
    for (int i = 7; i >= 0; i--) {
        LED_SendBit((green >> i) & 1);
    }
    // Send the red component next
    for (int i = 7; i >= 0; i--) {
        LED_SendBit((red >> i) & 1);
    }
    // Send the blue component last (LSB)
    for (int i = 7; i >= 0; i--) {
        LED_SendBit((blue >> i) & 1);
    }
}

void Write(){
    for (int i = 0; i< N; i++){
        LED_SendColour(rgbArray[i*3],rgbArray[i*3+1],rgbArray[i*3+2]);
    }
    Delay_Ms(1);  //todo: adjust to minimal time needed
}

void SetLed(int i, uint8_t r,uint8_t g,uint8_t b){
    rgbArray[i*3] = r;
    rgbArray[i*3+1] = g;
    rgbArray[i*3+2] = b;
}

GPIO_TypeDef* PinToPort(int pin)
{
    if(pin <= 15) return GPIOA;
    if(pin <= 31) return GPIOB;
    if(pin <= 63) return GPIOD;
    return 0;
}

uint32_t PinToPeriph(int pin)
{
    if(pin <= 15) return RCC_APB2Periph_GPIOA;
    if(pin <= 31) return RCC_APB2Periph_GPIOB;
    if(pin <= 63) return RCC_APB2Periph_GPIOD;
    return 0;
}

uint16_t PinToBitMask(int pin)
{
    return 1<<(pin%16);
}

void DisableSWD_UsePinsAsGPIO() {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void EnableSWD_UsePinsAsGPIO() {
       RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA, ENABLE);
       GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, DISABLE);
}

void pinMode(uint8_t pin, uint8_t mode)
{
    if (pin == PIN_PA13 || pin == PIN_PA14){
        DisableSWD_UsePinsAsGPIO();
    }

    RCC_APB2PeriphClockCmd(PinToPeriph(pin), ENABLE);
    GPIO_InitTypeDef gpio_pin = {0};
    gpio_pin.GPIO_Pin = PinToBitMask(pin);
    switch (mode) {
        case OUTPUT:
            gpio_pin.GPIO_Mode = GPIO_Mode_Out_PP;
            break;
        case INPUT_PULLUP:
            gpio_pin.GPIO_Mode = GPIO_Mode_IPU;
            break;
        case INPUT_PULLDOWN:
             gpio_pin.GPIO_Mode = GPIO_Mode_IPD;
             break;
        case INPUT:
             gpio_pin.GPIO_Mode = GPIO_Mode_IN_FLOATING;
             break;
        default:
            gpio_pin.GPIO_Mode = GPIO_Mode_IN_FLOATING;
            break;
    }

    gpio_pin.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(PinToPort(pin), &gpio_pin);
}

void digitalWrite(uint8_t pin, int value)
{
    GPIO_WriteBit(PinToPort(pin), PinToBitMask(pin), value);
}

uint8_t digitalRead(uint8_t pin){
    return GPIO_ReadInputDataBit(PinToPort(pin), PinToBitMask(pin));
}

void initNeopixel()
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    GPIO_PinRemapConfig(AFIO_PCFR1_PD01_REMAP, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure = {0};
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
}

