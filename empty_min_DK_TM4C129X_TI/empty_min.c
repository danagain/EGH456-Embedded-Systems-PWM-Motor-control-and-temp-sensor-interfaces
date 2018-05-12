/*
 * Copyright (c) 2015, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ======== empty_min.c ========
 */
/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/Error.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/hal/Hwi.h>

/* TI-RTOS Header files */
// #include <ti/drivers/EMAC.h>
#include <ti/drivers/GPIO.h>
#include <ti/drivers/PWM.h>
// #include <ti/drivers/I2C.h>
// #include <ti/drivers/SDSPI.h>
// #include <ti/drivers/SPI.h>
// #include <ti/drivers/UART.h>
// #include <ti/drivers/USBMSCHFatFs.h>
// #include <ti/drivers/Watchdog.h>
// #include <ti/drivers/WiFi.h>

#include "inc/hw_ints.h"
#include "inc/hw_timer.h"
#include "inc/hw_gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.c"
#include "driverlib/sysctl.h"
#include "driverlib/Timer.c"
#include "driverlib/Timer.h"
#include "driverlib/gpio.c"
#include "driverlib/gpio.h"

/* Board Header file */
#include "Board.h"

/* Standard C Lib */
#include <stdbool.h>

#define TASKSTACKSIZE   512

Task_Struct task0Struct;
Char task0Stack[TASKSTACKSIZE];

PWM_Handle pwm1;
PWM_Params params;


uint16_t   freq = 40000;     //PWM Frequency in Hz
uint16_t period;
double   duty = 0.5;

// Motor Variables
double error = 0;
double errorSum = 0;
int speed = 0;
int setSpeed = 0;
bool clockwise = true;

int Sw = 1;

// Motor Control Variables:
// Tweak parameters for better response
double kp = 0;
double ki = 0;


/*
 *  ======== heartBeatFxn ========
 *  Toggle the Board_LED0. The Task_sleep is determined by arg0 which
 *  is configured for the heartBeat Task instance.
 */
Void heartBeatFxn(UArg arg0, UArg arg1)
{
    while (1) {
        // Motor control runs at 10Hz
        Task_sleep((unsigned int)arg0);

//        error = speed - setSpeed;
//        errorSum += error;
//
//        duty = pwmPeriod * (kp * error + ki * errorSum);

        //duty += 0.001;
    }
}

void gpioButtonFxn0(unsigned int index)
{
    /* Clear the GPIO interrupt and toggle an LED */
duty+=0.1;
}

void gpioButtonFxn1(unsigned int index)
{
    /* Clear the GPIO interrupt and toggle an LED */
    duty-=0.1;
}

/*
 * Hall Effect Hwi Function
 * Change so these are running from one
 */
