#include "./monitor.h"
#include <assert.h>
#include <thread>
#include <chrono>
#include <iostream>
using std::cout, std::flush, std::this_thread::sleep_for, std::chrono::seconds;

// Define limits and tolerances
const float temperatureUpper = 102.0f, temperatureLower = 95.0f;
const float pulseUpper = 100.0f, pulseLower = 60.0f;
const float spo2Lower = 90.0f;

const float tempWarningTolerance = temperatureUpper * 0.015;
const float pulseWarningTolerance = pulseUpper * 0.015;
const float spo2WarningTolerance = 100 * 0.015;

bool isValueCritical(float value, float lowerBound, float upperBound) {
    return (value < lowerBound || value > upperBound);
}

bool isWithinWarningRange(float value, float lowerBound, float upperBound, float tolerance) {
    return (value >= lowerBound - tolerance && value <= upperBound + tolerance);
}

bool isTemperatureCritical(float temperature) {
    return isValueCritical(temperature, temperatureLower, temperatureUpper);
}

bool isPulseRateCritical(float pulseRate) {
    return isValueCritical(pulseRate, pulseLower, pulseUpper);
}

bool isSpo2Critical(float spo2) {
    return (spo2 < spo2Lower);
}

bool isTemperatureWarning(float temperature) {
    return isWithinWarningRange(temperature,
                                temperatureLower, temperatureUpper, tempWarningTolerance);
}

bool isPulseRateWarning(float pulseRate) {
    return isWithinWarningRange(pulseRate, pulseLower, pulseUpper, pulseWarningTolerance);
}

bool isSpo2Warning(float spo2) {
    return (spo2 >= spo2Lower && spo2 <= spo2Lower + spo2WarningTolerance);
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

// Handle vitals with critical and warning checks
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
