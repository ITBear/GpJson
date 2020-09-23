#include "GpJsonMapper.hpp"
#include "GpJsonToStruct.hpp"
#include "GpJsonFromStruct.hpp"

namespace GPlatform {

GpTypeStructBase::SP    GpJsonMapper::SFromJsonInsitu (char* aJsonData)
{
    rapidjson::Document                 jsonDOM;
    rapidjson::Document::ConstObject    jsonObject  = GpJsonToStruct::SParseJsonDOM(aJsonData, jsonDOM);
    const GpTypeStructInfo&             structInfo  = GpJsonToStruct::SFindStructInfo(jsonObject);
    GpTypeStructBase::SP                resStruct   = structInfo.NewInstance();

    GpJsonToStruct::SReadStruct(resStruct.Vn(), jsonObject);

    return resStruct;
}

GpTypeStructBase::SP    GpJsonMapper::SFromJsonInsitu (char* aJsonData, const GpTypeStructInfo& aTypeStructInfo)
{
    rapidjson::Document                 jsonDOM;
    rapidjson::Document::ConstObject    jsonObject  = GpJsonToStruct::SParseJsonDOM(aJsonData, jsonDOM);
    const GpTypeStructInfo&             structInfo  = GpJsonToStruct::SCheckStructInfo(jsonObject, aTypeStructInfo, GpJsonToStruct::CheckMode::CAN_BE_DERIVED);
    GpTypeStructBase::SP                resStruct   = structInfo.NewInstance();

    GpJsonToStruct::SReadStruct(resStruct.Vn(), jsonObject);

    return resStruct;
}

std::string     GpJsonMapper::SToJson (const GpTypeStructBase& aStruct)
{
    rapidjson::Document jsonDOM;

    GpJsonFromStruct::SWrite(aStruct, jsonDOM.SetObject(), jsonDOM.GetAllocator());

    return GpJsonFromStruct::SToString(jsonDOM);
}

}//namespace GPlatform
