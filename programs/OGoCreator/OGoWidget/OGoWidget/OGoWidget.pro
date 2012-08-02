CONFIG      += designer plugin debug_and_release
TARGET      = $$qtLibraryTarget(ogographviewplugin)
TEMPLATE    = lib

HEADERS     = ogographviewplugin.h
SOURCES     = ogographviewplugin.cpp
RESOURCES   = icons.qrc
LIBS        += -L. 

target.path = $$[QT_INSTALL_PLUGINS]/designer
INSTALLS    += target

include(ogographview.pri)
