#include <rtthread.h>
#include <rthw.h>
#include <rtdevice.h>

#include <string.h>
#include "ap3216c.h"
#include "ap3216_read.h"
#define DBG_TAG "ap3216"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>
#include "aht21_read.h"
rt_mutex_t ap3216_mut = RT_NULL;
ap3216_data ap3216 = {0};
 void ap3216c_read_entry(void *parameter)
 {
	static ap3216c_device_t dev = RT_NULL;
	strcmp(dev->i2c->parent.parent.name, "i2c2");          
	if (dev)
	{
		ap3216c_deinit(dev);
	}
	dev = ap3216c_init("i2c2");
	if (dev == RT_NULL)
	{
		LOG_E("ap3216c init failed\n");
		return;
	}
	LOG_I("ap3216c init success\n");
	ap3216_mut = rt_mutex_create("ap3216", RT_IPC_FLAG_PRIO);
	if (ap3216_mut == RT_NULL)
	{
		LOG_E("The mutex initializes failure");
	}
	
	while (1)
	{   
		rt_mutex_take(ap3216_mut, RT_WAITING_FOREVER);       
		ap3216.ps_data = ap3216c_read_ps_data(dev);
		ap3216.brightness = ap3216c_read_ambient_light(dev);
		rt_mutex_release(ap3216_mut);
		rt_thread_mdelay(1000);
	}      
 }

int ap3216c_read(void)
{
	rt_thread_t tid;
	tid = rt_thread_create("ap3216c_read", ap3216c_read_entry, RT_NULL, 768, 8, 10);
	if (tid != RT_NULL)
	{
		rt_thread_startup(tid);
	}
	return RT_EOK;
}
INIT_APP_EXPORT(ap3216c_read);
