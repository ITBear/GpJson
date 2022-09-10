#pragma once

#include "GpJsonSerializerFlags.hpp"

#define RAPIDJSON_ASSERT(X) ::GPlatform::THROW_COND_GP(X, "Json processing error"_sv);

GP_WARNING_PUSH()
GP_WARNING_DISABLE(switch-enum)

#include <rapidjson/document.h>

GP_WARNING_POP()

namespace GPlatform {

class GP_JSON_API GpJsonToObject
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpJsonToObject);

public:
    static rapidjson::Document::ConstObject SParseJsonDom       (std::string_view       aJsonStr,
                                                                 rapidjson::Document&   aJsonDOM);
    static rapidjson::Document::ConstArray  SParseJsonDomVec    (std::string_view       aJsonStr,
                                                                 rapidjson::Document&   aJsonDOM);

    static rapidjson::Document::ConstObject SParseJsonDomInsitu (GpSpanPtrCharRW        aJsonStr,
                                                                 rapidjson::Document&   aJsonDOM);
    static void                             SReadObject         (GpReflectObject&                           aObject,
                                                                 const rapidjson::Document::ConstObject&    aJsonObject,
                                                                 const GpJsonSerializerFlags                aJsonSerializerFlags);
    static const GpReflectModel&            SCheckModel         (const rapidjson::Document::ConstObject&    aJsonObject,
                                                                     const GpReflectModel&                  aModel);
    static const GpReflectModel&            SCheckModel         (const rapidjson::Document::ConstObject&    aJsonObject,
                                                                 const GpVector<const GpReflectModel*>&     aModelVariants);

    static GpReflectModel::C::Opt::CRef     SFindModel          (const rapidjson::Document::ConstObject&    aJsonObject);

private:
    static std::optional<GpUUID>            SFindModelUid       (const rapidjson::Document::ConstObject&    aJsonObject);

private:
    static const std::array<std::string_view, 18>   sParseErrorCodes;
};

}//namespace GPlatform
