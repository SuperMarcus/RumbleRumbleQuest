#pragma once

#include <algorithm>
#include "beatsaber-hook/shared/utils/logging.hpp"

typedef struct Color32 {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
} Color32;

Logger& getLogger();

bool RRQSettingsControllerIsPresenting();
void RRQInstallHooks();
