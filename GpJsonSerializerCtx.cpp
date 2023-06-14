#include "GpJsonSerializerCtx.hpp"
#include "GpJsonToObject.hpp"

GP_WARNING_PUSH()
GP_WARNING_DISABLE(switch-enum)

#include <rapidjson/document.h>

GP_WARNING_POP()

namespace GPlatform {

std::u8string_view  _JsonValue2SV (const rapidjson::Document::GenericValue& aJsonValue)
{
    return GpUTF::S_STR_To_UTF8(std::string_view(aJsonValue.GetString(), aJsonValue.GetStringLength()));
}

std::u8string_view  _JsonValue2SV (const rapidjson::Document::GenericValue* aJsonValue)
{
    return GpUTF::S_STR_To_UTF8(std::string_view(aJsonValue->GetString(), aJsonValue->GetStringLength()));
}

GpJsonSerializerCtx::~GpJsonSerializerCtx (void) noexcept
{
    Clear();
}

void    GpJsonSerializerCtx::Clear (void) noexcept
{
    if (iJsonDOM)
    {
        delete static_cast<rapidjson::Document*>(iJsonDOM);
        iJsonDOM = nullptr;
    }

    if (iJsonRootValue)
    {
        if (iIsArray)
        {
            delete static_cast<rapidjson::Document::ConstArray*>(iJsonRootValue);
        } else
        {
            delete static_cast<rapidjson::Document::ConstObject*>(iJsonRootValue);
        }

        iJsonRootValue = nullptr;
    }
}

void    GpJsonSerializerCtx::Init (GpSpanPtrCharRW aJsonStr)
{
    Clear();

    iJsonDOM = new rapidjson::Document();

    GpJsonToObject::ParseResT parseRes = GpJsonToObject::SParseJsonDomInsitu
    (
        aJsonStr,
        *static_cast<rapidjson::Document*>(iJsonDOM)
    );

    if (std::holds_alternative<rapidjson::Document::ConstObject>(parseRes))
    {
        iIsArray = false;

        iJsonRootValue = new rapidjson::Document::ConstObject
        (
            std::get<rapidjson::Document::ConstObject>(parseRes)
        );
    } else if (std::holds_alternative<rapidjson::Document::ConstArray>(parseRes))
    {
        iIsArray = true;

        iJsonRootValue = new rapidjson::Document::ConstArray
        (
            std::get<rapidjson::Document::ConstArray>(parseRes)
        );
    }
}

std::optional<std::u8string>    GpJsonSerializerCtx::FindMemberStr (const std::u8string& aName)
{
    if (IsArray())
    {
        return std::nullopt;
    }

    const rapidjson::Document::ConstObject&     jsonObject  = RootValue<rapidjson::Document::ConstObject>();
    rapidjson::Document::ConstMemberIterator    iter        = jsonObject.FindMember(GpUTF::S_UTF8_To_STR(aName).data());

    if (iter != jsonObject.MemberEnd())
    {
        const auto& mitVal = iter->value;
        return std::u8string(_JsonValue2SV(mitVal));
    } else
    {
        return std::nullopt;
    }
}

}//namespace GPlatform
