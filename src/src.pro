QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app

SOURCES += main.cpp\
    mainwindow.cpp \
    languagewidget.cpp \
    aboutdialog.cpp \
    jsxparser.cpp

HEADERS  += mainwindow.h \
    languagewidget.h \
    aboutdialog.h \
    jsxparser.h

FORMS    += mainwindow.ui \
    languagewidget.ui \
    aboutdialog.ui

RESOURCES += resources.qrc

RC_ICONS = ../resources/icons/dutr_app.ico
