#include "./monitor.h"

#include "gtest/gtest.h"

TEST(Monitor, NotOkWhenAnyVitalIsOffRange) {
  ASSERT_FALSE(vitalsOk(99, 102, 70));
}

TEST(Monitor, OkWhenAnyVitalIsInRange) {
  ASSERT_TRUE(vitalsOk(98.1, 70, 98));
}

TEST(Monitor, NotOkWhenTemperatureIsTooHigh) {
  ASSERT_FALSE(vitalsOk(102.1, 75, 95));
}

TEST(Monitor, NotOkWhenTemperatureIsTooLow) {
  ASSERT_FALSE(vitalsOk(94.9, 75, 95));
}

TEST(Monitor, OkWhenTemperatureIsInRange) {
  ASSERT_TRUE(vitalsOk(98.6, 75, 95));
}

// Test pulse rate boundary conditions
TEST(Monitor, NotOkWhenPulseRateIsTooHigh) {
  ASSERT_FALSE(vitalsOk(98.6, 101, 95));
}

TEST(Monitor, NotOkWhenPulseRateIsTooLow) {
  ASSERT_FALSE(vitalsOk(98.6, 59, 95));
}

TEST(Monitor, OkWhenPulseRateIsInRange) {
  ASSERT_TRUE(vitalsOk(98.6, 75, 95));
}

// Test SPO2 boundary conditions
TEST(Monitor, NotOkWhenSpo2IsTooLow) {
  ASSERT_FALSE(vitalsOk(98.6, 75, 89));
}

TEST(Monitor, OkWhenSpo2IsInRange) {
  ASSERT_TRUE(vitalsOk(98.6, 75, 95));
}
