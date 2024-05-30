#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#include "GpJsonConfigure.hpp"

#if defined(GP_USE_JSON_FROM_XML)

#include "GpJson_global.hpp"

#include <GpCore2/GpUtils/Macro/GpMacroClass.hpp>
#include <string>

namespace GPlatform {

class GP_JSON_API GpJsonFromXml
{
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpJsonFromXml)

public:
    static std::string      SJsonFromXml    (std::string_view aXML);
};

}// namespace GPlatform

#endif// #if defined(GP_USE_JSON_FROM_XML)
