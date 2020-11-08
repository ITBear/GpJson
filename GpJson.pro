TEMPLATE        = lib
#CONFIG         += staticlib
VER_MAJ		    = 0
VER_MIN		    = 1
VER_PAT		    = 0
QMAKE_CXXFLAGS += -DGP_MODULE_UUID=73be6325-e256-436e-9db1-82788105e16e
QMAKE_CXXFLAGS += -DGP_TYPE_SYSTEM_STATIC_ADD_TO_MANAGER
DEFINES		   += GPJSON_LIBRARY
PACKET_NAME     = GpJson
DIR_LEVEL       = ./..

include(../../QtGlobalPro.pri)

#------------------------------ LIBS BEGIN ---------------------------------
os_windows{
	GP_CORE_LIB_V		= 2
}

os_linux{
}

LIBS += -lGpCore2$$TARGET_POSTFIX$$GP_CORE_LIB_V
#------------------------------ LIBS END ---------------------------------

SOURCES += \
	GpJsonFromStruct.cpp \
	GpJsonMapper.cpp \
	GpJsonMapperFlags.cpp \
	GpJsonToStruct.cpp

HEADERS += \
    GpJsonFromStruct.hpp \
    GpJsonMapper.hpp \
    GpJsonMapperFlags.hpp \
    GpJsonMapperTypeDetector.hpp \
    GpJsonToStruct.hpp \
    GpJson_global.hpp \
    GpJson.hpp
