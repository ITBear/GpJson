#include "GpJsonFromStruct.hpp"

#define RAPIDJSON_ASSERT(X) THROW_GPE_COND_CHECK_M(X, "Json processing error");

#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

namespace GPlatform {

template<typename T>
void    _ProcessVal (rapidjson::Value&                      aJValOut,
                     const T&                               aValue,
                     rapidjson::Document::AllocatorType&    aJsonAllocator,
                     const GpJsonMapperFlags                aFlags)
{
    constexpr const GpType::EnumT type = GpTypeUtils::SDetectType<T>();

    if constexpr (type == GpType::U_INT_8)
    {
        aJValOut.SetUint64(aValue);
    } else if constexpr (type == GpType::S_INT_8)
    {
        aJValOut.SetInt64(aValue);
    } else if constexpr (type == GpType::U_INT_16)
    {
        aJValOut.SetUint64(aValue);
    } else if constexpr (type == GpType::S_INT_16)
    {
        aJValOut.SetInt64(aValue);
    } else if constexpr (type == GpType::U_INT_32)
    {
        aJValOut.SetUint64(aValue);
    } else if constexpr (type == GpType::S_INT_32)
    {
        aJValOut.SetInt64(aValue);
    } else if constexpr (type == GpType::U_INT_64)
    {
        aJValOut.SetUint64(aValue);
    } else if constexpr (type == GpType::S_INT_64)
    {
        aJValOut.SetInt64(aValue);
    } else if constexpr (type == GpType::DOUBLE)
    {
        aJValOut.SetDouble(aValue);
    } else if constexpr (type == GpType::FLOAT)
    {
        aJValOut.SetDouble(double(aValue));
    } else if constexpr (type == GpType::BOOLEAN)
    {
        aJValOut.SetBool(aValue);
    } else if constexpr (type == GpType::UUID)
    {
        const std::string s = aValue.ToString();
        aJValOut.SetString(s.data(), NumOps::SConvert<rapidjson::SizeType>(s.length()), aJsonAllocator);
    } else if constexpr (type == GpType::STRING)
    {
        aJValOut.SetString(aValue.data(), NumOps::SConvert<rapidjson::SizeType>(aValue.length()), aJsonAllocator);
    } else if constexpr (type == GpType::BLOB)
    {
        const std::string s = GpStringOps::SFromBytes(aValue);
        aJValOut.SetString(s.data(), NumOps::SConvert<rapidjson::SizeType>(s.length()), aJsonAllocator);
    } else if constexpr (type == GpType::STRUCT)
    {
        THROW_GPE("Struct are not supported, use arrays of struct::SP instead"_sv);
    } else if constexpr (type == GpType::STRUCT_SP)
    {
        if (aValue.IsNULL())
        {
            aJValOut.SetNull();
        } else
        {
            const GpTypeStructBase&             structBase  = aValue.VCn();
            rapidjson::Document::GenericValue&  obj         = aJValOut.SetObject();
            GpJsonFromStruct::SWrite(structBase, obj, aJsonAllocator, aFlags);
        }
    } else if constexpr (type == GpType::ENUM)
    {
        THROW_GPE("Enums are not supported"_sv);
    } else
    {
        GpThrowCe<std::out_of_range>("Unknown type '"_sv + GpTypeUtils::STypeName<T>() + "'");
    }
}

template<typename ValGetter>
void    _ProcessContainer (const GpTypeStructBase&              aStruct,
                           const GpTypePropInfo&                aPropInfo,
                           rapidjson::Document::GenericValue&   aJsonObject,
                           rapidjson::Document::AllocatorType&  aJsonAllocator,
                           const GpJsonMapperFlags              aFlags)
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
            auto& container = ValGetter::UInt8(aStruct, aPropInfo);
            jsonArray.Reserve(NumOps::SConvert<rapidjson::SizeType>(container.size()), aJsonAllocator);

