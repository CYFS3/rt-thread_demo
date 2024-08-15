#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include <drv_gpio.h>
#define DRV_DEBUG
#define LOG_TAG    "drv.test"
#include <drv_log.h>
#define KEY_UP GET_PIN(C,5)
#define KEY_DOWN GET_PIN(C,6)
#define KEY_LEFT GET_PIN(C,7)
#define KEY_RIGHT GET_PIN(C,8)

void key_up_calback(void * args)
{
	int value = rt_pin_read(KEY_UP);
	LOG_I("key_up_calback value:%d",value);
	
}






