#include "GpJsonToObject.hpp"

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
        res = NumOps::SConvert<double>(aJsonValue.IsInt());
    } else if (aJsonValue.IsString())
    {
        const auto r = StrOps::SToNumeric(std::string_view(aJsonValue.GetString(), aJsonValue.GetStringLength()));
        if (std::holds_alternative<double>(r))
        {
            return std::get<double>(r);
        } else
        {
            return NumOps::SConvert<double>(std::get<s_int_64>(r));
        }
    } else
    {
        THROW_GP("Unsupported type"_sv);
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
    std::string_view propName = aProp.FlagTest(GpReflectPropFlag::NAME_OVERRIDE) ? aProp.FlagArg(GpReflectPropFlag::NAME_OVERRIDE).value() : aProp.Name();

    //Find json member
    iMit = aCtx.iJsonObject.FindMember
    (
        rapidjson::Document::ValueType
        (
            propName.data(),
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
        std::string_view sval = std::string_view(iMitVal->GetString(), iMitVal->GetStringLength());
        aProp.Value_UInt8(aDataPtr) = NumOps::SConvert<u_int_8>(StrOps::SToUI64(sval));
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
        std::string_view sval = std::string_view(iMitVal->GetString(), iMitVal->GetStringLength());
        aProp.Value_SInt8(aDataPtr) = NumOps::SConvert<s_int_8>(StrOps::SToSI64(sval));
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
        std::string_view sval = std::string_view(iMitVal->GetString(), iMitVal->GetStringLength());
        aProp.Value_UInt16(aDataPtr) = NumOps::SConvert<u_int_16>(StrOps::SToUI64(sval));
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
        std::string_view sval = std::string_view(iMitVal->GetString(), iMitVal->GetStringLength());
        aProp.Value_SInt16(aDataPtr) = NumOps::SConvert<s_int_16>(StrOps::SToSI64(sval));
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
        std::string_view sval = std::string_view(iMitVal->GetString(), iMitVal->GetStringLength());
        aProp.Value_UInt32(aDataPtr) = NumOps::SConvert<u_int_32>(StrOps::SToUI64(sval));
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
        std::string_view sval = std::string_view(iMitVal->GetString(), iMitVal->GetStringLength());
        aProp.Value_SInt32(aDataPtr) = NumOps::SConvert<s_int_32>(StrOps::SToSI64(sval));
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
        std::string_view sval = std::string_view(iMitVal->GetString(), iMitVal->GetStringLength());
        aProp.Value_UInt64(aDataPtr) = NumOps::SConvert<u_int_64>(StrOps::SToUI64(sval));
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
        std::string_view sval = std::string_view(iMitVal->GetString(), iMitVal->GetStringLength());
        aProp.Value_SInt64(aDataPtr) = NumOps::SConvert<s_int_64>(StrOps::SToSI64(sval));
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
    aProp.Value_UUID(aDataPtr) = GpUUID::SFromString({iMitVal->GetString(), iMitVal->GetStringLength()});
}

void    JVisitor_VisitValueCtx::Value_String
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      /*aCtx*/
)
{
    aProp.Value_String(aDataPtr) = std::string(iMitVal->GetString(), iMitVal->GetStringLength());
}

void    JVisitor_VisitValueCtx::Value_BLOB
(
    void*                   aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      /*aCtx*/
)
{
    std::string_view str(iMitVal->GetString(), iMitVal->GetStringLength());
    aProp.Value_BLOB(aDataPtr) = GpBase64::SDecodeToByteArray(str);
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
    aProp.Value_Enum(aDataPtr).FromString({iMitVal->GetString(), iMitVal->GetStringLength()});
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
        enumFlags.Set(std::string_view(v.GetString(), v.GetStringLength()));
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
    template<typename VT>
    void                ContainerReserve            (GpVector<VT>&  aContainer,
                                                     const size_t   aSize);

    template<typename VT,
             template<typename... TArgs> class CT>
    void                ContainerReserve            (CT<VT>&        aContainer,
                                                     const size_t   aSize);

    template<typename VT,
             typename... Ts>
    void                ContainerInsert             (GpVector<VT>&  aContainer,
                                                     Ts&&...            aArgs);

    template<typename VT,
             typename... Ts>
    void                ContainerInsert             (GpList<VT>&    aContainer,
                                                     Ts&&...        aArgs);

    template<typename VT,
             typename... Ts>
    void                ContainerInsert             (GpSet<VT>& aContainer,
                                                     Ts&&...    aArgs);

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
    std::string_view propName = aProp.FlagTest(GpReflectPropFlag::NAME_OVERRIDE) ? aProp.FlagArg(GpReflectPropFlag::NAME_OVERRIDE).value() : aProp.Name();

    //Find json member
    iMit = aCtx.iJsonObject.FindMember
    (
        rapidjson::Document::ValueType
        (
            propName.data(),
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
        [&](){return "Json value '"_sv + propName + "' must be array"_sv;}
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

template<typename VT>
void    JVisitor_VisitContainerCtx::ContainerReserve
(
    GpVector<VT>&   aContainer,
    const size_t    aSize
)
{
    aContainer.reserve(aSize);
}

template<typename VT,
         template<typename... TArgs> class CT>
void    JVisitor_VisitContainerCtx::ContainerReserve
(
    CT<VT>&         /*aContainer*/,
    const size_t    /*aSize*/
)
{
    //NOP
}

template<typename VT,
         typename... Ts>
void    JVisitor_VisitContainerCtx::ContainerInsert
(
    GpVector<VT>&   aContainer,
    Ts&&...         aArgs
)
{
    aContainer.emplace_back(std::forward<Ts>(aArgs)...);
}

template<typename VT,
         typename... Ts>
void    JVisitor_VisitContainerCtx::ContainerInsert
(
    GpList<VT>& aContainer,
    Ts&&...     aArgs
)
{
    aContainer.emplace_back(std::forward<Ts>(aArgs)...);
}

template<typename VT,
         typename... Ts>
void    JVisitor_VisitContainerCtx::ContainerInsert
(
    GpSet<VT>&  aContainer,
    Ts&&...         aArgs
)
{
    aContainer.insert(std::forward<Ts>(aArgs)...);
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
    container.clear();
    ContainerReserve(container, jArray.Size());

    for (const auto& v: jArray)
    {
        ContainerInsert(container, NumOps::SConvert<u_int_8>(v.GetUint64()));
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
    container.clear();
    ContainerReserve(container, jArray.Size());

    for (const auto& v: jArray)
    {
        ContainerInsert(container, NumOps::SConvert<s_int_8>(v.GetInt64()));
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
    container.clear();
    ContainerReserve(container, jArray.Size());

    for (const auto& v: jArray)
    {
        ContainerInsert(container, NumOps::SConvert<u_int_16>(v.GetUint64()));
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
    container.clear();
    ContainerReserve(container, jArray.Size());

    for (const auto& v: jArray)
    {
        ContainerInsert(container, NumOps::SConvert<s_int_16>(v.GetInt64()));
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
    container.clear();
    ContainerReserve(container, jArray.Size());

    for (const auto& v: jArray)
    {
        ContainerInsert(container, NumOps::SConvert<u_int_32>(v.GetUint64()));
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
    container.clear();
    ContainerReserve(container, jArray.Size());

    for (const auto& v: jArray)
    {
        ContainerInsert(container, NumOps::SConvert<s_int_32>(v.GetInt64()));
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
    container.clear();
    ContainerReserve(container, jArray.Size());

    for (const auto& v: jArray)
    {
        ContainerInsert(container, NumOps::SConvert<u_int_64>(v.GetUint64()));
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
    container.clear();
    ContainerReserve(container, jArray.Size());

    for (const auto& v: jArray)
    {
        ContainerInsert(container, NumOps::SConvert<s_int_64>(v.GetInt64()));
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
    container.clear();
    ContainerReserve(container, jArray.Size());

    for (const auto& v: jArray)
    {
        ContainerInsert(container, JVisitor_SReadDouble(v));
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
    container.clear();
    ContainerReserve(container, jArray.Size());

    for (const auto& v: jArray)
    {
        ContainerInsert(container, NumOps::SConvert<float>(JVisitor_SReadDouble(v)));
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
    container.clear();
    ContainerReserve(container, jArray.Size());

    for (const auto& v: jArray)
    {
        ContainerInsert(container, v.GetBool());
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
    container.clear();
    ContainerReserve(container, jArray.Size());

    for (const auto& v: jArray)
    {
        ContainerInsert(container, GpUUID::SFromString({v.GetString(), v.GetStringLength()}));
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
    container.clear();
    ContainerReserve(container, jArray.Size());

    for (const auto& v: jArray)
    {
        ContainerInsert(container, std::string(v.GetString(), v.GetStringLength()));
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
    container.clear();
    ContainerReserve(container, jArray.Size());

    for (const auto& v: jArray)
    {
        std::string_view str(v.GetString(), v.GetStringLength());
        ContainerInsert(container, GpBase64::SDecodeToByteArray(str));
    }
}

template<typename ValGetterT>
void    JVisitor_VisitContainerCtx::Value_Object
(
    void*                   /*aDataPtr*/,
    const GpReflectProp&    /*aProp*/,
    JVisitor_VisitCtx&      /*aCtx*/
)
{
    THROW_GP("Object arrays are not supported; use Object::SP instead."_sv);
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
    container.clear();
    ContainerReserve(container, jArray.Size());

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

        ContainerInsert(container, std::move(objectBaseSP));
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
    THROW_GP("Enum arrays are not supported."_sv);
}

template<typename ValGetterT>
void    JVisitor_VisitContainerCtx::Value_EnumFlags
(
    void*                   /*aDataPtr*/,
    const GpReflectProp&    /*aProp*/,
    JVisitor_VisitCtx&      /*aCtx*/
)
{
    THROW_GP("Enum flag arrays are not supported."_sv);
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
    T                   ProcessMapKey               (std::string_view aValue);

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
    std::string_view propName = aProp.FlagTest(GpReflectPropFlag::NAME_OVERRIDE) ? aProp.FlagArg(GpReflectPropFlag::NAME_OVERRIDE).value() : aProp.Name();

    //Find json member
    iMit = aCtx.iJsonObject.FindMember
    (
        rapidjson::Document::ValueType
        (
            propName.data(),
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
        [&](){return "Json value '"_sv + propName + "' must be object"_sv;}
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
            ProcessMapKey<KeyT>(std::string_view(name.GetString(), name.GetStringLength())),
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
            ProcessMapKey<KeyT>(std::string_view(name.GetString(), name.GetStringLength())),
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
            ProcessMapKey<KeyT>(std::string_view(name.GetString(), name.GetStringLength())),
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
            ProcessMapKey<KeyT>(std::string_view(name.GetString(), name.GetStringLength())),
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
            ProcessMapKey<KeyT>(std::string_view(name.GetString(), name.GetStringLength())),
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
            ProcessMapKey<KeyT>(std::string_view(name.GetString(), name.GetStringLength())),
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
            ProcessMapKey<KeyT>(std::string_view(name.GetString(), name.GetStringLength())),
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
            ProcessMapKey<KeyT>(std::string_view(name.GetString(), name.GetStringLength())),
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
            ProcessMapKey<KeyT>(std::string_view(name.GetString(), name.GetStringLength())),
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
            ProcessMapKey<KeyT>(std::string_view(name.GetString(), name.GetStringLength())),
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
            ProcessMapKey<KeyT>(std::string_view(name.GetString(), name.GetStringLength())),
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
            ProcessMapKey<KeyT>(std::string_view(name.GetString(), name.GetStringLength())),
            GpUUID::SFromString({value.GetString(), value.GetStringLength()})
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

        std::string s({value.GetString(), value.GetStringLength()});

        container.try_emplace
        (
            ProcessMapKey<KeyT>(std::string_view(name.GetString(), name.GetStringLength())),
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

        std::string_view    str(value.GetString(), value.GetStringLength());
        GpBytesArray        data = GpBase64::SDecodeToByteArray(str);

        container.try_emplace
        (
            ProcessMapKey<KeyT>(std::string_view(name.GetString(), name.GetStringLength())),
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
            ProcessMapKey<KeyT>(std::string_view(name.GetString(), name.GetStringLength())),
            std::move(objectBaseSP)
        );
    }
}

template<typename T>
T   JVisitor_VisitMapCtx::ProcessMapKey (std::string_view aValue)
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
        GpThrowCe<std::out_of_range>("Booleans are not supported as map key");
    } else if constexpr (type == GpReflectType::UUID)
    {
        return GpUUID::SFromString(aValue);
    } else if constexpr (type == GpReflectType::STRING)
    {
        return std::string(aValue);
    } else if constexpr (type == GpReflectType::BLOB)
    {
        return GpBase64::SDecodeToByteArray(aValue);/*StrOps::SToBytes(aValue);*/
    } else if constexpr (type == GpReflectType::OBJECT)
    {
        GpThrowCe<std::out_of_range>("Objects are not supported as a map key");
    } else if constexpr (type == GpReflectType::OBJECT_SP)
    {
        GpThrowCe<std::out_of_range>("Objects SP are not supported as a map key");
    } else if constexpr (type == GpReflectType::ENUM)
    {
        GpThrowCe<std::out_of_range>("Enums are not supported as a map key");
    } else if constexpr (type == GpReflectType::ENUM_FLAGS)
    {
        GpThrowCe<std::out_of_range>("Enum flags are not supported as a map key");
    } else
    {
        GpThrowCe<std::out_of_range>("Unknown type '"_sv + GpReflectUtils::SModelName<T>() + "'");
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

const std::array<std::string_view, 18>  GpJsonToObject::sParseErrorCodes =
{
    "No error"_sv                                         , //kParseErrorNone = 0,
    "The document is empty"_sv                            , //kParseErrorDocumentEmpty,
    "The document root must not follow by other values"_sv, //kParseErrorDocumentRootNotSingular,
    "Invalid value"_sv                                    , //kParseErrorValueInvalid,
    "Missing a name for object member"_sv                 , //kParseErrorObjectMissName,
    "Missing a colon after a name of object member"_sv    , //kParseErrorObjectMissColon,
    "Missing a comma or '}' after an object member"_sv    , //kParseErrorObjectMissCommaOrCurlyBracket,
    "Missing a comma or ']' after an array element"_sv    , //kParseErrorArrayMissCommaOrSquareBracket,
    "Incorrect hex digit after \\u escape in string"_sv   , //kParseErrorStringUnicodeEscapeInvalidHex,
    "The surrogate pair in string is invalid"_sv          , //kParseErrorStringUnicodeSurrogateInvalid,
    "Invalid escape character in string"_sv               , //kParseErrorStringEscapeInvalid,
    "Missing a closing quotation mark in string"_sv       , //kParseErrorStringMissQuotationMark,
    "Invalid encoding in string"_sv                       , //kParseErrorStringInvalidEncoding,
    "Number too big to be stored in double"_sv            , //kParseErrorNumberTooBig,
    "Miss fraction part in number"_sv                     , //kParseErrorNumberMissFraction,
    "Miss exponent in number"_sv                          , //kParseErrorNumberMissExponent,
    "Parsing was terminated"_sv                           , //kParseErrorTermination,
    "Unspecific syntax error"_sv                            //kParseErrorUnspecificSyntaxError
};

rapidjson::Document::ConstObject    GpJsonToObject::SParseJsonDom
(
    std::string_view        aJsonStr,
    rapidjson::Document&    aJsonDOM
)
{
    THROW_COND_GP
    (
        aJsonStr.size() > 0,
        "Json string is empty"_sv
    );

    //Check for errors
    if (aJsonDOM.Parse(aJsonStr.data(), aJsonStr.size()).HasParseError())
    {
        const rapidjson::ParseErrorCode parseErrorCode = aJsonDOM.GetParseError();
        THROW_GP("JSON parse error: "_sv + sParseErrorCodes.at(int(parseErrorCode)));
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
    std::string_view        aJsonStr,
    rapidjson::Document&    aJsonDOM
)
{
    THROW_COND_GP
    (
        aJsonStr.size() > 0,
        "Json string is empty"_sv
    );

    //Check for errors
    if (aJsonDOM.Parse(aJsonStr.data(), aJsonStr.size()).HasParseError())
    {
        const rapidjson::ParseErrorCode parseErrorCode = aJsonDOM.GetParseError();
        THROW_GP("JSON parse error: "_sv + sParseErrorCodes.at(int(parseErrorCode)));
    }

    //Check for root element is object
    THROW_COND_GP
    (
        aJsonDOM.IsArray(),
        "Json root element must be an array"_sv
);

    return const_cast<const rapidjson::Document&>(aJsonDOM).GetArray();
}

rapidjson::Document::ConstObject    GpJsonToObject::SParseJsonDomInsitu
(
    GpSpanPtrCharRW         aJsonStr,
    rapidjson::Document&    aJsonDOM
)
{
    THROW_COND_GP
    (
        !aJsonStr.IsEmpty(),
        "Json data is null"_sv
    );

    //Check for errors
    if (aJsonDOM.ParseInsitu(aJsonStr.Ptr()).HasParseError())
    {
        const rapidjson::ParseErrorCode parseErrorCode = aJsonDOM.GetParseError();
        THROW_GP("JSON parse error: "_sv + sParseErrorCodes.at(int(parseErrorCode)));
    }

    //Check for root element is object
    THROW_COND_GP
    (
        aJsonDOM.IsObject(),
        "Json root element must be object"_sv
    );

    return const_cast<const rapidjson::Document&>(aJsonDOM).GetObject();
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
        "Model in json must be derived from model in code. But code model UID "_sv + modelUidBase
        + " and json model UID "_sv + modelUidJson
    );
}

const GpReflectModel&   GpJsonToObject::SCheckModel
(
    const rapidjson::Document::ConstObject& aJsonObject,
    const GpVector<const GpReflectModel*>&  aModelVariants
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
        "Model in json must be derived from model in code. But code model UID "_sv + modelUidBase
        + " and json model UID "_sv + modelUidJson
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

    const GpUUID modelUid = GpUUID::SFromString({mitUidVal.GetString(), mitUidVal.GetStringLength()});

    return modelUid;
}

}//namespace GPlatform
