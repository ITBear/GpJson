#include "GpJsonToObject.hpp"
#include "GpJsonUtilsInternal.hpp"

#include <GpCore2/GpUtils/Encoders/GpBase64.hpp>
#include <GpCore2/GpReflection/GpReflectManager.hpp>
#include <GpCore2/GpReflection/GpReflectUtils.hpp>
#include <GpCore2/GpReflection/GpReflectVisitor.hpp>

namespace GPlatform {

//------------------------------------- JVisitor_VisitCtx ------------------------------------------
namespace ObjectVisitor_JsonToObject {

double  JVisitor_SReadDouble (const rapidjson::Document::GenericValue& aJsonValue)
{
    double res = 0.0;

    if (aJsonValue.IsDouble())
    {
        res = NumOps::SConvert<double>(aJsonValue.GetDouble());
    } else if (aJsonValue.IsInt64())
    {
        res = NumOps::SConvert<double>(aJsonValue.GetInt64());
    } else if (aJsonValue.IsInt())
    {
        res = NumOps::SConvert<double>(static_cast<int>(aJsonValue.IsInt()));
    } else if (aJsonValue.IsString())
    {
        const auto r = StrOps::SToNumeric(_JsonValue2SV(aJsonValue));
        if (std::holds_alternative<double>(r))
        {
            return std::get<double>(r);
        } else
        {
            return NumOps::SConvert<double>(std::get<s_int_64>(r));
        }
    } else
    {
        THROW_GP(u8"Unsupported type"_sv);
    }

    return res;
}

class JVisitor_VisitCtx
{
public:
                                            JVisitor_VisitCtx   (const rapidjson::Document::ConstObject&    aJsonObject,
                                                                 const GpJsonSerializerFlags                aJsonSerializerFlags) noexcept;

    [[nodiscard]] bool                      OnVisitBegin        (const GpReflectModel& aModel);
    void                                    OnVisitEnd          (const GpReflectModel& aModel);

public:
    const rapidjson::Document::ConstObject& iJsonObject;
    const GpJsonSerializerFlags             iJsonSerializerFlags;
};

JVisitor_VisitCtx::JVisitor_VisitCtx
(
    const rapidjson::Document::ConstObject& aJsonObject,
    const GpJsonSerializerFlags             aJsonSerializerFlags
) noexcept:
iJsonObject(aJsonObject),
iJsonSerializerFlags(aJsonSerializerFlags)
{
}

bool    JVisitor_VisitCtx::OnVisitBegin (const GpReflectModel& /*aModel*/)
{
    return true;
}

void    JVisitor_VisitCtx::OnVisitEnd (const GpReflectModel& /*aModel*/)
{
}

//------------------------------------- JVisitor_VisitValueCtx ------------------------------------------
class JVisitor_VisitValueCtx
{
public:
                        JVisitor_VisitValueCtx  (void) noexcept {}

    [[nodiscard]] bool  OnVisitBegin            (void*                  aDataPtr,
                                                 const GpReflectProp&   aProp,
                                                 JVisitor_VisitCtx&     aCtx);
    void                OnVisitEnd              (void*                  aDataPtr,
                                                 const GpReflectProp&   aProp,
                                                 JVisitor_VisitCtx&     aCtx);
    void                Value_UInt8             (void*                  aDataPtr,
                                                 const GpReflectProp&   aProp,
                                                 JVisitor_VisitCtx&     aCtx);
    void                Value_SInt8             (void*                  aDataPtr,
                                                 const GpReflectProp&   aProp,
                                                 JVisitor_VisitCtx&     aCtx);
    void                Value_UInt16            (void*                  aDataPtr,
                                                 const GpReflectProp&   aProp,
                                                 JVisitor_VisitCtx&     aCtx);
    void                Value_SInt16            (void*                  aDataPtr,
                                                 const GpReflectProp&   aProp,
                                                 JVisitor_VisitCtx&     aCtx);
    void                Value_UInt32            (void*                  aDataPtr,
                                                 const GpReflectProp&   aProp,
                                                 JVisitor_VisitCtx&     aCtx);
    void                Value_SInt32            (void*                  aDataPtr,
                                                 const GpReflectProp&   aProp,
                                                 JVisitor_VisitCtx&     aCtx);
    void                Value_UInt64            (void*                  aDataPtr,
                                                 const GpReflectProp&   aProp,
                                                 JVisitor_VisitCtx&     aCtx);
    void                Value_SInt64            (void*                  aDataPtr,
                                                 const GpReflectProp&   aProp,
                                                 JVisitor_VisitCtx&     aCtx);
    void                Value_Double            (void*                  aDataPtr,
                                                 const GpReflectProp&   aProp,
                                                 JVisitor_VisitCtx&     aCtx);
    void                Value_Float             (void*                  aDataPtr,
                                                 const GpReflectProp&   aProp,
                                                 JVisitor_VisitCtx&     aCtx);
    void                Value_Bool              (void*                  aDataPtr,
                                                 const GpReflectProp&   aProp,
                                                 JVisitor_VisitCtx&     aCtx);
    void                Value_UUID              (void*                  aDataPtr,
                                                 const GpReflectProp&   aProp,
                                                 JVisitor_VisitCtx&     aCtx);
    void                Value_String            (void*                  aDataPtr,
                                                 const GpReflectProp&   aProp,
                                                 JVisitor_VisitCtx&     aCtx);
    void                Value_BLOB              (void*                  aDataPtr,
                                                 const GpReflectProp&   aProp,
                                                 JVisitor_VisitCtx&     aCtx);
    void                Value_Object            (void*                  aDataPtr,
                                                 const GpReflectProp&   aProp,
                                                 JVisitor_VisitCtx&     aCtx);
    void                Value_ObjectSP          (void*                  aDataPtr,
                                                 const GpReflectProp&   aProp,
                                                 JVisitor_VisitCtx&     aCtx);
    void                Value_Enum              (void*                  aDataPtr,
                                                 const GpReflectProp&   aProp,
                                                 JVisitor_VisitCtx&     aCtx);
    void                Value_EnumFlags         (void*                  aDataPtr,
                                                 const GpReflectProp&   aProp,
                                                 JVisitor_VisitCtx&     aCtx);

public:
    rapidjson::Document::ConstMemberIterator    iMit;
    const rapidjson::Document::GenericValue*    iMitVal = nullptr;
};

bool    JVisitor_VisitValueCtx::OnVisitBegin
(
    void*                   /*aDataPtr*/,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    std::u8string_view propName = aProp.FlagTest(GpReflectPropFlag::NAME_OVERRIDE) ?
                                  aProp.FlagArg(GpReflectPropFlag::NAME_OVERRIDE).value() : aProp.Name();

    //Find json member
    iMit = aCtx.iJsonObject.FindMember
    (
        rapidjson::Document::ValueType
        (
            GpUTF::S_As_STR(propName).data(),
            NumOps::SConvert<rapidjson::SizeType>(propName.size())
        )
    );

    if (   (iMit == aCtx.iJsonObject.MemberEnd())
        || (iMit->value.IsNull()))
    {
        return false;
    }

    iMitVal = &(iMit->value);

    return true;
}

void    JVisitor_VisitValueCtx::OnVisitEnd
(
    void*                   /*aDataPtr*/,
    const GpReflectProp&    /*aProp*/,
    JVisitor_VisitCtx&      /*aCtx*/
)
{
    //NOP
}

void    JVisitor_VisitValueCtx::Value_UInt8
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      /*aCtx*/
)
{
    if (iMitVal->IsNumber())
    {
        aProp.Value_UInt8(aDataPtr) = NumOps::SConvert<u_int_8>(iMitVal->GetUint64());
    } else
    {
        aProp.Value_UInt8(aDataPtr) = NumOps::SConvert<u_int_8>(StrOps::SToUI64(_JsonValue2SV(iMitVal)));
    }
}

void    JVisitor_VisitValueCtx::Value_SInt8
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      /*aCtx*/
)
{
    if (iMitVal->IsNumber())
    {
        aProp.Value_SInt8(aDataPtr) = NumOps::SConvert<s_int_8>(iMitVal->GetInt64());
    } else
    {
        aProp.Value_SInt8(aDataPtr) = NumOps::SConvert<s_int_8>(StrOps::SToSI64(_JsonValue2SV(iMitVal)));
    }
}

