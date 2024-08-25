
#include <GpJson/GpJsonSerializerCtx.hpp>
#include <GpJson/GpJsonToObject.hpp>
#include <GpJson/GpJsonUtilsInternal.hpp>

GP_WARNING_PUSH()

#if defined(GP_COMPILER_CLANG) || defined(GP_COMPILER_GCC)
    GP_WARNING_DISABLE(switch-enum)
#endif// #if defined(GP_COMPILER_CLANG) || defined(GP_COMPILER_GCC)

#   include <rapidjson/document.h>

GP_WARNING_POP()

namespace GPlatform {

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

void    GpJsonSerializerCtx::Parse (GpSpanCharRW aJsonStr)
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

std::optional<std::string>  GpJsonSerializerCtx::FindMemberStr (const std::string& aName) const
{
    if (IsArray())
    {
        return std::nullopt;
    }

    const rapidjson::Document::ConstObject&     jsonObject  = *reinterpret_cast<const rapidjson::Document::ConstObject*>(RootAsObject());
    rapidjson::Document::ConstMemberIterator    iter        = jsonObject.FindMember(std::data(aName));

    if (iter != jsonObject.MemberEnd())
    {
        const auto& mitVal = iter->value;
        return std::string(_JsonValue2SV(mitVal));
    } else
    {
        return std::nullopt;
    }
}

const void* GpJsonSerializerCtx::RootAsObject (void) const
{
    THROW_COND_GP
    (
        iJsonRootValue != nullptr,
        "Json root value is null. Call Init first"_sv
    );

    THROW_COND_GP
    (
        iIsArray == false,
        "Json root value is array"_sv
    );

    return iJsonRootValue;
}

const void* GpJsonSerializerCtx::RootAsArray (void) const
{
    THROW_COND_GP
    (
        iJsonRootValue != nullptr,
        "Json root value is null. Call Init first"_sv
    );

    THROW_COND_GP
    (
        iIsArray == true,
        "Json root value is object"_sv
    );

    return iJsonRootValue;
}

}// namespace GPlatform
