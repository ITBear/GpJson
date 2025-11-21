#include <GpJson/GpJsonLib.hpp>
#include <GpCore2/GpUtils/Other/GpLinkedLibsInfo.hpp>

GP_STATIC_INITIALIZER_IMPL(GpJson)
GP_LIB_REGISTRATOR(GpJsonLib)

void    GpJson_StaticInitializer::OnInitialize (void)
{
    GpJsonLib::SRegisterSelf();
}
