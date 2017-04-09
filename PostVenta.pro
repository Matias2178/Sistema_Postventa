greaterThan(QT_MAJOR_VERSION, 4) {
    QT       += widgets serialport
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
    lin.cpp

HEADERS += \
    mainwindow.h \
    settingsdialog.h \
    console.h \
    archivos.h \
    utilidades.h

FORMS += \
    mainwindow.ui \
    settingsdialog.ui

RESOURCES += \
    terminal.qrc \
    app_icon.qrc

DISTFILES += \
    ../../../Downloads/Iconos/Actions-go-previous-icon.png


