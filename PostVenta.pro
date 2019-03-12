greaterThan(QT_MAJOR_VERSION, 4) {
    QT       += widgets serialport sql printsupport gui core
} else {
    include($$QTSERIALPORT_PROJECT_ROOT/src/serialport/qt4support/serialport.prf)
}

TARGET = PostVenta
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    settingsdialog.cpp \
    lin.cpp \
    selecciondeeditores.cpp \
    dbproductoseditar.cpp \
    dbagente.cpp \
    ingreso.cpp \
    trabajo.cpp \
    Reparaciones.cpp \
    user.cpp \
    variables.cpp \
    dbmanejo.cpp \
    busqueda.cpp \
    consulta.cpp \
    gruposyfallas.cpp \
    fallasproductos.cpp \
    dbrutasarchivo.cpp \
    archivos.cpp \
    ingresoreparaciones.cpp \
    reparacioneseditar.cpp \
    reporteinterno.cpp

HEADERS += \
    mainwindow.h \
    settingsdialog.h \
    selecciondeeditores.h \
    dbproductoseditar.h \
    dbagente.h \
    ingreso.h \
    trabajo.h \
    reparaciones.h \
    user.h \
    variables.h \
    dbmanejo.h \
    busqueda.h \
    consulta.h \
    gruposyfallas.h \
    fallasproductos.h \
    reporte.h \
    dbrutasarchivo.h \
    archivos.h \
    ingresoreparaciones.h \
    reparacioneseditar.h \
    reporteinterno.h

FORMS += \
    mainwindow.ui \
    settingsdialog.ui \
    selecciondeeditores.ui \
    dbproductoseditar.ui \
    dbagente.ui \
    ingreso.ui \
    trabajo.ui \
    reparaciones.ui \
    busqueda.ui \
    consulta.ui \
    gruposyfallas.ui \
    fallasproductos.ui \
    archivos.ui \
    ingresoreparaciones.ui \
    reparacioneseditar.ui

RESOURCES += \
    PostVenta.qrc

DISTFILES += \
    ../../../Downloads/Iconos/Actions-go-previous-icon.png

RC_FILE = app_icon.qrc


