#include "uln2003.h"

// 8 adımlı half-step dizisi
static const uint8_t step_seq[8][4] = {
    {1,0,0,0},
    {1,1,0,0},
    {0,1,0,0},
    {0,1,1,0},
    {0,0,1,0},
    {0,0,1,1},
    {0,0,0,1},
    {1,0,0,1}
};

static void uln2003_SetStep(ULN2003_HandleTypeDef *hmotor, uint8_t step)
{
    for(int i=0; i<4; i++)
    {
        HAL_GPIO_WritePin(hmotor->ports[i], hmotor->pins[i], (step_seq[step][i]) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    }
}

ULN2003_Status uln2003_Init(ULN2003_HandleTypeDef *hmotor)
{
    if(hmotor == NULL) return ULN2003_ERROR;

    hmotor->ports[0] = hmotor->IN1.port;
    hmotor->ports[1] = hmotor->IN2.port;
    hmotor->ports[2] = hmotor->IN3.port;
    hmotor->ports[3] = hmotor->IN4.port;

    hmotor->pins[0] = hmotor->IN1.pin;
    hmotor->pins[1] = hmotor->IN2.pin;
    hmotor->pins[2] = hmotor->IN3.pin;
    hmotor->pins[3] = hmotor->IN4.pin;

    if(hmotor->step_delay_ms==0) hmotor->step_delay_ms = 2;
    hmotor->current_step = 0;

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

    for (int i = 0; i < 4; i++) {
        GPIO_InitStruct.Pin = hmotor->pins[i];
        HAL_GPIO_Init(hmotor->ports[i], &GPIO_InitStruct);
    }

    return ULN2003_OK;
}

ULN2003_Status uln2003_RunSteps(ULN2003_HandleTypeDef *hmotor, int32_t steps, ULN2003_DIRECTION direction)
{
    if(hmotor == NULL || steps <= 0) return ULN2003_ERROR;

    int8_t dir = (direction == ULN2003_DIRECTION_FORWARD) ? 1 : -1;

    for(int32_t i=0; i<steps; i++)
    {
    	uln2003_SetStep(hmotor, hmotor->current_step);

        HAL_Delay(hmotor->step_delay_ms);

        int16_t next_step = hmotor->current_step + dir;
        if(next_step > 7) next_step = 0;
        else if(next_step < 0) next_step = 7;

        hmotor->current_step = next_step;
    }

    return ULN2003_OK;
}

void uln2003_Stop(ULN2003_HandleTypeDef *hmotor)
{
    if(hmotor == NULL) return;

    for(int i=0; i<4; i++)
    {
        HAL_GPIO_WritePin(hmotor->ports[i], hmotor->pins[i], GPIO_PIN_RESET);
    }
}
