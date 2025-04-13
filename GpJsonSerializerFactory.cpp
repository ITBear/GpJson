#include <GpJson/GpJsonSerializerFactory.hpp>
#include <GpJson/GpJsonSerializer.hpp>

namespace GPlatform {

GpJsonSerializerFactory::~GpJsonSerializerFactory (void) noexcept
{
}

GpReflectSerializer::SP GpJsonSerializerFactory::NewInstance (void) const
{
    return MakeSP<GpJsonSerializer>(iFlags);
}

}// namespace GPlatform
