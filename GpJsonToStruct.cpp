#include "GpJsonToStruct.hpp"

namespace GPlatform {

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

rapidjson::Document::ConstObject    GpJsonToStruct::SParseJsonDOM (char* aJsonData, rapidjson::Document& aJsonDOM)
{
    THROW_GPE_COND_CHECK_M(aJsonData != nullptr, "Json data is null"_sv);

    //Check for errors
    if (aJsonDOM.ParseInsitu(const_cast<char*>(aJsonData)).HasParseError())
    {
        const rapidjson::ParseErrorCode parseErrorCode = aJsonDOM.GetParseError();
        THROW_GPE("JSON parse error: "_sv + sParseErrorCodes.at(int(parseErrorCode)));
    }

    //Check for root element is object
    THROW_GPE_COND_CHECK_M(aJsonDOM.IsObject(), "Json root element must be object"_sv);

    return const_cast<const rapidjson::Document&>(aJsonDOM).GetObject();
}

const GpTypeStructInfo& GpJsonToStruct::SFindStructInfo (const rapidjson::Document::ConstObject& aJsonObject)
{
    //Detect type struct info
    rapidjson::Document::ConstMemberIterator mit = aJsonObject.FindMember("@");
    THROW_GPE_COND_CHECK_M(mit != aJsonObject.MemberEnd(), "Json member '@' was not found"_sv);

    //Get struct uid
    GpUUID structUID;

    try
    {
        structUID = GpUUID::SFromString(mit->value.GetString());
    } catch (...)
    {
        THROW_GPE("Json member '@' must be UUID"_sv);
    }

    //Find struct info by uid
    const auto structUidOpt = GpTypeManager::S().Find(structUID);

    THROW_GPE_COND_CHECK_M(structUidOpt.has_value(), "Struct info was not found by uid = '"_sv + structUID.ToString() + "'"_sv);

     return structUidOpt.value();
}

const GpTypeStructInfo&     GpJsonToStruct::SCheckStructInfo (const rapidjson::Document::ConstObject&   aJonObject,
                                                              const GpTypeStructInfo&                   aStructInfoBase,
                                                              const CheckMode                           aCheckMode)
{
    const GpTypeStructInfo& structInfoJson = SFindStructInfo(aJonObject);
    if (aStructInfoBase.UID() != structInfoJson.UID())
    {
        if (aCheckMode == CheckMode::MUST_BE_EQUAL)
        {
            THROW_GPE("Struct UID`s in json and in c++ must be equal. But c++ struct UID "_sv + aStructInfoBase.UID().ToString()
                      + " and json struct UID "_sv + structInfoJson.UID().ToString());
        } else if (GpTypeManager::S().IsBaseOf(aStructInfoBase.UID(), structInfoJson.UID()) == false)
        {
            THROW_GPE("Struct with UID "_sv + aStructInfoBase.UID().ToString()
                      + " must be base of struct with UID "_sv + structInfoJson.UID().ToString());
        }
    }

    return structInfoJson;
}

void    GpJsonToStruct::SReadStruct (GpTypeStructBase&                          aStruct,
                                     const rapidjson::Document::ConstObject&    aJsonObject)
{
    const GpTypeStructInfo& structInfo  = aStruct.TypeStructInfo();
    const auto&             props       = structInfo.Props();

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
            THROW_GPE("Failed to read value from json. Struct "_sv + structInfo.Name() + "."_sv + propInfo.Name() + "\nReason:\n"_sv + e.what());
        } catch (...)
        {
            THROW_GPE("Failed to read value from json. Struct "_sv + structInfo.Name() + "."_sv + propInfo.Name() + "\nReason:\nUnknown exception");
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
    rapidjson::Document::ConstMemberIterator mit = aJsonObject.FindMember(propName.data());

    if (   (mit == aJsonObject.MemberEnd())
        || (mit->value.IsNull()))
    {
        return;
    }

    switch (propType)
    {
        case GpType::U_INT_8:
        {
            aPropInfo.Value_UInt8(aStruct) = NumOps::SConvert<u_int_8>(mit->value.GetUint64());
        } break;
        case GpType::S_INT_8:
        {
            aPropInfo.Value_UInt8(aStruct) = NumOps::SConvert<s_int_8>(mit->value.GetInt64());
        } break;
        case GpType::U_INT_16:
        {
            aPropInfo.Value_UInt16(aStruct) = NumOps::SConvert<u_int_16>(mit->value.GetUint64());
        } break;
        case GpType::S_INT_16:
        {
            aPropInfo.Value_UInt16(aStruct) = NumOps::SConvert<s_int_16>(mit->value.GetInt64());
        } break;
        case GpType::U_INT_32:
        {
            aPropInfo.Value_UInt32(aStruct) = NumOps::SConvert<u_int_32>(mit->value.GetUint64());
        } break;
        case GpType::S_INT_32:
        {
            aPropInfo.Value_UInt32(aStruct) = NumOps::SConvert<s_int_32>(mit->value.GetInt64());
        } break;
        case GpType::U_INT_64:
        {
            aPropInfo.Value_UInt64(aStruct) = NumOps::SConvert<u_int_64>(mit->value.GetUint64());
        } break;
        case GpType::S_INT_64:
        {
            aPropInfo.Value_UInt64(aStruct) = NumOps::SConvert<s_int_64>(mit->value.GetInt64());
        } break;
        case GpType::DOUBLE:
        {
            aPropInfo.Value_Double(aStruct) = NumOps::SConvert<double>(mit->value.GetDouble());
        } break;
        case GpType::FLOAT:
        {
            aPropInfo.Value_Float(aStruct) = NumOps::SConvert<float>(mit->value.GetDouble());
        } break;
        case GpType::BOOLEAN:
        {
            aPropInfo.Value_Bool(aStruct) = mit->value.GetBool();
        } break;
        case GpType::UUID:
        {
            aPropInfo.Value_UUID(aStruct) = GpUUID::SFromString(mit->value.GetString());
        } break;
        case GpType::STRING:
        {
            aPropInfo.Value_String(aStruct) = mit->value.GetString();
        } break;
        case GpType::BLOB:
        {
            GpRawPtrCharR strPtr(mit->value.GetString(), mit->value.GetStringLength());
            aPropInfo.Value_BLOB(aStruct) = GpStringOps::SToBytes(strPtr);
        } break;
        case GpType::STRUCT:
        {
            GpTypeStructBase&                   structBase      = aPropInfo.Value_Struct(aStruct);
            const GpTypeStructInfo&             structInfoBase  = structBase.TypeStructInfo();
            rapidjson::Document::ConstObject    jsonObject      = mit->value.GetObject();
            GpJsonToStruct::SCheckStructInfo(jsonObject, structInfoBase, GpJsonToStruct::CheckMode::MUST_BE_EQUAL);
            SReadStruct(structBase, jsonObject);
        } break;
        case GpType::STRUCT_SP:
        {
            GpTypeStructBase::SP&               structBaseSP    = aPropInfo.Value_StructSP(aStruct);
            const GpTypeStructInfo&             structInfoBase  = GpTypeManager::S().Find(aPropInfo.StructTypeUID()).value();
            rapidjson::Document::ConstObject    jsonObject      = mit->value.GetObject();
            const GpTypeStructInfo&             structInfoJson  = GpJsonToStruct::SCheckStructInfo(jsonObject, structInfoBase, GpJsonToStruct::CheckMode::CAN_BE_DERIVED);
                                                structBaseSP    = structInfoJson.NewInstance();
            GpTypeStructBase&                   structBase      = structBaseSP.Vn();
            SReadStruct(structBase, jsonObject);
        } break;
        case GpType::ENUM:
        {
            aPropInfo.Value_Enum(aStruct).FromString(mit->value.GetString());
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
    const GpType::EnumT propType = aPropInfo.Type();
    std::string_view    propName = aPropInfo.Name();

    //Find json member
    rapidjson::Document::ConstMemberIterator mit = aJsonObject.FindMember(propName.data());

    if (   (mit == aJsonObject.MemberEnd())
        || (mit->value.IsNull()))
    {
        return;
    }

    THROW_GPE_COND_CHECK_M(mit->value.IsArray(), "Json value '"_sv + propName + "' must be array"_sv);

    rapidjson::Document::ConstArray array = mit->value.GetArray();

    switch (propType)
    {
        case GpType::U_INT_8:
        {
            GpVector<u_int_8>& propVec = aPropInfo.Value_Vec_UInt8(aStruct);
            propVec.clear();
            propVec.reserve(array.Size());
            for (const auto& v: array)
            {
                propVec.emplace_back(NumOps::SConvert<u_int_8>(v.GetUint64()));
            }
        } break;
        case GpType::S_INT_8:
        {
            GpVector<s_int_8>& propVec = aPropInfo.Value_Vec_SInt8(aStruct);
            propVec.clear();
            propVec.reserve(array.Size());
            for (const auto& v: array)
            {
                propVec.emplace_back(NumOps::SConvert<s_int_8>(v.GetInt64()));
            }
        } break;
        case GpType::U_INT_16:
        {
            GpVector<u_int_16>& propVec = aPropInfo.Value_Vec_UInt16(aStruct);
            propVec.clear();
            propVec.reserve(array.Size());
            for (const auto& v: array)
            {
                propVec.emplace_back(NumOps::SConvert<u_int_16>(v.GetUint64()));
            }
        } break;
        case GpType::S_INT_16:
        {
            GpVector<s_int_16>& propVec = aPropInfo.Value_Vec_SInt16(aStruct);
            propVec.clear();
            propVec.reserve(array.Size());
            for (const auto& v: array)
            {
                propVec.emplace_back(NumOps::SConvert<s_int_16>(v.GetInt64()));
            }
        } break;
        case GpType::U_INT_32:
        {
            GpVector<u_int_32>& propVec = aPropInfo.Value_Vec_UInt32(aStruct);
            propVec.clear();
            propVec.reserve(array.Size());
            for (const auto& v: array)
            {
                propVec.emplace_back(NumOps::SConvert<u_int_32>(v.GetUint64()));
            }
        } break;
        case GpType::S_INT_32:
        {
            GpVector<s_int_32>& propVec = aPropInfo.Value_Vec_SInt32(aStruct);
            propVec.clear();
            propVec.reserve(array.Size());
            for (const auto& v: array)
            {
                propVec.emplace_back(NumOps::SConvert<s_int_32>(v.GetInt64()));
            }
        } break;
        case GpType::U_INT_64:
        {
            GpVector<u_int_64>& propVec = aPropInfo.Value_Vec_UInt64(aStruct);
            propVec.clear();
            propVec.reserve(array.Size());
            for (const auto& v: array)
            {
                propVec.emplace_back(NumOps::SConvert<u_int_64>(v.GetUint64()));
            }
        } break;
        case GpType::S_INT_64:
        {
            GpVector<s_int_64>& propVec = aPropInfo.Value_Vec_SInt64(aStruct);
            propVec.clear();
            propVec.reserve(array.Size());
            for (const auto& v: array)
            {
                propVec.emplace_back(NumOps::SConvert<s_int_64>(v.GetInt64()));
            }
        } break;
        case GpType::DOUBLE:
        {
            GpVector<double>& propVec = aPropInfo.Value_Vec_Double(aStruct);
            propVec.clear();
            propVec.reserve(array.Size());
            for (const auto& v: array)
            {
                propVec.emplace_back(NumOps::SConvert<double>(v.GetDouble()));
            }
        } break;
        case GpType::FLOAT:
        {
            GpVector<float>& propVec = aPropInfo.Value_Vec_Float(aStruct);
            propVec.clear();
            propVec.reserve(array.Size());
            for (const auto& v: array)
            {
                propVec.emplace_back(NumOps::SConvert<float>(v.GetDouble()));
            }
        } break;
        case GpType::BOOLEAN:
        {
            GpVector<bool>& propVec = aPropInfo.Value_Vec_Bool(aStruct);
            propVec.clear();
            propVec.reserve(array.Size());
            for (const auto& v: array)
            {
                propVec.emplace_back(v.GetBool());
            }
        } break;
        case GpType::UUID:
        {
            GpVector<GpUUID>& propVec = aPropInfo.Value_Vec_UUID(aStruct);
            propVec.clear();
            propVec.reserve(array.Size());
            for (const auto& v: array)
            {
                propVec.emplace_back(GpUUID::SFromString(v.GetString()));
            }
        } break;
        case GpType::STRING:
        {
            GpVector<std::string>& propVec = aPropInfo.Value_Vec_String(aStruct);
            propVec.clear();
            propVec.reserve(array.Size());
            for (const auto& v: array)
            {
                propVec.emplace_back(v.GetString());
            }
        } break;
        case GpType::BLOB:
        {
            GpVector<GpBytesArray>& propVec = aPropInfo.Value_Vec_BLOB(aStruct);
            propVec.clear();
            propVec.reserve(array.Size());
            for (const auto& v: array)
            {
                GpRawPtrCharR strPtr(v.GetString(),v.GetStringLength());
                propVec.emplace_back(GpStringOps::SToBytes(strPtr));
            }
        } break;
        case GpType::STRUCT:
        {
            THROW_GPE("Arrays of struct are not supported, use arrays of struct::SP instead"_sv);
        } break;
        case GpType::STRUCT_SP:
        {
            GpVector<GpTypeStructBase::SP>& propVec = aPropInfo.Value_Vec_StructSP(aStruct);
            propVec.clear();
            propVec.reserve(array.Size());

            const GpTypeStructInfo& structInfoBase = GpTypeManager::S().Find(aPropInfo.StructTypeUID()).value();

            for (const auto& v: array)
            {
                const rapidjson::Document::ConstObject& jsonObject      = v.GetObject();
                const GpTypeStructInfo&                 structInfoJson  = GpJsonToStruct::SCheckStructInfo(jsonObject, structInfoBase, GpJsonToStruct::CheckMode::CAN_BE_DERIVED);
                GpTypeStructBase::SP                    structBaseSP    = structInfoJson.NewInstance();
                GpTypeStructBase&                       structBase      = structBaseSP.Vn();
                SReadStruct(structBase, jsonObject);

                propVec.emplace_back(std::move(structBaseSP));
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

}//namespace GPlatform
