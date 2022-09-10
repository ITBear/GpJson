#include "GpJsonFromObject.hpp"

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
                                                             const bool                             aIsAsTuple) noexcept;

    [[nodiscard]] bool                  OnVisitBegin        (const GpReflectModel& aModel);
    void                                OnVisitEnd          (const GpReflectModel& aModel);

public:
    rapidjson::Document::GenericValue&  iJsonObject;
    rapidjson::Document::AllocatorType& iJsonAllocator;
    const GpJsonSerializerFlags         iJsonSerializerFlags;
    const bool                          iIsAsTuple = false;
};

JVisitor_VisitCtx::JVisitor_VisitCtx
(
    rapidjson::Document::GenericValue&  aJsonObject,
    rapidjson::Document::AllocatorType& aJsonAllocator,
    const GpJsonSerializerFlags         aJsonSerializerFlags,
    const bool                          aIsAsTuple
) noexcept:
iJsonObject(aJsonObject),
iJsonAllocator(aJsonAllocator),
iJsonSerializerFlags(aJsonSerializerFlags),
iIsAsTuple(aIsAsTuple)
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
            obj.SetString(modelUidStr.data(), NumOps::SConvert<rapidjson::SizeType>(modelUidStr.length()), iJsonAllocator);

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

    [[nodiscard]] bool  OnVisitBegin            (const void*            aDataPtr,
                                                 const GpReflectProp&   aProp,
                                                 JVisitor_VisitCtx&     aCtx);
    void                OnVisitEnd              (const void*            aDataPtr,
                                                 const GpReflectProp&   aProp,
                                                 JVisitor_VisitCtx&     aCtx);
    void                Value_UInt8             (const void*            aDataPtr,
                                                 const GpReflectProp&   aProp,
                                                 JVisitor_VisitCtx&     aCtx);
    void                Value_SInt8             (const void*            aDataPtr,
                                                 const GpReflectProp&   aProp,
                                                 JVisitor_VisitCtx&     aCtx);
    void                Value_UInt16            (const void*            aDataPtr,
                                                 const GpReflectProp&   aProp,
                                                 JVisitor_VisitCtx&     aCtx);
    void                Value_SInt16            (const void*            aDataPtr,
                                                 const GpReflectProp&   aProp,
                                                 JVisitor_VisitCtx&     aCtx);
    void                Value_UInt32            (const void*            aDataPtr,
                                                 const GpReflectProp&   aProp,
                                                 JVisitor_VisitCtx&     aCtx);
    void                Value_SInt32            (const void*            aDataPtr,
                                                 const GpReflectProp&   aProp,
                                                 JVisitor_VisitCtx&     aCtx);
    void                Value_UInt64            (const void*            aDataPtr,
                                                 const GpReflectProp&   aProp,
                                                 JVisitor_VisitCtx&     aCtx);
    void                Value_SInt64            (const void*            aDataPtr,
                                                 const GpReflectProp&   aProp,
                                                 JVisitor_VisitCtx&     aCtx);
    void                Value_Double            (const void*            aDataPtr,
                                                 const GpReflectProp&   aProp,
                                                 JVisitor_VisitCtx&     aCtx);
    void                Value_Float             (const void*            aDataPtr,
                                                 const GpReflectProp&   aProp,
                                                 JVisitor_VisitCtx&     aCtx);
    void                Value_Bool              (const void*            aDataPtr,
                                                 const GpReflectProp&   aProp,
                                                 JVisitor_VisitCtx&     aCtx);
    void                Value_UUID              (const void*            aDataPtr,
                                                 const GpReflectProp&   aProp,
                                                 JVisitor_VisitCtx&     aCtx);
    void                Value_String            (const void*            aDataPtr,
                                                 const GpReflectProp&   aProp,
                                                 JVisitor_VisitCtx&     aCtx);
    void                Value_BLOB              (const void*            aDataPtr,
                                                 const GpReflectProp&   aProp,
                                                 JVisitor_VisitCtx&     aCtx);
    void                Value_Object            (const void*            aDataPtr,
                                                 const GpReflectProp&   aProp,
                                                 JVisitor_VisitCtx&     aCtx);
    void                Value_ObjectSP          (const void*            aDataPtr,
                                                 const GpReflectProp&   aProp,
                                                 JVisitor_VisitCtx&     aCtx);
    void                Value_Enum              (const void*            aDataPtr,
                                                 const GpReflectProp&   aProp,
                                                 JVisitor_VisitCtx&     aCtx);
    void                Value_EnumFlags         (const void*            aDataPtr,
                                                 const GpReflectProp&   aProp,
                                                 JVisitor_VisitCtx&     aCtx);

