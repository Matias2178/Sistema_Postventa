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
#include <variables.h>
#include <busqueda.h>
#include <gruposyfallas.h>
#include <fallasproductos.h>
#include <QtPrintSupport/QPrinter>
#include <QPainter>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <archivos.h>

//! [0]

//Variables de uso General
QDateTime FechaHora;
dbManejo dbPostVenta;
dbRutasArchivo dbInfo;

//QDateTime FControl;
//


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

//! [0]
    ui->setupUi(this);

    FechaActual = QDate::currentDate();

    if(dbInfo.Abrir())
    {
        Archivos *ArchivoVentana = new Archivos(this);
        ArchivoVentana->setModal(true);
        ArchivoVentana->show();
    }

    RutaBaseDatos.clear();
    RutaInfoPDF.clear();
    RutaInfoExcel.clear();

    RutaBaseDatos.append(dbInfo.Buscar("dbReparaciones"));
    RutaInfoPDF.append(dbInfo.Buscar("InformePDF"));
    RutaInfoExcel.append(dbInfo.Buscar("InformeRep"));

//    qDebug() << "Bases de Datos: "<< RutaBaseDatos;
//    qDebug() << "Informe PDF: " << RutaInfoPDF;
//    qDebug() << "Informe Excel:" << RutaInfoExcel;

    dbPostVenta.SetDirDb(RutaBaseDatos);
    if(!dbPostVenta.dbAbrirCrear())
    {
        QString fileName = QFileDialog::getOpenFileName(
                    this,
                    "Analisis de equipo - Abrir archivos",
                    "",
                    "Text Files (*.sqlite);;All Files (*.sqlite)");

        int aa = fileName.lastIndexOf("/");
        QString direccion = fileName.mid(0,aa);


        qDebug() << fileName;
        dbPostVenta.SetDirDb(fileName);
        dbPostVenta.dbAbrirCrear();
    }


//! [1]
    serial = new QSerialPort(this);
    Reparacion = new Reparaciones(this);

//! [1]
    settings = new SettingsDialog;
    SelEditores = new selecciondeeditores;

    QTimer *Tiempo = new QTimer(this);

    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
    ui->actionQuit->setEnabled(true);
    ui->actionConfigure->setEnabled(true);

    initActionsConnections();

    connect(serial, SIGNAL(error(QSerialPort::SerialPortError)), this,
            SLOT(handleError(QSerialPort::SerialPortError)));

//! [2]
//   connect(serial, SIGNAL(readyRead()), Reparacion, SLOT(LIN_Lectura()));
    connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));
//! [2]
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
                       tr("<b>Version: %1.%2.%3 </b> en desarrollo Este es un programa desarrollado"
                          "para el departamento de PostVenta de SIID, para la generacion de reportes"
                          "de los equipos que ingresan para se reparados").arg(Ver1).arg(Ver2).arg(Ver3));
}


//! [6]
void MainWindow::writeData(const QByteArray &data)
{
    serial->write(data);
}
void MainWindow::readData()
{
    DatosLin.append(serial->readAll());
    Reparacion->LIN_Lectura();
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
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(about()));
    connect(ui->actionAboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    connect(ui->actionVersion, SIGNAL(triggered()),this, SLOT(Version()));
    Escribir = true;
}

void MainWindow::Version()
{
    QMessageBox::information(this, tr("Version programa"),
                       tr("<b>Version: %1.%2.%3 </b><c> - en desarrollo - Marzo 2017 - %3 -</c>"
                          "Programa desarrollado para el uso exclusivo el departamento de PostVenta de SIID"
                          "/n/rSoftware de generación de reportes de los equipos que ingresan para su analisis"
                          "/n/r<b><c>Uso exclusivo del departamento de Post Venta</b></c>"
                          "- Ing. Matias Martelossi -").arg(Ver1).arg(Ver2).arg(Ver3).arg (FVer));
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


void MainWindow::on_actionActualizar_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(
                this,
                "Analisis de equipo - Abrir archivos",
                "",
                "Text Files (*.sqlite);;All Files (*.*)");

    qDebug() << fileName;
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
 //   Reparacion->setModal(true);
    Reparacion->ActualizaDatos();
    Reparacion->show();
}

void MainWindow::on_PantallaBusqueda_clicked()
{
    Busqueda *BusquedaVentana = new Busqueda(this);
    BusquedaVentana->setModal(true);
    BusquedaVentana->show();
}