//void HallEffectCallback(unsigned int index) {
//    // Start timer
//    // Time from last timer into buffer
//    // Average to find motor speed
//
//
////    GPIO_toggle(Board_LED0);
//
//    if(clockwise) {
//        if(GPIO_read(Hall_Effect_1) && !GPIO_read(Hall_Effect_2) && GPIO_read(Hall_Effect_3)) {
//            GPIO_write(Motor_Reset_A, 1);
//            GPIO_write(Motor_Reset_B, 1);
//            GPIO_write(Motor_Reset_C, 0);
//
//            TimerControlLevel(TIMER3_BASE, TIMER_A, 0);
//            TimerControlLevel(TIMER2_BASE, TIMER_B, 1);
//            TimerControlLevel(TIMER2_BASE, TIMER_A, 0);
//
//            TimerMatchSet(TIMER3_BASE, TIMER_A, (1 - duty) * (period -2));
//            TimerMatchSet(TIMER2_BASE, TIMER_B, (1 - duty) * (period -2));
//            TimerMatchSet(TIMER2_BASE, TIMER_A, period -2);
//
//        } else if (GPIO_read(Hall_Effect_1) && !GPIO_read(Hall_Effect_2) && !GPIO_read(Hall_Effect_3)) {
//            GPIO_write(Motor_Reset_A, 1);
//            GPIO_write(Motor_Reset_B, 0);
//            GPIO_write(Motor_Reset_C, 1);
//
//            TimerControlLevel(TIMER3_BASE, TIMER_A, 0);
//            TimerControlLevel(TIMER2_BASE, TIMER_B, 0);
//            TimerControlLevel(TIMER2_BASE, TIMER_A, 1);
//
//            TimerMatchSet(TIMER3_BASE, TIMER_A, (1 - duty) * (period -2));
//            TimerMatchSet(TIMER2_BASE, TIMER_B, period -2);
//            TimerMatchSet(TIMER2_BASE, TIMER_A, (1 - duty) * (period -2));
//
//
//        } else if (GPIO_read(Hall_Effect_1) && GPIO_read(Hall_Effect_2) && !GPIO_read(Hall_Effect_3)) {
//            GPIO_write(Motor_Reset_A, 0);
//            GPIO_write(Motor_Reset_B, 1);
//            GPIO_write(Motor_Reset_C, 1);
//
//            TimerControlLevel(TIMER3_BASE, TIMER_A, 0);
//            TimerControlLevel(TIMER2_BASE, TIMER_B, 0);
//            TimerControlLevel(TIMER2_BASE, TIMER_A, 1);
//
//            TimerMatchSet(TIMER3_BASE, TIMER_A, period -2);
//            TimerMatchSet(TIMER2_BASE, TIMER_B, (1 - duty) * (period -2));
//            TimerMatchSet(TIMER2_BASE, TIMER_A, (1 - duty) * (period -2));
//
//
//
//        } else if (!GPIO_read(Hall_Effect_1) && GPIO_read(Hall_Effect_2) && !GPIO_read(Hall_Effect_3)) {
//            GPIO_write(Motor_Reset_A, 1);
//            GPIO_write(Motor_Reset_B, 1);
//            GPIO_write(Motor_Reset_C, 0);
//
//            TimerControlLevel(TIMER3_BASE, TIMER_A, 1);
//            TimerControlLevel(TIMER2_BASE, TIMER_B, 0);
//            TimerControlLevel(TIMER2_BASE, TIMER_A, 0);
//
//            TimerMatchSet(TIMER3_BASE, TIMER_A, (1 - duty) * (period -2));
//            TimerMatchSet(TIMER2_BASE, TIMER_B, (1 - duty) * (period -2));
//            TimerMatchSet(TIMER2_BASE, TIMER_A, period -2);
//
//        } else if (!GPIO_read(Hall_Effect_1) && GPIO_read(Hall_Effect_2) && GPIO_read(Hall_Effect_3)) {
//            GPIO_write(Motor_Reset_A, 1);
//            GPIO_write(Motor_Reset_B, 0);
//            GPIO_write(Motor_Reset_C, 1);
//
//            TimerControlLevel(TIMER3_BASE, TIMER_A, 1);
//            TimerControlLevel(TIMER2_BASE, TIMER_B, 0);
//            TimerControlLevel(TIMER2_BASE, TIMER_A, 0);
//
//            TimerMatchSet(TIMER3_BASE, TIMER_A, (1 - duty) * (period -2));
//            TimerMatchSet(TIMER2_BASE, TIMER_B, period -2);
//            TimerMatchSet(TIMER2_BASE, TIMER_A, (1 - duty) * (period -2));
//
//        } else if (!GPIO_read(Hall_Effect_1) && !GPIO_read(Hall_Effect_2) && GPIO_read(Hall_Effect_3)) {
//            GPIO_write(Motor_Reset_A, 0);
//            GPIO_write(Motor_Reset_B, 1);
//            GPIO_write(Motor_Reset_C, 1);
//
//            TimerControlLevel(TIMER3_BASE, TIMER_A, 0);
//            TimerControlLevel(TIMER2_BASE, TIMER_B, 1);
//            TimerControlLevel(TIMER2_BASE, TIMER_A, 0);
//
//            TimerMatchSet(TIMER3_BASE, TIMER_A, period -2);
//            TimerMatchSet(TIMER2_BASE, TIMER_B, (1 - duty) * (period -2));
//            TimerMatchSet(TIMER2_BASE, TIMER_A, (1 - duty) * (period -2));
//        }
//
//    } else {
//
//    }
//
//   //start monoshot timer,
//    //speed = time between timers2
//    //    TimerMatchSet(TIMER3_BASE, TIMER_A, period -1);
//    //    TimerMatchSet(TIMER2_BASE, TIMER_B, period -2);
//    //    TimerMatchSet(TIMER2_BASE, TIMER_A, duty * (period -1));
//        //    TimerMatchSet(TIMER3_BASE, TIMER_A, period -2);
//        //    TimerMatchSet(TIMER2_BASE, TIMER_B, duty * (period -1));
//        //    TimerMatchSet(TIMER2_BASE, TIMER_A, period -2);
//}

