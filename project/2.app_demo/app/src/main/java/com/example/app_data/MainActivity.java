package com.example.app_data;

import android.os.Bundle;
import android.util.Log;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import androidx.activity.EdgeToEdge;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.graphics.Insets;
import androidx.core.view.ViewCompat;
import androidx.core.view.WindowInsetsCompat;

import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.MqttCallback;
import org.eclipse.paho.client.mqttv3.MqttClient;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;
import org.json.JSONException;
import org.json.JSONObject;

public class MainActivity extends AppCompatActivity {
    private String mqtt_url = "tcp://a1Ayu4gbdUP.iot-as-mqtt.cn-shanghai.aliyuncs.com";
    private MqttClient client;
    private TextView temp;
    private TextView humi;
    private TextView light;
    private TextView person;
    int flag = 0;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        EdgeToEdge.enable(this);
        setContentView(R.layout.activity_main);
        ViewCompat.setOnApplyWindowInsetsListener(findViewById(R.id.main), (v, insets) -> {
            Insets systemBars = insets.getInsets(WindowInsetsCompat.Type.systemBars());
            v.setPadding(systemBars.left, systemBars.top, systemBars.right, systemBars.bottom);
            return insets;
        });
        link_mqtt();
        temp = findViewById(R.id.temp);
        humi = findViewById(R.id.humi);
        light = findViewById(R.id.lux);
        person = findViewById(R.id.person);
        EditText et_content = findViewById(R.id.et_content);
        findViewById(R.id.btn_set).setOnClickListener(v -> {
            //编码utf-8
            String str = et_content.getText().toString();
            JSONObject jsonObject = new JSONObject();
            try {
                jsonObject.put("txt", str);
            } catch (JSONException e) {
                e.printStackTrace();
            }
            // 发布消息
            try {
                client.publish("/a1Ayu4gbdUP/app_dev/user/update", jsonObject.toString().getBytes(), 0, false);
            } catch (MqttException e) {
                e.printStackTrace();
            }
    });
        Button but_led = findViewById(R.id.btn_led);
        but_led.setOnClickListener(v -> {
            //编码utf-8
            JSONObject jsonObject = new JSONObject();
           if(flag == 1) {
               try {
                   jsonObject.put("led", 0);
                    but_led.setText("关");
               } catch (JSONException e) {
                   e.printStackTrace();
               }
           }else {
               try {
                   jsonObject.put("led", 1);
                     but_led.setText("开");
               } catch (JSONException e) {
                   e.printStackTrace();
               }
           }
           flag = 1 - flag;
            // 发布消息
            try {
                client.publish("/a1Ayu4gbdUP/app_dev/user/update", jsonObject.toString().getBytes(), 0, false);
            } catch (MqttException e) {
                e.printStackTrace();
            }
        });
    }

    private void link_mqtt() {
        try {
            MqttSample mqttSample = new MqttSample(mqtt_url);
            client = mqttSample.getClient();
            client.setCallback(new MqttCallback() {
                @Override
                public void connectionLost(Throwable cause) {
                    // 安卓弹窗提示
                    Toast.makeText(MainActivity.this, "连接断开，正在重连", Toast.LENGTH_SHORT).show();
                }

                @Override
                public void messageArrived(String topic, MqttMessage message) throws Exception {
                    String msg = new String(message.getPayload());
                    // 转化为string
                    String str = new String(message.getPayload());
                    // 转化为json

                    JSONObject jsonObject = new JSONObject(str);
                    JSONObject items = jsonObject.getJSONObject("items");
                    String temperature = items.getJSONObject("CurrentTemperature").getString("value");
                    String huimidity = items.getJSONObject("CurrentHumidity").getString("value");
                    String LightLux = items.getJSONObject("LightLux").getString("value");
                    String pa_data = items.getJSONObject("pa_data").getString("value");
                    // 切回主线程
                    runOnUiThread(() -> {
                        temp.setText(temperature);
                        humi.setText(huimidity);
                        light.setText(LightLux);
                        person.setText(pa_data);
                    });


                }

                @Override
                public void deliveryComplete(IMqttDeliveryToken token) {
                    System.out.println("deliveryComplete---------" + token.isComplete());
                }
            });
        } catch (MqttException e) {
            throw new RuntimeException(e);
        }
    }
}