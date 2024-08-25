#pragma once

#include <GpJson/GpJson_global.hpp>

#include <GpCore2/GpUtils/Macro/GpMacroClass.hpp>
#include <GpCore2/GpUtils/Types/Containers/GpContainersT.hpp>
#include <GpCore2/GpUtils/Types/Containers/GpBytesArray.hpp>
#include <GpCore2/GpReflection/Serializers/GpReflectSerializerCtx.hpp>

namespace GPlatform {

class GP_JSON_API GpJsonSerializerCtx final: public GpReflectSerializerCtx
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpJsonSerializerCtx)
    CLASS_DD(GpJsonSerializerCtx)

public:
                                GpJsonSerializerCtx     (void) noexcept = default;
    virtual                     ~GpJsonSerializerCtx    (void) noexcept;

    void                        Clear                   (void) noexcept;
    void                        Parse                   (GpSpanCharRW aJsonStr);
    std::optional<std::string>  FindMemberStr           (const std::string& aName) const;

    bool                        IsArray                 (void) const noexcept {return iIsArray;}

    const void*                 RootAsObject            (void) const;
    const void*                 RootAsArray             (void) const;

private:
    void*   iJsonDOM        = nullptr;
    void*   iJsonRootValue  = nullptr;
    bool    iIsArray        = false;
};

}// namespace GPlatform
