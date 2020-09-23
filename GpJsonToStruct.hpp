#pragma once

#include "GpJson_global.hpp"

#define RAPIDJSON_ASSERT(X) THROW_GPE_COND_CHECK_M(X, "Json processing error");

#include <rapidjson/document.h>

namespace GPlatform {

class GpJsonToStruct
{
public:
    CLASS_REMOVE_CTRS(GpJsonToStruct);

    enum class CheckMode
    {
        MUST_BE_EQUAL,
        CAN_BE_DERIVED
    };

public:
    static rapidjson::Document::ConstObject SParseJsonDOM       (char*                  aJsonData,
                                                                 rapidjson::Document&   aJsonDOM);
    static const GpTypeStructInfo&          SFindStructInfo     (const rapidjson::Document::ConstObject&    aJsonObject);
    static const GpTypeStructInfo&          SCheckStructInfo    (const rapidjson::Document::ConstObject&    aJonObject,
                                                                 const GpTypeStructInfo&                    aStructInfoBase,
                                                                 const CheckMode                            aCheckMode);
    static void                             SReadStruct         (GpTypeStructBase&                          aStruct,
                                                                 const rapidjson::Document::ConstObject&    aJsonObject);

private:
    static void                             SReadValue          (GpTypeStructBase&                          aStruct,
                                                                 const GpTypePropInfo&                      aPropInfo,
                                                                 const rapidjson::Document::ConstObject&    aJsonObject);

    static void                             SReadValueVec       (GpTypeStructBase&                          aStruct,
                                                                 const GpTypePropInfo&                      aPropInfo,
                                                                 const rapidjson::Document::ConstObject&    aJsonObject);

private:
    static const std::array<std::string_view, 18>   sParseErrorCodes;
};

}//namespace GPlatform
