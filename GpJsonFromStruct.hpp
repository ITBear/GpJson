#pragma once

#include "GpJsonMapperFlags.hpp"

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

public:
    static std::string  SToString       (const rapidjson::Document& aJsonDOM);
    static void         SToString       (const rapidjson::Document& aJsonDOM,
                                         GpByteWriter&              aWriter);

    static void         SWrite          (const GpTypeStructBase&                aStruct,
                                         rapidjson::Document::GenericValue&     aJsonObject,
                                         rapidjson::Document::AllocatorType&    aJsonAllocator,
                                         const GpJsonMapperFlags                aFlags);

private:
    static void         SWriteValue     (const GpTypeStructBase&                aStruct,
                                         const GpTypePropInfo&                  aPropInfo,
                                         rapidjson::Document::GenericValue&     aJsonObject,
                                         rapidjson::Document::AllocatorType&    aJsonAllocator,
                                         const GpJsonMapperFlags                aFlags);

    static void         SWriteValueVec  (const GpTypeStructBase&                aStruct,
                                         const GpTypePropInfo&                  aPropInfo,
                                         rapidjson::Document::GenericValue&     aJsonObject,
                                         rapidjson::Document::AllocatorType&    aJsonAllocator,
                                         const GpJsonMapperFlags                aFlags);

    static void         SWriteValueList (const GpTypeStructBase&                aStruct,
                                         const GpTypePropInfo&                  aPropInfo,
                                         rapidjson::Document::GenericValue&     aJsonObject,
                                         rapidjson::Document::AllocatorType&    aJsonAllocator,
                                         const GpJsonMapperFlags                aFlags);

    static void         SWriteValueSet  (const GpTypeStructBase&                aStruct,
                                         const GpTypePropInfo&                  aPropInfo,
                                         rapidjson::Document::GenericValue&     aJsonObject,
                                         rapidjson::Document::AllocatorType&    aJsonAllocator,
                                         const GpJsonMapperFlags                aFlags);

    static void         SWriteValueMap  (const GpTypeStructBase&                aStruct,
                                         const GpTypePropInfo&                  aPropInfo,
                                         rapidjson::Document::GenericValue&     aJsonObject,
                                         rapidjson::Document::AllocatorType&    aJsonAllocator,
                                         const GpJsonMapperFlags                aFlags);
};

}//namespace GPlatform
