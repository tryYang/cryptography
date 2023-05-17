QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    AES.cpp \
    Base64.cpp \
    client.cpp \
    client_login.cpp \
    cregister.cpp \
    main.cpp \
    server.cpp \
    server_login.cpp \
    widget.cpp

HEADERS += \
    AES.h \
    Base64.h \
    client.h \
    client_login.h \
    cregister.h \
    picosha2.h \
    server.h \
    server_login.h \
    widget.h

FORMS += \
    client.ui \
    client_login.ui \
    cregister.ui \
    server.ui \
    server_login.ui \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
