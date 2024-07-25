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

#define LOG_TAG              "demo"
#define LOG_LVL              LOG_LVL_DBG
#include <ulog.h>
rt_mutex_t mu;
rt_thread_t thread1;
void thread2_entry(void * paramenter)
{
    while(1)
    {
        rt_thread_mdelay(100);
       
    }    
}
void thread3_entry(void * paramenter)
{
    while(1)
    {
        rt_mutex_take(mu,RT_WAITING_FOREVER);
        rt_thread_mdelay(500);
    }    
}

void thread1_entry(void * paramenter)
{
     mu = rt_mutex_create("demo",RT_IPC_FLAG_PRIO);
    if(mu == RT_NULL)
    {
        LOG_E("mu create fail!\n");
        return ;
    }
    rt_mutex_take(mu,0);
     rt_thread_t thread2 = RT_NULL,thread3 = RT_NULL;
    
    thread2 = rt_thread_create("thread2",thread2_entry,RT_NULL,1024,15,5);
    if(thread2 != RT_NULL)
    {
        rt_thread_startup(thread2);
    }
    thread3 = rt_thread_create("thread3",thread3_entry,RT_NULL,1024,10,5);
    if(thread3 != RT_NULL)
    {
        rt_thread_startup(thread3);
    }
    while(1)
    {
        
        rt_thread_mdelay(500);
        LOG_I("prio increate\n");
    }    
}

int main(void)
{
    
    thread1 = rt_thread_create("thread1",thread1_entry,RT_NULL,1024,18,5);
    if(thread1 != RT_NULL)
    {
        rt_thread_startup(thread1);
    }
    return 0;
   
}

