#ifndef __AP3216_READ_H__
#define __AP3216_READ_H__
typedef struct 
{
	rt_uint16_t ps_data;
	float brightness;
}ap3216_data;
extern rt_mailbox_t ap3216_mb;

#endif // __AP3216_READ_H__
