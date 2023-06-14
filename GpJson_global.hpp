#pragma once

#include "GpJsonConfigure.hpp"
#include "../GpCore2/GpUtils/GpUtils_global.hpp"

#if defined(GP_JSON_LIBRARY)
    #define GP_JSON_API GP_DECL_EXPORT
#else
    #define GP_JSON_API GP_DECL_IMPORT
#endif
