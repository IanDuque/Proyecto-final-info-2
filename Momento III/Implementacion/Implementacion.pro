QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    bloques.cpp \
    carro.cpp \
    egipcio.cpp \
    espaniol.cpp \
    explosion.cpp \
    indio.cpp \
    main.cpp \
    mainwindow.cpp \
    menu.cpp \
    movimientos.cpp \
    nivel1.cpp \
    nivel2.cpp \
    nivel3.cpp \
    nivelbase.cpp \
    obstaculo.cpp \
    personaje.cpp \
    proyectil.cpp \
    recolectables.cpp

HEADERS += \
    bloques.h \
    carro.h \
    egipcio.h \
    espaniol.h \
    explosion.h \
    indio.h \
    mainwindow.h \
    menu.h \
    movimientos.h \
    nivel1.h \
    nivel2.h \
    nivel3.h \
    nivelbase.h \
    obstaculo.h \
    personaje.h \
    proyectil.h \
    recolectables.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    sprites.qrc
