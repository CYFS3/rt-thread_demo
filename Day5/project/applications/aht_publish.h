#ifndef __AHT_PUBLISH_H__
#define __AHT_PUBLISH_H__
#include <rtthread.h>
typedef struct aht_publish
{
	/* data */
	rt_int32_t temp_int;
	rt_int32_t temp_dec;
	rt_int32_t humi_int;
	rt_int32_t humi_dec;
}aht_publish;


#endif
