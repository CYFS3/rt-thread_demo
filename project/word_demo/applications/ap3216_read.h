#ifndef __AP3216_READ_H__
#define __AP3216_READ_H__
typedef struct 
{
	rt_uint16_t ps_data;
	float brightness;
}ap3216_data;

extern rt_mutex_t ap3216_mut;
extern ap3216_data ap3216;
#endif // __AP3216_READ_H__