void    JVisitor_VisitValueCtx::Value_UInt16
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      /*aCtx*/
)
{
    if (iMitVal->IsNumber())
    {
        aProp.Value_UInt16(aDataPtr) = NumOps::SConvert<u_int_16>(iMitVal->GetUint64());
    } else
    {
        aProp.Value_UInt16(aDataPtr) = NumOps::SConvert<u_int_16>(StrOps::SToUI64(_JsonValue2SV(iMitVal)));
    }
}

void    JVisitor_VisitValueCtx::Value_SInt16
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      /*aCtx*/
)
{
    if (iMitVal->IsNumber())
    {
        aProp.Value_SInt16(aDataPtr) = NumOps::SConvert<s_int_16>(iMitVal->GetInt64());
    } else
    {
        aProp.Value_SInt16(aDataPtr) = NumOps::SConvert<s_int_16>(StrOps::SToSI64(_JsonValue2SV(iMitVal)));
    }
}

void    JVisitor_VisitValueCtx::Value_UInt32
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      /*aCtx*/
)
{
    if (iMitVal->IsNumber())
    {
        aProp.Value_UInt32(aDataPtr) = NumOps::SConvert<u_int_32>(iMitVal->GetUint64());
    } else
    {
        aProp.Value_UInt32(aDataPtr) = NumOps::SConvert<u_int_32>(StrOps::SToUI64(_JsonValue2SV(iMitVal)));
    }
}

void    JVisitor_VisitValueCtx::Value_SInt32
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      /*aCtx*/
)
{
    if (iMitVal->IsNumber())
    {
        aProp.Value_SInt32(aDataPtr) = NumOps::SConvert<s_int_32>(iMitVal->GetInt64());
    } else
    {
        aProp.Value_SInt32(aDataPtr) = NumOps::SConvert<s_int_32>(StrOps::SToSI64(_JsonValue2SV(iMitVal)));
    }
}

void    JVisitor_VisitValueCtx::Value_UInt64
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      /*aCtx*/
)
{
    if (iMitVal->IsNumber())
    {
        aProp.Value_UInt64(aDataPtr) = NumOps::SConvert<u_int_64>(iMitVal->GetUint64());
    } else
    {
        aProp.Value_UInt64(aDataPtr) = NumOps::SConvert<u_int_64>(StrOps::SToUI64(_JsonValue2SV(iMitVal)));
    }
}

void    JVisitor_VisitValueCtx::Value_SInt64
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      /*aCtx*/
)
{
    if (iMitVal->IsNumber())
    {
        aProp.Value_SInt64(aDataPtr) = NumOps::SConvert<s_int_64>(iMitVal->GetInt64());
    } else
    {
        aProp.Value_SInt64(aDataPtr) = NumOps::SConvert<s_int_64>(StrOps::SToSI64(_JsonValue2SV(iMitVal)));
    }
}

void    JVisitor_VisitValueCtx::Value_Double
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      /*aCtx*/
)
{
    aProp.Value_Double(aDataPtr) = JVisitor_SReadDouble(*iMitVal);
}

void    JVisitor_VisitValueCtx::Value_Float
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      /*aCtx*/
)
{
    aProp.Value_Float(aDataPtr) = NumOps::SConvert<float>(JVisitor_SReadDouble(*iMitVal));
}

void    JVisitor_VisitValueCtx::Value_Bool
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      /*aCtx*/
)
{
    aProp.Value_Bool(aDataPtr) = iMitVal->GetBool();
}

void    JVisitor_VisitValueCtx::Value_UUID
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      /*aCtx*/
)
{
    aProp.Value_UUID(aDataPtr) = GpUUID::SFromString(_JsonValue2SV(iMitVal));
}

void    JVisitor_VisitValueCtx::Value_String
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      /*aCtx*/
)
{
    aProp.Value_String(aDataPtr) = std::u8string(_JsonValue2SV(iMitVal));
}

void    JVisitor_VisitValueCtx::Value_BLOB
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      /*aCtx*/
)
{
    aProp.Value_BLOB(aDataPtr) = GpBase64::SDecodeToByteArray(_JsonValue2SV(iMitVal));
}

void    JVisitor_VisitValueCtx::Value_Object
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    GpReflectObject&                    object      = aProp.Value_Object(aDataPtr);
    rapidjson::Document::ConstObject    jsonObject  = iMitVal->GetObject();
    GpJsonToObject::SReadObject(object, jsonObject, aCtx.iJsonSerializerFlags);
}

void    JVisitor_VisitValueCtx::Value_ObjectSP
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    const GpReflectModel&               model       = GpReflectManager::S().Find(aProp.ModelUid());
    rapidjson::Document::ConstObject    jsonObject  = iMitVal->GetObject();
    const GpReflectModel&               modelJson   = GpJsonToObject::SCheckModel
    (
        jsonObject,
        model
    );

    GpReflectObject::SP&    objectBaseSP    = aProp.Value_ObjectSP(aDataPtr);
                            objectBaseSP    = modelJson.NewInstance();
    GpReflectObject&        objectBase      = objectBaseSP.Vn();

    GpJsonToObject::SReadObject(objectBase, jsonObject, aCtx.iJsonSerializerFlags);
}

