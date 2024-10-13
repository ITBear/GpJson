# ----------- Config -----------
TEMPLATE        = lib
QMAKE_CXXFLAGS += -DGP_REFLECTION_STATIC_ADD_TO_MANAGER
QMAKE_CXXFLAGS += -DGP_MODULE_UUID=73be6325-e256-436e-9db1-82788105e16e
PACKET_NAME     = GpJson
DEFINES        += GP_JSON_LIBRARY
_VER_MAJ        = 2
_VER_MIN        = 1
_VER_PAT        = 6
DIR_LEVEL       = ./..

include($$DIR_LEVEL/../QtGlobalPro.pri)

equals(var_link, "static") {
	CONFIG += staticlib
}

# ----------- Libraries -----------
equals(var_os, "windows") {
	LIBS += -lGpReflection$$TARGET_POSTFIX
	LIBS += -lGpUtils$$TARGET_POSTFIX
}

equals(var_os, "linux") {
	LIBS += -lGpReflection$$TARGET_POSTFIX
	LIBS += -lGpUtils$$TARGET_POSTFIX

	LIBS += -lfmt
}

# ----------- Sources and headers -----------
SOURCES += \
	GpJsonFromObject.cpp \
	GpJsonFromXml.cpp \
	GpJsonLib.cpp \
	GpJsonSerializer.cpp \
	GpJsonSerializerCtx.cpp \
	GpJsonSerializerFactory.cpp \
	GpJsonSerializerFlags.cpp \
	GpJsonToObject.cpp

HEADERS += \
    GpJsonConfigure.hpp \
    GpJsonFromObject.hpp \
    GpJsonFromXml.hpp \
    GpJsonLib.hpp \
    GpJsonSerializer.hpp \
    GpJsonSerializerCtx.hpp \
    GpJsonSerializerFactory.hpp \
    GpJsonSerializerFlags.hpp \
    GpJsonToObject.hpp \
    GpJsonUtilsInternal.hpp \
    GpJson_global.hpp
