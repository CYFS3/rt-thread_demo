package com.example.app_data;

import org.eclipse.paho.client.mqttv3.MqttClient;
import org.eclipse.paho.client.mqttv3.MqttConnectOptions;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.persist.MemoryPersistence;


public class MqttSample {
    private  final MqttClient client;
    // 持久化
    MemoryPersistence persistence = new MemoryPersistence();
    // MQTT 连接选项
    MqttConnectOptions connOpts = new MqttConnectOptions();
    // 设置认证信息
  public MqttSample(String broker) throws MqttException {
      connOpts.setUserName("app_dev&a1Ayu4gbdUP");
      connOpts.setPassword("B09DC3EAE90BC89B81916539032D690ED95D5D3F".toCharArray());
      client = new MqttClient(broker, "123|securemode=2,signmethod=hmacsha1,timestamp=1723047402243|", persistence);
      client.connect(connOpts);
      // 订阅主题
      client.subscribe("/a1Ayu4gbdUP/app_dev/user/get",0);
  }
  public MqttClient getClient(){
      return client;
  }
}