void    JVisitor_VisitValueCtx::Value_Enum
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      /*aCtx*/
)
{
    aProp.Value_Enum(aDataPtr).FromString(_JsonValue2SV(iMitVal));
}

void    JVisitor_VisitValueCtx::Value_EnumFlags
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      /*aCtx*/
)
{
    rapidjson::Document::ConstArray array       = iMitVal->GetArray();
    GpEnumFlags&                    enumFlags   = aProp.Value_EnumFlags(aDataPtr);

    for (const auto& v: array)
    {
        enumFlags.Set(_JsonValue2SV(v));
    }
}

//------------------------------------- JVisitor_VisitContainerCtx ------------------------------------------
class JVisitor_VisitContainerCtx
{
public:
                        JVisitor_VisitContainerCtx  (void) noexcept {}

    [[nodiscard]] bool  OnVisitBegin                (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     JVisitor_VisitCtx&     aCtx);
    void                OnVisitEnd                  (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     JVisitor_VisitCtx&     aCtx);

    template<typename ValGetterT>
    void                Value_UInt8                 (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     JVisitor_VisitCtx&     aCtx);

    template<typename ValGetterT>
    void                Value_SInt8                 (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     JVisitor_VisitCtx&     aCtx);

    template<typename ValGetterT>
    void                Value_UInt16                (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     JVisitor_VisitCtx&     aCtx);

    template<typename ValGetterT>
    void                Value_SInt16                (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     JVisitor_VisitCtx&     aCtx);

    template<typename ValGetterT>
    void                Value_UInt32                (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     JVisitor_VisitCtx&     aCtx);

    template<typename ValGetterT>
    void                Value_SInt32                (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     JVisitor_VisitCtx&     aCtx);

    template<typename ValGetterT>
    void                Value_UInt64                (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     JVisitor_VisitCtx&     aCtx);

    template<typename ValGetterT>
    void                Value_SInt64                (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     JVisitor_VisitCtx&     aCtx);

    template<typename ValGetterT>
    void                Value_Double                (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     JVisitor_VisitCtx&     aCtx);

    template<typename ValGetterT>
    void                Value_Float                 (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     JVisitor_VisitCtx&     aCtx);

    template<typename ValGetterT>
    void                Value_Bool                  (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     JVisitor_VisitCtx&     aCtx);

    template<typename ValGetterT>
    void                Value_UUID                  (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     JVisitor_VisitCtx&     aCtx);

    template<typename ValGetterT>
    void                Value_String                (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     JVisitor_VisitCtx&     aCtx);

    template<typename ValGetterT>
    void                Value_BLOB                  (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     JVisitor_VisitCtx&     aCtx);

    template<typename ValGetterT>
    void                Value_Object                (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     JVisitor_VisitCtx&     aCtx);

    template<typename ValGetterT>
    void                Value_ObjectSP              (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     JVisitor_VisitCtx&     aCtx);

    template<typename ValGetterT>
    void                Value_Enum                  (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     JVisitor_VisitCtx&     aCtx);

    template<typename ValGetterT>
    void                Value_EnumFlags             (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     JVisitor_VisitCtx&     aCtx);

private:
    template<typename T>
    static void         SClear                      (std::array<T, 0>&) {}

    template<typename T>
    static void         SClear                      (std::vector<T>& aVector) {aVector.clear();}

    static void         SClear                      (GpVectorReflectObjWrapBase& aVector) {aVector.clear();}

    template<typename T>
    static void         SReserve                    (std::array<T, 0>&,
                                                     const size_t) {}

    template<typename T>
    static void         SReserve                    (std::vector<T>&    aVector,
                                                     const size_t       aSize) {aVector.reserve(aSize);}

    static void         SReserve                    (GpVectorReflectObjWrapBase&    aVector,
                                                     const size_t                   aSize) {aVector.reserve(aSize);}

    template<typename T,
             typename... Ts>
    void                SEmplaceBack                (std::array<T, 0>&,
                                                     Ts&&...) {}

    template<typename T,
             typename... Ts>
    void                SEmplaceBack                (std::vector<T>&    aVector,
                                                     Ts&&...            aArgs) {aVector.emplace_back(std::forward<Ts>(aArgs)...);}

    void                SEmplaceBack                (GpVectorReflectObjWrapBase&    aVector,
                                                     GpReflectObject&&              aValue) {aVector.emplace_back(std::move(aValue));}

public:
    rapidjson::Document::ConstMemberIterator    iMit;
    const rapidjson::Document::GenericValue*    iMitVal = nullptr;
};

bool    JVisitor_VisitContainerCtx::OnVisitBegin
(
    void*                   /*aDataPtr*/,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    std::u8string_view propName = aProp.FlagTest(GpReflectPropFlag::NAME_OVERRIDE) ?
                                  aProp.FlagArg(GpReflectPropFlag::NAME_OVERRIDE).value() : aProp.Name();

    //Find json member
    iMit = aCtx.iJsonObject.FindMember
    (
        rapidjson::Document::ValueType
        (
            GpUTF::S_As_STR(propName).data(),
            NumOps::SConvert<rapidjson::SizeType>(propName.size())
        )
    );

    if (   (iMit == aCtx.iJsonObject.MemberEnd())
        || (iMit->value.IsNull()))
    {
        return false;
    }

    iMitVal = &(iMit->value);

    THROW_COND_GP
    (
        iMitVal->IsArray(),
        [&](){return u8"Json value '"_sv + propName + u8"' must be array"_sv;}
    );

    return true;
}

void    JVisitor_VisitContainerCtx::OnVisitEnd
(
    void*                   /*aDataPtr*/,
    const GpReflectProp&    /*aProp*/,
    JVisitor_VisitCtx&      /*aCtx*/
)
{
    //NOP
}

template<typename ValGetterT>
void    JVisitor_VisitContainerCtx::Value_UInt8
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      /*aCtx*/
)
{
    rapidjson::Document::ConstArray jArray = iMitVal->GetArray();

    auto& container = ValGetterT::UInt8(aDataPtr, aProp);
    SClear(container);
    SReserve(container, jArray.Size());

    for (const auto& v: jArray)
    {
        SEmplaceBack(container, NumOps::SConvert<u_int_8>(v.GetUint64()));
    }
}

