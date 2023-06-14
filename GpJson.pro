TEMPLATE        = lib
#CONFIG         += staticlib
VER_MAJ		    = 1
VER_MIN		    = 3
VER_PAT		    = 0
QMAKE_CXXFLAGS += -DGP_MODULE_UUID=73be6325-e256-436e-9db1-82788105e16e
QMAKE_CXXFLAGS += -DGP_REFLECTION_STATIC_ADD_TO_MANAGER
DEFINES		   += GP_JSON_LIBRARY
PACKET_NAME     = GpJson
DIR_LEVEL       = ./..

include(../../QtGlobalPro.pri)

#------------------------------ LIBS BEGIN ---------------------------------
os_windows{
}

os_linux{
}
#------------------------------- LIBS END ----------------------------------

SOURCES += \
	GpJsonFromObject.cpp \
	GpJsonFromXml.cpp \
	GpJsonSerializer.cpp \
	GpJsonSerializerCtx.cpp \
	GpJsonSerializerFactory.cpp \
	GpJsonSerializerFlags.cpp \
	GpJsonToObject.cpp

HEADERS += \
    GpJsonConfigure.hpp \
    GpJsonFromObject.hpp \
    GpJsonFromXml.hpp \
    GpJsonSerializer.hpp \
    GpJsonSerializerCtx.hpp \
    GpJsonSerializerFactory.hpp \
    GpJsonSerializerFlags.hpp \
    GpJsonToObject.hpp \
    GpJsonUtilsInternal.hpp \
    GpJson_global.hpp \
    GpJson.hpp
