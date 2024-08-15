package com.example.app_data;

import android.os.Bundle;
import android.util.Log;
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
import org.json.JSONObject;

public class MainActivity extends AppCompatActivity {
    private String mqtt_url = "tcp://a1Ayu4gbdUP.iot-as-mqtt.cn-shanghai.aliyuncs.com";
    private MqttClient client;
    private TextView temp;
    private TextView humi;
    private TextView light;
    private TextView person;


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