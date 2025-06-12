#ifndef ULN2003_H
#define ULN2003_H

#if defined(STM32F103xB) || defined(STM32F103xE)
#include "stm32f1xx_hal.h" // STM32F1 series
#elif defined(STM32F407xx) || defined(STM32F429xx)
#include "stm32f4xx_hal.h" // STM32F4 series
#else
#error "Unsupported or unknown STM32 family"
#endif
#include <stdint.h>

typedef enum {
    ULN2003_OK = 0,
    ULN2003_ERROR
} ULN2003_Status;

typedef struct{
	GPIO_TypeDef* port;
	uint16_t pin;
}GpioPin;

typedef enum{
	ULN2003_DIRECTION_FORWARD,
	ULN2003_DIRECTION_BACKWARD
}ULN2003_DIRECTION;

typedef struct {
    GpioPin IN1;
    GpioPin IN2;
    GpioPin IN3;
    GpioPin IN4;

    GPIO_TypeDef* ports[4];
    uint16_t pins[4];

    uint32_t step_delay_ms;
    uint8_t step_sequence[8][4];
    uint8_t current_step;
} ULN2003_HandleTypeDef;

ULN2003_Status uln2003_Init(ULN2003_HandleTypeDef *hmotor);

ULN2003_Status uln2003_RunSteps(ULN2003_HandleTypeDef *hmotor, int32_t steps, ULN2003_DIRECTION direction);

void uln2003_Stop(ULN2003_HandleTypeDef *hmotor);

#endif
