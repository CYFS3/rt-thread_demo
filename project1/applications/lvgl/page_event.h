#ifndef __PAGE_EVENT_H
#define __PAGE_EVENT_H
#include <rtthread.h>
/*Clock*/
typedef struct
{
    rt_uint8_t hour;
    rt_uint8_t min;
    rt_uint8_t sec;
    rt_uint8_t ms;
    rt_uint8_t year;
    rt_uint8_t  month;
    rt_uint8_t  date;
    rt_uint8_t  week;
} Clock_Value_t;

void date_thread_crearte(void);
void data_thread_delete(void);
int read_page_start(void);
void read_word(int count,char * buf);
void read_page_end(int count);

#endif
