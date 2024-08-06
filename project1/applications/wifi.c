#include <rtthread.h>
#include <rtdevice.h>

int wifi_join_init(void)
{
	system("wifi scan");    
	system("wifi join CYFS 12345678");
	return RT_EOK;
}



