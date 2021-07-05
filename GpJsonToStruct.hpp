#pragma once

#include "GpJsonMapperFlags.hpp"

#define RAPIDJSON_ASSERT(X) ::GPlatform::THROW_GPE_COND(X, "Json processing error"_sv);

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
                                                                 const rapidjson::Document::ConstObject&    aJsonObject,
                                                                 const GpJsonMapperFlags&                   aJsonMapperFlags);
    static const GpTypeStructInfo&          SCheckTypeInfo      (const rapidjson::Document::ConstObject&    aJsonObject,
                                                                 const GpTypeStructInfo&                    aTypeInfoBase,
                                                                 const CheckMode                            aCheckMode,
                                                                 const GpJsonMapperFlags&                   aJsonMapperFlags);
    static FindTypeInfoResT                 SFindTypeInfo       (const rapidjson::Document::ConstObject&    aJsonObject,
                                                                 const GpJsonMapperFlags&                   aJsonMapperFlags);

private:
    static void                             SReadValue          (GpTypeStructBase&                          aStruct,
                                                                 const GpTypePropInfo&                      aPropInfo,
                                                                 const rapidjson::Document::ConstObject&    aJsonObject,
                                                                 const GpJsonMapperFlags&                   aJsonMapperFlags);

    static void                             SReadValueVec       (GpTypeStructBase&                          aStruct,
                                                                 const GpTypePropInfo&                      aPropInfo,
                                                                 const rapidjson::Document::ConstObject&    aJsonObject,
                                                                 const GpJsonMapperFlags&                   aJsonMapperFlags);

    static void                             SReadValueList      (GpTypeStructBase&                          aStruct,
                                                                 const GpTypePropInfo&                      aPropInfo,
                                                                 const rapidjson::Document::ConstObject&    aJsonObject,
                                                                 const GpJsonMapperFlags&                   aJsonMapperFlags);

    static void                             SReadValueSet       (GpTypeStructBase&                          aStruct,
                                                                 const GpTypePropInfo&                      aPropInfo,
                                                                 const rapidjson::Document::ConstObject&    aJsonObject,
                                                                 const GpJsonMapperFlags&                   aJsonMapperFlags);

    static void                             SReadValueMap       (GpTypeStructBase&                          aStruct,
                                                                 const GpTypePropInfo&                      aPropInfo,
                                                                 const rapidjson::Document::ConstObject&    aJsonObject,
                                                                 const GpJsonMapperFlags&                   aJsonMapperFlags);

private:
    static const std::array<std::string_view, 18>   sParseErrorCodes;
};

}//namespace GPlatform
