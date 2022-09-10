#pragma once

#include "../GpCore2/GpUtils/GpUtils.hpp"
#include "../GpCore2/GpReflection/GpReflection.hpp"

#if defined(GP_JSON_LIBRARY)
    #define GP_JSON_API GP_DECL_EXPORT
#else
    #define GP_JSON_API GP_DECL_IMPORT
#endif
