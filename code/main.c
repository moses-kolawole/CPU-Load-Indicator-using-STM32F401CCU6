#include "main.h"

/*==========================
        VARIABLES
==========================*/
uint32_t total_busy_time = 0;
uint32_t total_idle_time = 0;

uint32_t cpu_utilization = 0;

uint32_t task_start_time = 0;
uint32_t task_end_time = 0;
uint32_t busy_time = 0;

uint32_t idle_start_time = 0;
uint32_t idle_end_time = 0;
uint32_t idle_time = 0;

int main(void)
{
    /*==========================
        ENABLE GPIO CLOCK
    ==========================*/
    RCC->AHB1ENR |= (1 << 0);

    /*==========================
        PA6 OUTPUT
    ==========================*/
    GPIOA->MODER &= ~(3 << (6 * 2));
    GPIOA->MODER |=  (1 << (6 * 2));

    /*==========================
        ENABLE TIM2 CLOCK
    ==========================*/
    RCC->APB1ENR |= (1 << 0);

    /*==========================
        TIMER CONFIGURATION
    ==========================*/
    TIM2->PSC = 15;
    TIM2->ARR = 0xFFFFFFFF;

    /*==========================
        START TIMER
    ==========================*/
    TIM2->CR1 |= (1 << 0);

    while(1)
    {
        /*==========================
            MEASURE IDLE START
        ==========================*/
        idle_start_time = TIM2->CNT;

        /*==========================
            CPU WAITING
        ==========================*/
        while((TIM2->CNT - idle_start_time) < 100000)
        {
            /* Processor is waiting */
        }

        /*==========================
            MEASURE IDLE END
        ==========================*/
        idle_end_time = TIM2->CNT;

        /*==========================
            CALCULATE IDLE TIME
        ==========================*/
        idle_time = idle_end_time - idle_start_time;
        total_idle_time += idle_time;

        /*==========================
            RECORD TASK START
        ==========================*/
        task_start_time = TIM2->CNT;

        /*==========================
            EXECUTE TASK
        ==========================*/
        for(volatile uint32_t i = 0; i < 200000; i++)
        {
            /* Simulated Processing */
        }

        /*==========================
            RECORD TASK END
        ==========================*/
        task_end_time = TIM2->CNT;

        /*==========================
            CALCULATE BUSY TIME
        ==========================*/
        busy_time = task_end_time - task_start_time;
        total_busy_time += busy_time;

        /*==========================
            CALCULATE CPU UTILIZATION
        ==========================*/
        if((total_busy_time + total_idle_time) != 0)
        {
            cpu_utilization =
            (total_busy_time * 100) /
            (total_busy_time + total_idle_time);
        }

        /*==========================
            CPU LOAD INDICATOR
        ==========================*/
        if(cpu_utilization >= 50)
        {
            GPIOA->ODR |= (1 << 6);
        }
        else
        {
            GPIOA->ODR &= ~(1 << 6);
        }
    }
}
