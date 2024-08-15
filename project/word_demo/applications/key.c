#include <rtthread.h>
#include <drv_gpio.h>
#include <stdbool.h>
#define KEY_LEFT GET_PIN(C,0)
#define KEY_DOWN GET_PIN(C,1)
#define KEY_RIGHT GET_PIN(C,4)
#define KEY_UP GET_PIN(C,5)


void key_init(void)
{
	rt_pin_mode(KEY_LEFT, PIN_MODE_INPUT_PULLUP );
	rt_pin_mode(KEY_DOWN, PIN_MODE_INPUT_PULLUP );
	rt_pin_mode(KEY_RIGHT, PIN_MODE_INPUT_PULLUP );
	rt_pin_mode(KEY_UP, PIN_MODE_INPUT_PULLUP );
}

bool key_is_pressed(uint8_t key)
{
	rt_thread_mdelay(50);
	if(key == 0)
	{
		
		return (rt_pin_read(KEY_LEFT) == PIN_LOW);
	}
	else if(key == 1)
	{
		return (rt_pin_read(KEY_RIGHT) == PIN_LOW);
	}
	else if(key == 2)
	{
		return (rt_pin_read(KEY_UP) == PIN_LOW);
	}
	else if(key == 3)
	{
		return (rt_pin_read(KEY_DOWN) == PIN_LOW);
	}
	
}


