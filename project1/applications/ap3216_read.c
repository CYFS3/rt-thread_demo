#include <rtthread.h>
#include <rthw.h>
#include <rtdevice.h>
#include <finsh.h>

#include <string.h>
#include "ap3216c.h"




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
		rt_kprintf("ap3216c init failed\n");
		return;
	}
	rt_kprintf("ap3216c init success\n");
	rt_uint16_t ps_data;
	float brightness;
	while (1)
	{          
		ps_data = ap3216c_read_ps_data(dev);
		if (ps_data == 0)
		{
			rt_kprintf("object is not proximity of sensor \n");
		}
		else
		{
			rt_kprintf("ap3216c read current ps data      : %d\n", ps_data);
		}

		brightness = ap3216c_read_ambient_light(dev);
		rt_kprintf("ap3216c measure current brightness: %d.%d(lux) \n", (int)brightness, ((int)(10 * brightness) % 10));
		rt_thread_mdelay(1000);
	}      
 }

int ap3216c_read(void)
{
	rt_thread_t tid;
	tid = rt_thread_create("ap3216c_read", ap3216c_read_entry, RT_NULL, 2048, 10, 10);
	if (tid != RT_NULL)
	{
		rt_thread_startup(tid);
	}
	return RT_EOK;
}
//INIT_APP_EXPORT(ap3216c_read);
