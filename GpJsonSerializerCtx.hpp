#pragma once

#include "GpJson_global.hpp"
#include "../GpCore2/GpUtils/Macro/GpMacroClass.hpp"
#include "../GpCore2/GpUtils/Types/Containers/GpContainersT.hpp"
#include "../GpCore2/GpUtils/Types/Containers/GpBytesArray.hpp"
#include "../GpCore2/GpReflection/Serializers/GpReflectSerializerCtx.hpp"

namespace GPlatform {

class GP_JSON_API GpJsonSerializerCtx final: public GpReflectSerializerCtx
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpJsonSerializerCtx)
    CLASS_DD(GpJsonSerializerCtx)

public:
                                    GpJsonSerializerCtx     (void) noexcept = default;
    virtual                         ~GpJsonSerializerCtx    (void) noexcept;

    void                            Clear                   (void) noexcept;
    void                            Init                    (GpSpanPtrCharU8RW aJsonStr);
    std::optional<std::u8string>    FindMemberStr           (const std::u8string& aName);

    bool                            IsArray                 (void) const noexcept {return iIsArray;}

    template<typename T>
    const T&                        RootValue               (void) const;

private:
    void*   iJsonDOM        = nullptr;
    void*   iJsonRootValue  = nullptr;
    bool    iIsArray        = false;
};

template<typename T>
const T&    GpJsonSerializerCtx::RootValue (void) const
{
    THROW_COND_GP
    (
        iJsonRootValue != nullptr,
        "Json root value is null. Call Init first"_sv
    );

    return *static_cast<const T*>(iJsonRootValue);
}

}//namespace GPlatform
