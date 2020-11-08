#include "GpJsonToStruct.hpp"

namespace GPlatform {

class _PreprocVec
{
public:
    template<typename T>
    static void SProc(GpVector<T>& aVec, const size_t aSize)
    {
        aVec.clear();
        aVec.reserve(aSize);
    }
};

class _PreprocList
{
public:
    template<typename T>
    static void SProc(GpList<T>& aVec, const size_t /*aSize*/)
    {
        aVec.clear();
    }
};

class _PreprocSet
{
public:
    template<typename T>
    static void SProc(GpSet<T>& aVec, const size_t /*aSize*/)
    {
        aVec.clear();
    }
};

class _InserterVec
{
public:
    template<typename T, typename... Ts>
    static void SInsert(GpVector<T>& aVec, Ts&&... aArgs)
    {
        aVec.emplace_back(std::forward<Ts>(aArgs)...);
    }
};

class _InserterList
{
public:
    template<typename T, typename... Ts>
    static void SInsert(GpList<T>& aVec, Ts&&... aArgs)
    {
        aVec.emplace_back(std::forward<Ts>(aArgs)...);
    }
};

class _InserterSet
{
public:
    template<typename T, typename... Ts>
    static void SInsert(GpSet<T>& aVec, Ts&&... aArgs)
    {
        aVec.insert(std::forward<Ts>(aArgs)...);
    }
};


template<typename Prepocessor,
         typename Inserter,
         typename ValGetter>
void    _ReadContainer (GpTypeStructBase&                           aStruct,
                        const GpTypePropInfo&                       aPropInfo,
                        const rapidjson::Document::ConstObject&     aJsonObject)
{
    const GpType::EnumT propType = aPropInfo.Type();
    std::string_view    propName = aPropInfo.Name();

    //Find json member
    rapidjson::Document::ConstMemberIterator mit = aJsonObject.FindMember(rapidjson::Document::ValueType(propName.data(),
                                                                                                         NumOps::SConvert<rapidjson::SizeType>(propName.size())));

    if (   (mit == aJsonObject.MemberEnd())
        || (mit->value.IsNull()))
    {
        return;
    }

    const auto& mitVal = mit->value;

    THROW_GPE_COND_CHECK_M(mitVal.IsArray(), "Json value '"_sv + propName + "' must be array"_sv);

    rapidjson::Document::ConstArray array       = mitVal.GetArray();
    const size_t                    arraySize   = array.Size();

    switch (propType)
    {
        case GpType::U_INT_8:
        {
            auto& container = ValGetter::UInt8(aStruct, aPropInfo);
            Prepocessor::SProc(container, arraySize);
            for (const auto& v: array)
            {
                Inserter::SInsert(container, NumOps::SConvert<u_int_8>(v.GetUint64()));
            }
        } break;
        case GpType::S_INT_8:
        {
            auto& container = ValGetter::SInt8(aStruct, aPropInfo);
            Prepocessor::SProc(container, arraySize);
            for (const auto& v: array)
            {
                Inserter::SInsert(container, NumOps::SConvert<s_int_8>(v.GetInt64()));
            }
        } break;
        case GpType::U_INT_16:
        {
            auto& container = ValGetter::UInt16(aStruct, aPropInfo);
            Prepocessor::SProc(container, arraySize);
            for (const auto& v: array)
            {
                Inserter::SInsert(container, NumOps::SConvert<u_int_16>(v.GetUint64()));
            }
        } break;
        case GpType::S_INT_16:
        {
            auto& container = ValGetter::SInt16(aStruct, aPropInfo);
            Prepocessor::SProc(container, arraySize);
            for (const auto& v: array)
            {
                Inserter::SInsert(container, NumOps::SConvert<s_int_16>(v.GetInt64()));
            }
        } break;
        case GpType::U_INT_32:
        {
            auto& container = ValGetter::UInt32(aStruct, aPropInfo);
            Prepocessor::SProc(container, arraySize);
            for (const auto& v: array)
            {
                Inserter::SInsert(container, NumOps::SConvert<u_int_32>(v.GetUint64()));
            }
        } break;
        case GpType::S_INT_32:
        {
            auto& container = ValGetter::SInt32(aStruct, aPropInfo);
            Prepocessor::SProc(container, arraySize);
            for (const auto& v: array)
            {
                Inserter::SInsert(container, NumOps::SConvert<s_int_32>(v.GetInt64()));
            }
        } break;
        case GpType::U_INT_64:
        {
            auto& container = ValGetter::UInt64(aStruct, aPropInfo);
            Prepocessor::SProc(container, arraySize);
            for (const auto& v: array)
            {
                Inserter::SInsert(container, NumOps::SConvert<u_int_64>(v.GetUint64()));
            }
        } break;
        case GpType::S_INT_64:
        {
            auto& container = ValGetter::SInt64(aStruct, aPropInfo);
            Prepocessor::SProc(container, arraySize);
            for (const auto& v: array)
            {
                Inserter::SInsert(container, NumOps::SConvert<s_int_64>(v.GetInt64()));
            }
        } break;
        case GpType::DOUBLE:
        {
            auto& container = ValGetter::Double(aStruct, aPropInfo);
            Prepocessor::SProc(container, arraySize);
            for (const auto& v: array)
            {
                Inserter::SInsert(container, NumOps::SConvert<double>(v.GetDouble()));
            }
        } break;
        case GpType::FLOAT:
        {
            auto& container = ValGetter::Float(aStruct, aPropInfo);
            Prepocessor::SProc(container, arraySize);
            for (const auto& v: array)
            {
                Inserter::SInsert(container, NumOps::SConvert<float>(v.GetDouble()));
            }
        } break;
        case GpType::BOOLEAN:
        {
            auto& container = ValGetter::Bool(aStruct, aPropInfo);
            Prepocessor::SProc(container, arraySize);
            for (const auto& v: array)
            {
                Inserter::SInsert(container, v.GetBool());
            }
        } break;
        case GpType::UUID:
        {
            auto& container = ValGetter::UUID(aStruct, aPropInfo);
            Prepocessor::SProc(container, arraySize);
            for (const auto& v: array)
            {
                Inserter::SInsert(container, GpUUID::SFromString({v.GetString(), v.GetStringLength()}));
            }
        } break;
        case GpType::STRING:
        {
            auto& container = ValGetter::String(aStruct, aPropInfo);
            Prepocessor::SProc(container, arraySize);
            for (const auto& v: array)
            {
                Inserter::SInsert(container, std::string(v.GetString(), v.GetStringLength()));
            }
        } break;
        case GpType::BLOB:
        {
            auto& container = ValGetter::BLOB(aStruct, aPropInfo);
            Prepocessor::SProc(container, arraySize);
            for (const auto& v: array)
            {
                GpRawPtrCharR strPtr(v.GetString(), v.GetStringLength());
                Inserter::SInsert(container, GpStringOps::SToBytes(strPtr));
            }
        } break;
        case GpType::STRUCT:
        {
            THROW_GPE("Arrays of struct are not supported, use arrays of struct::SP instead"_sv);
        } break;
        case GpType::STRUCT_SP:
        {
            auto& container = ValGetter::StructSP(aStruct, aPropInfo);
            Prepocessor::SProc(container, arraySize);

            const GpTypeStructInfo& typeInfoBase = GpTypeManager::S().Find(aPropInfo.StructTypeUID()).value();

            for (const auto& v: array)
            {
                const rapidjson::Document::ConstObject& jsonObject      = v.GetObject();
                const GpTypeStructInfo&                 typeInfoJson    = GpJsonToStruct::SCheckTypeInfo(jsonObject, typeInfoBase, GpJsonToStruct::CheckMode::CAN_BE_DERIVED);
                GpTypeStructBase::SP                    structBaseSP    = typeInfoJson.NewInstance();
                GpTypeStructBase&                       structBase      = structBaseSP.Vn();
                GpJsonToStruct::SReadStruct(structBase, jsonObject);

                Inserter::SInsert(container, std::move(structBaseSP));
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
}

template<typename T>
T   _ReadMapKey (std::string_view aValue)
{
    constexpr const GpType::EnumT type = GpTypeUtils::SDetectType<T>();

    if constexpr (type == GpType::U_INT_8)
    {
        return NumOps::SConvert<u_int_8>(GpStringOps::SToUI64(aValue));
    } else if constexpr (type == GpType::S_INT_8)
    {
        return NumOps::SConvert<s_int_8>(GpStringOps::SToSI64(aValue));
    } else if constexpr (type == GpType::U_INT_16)
    {
        return NumOps::SConvert<u_int_16>(GpStringOps::SToUI64(aValue));
    } else if constexpr (type == GpType::S_INT_16)
    {
        return NumOps::SConvert<s_int_16>(GpStringOps::SToSI64(aValue));
    } else if constexpr (type == GpType::U_INT_32)
    {
        return NumOps::SConvert<u_int_32>(GpStringOps::SToUI64(aValue));
    } else if constexpr (type == GpType::S_INT_32)
    {
        return NumOps::SConvert<s_int_32>(GpStringOps::SToSI64(aValue));
    } else if constexpr (type == GpType::U_INT_64)
    {
        return NumOps::SConvert<u_int_64>(GpStringOps::SToUI64(aValue));
    } else if constexpr (type == GpType::S_INT_64)
    {
        return NumOps::SConvert<s_int_64>(GpStringOps::SToSI64(aValue));
    } else if constexpr (type == GpType::DOUBLE)
    {
        return GpStringOps::SToDouble_fast(aValue);
    } else if constexpr (type == GpType::FLOAT)
    {
        return NumOps::SConvert<float>(GpStringOps::SToDouble_fast(aValue));
    } else if constexpr (type == GpType::BOOLEAN)
    {
        GpThrowCe<std::out_of_range>("Bool as map key are not supported");
    } else if constexpr (type == GpType::UUID)
    {
        return GpUUID::SFromString(aValue);
    } else if constexpr (type == GpType::STRING)
    {
        return std::string(aValue);
    } else if constexpr (type == GpType::BLOB)
    {
        return GpStringOps::SToBytes(aValue);
    } else if constexpr (type == GpType::STRUCT)
    {
        GpThrowCe<std::out_of_range>("Struct as map key are not supported");
    } else if constexpr (type == GpType::STRUCT_SP)
    {
        GpThrowCe<std::out_of_range>("Struct SP as map key are not supported");
    } else if constexpr (type == GpType::ENUM)
    {
        GpThrowCe<std::out_of_range>("Enum as map key are not supported");
    } else
    {
        GpThrowCe<std::out_of_range>("Unknown type '"_sv + GpTypeUtils::STypeName<T>() + "'");
    }
}

template<typename                       Key,
         template<typename...> class    ValGetter>
void    _ReadMap    (GpTypeStructBase&                          aStruct,
                     const GpTypePropInfo&                      aPropInfo,
                     const rapidjson::Document::ConstObject&    aJsonObject)
{
    const GpType::EnumT propType = aPropInfo.Type();

    switch (propType)
    {
        case GpType::U_INT_8:
        {
            auto& container = ValGetter<Key>::UInt8(aStruct, aPropInfo);
            for (const auto& v: aJsonObject)
            {
                const auto& name    = v.name;
                const auto& value   = v.value;
                container.emplace(_ReadMapKey<Key>(std::string_view(name.GetString(), name.GetStringLength())),
                                  NumOps::SConvert<u_int_8>(value.GetUint64()));
            }
        } break;
        case GpType::S_INT_8:
        {
            auto& container = ValGetter<Key>::SInt8(aStruct, aPropInfo);
            for (const auto& v: aJsonObject)
            {
                const auto& name    = v.name;
                const auto& value   = v.value;
                container.emplace(_ReadMapKey<Key>(std::string_view(name.GetString(), name.GetStringLength())),
                                  NumOps::SConvert<s_int_8>(value.GetInt64()));
            }
        } break;
        case GpType::U_INT_16:
        {
            auto& container = ValGetter<Key>::UInt16(aStruct, aPropInfo);
            for (const auto& v: aJsonObject)
            {
                const auto& name    = v.name;
                const auto& value   = v.value;
                container.emplace(_ReadMapKey<Key>(std::string_view(name.GetString(), name.GetStringLength())),
                                  NumOps::SConvert<u_int_16>(value.GetUint64()));
            }
        } break;
        case GpType::S_INT_16:
        {
            auto& container = ValGetter<Key>::SInt16(aStruct, aPropInfo);
            for (const auto& v: aJsonObject)
            {
                const auto& name    = v.name;
                const auto& value   = v.value;
                container.emplace(_ReadMapKey<Key>(std::string_view(name.GetString(), name.GetStringLength())),
                                  NumOps::SConvert<s_int_16>(value.GetInt64()));
            }
        } break;
        case GpType::U_INT_32:
        {
            auto& container = ValGetter<Key>::UInt32(aStruct, aPropInfo);
            for (const auto& v: aJsonObject)
            {
                const auto& name    = v.name;
                const auto& value   = v.value;
                container.emplace(_ReadMapKey<Key>(std::string_view(name.GetString(), name.GetStringLength())),
                                  NumOps::SConvert<u_int_32>(value.GetUint64()));
            }
        } break;
        case GpType::S_INT_32:
        {
            auto& container = ValGetter<Key>::SInt32(aStruct, aPropInfo);
            for (const auto& v: aJsonObject)
            {
                const auto& name    = v.name;
                const auto& value   = v.value;
                container.emplace(_ReadMapKey<Key>(std::string_view(name.GetString(), name.GetStringLength())),
                                  NumOps::SConvert<s_int_32>(value.GetInt64()));
            }
        } break;
        case GpType::U_INT_64:
        {
            auto& container = ValGetter<Key>::UInt64(aStruct, aPropInfo);
            for (const auto& v: aJsonObject)
            {
                const auto& name    = v.name;
                const auto& value   = v.value;
                container.emplace(_ReadMapKey<Key>(std::string_view(name.GetString(), name.GetStringLength())),
                                  NumOps::SConvert<u_int_64>(value.GetUint64()));
            }
        } break;
        case GpType::S_INT_64:
        {
            auto& container = ValGetter<Key>::SInt64(aStruct, aPropInfo);
            for (const auto& v: aJsonObject)
            {
                const auto& name    = v.name;
                const auto& value   = v.value;
                container.emplace(_ReadMapKey<Key>(std::string_view(name.GetString(), name.GetStringLength())),
                                  NumOps::SConvert<s_int_64>(value.GetInt64()));
            }
        } break;
        case GpType::DOUBLE:
        {
            auto& container = ValGetter<Key>::Double(aStruct, aPropInfo);
            for (const auto& v: aJsonObject)
            {
                const auto& name    = v.name;
                const auto& value   = v.value;
                container.emplace(_ReadMapKey<Key>(std::string_view(name.GetString(), name.GetStringLength())),
                                  NumOps::SConvert<double>(value.GetDouble()));
            }
        } break;
        case GpType::FLOAT:
        {
            auto& container = ValGetter<Key>::Float(aStruct, aPropInfo);
            for (const auto& v: aJsonObject)
            {
                const auto& name    = v.name;
                const auto& value   = v.value;
                container.emplace(_ReadMapKey<Key>(std::string_view(name.GetString(), name.GetStringLength())),
                                  NumOps::SConvert<float>(value.GetDouble()));
            }
        } break;
        case GpType::BOOLEAN:
        {
            auto& container = ValGetter<Key>::Bool(aStruct, aPropInfo);
            for (const auto& v: aJsonObject)
            {
                const auto& name    = v.name;
                const auto& value   = v.value;
                container.emplace(_ReadMapKey<Key>(std::string_view(name.GetString(), name.GetStringLength())),
                                  value.GetBool());
            }
        } break;
        case GpType::UUID:
        {
            auto& container = ValGetter<Key>::UUID(aStruct, aPropInfo);
            for (const auto& v: aJsonObject)
            {
                const auto& name    = v.name;
                const auto& value   = v.value;
                container.emplace(_ReadMapKey<Key>(std::string_view(name.GetString(), name.GetStringLength())),
                                  GpUUID::SFromString({value.GetString(), value.GetStringLength()}));
            }
        } break;
        case GpType::STRING:
        {
            auto& container = ValGetter<Key>::String(aStruct, aPropInfo);
            for (const auto& v: aJsonObject)
            {
                const auto& name    = v.name;
                const auto& value   = v.value;

                std::string s({value.GetString(), value.GetStringLength()});

                container.emplace(_ReadMapKey<Key>(std::string_view(name.GetString(), name.GetStringLength())),
                                  std::move(s));
            }
        } break;
        case GpType::BLOB:
        {
            auto& container = ValGetter<Key>::BLOB(aStruct, aPropInfo);
            for (const auto& v: aJsonObject)
            {
                const auto& name    = v.name;
                const auto& value   = v.value;

                GpRawPtrCharR   strPtr(value.GetString(), value.GetStringLength());
                GpBytesArray    data = GpStringOps::SToBytes(strPtr);

                container.emplace(_ReadMapKey<Key>(std::string_view(name.GetString(), name.GetStringLength())),
                                  std::move(data));
            }
        } break;
        case GpType::STRUCT:
        {
            THROW_GPE("struct as map value are not supported, use struct::SP instead"_sv);
        } break;
        case GpType::STRUCT_SP:
        {
            auto& container = ValGetter<Key>::StructSP(aStruct, aPropInfo);

            const GpTypeStructInfo& typeInfoBase = GpTypeManager::S().Find(aPropInfo.StructTypeUID()).value();

            for (const auto& v: aJsonObject)
            {
                const auto& name    = v.name;
                const auto& value   = v.value;

                const rapidjson::Document::ConstObject& jsonObject      = value.GetObject();
                const GpTypeStructInfo&                 typeInfoJson    = GpJsonToStruct::SCheckTypeInfo(jsonObject,
                                                                                                         typeInfoBase,
                                                                                                         GpJsonToStruct::CheckMode::CAN_BE_DERIVED);
                GpTypeStructBase::SP                    structBaseSP    = typeInfoJson.NewInstance();
                GpTypeStructBase&                       structBase      = structBaseSP.Vn();
                GpJsonToStruct::SReadStruct(structBase, jsonObject);

                container.emplace(_ReadMapKey<Key>(std::string_view(name.GetString(), name.GetStringLength())),
                                  std::move(structBaseSP));
            }
        } break;
        case GpType::ENUM:
        {
            THROW_GPE("enums as map value are not supported"_sv);
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

const std::array<std::string_view, 18>  GpJsonToStruct::sParseErrorCodes =
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

rapidjson::Document::ConstObject    GpJsonToStruct::SParseJsonDOM (GpRawPtrCharRW aJsonData, rapidjson::Document& aJsonDOM)
{
    THROW_GPE_COND_CHECK_M(!aJsonData.IsEmpty(), "Json data is null"_sv);

    //Check for errors
    if (aJsonDOM.ParseInsitu(aJsonData.Ptr()).HasParseError())
    {
        const rapidjson::ParseErrorCode parseErrorCode = aJsonDOM.GetParseError();
        THROW_GPE("JSON parse error: "_sv + sParseErrorCodes.at(int(parseErrorCode)));
    }

    //Check for root element is object
    THROW_GPE_COND_CHECK_M(aJsonDOM.IsObject(), "Json root element must be object"_sv);

    return const_cast<const rapidjson::Document&>(aJsonDOM).GetObject();
}

const GpTypeStructInfo&     GpJsonToStruct::SCheckTypeInfo (const rapidjson::Document::ConstObject& aJonObject,
                                                            const GpTypeStructInfo&                 aTypeInfoBase,
                                                            const CheckMode                         aCheckMode)
{
    FindTypeInfoResT findRes = SFindTypeInfo(aJonObject);

    if (findRes.has_value())
    {
        const GpTypeStructInfo& typeInfoJson = findRes.value().get();
        if (aTypeInfoBase.UID() != typeInfoJson.UID())
        {
            if (aCheckMode == CheckMode::MUST_BE_EQUAL)
            {
                THROW_GPE("Struct UID`s in json and in c++ must be equal. But c++ struct UID "_sv + aTypeInfoBase.UID().ToString()
                          + " and json struct UID "_sv + typeInfoJson.UID().ToString());
            } else if (GpTypeManager::S().IsBaseOf(aTypeInfoBase.UID(), typeInfoJson.UID()) == false)
            {
                THROW_GPE("Struct with UID "_sv + aTypeInfoBase.UID().ToString()
                          + " must be base of struct with UID "_sv + typeInfoJson.UID().ToString());
            }
        }

        return typeInfoJson;
    } else
    {
        return aTypeInfoBase;
    }
}

GpJsonToStruct::FindTypeInfoResT    GpJsonToStruct::SFindTypeInfo (const rapidjson::Document::ConstObject& aJsonObject)
{
    //Detect type struct info
    rapidjson::Document::ConstMemberIterator mit = aJsonObject.FindMember("@");

    if (mit == aJsonObject.MemberEnd())
    {
        return std::nullopt;
    }

    //Get struct uid
    GpUUID structUID;

    const auto& mitVal = mit->value;
    structUID = GpUUID::SFromString({mitVal.GetString(), mitVal.GetStringLength()});

    //Find struct info by uid
    const auto structUidOpt = GpTypeManager::S().Find(structUID);

    THROW_GPE_COND_CHECK_M(structUidOpt.has_value(), "Struct info was not found by uid = '"_sv + structUID.ToString() + "'"_sv);

     return structUidOpt.value();
}

void    GpJsonToStruct::SReadStruct (GpTypeStructBase&                          aStruct,
                                     const rapidjson::Document::ConstObject&    aJsonObject)
{
    const GpTypeStructInfo& typeInfo    = aStruct.TypeInfo();
    const auto&             props       = typeInfo.Props();

    for (const GpTypePropInfo& propInfo: props)
    {
        const GpTypeContainer::EnumT containerType = propInfo.Container();
        try
        {
            if (containerType == GpTypeContainer::NO)
            {
                SReadValue(aStruct, propInfo, aJsonObject);
            } else if (containerType == GpTypeContainer::VECTOR)
            {
                SReadValueVec(aStruct, propInfo, aJsonObject);
            } else if (containerType == GpTypeContainer::LIST)
            {
                SReadValueList(aStruct, propInfo, aJsonObject);
            } else if (containerType == GpTypeContainer::SET)
            {
                SReadValueSet(aStruct, propInfo, aJsonObject);
            } else if (containerType == GpTypeContainer::MAP)
            {
                SReadValueMap(aStruct, propInfo, aJsonObject);
            }
        } catch (const std::exception& e)
        {
            THROW_GPE("Failed to read value from json. Struct "_sv + typeInfo.Name() + "."_sv + propInfo.Name() + "\nReason:\n"_sv + e.what());
        } catch (...)
        {
            THROW_GPE("Failed to read value from json. Struct "_sv + typeInfo.Name() + "."_sv + propInfo.Name() + "\nReason:\nUnknown exception");
        }
    }
}

void    GpJsonToStruct::SReadValue (GpTypeStructBase&                       aStruct,
                                    const GpTypePropInfo&                   aPropInfo,
                                    const rapidjson::Document::ConstObject& aJsonObject)
{
    const GpType::EnumT propType = aPropInfo.Type();
    std::string_view    propName = aPropInfo.Name();

    //Find json member
    rapidjson::Document::ConstMemberIterator mit = aJsonObject.FindMember(rapidjson::Document::ValueType(propName.data(),
                                                                                                         NumOps::SConvert<rapidjson::SizeType>(propName.size())));

    if (   (mit == aJsonObject.MemberEnd())
        || (mit->value.IsNull()))
    {
        return;
    }

    const auto& mitVal = mit->value;

    switch (propType)
    {
        case GpType::U_INT_8:
        {
            aPropInfo.Value_UInt8(aStruct) = NumOps::SConvert<u_int_8>(mitVal.GetUint64());
        } break;
        case GpType::S_INT_8:
        {
            aPropInfo.Value_UInt8(aStruct) = NumOps::SConvert<s_int_8>(mitVal.GetInt64());
        } break;
        case GpType::U_INT_16:
        {
            aPropInfo.Value_UInt16(aStruct) = NumOps::SConvert<u_int_16>(mitVal.GetUint64());
        } break;
        case GpType::S_INT_16:
        {
            aPropInfo.Value_UInt16(aStruct) = NumOps::SConvert<s_int_16>(mitVal.GetInt64());
        } break;
        case GpType::U_INT_32:
        {
            aPropInfo.Value_UInt32(aStruct) = NumOps::SConvert<u_int_32>(mitVal.GetUint64());
        } break;
        case GpType::S_INT_32:
        {
            aPropInfo.Value_UInt32(aStruct) = NumOps::SConvert<s_int_32>(mitVal.GetInt64());
        } break;
        case GpType::U_INT_64:
        {
            aPropInfo.Value_UInt64(aStruct) = NumOps::SConvert<u_int_64>(mitVal.GetUint64());
        } break;
        case GpType::S_INT_64:
        {
            aPropInfo.Value_UInt64(aStruct) = NumOps::SConvert<s_int_64>(mitVal.GetInt64());
        } break;
        case GpType::DOUBLE:
        {
            aPropInfo.Value_Double(aStruct) = NumOps::SConvert<double>(mitVal.GetDouble());
        } break;
        case GpType::FLOAT:
        {
            aPropInfo.Value_Float(aStruct) = NumOps::SConvert<float>(mitVal.GetDouble());
        } break;
        case GpType::BOOLEAN:
        {
            aPropInfo.Value_Bool(aStruct) = mitVal.GetBool();
        } break;
        case GpType::UUID:
        {
            aPropInfo.Value_UUID(aStruct) = GpUUID::SFromString({mitVal.GetString(), mitVal.GetStringLength()});
        } break;
        case GpType::STRING:
        {
            aPropInfo.Value_String(aStruct) = std::string(mitVal.GetString(), mitVal.GetStringLength());
        } break;
        case GpType::BLOB:
        {
            GpRawPtrCharR strPtr(mitVal.GetString(), mitVal.GetStringLength());
            aPropInfo.Value_BLOB(aStruct) = GpStringOps::SToBytes(strPtr);
        } break;
        case GpType::STRUCT:
        {
            GpTypeStructBase&                   structBase      = aPropInfo.Value_Struct(aStruct);
            rapidjson::Document::ConstObject    jsonObject      = mitVal.GetObject();
            SReadStruct(structBase, jsonObject);
        } break;
        case GpType::STRUCT_SP:
        {
            GpTypeStructBase::SP&               structBaseSP    = aPropInfo.Value_StructSP(aStruct);
            const GpTypeStructInfo&             typeInfoBase    = GpTypeManager::S().Find(aPropInfo.StructTypeUID()).value();
            rapidjson::Document::ConstObject    jsonObject      = mitVal.GetObject();
            const GpTypeStructInfo&             typeInfoJson    = GpJsonToStruct::SCheckTypeInfo(jsonObject, typeInfoBase, GpJsonToStruct::CheckMode::CAN_BE_DERIVED);
                                                structBaseSP    = typeInfoJson.NewInstance();
            GpTypeStructBase&                   structBase      = structBaseSP.Vn();
            SReadStruct(structBase, jsonObject);
        } break;
        case GpType::ENUM:
        {
            aPropInfo.Value_Enum(aStruct).FromString({mitVal.GetString(), mitVal.GetStringLength()});
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

void    GpJsonToStruct::SReadValueVec (GpTypeStructBase&                        aStruct,
                                       const GpTypePropInfo&                    aPropInfo,
                                       const rapidjson::Document::ConstObject&  aJsonObject)
{
    _ReadContainer<_PreprocVec, _InserterVec, GpTypePropInfoGetter_Vec>(aStruct, aPropInfo, aJsonObject);
}

void    GpJsonToStruct::SReadValueList (GpTypeStructBase&                       aStruct,
                                        const GpTypePropInfo&                   aPropInfo,
                                        const rapidjson::Document::ConstObject& aJsonObject)
{
    _ReadContainer<_PreprocList, _InserterList, GpTypePropInfoGetter_List>(aStruct, aPropInfo, aJsonObject);
}

void    GpJsonToStruct::SReadValueSet (GpTypeStructBase&                        aStruct,
                                        const GpTypePropInfo&                   aPropInfo,
                                        const rapidjson::Document::ConstObject& aJsonObject)
{
    _ReadContainer<_PreprocSet, _InserterSet, GpTypePropInfoGetter_Set>(aStruct, aPropInfo, aJsonObject);
}

void    GpJsonToStruct::SReadValueMap (GpTypeStructBase&                        aStruct,
                                       const GpTypePropInfo&                    aPropInfo,
                                       const rapidjson::Document::ConstObject&  aJsonObject)
{
    const GpType::EnumT keyType     = aPropInfo.ContainerKeyType();
    std::string_view    propName    = aPropInfo.Name();

    //Find json member
    rapidjson::Document::ConstMemberIterator mit = aJsonObject.FindMember(rapidjson::Document::ValueType(propName.data(),
                                                                                                         NumOps::SConvert<rapidjson::SizeType>(propName.size())));

    if (   (mit == aJsonObject.MemberEnd())
        || (mit->value.IsNull()))
    {
        return;
    }

    const auto& mitVal = mit->value;

    THROW_GPE_COND_CHECK_M(mitVal.IsObject(), "Json value '"_sv + propName + "' must be object"_sv);

    rapidjson::Document::ConstObject jObj = mitVal.GetObject();

    switch (keyType)
    {
        case GpType::U_INT_8:
        {
            _ReadMap<u_int_8, GpTypePropInfoGetter_Map>(aStruct, aPropInfo, jObj);
        } break;
        case GpType::S_INT_8:
        {
            _ReadMap<s_int_8, GpTypePropInfoGetter_Map>(aStruct, aPropInfo, jObj);
        } break;
        case GpType::U_INT_16:
        {
            _ReadMap<u_int_16, GpTypePropInfoGetter_Map>(aStruct, aPropInfo, jObj);
        } break;
        case GpType::S_INT_16:
        {
            _ReadMap<s_int_16, GpTypePropInfoGetter_Map>(aStruct, aPropInfo, jObj);
        } break;
        case GpType::U_INT_32:
        {
            _ReadMap<u_int_32, GpTypePropInfoGetter_Map>(aStruct, aPropInfo, jObj);
        } break;
        case GpType::S_INT_32:
        {
            _ReadMap<s_int_32, GpTypePropInfoGetter_Map>(aStruct, aPropInfo, jObj);
        } break;
        case GpType::U_INT_64:
        {
            _ReadMap<u_int_64, GpTypePropInfoGetter_Map>(aStruct, aPropInfo, jObj);
        } break;
        case GpType::S_INT_64:
        {
            _ReadMap<u_int_64, GpTypePropInfoGetter_Map>(aStruct, aPropInfo, jObj);
        } break;
        case GpType::DOUBLE:
        {
            _ReadMap<double, GpTypePropInfoGetter_Map>(aStruct, aPropInfo, jObj);
        } break;
        case GpType::FLOAT:
        {
            _ReadMap<float, GpTypePropInfoGetter_Map>(aStruct, aPropInfo, jObj);
        } break;
        case GpType::BOOLEAN:
        {
            THROW_GPE("bool as map key are not supported"_sv);
        } break;
        case GpType::UUID:
        {
            _ReadMap<GpUUID, GpTypePropInfoGetter_Map>(aStruct, aPropInfo, jObj);
        } break;
        case GpType::STRING:
        {
            _ReadMap<std::string, GpTypePropInfoGetter_Map>(aStruct, aPropInfo, jObj);
        } break;
        case GpType::BLOB:
        {
            _ReadMap<GpBytesArray, GpTypePropInfoGetter_Map>(aStruct, aPropInfo, jObj);
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
}

}//namespace GPlatform