            for (const u_int_8 e: container)
            {
                jsonArray.PushBack(rapidjson::Value().SetUint64(e), aJsonAllocator);
            }
        } break;
        case GpType::S_INT_8:
        {
            auto& container = ValGetter::SInt8(aStruct, aPropInfo);
            jsonArray.Reserve(NumOps::SConvert<rapidjson::SizeType>(container.size()), aJsonAllocator);

            for (const s_int_8 e: container)
            {
                jsonArray.PushBack(rapidjson::Value().SetInt64(e), aJsonAllocator);
            }
        } break;
        case GpType::U_INT_16:
        {
            auto& container = ValGetter::UInt16(aStruct, aPropInfo);
            jsonArray.Reserve(NumOps::SConvert<rapidjson::SizeType>(container.size()), aJsonAllocator);

            for (const u_int_16 e: container)
            {
                jsonArray.PushBack(rapidjson::Value().SetUint64(e), aJsonAllocator);
            }
        } break;
        case GpType::S_INT_16:
        {
            auto& container = ValGetter::SInt16(aStruct, aPropInfo);
            jsonArray.Reserve(NumOps::SConvert<rapidjson::SizeType>(container.size()), aJsonAllocator);

            for (const s_int_16 e: container)
            {
                jsonArray.PushBack(rapidjson::Value().SetInt64(e), aJsonAllocator);
            }
        } break;
        case GpType::U_INT_32:
        {
            auto& container = ValGetter::UInt32(aStruct, aPropInfo);
            jsonArray.Reserve(NumOps::SConvert<rapidjson::SizeType>(container.size()), aJsonAllocator);

            for (const u_int_32 e: container)
            {
                jsonArray.PushBack(rapidjson::Value().SetUint64(e), aJsonAllocator);
            }
        } break;
        case GpType::S_INT_32:
        {
            auto& container = ValGetter::SInt32(aStruct, aPropInfo);
            jsonArray.Reserve(NumOps::SConvert<rapidjson::SizeType>(container.size()), aJsonAllocator);

            for (const s_int_32 e: container)
            {
                jsonArray.PushBack(rapidjson::Value().SetInt64(e), aJsonAllocator);
            }
        } break;
        case GpType::U_INT_64:
        {
            auto& container = ValGetter::UInt64(aStruct, aPropInfo);
            jsonArray.Reserve(NumOps::SConvert<rapidjson::SizeType>(container.size()), aJsonAllocator);

            for (const u_int_64 e: container)
            {
                jsonArray.PushBack(rapidjson::Value().SetUint64(e), aJsonAllocator);
            }
        } break;
        case GpType::S_INT_64:
        {
            auto& container = ValGetter::SInt64(aStruct, aPropInfo);
            jsonArray.Reserve(NumOps::SConvert<rapidjson::SizeType>(container.size()), aJsonAllocator);

            for (const s_int_64 e: container)
            {
                jsonArray.PushBack(rapidjson::Value().SetInt64(e), aJsonAllocator);
            }
        } break;
        case GpType::DOUBLE:
        {
            auto& container = ValGetter::Double(aStruct, aPropInfo);
            jsonArray.Reserve(NumOps::SConvert<rapidjson::SizeType>(container.size()), aJsonAllocator);

            for (const double e: container)
            {
                jsonArray.PushBack(rapidjson::Value().SetDouble(e), aJsonAllocator);
            }
        } break;
        case GpType::FLOAT:
        {
            auto& container = ValGetter::Float(aStruct, aPropInfo);
            jsonArray.Reserve(NumOps::SConvert<rapidjson::SizeType>(container.size()), aJsonAllocator);

            for (const float e: container)
            {
                jsonArray.PushBack(rapidjson::Value().SetDouble(e), aJsonAllocator);
            }
        } break;
        case GpType::BOOLEAN:
        {
            auto& container = ValGetter::Bool(aStruct, aPropInfo);
            jsonArray.Reserve(NumOps::SConvert<rapidjson::SizeType>(container.size()), aJsonAllocator);

            for (const bool e: container)
            {
                jsonArray.PushBack(rapidjson::Value().SetBool(e), aJsonAllocator);
            }
        } break;
        case GpType::UUID:
        {
            auto& container = ValGetter::UUID(aStruct, aPropInfo);
            jsonArray.Reserve(NumOps::SConvert<rapidjson::SizeType>(container.size()), aJsonAllocator);

            for (const GpUUID& e: container)
            {
                const std::string s = e.ToString();
                rapidjson::Value jv;
                jv.SetString(s.data(), NumOps::SConvert<rapidjson::SizeType>(s.length()), aJsonAllocator);

                jsonArray.PushBack(jv.Move(), aJsonAllocator);
            }
        } break;
        case GpType::STRING:
        {
            auto& container = ValGetter::String(aStruct, aPropInfo);
            jsonArray.Reserve(NumOps::SConvert<rapidjson::SizeType>(container.size()), aJsonAllocator);

            for (const std::string& e: container)
            {
                rapidjson::Value jv;
                jv.SetString(e.data(), NumOps::SConvert<rapidjson::SizeType>(e.length()), aJsonAllocator);

                jsonArray.PushBack(jv.Move(), aJsonAllocator);
            }
        } break;
        case GpType::BLOB:
        {
            auto& container = ValGetter::BLOB(aStruct, aPropInfo);
            jsonArray.Reserve(NumOps::SConvert<rapidjson::SizeType>(container.size()), aJsonAllocator);

            for (const GpBytesArray& e: container)
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
            auto& container = ValGetter::StructSP(aStruct, aPropInfo);
            jsonArray.Reserve(NumOps::SConvert<rapidjson::SizeType>(container.size()), aJsonAllocator);

            for (const GpTypeStructBase::SP& e: container)
            {
                rapidjson::Value jv;

                if (e.IsNULL())
                {
                    jv.SetNull();
                } else
                {
                    const GpTypeStructBase&             structBase  = e.VCn();
                    rapidjson::Document::GenericValue&  obj         = jv.SetObject();
                    GpJsonFromStruct::SWrite(structBase, obj, aJsonAllocator, aFlags);
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

template<typename KeyT,
         typename CT>
void    _ProcessMapContainer (const CT&                             aContainer,
                              rapidjson::Document::GenericValue&    aJsonMap,
                              rapidjson::Document::AllocatorType&   aJsonAllocator,
                              const GpJsonMapperFlags               aFlags)
{
    //aJsonMap.Reserve(NumOps::SConvert<rapidjson::SizeType>(aContainer.size()), aJsonAllocator);

    for (const auto&[k, v]: aContainer)
    {
        //Key
        rapidjson::Value jMapElementKey;
        _ProcessVal(jMapElementKey, k, aJsonAllocator, aFlags);

        //Value
        rapidjson::Value jMapElementVal;
        _ProcessVal(jMapElementVal, v, aJsonAllocator, aFlags);

        rapidjson::Value                    jMapElement;
        //rapidjson::Document::GenericValue&    jMapElementObj = jMapElement.SetObject();
        //jMapElementObj.AddMember(jMapElementKey.Move(), jMapElementVal.Move(), aJsonAllocator);
        aJsonMap.AddMember(jMapElementKey.Move(), jMapElementVal.Move(), aJsonAllocator);
    }
}

template<typename                       Key,
         template<typename...> class    ValGetter>
void    _ProcessMap (const GpTypeStructBase&                aStruct,
                     const GpTypePropInfo&                  aPropInfo,
                     rapidjson::Document::GenericValue&     aJsonObject,
                     rapidjson::Document::AllocatorType&    aJsonAllocator,
                     const GpJsonMapperFlags                aFlags)
{
    const GpType::EnumT                 propType    = aPropInfo.Type();
    rapidjson::Document::GenericValue&  jsonMap     = aJsonObject;

    switch (propType)
    {
        case GpType::U_INT_8:
        {
            auto& container = ValGetter<Key>::UInt8(aStruct, aPropInfo);
            _ProcessMapContainer<Key>(container, jsonMap, aJsonAllocator, aFlags);
        } break;
        case GpType::S_INT_8:
        {
            auto& container = ValGetter<Key>::SInt8(aStruct, aPropInfo);
            _ProcessMapContainer<Key>(container, jsonMap, aJsonAllocator, aFlags);
        } break;
        case GpType::U_INT_16:
        {
            auto& container = ValGetter<Key>::UInt16(aStruct, aPropInfo);
            _ProcessMapContainer<Key>(container, jsonMap, aJsonAllocator, aFlags);
        } break;
        case GpType::S_INT_16:
        {
            auto& container = ValGetter<Key>::SInt16(aStruct, aPropInfo);
            _ProcessMapContainer<Key>(container, jsonMap, aJsonAllocator, aFlags);
        } break;
        case GpType::U_INT_32:
        {
            auto& container = ValGetter<Key>::UInt32(aStruct, aPropInfo);
            _ProcessMapContainer<Key>(container, jsonMap, aJsonAllocator, aFlags);
        } break;
        case GpType::S_INT_32:
        {
            auto& container = ValGetter<Key>::SInt32(aStruct, aPropInfo);
            _ProcessMapContainer<Key>(container, jsonMap, aJsonAllocator, aFlags);
        } break;
        case GpType::U_INT_64:
        {
            auto& container = ValGetter<Key>::UInt64(aStruct, aPropInfo);
            _ProcessMapContainer<Key>(container, jsonMap, aJsonAllocator, aFlags);
        } break;
        case GpType::S_INT_64:
        {
            auto& container = ValGetter<Key>::SInt64(aStruct, aPropInfo);
            _ProcessMapContainer<Key>(container, jsonMap, aJsonAllocator, aFlags);
        } break;
        case GpType::DOUBLE:
        {
            auto& container = ValGetter<Key>::Double(aStruct, aPropInfo);
            _ProcessMapContainer<Key>(container, jsonMap, aJsonAllocator, aFlags);
        } break;
        case GpType::FLOAT:
        {
            auto& container = ValGetter<Key>::Float(aStruct, aPropInfo);
            _ProcessMapContainer<Key>(container, jsonMap, aJsonAllocator, aFlags);
        } break;
        case GpType::BOOLEAN:
        {
            auto& container = ValGetter<Key>::Bool(aStruct, aPropInfo);
            _ProcessMapContainer<Key>(container, jsonMap, aJsonAllocator, aFlags);
        } break;
        case GpType::UUID:
        {
            auto& container = ValGetter<Key>::UUID(aStruct, aPropInfo);
            _ProcessMapContainer<Key>(container, jsonMap, aJsonAllocator, aFlags);
        } break;
        case GpType::STRING:
        {
            auto& container = ValGetter<Key>::String(aStruct, aPropInfo);
            _ProcessMapContainer<Key>(container, jsonMap, aJsonAllocator, aFlags);
        } break;
        case GpType::BLOB:
        {
            auto& container = ValGetter<Key>::BLOB(aStruct, aPropInfo);
            _ProcessMapContainer<Key>(container, jsonMap, aJsonAllocator, aFlags);
        } break;
        case GpType::STRUCT:
        {
            THROW_GPE("struct as map value are not supported, use struct::SP instead"_sv);
        } break;
        case GpType::STRUCT_SP:
        {
            auto& container = ValGetter<Key>::StructSP(aStruct, aPropInfo);
            _ProcessMapContainer<Key>(container, jsonMap, aJsonAllocator, aFlags);
        } break;
        case GpType::ENUM:
        {
            THROW_GPE("enums are not supported"_sv);
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
}


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
                                  const GpJsonMapperFlags               aFlags)
{
    const GpTypeStructInfo& typeInfo    = aStruct.TypeInfo();
    const auto&             props       = typeInfo.Props();

    if (aFlags.Test(GpJsonMapperFlag::WRITE_STRUCT_UID))
    {
        const std::string structUidStr = typeInfo.UID().ToString();
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
                SWriteValueList(aStruct, propInfo, aJsonObject, aJsonAllocator, aFlags);
            } else if (containerType == GpTypeContainer::SET)
            {
                SWriteValueSet(aStruct, propInfo, aJsonObject, aJsonAllocator, aFlags);
            } else if (containerType == GpTypeContainer::MAP)
            {
                SWriteValueMap(aStruct, propInfo, aJsonObject, aJsonAllocator, aFlags);
            }
        } catch (const std::exception& e)
        {
            THROW_GPE("Failed to write value to json. Struct "_sv + typeInfo.Name() + "."_sv + propInfo.Name() + "\nReason:\n"_sv + e.what());
        } catch (...)
        {
            THROW_GPE("Failed to write value to json. Struct "_sv + typeInfo.Name() + "."_sv + propInfo.Name() + "\nReason:\nUnknown exception");
        }
    }

}

void    GpJsonFromStruct::SWriteValue (const GpTypeStructBase&              aStruct,
                                       const GpTypePropInfo&                aPropInfo,
                                       rapidjson::Document::GenericValue&   aJsonObject,
                                       rapidjson::Document::AllocatorType&  aJsonAllocator,
                                       const GpJsonMapperFlags              aFlags)
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
                                          const GpJsonMapperFlags               aFlags)
{
    _ProcessContainer<GpTypePropInfoGetter_Vec>(aStruct, aPropInfo, aJsonObject, aJsonAllocator, aFlags);
}

void    GpJsonFromStruct::SWriteValueList (const GpTypeStructBase&              aStruct,
                                           const GpTypePropInfo&                aPropInfo,
                                           rapidjson::Document::GenericValue&   aJsonObject,
                                           rapidjson::Document::AllocatorType&  aJsonAllocator,
                                           const GpJsonMapperFlags              aFlags)
{
    _ProcessContainer<GpTypePropInfoGetter_List>(aStruct, aPropInfo, aJsonObject, aJsonAllocator, aFlags);
}

void    GpJsonFromStruct::SWriteValueSet (const GpTypeStructBase&               aStruct,
                                          const GpTypePropInfo&                 aPropInfo,
                                          rapidjson::Document::GenericValue&    aJsonObject,
                                          rapidjson::Document::AllocatorType&   aJsonAllocator,
                                          const GpJsonMapperFlags               aFlags)
{
    _ProcessContainer<GpTypePropInfoGetter_Set>(aStruct, aPropInfo, aJsonObject, aJsonAllocator, aFlags);
}

void    GpJsonFromStruct::SWriteValueMap (const GpTypeStructBase&               aStruct,
                                          const GpTypePropInfo&                 aPropInfo,
                                          rapidjson::Document::GenericValue&    aJsonObject,
                                          rapidjson::Document::AllocatorType&   aJsonAllocator,
                                          const GpJsonMapperFlags               aFlags)
{
    const GpType::EnumT keyType     = aPropInfo.ContainerKeyType();
    std::string_view    propName    = aPropInfo.Name();

    rapidjson::Value jsonMemberName;
    rapidjson::Value jsonMemberValue;

    jsonMemberName.SetString(propName.data(), NumOps::SConvert<rapidjson::SizeType>(propName.length()), aJsonAllocator);
    rapidjson::Document::GenericValue& jsonObject = jsonMemberValue.SetObject();

    switch (keyType)
    {
        case GpType::U_INT_8:
        {
            _ProcessMap<u_int_8, GpTypePropInfoGetter_Map>(aStruct, aPropInfo, jsonObject, aJsonAllocator, aFlags);
        } break;
        case GpType::S_INT_8:
        {
            _ProcessMap<s_int_8, GpTypePropInfoGetter_Map>(aStruct, aPropInfo, jsonObject, aJsonAllocator, aFlags);
        } break;
        case GpType::U_INT_16:
        {
            _ProcessMap<u_int_16, GpTypePropInfoGetter_Map>(aStruct, aPropInfo, jsonObject, aJsonAllocator, aFlags);
        } break;
        case GpType::S_INT_16:
        {
            _ProcessMap<s_int_16, GpTypePropInfoGetter_Map>(aStruct, aPropInfo, jsonObject, aJsonAllocator, aFlags);
        } break;
        case GpType::U_INT_32:
        {
            _ProcessMap<u_int_32, GpTypePropInfoGetter_Map>(aStruct, aPropInfo, jsonObject, aJsonAllocator, aFlags);
        } break;
        case GpType::S_INT_32:
        {
            _ProcessMap<s_int_32, GpTypePropInfoGetter_Map>(aStruct, aPropInfo, jsonObject, aJsonAllocator, aFlags);
        } break;
        case GpType::U_INT_64:
        {
            _ProcessMap<u_int_64, GpTypePropInfoGetter_Map>(aStruct, aPropInfo, jsonObject, aJsonAllocator, aFlags);
        } break;
        case GpType::S_INT_64:
        {
            _ProcessMap<u_int_64, GpTypePropInfoGetter_Map>(aStruct, aPropInfo, jsonObject, aJsonAllocator, aFlags);
        } break;
        case GpType::DOUBLE:
        {
            _ProcessMap<double, GpTypePropInfoGetter_Map>(aStruct, aPropInfo, jsonObject, aJsonAllocator, aFlags);
        } break;
        case GpType::FLOAT:
        {
            _ProcessMap<float, GpTypePropInfoGetter_Map>(aStruct, aPropInfo, jsonObject, aJsonAllocator, aFlags);
        } break;
        case GpType::BOOLEAN:
        {
            THROW_GPE("bool as map key are not supported"_sv);
        } break;
        case GpType::UUID:
        {
            _ProcessMap<GpUUID, GpTypePropInfoGetter_Map>(aStruct, aPropInfo, jsonObject, aJsonAllocator, aFlags);
        } break;
        case GpType::STRING:
        {
            _ProcessMap<std::string, GpTypePropInfoGetter_Map>(aStruct, aPropInfo, jsonObject, aJsonAllocator, aFlags);
        } break;
        case GpType::BLOB:
        {
            _ProcessMap<GpBytesArray, GpTypePropInfoGetter_Map>(aStruct, aPropInfo, jsonObject, aJsonAllocator, aFlags);
        } break;
        case GpType::STRUCT:
        {
            THROW_GPE("struct as map key are not supported"_sv);
        } break;
        case GpType::STRUCT_SP:
        {
            THROW_GPE("struct::sp as map key are not supported"_sv);
        } break;
        case GpType::ENUM:
        {
            THROW_GPE("enums as map key are not supported"_sv);
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
