#pragma once

#include "GpJsonSerializerFlags.hpp"
#include <GpCore2/GpUtils/Macro/GpMacroWarnings.hpp>
#include <GpCore2/GpUtils/Streams/GpByteWriter.hpp>

#define RAPIDJSON_ASSERT(X) ::GPlatform::THROW_COND_GP(X, "Json processing error"_sv);

GP_WARNING_PUSH()
GP_WARNING_DISABLE(switch-enum)

#include <rapidjson/document.h>

GP_WARNING_POP()

namespace GPlatform {

class GpReflectObject;

class GpJsonFromObject
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpJsonFromObject)

public:
    static std::u8string    SToString       (const rapidjson::Document& aJsonDOM);
    static void             SToString       (const rapidjson::Document& aJsonDOM,
                                             GpByteWriter&              aWriter);

    static void             SWrite          (const GpReflectObject&                 aObject,
                                             rapidjson::Document::GenericValue&     aJsonObject,
                                             rapidjson::Document::AllocatorType&    aJsonAllocator,
                                             const GpJsonSerializerFlags            aJsonSerializerFlags,
                                             const bool                             aIsAsTuple);
};

}//namespace GPlatform
