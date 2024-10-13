#pragma once

#include <GpJson/GpJson_global.hpp>
#include <GpCore2/GpUtils/Types/Enums/GpEnum.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpStringOps.hpp>

namespace GPlatform {

GP_ENUM(GP_JSON_API, GpJsonSerializerFlag,
    WRITE_MODEL_UID
);

using GpJsonSerializerFlags = GpEnumFlagsST<GpJsonSerializerFlag>;

}// namespace GPlatform
