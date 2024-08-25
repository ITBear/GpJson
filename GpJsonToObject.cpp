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
        THROW_GP("Unsupported type"_sv);
    }

    return res;
}

class JVisitor_VisitCtx
{
public:
                                            JVisitor_VisitCtx   (const rapidjson::Document::ConstObject&    aJsonObject,
                                                                 const GpJsonSerializerFlags                aJsonSerializerFlags,
                                                                 void*                                      aDataPtr) noexcept;

    [[nodiscard]] bool                      OnVisitBegin        (const GpReflectModel& aModel);
    void                                    OnVisitEnd          (const GpReflectModel& aModel);

public:
    const rapidjson::Document::ConstObject& iJsonObject;
    const GpJsonSerializerFlags             iJsonSerializerFlags;
    void*                                   iDataPtr = nullptr;
};

JVisitor_VisitCtx::JVisitor_VisitCtx
(
    const rapidjson::Document::ConstObject& aJsonObject,
    const GpJsonSerializerFlags             aJsonSerializerFlags,
    void*                                   aDataPtr
) noexcept:
iJsonObject         (aJsonObject),
iJsonSerializerFlags(aJsonSerializerFlags),
iDataPtr            (aDataPtr)
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

    bool    OnVisitBegin            (const GpReflectProp&   aProp,
                                     JVisitor_VisitCtx&     aCtx);
    void    OnVisitEnd              (const GpReflectProp&   aProp,
                                     JVisitor_VisitCtx&     aCtx);
    void    UI8                     (const GpReflectProp&   aProp,
                                     JVisitor_VisitCtx&     aCtx);
    void    SI8                     (const GpReflectProp&   aProp,
                                     JVisitor_VisitCtx&     aCtx);
    void    UI16                    (const GpReflectProp&   aProp,
                                     JVisitor_VisitCtx&     aCtx);
    void    SI16                    (const GpReflectProp&   aProp,
                                     JVisitor_VisitCtx&     aCtx);
    void    UI32                    (const GpReflectProp&   aProp,
                                     JVisitor_VisitCtx&     aCtx);
    void    SI32                    (const GpReflectProp&   aProp,
                                     JVisitor_VisitCtx&     aCtx);
    void    UI64                    (const GpReflectProp&   aProp,
                                     JVisitor_VisitCtx&     aCtx);
    void    SI64                    (const GpReflectProp&   aProp,
                                     JVisitor_VisitCtx&     aCtx);
    void    Double                  (const GpReflectProp&   aProp,
                                     JVisitor_VisitCtx&     aCtx);
    void    Float                   (const GpReflectProp&   aProp,
                                     JVisitor_VisitCtx&     aCtx);
    void    Bool                    (const GpReflectProp&   aProp,
                                     JVisitor_VisitCtx&     aCtx);
    void    UUID                    (const GpReflectProp&   aProp,
                                     JVisitor_VisitCtx&     aCtx);
    void    String                  (const GpReflectProp&   aProp,
                                     JVisitor_VisitCtx&     aCtx);
    void    BLOB                    (const GpReflectProp&   aProp,
                                     JVisitor_VisitCtx&     aCtx);
    void    Object                  (const GpReflectProp&   aProp,
                                     JVisitor_VisitCtx&     aCtx);
    void    ObjectSP                (const GpReflectProp&   aProp,
                                     JVisitor_VisitCtx&     aCtx);
    void    Enum                    (const GpReflectProp&   aProp,
                                     JVisitor_VisitCtx&     aCtx);
    void    EnumFlags               (const GpReflectProp&   aProp,
                                     JVisitor_VisitCtx&     aCtx);

public:
    rapidjson::Document::ConstMemberIterator    iMit;
    const rapidjson::Document::GenericValue*    iMitVal = nullptr;
};

