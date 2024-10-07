#include <Arduino.h>
#include "BME280Connection.h"
#include <iomanip>
#include <sstream>

static const char* TAG = "BME280";

namespace bme280
{
    static void sensor_task(void * arg){
      BME280TaskArgs taskArgs = *static_cast<BME280TaskArgs *>(arg); 
      Bme280TwoWire sensor =  *taskArgs.pSensor;
        for (;;) {
            
            float temperature = sensor.getTemperature();
            float pressure = sensor.getPressure() / 100;
            float humidity = sensor.getHumidity();

            std::stringstream stream;
            stream << "{\"temperature\":" << std::fixed << std::setprecision(1) << temperature << ",";
            stream << "\"pressure\":" << std::fixed << std::setprecision(0) << pressure << ",";
            stream << "\"humidity\":" << std::fixed << std::setprecision(1) << humidity << "}";
            std::string s = stream.str();
            
            taskArgs.pSensorDataNotifier->broadcast(s);
            delay(10000);
        }
        vTaskDelete(NULL);
        //sensor_task_handle = NULL;
    }

  BME280Connection::BME280Connection(BME280Config config) :
    mConfig(config)
  {
    mTaskArgs.pSensorDataNotifier = &mSensorDataNotifier;
  }

  void BME280Connection::setup()
  {
    ESP_LOGI(TAG, "Starting BME280 Sensor on %d, %d", mConfig.sda_pin, mConfig.scl_pin);
    Wire.begin(mConfig.sda_pin, mConfig.scl_pin);
    if (sensor.begin(Bme280TwoWireAddress::Primary)) {
        sensor.setSettings(Bme280Settings::indoor());
        ESP_LOGI(TAG, "BME280 Sensor successfully initialzed");
        if(!sensor_task_handle){
            mTaskArgs.pSensor = &sensor;
            xTaskCreateUniversal(sensor_task, "sensor_task", 4096, &mTaskArgs, tskIDLE_PRIORITY, &sensor_task_handle, CONFIG_ARDUINO_RUNNING_CORE);
            if(!sensor_task_handle){
                ESP_LOGE(TAG, "Sensor Task Start Failed!");
            }
        }
    } else {
      ESP_LOGE(TAG, "Wire.begin failed!");
    }
  }

  BME280Connection::~BME280Connection()
  {
    ESP_LOGI(TAG, "Shutting down BME280");
  }

  void BME280Connection::registerSensorDataCallback(BME280DataCB cb)
  {
    mSensorDataNotifier.addCB(cb);
  }

} // namespace bme280
