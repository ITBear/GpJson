#pragma once

#include "GpJson_global.hpp"

#define RAPIDJSON_ASSERT(X) THROW_GPE_COND_CHECK_M(X, "Json processing error");

GP_WARNING_PUSH()
GP_WARNING_DISABLE(switch-enum)

#include <rapidjson/document.h>

GP_WARNING_POP()

namespace GPlatform {

class GpJsonFromStruct
{
public:
    CLASS_REMOVE_CTRS(GpJsonFromStruct);

    enum class Flags: size_t
    {
        WRITE_STRUCT_UID    = 1 << 0
    };

public:
    static std::string  SToString       (const rapidjson::Document& aJsonDOM);
    static void         SToString       (const rapidjson::Document& aJsonDOM,
                                         GpByteWriter&              aWriter);

    static void         SWrite          (const GpTypeStructBase&                aStruct,
                                         rapidjson::Document::GenericValue&     aJsonObject,
                                         rapidjson::Document::AllocatorType&    aJsonAllocator,
                                         const size_t                           aFlags);

private:
    static void         SWriteValue     (const GpTypeStructBase&                aStruct,
                                         const GpTypePropInfo&                  aPropInfo,
                                         rapidjson::Document::GenericValue&     aJsonObject,
                                         rapidjson::Document::AllocatorType&    aJsonAllocator,
                                         const size_t                           aFlags);

    static void         SWriteValueVec  (const GpTypeStructBase&                aStruct,
                                         const GpTypePropInfo&                  aPropInfo,
                                         rapidjson::Document::GenericValue&     aJsonObject,
                                         rapidjson::Document::AllocatorType&    aJsonAllocator,
                                         const size_t                           aFlags);
};

}//namespace GPlatform
