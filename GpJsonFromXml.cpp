#include "GpJsonFromXml.hpp"

#if defined(GP_USE_JSON_FROM_XML)

#include <GpCore2/GpUtils/Exceptions/GpException.hpp>

namespace GPlatform {

std::string GpJsonFromXml::SJsonFromXml (std::string_view /*aXML*/)
{
    //TODO: implement
    THROW_GP_NOT_IMPLEMENTED();
}

}// namespace GPlatform

#endif// #if defined(GP_USE_JSON_FROM_XML)
