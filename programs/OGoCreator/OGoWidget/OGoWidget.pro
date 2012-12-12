#CONFIG      += designer plugin debug_and_release
CONFIG      += designer plugin


#TARGET      = $$qtLibraryTarget(ogographviewplugin)
     win32:CONFIG(release, debug|release): TARGET = ../../../lib/Release/ogographviewplugin
else:win32:CONFIG(debug, debug|release):   TARGET = ../../../lib/Debug/ogographviewplugindbg
else:symbian:                              TARGET = ../../../lib/Release/ogographviewplugin
else:unix:CONFIG(debug, debug|release):    TARGET = ../../../lib/Debug/ogographviewplugindbg
else:unix:CONFIG(release, debug|release):  TARGET = ../../../lib/Release/ogographviewplugin

     win32:CONFIG(release, debug|release): OBJECTS_DIR=../../../obj/Release/OGoCreator
else:win32:CONFIG(debug, debug|release):   OBJECTS_DIR=../../../obj/Debug/OGoCreator
else:symbian:                              OBJECTS_DIR=../../../obj/Release/OGoCreator
else:unix:CONFIG(debug, debug|release):    OBJECTS_DIR=../../../obj/Debug/OGoCreator
else:unix:CONFIG(release, debug|release):  OBJECTS_DIR=../../../obj/Release/OGoCreator

TEMPLATE    = lib

HEADERS     = ogographviewplugin.h
SOURCES     = ogographviewplugin.cpp
#RESOURCES  = icons.qrc
#LIBS        += -L. 

target.path = $$[QT_INSTALL_PLUGINS]/designer
INSTALLS    += target

include(ogographview.pri)
