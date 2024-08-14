#include <rtthread.h>
#include <rtdevice.h>
#include "aht10.h"

#define DBG_TAG "aht10"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>
#include "aht21_read.h"
rt_mailbox_t aht10_mb;
static void aht10_entry(void *parameter)
{
    aht21_data aht21;
    aht10_device_t dev;
    const char *i2c_bus_name = "i2c3";
    int count = 0;
    rt_thread_mdelay(2000);
    dev = aht10_init(i2c_bus_name);
    if (dev == RT_NULL)
    {
        LOG_E("The sensor initializes failure");
    }
    LOG_I("AHT10 has been initialized!");
    aht10_mb = rt_mb_create("aht10_mb", 10, RT_IPC_FLAG_FIFO);
    while (1)
    {
        aht21.humidity = aht10_read_humidity(dev);
        aht21.temperature = aht10_read_temperature(dev);
        rt_mb_send(aht10_mb, (rt_ubase_t)&aht21);
        rt_thread_mdelay(1000);
    }
}

int aht10_thread_port(void)
{
    rt_thread_t res = rt_thread_create("aht10", aht10_entry, RT_NULL, 1024, 8, 50);
    if(res == RT_NULL)
    {
        LOG_E("aht10 thread create failed!");
        return -RT_ERROR;
    }

    rt_thread_startup(res);

    return RT_EOK;
}
INIT_DEVICE_EXPORT(aht10_thread_port);