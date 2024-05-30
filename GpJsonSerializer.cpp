#include "GpJsonSerializer.hpp"
#include "GpJsonToObject.hpp"
#include "GpJsonFromObject.hpp"
#include "GpJsonSerializerCtx.hpp"

namespace GPlatform {

GpJsonSerializer::~GpJsonSerializer (void) noexcept
{
}

GpReflectObject::SP GpJsonSerializer::ToObject (GpSpanByteR aData) const
{
    return SFromStr(aData.AsStringView(), iFlags);
}

GpReflectObject::SP GpJsonSerializer::ToObject
(
    GpSpanByteR             aData,
    const GpReflectModel&   aModel
) const
{
    return SFromStr(aData.AsStringView(), aModel, iFlags);
}

GpReflectObject::SP GpJsonSerializer::ToObject
(
    GpSpanByteR                                 aData,
    const std::vector<const GpReflectModel*>&   aModelVariants
) const
{
    return SFromStr(aData.AsStringView(), aModelVariants, iFlags);
}

GpReflectObject::SP GpJsonSerializer::ToObject
(
    GpReflectSerializerCtx&     aCtx,
    const GpReflectModel&       aModel
) const
{
    const GpJsonSerializerCtx& ctx = static_cast<const GpJsonSerializerCtx&>(aCtx);

    if (ctx.IsArray())
    {
        return aModel.NewInstance();
    }

    const rapidjson::Document::ConstObject& jsonObject  = *reinterpret_cast<const rapidjson::Document::ConstObject*>(ctx.RootAsObject());
    const GpReflectModel&                   model       = GpJsonToObject::SCheckModel
    (
        jsonObject,
        aModel
    );

    GpReflectObject::SP object = model.NewInstance();

    GpJsonToObject::SReadObject(object.Vn(), jsonObject, iFlags);

    return object;
}

void    GpJsonSerializer::FromObject
(
    const GpReflectObject&  aObject,
    GpByteWriter&           aWriter
) const
{
    SToStr(aObject, aWriter, iFlags);
}

GpReflectObject::SP GpJsonSerializer::SFromStr
(
    std::string_view            aJsonStr,
    const GpJsonSerializerFlags aFlags
)
{
    rapidjson::Document                 jsonDOM;
    rapidjson::Document::ConstObject    jsonObject  = GpJsonToObject::SParseJsonDom(aJsonStr, jsonDOM);
    GpReflectModel::C::Opt::CRef        modelOpt    = GpJsonToObject::SFindModel(jsonObject);

    THROW_COND_GP
    (
        modelOpt.has_value(),
        "Json member '@' was not found"_sv
    );

    const GpReflectModel&   model   = modelOpt.value();
    GpReflectObject::SP     object  = model.NewInstance();

    GpJsonToObject::SReadObject(object.Vn(), jsonObject, aFlags);

    return object;
}

GpReflectObject::SP GpJsonSerializer::SFromStr
(
    std::string_view            aJsonStr,
    const GpReflectModel&       aModel,
    const GpJsonSerializerFlags aFlags
)
{
    rapidjson::Document                 jsonDOM;
    rapidjson::Document::ConstObject    jsonObject  = GpJsonToObject::SParseJsonDom(aJsonStr, jsonDOM);
    const GpReflectModel&               model       = GpJsonToObject::SCheckModel
    (
        jsonObject,
        aModel
    );

    GpReflectObject::SP object = model.NewInstance();

    GpJsonToObject::SReadObject(object.Vn(), jsonObject, aFlags);

    return object;
}

GpReflectObject::SP GpJsonSerializer::SFromStr
(
    std::string_view                            aJsonStr,
    const std::vector<const GpReflectModel*>&   aModelVariants,
    const GpJsonSerializerFlags                 aFlags
)
{
    rapidjson::Document                 jsonDOM;
    rapidjson::Document::ConstObject    jsonObject  = GpJsonToObject::SParseJsonDom(aJsonStr, jsonDOM);
    const GpReflectModel&               model       = GpJsonToObject::SCheckModel
    (
        jsonObject,
        aModelVariants
    );

    GpReflectObject::SP object = model.NewInstance();

    GpJsonToObject::SReadObject(object.Vn(), jsonObject, aFlags);

    return object;
}

void    GpJsonSerializer::SFromStr
(
    std::string_view            aJsonStr,
    GpReflectObject&            aOut,
    const GpJsonSerializerFlags aFlags
)
{
    rapidjson::Document                 jsonDOM;
    rapidjson::Document::ConstObject    jsonObject  = GpJsonToObject::SParseJsonDom(aJsonStr, jsonDOM);

    GpJsonToObject::SReadObject(aOut, jsonObject, aFlags);
}

GpReflectObject::C::Vec::SP GpJsonSerializer::SFromStrVec
(
    std::string_view            aJsonStr,
    const GpJsonSerializerFlags aFlags
)
{
    rapidjson::Document             jsonDOM;
    rapidjson::Document::ConstArray jsonArray = GpJsonToObject::SParseJsonDomVec(aJsonStr, jsonDOM);
    GpReflectObject::C::Vec::SP res;
    res.reserve(jsonArray.Size());

    for (const auto& jsonArrayElement: jsonArray)
    {
        GpReflectModel::C::Opt::CRef modelOpt = GpJsonToObject::SFindModel(jsonArrayElement.GetObject());

        THROW_COND_GP
        (
            modelOpt.has_value(),
            "Json member '@' was not found"_sv
        );

        const GpReflectModel&   model   = modelOpt.value();
        GpReflectObject::SP     object  = model.NewInstance();

        GpJsonToObject::SReadObject(object.Vn(), jsonArrayElement.GetObject(), aFlags);

        res.emplace_back(object);
    }

    return res;
}

GpReflectObject::C::Vec::SP GpJsonSerializer::SFromStrVec
(
    std::string_view            aJsonStr,
    const GpReflectModel&       aModel,
    const GpJsonSerializerFlags aFlags
)
{
    rapidjson::Document             jsonDOM;
    rapidjson::Document::ConstArray jsonArray = GpJsonToObject::SParseJsonDomVec(aJsonStr, jsonDOM);
    GpReflectObject::C::Vec::SP res;
    res.reserve(jsonArray.Size());

    for (const auto& jsonArrayElement: jsonArray)
    {
        const GpReflectModel& model = GpJsonToObject::SCheckModel
        (
            jsonArrayElement.GetObject(),
            aModel
        );

        GpReflectObject::SP object = model.NewInstance();
        GpJsonToObject::SReadObject(object.Vn(), jsonArrayElement.GetObject(), aFlags);

        res.emplace_back(object);
    }

    return res;
}

GpReflectObject::C::Vec::SP GpJsonSerializer::SFromStrVec
(
    std::string_view                            aJsonStr,
    const std::vector<const GpReflectModel*>&   aModelVariants,
    const GpJsonSerializerFlags                 aFlags
)
{
    rapidjson::Document             jsonDOM;
    rapidjson::Document::ConstArray jsonArray = GpJsonToObject::SParseJsonDomVec(aJsonStr, jsonDOM);
    GpReflectObject::C::Vec::SP res;
    res.reserve(jsonArray.Size());

    for (const auto& jsonArrayElement: jsonArray)
    {
        const GpReflectModel& model = GpJsonToObject::SCheckModel
        (
            jsonArrayElement.GetObject(),
            aModelVariants
        );

        GpReflectObject::SP object = model.NewInstance();
        GpJsonToObject::SReadObject(object.Vn(), jsonArrayElement.GetObject(), aFlags);

        res.emplace_back(object);
    }

    return res;
}

GpReflectObject::SP GpJsonSerializer::SFromStrInsitu
(
    GpSpanCharRW                aJsonStr,
    const GpJsonSerializerFlags aFlags
)
{
    rapidjson::Document         jsonDOM;
    GpJsonToObject::ParseResT   parseRes    = GpJsonToObject::SParseJsonDomInsitu(aJsonStr, jsonDOM);

    THROW_COND_GP
    (
        std::holds_alternative<rapidjson::Document::ConstObject>(parseRes),
        "Json root must be object, not array"_sv
    );

    rapidjson::Document::ConstObject    jsonObject  = std::get<rapidjson::Document::ConstObject>(parseRes);
    GpReflectModel::C::Opt::CRef        modelOpt    = GpJsonToObject::SFindModel(jsonObject);

    THROW_COND_GP
    (
        modelOpt.has_value(),
        "Json member '@' was not found"_sv
    );

    const GpReflectModel&   model   = modelOpt.value();
    GpReflectObject::SP     object  = model.NewInstance();

    GpJsonToObject::SReadObject(object.Vn(), jsonObject, aFlags);

    return object;
}

GpReflectObject::SP GpJsonSerializer::SFromStrInsitu
(
    GpSpanCharRW                aJsonStr,
    const GpReflectModel&       aModel,
    const GpJsonSerializerFlags aFlags
)
{
    rapidjson::Document         jsonDOM;
    GpJsonToObject::ParseResT   parseRes    = GpJsonToObject::SParseJsonDomInsitu(aJsonStr, jsonDOM);

    THROW_COND_GP
    (
        std::holds_alternative<rapidjson::Document::ConstObject>(parseRes),
        "Json root must be object, not array"_sv
    );

    rapidjson::Document::ConstObject    jsonObject  = std::get<rapidjson::Document::ConstObject>(parseRes);
    const GpReflectModel&               model       = GpJsonToObject::SCheckModel
    (
        jsonObject,
        aModel
    );

    GpReflectObject::SP object = model.NewInstance();

    GpJsonToObject::SReadObject(object.Vn(), jsonObject, aFlags);

    return object;
}

void    GpJsonSerializer::SFromStrInsitu
(
    GpSpanCharRW                aJsonStr,
    GpReflectObject&            aOut,
    const GpJsonSerializerFlags aFlags
)
{
    rapidjson::Document         jsonDOM;
    GpJsonToObject::ParseResT   parseRes    = GpJsonToObject::SParseJsonDomInsitu(aJsonStr, jsonDOM);

    THROW_COND_GP
    (
        std::holds_alternative<rapidjson::Document::ConstObject>(parseRes),
        "Json root must be object, not array"_sv
    );

    rapidjson::Document::ConstObject    jsonObject  = std::get<rapidjson::Document::ConstObject>(parseRes);

    GpJsonToObject::SReadObject(aOut, jsonObject, aFlags);
}

std::string GpJsonSerializer::SToStr
(
    const GpReflectObject&      aObject,
    const GpJsonSerializerFlags aFlags
)
{
    rapidjson::Document jsonDOM;    

    GpJsonFromObject::SWrite(aObject, jsonDOM.SetObject(), jsonDOM.GetAllocator(), aFlags, false);
    return GpJsonFromObject::SToString(jsonDOM);
}

void    GpJsonSerializer::SToStr
(
    const GpReflectObject&      aObject,
    GpByteWriter&               aWriter,
    const GpJsonSerializerFlags aFlags
)
{
    rapidjson::Document jsonDOM;

    GpJsonFromObject::SWrite(aObject, jsonDOM.SetObject(), jsonDOM.GetAllocator(), aFlags, false);
    GpJsonFromObject::SToString(jsonDOM, aWriter);
}

}// namespace GPlatform
