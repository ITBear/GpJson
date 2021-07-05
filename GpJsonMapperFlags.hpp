#pragma once

#include "GpJson_global.hpp"

namespace GPlatform {

GP_ENUM(GPJSON_API, GpJsonMapperFlag,
    WRITE_STRUCT_UID,
    IGNORE_WRONG_STRUCT_UID
);

using GpJsonMapperFlags = GpEnumFlagsST<GpJsonMapperFlag>;

}//namespace GPlatform
