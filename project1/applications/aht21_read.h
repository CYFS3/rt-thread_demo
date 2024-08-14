#ifndef __AHT21_READ_H__
#define __AHT21_READ_H__
typedef struct
{
	float humidity;
	float temperature;
} aht21_data;
extern rt_mailbox_t aht10_mb;

#endif // __AHT21_READ_H__