#pragma once

enum class Button {
    SW2 = 2,
    SW3 = 3,
};

bool readSWClick(Button btn);
bool readSW(Button btn);
