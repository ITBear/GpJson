#pragma once

#include "GpJson_global.hpp"

#define RAPIDJSON_ASSERT(X) THROW_GPE_COND_CHECK_M(X, "Json processing error");

GP_WARNING_PUSH()
GP_WARNING_DISABLE(switch-enum)

#include <rapidjson/document.h>

GP_WARNING_POP()

namespace GPlatform {

class GPJSON_API GpJsonToStruct
{
public:
    CLASS_REMOVE_CTRS(GpJsonToStruct);

    enum class CheckMode
    {
        MUST_BE_EQUAL,
        CAN_BE_DERIVED
    };

    using FindTypeInfoResT = std::optional<std::reference_wrapper<const GpTypeStructInfo>>;

public:
    static rapidjson::Document::ConstObject SParseJsonDom       (GpRawPtrCharR          aJsonData,
                                                                 rapidjson::Document&   aJsonDOM);
    static rapidjson::Document::ConstObject SParseJsonDomInsitu (GpRawPtrCharRW         aJsonData,
                                                                 rapidjson::Document&   aJsonDOM);
    static void                             SReadStruct         (GpTypeStructBase&                          aStruct,
                                                                 const rapidjson::Document::ConstObject&    aJsonObject);
    static const GpTypeStructInfo&          SCheckTypeInfo      (const rapidjson::Document::ConstObject&    aJonObject,
                                                                 const GpTypeStructInfo&                    aTypeInfoBase,
                                                                 const CheckMode                            aCheckMode);
    static FindTypeInfoResT                 SFindTypeInfo       (const rapidjson::Document::ConstObject&    aJsonObject);

private:
    static void                             SReadValue          (GpTypeStructBase&                          aStruct,
                                                                 const GpTypePropInfo&                      aPropInfo,
                                                                 const rapidjson::Document::ConstObject&    aJsonObject);

    static void                             SReadValueVec       (GpTypeStructBase&                          aStruct,
                                                                 const GpTypePropInfo&                      aPropInfo,
                                                                 const rapidjson::Document::ConstObject&    aJsonObject);

    static void                             SReadValueList      (GpTypeStructBase&                          aStruct,
                                                                 const GpTypePropInfo&                      aPropInfo,
                                                                 const rapidjson::Document::ConstObject&    aJsonObject);

    static void                             SReadValueSet       (GpTypeStructBase&                          aStruct,
                                                                 const GpTypePropInfo&                      aPropInfo,
                                                                 const rapidjson::Document::ConstObject&    aJsonObject);

    static void                             SReadValueMap       (GpTypeStructBase&                          aStruct,
                                                                 const GpTypePropInfo&                      aPropInfo,
                                                                 const rapidjson::Document::ConstObject&    aJsonObject);

private:
    static const std::array<std::string_view, 18>   sParseErrorCodes;
};

}//namespace GPlatform
