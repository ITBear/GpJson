#include "GpJsonMapper.hpp"
#include "GpJsonToStruct.hpp"
#include "GpJsonFromStruct.hpp"

namespace GPlatform {

GpTypeStructBase::SP    GpJsonMapper::SFromJson
(
    GpRawPtrCharR               aJsonData,
    const GpJsonMapperFlags&    aFlags
)
{
    rapidjson::Document                 jsonDOM;
    rapidjson::Document::ConstObject    jsonObject      = GpJsonToStruct::SParseJsonDom(aJsonData, jsonDOM);
    GpJsonToStruct::FindTypeInfoResT    typeInfoFindRes = GpJsonToStruct::SFindTypeInfo(jsonObject, aFlags);

    if (!typeInfoFindRes.has_value())
    {
        THROW_GPE("Json member '@' was not found"_sv);
    }

    const GpTypeStructInfo& typeInfo    = typeInfoFindRes.value().get();
    GpTypeStructBase::SP    resStruct   = typeInfo.NewInstance();

    GpJsonToStruct::SReadStruct(resStruct.Vn(), jsonObject, aFlags);

    return resStruct;
}

GpTypeStructBase::SP    GpJsonMapper::SFromJson
(
    GpRawPtrCharR               aJsonData,
    const GpTypeStructInfo&     aTypeInfo,
    const GpJsonMapperFlags&    aFlags
)
{
    rapidjson::Document                 jsonDOM;
    rapidjson::Document::ConstObject    jsonObject  = GpJsonToStruct::SParseJsonDom(aJsonData, jsonDOM);
    const GpTypeStructInfo&             typeInfo    = GpJsonToStruct::SCheckTypeInfo(jsonObject,
                                                                                     aTypeInfo,
                                                                                     GpJsonToStruct::CheckMode::CAN_BE_DERIVED,
                                                                                     aFlags);
    GpTypeStructBase::SP                resStruct   = typeInfo.NewInstance();

    GpJsonToStruct::SReadStruct(resStruct.Vn(), jsonObject, aFlags);

    return resStruct;
}

void    GpJsonMapper::SFromJson
(
    GpRawPtrCharR               aJsonData,
    GpTypeStructBase&           aOut,
    const GpJsonMapperFlags&    aFlags
)
{
    rapidjson::Document                 jsonDOM;
    rapidjson::Document::ConstObject    jsonObject  = GpJsonToStruct::SParseJsonDom(aJsonData, jsonDOM);

    GpJsonToStruct::SReadStruct(aOut, jsonObject, aFlags);
}

GpTypeStructBase::SP    GpJsonMapper::SFromJsonInsitu
(
    GpRawPtrCharRW              aJsonData,
    const GpJsonMapperFlags&    aFlags
)
{
    rapidjson::Document                 jsonDOM;
    rapidjson::Document::ConstObject    jsonObject      = GpJsonToStruct::SParseJsonDomInsitu(aJsonData, jsonDOM);
    GpJsonToStruct::FindTypeInfoResT    typeInfoFindRes = GpJsonToStruct::SFindTypeInfo(jsonObject, aFlags);

    if (!typeInfoFindRes.has_value())
    {
        THROW_GPE("Json member '@' was not found"_sv);
    }

    const GpTypeStructInfo& typeInfo    = typeInfoFindRes.value().get();
    GpTypeStructBase::SP    resStruct   = typeInfo.NewInstance();

    GpJsonToStruct::SReadStruct(resStruct.Vn(), jsonObject, aFlags);

    return resStruct;
}

GpTypeStructBase::SP    GpJsonMapper::SFromJsonInsitu
(
    GpRawPtrCharRW              aJsonData,
    const GpTypeStructInfo&     aTypeInfo,
    const GpJsonMapperFlags&    aFlags
)
{
    rapidjson::Document                 jsonDOM;
    rapidjson::Document::ConstObject    jsonObject  = GpJsonToStruct::SParseJsonDomInsitu(aJsonData, jsonDOM);
    const GpTypeStructInfo&             typeInfo    = GpJsonToStruct::SCheckTypeInfo(jsonObject,
                                                                                     aTypeInfo,
                                                                                     GpJsonToStruct::CheckMode::CAN_BE_DERIVED,
                                                                                     aFlags);
    GpTypeStructBase::SP                resStruct   = typeInfo.NewInstance();

    GpJsonToStruct::SReadStruct(resStruct.Vn(), jsonObject, aFlags);

    return resStruct;
}

void    GpJsonMapper::SFromJsonInsitu
(
    GpRawPtrCharRW              aJsonData,
    GpTypeStructBase&           aOut,
    const GpJsonMapperFlags&    aFlags
)
{
    rapidjson::Document                 jsonDOM;
    rapidjson::Document::ConstObject    jsonObject  = GpJsonToStruct::SParseJsonDomInsitu(aJsonData, jsonDOM);

    GpJsonToStruct::SReadStruct(aOut, jsonObject, aFlags);
}

std::string GpJsonMapper::SToJson
(
    const GpTypeStructBase&     aStruct,
    const GpJsonMapperFlags&    aFlags
)
{
    rapidjson::Document jsonDOM;

    GpJsonFromStruct::SWrite(aStruct, jsonDOM.SetObject(), jsonDOM.GetAllocator(), aFlags, false);
    return GpJsonFromStruct::SToString(jsonDOM);
}

void    GpJsonMapper::SToJson
(
    const GpTypeStructBase&     aStruct,
    GpByteWriter&               aWriter,
    const GpJsonMapperFlags&    aFlags
)
{
    rapidjson::Document jsonDOM;

    GpJsonFromStruct::SWrite(aStruct, jsonDOM.SetObject(), jsonDOM.GetAllocator(), aFlags, false);
    GpJsonFromStruct::SToString(jsonDOM, aWriter);
}

}//namespace GPlatform
