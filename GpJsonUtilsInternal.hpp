#pragma once

#include <string>
#include <string_view>

#include <GpCore2/GpUtils/Types/Strings/GpUTF.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpStringOps.hpp>
#include <GpCore2/GpUtils/Macro/GpMacroWarnings.hpp>

#define RAPIDJSON_ASSERT(X) ::GPlatform::THROW_COND_GP(X, "Json processing error"_sv);

GP_WARNING_PUSH()
GP_WARNING_DISABLE(switch-enum)

#include <rapidjson/document.h>

GP_WARNING_POP()

namespace GPlatform {

inline std::u8string_view   _JsonValue2SV (const rapidjson::Document::GenericValue& aJsonValue)
{
    return GpUTF::S_As_UTF8(std::string_view(aJsonValue.GetString(), aJsonValue.GetStringLength()));
}

inline std::u8string_view   _JsonValue2SV (const rapidjson::Document::GenericValue* aJsonValue)
{
    return GpUTF::S_As_UTF8(std::string_view(aJsonValue->GetString(), aJsonValue->GetStringLength()));
}

inline void _JsonSetStr
(
    rapidjson::Value&                   aObj,
    std::string_view                    aValue,
    rapidjson::Document::AllocatorType& aJsonAllocator
)
{
    aObj.SetString
    (
        aValue.data(),
        NumOps::SConvert<rapidjson::SizeType>(aValue.length()),
        aJsonAllocator
    );
}

inline void _JsonSetStr
(
    rapidjson::Value&                   aObj,
    std::u8string_view                  aValue,
    rapidjson::Document::AllocatorType& aJsonAllocator
)
{
    aObj.SetString
    (
        ::GPlatform::GpUTF::S_As_STR(aValue).data(),
        NumOps::SConvert<rapidjson::SizeType>(aValue.length()),
        aJsonAllocator
    );
}

}//namespace GPlatform
