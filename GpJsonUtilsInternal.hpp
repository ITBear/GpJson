#pragma once

#include <string>
#include <string_view>

#include <GpCore2/GpUtils/Types/Strings/GpUTF.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpStringOps.hpp>
#include <GpCore2/GpUtils/Macro/GpMacroWarnings.hpp>

#define RAPIDJSON_ASSERT(X) ::GPlatform::THROW_COND_GP(X, "Json processing error"_sv);

GP_WARNING_PUSH()

#if defined(GP_COMPILER_CLANG) || defined(GP_COMPILER_GCC)
    GP_WARNING_DISABLE(switch-enum)
#endif// #if defined(GP_COMPILER_CLANG) || defined(GP_COMPILER_GCC)

#   include <rapidjson/document.h>

GP_WARNING_POP()

namespace GPlatform {

inline std::string_view _JsonValue2SV (const rapidjson::Document::GenericValue& aJsonValue)
{
    return std::string_view(aJsonValue.GetString(), aJsonValue.GetStringLength());
}

inline std::string_view _JsonValue2SV (const rapidjson::Document::GenericValue* aJsonValue)
{
    return std::string_view(aJsonValue->GetString(), aJsonValue->GetStringLength());
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
        std::data(aValue),
        NumOps::SConvert<rapidjson::SizeType>(aValue.length()),
        aJsonAllocator
    );
}

}// namespace GPlatform
