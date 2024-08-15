#include <rtthread.h>
#include "events_init.h"
#include "lvgl.h"
#include <drv_gpio.h>
#include <drv_matrix_led.h>
#define PIN_BEEP        GET_PIN(B, 0)  
void pop_up_entry(char * buf)
{
	int flag = 0;
	rt_pin_write(PIN_BEEP,PIN_HIGH);
	if(lv_scr_act() == guider_ui.screen) 
	{
		flag = 0;
		ui_load_scr_animation(&guider_ui, &guider_ui.screen_2, guider_ui.screen_2_del, &guider_ui.screen_del, setup_scr_screen_2, LV_SCR_LOAD_ANIM_NONE, 0, 0, false, false);
	}
	else
	{
		flag = 1;
		ui_load_scr_animation(&guider_ui, &guider_ui.screen_2, guider_ui.screen_2_del, &guider_ui.screen_2_del, setup_scr_screen_2, LV_SCR_LOAD_ANIM_NONE, 0, 0, false, false);
	}
	lv_label_set_text(guider_ui.screen_2_label_1, buf);
	rt_thread_mdelay(3000);
	
	if(flag == 0)
	{
		ui_load_scr_animation(&guider_ui, &guider_ui.screen, guider_ui.screen_del, &guider_ui.screen_2_del, setup_scr_screen, LV_SCR_LOAD_ANIM_NONE, 0, 0, false, false);
	}
	else
	{
		ui_load_scr_animation(&guider_ui, &guider_ui.screen_1, guider_ui.screen_1_del, &guider_ui.screen_2_del, setup_scr_screen, LV_SCR_LOAD_ANIM_NONE, 0, 0, false, false);
	}
	rt_pin_write(PIN_BEEP,PIN_LOW);
	
}

rt_thread_t led_matrix_thread;

static void led_matrix_example_entry(void * parameter)
{
	int i = 0;
	while (1)
	{
		led_matrix_fill_test(i);
		i++;
		if (i == 4)
		{
			i = 0;
		}
		rt_thread_mdelay(1000);
	}
}
void led_matrix_thread_create(void)
{
	led_matrix_thread = rt_thread_create("led_matrix",
		led_matrix_example_entry, RT_NULL,
		512, 10, 10);
	if (led_matrix_thread != RT_NULL)
	{
		rt_thread_startup(led_matrix_thread);
	}
}
void led_matrix_thread_exit(void)
{
	if (led_matrix_thread != RT_NULL)
	{
		led_matrix_rst();
		rt_thread_delete(led_matrix_thread);
		led_matrix_thread = RT_NULL;
	}
}