#pragma once

#include "GpJsonMapper.hpp"

namespace GPlatform {

class GPJSON_API GpTypeMapperJson final: public GpTypeMapper
{
public:
    CLASS_DECLARE_DEFAULTS(GpTypeMapperJson)

public:
                                    GpTypeMapperJson    (void) noexcept;
                                    GpTypeMapperJson    (const GpJsonMapperFlags aFlags) noexcept;

    virtual                         ~GpTypeMapperJson   (void) noexcept override final;

    virtual GpTypeStructBase::SP    ToStruct            (GpRawPtrByteR aData) const override final;
    virtual GpTypeStructBase::SP    ToStruct            (GpRawPtrByteR              aData,
                                                         const GpTypeStructInfo&    aTypeInfo) const override final;

    virtual void                    FromStruct          (const GpTypeStructBase&    aStruct,
                                                         GpByteWriter&              aWriter) const override final;

private:
    const GpJsonMapperFlags         iFlags;
};

}//namespace GPlatform
