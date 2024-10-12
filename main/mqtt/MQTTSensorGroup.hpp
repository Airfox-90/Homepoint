#pragma once

#include "MQTTGroup.hpp"
#include "MQTTSensorUtils.hpp"
#include "MQTTSensorTypes.hpp"

#include <optional>
#include <vector>

namespace mqtt
{
  struct MQTTSensorDevice
  {
    std::string name;
    std::string getTopic;
    std::string setTopic;

    std::string value;
    std::vector<ValueTuple> mappedValues;

    std::string getFirstValue()
    {
      if (dataType == MQTTSensorDataType::JSON)
      {
        if (firstUnit.size() > 0)
        {
          return util::FindValue(mappedValues, MQTTFirstKey) + firstUnit;
        } else {
          return util::FindValue(mappedValues, MQTTFirstKey);
        }
      }
      if (firstUnit.size() > 0)
      {
        return value  + firstUnit;
      } else {
        return value;
      }
    }

    std::string getSecondValue()
    {
      if (dataType == MQTTSensorDataType::JSON)
      {
        return util::FindValue(mappedValues, MQTTSecondKey);
      }
      return value;
    }

    void setValue(const std::string topicData)
    {
      if (dataType == MQTTSensorDataType::JSON)
      {
        for (auto& dev : mappedValues)
        {
          const auto key = std::get<1>(dev);
          if (const auto val = util::JsonToValues(topicData, key))
          {
            std::get<2>(dev) = *val;
          }
        }
      }
      else
      {
        value = topicData;
      }
    }

    int deviceId = 0;
    MQTTSensorType sensorType;
    MQTTSensorDataType dataType;
    MQTTSensorDataSource dataSource;
    bool publishSensorValues = false;
    std::string firstIconName;
    std::optional<std::string> secondIconName;
    std::string firstUnit = "";
    std::string secondUnit = "";
    std::string lastJsonData = "";
  };

  struct MQTTSensorGroup : public MQTTGroup
  {
    GroupDevices<MQTTSensorDevice> mSensorDevices;
  };
} // namespace mqtt
