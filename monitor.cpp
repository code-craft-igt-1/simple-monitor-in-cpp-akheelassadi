#include "./monitor.h"
#include <assert.h>
#include <thread>
#include <chrono>
#include <iostream>
using std::cout, std::flush, std::this_thread::sleep_for, std::chrono::seconds;

// Warning tolerances (1.5% of upper limit)
const float tempWarningTolerance = 102 * 0.015;
const float pulseWarningTolerance = 100 * 0.015;
const float spo2WarningTolerance = 100 * 0.015;

bool isTemperatureCritical(float temperature) {
  return (temperature > 102 || temperature < 95);
}

bool isPulseRateCritical(float pulseRate) {
  return (pulseRate < 60 || pulseRate > 100);
}

bool isSpo2Critical(float spo2) {
  return (spo2 < 90);
}

bool isTemperatureWarning(float temperature) {
    return (temperature >= 95 && temperature <= 95 + tempWarningTolerance) ||
        (temperature >= 102 - tempWarningTolerance && temperature <= 102);
}

bool isPulseRateWarning(float pulseRate) {
    return (pulseRate >= 60 && pulseRate <= 60 + pulseWarningTolerance) ||
        (pulseRate >= 100 - pulseWarningTolerance && pulseRate <= 100);
}

bool isSpo2Warning(float spo2) {
    return (spo2 >= 90 && spo2 <= 90 + spo2WarningTolerance);
}

void displayEarlyWarning(const std::string& message) {
    cout << "Warning: " << message << "\n";
}

void displayWarning(const std::string& message) {
  cout << message << "\n";
  for (int i = 0; i < 6; i++) {
    cout << "\r* " << flush;
    sleep_for(seconds(1));
    cout << "\r *" << flush;
    sleep_for(seconds(1));
  }
}

int handleAndCheckVitals(const std::string& criticalMessage,
                         const std::string& warningMessage,
                         bool isCritical, bool isWarning) {
    if (isWarning) {
        displayEarlyWarning(warningMessage);
    }
    if (isCritical) {
        displayWarning(criticalMessage);
        return 0;
    }
    return 1;
}

int vitalsOk(float temperature, float pulseRate, float spo2) {
    int result = 1;
    result &= handleAndCheckVitals("Temperature is critical!",
        "Approaching hypothermia/hyperthermia!",
        isTemperatureCritical(temperature),
        isTemperatureWarning(temperature));

    result &= handleAndCheckVitals("Pulse Rate is out of range!",
        "Approaching abnormal pulse rate!",
        isPulseRateCritical(pulseRate),
        isPulseRateWarning(pulseRate));

    result &= handleAndCheckVitals("Oxygen Saturation out of range!",
        "Approaching abnormal SPO2 levels!",
        isSpo2Critical(spo2),
        isSpo2Warning(spo2));

    return result;
}
