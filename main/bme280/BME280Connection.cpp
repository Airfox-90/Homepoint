#include <Arduino.h>
#include "BME280Connection.h"

static const char* TAG = "BME280";

namespace bme280
{
  BME280Connection::BME280Connection(BME280Config config) :
    mConfig(config)
  {

  }

  void BME280Connection::setup()
  {
    Wire.begin(mConfig.sda_pin, mConfig.scl_pin);
    if (sensor.begin(Bme280TwoWireAddress::Primary)) {
        sensor.setSettings(Bme280Settings::indoor());
        ESP_LOGI(TAG, "BME280 Sensor successfully initialzed");
    };
  }

  BME280Connection::~BME280Connection()
  {
    ESP_LOGI(TAG, "Shutting down BME280");
  }

} // namespace bme280
