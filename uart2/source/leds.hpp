#pragma once

#include <chrono>

enum class LED {
    RGB_Red,
    RGB_Green,
    RGB_Blue,
};

enum Color {
    Black = 0,
    Red = 1 << 0,
    Green = 1 << 1,
    Blue = 1 << 2,
    Yellow = Red | Green,
    Cyan = Green | Blue,
    Magenta = Red | Blue,
    White = Red | Green | Blue,
};

void setLED(LED led, bool on);
void showColor(Color color);
void blink(Color color,
    unsigned times = 3,
    std::chrono::microseconds onTime = std::chrono::milliseconds(500),
    std::chrono::microseconds offTime = std::chrono::milliseconds(500));
