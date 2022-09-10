#pragma once

#include "GpJson_global.hpp"

namespace GPlatform {

GP_ENUM(GP_JSON_API, GpJsonSerializerFlag,
    WRITE_MODEL_UID
);

using GpJsonSerializerFlags = GpEnumFlagsST<GpJsonSerializerFlag>;

}//namespace GPlatform
