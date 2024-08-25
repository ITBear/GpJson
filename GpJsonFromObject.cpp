#include "GpJsonFromObject.hpp"
#include "GpJsonUtilsInternal.hpp"

#include <GpCore2/GpReflection/GpReflectObject.hpp>
#include <GpCore2/GpReflection/GpReflectUtils.hpp>
#include <GpCore2/GpReflection/GpReflectVisitor.hpp>
#include <GpCore2/GpUtils/Encoders/GpBase64.hpp>

#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

namespace GPlatform {

namespace ObjectVisitor_JsonFromObject {

//------------------------------------- JVisitor_VisitCtx ------------------------------------------
class JVisitor_VisitCtx
{
public:
                                        JVisitor_VisitCtx   (rapidjson::Document::GenericValue&     aJsonObject,
                                                             rapidjson::Document::AllocatorType&    aJsonAllocator,
                                                             const GpJsonSerializerFlags            aJsonSerializerFlags,
                                                             const bool                             aIsAsTuple,
                                                             const void*                            aDataPtr) noexcept;

    [[nodiscard]] bool                  OnVisitBegin        (const GpReflectModel& aModel);
    void                                OnVisitEnd          (const GpReflectModel& aModel);

public:
    rapidjson::Document::GenericValue&  iJsonObject;
    rapidjson::Document::AllocatorType& iJsonAllocator;
    const GpJsonSerializerFlags         iJsonSerializerFlags;
    const bool                          iIsAsTuple  = false;
    const void*                         iDataPtr    = nullptr;
};

JVisitor_VisitCtx::JVisitor_VisitCtx
(
    rapidjson::Document::GenericValue&  aJsonObject,
    rapidjson::Document::AllocatorType& aJsonAllocator,
    const GpJsonSerializerFlags         aJsonSerializerFlags,
    const bool                          aIsAsTuple,
    const void*                         aDataPtr
) noexcept:
iJsonObject         (aJsonObject),
iJsonAllocator      (aJsonAllocator),
iJsonSerializerFlags(aJsonSerializerFlags),
iIsAsTuple          (aIsAsTuple),
iDataPtr            (aDataPtr)
{
}

bool    JVisitor_VisitCtx::OnVisitBegin (const GpReflectModel& aModel)
{
    if (!iIsAsTuple)
    {
        if (iJsonSerializerFlags.Test(GpJsonSerializerFlag::WRITE_MODEL_UID))
        {
            rapidjson::Value name, obj;

            name.SetString("@", iJsonAllocator);

            const std::string modelUidStr = aModel.Uid().ToString();

            _JsonSetStr(obj, modelUidStr, iJsonAllocator);
            //obj.SetString(std::data(modelUidStr), NumOps::SConvert<rapidjson::SizeType>(std::size(modelUidStr)), iJsonAllocator);

            iJsonObject.AddMember(name.Move(), obj.Move(), iJsonAllocator);
        }
    }

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
    rapidjson::Value    iJsonMemberName;
    rapidjson::Value    iJsonMemberValue;
};

bool    JVisitor_VisitValueCtx::OnVisitBegin
(
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    if (!aCtx.iIsAsTuple)
    {
        if (aProp.FlagTest(GpReflectPropFlag::NAME_OVERRIDE))
        {
            std::string_view name = aProp.FlagArg(GpReflectPropFlag::NAME_OVERRIDE).value();
            //iJsonMemberName.SetString(std::data(name), NumOps::SConvert<rapidjson::SizeType>(std::size(name)), aCtx.iJsonAllocator);
            _JsonSetStr(iJsonMemberName, name, aCtx.iJsonAllocator);
        } else
        {
            std::string_view propName = aProp.Name();
            //iJsonMemberName.SetString(std::data(propName), NumOps::SConvert<rapidjson::SizeType>(std::size(propName)), aCtx.iJsonAllocator);
            _JsonSetStr(iJsonMemberName, propName, aCtx.iJsonAllocator);
        }
    }

    return true;
}

void    JVisitor_VisitValueCtx::OnVisitEnd
(
    [[maybe_unused]] const GpReflectProp&   aProp,
    JVisitor_VisitCtx&                      aCtx
)
{
    if (aCtx.iIsAsTuple)
    {
        aCtx.iJsonObject.PushBack(iJsonMemberValue.Move(), aCtx.iJsonAllocator);
    } else
    {
        aCtx.iJsonObject.AddMember(iJsonMemberName.Move(), iJsonMemberValue.Move(), aCtx.iJsonAllocator);
    }
}

void    JVisitor_VisitValueCtx::UI8
(
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    iJsonMemberValue.SetUint64(aProp.Value_UI8(aCtx.iDataPtr));
}

void    JVisitor_VisitValueCtx::SI8
(
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    iJsonMemberValue.SetInt64(aProp.Value_SI8(aCtx.iDataPtr));
}

void    JVisitor_VisitValueCtx::UI16
(
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    iJsonMemberValue.SetUint64(aProp.Value_UI16(aCtx.iDataPtr));
}

void    JVisitor_VisitValueCtx::SI16
(
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    iJsonMemberValue.SetInt64(aProp.Value_SI16(aCtx.iDataPtr));
}

void    JVisitor_VisitValueCtx::UI32
(
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    iJsonMemberValue.SetUint64(aProp.Value_UI32(aCtx.iDataPtr));
}

void    JVisitor_VisitValueCtx::SI32
(
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    iJsonMemberValue.SetInt64(aProp.Value_SI32(aCtx.iDataPtr));
}

void    JVisitor_VisitValueCtx::UI64
(
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    iJsonMemberValue.SetUint64(aProp.Value_UI64(aCtx.iDataPtr));
}

void    JVisitor_VisitValueCtx::SI64
(
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    iJsonMemberValue.SetInt64(aProp.Value_SI64(aCtx.iDataPtr));
}

void    JVisitor_VisitValueCtx::Double
(
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    iJsonMemberValue.SetDouble(aProp.Value_Double(aCtx.iDataPtr));
}

void    JVisitor_VisitValueCtx::Float
(
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    iJsonMemberValue.SetDouble(static_cast<double>(aProp.Value_Float(aCtx.iDataPtr)));
}

void    JVisitor_VisitValueCtx::Bool
(
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    iJsonMemberValue.SetBool(aProp.Value_Bool(aCtx.iDataPtr));
}

void    JVisitor_VisitValueCtx::UUID
(
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    const std::string propVal = aProp.Value_UUID(aCtx.iDataPtr).ToString();
    _JsonSetStr(iJsonMemberValue, propVal, aCtx.iJsonAllocator);
}

void    JVisitor_VisitValueCtx::String
(
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    const std::string& propVal = aProp.Value_String(aCtx.iDataPtr);
    _JsonSetStr(iJsonMemberValue, propVal, aCtx.iJsonAllocator);
}

void    JVisitor_VisitValueCtx::BLOB
(
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    const GpBytesArray& blob = aProp.Value_BLOB(aCtx.iDataPtr);
    GpSpanByteR         blobSpan(std::data(blob), std::size(blob));
    const std::string   propVal = GpBase64::SEncode<std::string>(blobSpan, 0);
    _JsonSetStr(iJsonMemberValue, propVal, aCtx.iJsonAllocator);
}

void    JVisitor_VisitValueCtx::Object
(
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    const GpReflectObject&              object      = aProp.Value_Object(aCtx.iDataPtr);
    const bool                          isAsTuple   = aProp.FlagTest(GpReflectPropFlag::AS_TUPLE);
    rapidjson::Document::GenericValue&  obj         = isAsTuple ? iJsonMemberValue.SetArray() : iJsonMemberValue.SetObject();
    GpJsonFromObject::SWrite(object, obj, aCtx.iJsonAllocator, aCtx.iJsonSerializerFlags, isAsTuple);
}

void    JVisitor_VisitValueCtx::ObjectSP
(
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    const GpReflectObject::SP& objectSP = aProp.Value_ObjectSP(aCtx.iDataPtr);

    if (objectSP.IsNULL())
    {
        iJsonMemberValue.SetNull();
    } else
    {
        const GpReflectObject&              object      = objectSP.Vn();
        const bool                          isAsTuple   = aProp.FlagTest(GpReflectPropFlag::AS_TUPLE);
        rapidjson::Document::GenericValue&  obj         = isAsTuple ? iJsonMemberValue.SetArray() : iJsonMemberValue.SetObject();
        GpJsonFromObject::SWrite(object, obj, aCtx.iJsonAllocator, aCtx.iJsonSerializerFlags, isAsTuple);
    }
}

void    JVisitor_VisitValueCtx::Enum
(
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    std::string_view propVal = aProp.Value_Enum(aCtx.iDataPtr).ToString();
    //iJsonMemberValue.SetString(std::data(propVal), NumOps::SConvert<rapidjson::SizeType>(std::size(propVal)), aCtx.iJsonAllocator);
    _JsonSetStr(iJsonMemberValue, propVal, aCtx.iJsonAllocator);
}

void    JVisitor_VisitValueCtx::EnumFlags
(
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    const GpEnumFlags&                  enumFlags       = aProp.Value_EnumFlags(aCtx.iDataPtr);
    std::vector<std::string_view>       enumFlagNames   = enumFlags.ToStringViewArray();
    rapidjson::Document::GenericValue&  jsonArray       = iJsonMemberValue.SetArray();

    for (std::string_view flagName: enumFlagNames)
    {
        rapidjson::Value jv;
        //jv.SetString(std::data(flagName), NumOps::SConvert<rapidjson::SizeType>(std::size(flagName)), aCtx.iJsonAllocator);
        _JsonSetStr(jv, flagName, aCtx.iJsonAllocator);
        jsonArray.PushBack(jv.Move(), aCtx.iJsonAllocator);
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

public:
    rapidjson::Value                    iJsonMemberName;
    rapidjson::Value                    iJsonMemberValue;
    rapidjson::Document::GenericValue*  iJsonArray = nullptr;
};

bool    JVisitor_VisitVecCtx::OnVisitBegin
(
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    if (!aCtx.iIsAsTuple)
    {
        if (aProp.FlagTest(GpReflectPropFlag::NAME_OVERRIDE))
        {
            std::string_view name = aProp.FlagArg(GpReflectPropFlag::NAME_OVERRIDE).value();
            //iJsonMemberName.SetString(std::data(name), NumOps::SConvert<rapidjson::SizeType>(std::size(name)), aCtx.iJsonAllocator);
            _JsonSetStr(iJsonMemberName, name, aCtx.iJsonAllocator);
        } else
        {
            std::string_view    propName = aProp.Name();
            //iJsonMemberName.SetString(std::data(propName), NumOps::SConvert<rapidjson::SizeType>(std::size(propName)), aCtx.iJsonAllocator);
            _JsonSetStr(iJsonMemberName, propName, aCtx.iJsonAllocator);
        }
    }

    iJsonArray = &(iJsonMemberValue.SetArray());

    return true;
}

void    JVisitor_VisitVecCtx::OnVisitEnd
(
    [[maybe_unused]] const GpReflectProp&   aProp,
    JVisitor_VisitCtx&                      aCtx
)
{
    if (aCtx.iIsAsTuple)
    {
        aCtx.iJsonObject.PushBack(iJsonMemberValue.Move(), aCtx.iJsonAllocator);
    } else
    {
        aCtx.iJsonObject.AddMember(iJsonMemberName.Move(), iJsonMemberValue.Move(), aCtx.iJsonAllocator);
    }
}

template<typename ValGetterT>
void    JVisitor_VisitVecCtx::UI8
(
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    const auto& container = ValGetterT::UI8(aCtx.iDataPtr, aProp);
    iJsonArray->Reserve(NumOps::SConvert<rapidjson::SizeType>(std::size(container)), aCtx.iJsonAllocator);

    for (const u_int_8 e: container)
    {
        iJsonArray->PushBack(rapidjson::Value().SetUint64(e), aCtx.iJsonAllocator);
    }
}

template<typename ValGetterT>
void    JVisitor_VisitVecCtx::SI8
(
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    const auto& container = ValGetterT::SI8(aCtx.iDataPtr, aProp);
    iJsonArray->Reserve(NumOps::SConvert<rapidjson::SizeType>(std::size(container)), aCtx.iJsonAllocator);

    for (const s_int_8 e: container)
    {
        iJsonArray->PushBack(rapidjson::Value().SetInt64(e), aCtx.iJsonAllocator);
    }
}

template<typename ValGetterT>
void    JVisitor_VisitVecCtx::UI16
(
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    auto& container = ValGetterT::UI16(aCtx.iDataPtr, aProp);
    iJsonArray->Reserve(NumOps::SConvert<rapidjson::SizeType>(std::size(container)), aCtx.iJsonAllocator);

    for (const u_int_16 e: container)
    {
        iJsonArray->PushBack(rapidjson::Value().SetUint64(e), aCtx.iJsonAllocator);
    }
}

template<typename ValGetterT>
void    JVisitor_VisitVecCtx::SI16
(
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    const auto& container = ValGetterT::SI16(aCtx.iDataPtr, aProp);
    iJsonArray->Reserve(NumOps::SConvert<rapidjson::SizeType>(std::size(container)), aCtx.iJsonAllocator);

    for (const s_int_16 e: container)
    {
        iJsonArray->PushBack(rapidjson::Value().SetInt64(e), aCtx.iJsonAllocator);
    }
}

template<typename ValGetterT>
void    JVisitor_VisitVecCtx::UI32
(
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    const auto& container = ValGetterT::UI32(aCtx.iDataPtr, aProp);
    iJsonArray->Reserve(NumOps::SConvert<rapidjson::SizeType>(std::size(container)), aCtx.iJsonAllocator);

    for (const u_int_32 e: container)
    {
        iJsonArray->PushBack(rapidjson::Value().SetUint64(e), aCtx.iJsonAllocator);
    }
}

template<typename ValGetterT>
void    JVisitor_VisitVecCtx::SI32
(
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    const auto& container = ValGetterT::SI32(aCtx.iDataPtr, aProp);
    iJsonArray->Reserve(NumOps::SConvert<rapidjson::SizeType>(std::size(container)), aCtx.iJsonAllocator);

    for (const s_int_32 e: container)
    {
        iJsonArray->PushBack(rapidjson::Value().SetInt64(e), aCtx.iJsonAllocator);
    }
}

template<typename ValGetterT>
void    JVisitor_VisitVecCtx::UI64
(
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    const auto& container = ValGetterT::UI64(aCtx.iDataPtr, aProp);
    iJsonArray->Reserve(NumOps::SConvert<rapidjson::SizeType>(std::size(container)), aCtx.iJsonAllocator);

    for (const u_int_64 e: container)
    {
        iJsonArray->PushBack(rapidjson::Value().SetUint64(e), aCtx.iJsonAllocator);
    }
}

template<typename ValGetterT>
void    JVisitor_VisitVecCtx::SI64
(
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    const auto& container = ValGetterT::SI64(aCtx.iDataPtr, aProp);
    iJsonArray->Reserve(NumOps::SConvert<rapidjson::SizeType>(std::size(container)), aCtx.iJsonAllocator);

    for (const s_int_64 e: container)
    {
        iJsonArray->PushBack(rapidjson::Value().SetInt64(e), aCtx.iJsonAllocator);
    }
}

template<typename ValGetterT>
void    JVisitor_VisitVecCtx::Double
(
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    const auto& container = ValGetterT::Double(aCtx.iDataPtr, aProp);
    iJsonArray->Reserve(NumOps::SConvert<rapidjson::SizeType>(std::size(container)), aCtx.iJsonAllocator);

    for (const double e: container)
    {
        iJsonArray->PushBack(rapidjson::Value().SetDouble(e), aCtx.iJsonAllocator);
    }
}

template<typename ValGetterT>
void    JVisitor_VisitVecCtx::Float
(
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    const auto& container = ValGetterT::Float(aCtx.iDataPtr, aProp);
    iJsonArray->Reserve(NumOps::SConvert<rapidjson::SizeType>(std::size(container)), aCtx.iJsonAllocator);

    for (const float e: container)
    {
        iJsonArray->PushBack
        (
            rapidjson::Value().SetDouble(static_cast<double>(e)),
            aCtx.iJsonAllocator
        );
    }
}

template<typename ValGetterT>
void    JVisitor_VisitVecCtx::UUID
(
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    const auto& container = ValGetterT::UUID(aCtx.iDataPtr, aProp);
    iJsonArray->Reserve(NumOps::SConvert<rapidjson::SizeType>(std::size(container)), aCtx.iJsonAllocator);

    for (const GpUUID& e: container)
    {
        const std::string s = e.ToString();
        rapidjson::Value jv;
        _JsonSetStr(jv, s, aCtx.iJsonAllocator);

        iJsonArray->PushBack(jv.Move(), aCtx.iJsonAllocator);
    }
}

template<typename ValGetterT>
void    JVisitor_VisitVecCtx::String
(
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    const auto& container = ValGetterT::String(aCtx.iDataPtr, aProp);
    iJsonArray->Reserve(NumOps::SConvert<rapidjson::SizeType>(std::size(container)), aCtx.iJsonAllocator);

    for (const std::string& e: container)
    {
        rapidjson::Value jv;
        //jv.SetString(std::data(e), NumOps::SConvert<rapidjson::SizeType>(std::size(e)), aCtx.iJsonAllocator);
        _JsonSetStr(jv, e, aCtx.iJsonAllocator);

        iJsonArray->PushBack(jv.Move(), aCtx.iJsonAllocator);
    }
}

template<typename ValGetterT>
void    JVisitor_VisitVecCtx::BLOB
(
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    const auto& container = ValGetterT::BLOB(aCtx.iDataPtr, aProp);
    iJsonArray->Reserve(NumOps::SConvert<rapidjson::SizeType>(std::size(container)), aCtx.iJsonAllocator);

    for (const GpBytesArray& e: container)
    {
        const std::string s = GpBase64::SEncode<std::string>(GpSpanByteR(std::data(e), std::size(e)), 0);//StrOps::SFromBytes(e);
        rapidjson::Value jv;
        //jv.SetString(std::data(s), NumOps::SConvert<rapidjson::SizeType>(std::size(s)), aCtx.iJsonAllocator);
        _JsonSetStr(jv, s, aCtx.iJsonAllocator);

        iJsonArray->PushBack(jv.Move(), aCtx.iJsonAllocator);
    }
}

template<typename ValGetterT>
void    JVisitor_VisitVecCtx::Object
(
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    const auto& container = ValGetterT::Object(aCtx.iDataPtr, aProp);
    iJsonArray->Reserve(NumOps::SConvert<rapidjson::SizeType>(std::size(container)), aCtx.iJsonAllocator);

    const size_t size = std::size(container);

    for (size_t id = 0; id < size; id++)
    {
        rapidjson::Value jv;

        const GpReflectObject&              object      = container[id];
        const bool                          isAsTuple   = aProp.FlagTest(GpReflectPropFlag::AS_TUPLE);
        rapidjson::Document::GenericValue&  obj         = isAsTuple ? jv.SetArray() : jv.SetObject();
        GpJsonFromObject::SWrite(object, obj, aCtx.iJsonAllocator, aCtx.iJsonSerializerFlags, isAsTuple);

        iJsonArray->PushBack(jv.Move(), aCtx.iJsonAllocator);
    }
}

template<typename ValGetterT>
void    JVisitor_VisitVecCtx::ObjectSP
(
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    const auto& container = ValGetterT::ObjectSP(aCtx.iDataPtr, aProp);
    iJsonArray->Reserve(NumOps::SConvert<rapidjson::SizeType>(std::size(container)), aCtx.iJsonAllocator);

    for (const GpReflectObject::SP& e: container)
    {
        rapidjson::Value jv;

        if (e.IsNULL())
        {
            jv.SetNull();
        } else
        {
            const GpReflectObject&              object      = e.Vn();
            const bool                          isAsTuple   = aProp.FlagTest(GpReflectPropFlag::AS_TUPLE);
            rapidjson::Document::GenericValue&  obj         = isAsTuple ? jv.SetArray() : jv.SetObject();
            GpJsonFromObject::SWrite(object, obj, aCtx.iJsonAllocator, aCtx.iJsonSerializerFlags, isAsTuple);
        }

        iJsonArray->PushBack(jv.Move(), aCtx.iJsonAllocator);
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

    template<typename CT>
    void    Process                 (const CT&              aContainer,
                                     const GpReflectProp&   aProp,
                                     JVisitor_VisitCtx&     aCtx);

    template<typename VT>
    void    ProcessMapKey           (rapidjson::Value&                      aJValOut,
                                     const VT&                              aValue,
                                     rapidjson::Document::AllocatorType&    aJsonAllocator);

    template<typename VT>
    void    ProcessMapVal           (rapidjson::Value&                      aJValOut,
                                     const VT&                              aValue,
                                     const GpReflectProp&                   aProp,
                                     rapidjson::Document::AllocatorType&    aJsonAllocator,
                                     const GpJsonSerializerFlags&           aJsonSerializerFlags);

public:
    rapidjson::Value                    iJsonMemberName;
    rapidjson::Value                    iJsonMemberValue;
    rapidjson::Document::GenericValue*  iJsonObject = nullptr;
};

bool    JVisitor_VisitMapCtx::OnVisitBegin
(
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    if (!aCtx.iIsAsTuple)
    {
        if (aProp.FlagTest(GpReflectPropFlag::NAME_OVERRIDE))
        {
            std::string_view name = aProp.FlagArg(GpReflectPropFlag::NAME_OVERRIDE).value();
            //iJsonMemberName.SetString(std::data(name), NumOps::SConvert<rapidjson::SizeType>(std::size(name)), aCtx.iJsonAllocator);
            _JsonSetStr(iJsonMemberName, name, aCtx.iJsonAllocator);
        } else
        {
            std::string_view propName = aProp.Name();
            //iJsonMemberName.SetString(std::data(propName), NumOps::SConvert<rapidjson::SizeType>(std::size(propName)), aCtx.iJsonAllocator);
            _JsonSetStr(iJsonMemberName, propName, aCtx.iJsonAllocator);
        }
    }

    iJsonObject = &(iJsonMemberValue.SetObject());

    return true;
}

void    JVisitor_VisitMapCtx::OnVisitEnd
(
    [[maybe_unused]] const GpReflectProp&   aProp,
    JVisitor_VisitCtx&                      aCtx
)
{
    if (aCtx.iIsAsTuple)
    {
        aCtx.iJsonObject.PushBack(iJsonMemberValue.Move(), aCtx.iJsonAllocator);
    } else
    {
        aCtx.iJsonObject.AddMember(iJsonMemberName.Move(), iJsonMemberValue.Move(), aCtx.iJsonAllocator);
    }
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
    const auto& container = ValGetterT<KeyT>::UI8(aCtx.iDataPtr, aProp);
    Process(container, aProp, aCtx);
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
    const auto& container = ValGetterT<KeyT>::SI8(aCtx.iDataPtr, aProp);
    Process(container, aProp, aCtx);
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
    const auto& container = ValGetterT<KeyT>::UI16(aCtx.iDataPtr, aProp);
    Process(container, aProp, aCtx);
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
    const auto& container = ValGetterT<KeyT>::SI16(aCtx.iDataPtr, aProp);
    Process(container, aProp, aCtx);
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
    const auto& container = ValGetterT<KeyT>::UI32(aCtx.iDataPtr, aProp);
    Process(container, aProp, aCtx);
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
    const auto& container = ValGetterT<KeyT>::SI32(aCtx.iDataPtr, aProp);
    Process(container, aProp, aCtx);
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
    const auto& container = ValGetterT<KeyT>::UI64(aCtx.iDataPtr, aProp);
    Process(container, aProp, aCtx);
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
    const auto& container = ValGetterT<KeyT>::SI64(aCtx.iDataPtr, aProp);
    Process(container, aProp, aCtx);
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
    const auto& container = ValGetterT<KeyT>::Double(aCtx.iDataPtr, aProp);
    Process(container, aProp, aCtx);
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
    const auto& container = ValGetterT<KeyT>::Float(aCtx.iDataPtr, aProp);
    Process(container, aProp, aCtx);
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
    const auto& container = ValGetterT<KeyT>::UUID(aCtx.iDataPtr, aProp);
    Process(container, aProp, aCtx);
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
    const auto& container = ValGetterT<KeyT>::String(aCtx.iDataPtr, aProp);
    Process(container, aProp, aCtx);
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
    const auto& container = ValGetterT<KeyT>::BLOB(aCtx.iDataPtr, aProp);
    Process(container, aProp, aCtx);
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
    const auto& container = ValGetterT<KeyT>::ObjectSP(aCtx.iDataPtr, aProp);
    Process(container, aProp, aCtx);
}

template<typename CT>
void    JVisitor_VisitMapCtx::Process
(
    const CT&               aContainer,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    rapidjson::Document::AllocatorType& jsonAllocator       = aCtx.iJsonAllocator;
    const GpJsonSerializerFlags         jsonSerializerFlags = aCtx.iJsonSerializerFlags;

    for (const auto&[k, v]: aContainer)
    {
        //Key
        rapidjson::Value jMapElementKey;
        ProcessMapKey(jMapElementKey, k, jsonAllocator);

        //Value
        rapidjson::Value jMapElementVal;
        ProcessMapVal(jMapElementVal, v, aProp, jsonAllocator, jsonSerializerFlags);

        iJsonObject->AddMember(jMapElementKey.Move(), jMapElementVal.Move(), jsonAllocator);
    }
}

template<typename VT>
void    JVisitor_VisitMapCtx::ProcessMapKey
(
    rapidjson::Value&                   aJValOut,
    const VT&                           aValue,
    rapidjson::Document::AllocatorType& aJsonAllocator
)
{
    constexpr const GpReflectType::EnumT type = GpReflectUtils::SDetectType<VT>();

    if constexpr (type == GpReflectType::U_INT_8)
    {
        const std::string s = std::to_string(aValue);
        _JsonSetStr(aJValOut, s, aJsonAllocator);
    } else if constexpr (type == GpReflectType::S_INT_8)
    {
        const std::string s = std::to_string(aValue);
        _JsonSetStr(aJValOut, s, aJsonAllocator);
    } else if constexpr (type == GpReflectType::U_INT_16)
    {
        const std::string s = std::to_string(aValue);
        _JsonSetStr(aJValOut, s, aJsonAllocator);
    } else if constexpr (type == GpReflectType::S_INT_16)
    {
        const std::string s = std::to_string(aValue);
        _JsonSetStr(aJValOut, s, aJsonAllocator);
    } else if constexpr (type == GpReflectType::U_INT_32)
    {
        const std::string s = std::to_string(aValue);
        _JsonSetStr(aJValOut, s, aJsonAllocator);
    } else if constexpr (type == GpReflectType::S_INT_32)
    {
        const std::string s = std::to_string(aValue);
        _JsonSetStr(aJValOut, s, aJsonAllocator);
    } else if constexpr (type == GpReflectType::U_INT_64)
    {
        const std::string s = std::to_string(aValue);
        _JsonSetStr(aJValOut, s, aJsonAllocator);
    } else if constexpr (type == GpReflectType::S_INT_64)
    {
        const std::string s = std::to_string(aValue);
        _JsonSetStr(aJValOut, s, aJsonAllocator);
    } else if constexpr (type == GpReflectType::DOUBLE)
    {
        const std::string s = std::to_string(aValue);
        _JsonSetStr(aJValOut, s, aJsonAllocator);
    } else if constexpr (type == GpReflectType::FLOAT)
    {
        const std::string s = std::to_string(aValue);
        _JsonSetStr(aJValOut, s, aJsonAllocator);
    } else if constexpr (type == GpReflectType::BOOLEAN)
    {
        std::string_view s = aValue ? "true"_sv : "false"_sv;
        _JsonSetStr(aJValOut, s, aJsonAllocator);
    } else if constexpr (type == GpReflectType::UUID)
    {
        const std::string s = aValue.ToString();
        _JsonSetStr(aJValOut, s, aJsonAllocator);
    } else if constexpr (type == GpReflectType::STRING)
    {
        _JsonSetStr(aJValOut, aValue, aJsonAllocator);
    } else if constexpr (type == GpReflectType::BLOB)
    {
        const std::string s = GpBase64::SEncode<std::string>(GpSpanByteR(std::data(aValue), std::size(aValue)), 0);/*StrOps::SFromBytes(aValue);*/
        _JsonSetStr(aJValOut, s, aJsonAllocator);
    } else if constexpr (type == GpReflectType::OBJECT)
    {
        THROW_GP("Objects are not supported as map key"_sv);
    } else if constexpr (type == GpReflectType::OBJECT_SP)
    {
        THROW_GP("Objects are not supported as map key"_sv);
    } else if constexpr (type == GpReflectType::ENUM)
    {
        THROW_GP("Enums are not supported as map key"_sv);
    } else if constexpr (type == GpReflectType::ENUM_FLAGS)
    {
        THROW_GP("Enums flags are not supported as map key"_sv);
    } else
    {
        GpThrowCe<GpException>("Unknown type '"_sv + GpReflectUtils::SModelName<VT>() + "'"_sv);
    }
}

template<typename VT>
void    JVisitor_VisitMapCtx::ProcessMapVal
(
    rapidjson::Value&                   aJValOut,
    const VT&                           aValue,
    const GpReflectProp&                aProp,
    rapidjson::Document::AllocatorType& aJsonAllocator,
    const GpJsonSerializerFlags&        aJsonSerializerFlags
)
{
    constexpr const GpReflectType::EnumT type = GpReflectUtils::SDetectType<VT>();

    if constexpr (type == GpReflectType::U_INT_8)
    {
        aJValOut.SetUint64(aValue);
    } else if constexpr (type == GpReflectType::S_INT_8)
    {
        aJValOut.SetInt64(aValue);
    } else if constexpr (type == GpReflectType::U_INT_16)
    {
        aJValOut.SetUint64(aValue);
    } else if constexpr (type == GpReflectType::S_INT_16)
    {
        aJValOut.SetInt64(aValue);
    } else if constexpr (type == GpReflectType::U_INT_32)
    {
        aJValOut.SetUint64(aValue);
    } else if constexpr (type == GpReflectType::S_INT_32)
    {
        aJValOut.SetInt64(aValue);
    } else if constexpr (type == GpReflectType::U_INT_64)
    {
        aJValOut.SetUint64(aValue);
    } else if constexpr (type == GpReflectType::S_INT_64)
    {
        aJValOut.SetInt64(aValue);
    } else if constexpr (type == GpReflectType::DOUBLE)
    {
        aJValOut.SetDouble(aValue);
    } else if constexpr (type == GpReflectType::FLOAT)
    {
        aJValOut.SetDouble(double(aValue));
    } else if constexpr (type == GpReflectType::BOOLEAN)
    {
        aJValOut.SetBool(aValue);
    } else if constexpr (type == GpReflectType::UUID)
    {
        const std::string s = aValue.ToString();
        _JsonSetStr(aJValOut, s, aJsonAllocator);
    } else if constexpr (type == GpReflectType::STRING)
    {
        _JsonSetStr(aJValOut, aValue, aJsonAllocator);
    } else if constexpr (type == GpReflectType::BLOB)
    {
        const std::string s = GpBase64::SEncode<std::string>(GpSpanByteR(std::data(aValue), std::size(aValue)), 0);/*StrOps::SFromBytes(aValue);*/
        _JsonSetStr(aJValOut, s, aJsonAllocator);
    } else if constexpr (type == GpReflectType::OBJECT)
    {
        THROW_GP("Object are not supported, use arrays of Object::SP instead"_sv);
    } else if constexpr (type == GpReflectType::OBJECT_SP)
    {
        if (aValue.IsNULL())
        {
            aJValOut.SetNull();
        } else
        {
            const GpReflectObject&              object      = aValue.Vn();
            const bool                          isAsTuple   = aProp.FlagTest(GpReflectPropFlag::AS_TUPLE);
            rapidjson::Document::GenericValue&  obj         = isAsTuple ? aJValOut.SetArray() : aJValOut.SetObject();
            GpJsonFromObject::SWrite(object, obj, aJsonAllocator, aJsonSerializerFlags, isAsTuple);
        }
    } else if constexpr (type == GpReflectType::ENUM)
    {
        THROW_GP("Enums are not supported"_sv);
    } else if constexpr (type == GpReflectType::ENUM_FLAGS)
    {
        THROW_GP("Enums flags are not supported"_sv);
    } else
    {
        GpThrowCe<GpException>("Unknown type '"_sv + GpReflectUtils::SModelName<VT>() + "'"_sv);
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

}// namespace ObjectVisitor_JsonFromObject

//-------------------------------------------------------------------------------

std::string GpJsonFromObject::SToString (const rapidjson::Document& aJsonDOM)
{
    rapidjson::StringBuffer                     buffer;
    rapidjson::Writer<rapidjson::StringBuffer>  writer(buffer);
    aJsonDOM.Accept(writer);

    return std::string(std::string_view(buffer.GetString(), buffer.GetSize()));
}

void    GpJsonFromObject::SToString
(
    const rapidjson::Document&  aJsonDOM,
    GpByteWriter&               aWriter
)
{
    aWriter.Bytes(SToString(aJsonDOM));
}

void    GpJsonFromObject::SWrite
(
    const GpReflectObject&              aObject,
    rapidjson::Document::GenericValue&  aJsonObject,
    rapidjson::Document::AllocatorType& aJsonAllocator,
    const GpJsonSerializerFlags         aJsonSerializerFlags,
    const bool                          aIsAsTuple
)
{
    GpReflectVisitor<ObjectVisitor_JsonFromObject::JVisitor> visitor;
    ObjectVisitor_JsonFromObject::JVisitor_VisitCtx ctx
    (
        aJsonObject,
        aJsonAllocator,
        aJsonSerializerFlags,
        aIsAsTuple,
        aObject.ReflectDataPtr()
    );

    GpReflectModel::CSP modelCSP = aObject.ReflectModel();
    visitor.Visit(modelCSP.Vn(), ctx);
}

}// namespace GPlatform
