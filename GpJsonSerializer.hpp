#pragma once

#include "GpJsonSerializerFlags.hpp"
#include <GpCore2/GpReflection/Serializers/GpReflectSerializer.hpp>

namespace GPlatform {

class GP_JSON_API GpJsonSerializer final: public GpReflectSerializer
{
public:
    CLASS_DD(GpJsonSerializer)

public:
                                        GpJsonSerializer    (void) noexcept = default;
                                        GpJsonSerializer    (const GpJsonSerializerFlags aFlags) noexcept:iFlags(aFlags) {}
    virtual                             ~GpJsonSerializer   (void) noexcept override final;

    virtual GpReflectObject::SP         ToObject            (GpSpanPtrByteR             aData) const override final;
    virtual GpReflectObject::SP         ToObject            (GpSpanPtrByteR             aData,
                                                             const GpReflectModel&      aModel) const override final;
    virtual GpReflectObject::SP         ToObject            (GpSpanPtrByteR                             aData,
                                                             const std::vector<const GpReflectModel*>&  aModelVariants) const override final;
    virtual GpReflectObject::SP         ToObject            (GpReflectSerializerCtx&    aCtx,
                                                             const GpReflectModel&      aModel) const override final;

    virtual void                        FromObject          (const GpReflectObject& aObject,
                                                             GpByteWriter&          aWriter) const override final;

private:
    const GpJsonSerializerFlags         iFlags;

public:
    static GpReflectObject::SP          SFromStr            (std::u8string_view             aJsonStr,
                                                             const GpJsonSerializerFlags    aFlags);
    static GpReflectObject::SP          SFromStr            (std::u8string_view             aJsonStr,
                                                             const GpReflectModel&          aModel,
                                                             const GpJsonSerializerFlags    aFlags);
    static GpReflectObject::SP          SFromStr            (std::u8string_view                         aJsonStr,
                                                             const std::vector<const GpReflectModel*>&  aModelVariants,
                                                             const GpJsonSerializerFlags                aFlags);
    static void                         SFromStr            (std::u8string_view             aJsonStr,
                                                             GpReflectObject&               aOut,
                                                             const GpJsonSerializerFlags    aFlags);

    template <typename T> static
    typename T::SP                      SFromStr            (std::u8string_view             aJsonStr,
                                                             const GpJsonSerializerFlags    aFlags);

    static GpReflectObject::C::Vec::SP  SFromStrVec         (std::u8string_view             aJsonStr,
                                                             const GpJsonSerializerFlags    aFlags);
    static GpReflectObject::C::Vec::SP  SFromStrVec         (std::u8string_view             aJsonStr,
                                                             const GpReflectModel&          aModel,
                                                             const GpJsonSerializerFlags    aFlags);
    static GpReflectObject::C::Vec::SP  SFromStrVec         (std::u8string_view                         aJsonStr,
                                                             const std::vector<const GpReflectModel*>&  aModelVariants,
                                                             const GpJsonSerializerFlags                aFlags);

    template <typename T> static
    typename T::C::Vec::SP              SFromStrVec         (std::u8string_view             aJsonStr,
                                                             const GpJsonSerializerFlags    aFlags);

    static GpReflectObject::SP          SFromStrInsitu      (GpSpanPtrCharU8RW              aJsonStr,
                                                             const GpJsonSerializerFlags    aFlags);
    static GpReflectObject::SP          SFromStrInsitu      (GpSpanPtrCharU8RW              aJsonStr,
                                                             const GpReflectModel&          aModel,
                                                             const GpJsonSerializerFlags    aFlags);
    static void                         SFromStrInsitu      (GpSpanPtrCharU8RW              aJsonStr,
                                                             GpReflectObject&               aOut,
                                                             const GpJsonSerializerFlags    aFlags);

    template <typename T> static
    typename T::SP                      SFromStrInsitu      (GpSpanPtrCharU8RW              aJsonStr,
                                                             const GpJsonSerializerFlags    aFlags);

    static std::u8string                SToStr              (const GpReflectObject&         aObject,
                                                             const GpJsonSerializerFlags    aFlags);
    static void                         SToStr              (const GpReflectObject&         aObject,
                                                             GpByteWriter&                  aWriter,
                                                             const GpJsonSerializerFlags    aFlags);
};

template <typename T>
typename T::SP  GpJsonSerializer::SFromStr
(
    std::u8string_view          aJsonStr,
    const GpJsonSerializerFlags aFlags
)
{
    GpReflectObject::SP obj = SFromStr(aJsonStr, T::SReflectModel(), aFlags);
    return obj.CastAs<typename T::SP>();
}

template <typename T>
typename T::C::Vec::SP  GpJsonSerializer::SFromStrVec
(
    std::u8string_view          aJsonStr,
    const GpJsonSerializerFlags aFlags
)
{
    GpReflectObject::C::Vec::SP vec = SFromStrVec(aJsonStr, T::SReflectModel(), aFlags);
    typename T::C::Vec::SP          res;

    res.reserve(vec.size());

    for (GpReflectObject::SP& e: vec)
    {
        res.emplace_back(e.CastAs<typename T::SP>());
    }

    return res;
}

template <typename T>
typename T::SP  GpJsonSerializer::SFromStrInsitu
(
    GpSpanPtrCharU8RW           aJsonStr,
    const GpJsonSerializerFlags aFlags
)
{
    GpReflectObject::SP obj = SFromStrInsitu(aJsonStr, T::SReflectModel(), aFlags);
    return obj.CastAs<typename T::SP>();
}

}//namespace GPlatform