bool    JVisitor_VisitValueCtx::OnVisitBegin
(
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    std::string_view propName = aProp.FlagTest(GpReflectPropFlag::NAME_OVERRIDE) ?
                                aProp.FlagArg(GpReflectPropFlag::NAME_OVERRIDE).value() : aProp.Name();

    //Find json member
    iMit = aCtx.iJsonObject.FindMember
    (
        rapidjson::Document::ValueType
        (
            std::data(propName),
            NumOps::SConvert<rapidjson::SizeType>(std::size(propName))
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
    [[maybe_unused]] const GpReflectProp&   aProp,
    [[maybe_unused]] JVisitor_VisitCtx&     aCtx
)
{
    //NOP
}

void    JVisitor_VisitValueCtx::UI8
(
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    if (iMitVal->IsNumber())
    {
        aProp.Value_UI8(aCtx.iDataPtr) = NumOps::SConvert<u_int_8>(iMitVal->GetUint64());
    } else
    {
        aProp.Value_UI8(aCtx.iDataPtr) = NumOps::SConvert<u_int_8>(StrOps::SToUI64(_JsonValue2SV(iMitVal)));
    }
}

void    JVisitor_VisitValueCtx::SI8
(
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    if (iMitVal->IsNumber())
    {
        aProp.Value_SI8(aCtx.iDataPtr) = NumOps::SConvert<s_int_8>(iMitVal->GetInt64());
    } else
    {
        aProp.Value_SI8(aCtx.iDataPtr) = NumOps::SConvert<s_int_8>(StrOps::SToSI64(_JsonValue2SV(iMitVal)));
    }
}

void    JVisitor_VisitValueCtx::UI16
(
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    if (iMitVal->IsNumber())
    {
        aProp.Value_UI16(aCtx.iDataPtr) = NumOps::SConvert<u_int_16>(iMitVal->GetUint64());
    } else
    {
        aProp.Value_UI16(aCtx.iDataPtr) = NumOps::SConvert<u_int_16>(StrOps::SToUI64(_JsonValue2SV(iMitVal)));
    }
}

void    JVisitor_VisitValueCtx::SI16
(
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    if (iMitVal->IsNumber())
    {
        aProp.Value_SI16(aCtx.iDataPtr) = NumOps::SConvert<s_int_16>(iMitVal->GetInt64());
    } else
    {
        aProp.Value_SI16(aCtx.iDataPtr) = NumOps::SConvert<s_int_16>(StrOps::SToSI64(_JsonValue2SV(iMitVal)));
    }
}

void    JVisitor_VisitValueCtx::UI32
(
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    if (iMitVal->IsNumber())
    {
        aProp.Value_UI32(aCtx.iDataPtr) = NumOps::SConvert<u_int_32>(iMitVal->GetUint64());
    } else
    {
        aProp.Value_UI32(aCtx.iDataPtr) = NumOps::SConvert<u_int_32>(StrOps::SToUI64(_JsonValue2SV(iMitVal)));
    }
}

void    JVisitor_VisitValueCtx::SI32
(
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    if (iMitVal->IsNumber())
    {
        aProp.Value_SI32(aCtx.iDataPtr) = NumOps::SConvert<s_int_32>(iMitVal->GetInt64());
    } else
    {
        aProp.Value_SI32(aCtx.iDataPtr) = NumOps::SConvert<s_int_32>(StrOps::SToSI64(_JsonValue2SV(iMitVal)));
    }
}

void    JVisitor_VisitValueCtx::UI64
(
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    if (iMitVal->IsNumber())
    {
        aProp.Value_UI64(aCtx.iDataPtr) = NumOps::SConvert<u_int_64>(iMitVal->GetUint64());
    } else
    {
        aProp.Value_UI64(aCtx.iDataPtr) = NumOps::SConvert<u_int_64>(StrOps::SToUI64(_JsonValue2SV(iMitVal)));
    }
}

void    JVisitor_VisitValueCtx::SI64
(
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    if (iMitVal->IsNumber())
    {
        aProp.Value_SI64(aCtx.iDataPtr) = NumOps::SConvert<s_int_64>(iMitVal->GetInt64());
    } else
    {
        aProp.Value_SI64(aCtx.iDataPtr) = NumOps::SConvert<s_int_64>(StrOps::SToSI64(_JsonValue2SV(iMitVal)));
    }
}

void    JVisitor_VisitValueCtx::Double
(
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    aProp.Value_Double(aCtx.iDataPtr) = JVisitor_SReadDouble(*iMitVal);
}

void    JVisitor_VisitValueCtx::Float
(
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    aProp.Value_Float(aCtx.iDataPtr) = NumOps::SConvert<float>(JVisitor_SReadDouble(*iMitVal));
}

void    JVisitor_VisitValueCtx::Bool
(
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    aProp.Value_Bool(aCtx.iDataPtr) = iMitVal->GetBool();
}

void    JVisitor_VisitValueCtx::UUID
(
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    aProp.Value_UUID(aCtx.iDataPtr) = GpUUID::SFromString(_JsonValue2SV(iMitVal));
}

void    JVisitor_VisitValueCtx::String
(
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    aProp.Value_String(aCtx.iDataPtr) = std::string{_JsonValue2SV(iMitVal)};
}

void    JVisitor_VisitValueCtx::BLOB
(
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    aProp.Value_BLOB(aCtx.iDataPtr) = GpBase64::SDecode<GpBytesArray>(_JsonValue2SV(iMitVal));
}

void    JVisitor_VisitValueCtx::Object
(
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    GpReflectObject&                    object      = aProp.Value_Object(aCtx.iDataPtr);
    rapidjson::Document::ConstObject    jsonObject  = iMitVal->GetObject();
    GpJsonToObject::SReadObject(object, jsonObject, aCtx.iJsonSerializerFlags);
}

void    JVisitor_VisitValueCtx::ObjectSP
(
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    GpReflectModel::CSP                 modelCSP    = GpReflectManager::S().Find(aProp.ModelUid());
    const GpReflectModel&               model       = modelCSP.Vn();
    rapidjson::Document::ConstObject    jsonObject  = iMitVal->GetObject();
    const GpReflectModel&               modelJson   = GpJsonToObject::SCheckModel
    (
        jsonObject,
        model
    );

    GpReflectObject::SP&    objectBaseSP    = aProp.Value_ObjectSP(aCtx.iDataPtr);
                            objectBaseSP    = modelJson.NewInstance();
    GpReflectObject&        objectBase      = objectBaseSP.Vn();

    GpJsonToObject::SReadObject(objectBase, jsonObject, aCtx.iJsonSerializerFlags);
}

void    JVisitor_VisitValueCtx::Enum
(
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    aProp.Value_Enum(aCtx.iDataPtr).FromString(_JsonValue2SV(iMitVal));
}

void    JVisitor_VisitValueCtx::EnumFlags
(
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    rapidjson::Document::ConstArray array       = iMitVal->GetArray();
    GpEnumFlags&                    enumFlags   = aProp.Value_EnumFlags(aCtx.iDataPtr);

    for (const auto& v: array)
    {
        enumFlags.Combine(_JsonValue2SV(v));
    }
}

//------------------------------------- JVisitor_VisitVecCtx ------------------------------------------
class JVisitor_VisitVecCtx
{
public:
            JVisitor_VisitVecCtx    (void) noexcept {}

    bool    OnVisitBegin            (const GpReflectProp&   aProp,
                                     JVisitor_VisitCtx&     aCtx);
    void    OnVisitEnd              (const GpReflectProp&   aProp,
                                     JVisitor_VisitCtx&     aCtx);

    template<typename ValGetterT>
    void    UI8                     (const GpReflectProp&   aProp,
                                     JVisitor_VisitCtx&     aCtx);

    template<typename ValGetterT>
    void    SI8                     (const GpReflectProp&   aProp,
                                     JVisitor_VisitCtx&     aCtx);

    template<typename ValGetterT>
    void    UI16                    (const GpReflectProp&   aProp,
                                     JVisitor_VisitCtx&     aCtx);

    template<typename ValGetterT>
    void    SI16                    (const GpReflectProp&   aProp,
                                     JVisitor_VisitCtx&     aCtx);

    template<typename ValGetterT>
    void    UI32                    (const GpReflectProp&   aProp,
                                     JVisitor_VisitCtx&     aCtx);

    template<typename ValGetterT>
    void    SI32                    (const GpReflectProp&   aProp,
                                     JVisitor_VisitCtx&     aCtx);

    template<typename ValGetterT>
    void    UI64                    (const GpReflectProp&   aProp,
                                     JVisitor_VisitCtx&     aCtx);

    template<typename ValGetterT>
    void    SI64                    (const GpReflectProp&   aProp,
                                     JVisitor_VisitCtx&     aCtx);

    template<typename ValGetterT>
    void    Double                  (const GpReflectProp&   aProp,
                                     JVisitor_VisitCtx&     aCtx);

    template<typename ValGetterT>
    void    Float                   (const GpReflectProp&   aProp,
                                     JVisitor_VisitCtx&     aCtx);

    template<typename ValGetterT>
    void    UUID                    (const GpReflectProp&   aProp,
                                     JVisitor_VisitCtx&     aCtx);

    template<typename ValGetterT>
    void    String                  (const GpReflectProp&   aProp,
                                     JVisitor_VisitCtx&     aCtx);

    template<typename ValGetterT>
    void    BLOB                    (const GpReflectProp&   aProp,
                                     JVisitor_VisitCtx&     aCtx);

    template<typename ValGetterT>
    void    Object                  (const GpReflectProp&   aProp,
                                     JVisitor_VisitCtx&     aCtx);

    template<typename ValGetterT>
    void    ObjectSP                (const GpReflectProp&   aProp,
                                     JVisitor_VisitCtx&     aCtx);

private:
    template<typename T>
    static void         SClear      (std::array<T, 0>&) {}

    template<typename T>
    static void         SClear      (std::vector<T>& aVector) {aVector.clear();}

    static void         SClear      (GpVectorReflectObjWrapBase& aVector) {aVector.clear();}

    template<typename T>
    static void         SReserve    (std::array<T, 0>&,
                                     const size_t) {}

    template<typename T>
    static void         SReserve    (std::vector<T>&    aVector,
                                     const size_t       aSize) {aVector.reserve(aSize);}

    static void         SReserve    (GpVectorReflectObjWrapBase&    aVector,
                                     const size_t                   aSize) {aVector.reserve(aSize);}

    template<typename T,
             typename... Ts>
    void                SEmplaceBack(std::array<T, 0>&,
                                     Ts&&...) {}

    template<typename T,
             typename... Ts>
    void                SEmplaceBack(std::vector<T>&    aVector,
                                     Ts&&...            aArgs) {aVector.emplace_back(std::forward<Ts>(aArgs)...);}

    void                SEmplaceBack(GpVectorReflectObjWrapBase&    aVector,
                                     GpReflectObject&&              aValue) {aVector.emplace_back(std::move(aValue));}

public:
    rapidjson::Document::ConstMemberIterator    iMit;
    const rapidjson::Document::GenericValue*    iMitVal = nullptr;
};

bool    JVisitor_VisitVecCtx::OnVisitBegin
(
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    std::string_view propName = aProp.FlagTest(GpReflectPropFlag::NAME_OVERRIDE) ?
                                aProp.FlagArg(GpReflectPropFlag::NAME_OVERRIDE).value() : aProp.Name();

    //Find json member
    iMit = aCtx.iJsonObject.FindMember
    (
        rapidjson::Document::ValueType
        (
            std::data(propName),
            NumOps::SConvert<rapidjson::SizeType>(std::size(propName))
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

void    JVisitor_VisitVecCtx::OnVisitEnd
(
    [[maybe_unused]] const GpReflectProp&   aProp,
    [[maybe_unused]] JVisitor_VisitCtx&     aCtx
)
{
    //NOP
}

template<typename ValGetterT>
void    JVisitor_VisitVecCtx::UI8
(
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    rapidjson::Document::ConstArray jArray = iMitVal->GetArray();

    auto& container = ValGetterT::UI8(aCtx.iDataPtr, aProp);
    SClear(container);
    SReserve(container, jArray.Size());

    for (const auto& v: jArray)
    {
        SEmplaceBack(container, NumOps::SConvert<u_int_8>(v.GetUint64()));
    }
}

template<typename ValGetterT>
void    JVisitor_VisitVecCtx::SI8
(
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    rapidjson::Document::ConstArray jArray = iMitVal->GetArray();

    auto& container = ValGetterT::SI8(aCtx.iDataPtr, aProp);
    SClear(container);
    SReserve(container, jArray.Size());

    for (const auto& v: jArray)
    {
        SEmplaceBack(container, NumOps::SConvert<s_int_8>(v.GetInt64()));
    }
}

template<typename ValGetterT>
void    JVisitor_VisitVecCtx::UI16
(
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    rapidjson::Document::ConstArray jArray = iMitVal->GetArray();

    auto& container = ValGetterT::UI16(aCtx.iDataPtr, aProp);
    SClear(container);
    SReserve(container, jArray.Size());

    for (const auto& v: jArray)
    {
        SEmplaceBack(container, NumOps::SConvert<u_int_16>(v.GetUint64()));
    }
}

template<typename ValGetterT>
void    JVisitor_VisitVecCtx::SI16
(
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    rapidjson::Document::ConstArray jArray = iMitVal->GetArray();

    auto& container = ValGetterT::SI16(aCtx.iDataPtr, aProp);
    SClear(container);
    SReserve(container, jArray.Size());

    for (const auto& v: jArray)
    {
        SEmplaceBack(container, NumOps::SConvert<s_int_16>(v.GetInt64()));
    }
}

template<typename ValGetterT>
void    JVisitor_VisitVecCtx::UI32
(
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    rapidjson::Document::ConstArray jArray = iMitVal->GetArray();

    auto& container = ValGetterT::UI32(aCtx.iDataPtr, aProp);
    SClear(container);
    SReserve(container, jArray.Size());

    for (const auto& v: jArray)
    {
        SEmplaceBack(container, NumOps::SConvert<u_int_32>(v.GetUint64()));
    }
}

template<typename ValGetterT>
void    JVisitor_VisitVecCtx::SI32
(
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    rapidjson::Document::ConstArray jArray = iMitVal->GetArray();

    auto& container = ValGetterT::SI32(aCtx.iDataPtr, aProp);
    SClear(container);
    SReserve(container, jArray.Size());

    for (const auto& v: jArray)
    {
        SEmplaceBack(container, NumOps::SConvert<s_int_32>(v.GetInt64()));
    }
}

template<typename ValGetterT>
void    JVisitor_VisitVecCtx::UI64
(
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    rapidjson::Document::ConstArray jArray = iMitVal->GetArray();

    auto& container = ValGetterT::UI64(aCtx.iDataPtr, aProp);
    SClear(container);
    SReserve(container, jArray.Size());

    for (const auto& v: jArray)
    {
        SEmplaceBack(container, NumOps::SConvert<u_int_64>(v.GetUint64()));
    }
}

template<typename ValGetterT>
void    JVisitor_VisitVecCtx::SI64
(
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    rapidjson::Document::ConstArray jArray = iMitVal->GetArray();

    auto& container = ValGetterT::SI64(aCtx.iDataPtr, aProp);
    SClear(container);
    SReserve(container, jArray.Size());

    for (const auto& v: jArray)
    {
        SEmplaceBack(container, NumOps::SConvert<s_int_64>(v.GetInt64()));
    }
}

template<typename ValGetterT>
void    JVisitor_VisitVecCtx::Double
(
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    rapidjson::Document::ConstArray jArray = iMitVal->GetArray();

    auto& container = ValGetterT::Double(aCtx.iDataPtr, aProp);
    SClear(container);
    SReserve(container, jArray.Size());

    for (const auto& v: jArray)
    {
        SEmplaceBack(container, JVisitor_SReadDouble(v));
    }
}

template<typename ValGetterT>
void    JVisitor_VisitVecCtx::Float
(
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    rapidjson::Document::ConstArray jArray = iMitVal->GetArray();

    auto& container = ValGetterT::Float(aCtx.iDataPtr, aProp);
    SClear(container);
    SReserve(container, jArray.Size());

    for (const auto& v: jArray)
    {
        SEmplaceBack(container, NumOps::SConvert<float>(JVisitor_SReadDouble(v)));
    }
}

template<typename ValGetterT>
void    JVisitor_VisitVecCtx::UUID
(
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    rapidjson::Document::ConstArray jArray = iMitVal->GetArray();

    auto& container = ValGetterT::UUID(aCtx.iDataPtr, aProp);
    SClear(container);
    SReserve(container, jArray.Size());

    for (const auto& v: jArray)
    {
        SEmplaceBack(container, GpUUID::SFromString(_JsonValue2SV(v)));
    }
}

template<typename ValGetterT>
void    JVisitor_VisitVecCtx::String
(
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    rapidjson::Document::ConstArray jArray = iMitVal->GetArray();

    auto& container = ValGetterT::String(aCtx.iDataPtr, aProp);
    SClear(container);
    SReserve(container, jArray.Size());

    for (const auto& v: jArray)
    {
        SEmplaceBack(container, std::string(_JsonValue2SV(v)));
    }
}

template<typename ValGetterT>
void    JVisitor_VisitVecCtx::BLOB
(
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    rapidjson::Document::ConstArray jArray = iMitVal->GetArray();

    auto& container = ValGetterT::BLOB(aCtx.iDataPtr, aProp);
    SClear(container);
    SReserve(container, jArray.Size());

    for (const auto& v: jArray)
    {
        SEmplaceBack(container, GpBase64::SDecode<GpBytesArray>(_JsonValue2SV(v)));
    }
}

template<typename ValGetterT>
void    JVisitor_VisitVecCtx::Object
(
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    rapidjson::Document::ConstArray jArray = iMitVal->GetArray();

    auto& container = ValGetterT::Object(aCtx.iDataPtr, aProp);
    SClear(container);
    SReserve(container, jArray.Size());

    GpReflectModel::CSP     modelCSP    = GpReflectManager::S().Find(aProp.ModelUid());
    const GpReflectModel&   model       = modelCSP.Vn();

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
void    JVisitor_VisitVecCtx::ObjectSP
(
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    rapidjson::Document::ConstArray jArray = iMitVal->GetArray();

    auto& container = ValGetterT::ObjectSP(aCtx.iDataPtr, aProp);
    SClear(container);
    SReserve(container, jArray.Size());

    GpReflectModel::CSP     modelCSP    = GpReflectManager::S().Find(aProp.ModelUid());
    const GpReflectModel&   model       = modelCSP.Vn();

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

//------------------------------------- JVisitor_VisitMapCtx ------------------------------------------
class JVisitor_VisitMapCtx
{
public:
            JVisitor_VisitMapCtx    (void) noexcept {}

    bool    OnVisitBegin            (const GpReflectProp&   aProp,
                                     JVisitor_VisitCtx&     aCtx);
    void    OnVisitEnd              (const GpReflectProp&   aProp,
                                     JVisitor_VisitCtx&     aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void    K_UI8                   (const GpReflectProp&   aProp,
                                     JVisitor_VisitCtx&     aCtx);
    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void    K_SI8                   (const GpReflectProp&   aProp,
                                     JVisitor_VisitCtx&     aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void    K_UI16                  (const GpReflectProp&   aProp,
                                     JVisitor_VisitCtx&     aCtx);
    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void    K_SI16                  (const GpReflectProp&   aProp,
                                     JVisitor_VisitCtx&     aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void    K_UI32                  (const GpReflectProp&   aProp,
                                     JVisitor_VisitCtx&     aCtx);
    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void    K_SI32                  (const GpReflectProp&   aProp,
                                     JVisitor_VisitCtx&     aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void    K_UI64                  (const GpReflectProp&   aProp,
                                     JVisitor_VisitCtx&     aCtx);
    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void    K_SI64                  (const GpReflectProp&   aProp,
                                     JVisitor_VisitCtx&     aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void    K_Double                (const GpReflectProp&   aProp,
                                     JVisitor_VisitCtx&     aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void    K_Float                 (const GpReflectProp&   aProp,
                                     JVisitor_VisitCtx&     aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void    K_UUID                  (const GpReflectProp&   aProp,
                                     JVisitor_VisitCtx&     aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void    K_String                (const GpReflectProp&   aProp,
                                     JVisitor_VisitCtx&     aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void    K_BLOB                  (const GpReflectProp&   aProp,
                                     JVisitor_VisitCtx&     aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void    K_ObjectSP              (const GpReflectProp&   aProp,
                                     JVisitor_VisitCtx&     aCtx);

private:
    template<typename T>
    T       ProcessMapKey           (std::string_view aValue);

public:
    rapidjson::Document::ConstMemberIterator    iMit;
    const rapidjson::Document::GenericValue*    iMitVal = nullptr;
};

bool    JVisitor_VisitMapCtx::OnVisitBegin
(
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    std::string_view propName = aProp.FlagTest(GpReflectPropFlag::NAME_OVERRIDE) ?
                                aProp.FlagArg(GpReflectPropFlag::NAME_OVERRIDE).value() : aProp.Name();

    //Find json member
    iMit = aCtx.iJsonObject.FindMember
    (
        rapidjson::Document::ValueType
        (
            std::data(propName),
            NumOps::SConvert<rapidjson::SizeType>(std::size(propName))
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
    [[maybe_unused]] const GpReflectProp&   aProp,
    [[maybe_unused]] JVisitor_VisitCtx&     aCtx
)
{
    //NOP
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    JVisitor_VisitMapCtx::K_UI8
(
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    rapidjson::Document::ConstObject    jObj        = iMitVal->GetObject();
    auto&                               container   = ValGetterT<KeyT>::UI8(aCtx.iDataPtr, aProp);

    for (const auto& [name, value]: jObj)
    {
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
void    JVisitor_VisitMapCtx::K_SI8
(
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    rapidjson::Document::ConstObject    jObj        = iMitVal->GetObject();
    auto&                               container   = ValGetterT<KeyT>::SI8(aCtx.iDataPtr, aProp);

    for (const auto& [name, value]: jObj)
    {
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
void    JVisitor_VisitMapCtx::K_UI16
(
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    rapidjson::Document::ConstObject    jObj        = iMitVal->GetObject();
    auto&                               container   = ValGetterT<KeyT>::UI16(aCtx.iDataPtr, aProp);

    for (const auto& [name, value]: jObj)
    {
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
void    JVisitor_VisitMapCtx::K_SI16
(
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    rapidjson::Document::ConstObject    jObj        = iMitVal->GetObject();
    auto&                               container   = ValGetterT<KeyT>::SI16(aCtx.iDataPtr, aProp);

    for (const auto& [name, value]: jObj)
    {
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
void    JVisitor_VisitMapCtx::K_UI32
(
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    rapidjson::Document::ConstObject    jObj        = iMitVal->GetObject();
    auto&                               container   = ValGetterT<KeyT>::UI32(aCtx.iDataPtr, aProp);

    for (const auto& [name, value]: jObj)
    {
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
void    JVisitor_VisitMapCtx::K_SI32
(
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    rapidjson::Document::ConstObject    jObj        = iMitVal->GetObject();
    auto&                               container   = ValGetterT<KeyT>::SI32(aCtx.iDataPtr, aProp);

    for (const auto& [name, value]: jObj)
    {
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
void    JVisitor_VisitMapCtx::K_UI64
(
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    rapidjson::Document::ConstObject    jObj        = iMitVal->GetObject();
    auto&                               container   = ValGetterT<KeyT>::UI64(aCtx.iDataPtr, aProp);

    for (const auto& [name, value]: jObj)
    {
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
void    JVisitor_VisitMapCtx::K_SI64
(
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    rapidjson::Document::ConstObject    jObj        = iMitVal->GetObject();
    auto&                               container   = ValGetterT<KeyT>::SI64(aCtx.iDataPtr, aProp);

    for (const auto& [name, value]: jObj)
    {
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
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    rapidjson::Document::ConstObject    jObj        = iMitVal->GetObject();
    auto&                               container   = ValGetterT<KeyT>::Double(aCtx.iDataPtr, aProp);

    for (const auto& [name, value]: jObj)
    {
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
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    rapidjson::Document::ConstObject    jObj        = iMitVal->GetObject();
    auto&                               container   = ValGetterT<KeyT>::Float(aCtx.iDataPtr, aProp);

    for (const auto& [name, value]: jObj)
    {
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
void    JVisitor_VisitMapCtx::K_UUID
(
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    rapidjson::Document::ConstObject    jObj        = iMitVal->GetObject();
    auto&                               container   = ValGetterT<KeyT>::UUID(aCtx.iDataPtr, aProp);

    for (const auto& [name, value]: jObj)
    {
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
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    rapidjson::Document::ConstObject    jObj        = iMitVal->GetObject();
    auto&                               container   = ValGetterT<KeyT>::String(aCtx.iDataPtr, aProp);

    for (const auto& [name, value]: jObj)
    {
        std::string s(_JsonValue2SV(value));

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
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    rapidjson::Document::ConstObject    jObj        = iMitVal->GetObject();
    auto&                               container   = ValGetterT<KeyT>::BLOB(aCtx.iDataPtr, aProp);

    for (const auto& [name, value]: jObj)
    {
        container.try_emplace
        (
            ProcessMapKey<KeyT>(_JsonValue2SV(name)),
            GpBase64::SDecode<GpBytesArray>(_JsonValue2SV(value))
        );
    }
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    JVisitor_VisitMapCtx::K_ObjectSP
(
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    rapidjson::Document::ConstObject    jObj        = iMitVal->GetObject();
    auto&                               container   = ValGetterT<KeyT>::ObjectSP(aCtx.iDataPtr, aProp);
    GpReflectModel::CSP                 modelCSP    = GpReflectManager::S().Find(aProp.ModelUid());
    const GpReflectModel&               model       = modelCSP.Vn();

    for (const auto& [name, value]: jObj)
    {
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
        GpThrowCe<GpException>("Booleans are not supported as map key");
    } else if constexpr (type == GpReflectType::UUID)
    {
        return GpUUID::SFromString(aValue);
    } else if constexpr (type == GpReflectType::STRING)
    {
        return std::string(aValue);
    } else if constexpr (type == GpReflectType::BLOB)
    {
        return GpBase64::SDecode<GpBytesArray>(aValue);
    } else if constexpr (type == GpReflectType::OBJECT)
    {
        GpThrowCe<GpException>("Objects are not supported as a map key");
    } else if constexpr (type == GpReflectType::OBJECT_SP)
    {
        GpThrowCe<GpException>("Objects SP are not supported as a map key");
    } else if constexpr (type == GpReflectType::ENUM)
    {
        GpThrowCe<GpException>("Enums are not supported as a map key");
    } else if constexpr (type == GpReflectType::ENUM_FLAGS)
    {
        GpThrowCe<GpException>("Enum flags are not supported as a map key");
    } else
    {
        GpThrowCe<GpException>("Unknown type '"_sv + GpReflectUtils::SModelName<T>() + "'");
    }
}

//------------------------------------- JVisitor ------------------------------------------

class JVisitor
{
public:
    using VisitCtx      = JVisitor_VisitCtx;
    using VisitValueCtx = JVisitor_VisitValueCtx;
    using VisitVecCtx   = JVisitor_VisitVecCtx;
    using VisitMapCtx   = JVisitor_VisitMapCtx;
};

}// namespace ObjectVisitor_JsonToObject

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
        !aJsonStr.empty(),
        "Json string is empty"_sv
    );

    //Check for errors
    std::string_view jsonStr = aJsonStr;
    if (aJsonDOM.Parse(std::data(jsonStr), std::size(jsonStr)).HasParseError())
    {
        const rapidjson::ParseErrorCode parseErrorCode = aJsonDOM.GetParseError();
        THROW_GP("JSON parse error: "_sv + sParseErrorCodes.at(size_t(parseErrorCode)));
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
        !aJsonStr.empty(),
        "Json string is empty"_sv
    );

    //Check for errors
    std::string_view jsonStr = aJsonStr;
    if (aJsonDOM.Parse(std::data(jsonStr), std::size(jsonStr)).HasParseError())
    {
        const rapidjson::ParseErrorCode parseErrorCode = aJsonDOM.GetParseError();
        THROW_GP("JSON parse error: "_sv + sParseErrorCodes.at(size_t(parseErrorCode)));
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
    GpSpanCharRW            aJsonStr,
    rapidjson::Document&    aJsonDOM
)
{
    THROW_COND_GP
    (
        !aJsonStr.Empty(),
        "Json data is null"_sv
    );

    //Check for errors
    if (aJsonDOM.ParseInsitu(reinterpret_cast<char*>(aJsonStr.Ptr())).HasParseError())
    {
        const rapidjson::ParseErrorCode parseErrorCode = aJsonDOM.GetParseError();
        THROW_GP("JSON parse error: "_sv + sParseErrorCodes.at(size_t(parseErrorCode)));
    }

    if (aJsonDOM.IsObject())
    {
        return const_cast<const rapidjson::Document&>(aJsonDOM).GetObject();
    } else if (aJsonDOM.IsArray())
    {
        return const_cast<const rapidjson::Document&>(aJsonDOM).GetArray();
    } else
    {
        THROW_GP("Json root element must be object or array"_sv);
    }
}

void    GpJsonToObject::SReadObject
(
    GpReflectObject&                        aObject,
    const rapidjson::Document::ConstObject& aJsonObject,
    const GpJsonSerializerFlags             aJsonSerializerFlags
)
{
    GpReflectVisitor<ObjectVisitor_JsonToObject::JVisitor>  visitor;
    ObjectVisitor_JsonToObject::JVisitor_VisitCtx           ctx(aJsonObject, aJsonSerializerFlags, aObject.ReflectDataPtr());

    GpReflectModel::CSP     modelCSP    = aObject.ReflectModel();
    const GpReflectModel&   model       = modelCSP.Vn();

    visitor.Visit(model, ctx);
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
    GpReflectModel::CSP     modelJsonCSP    = GpReflectManager::S().Find(modelUidJson);
    const GpReflectModel&   modelJson       = modelJsonCSP.Vn();

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
        fmt::format
        (
            "Model in json must be derived from model in code. But code model UID '{}' and json model UID '{}'",
            modelUidBase,
            modelUidJson
        )
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
    GpReflectModel::CSP     modelJsonCSP    = GpReflectManager::S().Find(modelUidJson);
    const GpReflectModel&   modelJson       = modelJsonCSP.Vn();
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
        fmt::format
        (
            "Model in json must be derived from model in code. But code model UID '{}' and json model UID '{}'",
            modelUidBase,
            modelUidJson
        )
    );
}

GpReflectModel::C::Opt::CRef    GpJsonToObject::SFindModel (const rapidjson::Document::ConstObject& aJsonObject)
{
    std::optional<GpUUID> modelUidJsonOpt = SFindModelUid(aJsonObject);

    if (modelUidJsonOpt.has_value() == false)
    {
        return std::nullopt;
    }

    GpReflectModel::CSP     modelJsonCSP    = GpReflectManager::S().Find(modelUidJsonOpt.value());
    const GpReflectModel&   modelJson       = modelJsonCSP.Vn();
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

    const auto&     mitUidVal   = mit->value;
    const GpUUID    modelUid    = GpUUID::SFromString(_JsonValue2SV(mitUidVal));

    return modelUid;
}

}// namespace GPlatform
