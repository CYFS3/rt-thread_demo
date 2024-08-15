#include <rtthread.h>
#include <rtdevice.h>


rt_sem_t wifi_sem = RT_NULL;
void wifi_event_handler(int event, struct rt_wlan_buff *buff, void *parameter)
{
	if (event == RT_WLAN_EVT_READY)
	{
		system("ntp_sync");
		rt_sem_release(wifi_sem);
	}
}
// wifi join entry
void wifi_join_init(void * parameter)
{
	wifi_sem = rt_sem_create("wifi_join", 1, RT_IPC_FLAG_FIFO);
	rt_sem_take (wifi_sem, RT_WAITING_FOREVER);
	rt_thread_delay(2000);
	rt_wlan_register_event_handler(RT_WLAN_EVT_READY, wifi_event_handler, NULL);
	rt_kprintf("wifi join\n");
	system("wifi scan");    
	char arr[] = "wifi join CYFS 12345678";
	system(arr);
}
int wifi_join(void)
{
	rt_thread_t tid;
	tid = rt_thread_create("wifi_join", wifi_join_init, RT_NULL, 2048, 5, 10);
	if (tid != RT_NULL)
	{
		rt_thread_startup(tid);
		return RT_EOK;
	}
	return -RT_ERROR;
}
INIT_APP_EXPORT(wifi_join);

