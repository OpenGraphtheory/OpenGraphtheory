#-------------------------------------------------
#
# Project created by QtCreator 2012-07-26T14:22:40
#
#-------------------------------------------------

QT       += core gui

TARGET = OGoCreator
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    EditAction.cpp

HEADERS  += mainwindow.h \
    EditAction.h

FORMS    += mainwindow.ui

unix|win32:CONFIG(debug, debug|release): LIBS += -lopengtdbg
else:unix|win32:CONFIG(release, debug|release): LIBS += -lopengt

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../OGoWidget/OGoWidget-build-desktop-Qt_aus_PATH_Release/release/ -logographviewplugin
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../OGoWidget/OGoWidget-build-desktop-Qt_aus_PATH_Release/debug/ -logographviewplugin
else:symbian: LIBS += -logographviewplugin
else:unix: LIBS += -L$$PWD/../OGoWidget/OGoWidget-build-desktop-Qt_aus_PATH_Release/ -logographviewplugin

INCLUDEPATH += $$PWD/../OGoWidget/OGoWidget-build-desktop-Qt_aus_PATH_Release
DEPENDPATH += $$PWD/../OGoWidget/OGoWidget-build-desktop-Qt_aus_PATH_Release