public:
    rapidjson::Value    iJsonMemberName;
    rapidjson::Value    iJsonMemberValue;
};

bool    JVisitor_VisitValueCtx::OnVisitBegin
(
    const void*             /*aDataPtr*/,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    if (!aCtx.iIsAsTuple)
    {
        if (aProp.FlagTest(GpReflectPropFlag::NAME_OVERRIDE))
        {
            std::string_view name = aProp.FlagArg(GpReflectPropFlag::NAME_OVERRIDE).value();
            iJsonMemberName.SetString(name.data(), NumOps::SConvert<rapidjson::SizeType>(name.length()), aCtx.iJsonAllocator);
        } else
        {
            std::string_view propName = aProp.Name();
            iJsonMemberName.SetString(propName.data(), NumOps::SConvert<rapidjson::SizeType>(propName.length()), aCtx.iJsonAllocator);
        }
    }

    return true;
}

void    JVisitor_VisitValueCtx::OnVisitEnd
(
    const void*             /*aDataPtr*/,
    const GpReflectProp&    /*aProp*/,
    JVisitor_VisitCtx&      aCtx
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

void    JVisitor_VisitValueCtx::Value_UInt8
(
    const void*             aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      /*aCtx*/
)
{
    iJsonMemberValue.SetUint64(aProp.Value_UInt8(aDataPtr));
}

void    JVisitor_VisitValueCtx::Value_SInt8
(
    const void*             aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      /*aCtx*/
)
{
    iJsonMemberValue.SetInt64(aProp.Value_SInt8(aDataPtr));
}

void    JVisitor_VisitValueCtx::Value_UInt16
(
    const void*             aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      /*aCtx*/
)
{
    iJsonMemberValue.SetUint64(aProp.Value_UInt16(aDataPtr));
}

void    JVisitor_VisitValueCtx::Value_SInt16
(
    const void*             aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      /*aCtx*/
)
{
    iJsonMemberValue.SetInt64(aProp.Value_SInt16(aDataPtr));
}

void    JVisitor_VisitValueCtx::Value_UInt32
(
    const void*             aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      /*aCtx*/
)
{
    iJsonMemberValue.SetUint64(aProp.Value_UInt32(aDataPtr));
}

void    JVisitor_VisitValueCtx::Value_SInt32
(
    const void*             aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      /*aCtx*/
)
{
    iJsonMemberValue.SetInt64(aProp.Value_SInt32(aDataPtr));
}

void    JVisitor_VisitValueCtx::Value_UInt64
(
    const void*             aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      /*aCtx*/
)
{
    iJsonMemberValue.SetUint64(aProp.Value_UInt64(aDataPtr));
}

void    JVisitor_VisitValueCtx::Value_SInt64
(
    const void*             aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      /*aCtx*/
)
{
    iJsonMemberValue.SetInt64(aProp.Value_SInt64(aDataPtr));
}

void    JVisitor_VisitValueCtx::Value_Double
(
    const void*             aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      /*aCtx*/
)
{
    iJsonMemberValue.SetDouble(aProp.Value_Double(aDataPtr));
}

void    JVisitor_VisitValueCtx::Value_Float
(
    const void*             aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      /*aCtx*/
)
{
    iJsonMemberValue.SetDouble(aProp.Value_Float(aDataPtr));
}

void    JVisitor_VisitValueCtx::Value_Bool
(
    const void*             aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      /*aCtx*/
)
{
    iJsonMemberValue.SetBool(aProp.Value_Bool(aDataPtr));
}

void    JVisitor_VisitValueCtx::Value_UUID
(
    const void*             aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    const std::string propVal = aProp.Value_UUID(aDataPtr).ToString();
    iJsonMemberValue.SetString(propVal.data(), NumOps::SConvert<rapidjson::SizeType>(propVal.length()), aCtx.iJsonAllocator);
}

void    JVisitor_VisitValueCtx::Value_String
(
    const void*             aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    const std::string& propVal = aProp.Value_String(aDataPtr);
    iJsonMemberValue.SetString(propVal.data(), NumOps::SConvert<rapidjson::SizeType>(propVal.length()), aCtx.iJsonAllocator);
}

void    JVisitor_VisitValueCtx::Value_BLOB
(
    const void*             aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    const std::string propVal = GpBase64::SEncodeToStr(aProp.Value_BLOB(aDataPtr), 0);//StrOps::SFromBytes(aProp.Value_BLOB(aDataPtr));
    iJsonMemberValue.SetString(propVal.data(), NumOps::SConvert<rapidjson::SizeType>(propVal.length()), aCtx.iJsonAllocator);
}

void    JVisitor_VisitValueCtx::Value_Object
(
    const void*             aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    const GpReflectObject&              object      = aProp.Value_Object(aDataPtr);
    const bool                          isAsTuple   = aProp.FlagTest(GpReflectPropFlag::AS_TUPLE);
    rapidjson::Document::GenericValue&  obj         = isAsTuple ? iJsonMemberValue.SetArray() : iJsonMemberValue.SetObject();
    GpJsonFromObject::SWrite(object, obj, aCtx.iJsonAllocator, aCtx.iJsonSerializerFlags, isAsTuple);
}

void    JVisitor_VisitValueCtx::Value_ObjectSP
(
    const void*             aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    const GpReflectObject::SP& objectSP = aProp.Value_ObjectSP(aDataPtr);

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

void    JVisitor_VisitValueCtx::Value_Enum
(
    const void*             aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    std::string_view propVal = aProp.Value_Enum(aDataPtr).ToString();
    iJsonMemberValue.SetString(propVal.data(), NumOps::SConvert<rapidjson::SizeType>(propVal.length()), aCtx.iJsonAllocator);
}

void    JVisitor_VisitValueCtx::Value_EnumFlags
(
    const void*             aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    const GpEnumFlags&                  enumFlags       = aProp.Value_EnumFlags(aDataPtr);
    GpVector<std::string_view>          enumFlagNames   = enumFlags.ToStringViewArray();
    rapidjson::Document::GenericValue&  jsonArray       = iJsonMemberValue.SetArray();

    for (std::string_view flagName: enumFlagNames)
    {
        rapidjson::Value jv;
        jv.SetString(flagName.data(), NumOps::SConvert<rapidjson::SizeType>(flagName.length()), aCtx.iJsonAllocator);
        jsonArray.PushBack(jv.Move(), aCtx.iJsonAllocator);
    }
}

//------------------------------------- JVisitor_VisitContainerCtx ------------------------------------------
class JVisitor_VisitContainerCtx
{
public:
                        JVisitor_VisitContainerCtx  (void) noexcept {}

    [[nodiscard]] bool  OnVisitBegin                (const void*            aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     JVisitor_VisitCtx&     aCtx);
    void                OnVisitEnd                  (const void*            aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     JVisitor_VisitCtx&     aCtx);

    template<typename ValGetterT>
    void                Value_UInt8                 (const void*            aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     JVisitor_VisitCtx&     aCtx);

    template<typename ValGetterT>
    void                Value_SInt8                 (const void*            aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     JVisitor_VisitCtx&     aCtx);

    template<typename ValGetterT>
    void                Value_UInt16                (const void*            aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     JVisitor_VisitCtx&     aCtx);

    template<typename ValGetterT>
    void                Value_SInt16                (const void*            aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     JVisitor_VisitCtx&     aCtx);

    template<typename ValGetterT>
    void                Value_UInt32                (const void*            aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     JVisitor_VisitCtx&     aCtx);

    template<typename ValGetterT>
    void                Value_SInt32                (const void*            aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     JVisitor_VisitCtx&     aCtx);

    template<typename ValGetterT>
    void                Value_UInt64                (const void*            aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     JVisitor_VisitCtx&     aCtx);

    template<typename ValGetterT>
    void                Value_SInt64                (const void*            aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     JVisitor_VisitCtx&     aCtx);

    template<typename ValGetterT>
    void                Value_Double                (const void*            aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     JVisitor_VisitCtx&     aCtx);

    template<typename ValGetterT>
    void                Value_Float                 (const void*            aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     JVisitor_VisitCtx&     aCtx);

    template<typename ValGetterT>
    void                Value_Bool                  (const void*            aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     JVisitor_VisitCtx&     aCtx);

    template<typename ValGetterT>
    void                Value_UUID                  (const void*            aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     JVisitor_VisitCtx&     aCtx);

    template<typename ValGetterT>
    void                Value_String                (const void*            aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     JVisitor_VisitCtx&     aCtx);

    template<typename ValGetterT>
    void                Value_BLOB                  (const void*            aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     JVisitor_VisitCtx&     aCtx);

    template<typename ValGetterT>
    void                Value_Object                (const void*            aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     JVisitor_VisitCtx&     aCtx);

    template<typename ValGetterT>
    void                Value_ObjectSP              (const void*            aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     JVisitor_VisitCtx&     aCtx);

    template<typename ValGetterT>
    void                Value_Enum                  (const void*            aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     JVisitor_VisitCtx&     aCtx);

    template<typename ValGetterT>
    void                Value_EnumFlags             (const void*            aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     JVisitor_VisitCtx&     aCtx);

public:
    rapidjson::Value                    iJsonMemberName;
    rapidjson::Value                    iJsonMemberValue;
    rapidjson::Document::GenericValue*  iJsonArray = nullptr;
};

bool    JVisitor_VisitContainerCtx::OnVisitBegin
(
    const void*             /*aDataPtr*/,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    if (!aCtx.iIsAsTuple)
    {
        if (aProp.FlagTest(GpReflectPropFlag::NAME_OVERRIDE))
        {
            std::string_view name = aProp.FlagArg(GpReflectPropFlag::NAME_OVERRIDE).value();
            iJsonMemberName.SetString(name.data(), NumOps::SConvert<rapidjson::SizeType>(name.length()), aCtx.iJsonAllocator);
        } else
        {
            std::string_view    propName = aProp.Name();
            iJsonMemberName.SetString(propName.data(), NumOps::SConvert<rapidjson::SizeType>(propName.length()), aCtx.iJsonAllocator);
        }
    }

    iJsonArray = &(iJsonMemberValue.SetArray());

    return true;
}

void    JVisitor_VisitContainerCtx::OnVisitEnd
(
    const void*             /*aDataPtr*/,
    const GpReflectProp&    /*aProp*/,
    JVisitor_VisitCtx&      aCtx
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
void    JVisitor_VisitContainerCtx::Value_UInt8
(
    const void*             aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    auto& container = ValGetterT::UInt8(aDataPtr, aProp);
    iJsonArray->Reserve(NumOps::SConvert<rapidjson::SizeType>(container.size()), aCtx.iJsonAllocator);

    for (const u_int_8 e: container)
    {
        iJsonArray->PushBack(rapidjson::Value().SetUint64(e), aCtx.iJsonAllocator);
    }
}

template<typename ValGetterT>
void    JVisitor_VisitContainerCtx::Value_SInt8
(
    const void*             aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    auto& container = ValGetterT::SInt8(aDataPtr, aProp);
    iJsonArray->Reserve(NumOps::SConvert<rapidjson::SizeType>(container.size()), aCtx.iJsonAllocator);

    for (const s_int_8 e: container)
    {
        iJsonArray->PushBack(rapidjson::Value().SetInt64(e), aCtx.iJsonAllocator);
    }
}

template<typename ValGetterT>
void    JVisitor_VisitContainerCtx::Value_UInt16
(
    const void*             aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    auto& container = ValGetterT::UInt16(aDataPtr, aProp);
    iJsonArray->Reserve(NumOps::SConvert<rapidjson::SizeType>(container.size()), aCtx.iJsonAllocator);

    for (const u_int_16 e: container)
    {
        iJsonArray->PushBack(rapidjson::Value().SetUint64(e), aCtx.iJsonAllocator);
    }
}

template<typename ValGetterT>
void    JVisitor_VisitContainerCtx::Value_SInt16
(
    const void*             aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    auto& container = ValGetterT::SInt16(aDataPtr, aProp);
    iJsonArray->Reserve(NumOps::SConvert<rapidjson::SizeType>(container.size()), aCtx.iJsonAllocator);

    for (const s_int_16 e: container)
    {
        iJsonArray->PushBack(rapidjson::Value().SetInt64(e), aCtx.iJsonAllocator);
    }
}

template<typename ValGetterT>
void    JVisitor_VisitContainerCtx::Value_UInt32
(
    const void*             aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    auto& container = ValGetterT::UInt32(aDataPtr, aProp);
    iJsonArray->Reserve(NumOps::SConvert<rapidjson::SizeType>(container.size()), aCtx.iJsonAllocator);

    for (const u_int_32 e: container)
    {
        iJsonArray->PushBack(rapidjson::Value().SetUint64(e), aCtx.iJsonAllocator);
    }
}

template<typename ValGetterT>
void    JVisitor_VisitContainerCtx::Value_SInt32
(
    const void*             aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    auto& container = ValGetterT::SInt32(aDataPtr, aProp);
    iJsonArray->Reserve(NumOps::SConvert<rapidjson::SizeType>(container.size()), aCtx.iJsonAllocator);

    for (const s_int_32 e: container)
    {
        iJsonArray->PushBack(rapidjson::Value().SetInt64(e), aCtx.iJsonAllocator);
    }
}

template<typename ValGetterT>
void    JVisitor_VisitContainerCtx::Value_UInt64
(
    const void*             aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    auto& container = ValGetterT::UInt64(aDataPtr, aProp);
    iJsonArray->Reserve(NumOps::SConvert<rapidjson::SizeType>(container.size()), aCtx.iJsonAllocator);

    for (const u_int_64 e: container)
    {
        iJsonArray->PushBack(rapidjson::Value().SetUint64(e), aCtx.iJsonAllocator);
    }
}

template<typename ValGetterT>
void    JVisitor_VisitContainerCtx::Value_SInt64
(
    const void*             aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    auto& container = ValGetterT::SInt64(aDataPtr, aProp);
    iJsonArray->Reserve(NumOps::SConvert<rapidjson::SizeType>(container.size()), aCtx.iJsonAllocator);

    for (const s_int_64 e: container)
    {
        iJsonArray->PushBack(rapidjson::Value().SetInt64(e), aCtx.iJsonAllocator);
    }
}

template<typename ValGetterT>
void    JVisitor_VisitContainerCtx::Value_Double
(
    const void*             aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    auto& container = ValGetterT::Double(aDataPtr, aProp);
    iJsonArray->Reserve(NumOps::SConvert<rapidjson::SizeType>(container.size()), aCtx.iJsonAllocator);

    for (const double e: container)
    {
        iJsonArray->PushBack(rapidjson::Value().SetDouble(e), aCtx.iJsonAllocator);
    }
}

template<typename ValGetterT>
void    JVisitor_VisitContainerCtx::Value_Float
(
    const void*             aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    auto& container = ValGetterT::Float(aDataPtr, aProp);
    iJsonArray->Reserve(NumOps::SConvert<rapidjson::SizeType>(container.size()), aCtx.iJsonAllocator);

    for (const float e: container)
    {
        iJsonArray->PushBack(rapidjson::Value().SetDouble(e), aCtx.iJsonAllocator);
    }
}

template<typename ValGetterT>
void    JVisitor_VisitContainerCtx::Value_Bool
(
    const void*             aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    auto& container = ValGetterT::Bool(aDataPtr, aProp);
    iJsonArray->Reserve(NumOps::SConvert<rapidjson::SizeType>(container.size()), aCtx.iJsonAllocator);

    for (const bool e: container)
    {
        iJsonArray->PushBack(rapidjson::Value().SetBool(e), aCtx.iJsonAllocator);
    }
}

template<typename ValGetterT>
void    JVisitor_VisitContainerCtx::Value_UUID
(
    const void*             aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    auto& container = ValGetterT::UUID(aDataPtr, aProp);
    iJsonArray->Reserve(NumOps::SConvert<rapidjson::SizeType>(container.size()), aCtx.iJsonAllocator);

    for (const GpUUID& e: container)
    {
        const std::string s = e.ToString();
        rapidjson::Value jv;
        jv.SetString(s.data(), NumOps::SConvert<rapidjson::SizeType>(s.length()), aCtx.iJsonAllocator);

        iJsonArray->PushBack(jv.Move(), aCtx.iJsonAllocator);
    }
}

template<typename ValGetterT>
void    JVisitor_VisitContainerCtx::Value_String
(
    const void*             aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    auto& container = ValGetterT::String(aDataPtr, aProp);
    iJsonArray->Reserve(NumOps::SConvert<rapidjson::SizeType>(container.size()), aCtx.iJsonAllocator);

    for (const std::string& e: container)
    {
        rapidjson::Value jv;
        jv.SetString(e.data(), NumOps::SConvert<rapidjson::SizeType>(e.length()), aCtx.iJsonAllocator);

        iJsonArray->PushBack(jv.Move(), aCtx.iJsonAllocator);
    }
}

template<typename ValGetterT>
void    JVisitor_VisitContainerCtx::Value_BLOB
(
    const void*             aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    auto& container = ValGetterT::BLOB(aDataPtr, aProp);
    iJsonArray->Reserve(NumOps::SConvert<rapidjson::SizeType>(container.size()), aCtx.iJsonAllocator);

    for (const GpBytesArray& e: container)
    {
        const std::string s = GpBase64::SEncodeToStr(e, 0);//StrOps::SFromBytes(e);
        rapidjson::Value jv;
        jv.SetString(s.data(), NumOps::SConvert<rapidjson::SizeType>(s.length()), aCtx.iJsonAllocator);

        iJsonArray->PushBack(jv.Move(), aCtx.iJsonAllocator);
    }
}

template<typename ValGetterT>
void    JVisitor_VisitContainerCtx::Value_Object
(
    const void*             /*aDataPtr*/,
    const GpReflectProp&    /*aProp*/,
    JVisitor_VisitCtx&      /*aCtx*/
)
{
    THROW_GP("Object arrays are not supported, use arrays of Object::SP instead"_sv);
}

template<typename ValGetterT>
void    JVisitor_VisitContainerCtx::Value_ObjectSP
(
    const void*             aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    auto& container = ValGetterT::ObjectSP(aDataPtr, aProp);
    iJsonArray->Reserve(NumOps::SConvert<rapidjson::SizeType>(container.size()), aCtx.iJsonAllocator);

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

template<typename ValGetterT>
void    JVisitor_VisitContainerCtx::Value_Enum
(
    const void*             /*aDataPtr*/,
    const GpReflectProp&    /*aProp*/,
    JVisitor_VisitCtx&      /*aCtx*/
)
{
    THROW_GP("Arrays of enums are not supported"_sv);
}

template<typename ValGetterT>
void    JVisitor_VisitContainerCtx::Value_EnumFlags
(
    const void*             /*aDataPtr*/,
    const GpReflectProp&    /*aProp*/,
    JVisitor_VisitCtx&      /*aCtx*/
)
{
    THROW_GP("Arrays of enum flags are not supported"_sv);
}

//------------------------------------- JVisitor_VisitMapCtx ------------------------------------------
class JVisitor_VisitMapCtx
{
public:
                        JVisitor_VisitMapCtx        (void) noexcept {}

    [[nodiscard]] bool  OnVisitBegin                (const void*            aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     JVisitor_VisitCtx&     aCtx);
    void                OnVisitEnd                  (const void*            aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     JVisitor_VisitCtx&     aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void                K_UInt8                     (const void*            aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     JVisitor_VisitCtx&     aCtx);
    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void                K_SInt8                     (const void*            aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     JVisitor_VisitCtx&     aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void                K_UInt16                    (const void*        aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     JVisitor_VisitCtx&     aCtx);
    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void                K_SInt16                    (const void*            aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     JVisitor_VisitCtx&     aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void                K_UInt32                    (const void*        aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     JVisitor_VisitCtx&     aCtx);
    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void                K_SInt32                    (const void*            aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     JVisitor_VisitCtx&     aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void                K_UInt64                    (const void*        aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     JVisitor_VisitCtx&     aCtx);
    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void                K_SInt64                    (const void*            aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     JVisitor_VisitCtx&     aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void                K_Double                    (const void*            aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     JVisitor_VisitCtx&     aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void                K_Float                     (const void*            aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     JVisitor_VisitCtx&     aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void                K_Bool                      (const void*            aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     JVisitor_VisitCtx&     aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void                K_UUID                      (const void*            aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     JVisitor_VisitCtx&     aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void                K_String                    (const void*            aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     JVisitor_VisitCtx&     aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void                K_BLOB                      (const void*            aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     JVisitor_VisitCtx&     aCtx);

    template<typename                       KeyT,
             typename                       ValT,
             template<typename...> class    ValGetterT>
    void                K_ObjectSP                  (const void*            aDataPtr,
                                                     const GpReflectProp&   aProp,
                                                     JVisitor_VisitCtx&     aCtx);

    template<typename CT>
    void                Process                     (const CT&              aContainer,
                                                     const GpReflectProp&   aProp,
                                                     JVisitor_VisitCtx&     aCtx);

    template<typename VT>
    void                ProcessMapKey               (rapidjson::Value&                      aJValOut,
                                                     const VT&                              aValue,
                                                     rapidjson::Document::AllocatorType&    aJsonAllocator);

    template<typename VT>
    void                ProcessMapVal               (rapidjson::Value&                      aJValOut,
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
    const void*             /*aDataPtr*/,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    if (!aCtx.iIsAsTuple)
    {
        if (aProp.FlagTest(GpReflectPropFlag::NAME_OVERRIDE))
        {
            std::string_view name = aProp.FlagArg(GpReflectPropFlag::NAME_OVERRIDE).value();
            iJsonMemberName.SetString(name.data(), NumOps::SConvert<rapidjson::SizeType>(name.length()), aCtx.iJsonAllocator);
        } else
        {
            std::string_view propName = aProp.Name();
            iJsonMemberName.SetString(propName.data(), NumOps::SConvert<rapidjson::SizeType>(propName.length()), aCtx.iJsonAllocator);
        }
    }

    iJsonObject = &(iJsonMemberValue.SetObject());

    return true;
}

void    JVisitor_VisitMapCtx::OnVisitEnd
(
    const void*             /*aDataPtr*/,
    const GpReflectProp&    /*aProp*/,
    JVisitor_VisitCtx&      aCtx
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
void    JVisitor_VisitMapCtx::K_UInt8
(
    const void*             aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    const auto& container = ValGetterT<KeyT>::UInt8(aDataPtr, aProp);
    Process(container, aProp, aCtx);
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    JVisitor_VisitMapCtx::K_SInt8
(
    const void*             aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    const auto& container = ValGetterT<KeyT>::SInt8(aDataPtr, aProp);
    Process(container, aProp, aCtx);
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    JVisitor_VisitMapCtx::K_UInt16
(
    const void*             aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    const auto& container = ValGetterT<KeyT>::UInt16(aDataPtr, aProp);
    Process(container, aProp, aCtx);
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    JVisitor_VisitMapCtx::K_SInt16
(
    const void*             aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    const auto& container = ValGetterT<KeyT>::SInt16(aDataPtr, aProp);
    Process(container, aProp, aCtx);
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    JVisitor_VisitMapCtx::K_UInt32
(
    const void*             aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    const auto& container = ValGetterT<KeyT>::UInt32(aDataPtr, aProp);
    Process(container, aProp, aCtx);
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    JVisitor_VisitMapCtx::K_SInt32
(
    const void*             aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    const auto& container = ValGetterT<KeyT>::SInt32(aDataPtr, aProp);
    Process(container, aProp, aCtx);
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    JVisitor_VisitMapCtx::K_UInt64
(
    const void*             aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    const auto& container = ValGetterT<KeyT>::UInt64(aDataPtr, aProp);
    Process(container, aProp, aCtx);
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    JVisitor_VisitMapCtx::K_SInt64
(
    const void*             aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    const auto& container = ValGetterT<KeyT>::SInt64(aDataPtr, aProp);
    Process(container, aProp, aCtx);
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    JVisitor_VisitMapCtx::K_Double
(
    const void*             aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    const auto& container = ValGetterT<KeyT>::Double(aDataPtr, aProp);
    Process(container, aProp, aCtx);
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    JVisitor_VisitMapCtx::K_Float
(
    const void*             aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    const auto& container = ValGetterT<KeyT>::Float(aDataPtr, aProp);
    Process(container, aProp, aCtx);
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    JVisitor_VisitMapCtx::K_Bool
(
    const void*             aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    const auto& container = ValGetterT<KeyT>::Bool(aDataPtr, aProp);
    Process(container, aProp, aCtx);
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    JVisitor_VisitMapCtx::K_UUID
(
    const void*             aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    const auto& container = ValGetterT<KeyT>::UUID(aDataPtr, aProp);
    Process(container, aProp, aCtx);
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    JVisitor_VisitMapCtx::K_String
(
    const void*             aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    const auto& container = ValGetterT<KeyT>::String(aDataPtr, aProp);
    Process(container, aProp, aCtx);
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    JVisitor_VisitMapCtx::K_BLOB
(
    const void*             aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    const auto& container = ValGetterT<KeyT>::BLOB(aDataPtr, aProp);
    Process(container, aProp, aCtx);
}

template<typename                       KeyT,
         typename                       ValT,
         template<typename...> class    ValGetterT>
void    JVisitor_VisitMapCtx::K_ObjectSP
(
    const void*             aDataPtr,
    const GpReflectProp&    aProp,
    JVisitor_VisitCtx&      aCtx
)
{
    const auto& container = ValGetterT<KeyT>::ObjectSP(aDataPtr, aProp);
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
        const std::string s = StrOps::SToString(aValue);
        aJValOut.SetString(s.data(), NumOps::SConvert<rapidjson::SizeType>(s.length()), aJsonAllocator);
    } else if constexpr (type == GpReflectType::S_INT_8)
    {
        const std::string s = StrOps::SToString(aValue);
        aJValOut.SetString(s.data(), NumOps::SConvert<rapidjson::SizeType>(s.length()), aJsonAllocator);
    } else if constexpr (type == GpReflectType::U_INT_16)
    {
        const std::string s = StrOps::SToString(aValue);
        aJValOut.SetString(s.data(), NumOps::SConvert<rapidjson::SizeType>(s.length()), aJsonAllocator);
    } else if constexpr (type == GpReflectType::S_INT_16)
    {
        const std::string s = StrOps::SToString(aValue);
        aJValOut.SetString(s.data(), NumOps::SConvert<rapidjson::SizeType>(s.length()), aJsonAllocator);
    } else if constexpr (type == GpReflectType::U_INT_32)
    {
        const std::string s = StrOps::SToString(aValue);
        aJValOut.SetString(s.data(), NumOps::SConvert<rapidjson::SizeType>(s.length()), aJsonAllocator);
    } else if constexpr (type == GpReflectType::S_INT_32)
    {
        const std::string s = StrOps::SToString(aValue);
        aJValOut.SetString(s.data(), NumOps::SConvert<rapidjson::SizeType>(s.length()), aJsonAllocator);
    } else if constexpr (type == GpReflectType::U_INT_64)
    {
        const std::string s = StrOps::SToString(aValue);
        aJValOut.SetString(s.data(), NumOps::SConvert<rapidjson::SizeType>(s.length()), aJsonAllocator);
    } else if constexpr (type == GpReflectType::S_INT_64)
    {
        const std::string s = StrOps::SToString(aValue);
        aJValOut.SetString(s.data(), NumOps::SConvert<rapidjson::SizeType>(s.length()), aJsonAllocator);
    } else if constexpr (type == GpReflectType::DOUBLE)
    {
        const std::string s = StrOps::SToString(aValue);
        aJValOut.SetString(s.data(), NumOps::SConvert<rapidjson::SizeType>(s.length()), aJsonAllocator);
    } else if constexpr (type == GpReflectType::FLOAT)
    {
        const std::string s = StrOps::SToString(aValue);
        aJValOut.SetString(s.data(), NumOps::SConvert<rapidjson::SizeType>(s.length()), aJsonAllocator);
    } else if constexpr (type == GpReflectType::BOOLEAN)
    {
        std::string_view s = aValue ? "true"_sv : "false"_sv;
        aJValOut.SetString(s.data(), NumOps::SConvert<rapidjson::SizeType>(s.length()), aJsonAllocator);
    } else if constexpr (type == GpReflectType::UUID)
    {
        const std::string s = aValue.ToString();
        aJValOut.SetString(s.data(), NumOps::SConvert<rapidjson::SizeType>(s.length()), aJsonAllocator);
    } else if constexpr (type == GpReflectType::STRING)
    {
        aJValOut.SetString(aValue.data(), NumOps::SConvert<rapidjson::SizeType>(aValue.length()), aJsonAllocator);
    } else if constexpr (type == GpReflectType::BLOB)
    {
        const std::string s = GpBase64::SEncodeToStr(aValue, 0);/*StrOps::SFromBytes(aValue);*/
        aJValOut.SetString(s.data(), NumOps::SConvert<rapidjson::SizeType>(s.length()), aJsonAllocator);
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
        GpThrowCe<std::out_of_range>("Unknown type '"_sv + GpReflectUtils::SModelName<VT>() + "'"_sv);
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
        aJValOut.SetString(s.data(), NumOps::SConvert<rapidjson::SizeType>(s.length()), aJsonAllocator);
    } else if constexpr (type == GpReflectType::STRING)
    {
        aJValOut.SetString(aValue.data(), NumOps::SConvert<rapidjson::SizeType>(aValue.length()), aJsonAllocator);
    } else if constexpr (type == GpReflectType::BLOB)
    {
        const std::string s = GpBase64::SEncodeToStr(aValue, 0);/*StrOps::SFromBytes(aValue);*/
        aJValOut.SetString(s.data(), NumOps::SConvert<rapidjson::SizeType>(s.length()), aJsonAllocator);
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
        GpThrowCe<std::out_of_range>("Unknown type '"_sv + GpReflectUtils::SModelName<VT>() + "'"_sv);
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

}//namespace ObjectVisitor_JsonFromObject

//-------------------------------------------------------------------------------

std::string GpJsonFromObject::SToString (const rapidjson::Document& aJsonDOM)
{
    rapidjson::StringBuffer                     buffer;
    rapidjson::Writer<rapidjson::StringBuffer>  writer(buffer);
    aJsonDOM.Accept(writer);

    return std::string(buffer.GetString(), buffer.GetSize());
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
    GpReflectVisitor<const GpReflectObject, ObjectVisitor_JsonFromObject::JVisitor> visitor;
    ObjectVisitor_JsonFromObject::JVisitor_VisitCtx ctx(aJsonObject, aJsonAllocator, aJsonSerializerFlags, aIsAsTuple);

    const GpReflectModel& model = aObject.ReflectModel();
    visitor.Visit(model, aObject.ReflectDataPtr(), ctx);
}

}//namespace GPlatform
