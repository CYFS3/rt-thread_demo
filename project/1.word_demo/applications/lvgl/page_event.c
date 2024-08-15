#include "gui_guider.h"
#include "events_init.h"
#include <time.h>
#include <rtthread.h>
#include <page_event.h>
#include <dfs_posix.h>
#include <stdio.h>
#include <rtthread.h>
#include "aht21_read.h"
#include "ap3216_read.h"
#define DBG_ENABLE
#define DBG_SECTION_NAME "page_event"
#define DBG_LEVEL DBG_LOG
#define DBG_COLOR
#include <rtdbg.h>
extern lv_ui guider_ui;
static Clock_Value_t Clock;
rt_thread_t data_thread;
rt_thread_t word_thread;
static void main_page_entry(void * parameter)
{
	struct tm *Time;
	time_t now;
	while (ap3216_mut == RT_NULL || ant10_mut == RT_NULL)
	{
		rt_thread_mdelay(500);
	}
	while (1)
	{
		now = time(RT_NULL) + 28800;
		Time = gmtime(&now);
		Clock.year = Time->tm_year+1900;
		Clock.month = Time->tm_mon+1;
		Clock.date = Time->tm_mday;
		Clock.week = Time->tm_wday;
		Clock.hour = Time->tm_hour;
		Clock.min = Time->tm_min;
		Clock.sec = Time->tm_sec;
		lv_label_set_text_fmt(guider_ui.screen_label_1, "%d:%d:%d",Clock.hour, Clock.min, Clock.sec);
		lv_label_set_text_fmt(guider_ui.screen_label_5, "%d-%d-%d",Clock.year, Clock.month, Clock.date);
		rt_mutex_take(ap3216_mut, RT_WAITING_FOREVER);
       	lv_label_set_text_fmt(guider_ui.screen_label_4,"%.2f",ap3216.brightness);
        rt_mutex_release(ap3216_mut);
		rt_mutex_take(ant10_mut, RT_WAITING_FOREVER);
		lv_label_set_text_fmt(guider_ui.screen_label_2,"%.2f",aht21.temperature);
		lv_label_set_text_fmt(guider_ui.screen_label_3, "%.2f",aht21.humidity);
        rt_mutex_release(ant10_mut);
		rt_thread_mdelay(1000);
	}
	
}


static void word_page_entry(void * parameter)
{
	struct tm *Time;
	time_t now;
	while (1)
	{
		now = time(RT_NULL) + 28800;
		Time = gmtime(&now);
		Clock.hour = Time->tm_hour;
		Clock.min = Time->tm_min;
		Clock.sec = Time->tm_sec;
		lv_label_set_text_fmt(guider_ui.screen_1_label_1, "%d:%d:%d",Clock.hour, Clock.min, Clock.sec);
		rt_thread_mdelay(1000);		
	}
	
}
void word_thread_create(void)
{
	word_thread = rt_thread_create("word_thread", word_page_entry, RT_NULL, 1536, 10, 5);
	if(word_thread != RT_NULL)
	{
		rt_thread_startup(word_thread);
	}
}
void word_thread_delete(void)
{
	if(word_thread != RT_NULL)
	{
		rt_thread_delete(word_thread);
		word_thread = RT_NULL;
	}
}

void date_thread_crearte(void)
{
	data_thread =  rt_thread_create("date_thread", main_page_entry, RT_NULL, 1536, 10, 5);
	if(data_thread != RT_NULL)
	{
		rt_thread_startup(data_thread);
	}
}

void data_thread_delete(void)
{
	if(data_thread != RT_NULL)
	{
		rt_thread_delete(data_thread);
		data_thread = RT_NULL;
	}
}


int read_page_start(void)
{
	
	int fd = open("/sdcard/sys.txt",O_RDWR);
	if(fd == -1)
	{
		fd = open("/sdcard/sys.txt",O_CREAT|O_RDWR);
		if(fd == -1)
		{
			LOG_E("open failed");
			return  -1;
		}
		write(fd,"0",1);
	}
	char buf[10];
	int count = read(fd,buf,10);
	if(count == -1)
	{
		LOG_E("read failed");
		return -1;
	}
	close(fd);
	fd = -1;
	sscanf(buf,"%d",&count);
	LOG_I("read count:%d",count);
	return count;
}
void read_word(int count,char * buf)
{
	
	int fd = open("/sdcard/3.CET4.txt",O_RDWR);
	if(fd == -1)
	{
		LOG_E("open failed");
		return;
	}
	for(int i = 0; i <= count; i++)
	{
		int size = 0;
		while (1)
		{
			read(fd,&buf[size],1);
			if(buf[size] == '\n')
			{
				buf[size] = '\0';
				break;
			}
			else
			{
				size++;
			}
		}
	}
	LOG_I("read :%s",buf);
	close(fd);
}
void read_page_end(int count)
{
	int fd = open("/sdcard/sys.txt",O_TRUNC|O_WRONLY);
	char buf[10];
	if(fd == -1)
	{
		LOG_E("open failed");
		return;
	}
	sprintf(buf,"%d",count);
	int ret = write(fd,buf,strlen(buf));
	if(ret == -1)
	{
		LOG_E("write failed");
	}
	close(fd);
}



