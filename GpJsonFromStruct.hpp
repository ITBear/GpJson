#pragma once

#include "GpJson_global.hpp"

#define RAPIDJSON_ASSERT(X) THROW_GPE_COND_CHECK_M(X, "Json processing error");

#include <rapidjson/document.h>

namespace GPlatform {

class GpJsonFromStruct
{
public:
    CLASS_REMOVE_CTRS(GpJsonFromStruct);

public:
    static std::string  SToString       (const rapidjson::Document& aJsonDOM);
    static void         SWrite          (const GpTypeStructBase&                aStruct,
                                         rapidjson::Document::GenericValue&     aJsonObject,
                                         rapidjson::Document::AllocatorType&    aJsonAllocator);

private:
    static void         SWriteValue     (const GpTypeStructBase&                aStruct,
                                         const GpTypePropInfo&                  aPropInfo,
                                         rapidjson::Document::GenericValue&     aJsonObject,
                                         rapidjson::Document::AllocatorType&    aJsonAllocator);

    static void         SWriteValueVec  (const GpTypeStructBase&                aStruct,
                                         const GpTypePropInfo&                  aPropInfo,
                                         rapidjson::Document::GenericValue&     aJsonObject,
                                         rapidjson::Document::AllocatorType&    aJsonAllocator);
};

}//namespace GPlatform