template<typename ValGetterT>
void    JVisitor_VisitContainerCtx::Value_SInt8
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      /*aCtx*/
)
{
    rapidjson::Document::ConstArray jArray = iMitVal->GetArray();

    auto& container = ValGetterT::SInt8(aDataPtr, aProp);
    SClear(container);
    SReserve(container, jArray.Size());

    for (const auto& v: jArray)
    {
        SEmplaceBack(container, NumOps::SConvert<s_int_8>(v.GetInt64()));
    }
}

template<typename ValGetterT>
void    JVisitor_VisitContainerCtx::Value_UInt16
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      /*aCtx*/
)
{
    rapidjson::Document::ConstArray jArray = iMitVal->GetArray();

    auto& container = ValGetterT::UInt16(aDataPtr, aProp);
    SClear(container);
    SReserve(container, jArray.Size());

    for (const auto& v: jArray)
    {
        SEmplaceBack(container, NumOps::SConvert<u_int_16>(v.GetUint64()));
    }
}

template<typename ValGetterT>
void    JVisitor_VisitContainerCtx::Value_SInt16
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      /*aCtx*/
)
{
    rapidjson::Document::ConstArray jArray = iMitVal->GetArray();

    auto& container = ValGetterT::SInt16(aDataPtr, aProp);
    SClear(container);
    SReserve(container, jArray.Size());

    for (const auto& v: jArray)
    {
        SEmplaceBack(container, NumOps::SConvert<s_int_16>(v.GetInt64()));
    }
}

template<typename ValGetterT>
void    JVisitor_VisitContainerCtx::Value_UInt32
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      /*aCtx*/
)
{
    rapidjson::Document::ConstArray jArray = iMitVal->GetArray();

    auto& container = ValGetterT::UInt32(aDataPtr, aProp);
    SClear(container);
    SReserve(container, jArray.Size());

    for (const auto& v: jArray)
    {
        SEmplaceBack(container, NumOps::SConvert<u_int_32>(v.GetUint64()));
    }
}

template<typename ValGetterT>
void    JVisitor_VisitContainerCtx::Value_SInt32
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      /*aCtx*/
)
{
    rapidjson::Document::ConstArray jArray = iMitVal->GetArray();

    auto& container = ValGetterT::SInt32(aDataPtr, aProp);
    SClear(container);
    SReserve(container, jArray.Size());

    for (const auto& v: jArray)
    {
        SEmplaceBack(container, NumOps::SConvert<s_int_32>(v.GetInt64()));
    }
}

template<typename ValGetterT>
void    JVisitor_VisitContainerCtx::Value_UInt64
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      /*aCtx*/
)
{
    rapidjson::Document::ConstArray jArray = iMitVal->GetArray();

    auto& container = ValGetterT::UInt64(aDataPtr, aProp);
    SClear(container);
    SReserve(container, jArray.Size());

    for (const auto& v: jArray)
    {
        SEmplaceBack(container, NumOps::SConvert<u_int_64>(v.GetUint64()));
    }
}

template<typename ValGetterT>
void    JVisitor_VisitContainerCtx::Value_SInt64
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      /*aCtx*/
)
{
    rapidjson::Document::ConstArray jArray = iMitVal->GetArray();

    auto& container = ValGetterT::SInt64(aDataPtr, aProp);
    SClear(container);
    SReserve(container, jArray.Size());

    for (const auto& v: jArray)
    {
        SEmplaceBack(container, NumOps::SConvert<s_int_64>(v.GetInt64()));
    }
}

template<typename ValGetterT>
void    JVisitor_VisitContainerCtx::Value_Double
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      /*aCtx*/
)
{
    rapidjson::Document::ConstArray jArray = iMitVal->GetArray();

    auto& container = ValGetterT::Double(aDataPtr, aProp);
    SClear(container);
    SReserve(container, jArray.Size());

    for (const auto& v: jArray)
    {
        SEmplaceBack(container, JVisitor_SReadDouble(v));
    }
}

template<typename ValGetterT>
void    JVisitor_VisitContainerCtx::Value_Float
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      /*aCtx*/
)
{
    rapidjson::Document::ConstArray jArray = iMitVal->GetArray();

    auto& container = ValGetterT::Float(aDataPtr, aProp);
    SClear(container);
    SReserve(container, jArray.Size());

    for (const auto& v: jArray)
    {
        SEmplaceBack(container, NumOps::SConvert<float>(JVisitor_SReadDouble(v)));
    }
}

template<typename ValGetterT>
void    JVisitor_VisitContainerCtx::Value_Bool
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      /*aCtx*/
)
{
    rapidjson::Document::ConstArray jArray = iMitVal->GetArray();

    auto& container = ValGetterT::Bool(aDataPtr, aProp);
    SClear(container);
    SReserve(container, jArray.Size());

    for (const auto& v: jArray)
    {
        SEmplaceBack(container, v.GetBool());
    }
}

template<typename ValGetterT>
void    JVisitor_VisitContainerCtx::Value_UUID
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      /*aCtx*/
)
{
    rapidjson::Document::ConstArray jArray = iMitVal->GetArray();

    auto& container = ValGetterT::UUID(aDataPtr, aProp);
    SClear(container);
    SReserve(container, jArray.Size());

    for (const auto& v: jArray)
    {
        SEmplaceBack(container, GpUUID::SFromString(_JsonValue2SV(v)));
    }
}

template<typename ValGetterT>
void    JVisitor_VisitContainerCtx::Value_String
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      /*aCtx*/
)
{
    rapidjson::Document::ConstArray jArray = iMitVal->GetArray();

    auto& container = ValGetterT::String(aDataPtr, aProp);
    SClear(container);
    SReserve(container, jArray.Size());

    for (const auto& v: jArray)
    {
        SEmplaceBack(container, std::u8string(_JsonValue2SV(v)));
    }
}

template<typename ValGetterT>
void    JVisitor_VisitContainerCtx::Value_BLOB
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      /*aCtx*/
)
{
    rapidjson::Document::ConstArray jArray = iMitVal->GetArray();

    auto& container = ValGetterT::BLOB(aDataPtr, aProp);
    SClear(container);
    SReserve(container, jArray.Size());

    for (const auto& v: jArray)
    {
        SEmplaceBack(container, GpBase64::SDecodeToByteArray(_JsonValue2SV(v)));
    }
}

