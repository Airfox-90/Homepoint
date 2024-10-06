#pragma once

#include <Bme280.h>

namespace bme280
{

  struct BME280Config
  {
    int sda_pin = 32;
    int scl_pin = 33;
  };

  class BME280Connection
  {
    public:
      BME280Connection(BME280Config config);
      ~BME280Connection();
      void setup();
      
    private:
      BME280Config mConfig;
      Bme280TwoWire sensor;
  };
} // namespace bme280
