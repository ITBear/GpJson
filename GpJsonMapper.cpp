#include "GpJsonMapper.hpp"
#include "GpJsonToStruct.hpp"
#include "GpJsonFromStruct.hpp"
#include "GpJsonMapperTypeDetector.hpp"

namespace GPlatform {

GpTypeStructBase::SP    GpJsonMapper::SFromJsonInsitu (GpRawPtrCharRW aJsonData)
{
    rapidjson::Document                 jsonDOM;
    rapidjson::Document::ConstObject    jsonObject          = GpJsonToStruct::SParseJsonDOM(aJsonData, jsonDOM);
    GpJsonToStruct::FindStructInfoResT  structInfoFindRes   = GpJsonToStruct::SFindStructInfo(jsonObject);

    if (!structInfoFindRes.has_value())
    {
        THROW_GPE("Json member '@' was not found"_sv);
    }

    const GpTypeStructInfo&             structInfo  = structInfoFindRes.value().get();
    GpTypeStructBase::SP                resStruct   = structInfo.NewInstance();

    GpJsonToStruct::SReadStruct(resStruct.Vn(), jsonObject);

    return resStruct;
}

GpTypeStructBase::SP    GpJsonMapper::SFromJsonInsitu (GpRawPtrCharRW aJsonData, const GpTypeStructInfo& aTypeStructInfo)
{
    rapidjson::Document                 jsonDOM;
    rapidjson::Document::ConstObject    jsonObject  = GpJsonToStruct::SParseJsonDOM(aJsonData, jsonDOM);
    const GpTypeStructInfo&             structInfo  = GpJsonToStruct::SCheckStructInfo(jsonObject,
                                                                                       aTypeStructInfo,
                                                                                       GpJsonToStruct::CheckMode::CAN_BE_DERIVED);
    GpTypeStructBase::SP                resStruct   = structInfo.NewInstance();

    GpJsonToStruct::SReadStruct(resStruct.Vn(), jsonObject);

    return resStruct;
}

GpTypeStructBase::SP    GpJsonMapper::SFromJsonInsitu (GpRawPtrCharRW aJsonData, const GpJsonMapperTypeDetector& aTypeDetector)
{
    rapidjson::Document                 jsonDOM;
    rapidjson::Document::ConstObject    jsonObject  = GpJsonToStruct::SParseJsonDOM(aJsonData, jsonDOM);
    const GpTypeStructInfo&             structInfo  = aTypeDetector.DetectTypeStructInfo(jsonObject);
    GpTypeStructBase::SP                resStruct   = structInfo.NewInstance();

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