template<typename ValGetterT>
void    JVisitor_VisitContainerCtx::Value_Object
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    rapidjson::Document::ConstArray jArray = iMitVal->GetArray();

    auto& container = ValGetterT::Object(aDataPtr, aProp);
    SClear(container);
    SReserve(container, jArray.Size());

    const GpReflectModel& model = GpReflectManager::S().Find(aProp.ModelUid());

    for (const auto& v: jArray)
    {
        const rapidjson::Document::ConstObject& jsonObject  = v.GetObject();
        const GpReflectModel&                   modelJson   = GpJsonToObject::SCheckModel
        (
            jsonObject,
            model
        );

        GpReflectObject::SP objectBaseSP    = modelJson.NewInstance();
        GpReflectObject&    objectBase      = objectBaseSP.Vn();
        GpJsonToObject::SReadObject(objectBase, jsonObject, aCtx.iJsonSerializerFlags);

        if constexpr (std::is_same_v<typename std::remove_cvref_t<decltype(container)>::value_type, GpReflectObject>)
        {
            SEmplaceBack(container, std::move(objectBase));
        } else
        {
            SEmplaceBack(container, std::move(objectBaseSP));
        }
    }
}

template<typename ValGetterT>
void    JVisitor_VisitContainerCtx::Value_ObjectSP
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    rapidjson::Document::ConstArray jArray = iMitVal->GetArray();

    auto& container = ValGetterT::ObjectSP(aDataPtr, aProp);
    SClear(container);
    SReserve(container, jArray.Size());

    const GpReflectModel& model = GpReflectManager::S().Find(aProp.ModelUid());

    for (const auto& v: jArray)
    {
        const rapidjson::Document::ConstObject& jsonObject  = v.GetObject();
        const GpReflectModel&                   modelJson   = GpJsonToObject::SCheckModel
        (
            jsonObject,
            model
        );

        GpReflectObject::SP objectBaseSP    = modelJson.NewInstance();
        GpReflectObject&    objectBase      = objectBaseSP.Vn();
        GpJsonToObject::SReadObject(objectBase, jsonObject, aCtx.iJsonSerializerFlags);

        SEmplaceBack(container, std::move(objectBaseSP));
    }
}

template<typename ValGetterT>
void    JVisitor_VisitContainerCtx::Value_Enum
(
    void*                   /*aDataPtr*/,
    const GpReflectProp&    /*aProp*/,
    JVisitor_VisitCtx&      /*aCtx*/
)
{
    THROW_GP(u8"Enum arrays are not supported."_sv);
}

template<typename ValGetterT>
void    JVisitor_VisitContainerCtx::Value_EnumFlags
(
    void*                   /*aDataPtr*/,
    const GpReflectProp&    /*aProp*/,
    JVisitor_VisitCtx&      /*aCtx*/
)
{
    THROW_GP(u8"Enum flag arrays are not supported."_sv);
}

//------------------------------------- JVisitor_VisitMapCtx ------------------------------------------
class JVisitor_VisitMapCtx
{
public:
                        JVisitor_VisitMapCtx        (void) noexcept {}

