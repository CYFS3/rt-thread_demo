/*
 * Copyright (C) 2015-2018 Alibaba Group Holding Limited
 *
 * Again edit by rt-thread group
 * Change Logs:
 * Date          Author          Notes
 * 2019-07-21    MurphyZhao      first edit
 */

#include "dev_sign_api.h"
#include "mqtt_api.h"
#include "aht_publish.h"
#include "cjson.h"
#include <board.h>
#include <drv_gpio.h>
#include "aht10.h"
#include <dfs_posix.h>
char DEMO_PRODUCT_KEY[IOTX_PRODUCT_KEY_LEN + 1] = {0};
char DEMO_DEVICE_NAME[IOTX_DEVICE_NAME_LEN + 1] = {0};
char DEMO_DEVICE_SECRET[IOTX_DEVICE_SECRET_LEN + 1] = {0};

void *HAL_Malloc(uint32_t size);
void HAL_Free(void *ptr);
void HAL_Printf(const char *fmt, ...);
int HAL_GetProductKey(char product_key[IOTX_PRODUCT_KEY_LEN + 1]);
int HAL_GetDeviceName(char device_name[IOTX_DEVICE_NAME_LEN + 1]);
int HAL_GetDeviceSecret(char device_secret[IOTX_DEVICE_SECRET_LEN]);
uint64_t HAL_UptimeMs(void);
int HAL_Snprintf(char *str, const int len, const char *fmt, ...);

