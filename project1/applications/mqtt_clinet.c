#include "rtthread.h"
#include "dev_sign_api.h"
#include "mqtt_api.h"
#include "aht21_read.h"
#include "ap3216_read.h"
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

#define EXAMPLE_TRACE(fmt, ...)  \
do \
{ \
    HAL_Printf("%s|%03d :: ", __func__, __LINE__); \
    HAL_Printf(fmt, ##__VA_ARGS__); \
    HAL_Printf("%s", "\r\n"); \
} while(0)

static void example_message_arrive(void *pcontext, void *pclient, iotx_mqtt_event_msg_pt msg)
{
    iotx_mqtt_topic_info_t     *topic_info = (iotx_mqtt_topic_info_pt) msg->msg;

    switch (msg->event_type) 
    {
        case IOTX_MQTT_EVENT_PUBLISH_RECEIVED:
            /* print topic name and topic message */
            EXAMPLE_TRACE("Message Arrived:");
            EXAMPLE_TRACE("Topic  : %.*s", topic_info->topic_len, topic_info->ptopic);
            EXAMPLE_TRACE("Payload: %.*s", topic_info->payload_len, topic_info->payload);
            EXAMPLE_TRACE("\n");
            break;
        default:
            break;
    }
}

static int example_subscribe(void *handle)
{
    int res = 0;
    const char *fmt = "/sys/%s/%s/thing/event/property/post";
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

static int example_publish(void *handle,char * payload)
{
    int             res = 0;
    const char     *fmt = "/sys/%s/%s/thing/event/property/post";
    char           *topic = NULL;
    int             topic_len = 0;
    

    topic_len = strlen(fmt) + strlen(DEMO_PRODUCT_KEY) + strlen(DEMO_DEVICE_NAME) + 1;
    topic = HAL_Malloc(topic_len);
    if (topic == NULL) {
        EXAMPLE_TRACE("memory not enough");
        return -1;
    }
    memset(topic, 0, topic_len);
    HAL_Snprintf(topic, topic_len, fmt, DEMO_PRODUCT_KEY, DEMO_DEVICE_NAME);

    res = IOT_MQTT_Publish_Simple(0, topic, IOTX_MQTT_QOS0, payload, strlen(payload));
    if (res < 0) {
        //EXAMPLE_TRACE("publish failed, res = %d", res);
        HAL_Free(topic);
        return -1;
    }
    HAL_Free(topic);
    return 0;
}
static void example_event_handle(void *pcontext, void *pclient, iotx_mqtt_event_msg_pt msg)
{
 //  EXAMPLE_TRACE("msg->event_type : %d", msg->event_type);
}
extern rt_sem_t wifi_sem;
void mqtt_example_main(void *parameter)
{

    while (wifi_sem == RT_NULL)
    {
       rt_thread_mdelay(500);
    }
    rt_sem_take(wifi_sem, RT_WAITING_FOREVER);
    rt_sem_release(wifi_sem);
    rt_sem_delete(wifi_sem);
    void                   *pclient = NULL;
    int                     res = 0;
    int                     loop_cnt = 0;
    iotx_mqtt_param_t       mqtt_params;
	// 获取连接的参数设置，该参数是在menuconfig中定义的，最终在rtconfig.h中定义
    HAL_GetProductKey(DEMO_PRODUCT_KEY);
    HAL_GetDeviceName(DEMO_DEVICE_NAME);
    HAL_GetDeviceSecret(DEMO_DEVICE_SECRET);
    // 初始化连接参数
    memset(&mqtt_params, 0x0, sizeof(mqtt_params));

    mqtt_params.handle_event.h_fp = example_event_handle;

    pclient = IOT_MQTT_Construct(&mqtt_params);
    if (NULL == pclient) 
    {
        EXAMPLE_TRACE("MQTT construct failed");
        
    }

    // res = example_subscribe(pclient);
    // if (res < 0) 
    // {
    //     IOT_MQTT_Destroy(&pclient);
        
    // }
    aht21_data *aht21;
    ap3216_data *ap3216;
    char payload[256];
    while (1) 
    {
        rt_mb_recv(aht10_mb, (rt_ubase_t *)(&aht21), RT_WAITING_FOREVER);
        rt_mb_recv(ap3216_mb, (rt_ubase_t *)(&ap3216), RT_WAITING_FOREVER);
        //rt_kprintf("aht21->humidity:%.2f\n,aht21->temperature:%.2f\n,ap3216->brightness:%.2f\n,ap3216->ps_data:%d\n",aht21->humidity,aht21->temperature,ap3216->brightness,ap3216->ps_data);
        HAL_Snprintf(payload, sizeof(payload), "{\"params\":{\"pa_data\":%d,\"LightLux\":%.2f,\"CurrentHumidity\":%.2f,\"CurrentTemperature\":%.2f}}", 
        ap3216->ps_data,
        ap3216->brightness,
        aht21->humidity,
        aht21->temperature);
        res = example_publish(pclient,payload);       
        IOT_MQTT_Yield(pclient, 200);
        rt_thread_mdelay(1000);
    }
    
}

int mqtt_init(void)
{
    rt_thread_t tid;
    tid = rt_thread_create("mqtt_example", mqtt_example_main, RT_NULL, 3*1024, 10, 10);
    if (tid != RT_NULL)
    {
        rt_thread_startup(tid);
        return RT_EOK;
    }
    return -RT_ERROR;
}
//INIT_APP_EXPORT(mqtt_init);
