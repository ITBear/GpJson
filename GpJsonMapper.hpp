#pragma once

#include "GpJsonMapperFlags.hpp"

namespace GPlatform {

class GpJsonMapperTypeDetector;

class GPJSON_API GpJsonMapper
{
public:
    CLASS_REMOVE_CTRS(GpJsonMapper)

public:
    static GpTypeStructBase::SP     SFromJsonInsitu (GpRawPtrCharRW aJsonData);
    static GpTypeStructBase::SP     SFromJsonInsitu (GpRawPtrCharRW aJsonData, const GpTypeStructInfo& aTypeInfo);
    static GpTypeStructBase::SP     SFromJsonInsitu (GpRawPtrCharRW aJsonData, const GpJsonMapperTypeDetector& aTypeDetector);
    static void                     SFromJsonInsitu (GpRawPtrCharRW aJsonData, GpTypeStructBase& aOut);

    template <typename T> static
    typename T::SP                  SFromJsonInsitu (GpRawPtrCharRW aJsonData);

    static std::string              SToJson         (const GpTypeStructBase&    aStruct,
                                                     const GpJsonMapperFlags    aFlags);
    static void                     SToJson         (const GpTypeStructBase&    aStruct,
                                                     GpByteWriter&              aWriter,
                                                     const GpJsonMapperFlags    aFlags);
};

template <typename T>
typename T::SP  GpJsonMapper::SFromJsonInsitu (GpRawPtrCharRW aJsonData)
{
    GpTypeStructBase::SP obj = SFromJsonInsitu(aJsonData, T::STypeInfo());
    return obj.CastAs<typename T::SP>();
}

}//namespace GPlatform
