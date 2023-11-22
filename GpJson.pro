TEMPLATE        = lib
#CONFIG         += staticlib
VER_MAJ		    = 2
VER_MIN		    = 4
VER_PAT		    = 1
QMAKE_CXXFLAGS += -DGP_MODULE_UUID=73be6325-e256-436e-9db1-82788105e16e
QMAKE_CXXFLAGS += -DGP_REFLECTION_STATIC_ADD_TO_MANAGER
PACKET_NAME     = GpJson
DIR_LEVEL       = ./..

DEFINES		   += GP_JSON_LIBRARY
DEFINES        += "GP_CURRENT_LIB_VER_MAJ=\\\"$$VER_MAJ\\\""
DEFINES        += "GP_CURRENT_LIB_VER_MIN=\\\"$$VER_MIN\\\""
DEFINES        += "GP_CURRENT_LIB_VER_PAT=\\\"$$VER_PAT\\\""
DEFINES        += "GP_CURRENT_LIB_PACKET_NAME=\\\"$$PACKET_NAME\\\""

include(../../QtGlobalPro.pri)

#------------------------------ LIBS BEGIN ---------------------------------
os_windows{
}

os_linux{
}
#------------------------------- LIBS END ----------------------------------

SOURCES += \
	GpJson.cpp \
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
