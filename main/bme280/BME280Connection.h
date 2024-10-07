#pragma once

#include <Bme280.h>
#include <util/dispatcher.hpp>
#include <string>

extern "C"
{
  #include "freertos/task.h"
}

namespace bme280
{

  struct BME280Config
  {
    int sda_pin = 32;
    int scl_pin = 33;
  };

  struct BME280TaskArgs
  {
    Bme280TwoWire* pSensor;
    Dispatcher<std::string>* pSensorDataNotifier;
  };

  using BME280DataCB = std::function<void(std::string jsonData)>;

  class BME280Connection
  {
    public:
      BME280Connection(BME280Config config);
      ~BME280Connection();
      void setup();
      void registerSensorDataCallback(BME280DataCB cb);

    private:
      BME280Config mConfig;
      Bme280TwoWire sensor;
      BME280TaskArgs mTaskArgs;
      TaskHandle_t sensor_task_handle = NULL;
      Dispatcher<std::string> mSensorDataNotifier;
  };
} // namespace bme280
