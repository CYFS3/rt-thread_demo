#include <rtthread.h>
#include <rtdevice.h>

// wifi join entry
void wifi_join_init(void * parameter)
{
	rt_thread_mdelay(1000);
	rt_kprintf("wifi join\n");
	system("wifi scan");    
	char arr[] = "wifi join CYFS 12345678";
	system(arr);

}
// wifi join thrad
int wifi_join(void)
{
	rt_thread_t tid;
	tid = rt_thread_create("wifi_join", wifi_join_init, RT_NULL, 2048, 10, 10);
	if (tid != RT_NULL)
	{
		rt_thread_startup(tid);
	}
	return RT_EOK;
}
INIT_APP_EXPORT(wifi_join);

