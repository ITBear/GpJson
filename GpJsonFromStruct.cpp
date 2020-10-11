#include "GpJsonFromStruct.hpp"

#define RAPIDJSON_ASSERT(X) THROW_GPE_COND_CHECK_M(X, "Json processing error");

#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

namespace GPlatform {

std::string GpJsonFromStruct::SToString (const rapidjson::Document& aJsonDOM)
{
    rapidjson::StringBuffer                     buffer;
    rapidjson::Writer<rapidjson::StringBuffer>  writer(buffer);
    aJsonDOM.Accept(writer);

    return std::string(buffer.GetString(), buffer.GetSize());
}

void    GpJsonFromStruct::SToString (const rapidjson::Document& aJsonDOM,
                                     GpByteWriter&              aWriter)
{
    aWriter.Bytes(SToString(aJsonDOM));
}

void    GpJsonFromStruct::SWrite (const GpTypeStructBase&               aStruct,
                                  rapidjson::Document::GenericValue&    aJsonObject,
                                  rapidjson::Document::AllocatorType&   aJsonAllocator,
                                  const size_t                          aFlags)
{
    const GpTypeStructInfo& structInfo  = aStruct.TypeStructInfo();
    const auto&             props       = structInfo.Props();

    if (aFlags & size_t(Flags::WRITE_STRUCT_UID))
    {
        const std::string structUidStr = structInfo.UID().ToString();
        rapidjson::Value n, v;

        n.SetString("@", aJsonAllocator);
        v.SetString(structUidStr.data(), NumOps::SConvert<rapidjson::SizeType>(structUidStr.length()), aJsonAllocator);
        aJsonObject.AddMember(n.Move(), v.Move(), aJsonAllocator);
    }

    for (const GpTypePropInfo& propInfo: props)
    {
        const GpTypeContainer::EnumT containerType = propInfo.Container();
        try
        {
            if (containerType == GpTypeContainer::NO)
            {
                SWriteValue(aStruct, propInfo, aJsonObject, aJsonAllocator, aFlags);
            } else if (containerType == GpTypeContainer::VECTOR)
            {
                SWriteValueVec(aStruct, propInfo, aJsonObject, aJsonAllocator, aFlags);
            } else if (containerType == GpTypeContainer::LIST)
            {
                THROW_NOT_IMPLEMENTED();
            } else if (containerType == GpTypeContainer::SET)
            {
                THROW_NOT_IMPLEMENTED();
            } else if (containerType == GpTypeContainer::MAP)
            {
                THROW_NOT_IMPLEMENTED();
            }
        } catch (const std::exception& e)
        {
            THROW_GPE("Failed to write value to json. Struct "_sv + structInfo.Name() + "."_sv + propInfo.Name() + "\nReason:\n"_sv + e.what());
        } catch (...)
        {
            THROW_GPE("Failed to write value to json. Struct "_sv + structInfo.Name() + "."_sv + propInfo.Name() + "\nReason:\nUnknown exception");
        }
    }

}

void    GpJsonFromStruct::SWriteValue (const GpTypeStructBase&              aStruct,
                                       const GpTypePropInfo&                aPropInfo,
                                       rapidjson::Document::GenericValue&   aJsonObject,
                                       rapidjson::Document::AllocatorType&  aJsonAllocator,
                                       const size_t                         aFlags)
{
    const GpType::EnumT propType = aPropInfo.Type();
    std::string_view    propName = aPropInfo.Name();

    rapidjson::Value jsonMemberName;
    rapidjson::Value jsonMemberValue;

    jsonMemberName.SetString(propName.data(), NumOps::SConvert<rapidjson::SizeType>(propName.length()), aJsonAllocator);

    switch (propType)
    {
        case GpType::U_INT_8:
        {
            jsonMemberValue.SetUint64(aPropInfo.Value_UInt8(aStruct));
        } break;
        case GpType::S_INT_8:
        {
            jsonMemberValue.SetInt64(aPropInfo.Value_SInt8(aStruct));
        } break;
        case GpType::U_INT_16:
        {
            jsonMemberValue.SetUint64(aPropInfo.Value_UInt16(aStruct));
        } break;
        case GpType::S_INT_16:
        {
            jsonMemberValue.SetInt64(aPropInfo.Value_SInt16(aStruct));
        } break;
        case GpType::U_INT_32:
        {
            jsonMemberValue.SetUint64(aPropInfo.Value_UInt32(aStruct));
        } break;
        case GpType::S_INT_32:
        {
            jsonMemberValue.SetInt64(aPropInfo.Value_SInt32(aStruct));
        } break;
        case GpType::U_INT_64:
        {
            jsonMemberValue.SetUint64(aPropInfo.Value_UInt64(aStruct));
        } break;
        case GpType::S_INT_64:
        {
            jsonMemberValue.SetInt64(aPropInfo.Value_SInt64(aStruct));
        } break;
        case GpType::DOUBLE:
        {
            jsonMemberValue.SetDouble(aPropInfo.Value_Double(aStruct));
        } break;
        case GpType::FLOAT:
        {
            jsonMemberValue.SetDouble(aPropInfo.Value_Float(aStruct));
        } break;
        case GpType::BOOLEAN:
        {
            jsonMemberValue.SetBool(aPropInfo.Value_Bool(aStruct));
        } break;
        case GpType::UUID:
        {
            const std::string propVal = aPropInfo.Value_UUID(aStruct).ToString();
            jsonMemberValue.SetString(propVal.data(), NumOps::SConvert<rapidjson::SizeType>(propVal.length()), aJsonAllocator);
        } break;
        case GpType::STRING:
        {
            const std::string& propVal = aPropInfo.Value_String(aStruct);
            jsonMemberValue.SetString(propVal.data(), NumOps::SConvert<rapidjson::SizeType>(propVal.length()), aJsonAllocator);
        } break;
        case GpType::BLOB:
        {
            const std::string propVal = GpStringOps::SFromBytes(aPropInfo.Value_BLOB(aStruct));
            jsonMemberValue.SetString(propVal.data(), NumOps::SConvert<rapidjson::SizeType>(propVal.length()), aJsonAllocator);
        } break;
        case GpType::STRUCT:
        {
            const GpTypeStructBase&             structBase  = aPropInfo.Value_Struct(aStruct);
            rapidjson::Document::GenericValue&  obj         = jsonMemberValue.SetObject();
            SWrite(structBase, obj, aJsonAllocator, aFlags);
        } break;
        case GpType::STRUCT_SP:
        {
            const GpTypeStructBase::SP& structBaseSP = aPropInfo.Value_StructSP(aStruct);

            if (structBaseSP.IsNULL())
            {
                jsonMemberValue.SetNull();
            } else
            {
                const GpTypeStructBase&             structBase  = structBaseSP.VCn();
                rapidjson::Document::GenericValue&  obj         = jsonMemberValue.SetObject();
                SWrite(structBase, obj, aJsonAllocator, aFlags);
            }
        } break;
        case GpType::ENUM:
        {
            std::string_view propVal = aPropInfo.Value_Enum(aStruct).ToString();
            jsonMemberValue.SetString(propVal.data(), NumOps::SConvert<rapidjson::SizeType>(propVal.length()), aJsonAllocator);
        } break;
        case GpType::NOT_SET:
        {
            THROW_GPE("Type "_sv + GpType::SToString(aPropInfo.Type()));
        } break;
        default:
        {
            THROW_GPE("Unsupported type "_sv + GpType::SToString(aPropInfo.Type()));
        }
    }

    aJsonObject.AddMember(jsonMemberName.Move(), jsonMemberValue.Move(), aJsonAllocator);
}

void    GpJsonFromStruct::SWriteValueVec (const GpTypeStructBase&               aStruct,
                                          const GpTypePropInfo&                 aPropInfo,
                                          rapidjson::Document::GenericValue&    aJsonObject,
                                          rapidjson::Document::AllocatorType&   aJsonAllocator,
                                          const size_t                          aFlags)
{
    const GpType::EnumT propType = aPropInfo.Type();
    std::string_view    propName = aPropInfo.Name();

    rapidjson::Value jsonMemberName;
    rapidjson::Value jsonMemberValue;

    jsonMemberName.SetString(propName.data(), NumOps::SConvert<rapidjson::SizeType>(propName.length()), aJsonAllocator);
    rapidjson::Document::GenericValue& jsonArray = jsonMemberValue.SetArray();



    switch (propType)
    {
        case GpType::U_INT_8:
        {
            const auto& vec = aPropInfo.Value_Vec_UInt8(aStruct);
            jsonArray.Reserve(NumOps::SConvert<rapidjson::SizeType>(vec.size()), aJsonAllocator);

            for (const u_int_8 e: vec)
            {
                jsonArray.PushBack(rapidjson::Value().SetUint64(e), aJsonAllocator);
            }
        } break;
        case GpType::S_INT_8:
        {
            const auto& vec = aPropInfo.Value_Vec_SInt8(aStruct);
            jsonArray.Reserve(NumOps::SConvert<rapidjson::SizeType>(vec.size()), aJsonAllocator);

            for (const s_int_8 e: vec)
            {
                jsonArray.PushBack(rapidjson::Value().SetInt64(e), aJsonAllocator);
            }
        } break;
        case GpType::U_INT_16:
        {
            const auto& vec = aPropInfo.Value_Vec_UInt16(aStruct);
            jsonArray.Reserve(NumOps::SConvert<rapidjson::SizeType>(vec.size()), aJsonAllocator);

            for (const u_int_16 e: vec)
            {
                jsonArray.PushBack(rapidjson::Value().SetUint64(e), aJsonAllocator);
            }
        } break;
        case GpType::S_INT_16:
        {
            const auto& vec = aPropInfo.Value_Vec_SInt16(aStruct);
            jsonArray.Reserve(NumOps::SConvert<rapidjson::SizeType>(vec.size()), aJsonAllocator);

            for (const s_int_16 e: vec)
            {
                jsonArray.PushBack(rapidjson::Value().SetInt64(e), aJsonAllocator);
            }
        } break;
        case GpType::U_INT_32:
        {
            const auto& vec = aPropInfo.Value_Vec_UInt32(aStruct);
            jsonArray.Reserve(NumOps::SConvert<rapidjson::SizeType>(vec.size()), aJsonAllocator);

            for (const u_int_32 e: vec)
            {
                jsonArray.PushBack(rapidjson::Value().SetUint64(e), aJsonAllocator);
            }
        } break;
        case GpType::S_INT_32:
        {
            const auto& vec = aPropInfo.Value_Vec_SInt32(aStruct);
            jsonArray.Reserve(NumOps::SConvert<rapidjson::SizeType>(vec.size()), aJsonAllocator);

            for (const s_int_32 e: vec)
            {
                jsonArray.PushBack(rapidjson::Value().SetInt64(e), aJsonAllocator);
            }
        } break;
        case GpType::U_INT_64:
        {
            const auto& vec = aPropInfo.Value_Vec_UInt64(aStruct);
            jsonArray.Reserve(NumOps::SConvert<rapidjson::SizeType>(vec.size()), aJsonAllocator);

            for (const u_int_64 e: vec)
            {
                jsonArray.PushBack(rapidjson::Value().SetUint64(e), aJsonAllocator);
            }
        } break;
        case GpType::S_INT_64:
        {
            const auto& vec = aPropInfo.Value_Vec_SInt64(aStruct);
            jsonArray.Reserve(NumOps::SConvert<rapidjson::SizeType>(vec.size()), aJsonAllocator);

            for (const s_int_64 e: vec)
            {
                jsonArray.PushBack(rapidjson::Value().SetInt64(e), aJsonAllocator);
            }
        } break;
        case GpType::DOUBLE:
        {
            const auto& vec = aPropInfo.Value_Vec_Double(aStruct);
            jsonArray.Reserve(NumOps::SConvert<rapidjson::SizeType>(vec.size()), aJsonAllocator);

            for (const double e: vec)
            {
                jsonArray.PushBack(rapidjson::Value().SetDouble(e), aJsonAllocator);
            }
        } break;
        case GpType::FLOAT:
        {
            const auto& vec = aPropInfo.Value_Vec_Float(aStruct);
            jsonArray.Reserve(NumOps::SConvert<rapidjson::SizeType>(vec.size()), aJsonAllocator);

            for (const float e: vec)
            {
                jsonArray.PushBack(rapidjson::Value().SetDouble(e), aJsonAllocator);
            }
        } break;
        case GpType::BOOLEAN:
        {
            const auto& vec = aPropInfo.Value_Vec_Bool(aStruct);
            jsonArray.Reserve(NumOps::SConvert<rapidjson::SizeType>(vec.size()), aJsonAllocator);

            for (const bool e: vec)
            {
                jsonArray.PushBack(rapidjson::Value().SetBool(e), aJsonAllocator);
            }
        } break;
        case GpType::UUID:
        {
            const auto& vec = aPropInfo.Value_Vec_UUID(aStruct);
            jsonArray.Reserve(NumOps::SConvert<rapidjson::SizeType>(vec.size()), aJsonAllocator);

            for (const GpUUID& e: vec)
            {
                const std::string s = e.ToString();
                rapidjson::Value jv;
                jv.SetString(s.data(), NumOps::SConvert<rapidjson::SizeType>(s.length()), aJsonAllocator);

                jsonArray.PushBack(jv.Move(), aJsonAllocator);
            }
        } break;
        case GpType::STRING:
        {
            const auto& vec = aPropInfo.Value_Vec_String(aStruct);
            jsonArray.Reserve(NumOps::SConvert<rapidjson::SizeType>(vec.size()), aJsonAllocator);

            for (const std::string& e: vec)
            {
                rapidjson::Value jv;
                jv.SetString(e.data(), NumOps::SConvert<rapidjson::SizeType>(e.length()), aJsonAllocator);

                jsonArray.PushBack(jv.Move(), aJsonAllocator);
            }
        } break;
        case GpType::BLOB:
        {
            const auto& vec = aPropInfo.Value_Vec_BLOB(aStruct);
            jsonArray.Reserve(NumOps::SConvert<rapidjson::SizeType>(vec.size()), aJsonAllocator);

            for (const GpBytesArray& e: vec)
            {
                const std::string s = GpStringOps::SFromBytes(e);
                rapidjson::Value jv;
                jv.SetString(s.data(), NumOps::SConvert<rapidjson::SizeType>(s.length()), aJsonAllocator);

                jsonArray.PushBack(jv.Move(), aJsonAllocator);
            }
        } break;
        case GpType::STRUCT:
        {
            THROW_GPE("Arrays of struct are not supported, use arrays of struct::SP instead"_sv);
        } break;
        case GpType::STRUCT_SP:
        {
            const auto& vec = aPropInfo.Value_Vec_StructSP(aStruct);
            jsonArray.Reserve(NumOps::SConvert<rapidjson::SizeType>(vec.size()), aJsonAllocator);

            for (const GpTypeStructBase::SP& e: vec)
            {
                rapidjson::Value jv;

                if (e.IsNULL())
                {
                    jv.SetNull();
                } else
                {
                    const GpTypeStructBase&             structBase  = e.VCn();
                    rapidjson::Document::GenericValue&  obj         = jv.SetObject();
                    SWrite(structBase, obj, aJsonAllocator, aFlags);
                }

                jsonArray.PushBack(jv.Move(), aJsonAllocator);
            }
        } break;
        case GpType::ENUM:
        {
            THROW_GPE("Arrays of enums are not supported"_sv);
        } break;
        case GpType::NOT_SET:
        {
            THROW_GPE("Type "_sv + GpType::SToString(aPropInfo.Type()));
        } break;
        default:
        {
            THROW_GPE("Unsupported type "_sv + GpType::SToString(aPropInfo.Type()));
        }
    }

    aJsonObject.AddMember(jsonMemberName.Move(), jsonMemberValue.Move(), aJsonAllocator);
}


}//namespace GPlatform
