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
#include "hello.h"
static char thread1_addr[512];
static struct rt_thread thread1;
void thread1_entry(void * paramenter)
{
    while(1)
    {
        rt_kprintf("this is thread1\n");
        rt_thread_mdelay(500);
    }    
}
void thread2_entry(void * paramenter)
{
    while(1)
    {
        rt_kprintf("this is thread2\n");
        print_hello();
        rt_thread_mdelay(500);
    }    
}
int main(void)
{
    rt_err_t ret= rt_thread_init(&thread1,"thread1",thread1_entry,RT_NULL,thread1_addr,256,10,10);
    if(ret == RT_EOK)
    {
        rt_thread_startup(&thread1);   
    }
    rt_thread_t thread2;
    thread2 = rt_thread_create("thread2",thread2_entry,RT_NULL,512,10,10);
    if(thread2 != RT_NULL)
    {
        rt_thread_startup(thread2);
    }
    return 0;
}
