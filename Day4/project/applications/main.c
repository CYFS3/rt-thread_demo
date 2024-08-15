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
#include <drv_spi.h>
static char thread1_addr[512];
static struct rt_thread thread1;
void thread1_entry(void * paramenter)
{
    while(1)
    {
        rt_kprintf("this is thread1\r\n");
        rt_thread_mdelay(500);
    }    
}
void thread2_entry(void * paramenter)
{
    while(1)
    {
        rt_kprintf("this is thread2\r\n");
        rt_thread_mdelay(500);
    }    
}
int main(void)
{
    
   
wifi_spi_device_init();
    rt_thread_t thread2;
    
    while (1)
    {
          rt_thread_mdelay(500);

    }
}
