QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    customtextedit.cpp \
    highlighter.cpp \
    main.cpp \
    maincontroller.cpp \
    mainwindow.cpp \
    qpastebin.cpp \
    tabwidget.cpp

HEADERS += \
    customtextedit.h \
    highlighter.h \
    maincontroller.h \
    mainwindow.h \
    qpastebin.h \
    tabwidget.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc

RC_ICONS = resources/notepad.ico
