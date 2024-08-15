/*
 * Copyright (c) 2023, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-07-06     Supperthomas first version
 * 2023-12-03     Meco Man     support nano version
 */

#include <board.h>
#include <rtthread.h>
#include <drv_gpio.h>
#include <drv_lcd.h>
#ifndef RT_USING_NANO
#include <rtdevice.h>
#endif /* RT_USING_NANO */
#include "fal.h"
#define GPIO_LED_B    GET_PIN(F, 11)
#define GPIO_LED_R    GET_PIN(F, 12)



int main(void)
{
       rt_pin_mode(GPIO_LED_R, PIN_MODE_OUTPUT);
    rt_device_t rtc = rt_device_find("rtc");
    if (rtc == RT_NULL)
    {
        rt_kprintf("rtc not found\n");
    }
    else
    {
        rt_kprintf("rtc found\n");
        rt_device_open(rtc, 0);
    } 

   
}
