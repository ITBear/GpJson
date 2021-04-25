#include "GpTypeMapperJson.hpp"
#include <iostream>

namespace GPlatform {

GpTypeMapperJson::GpTypeMapperJson (void) noexcept
{
}

GpTypeMapperJson::GpTypeMapperJson (const GpJsonMapperFlags aFlags) noexcept:
iFlags(aFlags)
{
}

GpTypeMapperJson::~GpTypeMapperJson (void) noexcept
{
}

GpTypeStructBase::SP    GpTypeMapperJson::ToStruct (GpRawPtrByteR aData) const
{
    return GpJsonMapper::SFromJson(aData);
}

GpTypeStructBase::SP    GpTypeMapperJson::ToStruct
(
    GpRawPtrByteR           aData,
    const GpTypeStructInfo& aTypeInfo
) const
{
    return GpJsonMapper::SFromJson(aData, aTypeInfo);
}

void    GpTypeMapperJson::FromStruct
(
    const GpTypeStructBase& aStruct,
    GpByteWriter&           aWriter
) const
{
    GpJsonMapper::SToJson(aStruct, aWriter, iFlags);
}

}//namespace GPlatform
