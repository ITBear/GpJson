#pragma once

#include "GpTypeMapperJson.hpp"

namespace GPlatform {

class GPJSON_API GpTypeMapperJsonFactory final: public GpTypeMapperFactory
{
public:
    CLASS_DECLARE_DEFAULTS(GpTypeMapperJsonFactory)

public:
                                    GpTypeMapperJsonFactory     (void) noexcept;
                                    GpTypeMapperJsonFactory     (const GpJsonMapperFlags aFlags) noexcept;
    virtual                         ~GpTypeMapperJsonFactory    (void) noexcept override final;

    virtual GpTypeMapper::SP        NewInstance                 (void) const override final;

private:
    const GpJsonMapperFlags         iFlags;
};

}//namespace GPlatform
