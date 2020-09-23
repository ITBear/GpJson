#pragma once

#include "../GpCore2/GpCore.hpp"

#if defined(GPJSON_LIBRARY)
    #define GPJSON_API GP_DECL_EXPORT
#else
    #define GPJSON_API GP_DECL_IMPORT
#endif
