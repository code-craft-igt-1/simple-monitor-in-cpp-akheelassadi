#include "./monitor.h"
#include <assert.h>
#include <thread>
#include <chrono>
#include <iostream>
using std::cout, std::flush, std::this_thread::sleep_for, std::chrono::seconds;

bool isTemperatureCritical(float temperature) {
  return (temperature > 102 || temperature < 95);
}

bool isPulseRateCritical(float pulseRate) {
  return (pulseRate < 60 || pulseRate > 100);
}

bool isSpo2Critical(float spo2) {
  return (spo2 < 90);
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

int handleVitalCheck(const std::string& message, bool isCritical) {
    if (isCritical) {
        displayWarning(message);
        return 0;
    }
    return 1;
}

int vitalsOk(float temperature, float pulseRate, float spo2) {
    if (handleVitalCheck("Temperature is critical", isTemperatureCritical(temperature)) == 0) {
        return 0;
    }
    if (handleVitalCheck("Pulse Rate is out of range", isPulseRateCritical(pulseRate)) == 0) {
        return 0;
    }
    if (handleVitalCheck("Oxygen Saturation out of range", isSpo2Critical(spo2)) == 0) {
        return 0;
    }
    return 1;
}