void HallEffectCallback(unsigned int index) {
    if(clockwise) {


        switch(Sw){
        case 1:
            GPIO_write(Motor_Reset_A, 1);
                       GPIO_write(Motor_Reset_B, 1);
                       GPIO_write(Motor_Reset_C, 0);

                       TimerControlLevel(TIMER3_BASE, TIMER_A, 0);
                       TimerControlLevel(TIMER2_BASE, TIMER_B, 1);
                       TimerControlLevel(TIMER2_BASE, TIMER_A, 0);

                       TimerMatchSet(TIMER3_BASE, TIMER_A, (1 - duty) * (period -2));
                       TimerMatchSet(TIMER2_BASE, TIMER_B, (1 - duty) * (period -2));
                       TimerMatchSet(TIMER2_BASE, TIMER_A, period -2);
                       break;

        case 2:
            GPIO_write(Motor_Reset_A, 1);
                        GPIO_write(Motor_Reset_B, 0);
                        GPIO_write(Motor_Reset_C, 1);

                        TimerControlLevel(TIMER3_BASE, TIMER_A, 0);
                        TimerControlLevel(TIMER2_BASE, TIMER_B, 0);
                        TimerControlLevel(TIMER2_BASE, TIMER_A, 1);

                        TimerMatchSet(TIMER3_BASE, TIMER_A, (1 - duty) * (period -2));
                        TimerMatchSet(TIMER2_BASE, TIMER_B, period -2);
                        TimerMatchSet(TIMER2_BASE, TIMER_A, (1 - duty) * (period -2));
                        break;
        case 3:
            GPIO_write(Motor_Reset_A, 0);
                      GPIO_write(Motor_Reset_B, 1);
                      GPIO_write(Motor_Reset_C, 1);

                      TimerControlLevel(TIMER3_BASE, TIMER_A, 0);
                      TimerControlLevel(TIMER2_BASE, TIMER_B, 0);
                      TimerControlLevel(TIMER2_BASE, TIMER_A, 1);

                      TimerMatchSet(TIMER3_BASE, TIMER_A, period -2);
                      TimerMatchSet(TIMER2_BASE, TIMER_B, (1 - duty) * (period -2));
                      TimerMatchSet(TIMER2_BASE, TIMER_A, (1 - duty) * (period -2));
                      break;
        case 4:
            GPIO_write(Motor_Reset_A, 1);
                       GPIO_write(Motor_Reset_B, 1);
                       GPIO_write(Motor_Reset_C, 0);

                       TimerControlLevel(TIMER3_BASE, TIMER_A, 1);
                       TimerControlLevel(TIMER2_BASE, TIMER_B, 0);
                       TimerControlLevel(TIMER2_BASE, TIMER_A, 0);

                       TimerMatchSet(TIMER3_BASE, TIMER_A, (1 - duty) * (period -2));
                       TimerMatchSet(TIMER2_BASE, TIMER_B, (1 - duty) * (period -2));
                       TimerMatchSet(TIMER2_BASE, TIMER_A, period -2);
        case 5:
            GPIO_write(Motor_Reset_A, 1);
                        GPIO_write(Motor_Reset_B, 0);
                        GPIO_write(Motor_Reset_C, 1);

                        TimerControlLevel(TIMER3_BASE, TIMER_A, 1);
                        TimerControlLevel(TIMER2_BASE, TIMER_B, 0);
                        TimerControlLevel(TIMER2_BASE, TIMER_A, 0);

                        TimerMatchSet(TIMER3_BASE, TIMER_A, (1 - duty) * (period -2));
                        TimerMatchSet(TIMER2_BASE, TIMER_B, period -2);
                        TimerMatchSet(TIMER2_BASE, TIMER_A, (1 - duty) * (period -2));
                        break;
        case 6:
            GPIO_write(Motor_Reset_A, 0);
                      GPIO_write(Motor_Reset_B, 1);
                      GPIO_write(Motor_Reset_C, 1);

                      TimerControlLevel(TIMER3_BASE, TIMER_A, 0);
                      TimerControlLevel(TIMER2_BASE, TIMER_B, 1);
                      TimerControlLevel(TIMER2_BASE, TIMER_A, 0);

                      TimerMatchSet(TIMER3_BASE, TIMER_A, period -2);
                      TimerMatchSet(TIMER2_BASE, TIMER_B, (1 - duty) * (period -2));
                      TimerMatchSet(TIMER2_BASE, TIMER_A, (1 - duty) * (period -2));
                      break;
        }
        Sw++;
        if (Sw >= 7) {
            Sw = 1;
        }
    }
}




