#include "GpJsonMapper.hpp"
#include "GpJsonToStruct.hpp"
#include "GpJsonFromStruct.hpp"
#include "GpJsonMapperTypeDetector.hpp"

namespace GPlatform {

GpTypeStructBase::SP    GpJsonMapper::SFromJsonInsitu (GpRawPtrCharRW aJsonData)
{
    rapidjson::Document                 jsonDOM;
    rapidjson::Document::ConstObject    jsonObject      = GpJsonToStruct::SParseJsonDOM(aJsonData, jsonDOM);
    GpJsonToStruct::FindTypeInfoResT    typeInfoFindRes = GpJsonToStruct::SFindTypeInfo(jsonObject);

    if (!typeInfoFindRes.has_value())
    {
        THROW_GPE("Json member '@' was not found"_sv);
    }

    const GpTypeStructInfo& typeInfo    = typeInfoFindRes.value().get();
    GpTypeStructBase::SP    resStruct   = typeInfo.NewInstance();

    GpJsonToStruct::SReadStruct(resStruct.Vn(), jsonObject);

    return resStruct;
}

GpTypeStructBase::SP    GpJsonMapper::SFromJsonInsitu (GpRawPtrCharRW aJsonData, const GpTypeStructInfo& aTypeInfo)
{
    rapidjson::Document                 jsonDOM;
    rapidjson::Document::ConstObject    jsonObject  = GpJsonToStruct::SParseJsonDOM(aJsonData, jsonDOM);
    const GpTypeStructInfo&             typeInfo    = GpJsonToStruct::SCheckTypeInfo(jsonObject,
                                                                                     aTypeInfo,
                                                                                     GpJsonToStruct::CheckMode::CAN_BE_DERIVED);
    GpTypeStructBase::SP                resStruct   = typeInfo.NewInstance();

    GpJsonToStruct::SReadStruct(resStruct.Vn(), jsonObject);

    return resStruct;
}

GpTypeStructBase::SP    GpJsonMapper::SFromJsonInsitu (GpRawPtrCharRW aJsonData, const GpJsonMapperTypeDetector& aTypeDetector)
{
    rapidjson::Document                 jsonDOM;
    rapidjson::Document::ConstObject    jsonObject  = GpJsonToStruct::SParseJsonDOM(aJsonData, jsonDOM);
    const GpTypeStructInfo&             typeInfo    = aTypeDetector.DetectTypeInfo(jsonObject);
    GpTypeStructBase::SP                resStruct   = typeInfo.NewInstance();

    GpJsonToStruct::SReadStruct(resStruct.Vn(), jsonObject);

    return resStruct;
}

void    GpJsonMapper::SFromJsonInsitu (GpRawPtrCharRW aJsonData, GpTypeStructBase& aOut)
{
    rapidjson::Document                 jsonDOM;
    rapidjson::Document::ConstObject    jsonObject  = GpJsonToStruct::SParseJsonDOM(aJsonData, jsonDOM);

    GpJsonToStruct::SReadStruct(aOut, jsonObject);
}

std::string     GpJsonMapper::SToJson (const GpTypeStructBase&  aStruct,
                                       const GpJsonMapperFlags  aFlags)
{
    rapidjson::Document jsonDOM;

    GpJsonFromStruct::SWrite(aStruct, jsonDOM.SetObject(), jsonDOM.GetAllocator(), aFlags);
    return GpJsonFromStruct::SToString(jsonDOM);
}

void    GpJsonMapper::SToJson (const GpTypeStructBase&  aStruct,
                               GpByteWriter&            aWriter,
                               const GpJsonMapperFlags  aFlags)
{
    rapidjson::Document jsonDOM;

    GpJsonFromStruct::SWrite(aStruct, jsonDOM.SetObject(), jsonDOM.GetAllocator(), aFlags);
    GpJsonFromStruct::SToString(jsonDOM, aWriter);
}

}//namespace GPlatform
