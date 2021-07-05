#pragma once

#include "GpJsonMapperFlags.hpp"

namespace GPlatform {

class GPJSON_API GpJsonMapper
{
public:
    CLASS_REMOVE_CTRS(GpJsonMapper)

public:
    static GpTypeStructBase::SP     SFromJson       (GpRawPtrCharR              aJsonData,
                                                     const GpJsonMapperFlags&   aFlags);
    static GpTypeStructBase::SP     SFromJson       (GpRawPtrCharR              aJsonData,
                                                     const GpTypeStructInfo&    aTypeInfo,
                                                     const GpJsonMapperFlags&   aFlags);
    static void                     SFromJson       (GpRawPtrCharR              aJsonData,
                                                     GpTypeStructBase&          aOut,
                                                     const GpJsonMapperFlags&   aFlags);

    template <typename T> static
    typename T::SP                  SFromJson       (GpRawPtrCharR              aJsonData,
                                                     const GpJsonMapperFlags&   aFlags);

    static GpTypeStructBase::SP     SFromJsonInsitu (GpRawPtrCharRW             aJsonData,
                                                     const GpJsonMapperFlags&   aFlags);
    static GpTypeStructBase::SP     SFromJsonInsitu (GpRawPtrCharRW             aJsonData,
                                                     const GpTypeStructInfo&    aTypeInfo,
                                                     const GpJsonMapperFlags&   aFlags);
    static void                     SFromJsonInsitu (GpRawPtrCharRW             aJsonData,
                                                     GpTypeStructBase&          aOut,
                                                     const GpJsonMapperFlags&   aFlags);

    template <typename T> static
    typename T::SP                  SFromJsonInsitu (GpRawPtrCharRW             aJsonData,
                                                     const GpJsonMapperFlags&   aFlags);

    static std::string              SToJson         (const GpTypeStructBase&    aStruct,
                                                     const GpJsonMapperFlags&   aFlags);
    static void                     SToJson         (const GpTypeStructBase&    aStruct,
                                                     GpByteWriter&              aWriter,
                                                     const GpJsonMapperFlags&   aFlags);
};

template <typename T>
typename T::SP  GpJsonMapper::SFromJson
(
    GpRawPtrCharR               aJsonData,
    const GpJsonMapperFlags&    aFlags
)
{
    GpTypeStructBase::SP obj = SFromJson(aJsonData, T::STypeInfo(), aFlags);
    return obj.CastAs<typename T::SP>();
}

template <typename T>
typename T::SP  GpJsonMapper::SFromJsonInsitu
(
    GpRawPtrCharRW              aJsonData,
    const GpJsonMapperFlags&    aFlags
)
{
    GpTypeStructBase::SP obj = SFromJsonInsitu(aJsonData, T::STypeInfo(), aFlags);
    return obj.CastAs<typename T::SP>();
}

}//namespace GPlatform
