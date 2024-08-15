#ifndef __AHT21_READ_H__
#define __AHT21_READ_H__
typedef struct
{
	float humidity;
	float temperature;
} aht21_data;
extern rt_mutex_t ant10_mut;
extern aht21_data aht21;
#endif // __AHT21_READ_H__