    [[nodiscard]] bool  OnVisitBegin                (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     JVisitor_VisitCtx&     aCtx);
    void                OnVisitEnd                  (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     JVisitor_VisitCtx&     aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void                K_UInt8                     (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     JVisitor_VisitCtx&     aCtx);
    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void                K_SInt8                     (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     JVisitor_VisitCtx&     aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void                K_UInt16                    (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     JVisitor_VisitCtx&     aCtx);
    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void                K_SInt16                    (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     JVisitor_VisitCtx&     aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void                K_UInt32                    (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     JVisitor_VisitCtx&     aCtx);
    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void                K_SInt32                    (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     JVisitor_VisitCtx&     aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void                K_UInt64                    (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     JVisitor_VisitCtx&     aCtx);
    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void                K_SInt64                    (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     JVisitor_VisitCtx&     aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void                K_Double                    (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     JVisitor_VisitCtx&     aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void                K_Float                     (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     JVisitor_VisitCtx&     aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void                K_Bool                      (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     JVisitor_VisitCtx&     aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void                K_UUID                      (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     JVisitor_VisitCtx&     aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void                K_String                    (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     JVisitor_VisitCtx&     aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void                K_BLOB                      (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     JVisitor_VisitCtx&     aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void                K_ObjectSP                  (void*                  aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     JVisitor_VisitCtx&     aCtx);

private:
    template<typename T>
    T                   ProcessMapKey               (std::u8string_view aValue);

public:
    rapidjson::Document::ConstMemberIterator    iMit;
    const rapidjson::Document::GenericValue*    iMitVal = nullptr;
};

bool    JVisitor_VisitMapCtx::OnVisitBegin
(
    void*                   /*aDataPtr*/,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    std::u8string_view propName = aProp.FlagTest(GpReflectPropFlag::NAME_OVERRIDE) ?
                                  aProp.FlagArg(GpReflectPropFlag::NAME_OVERRIDE).value() : aProp.Name();

    //Find json member
    iMit = aCtx.iJsonObject.FindMember
    (
        rapidjson::Document::ValueType
        (
            GpUTF::S_As_STR(propName).data(),
            NumOps::SConvert<rapidjson::SizeType>(propName.size())
        )
    );

    if (   (iMit == aCtx.iJsonObject.MemberEnd())
        || (iMit->value.IsNull()))
    {
        return false;
    }

    iMitVal = &(iMit->value);

    THROW_COND_GP
    (
        iMitVal->IsObject(),
        [&](){return u8"Json value '"_sv + propName + u8"' must be object"_sv;}
    );

    return true;
}

void    JVisitor_VisitMapCtx::OnVisitEnd
(
    void*                   /*aDataPtr*/,
    const GpReflectProp&    /*aProp*/,
    JVisitor_VisitCtx&      /*aCtx*/
)
{
    //NOP
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    JVisitor_VisitMapCtx::K_UInt8
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      /*aCtx*/
)
{
    rapidjson::Document::ConstObject    jObj        = iMitVal->GetObject();
    auto&                               container   = ValGetterT<KeyT>::UInt8(aDataPtr, aProp);

    for (const auto& v: jObj)
    {
        const auto& name    = v.name;
        const auto& value   = v.value;
        container.try_emplace
        (
            ProcessMapKey<KeyT>(_JsonValue2SV(name)),
            NumOps::SConvert<u_int_8>(value.GetUint64())
        );
    }
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    JVisitor_VisitMapCtx::K_SInt8
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      /*aCtx*/
)
{
    rapidjson::Document::ConstObject    jObj        = iMitVal->GetObject();
    auto&                               container   = ValGetterT<KeyT>::SInt8(aDataPtr, aProp);

    for (const auto& v: jObj)
    {
        const auto& name    = v.name;
        const auto& value   = v.value;
        container.try_emplace
        (
            ProcessMapKey<KeyT>(_JsonValue2SV(name)),
            NumOps::SConvert<s_int_8>(value.GetInt64())
        );
    }
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    JVisitor_VisitMapCtx::K_UInt16
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      /*aCtx*/
)
{
    rapidjson::Document::ConstObject    jObj        = iMitVal->GetObject();
    auto&                               container   = ValGetterT<KeyT>::UInt16(aDataPtr, aProp);

    for (const auto& v: jObj)
    {
        const auto& name    = v.name;
        const auto& value   = v.value;
        container.try_emplace
        (
            ProcessMapKey<KeyT>(_JsonValue2SV(name)),
            NumOps::SConvert<u_int_16>(value.GetUint64())
        );
    }
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    JVisitor_VisitMapCtx::K_SInt16
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      /*aCtx*/
)
{
    rapidjson::Document::ConstObject    jObj        = iMitVal->GetObject();
    auto&                               container   = ValGetterT<KeyT>::SInt16(aDataPtr, aProp);

    for (const auto& v: jObj)
    {
        const auto& name    = v.name;
        const auto& value   = v.value;
        container.try_emplace
        (
            ProcessMapKey<KeyT>(_JsonValue2SV(name)),
            NumOps::SConvert<s_int_16>(value.GetInt64())
        );
    }
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    JVisitor_VisitMapCtx::K_UInt32
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      /*aCtx*/
)
{
    rapidjson::Document::ConstObject    jObj        = iMitVal->GetObject();
    auto&                               container   = ValGetterT<KeyT>::UInt32(aDataPtr, aProp);

    for (const auto& v: jObj)
    {
        const auto& name    = v.name;
        const auto& value   = v.value;
        container.try_emplace
        (
            ProcessMapKey<KeyT>(_JsonValue2SV(name)),
            NumOps::SConvert<u_int_32>(value.GetUint64())
        );
    }
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    JVisitor_VisitMapCtx::K_SInt32
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      /*aCtx*/
)
{
    rapidjson::Document::ConstObject    jObj        = iMitVal->GetObject();
    auto&                               container   = ValGetterT<KeyT>::SInt32(aDataPtr, aProp);

    for (const auto& v: jObj)
    {
        const auto& name    = v.name;
        const auto& value   = v.value;
        container.try_emplace
        (
            ProcessMapKey<KeyT>(_JsonValue2SV(name)),
            NumOps::SConvert<s_int_32>(value.GetInt64())
        );
    }
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    JVisitor_VisitMapCtx::K_UInt64
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      /*aCtx*/
)
{
    rapidjson::Document::ConstObject    jObj        = iMitVal->GetObject();
    auto&                               container   = ValGetterT<KeyT>::UInt64(aDataPtr, aProp);

    for (const auto& v: jObj)
    {
        const auto& name    = v.name;
        const auto& value   = v.value;
        container.try_emplace
        (
            ProcessMapKey<KeyT>(_JsonValue2SV(name)),
            NumOps::SConvert<u_int_64>(value.GetUint64())
        );
    }
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    JVisitor_VisitMapCtx::K_SInt64
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      /*aCtx*/
)
{
    rapidjson::Document::ConstObject    jObj        = iMitVal->GetObject();
    auto&                               container   = ValGetterT<KeyT>::SInt64(aDataPtr, aProp);

    for (const auto& v: jObj)
    {
        const auto& name    = v.name;
        const auto& value   = v.value;
        container.try_emplace
        (
            ProcessMapKey<KeyT>(_JsonValue2SV(name)),
            NumOps::SConvert<s_int_64>(value.GetInt64())
        );
    }
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    JVisitor_VisitMapCtx::K_Double
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      /*aCtx*/
)
{
    rapidjson::Document::ConstObject    jObj        = iMitVal->GetObject();
    auto&                               container   = ValGetterT<KeyT>::Double(aDataPtr, aProp);

    for (const auto& v: jObj)
    {
        const auto& name    = v.name;
        const auto& value   = v.value;

        container.try_emplace
        (
            ProcessMapKey<KeyT>(_JsonValue2SV(name)),
            JVisitor_SReadDouble(value)
        );
    }
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    JVisitor_VisitMapCtx::K_Float
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      /*aCtx*/
)
{
    rapidjson::Document::ConstObject    jObj        = iMitVal->GetObject();
    auto&                               container   = ValGetterT<KeyT>::Float(aDataPtr, aProp);

    for (const auto& v: jObj)
    {
        const auto& name    = v.name;
        const auto& value   = v.value;

        container.try_emplace
        (
            ProcessMapKey<KeyT>(_JsonValue2SV(name)),
            NumOps::SConvert<float>(JVisitor_SReadDouble(value))
        );
    }
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    JVisitor_VisitMapCtx::K_Bool
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      /*aCtx*/
)
{
    rapidjson::Document::ConstObject    jObj        = iMitVal->GetObject();
    auto&                               container   = ValGetterT<KeyT>::Bool(aDataPtr, aProp);

    for (const auto& v: jObj)
    {
        const auto& name    = v.name;
        const auto& value   = v.value;
        container.try_emplace
        (
            ProcessMapKey<KeyT>(_JsonValue2SV(name)),
            value.GetBool()
        );
    }
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    JVisitor_VisitMapCtx::K_UUID
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      /*aCtx*/
)
{
    rapidjson::Document::ConstObject    jObj        = iMitVal->GetObject();
    auto&                               container   = ValGetterT<KeyT>::UUID(aDataPtr, aProp);

    for (const auto& v: jObj)
    {
        const auto& name    = v.name;
        const auto& value   = v.value;
        container.try_emplace
        (
            ProcessMapKey<KeyT>(_JsonValue2SV(name)),
            GpUUID::SFromString(_JsonValue2SV(value))
        );
    }
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    JVisitor_VisitMapCtx::K_String
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      /*aCtx*/
)
{
    rapidjson::Document::ConstObject    jObj        = iMitVal->GetObject();
    auto&                               container   = ValGetterT<KeyT>::String(aDataPtr, aProp);

    for (const auto& v: jObj)
    {
        const auto& name    = v.name;
        const auto& value   = v.value;

        std::u8string s(_JsonValue2SV(value));

        container.try_emplace
        (
            ProcessMapKey<KeyT>(_JsonValue2SV(name)),
            std::move(s)
        );
    }
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    JVisitor_VisitMapCtx::K_BLOB
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      /*aCtx*/
)
{
    rapidjson::Document::ConstObject    jObj        = iMitVal->GetObject();
    auto&                               container   = ValGetterT<KeyT>::BLOB(aDataPtr, aProp);

    for (const auto& v: jObj)
    {
        const auto& name    = v.name;
        const auto& value   = v.value;

        GpBytesArray data = GpBase64::SDecodeToByteArray(_JsonValue2SV(value));

        container.try_emplace
        (
            ProcessMapKey<KeyT>(_JsonValue2SV(name)),
            std::move(data)
        );
    }
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    JVisitor_VisitMapCtx::K_ObjectSP
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    rapidjson::Document::ConstObject    jObj        = iMitVal->GetObject();
    auto&                               container   = ValGetterT<KeyT>::ObjectSP(aDataPtr, aProp);
    const GpReflectModel&               model       = GpReflectManager::S().Find(aProp.ModelUid());

    for (const auto& v: jObj)
    {
        const auto& name    = v.name;
        const auto& value   = v.value;

        const rapidjson::Document::ConstObject& jsonObject  = value.GetObject();
        const GpReflectModel&                   modelJson   = GpJsonToObject::SCheckModel
        (
            jsonObject,
            model
        );

        GpReflectObject::SP objectBaseSP    = modelJson.NewInstance();
        GpReflectObject&    objectBase      = objectBaseSP.Vn();
        GpJsonToObject::SReadObject(objectBase, jsonObject, aCtx.iJsonSerializerFlags);

        container.try_emplace
        (
            ProcessMapKey<KeyT>(_JsonValue2SV(name)),
            std::move(objectBaseSP)
        );
    }
}

template<typename T>
T   JVisitor_VisitMapCtx::ProcessMapKey (std::u8string_view aValue)
{
    constexpr const GpReflectType::EnumT type = GpReflectUtils::SDetectType<T>();

    if constexpr (type == GpReflectType::U_INT_8)
    {
        return NumOps::SConvert<u_int_8>(StrOps::SToUI64(aValue));
    } else if constexpr (type == GpReflectType::S_INT_8)
    {
        return NumOps::SConvert<s_int_8>(StrOps::SToSI64(aValue));
    } else if constexpr (type == GpReflectType::U_INT_16)
    {
        return NumOps::SConvert<u_int_16>(StrOps::SToUI64(aValue));
    } else if constexpr (type == GpReflectType::S_INT_16)
    {
        return NumOps::SConvert<s_int_16>(StrOps::SToSI64(aValue));
    } else if constexpr (type == GpReflectType::U_INT_32)
    {
        return NumOps::SConvert<u_int_32>(StrOps::SToUI64(aValue));
    } else if constexpr (type == GpReflectType::S_INT_32)
    {
        return NumOps::SConvert<s_int_32>(StrOps::SToSI64(aValue));
    } else if constexpr (type == GpReflectType::U_INT_64)
    {
        return NumOps::SConvert<u_int_64>(StrOps::SToUI64(aValue));
    } else if constexpr (type == GpReflectType::S_INT_64)
    {
        return NumOps::SConvert<s_int_64>(StrOps::SToSI64(aValue));
    } else if constexpr (type == GpReflectType::DOUBLE)
    {
        return StrOps::SToDouble(aValue);
    } else if constexpr (type == GpReflectType::FLOAT)
    {
        return NumOps::SConvert<float>(StrOps::SToDouble(aValue));
    } else if constexpr (type == GpReflectType::BOOLEAN)
    {
        GpThrowCe<GpException>(u8"Booleans are not supported as map key");
    } else if constexpr (type == GpReflectType::UUID)
    {
        return GpUUID::SFromString(aValue);
    } else if constexpr (type == GpReflectType::STRING)
    {
        return std::u8string(aValue);
    } else if constexpr (type == GpReflectType::BLOB)
    {
        return GpBase64::SDecodeToByteArray(aValue);/*StrOps::SToBytes(aValue);*/
    } else if constexpr (type == GpReflectType::OBJECT)
    {
        GpThrowCe<GpException>(u8"Objects are not supported as a map key");
    } else if constexpr (type == GpReflectType::OBJECT_SP)
    {
        GpThrowCe<GpException>(u8"Objects SP are not supported as a map key");
    } else if constexpr (type == GpReflectType::ENUM)
    {
        GpThrowCe<GpException>(u8"Enums are not supported as a map key");
    } else if constexpr (type == GpReflectType::ENUM_FLAGS)
    {
        GpThrowCe<GpException>(u8"Enum flags are not supported as a map key");
    } else
    {
        GpThrowCe<GpException>(u8"Unknown type '"_sv + GpReflectUtils::SModelName<T>() + u8"'");
    }
}

//------------------------------------- JVisitor ------------------------------------------

class JVisitor
{
public:
    using VisitCtx          = JVisitor_VisitCtx;
    using VisitValueCtx     = JVisitor_VisitValueCtx;
    using VisitContainerCtx = JVisitor_VisitContainerCtx;
    using VisitMapCtx       = JVisitor_VisitMapCtx;
};

}//namespace ObjectVisitor_JsonToObject

//-------------------------------------------------------------------------------

const std::array<std::u8string_view, 18>    GpJsonToObject::sParseErrorCodes =
{
    u8"No error"_sv                                         , //kParseErrorNone = 0,
    u8"The document is empty"_sv                            , //kParseErrorDocumentEmpty,
    u8"The document root must not follow by other values"_sv, //kParseErrorDocumentRootNotSingular,
    u8"Invalid value"_sv                                    , //kParseErrorValueInvalid,
    u8"Missing a name for object member"_sv                 , //kParseErrorObjectMissName,
    u8"Missing a colon after a name of object member"_sv    , //kParseErrorObjectMissColon,
    u8"Missing a comma or '}' after an object member"_sv    , //kParseErrorObjectMissCommaOrCurlyBracket,
    u8"Missing a comma or ']' after an array element"_sv    , //kParseErrorArrayMissCommaOrSquareBracket,
    u8"Incorrect hex digit after \\u escape in string"_sv   , //kParseErrorStringUnicodeEscapeInvalidHex,
    u8"The surrogate pair in string is invalid"_sv          , //kParseErrorStringUnicodeSurrogateInvalid,
    u8"Invalid escape character in string"_sv               , //kParseErrorStringEscapeInvalid,
    u8"Missing a closing quotation mark in string"_sv       , //kParseErrorStringMissQuotationMark,
    u8"Invalid encoding in string"_sv                       , //kParseErrorStringInvalidEncoding,
    u8"Number too big to be stored in double"_sv            , //kParseErrorNumberTooBig,
    u8"Miss fraction part in number"_sv                     , //kParseErrorNumberMissFraction,
    u8"Miss exponent in number"_sv                          , //kParseErrorNumberMissExponent,
    u8"Parsing was terminated"_sv                           , //kParseErrorTermination,
    u8"Unspecific syntax error"_sv                            //kParseErrorUnspecificSyntaxError
};

rapidjson::Document::ConstObject    GpJsonToObject::SParseJsonDom
(
    std::u8string_view      aJsonStr,
    rapidjson::Document&    aJsonDOM
)
{
    THROW_COND_GP
    (
        !aJsonStr.empty(),
        "Json string is empty"_sv
    );

    //Check for errors
    std::string_view jsonStr = GpUTF::S_As_STR(aJsonStr);
    if (aJsonDOM.Parse(jsonStr.data(), jsonStr.size()).HasParseError())
    {
        const rapidjson::ParseErrorCode parseErrorCode = aJsonDOM.GetParseError();
        THROW_GP(u8"JSON parse error: "_sv + sParseErrorCodes.at(size_t(parseErrorCode)));
    }

    //Check for root element is object
    THROW_COND_GP
    (
        aJsonDOM.IsObject(),
        "Json root element must be an object"_sv
    );

    return const_cast<const rapidjson::Document&>(aJsonDOM).GetObject();
}

rapidjson::Document::ConstArray GpJsonToObject::SParseJsonDomVec
(
    std::u8string_view      aJsonStr,
    rapidjson::Document&    aJsonDOM
)
{
    THROW_COND_GP
    (
        !aJsonStr.empty(),
        "Json string is empty"_sv
    );

    //Check for errors
    std::string_view jsonStr = GpUTF::S_As_STR(aJsonStr);
    if (aJsonDOM.Parse(jsonStr.data(), jsonStr.size()).HasParseError())
    {
        const rapidjson::ParseErrorCode parseErrorCode = aJsonDOM.GetParseError();
        THROW_GP(u8"JSON parse error: "_sv + sParseErrorCodes.at(size_t(parseErrorCode)));
    }

    //Check for root element is object
    THROW_COND_GP
    (
        aJsonDOM.IsArray(),
        "Json root element must be an array"_sv
);

    return const_cast<const rapidjson::Document&>(aJsonDOM).GetArray();
}

GpJsonToObject::ParseResT   GpJsonToObject::SParseJsonDomInsitu
(
    GpSpanPtrCharU8RW       aJsonStr,
    rapidjson::Document&    aJsonDOM
)
{
    THROW_COND_GP
    (
        !aJsonStr.IsEmpty(),
        "Json data is null"_sv
    );

    //Check for errors
    if (aJsonDOM.ParseInsitu(reinterpret_cast<char*>(aJsonStr.Ptr())).HasParseError())
    {
        const rapidjson::ParseErrorCode parseErrorCode = aJsonDOM.GetParseError();
        THROW_GP(u8"JSON parse error: "_sv + sParseErrorCodes.at(size_t(parseErrorCode)));
    }

    if (aJsonDOM.IsObject())
    {
        return const_cast<const rapidjson::Document&>(aJsonDOM).GetObject();
    } else if (aJsonDOM.IsArray())
    {
        return const_cast<const rapidjson::Document&>(aJsonDOM).GetArray();
    } else
    {
        THROW_GP(u8"Json root element must be object or array"_sv);
    }
}

void    GpJsonToObject::SReadObject
(
    GpReflectObject&                        aObject,
    const rapidjson::Document::ConstObject& aJsonObject,
    const GpJsonSerializerFlags             aJsonSerializerFlags
)
{
    GpReflectVisitor<GpReflectObject, ObjectVisitor_JsonToObject::JVisitor> visitor;
    ObjectVisitor_JsonToObject::JVisitor_VisitCtx ctx(aJsonObject, aJsonSerializerFlags);

    const GpReflectModel& model= aObject.ReflectModel();
    visitor.Visit(model, aObject.ReflectDataPtr(), ctx);
}

const GpReflectModel&   GpJsonToObject::SCheckModel
(
    const rapidjson::Document::ConstObject& aJsonObject,
    const GpReflectModel&                   aModel
)
{
    std::optional<GpUUID> modelUidJsonOpt = SFindModelUid(aJsonObject);

    if (modelUidJsonOpt.has_value() == false)
    {
        return aModel;
    }

    const GpUUID            modelUidJson    = modelUidJsonOpt.value();
    const GpReflectModel&   modelJson       = GpReflectManager::S().Find(modelUidJson);

    const GpUUID modelUidBase = aModel.Uid();

    const bool isBaseOf = GpReflectManager::S().IsBaseOf
    (
        modelUidBase,
        modelUidJson
    );

    if (isBaseOf)
    {
        return modelJson;
    }

    THROW_GP
    (
        u8"Model in json must be derived from model in code. But code model UID "_sv + modelUidBase
        + u8" and json model UID "_sv + modelUidJson
    );
}

const GpReflectModel&   GpJsonToObject::SCheckModel
(
    const rapidjson::Document::ConstObject& aJsonObject,
    const std::vector<const GpReflectModel*>&   aModelVariants
)
{
    std::optional<GpUUID> modelUidJsonOpt = SFindModelUid(aJsonObject);

    if (modelUidJsonOpt.has_value() == false)
    {
        return *aModelVariants.at(0);
    }

    const GpUUID            modelUidJson    = modelUidJsonOpt.value();
    const GpReflectModel&   modelJson       = GpReflectManager::S().Find(modelUidJson);
    GpUUID                  modelUidBase;

    for (const GpReflectModel* modelVariant: aModelVariants)
    {
        modelUidBase = modelVariant->Uid();

        const bool isBaseOf = GpReflectManager::S().IsBaseOf
        (
            modelUidBase,
            modelUidJson
        );

        if (isBaseOf)
        {
            return modelJson;
        }
    }

    THROW_GP
    (
        u8"Model in json must be derived from model in code. But code model UID "_sv + modelUidBase
        + u8" and json model UID "_sv + modelUidJson
    );
}

GpReflectModel::C::Opt::CRef    GpJsonToObject::SFindModel (const rapidjson::Document::ConstObject& aJsonObject)
{
    std::optional<GpUUID> modelUidJsonOpt = SFindModelUid(aJsonObject);

    if (modelUidJsonOpt.has_value() == false)
    {
        return std::nullopt;
    }

    const GpReflectModel& modelJson = GpReflectManager::S().Find(modelUidJsonOpt.value());
    return modelJson;
}

std::optional<GpUUID>   GpJsonToObject::SFindModelUid (const rapidjson::Document::ConstObject& aJsonObject)
{
    //Detect model uid
    rapidjson::Document::ConstMemberIterator mit = aJsonObject.FindMember("@");

    if (mit == aJsonObject.MemberEnd())
    {
        return std::nullopt;
    }

    const auto& mitUidVal = mit->value;

    const GpUUID modelUid = GpUUID::SFromString(_JsonValue2SV(mitUidVal));

    return modelUid;
}

}//namespace GPlatform