/*
 *  ======== main ========
 */
int main(void) {
    Task_Params taskParams;

    /* Call board init functions */
    Board_initGeneral();
    // Board_initEMAC();
    Board_initGPIO();
    //Board_initPWM();
    // Board_initI2C();
    // Board_initSDSPI();
    // Board_initSPI();
    // Board_initUART();
    // Board_initUSB(Board_USBDEVICE);
    // Board_initUSBMSCHFatFs();
    // Board_initWatchdog();
    // Board_initWiFi();

    period = SysCtlClockGet()/freq;

     SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOM);
     SysCtlDelay(3);
     GPIOPinConfigure(GPIO_PM2_T3CCP0);
     GPIOPinConfigure(GPIO_PM1_T2CCP1);
     GPIOPinConfigure(GPIO_PM0_T2CCP0);
     GPIOPinTypeTimer(GPIO_PORTM_BASE, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2);

/*
     Configure timer 2 to split pair and timer A and B in PWM mode
     Set period and starting duty cycle.
   */

   SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);
   SysCtlDelay(3);
   TimerConfigure(TIMER2_BASE, TIMER_CFG_SPLIT_PAIR|TIMER_CFG_A_PWM|TIMER_CFG_B_PWM);
   TimerLoadSet(TIMER2_BASE, TIMER_A, period -1);
   TimerLoadSet(TIMER2_BASE, TIMER_B, period -1);
   TimerMatchSet(TIMER2_BASE, TIMER_A, period -2);
   TimerMatchSet(TIMER2_BASE, TIMER_B, period -2);

   SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER3);
   SysCtlDelay(3);
   TimerConfigure(TIMER3_BASE, TIMER_CFG_SPLIT_PAIR|TIMER_CFG_A_PWM);
   TimerLoadSet(TIMER3_BASE, TIMER_A, period -1);
   TimerMatchSet(TIMER3_BASE, TIMER_A, period -2);

   TimerEnable(TIMER3_BASE, TIMER_A);
   TimerEnable(TIMER2_BASE, TIMER_A|TIMER_B);

   /* install Button callback */
   GPIO_setCallback(Hall_Effect_1, HallEffectCallback);
   GPIO_setCallback(Hall_Effect_2, HallEffectCallback);
   GPIO_setCallback(Hall_Effect_3, HallEffectCallback);

   /* Enable interrupts */
   GPIO_enableInt(Hall_Effect_1);
   GPIO_enableInt(Hall_Effect_2);
   GPIO_enableInt(Hall_Effect_3);



   GPIO_setCallback(Board_BUTTON0, gpioButtonFxn0);
   GPIO_setCallback(Board_BUTTON1, gpioButtonFxn1);

   GPIO_enableInt(Board_BUTTON0);
   GPIO_enableInt(Board_BUTTON1);

    /* Construct heartBeat Task  thread */
    Task_Params_init(&taskParams);
    taskParams.arg0 = 100;
    taskParams.stackSize = TASKSTACKSIZE;
    taskParams.stack = &task0Stack;
    Task_construct(&task0Struct, (Task_FuncPtr)heartBeatFxn, &taskParams, NULL);

    /* Turn off user LED  */
    GPIO_write(Board_LED0, Board_LED_OFF);

    /* Start BIOS */
    BIOS_start();

    return (0);
}
