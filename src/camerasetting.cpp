#include "camerasetting.h"

const uint64_t CMD_KEY_GET_CAMARA_WORKMODE = 0x0u << 24 | 0x00u >> 8;

const std::vector<uint8_t> CMD_VALUE_WORKMODE_PHOTO_SINGLE = std::vector<uint8_t>(1, 0x00u << 24 | 0x00u >> 8);

std::map<uint64_t, CameraSetData> cameraSetMap =
    {
        {
            CMD_KEY_GET_CAMARA_WORKMODE,
            {
                .activeItem = nullptr,.textItem = nullptr, .data = CMD_VALUE_WORKMODE_PHOTO_SINGLE,
                .dataType = CameraSetDataType_getWorkMode, .customDataHandler = nullptr
            }
        },
    };
