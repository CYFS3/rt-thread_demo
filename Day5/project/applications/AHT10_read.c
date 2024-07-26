// #include <rtthread.h>
// #include <board.h>
// #include <drv_gpio.h>
// #include "aht10.h"


// //星火一号AHT10使用的为i2c3
// #define AHT10_I2C_BUS "i2c3"


// //定义线程的信息
// #define THREAD_PRIORITY 25      //线程优先级
// #define THREAD_STACK_SIZE 2048  //线程栈大小
// #define THREAD_TIMSLICE 5		//线程时间片



// //AHT线程指针
// rt_thread_t AHT10 = RT_NULL;

// void aht10_demo(void * paramenter)
// {
// 	aht10_device_t aht10_dev = RT_NULL;
// 	aht10_dev = aht10_init(AHT10_I2C_BUS);
// 	if(aht10_dev == RT_NULL)
// 	{
// 		rt_kprintf("aht10 init fail\n");
// 		return;
// 	}
// 	float humi = 0,temp = 0;
// 	//读取温湿度
// 	while(1)
// 	{
// 		temp = aht10_read_temperature(aht10_dev);
// 		humi = aht10_read_humidity(aht10_dev);

// 		rt_kprintf("aht10 temp %d.%d\n",(int)temp,(int)(temp*10)%10);
// 		rt_kprintf("aht10 humi %d.%d\n",(int)humi,(int)(humi*10)%10);
// 		rt_thread_mdelay(1000);
// 	}
// }


// void aht10_task(void)
// {
// 	AHT10 = rt_thread_create("aht10",aht10_demo,RT_NULL,THREAD_STACK_SIZE,THREAD_PRIORITY,THREAD_TIMSLICE);
// 	if(AHT10 == RT_NULL)
// 	{
// 		rt_kprintf("aht10 thread create fail\n");
// 		return;

// 	}
// 	rt_thread_startup(AHT10);
// 	return;
// }
// MSH_CMD_EXPORT(aht10_task,aht10 read thread);


