#include "GpTypeMapperJsonFactory.hpp"

namespace GPlatform {

GpTypeMapperJsonFactory::GpTypeMapperJsonFactory (void) noexcept
{
}

GpTypeMapperJsonFactory::GpTypeMapperJsonFactory (const GpJsonMapperFlags aFlags) noexcept:
iFlags(aFlags)
{
}

GpTypeMapperJsonFactory::~GpTypeMapperJsonFactory (void) noexcept
{
}

GpTypeMapper::SP    GpTypeMapperJsonFactory::NewInstance (void) const
{
    return MakeSP<GpTypeMapperJson>(iFlags);
}

}//namespace GPlatform
