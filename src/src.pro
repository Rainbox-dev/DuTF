QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app

SOURCES += main.cpp\
    mainwindow.cpp \
    languagewidget.cpp \
    aboutdialog.cpp \
    jsxparser.cpp \
    searchwidget.cpp

HEADERS  += mainwindow.h \
    languagewidget.h \
    aboutdialog.h \
    jsxparser.h \
    searchwidget.h

FORMS    += mainwindow.ui \
    languagewidget.ui \
    aboutdialog.ui \
    searchwidget.ui

RESOURCES += resources.qrc

#WIN ICON
RC_ICONS = ../resources/icons/dutr_win.ico
#MAC ICON
ICON = ../resources/icons/dutr_mac.icns