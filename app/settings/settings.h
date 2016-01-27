#pragma once

#include "compiler/compiler_warnings_control.h"

DISABLE_COMPILER_WARNINGS
#include <QString>
RESTORE_COMPILER_WARNINGS

// Camera
#define SETTINGS_KEY_CAMERA_NAME_FILTER QStringLiteral("Camera/NameFilter")

#define SETTINGS_KEY_IMAGE_WIDTH QStringLiteral("Camera/ImageWidth")
#define SETTINGS_KEY_IMAGE_HEIGHT QStringLiteral("Camera/ImageHeight")

#define SETTINGS_KEY_IMAGE_PIXEL_VALUE_THRESHOLD QStringLiteral("Camera/PixelValueThreshold")

// Updates
#define SETTINGS_KEY_LAST_UPDATE_CHECK_TIMESTAMP QStringLiteral("Internal/Update/LastUpdateCheckTimestamp")
