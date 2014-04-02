#-------------------------------------------------
#
# Project created by QtCreator 2012-07-26T14:22:40
#
#-------------------------------------------------

QT       += widgets core gui

     win32:CONFIG(release, debug|release): TARGET = ../../../bin/Release/OGoCreator
else:win32:CONFIG(debug, debug|release):   TARGET = ../../../bin/Debug/OGoCreator
else:symbian:                              TARGET = ../../../bin/Release/OGoCreator
else:unix:CONFIG(debug, debug|release):    TARGET = ../../../bin/Debug/OGoCreator
else:unix:CONFIG(release, debug|release):  TARGET = ../../../bin/Release/OGoCreator


     win32:CONFIG(release, debug|release): OBJECTS_DIR=../../../obj/Release/OGoCreator
else:win32:CONFIG(debug, debug|release):   OBJECTS_DIR=../../../obj/Debug/OGoCreator
else:symbian:                              OBJECTS_DIR=../../../obj/Release/OGoCreator
else:unix:CONFIG(debug, debug|release):    OBJECTS_DIR=../../../obj/Debug/OGoCreator
else:unix:CONFIG(release, debug|release):  OBJECTS_DIR=../../../obj/Release/OGoCreator




TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
        EditAction.cpp

HEADERS  += mainwindow.h \
         EditAction.h

FORMS    += mainwindow.ui

     win32:CONFIG(release, debug|release): LIBS += -logographviewplugin -lopengt -L$$PWD/../../../lib/Release
else:win32:CONFIG(debug, debug|release):   LIBS += -logographviewplugindbg -lopengtdbg -L$$PWD/../../../lib/Debug
else:symbian:                              LIBS += -logographviewplugin -lopengt -L$$PWD/../../../lib/Release
else:unix:CONFIG(debug, debug|release):    LIBS += -logographviewplugindbg -lopengtdbg -L$$PWD/../../../lib/Debug
else:unix:CONFIG(release, debug|release):  LIBS += -logographviewplugin -lopengt -L$$PWD/../../../lib/Release

INCLUDEPATH += $$PWD/../OGoWidget/OGoWidget $$PWD/../../../opengt.so/Headers
