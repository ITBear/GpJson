#pragma once

#include "GpJson_global.hpp"

namespace GPlatform {

class GPJSON_API GpJsonMapper
{
public:
    CLASS_REMOVE_CTRS(GpJsonMapper)

public:
    static GpTypeStructBase::SP     SFromJsonInsitu (char* aJsonData);
    static GpTypeStructBase::SP     SFromJsonInsitu (char* aJsonData, const GpTypeStructInfo& aTypeStructInfo);

    template <typename T> static
    typename T::SP                  SFromJsonInsitu (char* aJsonData);

    static std::string              SToJson         (const GpTypeStructBase& aStruct);
};

template <typename T>
typename T::SP  GpJsonMapper::SFromJsonInsitu (char* aJsonData)
{
    GpTypeStructBase::SP obj = SFromJsonInsitu(aJsonData, T::STypeStructInfo());
    return obj.CastAs<typename T::SP>();
}

}//namespace GPlatform
