#include <rtthread.h>
#include <rthw.h>
#include <rtdevice.h>

#include <string.h>
#include "ap3216c.h"
#include "ap3216_read.h"
rt_mailbox_t ap3216_mb;
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
	ap3216_data ap3216;
	ap3216_mb = rt_mb_create("ap3216_mb", 10, RT_IPC_FLAG_FIFO);
	while (1)
	{          
		ap3216.ps_data = ap3216c_read_ps_data(dev);
		ap3216.brightness = ap3216c_read_ambient_light(dev);
		rt_mb_send(ap3216_mb,(rt_ubase_t)&ap3216);
		rt_thread_mdelay(1000);
	}      
 }

int ap3216c_read(void)
{
	rt_thread_t tid;
	tid = rt_thread_create("ap3216c_read", ap3216c_read_entry, RT_NULL, 1024, 8, 10);
	if (tid != RT_NULL)
	{
		rt_thread_startup(tid);
	}
	return RT_EOK;
}
INIT_APP_EXPORT(ap3216c_read);
