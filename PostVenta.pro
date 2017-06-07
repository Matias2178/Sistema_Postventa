greaterThan(QT_MAJOR_VERSION, 4) {
    QT       += widgets serialport sql printsupport gui
} else {
    include($$QTSERIALPORT_PROJECT_ROOT/src/serialport/qt4support/serialport.prf)
}

TARGET = PostVenta
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    settingsdialog.cpp \
    utilidades.cpp \
    lin.cpp \
    manejobd.cpp \
    selecciondeeditores.cpp \
    dbfallaseditar.cpp \
    dbproductoseditar.cpp \
    dbagente.cpp \
    ingreso.cpp \
    trabajo.cpp \
    Reparaciones.cpp \
    user.cpp \
    variables.cpp \
    dbmanejo.cpp

HEADERS += \
    mainwindow.h \
    settingsdialog.h \
    selecciondeeditores.h \
    dbfallaseditar.h \
    dbproductoseditar.h \
    dbagente.h \
    ingreso.h \
    trabajo.h \
    reparaciones.h \
    user.h \
    variables.h \
    dbmanejo.h

FORMS += \
    mainwindow.ui \
    settingsdialog.ui \
    selecciondeeditores.ui \
    dbfallaseditar.ui \
    dbproductoseditar.ui \
    dbagente.ui \
    ingreso.ui \
    trabajo.ui \
    reparaciones.ui

RESOURCES += \
    PostVenta.qrc

DISTFILES += \
    ../../../Downloads/Iconos/Actions-go-previous-icon.png

RC_FILE = app_icon.qrc