void MainWindow::on_Pdf_clicked()
{
    QPrinter printer;
    QString NArchivo;
    QString Texto;
    QDir Dir;
    int Linea, i;

    Dir.setCurrent(Dir.homePath());
    #define klinea 18;
    #define kLDatos 15;

    Linea = 10;
    NArchivo.clear();
    NArchivo.append("Hola1234.pdf");

    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(NArchivo);
    printer.setPageOrientation(QPageLayout::Portrait);

    QPainter painter;

    if (!painter.begin(&printer)) { // failed to open file
        qWarning("failed to open file, is it writable?");
        return ;
    }
 //Dibujamos el encabezado.

    QPen pen;
    pen.setColor(Qt::blue);
    pen.setWidth(2);
    pen.setStyle(Qt::SolidLine);

    painter.setPen(pen);

    QPointF points[] = {
        QPointF(50.0, 0),
        QPointF(50.0, 70.0),
        QPointF(130.0, 70.0),
        QPointF(130.0, 0.0),
        QPointF(670.0, 0.0),
        QPointF(670.0, 70.0),
        QPointF(750.0, 70.0),
        QPointF(750.0, 00.0),
        QPointF(50.0, 0.0),
        QPointF(50.0, 70.0),
        QPointF(750.0, 70.0),
        QPointF(750.0, 0.0)
    };
    painter.drawPolygon(points,12);

    painter.setPen(QFont::Bold);
    QFont font("Times", 20);
    font.setBold(true);
    painter.setFont(font);

    Linea = 48;
    painter.drawText( 230,Linea, "Informe de Reparaciones");

    font.setWeight(11);
    font.setPixelSize(12);
    font.setBold(true);
    font.setUnderline(true);
    painter.setFont(font);

    Linea +=45;

    Texto.clear();
    Texto.append("Agente: ");
    painter.drawText(50,Linea,Texto);
    Texto.clear();
    Texto.append("Fecha Ingreso: " );
    painter.drawText(250,Linea,Texto);

    Texto.clear();
    Texto.append("ID:  " );
    painter.drawText(550,Linea,Texto);

    Linea +=klinea;
    Texto.clear();
    Texto.append("Operario: ");
    painter.drawText(50,Linea,Texto);
    Texto.clear();
    Texto.append("Fecha Control: " );
    Texto.append(FechaHora.currentDateTime().toString("hh:mm:ss.z"));
    painter.drawText(550,Linea,Texto);
    Linea +=klinea;


    #define kinicio 135
    double inicio = kinicio;
    double renglon = 50;
    int a = 0;

    for(i=0;i<=1000;i++)
    {

        Linea += kLDatos;
        Linea += 5;

        font.setWeight(11);
        font.setPixelSize(11);
        font.setUnderline(false);
        painter.setFont(font);

        pen.setColor(Qt::black);
        painter.setPen(pen);

        Texto.clear();
        Texto.append("Cod: ");
        painter.drawText(55,Linea,Texto);

        Texto.clear();
        Texto.append( QString::number(i));
        painter.drawText(95,Linea,Texto);

        Texto.clear();
        Texto.append("Desc: ");
        painter.drawText(150,Linea,Texto);
        Texto.clear();
        Texto.append("SN: ");
        painter.drawText(560,Linea,Texto);
        font.setBold(true);
        Texto.clear();
        Texto.append("Bon: ");
        painter.drawText(660,Linea,Texto);
        font.setBold(true);

        Linea += kLDatos;
        Texto.clear();
        Texto.append("Falla: ");
        painter.drawText(55,Linea,Texto);
        font.setBold(true);

        Linea += kLDatos;
        Texto.clear();
        Texto.append("Observaciones: ");
        painter.drawText(55,Linea,Texto);
        font.setBold(true);

        font.setBold(false);
        font.setItalic(true);
        painter.setFont(font);

        pen.setWidth(1);
        painter.setPen(pen);
        inicio = inicio + renglon;
        painter.drawLine(50.0,inicio, 750.0, inicio);

        //Dibujamos el encabezado.
        a++;
       if(a==18)
       {

           a = 0;
           inicio = kinicio;
           if (! printer.newPage()) {
               qWarning("failed in flushing page to disk, disk full?");
               return ;
           }
           pen.setColor(Qt::blue);
           pen.setWidth(2);
           pen.setStyle(Qt::SolidLine);

           painter.setPen(pen);
           painter.drawPolygon(points,12);

           painter.setPen(QFont::Bold);
           QFont font("Times", 20);
           font.setBold(true);
           painter.setFont(font);

           Linea = 48;
           painter.drawText( 230,Linea, "Informe de Reparaciones");

           font.setWeight(11);
           font.setPixelSize(12);
           font.setBold(true);
           font.setUnderline(true);
           painter.setFont(font);

           Linea +=45;

           Texto.clear();
           Texto.append("Agente: ");
           painter.drawText(50,Linea,Texto);
           Texto.clear();
           Texto.append("Fecha Ingreso: " );
           painter.drawText(250,Linea,Texto);

           Texto.clear();
           Texto.append("ID:  " );
           painter.drawText(550,Linea,Texto);

           Linea +=klinea;
           Texto.clear();
           Texto.append("Operario: ");
           painter.drawText(50,Linea,Texto);
           Texto.clear();
           Texto.append("Fecha Control: " );
           Texto.append(FechaHora.currentDateTime().toString("hh:mm:ss.z"));
           painter.drawText(550,Linea,Texto);
           Linea +=klinea;
           font.setBold(false);
           font.setUnderline(false);
           painter.setFont(font);
       }

    }

     painter.end();
}

void MainWindow::on_actionArchivos_triggered()
{
    Archivos *ArchivoVentana = new Archivos(this);
    ArchivoVentana->setModal(true);
    ArchivoVentana->show();
 //   dbPostVenta.SetDirDb("D:/PostVenta.sqlite");
 //   dbPostVenta.dbAbrirCrear();
}
