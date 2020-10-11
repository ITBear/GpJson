#pragma once

#include "GpJson_global.hpp"

#define RAPIDJSON_ASSERT(X) THROW_GPE_COND_CHECK_M(X, "Json processing error");

GP_WARNING_PUSH()
GP_WARNING_DISABLE(switch-enum)

#include <rapidjson/document.h>

GP_WARNING_POP()

namespace GPlatform {

class GPJSON_API GpJsonMapperTypeDetector
{
public:
    CLASS_REMOVE_CTRS_EXCEPT_DEFAULT(GpJsonMapperTypeDetector)
    CLASS_DECLARE_DEFAULTS(GpJsonMapperTypeDetector)

public:
                                    GpJsonMapperTypeDetector    (void) noexcept {}
    virtual                         ~GpJsonMapperTypeDetector   (void) noexcept {}

    virtual const GpTypeStructInfo& DetectTypeStructInfo        (const rapidjson::Document::ConstObject& aJsonObject) const = 0;
};

}//namespace GPlatform