#define EXAMPLE_TRACE(fmt, ...)                        \
    do                                                 \
    {                                                  \
        HAL_Printf("%s|%03d :: ", __func__, __LINE__); \
        HAL_Printf(fmt, ##__VA_ARGS__);                \
        HAL_Printf("%s", "\r\n");                      \
    } while (0)

static void example_message_arrive(void *pcontext, void *pclient, iotx_mqtt_event_msg_pt msg)
{
    iotx_mqtt_topic_info_t *topic_info = (iotx_mqtt_topic_info_pt)msg->msg;

    switch (msg->event_type)
    {
    case IOTX_MQTT_EVENT_PUBLISH_RECEIVED:
        /* print topic name and topic message */
        EXAMPLE_TRACE("Message Arrived:");
        EXAMPLE_TRACE("Topic  : %.*s", topic_info->topic_len, topic_info->ptopic);
        EXAMPLE_TRACE("Payload: %.*s", topic_info->payload_len, topic_info->payload);
        EXAMPLE_TRACE("\n");
        cJSON *root = cJSON_Parse(topic_info->payload);
        cJSON *params = cJSON_GetObjectItem(root, "params");
        cJSON *LED = cJSON_GetObjectItem(params, "LEDSwitch");
        if (LED != NULL)
        {
            rt_kprintf("LED Switch: %d", LED->valueint);
            if(LED->valueint == 0)
            {
                rt_kprintf("LED ON\n");
                rt_pin_write(GET_PIN(F, 12), PIN_HIGH);
            }
            else
            {
                rt_kprintf("LED OFF\n");
                rt_pin_write(GET_PIN(F, 12), PIN_LOW);
            }
        }
        cJSON_Delete(root);

        break;
    default:
        break;
    }
}

static int example_subscribe(void *handle)
{
    int res = 0;
    const char *fmt = "/sys/%s/%s/thing/service/property/set";
    char *topic = NULL;
    int topic_len = 0;

    topic_len = strlen(fmt) + strlen(DEMO_PRODUCT_KEY) + strlen(DEMO_DEVICE_NAME) + 1;
    topic = HAL_Malloc(topic_len);
    if (topic == NULL)
    {
        EXAMPLE_TRACE("memory not enough");
        return -1;
    }
    memset(topic, 0, topic_len);
    HAL_Snprintf(topic, topic_len, fmt, DEMO_PRODUCT_KEY, DEMO_DEVICE_NAME);

    res = IOT_MQTT_Subscribe(handle, topic, IOTX_MQTT_QOS0, example_message_arrive, NULL);
    if (res < 0)
    {
        EXAMPLE_TRACE("subscribe failed");
        HAL_Free(topic);
        return -1;
    }

    HAL_Free(topic);
    return 0;
}

static int example_publish(void *handle,aht_publish * data)
{
    int res = 0;
    const char *fmt = "/sys/%s/%s/thing/event/property/post";
    char *topic = NULL;
    int topic_len = 0;
    char *payload = "{\"params\":{\"CurrentTemperature\":%d.%d,\"CurrentHumidity\":%d.%d}}";
    char message[256];
    HAL_Snprintf(message,256,payload,data->temp_int,data->temp_dec,data->humi_int,data->humi_dec);
    topic_len = strlen(fmt) + strlen(DEMO_PRODUCT_KEY) + strlen(DEMO_DEVICE_NAME) + 1;
    topic = HAL_Malloc(topic_len);
    if (topic == NULL)
    {
        EXAMPLE_TRACE("memory not enough");
        return -1;
    }
    memset(topic, 0, topic_len);
    HAL_Snprintf(topic, topic_len, fmt, DEMO_PRODUCT_KEY, DEMO_DEVICE_NAME);

    res = IOT_MQTT_Publish_Simple(0, topic, IOTX_MQTT_QOS0, message, strlen(message));
    if (res < 0)
    {
        EXAMPLE_TRACE("publish failed, res = %d", res);
        HAL_Free(topic);
        return -1;
    }

    HAL_Free(topic);
    return 0;
}

static void example_event_handle(void *pcontext, void *pclient, iotx_mqtt_event_msg_pt msg)
{
    EXAMPLE_TRACE("msg->event_type : %d", msg->event_type);
}



/*-----------------------------------------------------------------------------------*/

//星火一号AHT10使用的为i2c3
#define AHT10_I2C_BUS "i2c3"


//定义线程的信息
#define THREAD_PRIORITY 10      //线程优先级
#define THREAD_STACK_SIZE 2048  //线程栈大小
#define THREAD_TIMSLICE 5		//线程时间片


//发送数据邮箱
rt_mailbox_t aht10_mailbox = RT_NULL;
//AHT线程指针
rt_thread_t AHT10 = RT_NULL;

void aht10_demo(void * paramenter)
{
	aht10_device_t aht10_dev = RT_NULL;
	aht10_dev = aht10_init(AHT10_I2C_BUS);
	if(aht10_dev == RT_NULL)
	{
		rt_kprintf("aht10 init fail\n");
		return;
	}
    int fd = 0;
    int count = 0;
	float humi = 0,temp = 0;
    aht_publish data = {0};
    char buf[128];
    fd = open("/fal/Data.txt",O_TRUNC|O_RDWR );
     if(fd < 0)
        {
            rt_kprintf("open file fail\n");
            return;
        }
	//读取温湿度
	while(1)
	{
		temp = aht10_read_temperature(aht10_dev);
		humi = aht10_read_humidity(aht10_dev);

		data.temp_int = (int)temp;
        data.temp_dec = (int)(temp*10)%10;
        data.humi_int = (int)humi;
        data.humi_dec = (int)(humi*10)%10;
        rt_kprintf("aht10 temp %d.%d\n",(int)temp,(int)(temp*10)%10);
        rt_kprintf("aht10 humi %d.%d\n",(int)humi,(int)(humi*10)%10);
        rt_mb_send(aht10_mailbox,(rt_uint32_t)&data);
		
       
        rt_snprintf(buf,128,"Temp: %d.%d;Humi: %d.%d; Count: %d\n",(int)data.temp_int,(int)data.temp_dec,(int)data.humi_int,(int)data.humi_dec,++count);
        ssize_t write_len = 0;
        write_len = write(fd,buf,strlen(buf));
        if(write_len <= 0)
        {
            rt_kprintf("write file fail\n");
            close(fd);
            return;
        }
        fsync(fd);
        rt_thread_mdelay(5000);
	}
}


void aht10_task(void)
{
	AHT10 = rt_thread_create("aht10",aht10_demo,RT_NULL,THREAD_STACK_SIZE,9,THREAD_TIMSLICE);
	if(AHT10 == RT_NULL)
	{
		rt_kprintf("aht10 thread create fail\n");
		return;

	}
	rt_thread_startup(AHT10);
	return;
}





void mqtt_example_main(void *parameter)
{
    void *pclient = NULL;
    int res = 0;
    int loop_cnt = 0;
    iotx_mqtt_param_t mqtt_params;

    HAL_GetProductKey(DEMO_PRODUCT_KEY);
    HAL_GetDeviceName(DEMO_DEVICE_NAME);
    HAL_GetDeviceSecret(DEMO_DEVICE_SECRET);

    EXAMPLE_TRACE("mqtt example");


    memset(&mqtt_params, 0x0, sizeof(mqtt_params));

  
    mqtt_params.handle_event.h_fp = example_event_handle;

    pclient = IOT_MQTT_Construct(&mqtt_params);
    if (NULL == pclient)
    {
        EXAMPLE_TRACE("MQTT construct failed");
        return;
    }

    res = example_subscribe(pclient);
    if (res < 0)
    {
        IOT_MQTT_Destroy(&pclient);
        return;
    }
    aht_publish * data;
    while (1)
    {
        
        if(rt_mb_recv(aht10_mailbox,(rt_uint32_t *)&data,RT_WAITING_FOREVER) == RT_EOK)
        {
            example_publish(pclient,data);
        }
        IOT_MQTT_Yield(pclient, 200);

        
    }
}



rt_thread_t MQTT_Thread;

void MQTT_Creat_Thread(void)
{
    // 创建线程
    MQTT_Thread = rt_thread_create("MQTT_Thread", mqtt_example_main, RT_NULL, 4096, 10, 5);
    // 创建成功就启动
    if (MQTT_Thread != RT_NULL)
    {
        rt_thread_startup(MQTT_Thread);
    }
    else
    {
        rt_kprintf("MQTT_Thread_Thread Create Fail");
    }
}

void aht10_publish(void)
{
     aht10_mailbox = rt_mb_create("aht10",10,RT_IPC_FLAG_PRIO);
    if(aht10_mailbox == RT_NULL)
    {
        rt_kprintf("create mailbox fail\n");
        return;
    }
    aht10_task();
    MQTT_Creat_Thread();

}

MSH_CMD_EXPORT(aht10_publish, aht10 publish init);




/*   文件操作系统 */

#define WIFI_CS_PIN GET_PIN(F,10)
int WIFI_CS_DWON(void)
{
    rt_pin_mode(WIFI_CS_PIN,PIN_MODE_OUTPUT);
    rt_pin_write(WIFI_CS_PIN,PIN_LOW);
    return RT_EOK;
}
INIT_BOARD_EXPORT(WIFI_CS_DWON);

