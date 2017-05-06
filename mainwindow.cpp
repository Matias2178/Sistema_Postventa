/****************************************************************************
**
** Copyright (C) 2012 Denis Shienkov <denis.shienkov@gmail.com>
** Copyright (C) 2012 Laszlo Papp <lpapp@kde.org>
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtSerialPort module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settingsdialog.h"
#include  <selecciondeeditores.h>

#include <QMessageBox>
#include <QtSerialPort/QSerialPort>
#include <QLabel>
#include <QDateTime>
#include <QDebug>
#include <QQueue>
#include <QTimer>
#include <QFile>
#include <QtWidgets>
#include <QString>
#include <ingreso.h>
#include <trabajo.h>
#include <reparaciones.h>
//! [0]

//Variables de uso General


QDateTime FControl;
//


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    memoria = 17;

//! [0]
    ui->setupUi(this);
    FechaActual = QDate::currentDate();
    Mascaras();

    dbAbrirCrear();

    ProductosCrear();
    FallasCrear();
    AgentesCrear();
    OperarioCrear();
    ReparacionesCrear();
    MonitoresCrear();
    PerifericosCrear();
    CaudalimetroCrear();
    InstalacionesCrear();
    IngresoCrear();
    ProductosLeer();

//! [1]
    serial = new QSerialPort(this);

//! [1]
    settings = new SettingsDialog;
    SelEditores = new selecciondeeditores;

    ui->LectLIN->setHidden(true);


    QTimer *Tiempo = new QTimer(this);

    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
    ui->actionQuit->setEnabled(true);
    ui->actionConfigure->setEnabled(true);

    initActionsConnections();

    connect(serial, SIGNAL(error(QSerialPort::SerialPortError)), this,
            SLOT(handleError(QSerialPort::SerialPortError)));

//! [2]
    connect(serial, SIGNAL(readyRead()), this, SLOT(LIN_Lectura()));
//! [2]
    connect(ui->LectLIN,SIGNAL(getData(QByteArray)), this, SLOT(writeData(QByteArray)));
//! [3]
    connect(Tiempo, SIGNAL(timeout()), this, SLOT(LIN_Envio()));
    Tiempo->start(50);
}
//! [3]

MainWindow::~MainWindow()
{
    delete settings;
    delete SelEditores;
    delete ui;
}

//! [4]
void MainWindow::openSerialPort()
{
    SettingsDialog::Settings p = settings->settings();
    serial->setPortName(p.name);
    serial->setBaudRate(p.baudRate);
    serial->setDataBits(p.dataBits);
    serial->setParity(p.parity);
    serial->setStopBits(p.stopBits);
    serial->setFlowControl(p.flowControl);
    if (serial->open(QIODevice::ReadWrite)) {
            ui->actionConnect->setEnabled(false);
            ui->actionDisconnect->setEnabled(true);
            ui->actionConfigure->setEnabled(false);
            ui->statusBar->showMessage(tr("Connected to %1 : %2, %3, %4, %5, %6")
                                       .arg(p.name).arg(p.stringBaudRate).arg(p.stringDataBits)
                                       .arg(p.stringParity).arg(p.stringStopBits).arg(p.stringFlowControl));
    } else {
        QMessageBox::critical(this, tr("Error"), serial->errorString());

        ui->statusBar->showMessage(tr("Open error"));
    }
      EIndice = 1;
      LIndice = 1;
      Escribir = true;
//      NSerie = 0;
//      Siguiente = false;
}
//! [4]

//! [5]
void MainWindow::closeSerialPort()
{
    serial->close();
    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
    ui->actionConfigure->setEnabled(true);
    ui->statusBar->showMessage(tr("Disconnected"));
}
//! [5]

void MainWindow::about()
{
    QMessageBox::about(this, tr("Control de Equipos"),
                       tr("<b>Version: 1.00 </b> en desarrollo Este es un programa desarrollado"
                          "para el departamento de PostVenta de SIID, para la generacion de reportes"
                          "de los equipos que ingresan para se reparados"));
}


//! [6]
void MainWindow::writeData(const QByteArray &data)
{
    serial->write(data);
}
//! [6]

//! [7]

//! [7]

//! [8]
void MainWindow::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        QMessageBox::critical(this, tr("Critical Error"), serial->errorString());
        closeSerialPort();
    }
}
//! [8]
void MainWindow::initActionsConnections()
{
    connect(ui->actionConnect, SIGNAL(triggered()), this, SLOT(openSerialPort()));
    connect(ui->actionDisconnect, SIGNAL(triggered()), this, SLOT(closeSerialPort()));
    connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->actionConfigure, SIGNAL(triggered()), settings, SLOT(show()));
    connect(ui->actiondbEdit, SIGNAL(triggered()), SelEditores, SLOT(show()));
//    connect(ui->actionClear, SIGNAL(triggered()), textEdit, SLOT(clear()));
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(about()));
    connect(ui->actionAboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    connect(ui->actionVersion, SIGNAL(triggered()),this, SLOT(Version()));
    Escribir = true;
}

void MainWindow::Version()
{
    QMessageBox::information(this, tr("Version programa"),
                       tr("<b>Version: 1.00 </b><c> - en desarrollo - Marzo 20017 -</c>"
                          "Programa desarrollado para el uso exclusivo el departamento de PostVenta de SIID"
                          "Software de generación de reportes de los equipos que ingresan para su analisis"
                          "<b><c>Uso exclusivo del departamento de Post Venta</b></c>"
                          "- Ing. Matias Martelossi -"));
}

void MainWindow::on_actionGuardar_triggered()
{
    QString NArchivo;
    NArchivo.clear();
//##Modificar esto    NArchivo.append(ui->Agente->text());
    if(NArchivo.isEmpty())
    {
        QMessageBox::information(this,tr("Nombre Agente"),tr("Cargar el nombre del agente para generar"
                                                             " el nombre del archivo"));
        return;
    }
    NArchivo.clear();
//##Modificar esto    NArchivo.append("/home/Informes/" + ui->Agente->text() + FechaActual.toString("yyyyMMdd"));

    QString fileName = QFileDialog::getSaveFileName(
                this,
                "Analisis de equipo - Guardado de archivos",
                NArchivo,
                "Text Files (*.csv);;All Files (*.csv)");
    if(!fileName.isEmpty()){
        //curFile = fileName;
        saveFile(fileName);
    }

}

bool MainWindow::saveFile(const QString &fileName)
{
    QString info;
    QFile file(fileName);
    if(file.open(QFile::WriteOnly)){
//##        file.write(ui->Datos->toPlainText().toLatin1());
        info = DatosArchivo.join(0x0d);
        file.write(info.toLatin1());
        setCurrentFile(fileName);
        setWindowTitle(tr("Analisis Equipos - %1[*]").arg(QFileInfo(curFile).fileName()));
//###        ui->Datos->document()->setModified(false);
        return true;
    }else{
        QMessageBox::warning(
                    this,
                    "TextEditor",
                    tr("Cannot write file %1.\nError: %2")
                    .arg(curFile)
                    .arg(file.errorString()));
        return false;
    }
}

void MainWindow::setCurrentFile(const QString &fileName)
{
    curFile = fileName;

    setWindowTitle(tr("Text Editor - %1[*]").arg(QFileInfo(curFile).fileName()));

    QSettings settings;
    QStringList recentFilesList = settings.value("recentFilesList").toStringList();
    recentFilesList.removeAll(fileName);
    recentFilesList.prepend(fileName);
//    while(recentFilesList.size() > MaxRecenFiles)
//        recentFilesList.removeLast();

    settings.setValue("recentFilesList",recentFilesList);
//    updateRecentFileActions();
}

void MainWindow::on_actionClear_triggered()
{
     ConfInicio = false;

 //Borrado datos Monitores
//    BorraMonitores();


//Borrado de la planilla de Perifericos
//    BorraPerifericos();

    //FSensores
//    BorraSensores();

    //FRPM-TRB
//    BorraRMP();

    //Borra Moduladora
//    BorraMOD();
   //Borra datos GPS
//    BorraGPS();
   //Borra datos de Instalaciones
//    BorraINS();

}


//void MainWindow::on_Siguiente_clicked()
//{
//    Siguiente = false;
//    Guardar = true;
//    NSerie = 0;
//    ui->Siguiente->setEnabled(true);
//}


void MainWindow::on_actionActualizar_triggered()
{
    ProductosLeer();
}

void MainWindow::on_PantallaIngreso_clicked()
{
    Ingreso *IngresoVentana = new Ingreso(this);
    IngresoVentana->setModal(true);
    IngresoVentana->show();
}

void MainWindow::on_PantallaTrabajos_clicked()
{
    trabajo *TrabajoVentana = new trabajo(this);
    TrabajoVentana->setModal(true);
    TrabajoVentana->show();
}

void MainWindow::on_PantallaReparaciones_clicked()
{
    ReparacionesVentana = new Reparaciones(this);
    ReparacionesVentana->setModal(true);
    ReparacionesVentana->show();
}
