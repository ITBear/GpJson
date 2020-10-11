#pragma once

#include "GpJson_global.hpp"

namespace GPlatform {

class GpJsonMapperTypeDetector;

class GPJSON_API GpJsonMapper
{
public:
    CLASS_REMOVE_CTRS(GpJsonMapper)

    enum class Flags: size_t
    {
        WRITE_STRUCT_UID    = 1 << 0
    };

public:
    static GpTypeStructBase::SP     SFromJsonInsitu (GpRawPtrCharRW aJsonData);
    static GpTypeStructBase::SP     SFromJsonInsitu (GpRawPtrCharRW aJsonData, const GpTypeStructInfo& aTypeStructInfo);
    static GpTypeStructBase::SP     SFromJsonInsitu (GpRawPtrCharRW aJsonData, const GpJsonMapperTypeDetector& aTypeDetector);
    static void                     SFromJsonInsitu (GpRawPtrCharRW aJsonData, GpTypeStructBase& aOut);

    template <typename T> static
    typename T::SP                  SFromJsonInsitu (GpRawPtrCharRW aJsonData);

    static std::string              SToJson         (const GpTypeStructBase&    aStruct,
                                                     const size_t               aFlags);
    static void                     SToJson         (const GpTypeStructBase&    aStruct,
                                                     GpByteWriter&              aWriter,
                                                     const size_t               aFlags);
};

template <typename T>
typename T::SP  GpJsonMapper::SFromJsonInsitu (GpRawPtrCharRW aJsonData)
{
    GpTypeStructBase::SP obj = SFromJsonInsitu(aJsonData, T::STypeStructInfo());
    return obj.CastAs<typename T::SP>();
}

}//namespace GPlatform
