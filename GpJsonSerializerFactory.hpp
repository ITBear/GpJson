#pragma once

#include "GpJson_global.hpp"
#include "GpJsonSerializerFlags.hpp"

#include <GpCore2/GpReflection/Serializers/GpReflectSerializerFactory.hpp>

namespace GPlatform {

class GP_JSON_API GpJsonSerializerFactory final: public GpReflectSerializerFactory
{
public:
    CLASS_DD(GpJsonSerializerFactory)

public:
                                        GpJsonSerializerFactory     (void) noexcept = default;
    inline                              GpJsonSerializerFactory     (const GpJsonSerializerFlags aFlags) noexcept;
    virtual                             ~GpJsonSerializerFactory    (void) noexcept override final;

    virtual GpReflectSerializer::SP     NewInstance                 (void) const override final;

private:
    const GpJsonSerializerFlags         iFlags;
};

GpJsonSerializerFactory::GpJsonSerializerFactory (const GpJsonSerializerFlags aFlags) noexcept:
iFlags(aFlags)
{
}

}// namespace GPlatform
