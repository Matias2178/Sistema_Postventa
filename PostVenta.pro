greaterThan(QT_MAJOR_VERSION, 4) {
    QT       += widgets serialport sql
} else {
    include($$QTSERIALPORT_PROJECT_ROOT/src/serialport/qt4support/serialport.prf)
}

TARGET = PostVenta
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    settingsdialog.cpp \
    console.cpp \
    archivos.cpp \
    utilidades.cpp \
    lin.cpp \
    manejobd.cpp \
    selecciondeeditores.cpp \
    dbfallaseditar.cpp \
    dbproductoseditar.cpp \
    dbagente.cpp

HEADERS += \
    mainwindow.h \
    settingsdialog.h \
    console.h \
    archivos.h \
    utilidades.h \
    selecciondeeditores.h \
    dbfallaseditar.h \
    dbproductoseditar.h \
    dbagente.h

FORMS += \
    mainwindow.ui \
    settingsdialog.ui \
    selecciondeeditores.ui \
    dbfallaseditar.ui \
    dbproductoseditar.ui \
    dbagente.ui

RESOURCES += \
    app_icon.qrc \
    PostVenta.qrc

DISTFILES += \
    ../../../Downloads/Iconos/Actions-go-previous-icon.png


