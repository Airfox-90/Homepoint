#pragma once

#include <model/HardwareConfig.hpp>
#include <mqtt/MQTTConnection.h>
#include <mqtt/MQTTGroup.hpp>
#include <mqtt/MQTTSensorGroup.hpp>
#include <bme280/BME280Connection.h>

#include <map>
#include <string>
#include <tuple>
#include <vector>

struct WifiCredentials
{
  std::string mSSID;
  std::string mPassword;
  std::optional<std::string> mHostname;
};
using WebCredentials = std::tuple<std::string, std::string>;

namespace model
{
struct Model
{
  WifiCredentials mWifiCredentials;
  WebCredentials mWebCredentials;
  std::optional<std::string> mHostName;
  config::HardwareConfig mHardwareConfig;
  std::string mTimeZone;
  mqtt::MQTTConfig mMQTTServerConfig;
  std::vector<MQTTVariants> mMQTTGroups;
  bme280::BME280Config mBME280Config;

  bool hasWifiCredentials()
  {
    return mWifiCredentials.mSSID != " ";
  }
};
} // namespace model
