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
#include "console.h"
#include "settingsdialog.h"

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
//! [0]

//Variables de uso General


QDateTime FControl;
//


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    Item = 1;

//! [0]
    ui->setupUi(this);
    ui->SEN_ITEM->setText(QString::number(Item,10));
    FechaActual = QDate::currentDate();
    Mascaras();
    CambioPantalla(1);
//! [1]
    serial = new QSerialPort(this);

//! [1]
    settings = new SettingsDialog;

    ui->Fecha_Control->setText(FechaActual.toString("dd/MM/yyyy"));

    ui->LectLIN->setHidden(true);
    ui->tabWidget->setCurrentIndex(0);

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
      NSerie = 0;
      Siguiente = false;
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
    NArchivo.append(ui->Agente->text());
    if(NArchivo.isEmpty())
    {
        QMessageBox::information(this,tr("Nombre Agente"),tr("Cargar el nombre del agente para generar"
                                                             " el nombre del archivo"));
        return;
    }
    NArchivo.clear();
    NArchivo.append("/home/Informes/" + ui->Agente->text() + FechaActual.toString("yyyyMMdd"));

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
    QFile file(fileName);
    if(file.open(QFile::WriteOnly)){
        file.write(ui->Datos->toPlainText().toLatin1());
        setCurrentFile(fileName);
        setWindowTitle(tr("Analisis Equipos - %1[*]").arg(QFileInfo(curFile).fileName()));
        ui->Datos->document()->setModified(false);
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

    //Borrado de datos del Trabajo
    BorraIngreso();

 //Borrado datos Monitores
    BorraMonitores();


//Borrado de la planilla de Perifericos
    BorraPerifericos();

    //FSensores
    BorraSensores();

    //FRPM-TRB
    BorraRMP();

    //Borra Moduladora
    BorraMOD();
   //Borra datos GPS
    BorraGPS();
   //Borra datos de Instalaciones
    BorraINS();

//Cargo la fecha actual al campo de prueba
    ui->Fecha_Control->setText(FechaActual.toString("dd/MM/yyyy"));
    qDebug()<< FechaActual.toString("dd/MM/yyyy");
}

void MainWindow::on_Ingreso_Guardar_clicked()
{
    QString Ingreso;
    Ingreso.clear();
    Ingreso.append(ui->Agente->text() + ui->Operario->text() +ui->Fecha_Ingreso->text() + ui->Fecha_Control->text());
    if(Ingreso.isEmpty())
    {
       QMessageBox::information(this,tr("Cargar Datos"),tr("Faltan completar Campos"));
        return;
    }

    Ingreso.clear();
    if(!ConfInicio)
    {
        ui->Datos->setText("Informe de Reparaciones");
        ui->Datos->append("Agente:;;" + ui->Agente->text() + ";;;Fecha Ingreso :;" + ui->Fecha_Ingreso->text());
        ui->Datos->append("Operario:;;" + ui->Operario->text() + ";;;Fecha Control:;" + ui->Fecha_Control->text());
        ConfInicio = true;
        Columnas = true;
        EscColumnas = false;
    }

    if(!ui->Ingreso_Cantidad->value())
    {
        QMessageBox::critical(this, tr("Ingreso Cantidad"),
                              tr("<b>Presta Atencion:</b> Ingresar Cantidad de equipos"));
    }
    else
    {
        Ingreso.append(ui->Ingreso_Equipo->currentText());
        Ingreso.append(";;");
        Ingreso.append(ui->Ingreso_Cantidad->text());
        Ingreso.append(";");
        Ingreso.append(ui->Ingreso_Comentario->toPlainText());
        ui->Datos->append( Ingreso );
        ui->Ingreso_Comentario->clear();
        ui->Ingreso_Cantidad->clear();
        ui->Ingreso_Cantidad->setValue(0);
    }
}



void MainWindow::on_actionGateWay_triggered()
{

    QString NArchivo;

    NArchivo.clear();
    NArchivo.append(ui->Agente->text());
    NArchivo.append(FechaActual.toString("yyyyMMdd"));
    ui->Fecha_Control->setText(FechaActual.toString("dd/MM/yyyy"));

    qDebug() << "--FECHA DE FABRICACION--";
    qDebug() << FControl.currentDateTime();
    qDebug() << FechaActual.day();
    qDebug() << FechaActual.month();
    qDebug() << FechaActual.year();
    qDebug() << NArchivo;

  //  ui->Fecha_Control->setText(FechaActual.day() + "/" + FechaActual.month() + "/" + FechaActual.year());

}
//------------------------------------------------------------------
//Botones de Borrado y guardado de las diferentes pestañas
//------------------------------------------------------------------
void MainWindow::on_BT_B4500_clicked()
{
    BorraMonitores();
}

void MainWindow::on_BT_G4500_clicked()
{
    QString Info;
    bool sig;
    sig = false;

    if(!Columnas)
    {
        EncabezadoMsg();
        return;
    }
    else if (!EscColumnas)
        TituloColumnas();

    if(!ui->MON_TIPO->currentIndex())
    {
        QMessageBox::information(this,tr("Seleccion Tipo Monitor"),
                                 tr("Seleccionar tipo de Monitor"));
        return;
    }
    if (!ui->MON_BON->currentIndex())
    {
        BonificacionMsg();
        return;
    }

    Info.append(ui->SEN_ITEM->text() + ";" + ui->MON_TIPO->currentText() + ";" + ui->MON_NSerie->text() + ";" +
                "--/--/--;--/--/--;" + ui->MON_VSOFT->text() + ";--/--/--;;");
    if(ui->MON_PB->isChecked())
    {
        Info.append("4R7 + Prot Bus");
        sig = true;
    }
    if(ui->MON_ACT->isChecked())
    {
        if(sig)
            Info.append("-");
        Info.append("Act Soft");
        sig = true;
    }
    if(ui->MON_MIC->isChecked())
    {
        if(sig)
            Info.append("-");
        Info.append("Microtec");
        sig = true;
    }
    if(ui->MON_CC->isChecked())
    {
        if(sig)
            Info.append("-");
        Info.append("Carcaza");
        sig = true;
    }
    if(ui->MON_LCD->isChecked())
    {
        if(sig)
            Info.append("-");
        Info.append("LCD");
        sig = true;
    }
    if(ui->MON_FE->isChecked())
    {
        if(sig)
            Info.append("-");
        Info.append("FCE");
    }

    Info.append(";" + ui->MON_BON->currentText());

    Info.append(";" + ui->MON_COM->toPlainText());
    ui->Datos->append(Info);
    Item ++;
    ui->SEN_ITEM->setText(QString::number(Item,10));
    ui->MON_BON->setCurrentIndex(0);
}

void MainWindow::on_S_Guardar_clicked()
{
    QString Sensor;
    bool sig;
    if(!Columnas)
    {
        EncabezadoMsg();
        return;
    }
    else if (!EscColumnas)
        TituloColumnas();;

    if(!ui->S_TIPO->currentIndex())
    {
        QMessageBox::critical(this, tr("Seleccion de Sensor"),
                              tr("Seleccionar Tipo de sensor antes de guardar"));
    }
    else if (!ui->S_BON->currentIndex())
    {
        BonificacionMsg();
    }
    else
    {
        Sensor.append(ui->SEN_ITEM->text() + ";" + ui->S_TIPO->currentText() + ";" + ui->SEN_NSERIE->text()
                      + ";" + ui->SEN_FF->text()+ ";" + ui->SEN_FI->text() + ";" + ui->SEN_VS->text() +
                      ";" + ui->SEN_FS->text() + ";TD:" + ui->S_TD->text() + " - TM:" + ui->S_TM->text()
                      + " - SA:" + ui->S_CA->text() + ";");
        Item ++;
        ui->SEN_ITEM->setText(QString::number(Item,10));
     //--------------------------------------------------------------------------------
     //     Control de Fallas
     //--------------------------------------------------------------------------------
        if(ui->S_CAD->isChecked())
        {
            Sensor.append("CAD");
            sig = true;
        }
        if(ui->S_PES->isChecked())
        {
            if(sig)
                Sensor.append("-");
            Sensor.append("PES");
            sig = true;
        }
        if(ui->S_PRS->isChecked())
        {
            if(sig)
                Sensor.append("-");
            Sensor.append("PRF");
            sig = true;
        }
        if(ui->S_CCD->isChecked())
        {
            if(sig)
                Sensor.append("-");
            Sensor.append("CCD");
            sig = true;
        }
        sig = false;
        Sensor.append(";" + ui->S_BON->currentText());

        QString Testo = ui->S_COM->toPlainText();
        if(ui->S_FOK->isChecked())
            Testo.append(" Func. Ok");
         ui->Datos->append( Sensor + ";" + Testo);
    }
    Guardar = true;
    Siguiente = false;

}

void MainWindow::on_S_Borrar_Item_clicked()
{
    Item = 1;
    ui->SEN_ITEM->setText(QString::number(Item,10));
}

void MainWindow::on_S_ANT_clicked()
{
    PantallaActual -- ;
    if((PantallaActual > 5) || (!PantallaActual))
        PantallaActual = 5;
    CambioPantalla(PantallaActual);
    ui->SEN_FS->setInputMask("00/00/00");
}

void MainWindow::on_S_SIG_clicked()
{
    PantallaActual ++ ;
    if(PantallaActual > 5)
        PantallaActual = 1;
    CambioPantalla(PantallaActual);
    ui->SEN_FS->setInputMask("00/00/00");
}

void MainWindow::on_RPM_Guardar_clicked()
{
    QString Sensor;
    bool sig;
    sig = false;
    if(!Columnas)
    {
        EncabezadoMsg();
        return;
    }
    else if (!EscColumnas)
        TituloColumnas();
    if (!ui->RPM_BON->currentIndex())
    {
        BonificacionMsg();
        return;
    }

    if (RPM_TRB)
    {
        Sensor.append(ui->SEN_ITEM->text() + ";Sen RPM;");
    }
    else
    {
        Sensor.append(ui->SEN_ITEM->text() + ";Sen Turbina;");
    }

    Sensor.append(ui->SEN_NSERIE->text() + ";" + ui->SEN_FF->text()+ ";" + ui->SEN_FI->text() + ";"
                  + ui->SEN_VS->text() + ";" + ui->SEN_FS->text() + ";FK:" + ui->RPM_FK->text() + "Pls/Rev;");
     Item ++;
     ui->SEN_ITEM->setText(QString::number(Item,10));
     //--------------------------------------------------------------------------------
     //     Control de Fallas
     //--------------------------------------------------------------------------------
     if(ui->RPM_CAD->isChecked())
     {
         Sensor.append("CAD");
         sig = true;
     }
     if(ui->RPM_CMD->isChecked())
     {
         if(sig)
             Sensor.append("-");
         Sensor.append("CMD");
         sig = true;
     }
     if(ui->RPM_PED->isChecked())
     {
         if(sig)
             Sensor.append("-");
         Sensor.append("PED");
         sig = true;
     }
     if(ui->RPM_PES->isChecked())
     {
         if(sig)
             Sensor.append("-");

         Sensor.append("PES");
         sig = true;
     }
     sig = false;
     Sensor.append(";" + ui->RPM_BON->currentText());

     QString Testo = ui->RPM_COM->toPlainText();
     ui->Datos->append( Sensor + ";" + Testo);
     Guardar = true;
     Siguiente = false;
}


void MainWindow::on_MOD_Guardar_clicked()
{
    QString Sensor;
    bool sig;
    sig = false;
    if(!Columnas)
    {
        EncabezadoMsg();
        return;
    }
    else if (!EscColumnas)
        TituloColumnas();
    if (!ui->MOD_BON->currentIndex())
    {
        BonificacionMsg();
        return;
    }


    Sensor.append(ui->SEN_ITEM->text() + ";""MOD;");

    Sensor.append(ui->SEN_NSERIE->text() + ";" + ui->SEN_FF->text()+ ";" + ui->SEN_FI->text() + ";"
                  + ui->SEN_VS->text() + ";" + ui->SEN_FS->text() + ";DT:" + ui->MOD_DT->text()
                   + " DD:" + ui->MOD_DD->text()  + " RT:" + ui->MOD_RT->text() + ";" );
     Item ++;
     ui->SEN_ITEM->setText(QString::number(Item,10));
     //--------------------------------------------------------------------------------
     //     Control de Fallas
     //--------------------------------------------------------------------------------
     if(ui->MOD_CAD->isChecked())
     {
         Sensor.append("CAD");
         sig = true;
     }
     if(ui->MOD_AX2->isChecked())
     {
         if(sig)
             Sensor.append("-");
         Sensor.append("CMD");
         sig = true;
     }
     if(ui->MOD_AX1->isChecked())
     {
         if(sig)
             Sensor.append("-");
         Sensor.append("PED");
         sig = true;
     }
     if(ui->MOD_PES->isChecked())
     {
         if(sig)
             Sensor.append("-");

         Sensor.append("PES");
         sig = true;
     }
     sig = false;
     Sensor.append(";" + ui->MOD_BON->currentText());

     QString Testo = ui->MOD_COM->toPlainText();
     ui->Datos->append( Sensor + ";" + Testo);
     Guardar = true;
     Siguiente = false;
}

void MainWindow::on_GPS_Guardar_clicked()
{
    QString Sensor;
    bool sig;
    sig = false;
    if(!Columnas)
    {
        EncabezadoMsg();
        return;
    }
    else if (!EscColumnas)
        TituloColumnas();
    if (!ui->GPS_BON->currentIndex())
    {
        BonificacionMsg();
        return;
    }


    Sensor.append(ui->SEN_ITEM->text() + ";""GPS;");

    Sensor.append(ui->SEN_NSERIE->text() + ";" + ui->SEN_FF->text()+ ";" + ui->SEN_FI->text() + ";"
                  + ui->SEN_VS->text() + ";" + ui->SEN_FS->text() + ";");
     Item ++;
     ui->SEN_ITEM->setText(QString::number(Item,10));
     //--------------------------------------------------------------------------------
     //     Control de Fallas
     //--------------------------------------------------------------------------------
     if(ui->GPS_CAD->isChecked())
     {
         Sensor.append("CAD");
         sig = true;
     }
     if(ui->GPS_PES->isChecked())
     {
         if(sig)
             Sensor.append("-");
         Sensor.append("PES");
         sig = true;
     }
     if(ui->GPS_MGA->isChecked())
     {
         if(sig)
             Sensor.append("-");
         Sensor.append("MGA");
         sig = true;
     }
     if(ui->GPS_ATD->isChecked())
     {
         if(sig)
             Sensor.append("-");

         Sensor.append("ATD");
         sig = true;
     }
     sig = false;
     Sensor.append(";" + ui->GPS_BON->currentText());

     QString Testo = ui->GPS_COM->toPlainText();
     ui->Datos->append( Sensor + ";" + Testo);
     Guardar = true;
     Siguiente = false;
}

void MainWindow::on_GPS_Borrar_clicked()
{
    BorraGPS();
}

void MainWindow::on_MOD_Borrar_clicked()
{
    BorraMOD();
}

void MainWindow::on_RPM_Borrar_clicked()
{
    BorraRMP();
}

void MainWindow::on_S_Borrar_clicked()
{
    BorraSensores();
}

void MainWindow::on_INS_BGuardar_clicked()
{
    QString Info;
    bool sig;
    sig = false;
    if(!Columnas)
    {
        EncabezadoMsg();
        return;
    }
    else if (!EscColumnas)
        TituloColumnas();
    if(!ui->INS_TIPO->currentIndex())
    {
        QMessageBox::information(this,tr("Seleccion Instalación"),
                                 tr("Seleccionar tipo de Instalación"));
        return;
    }
    if (!ui->INS_BON->currentIndex())
    {
        BonificacionMsg();
        return;
    }


    Info.append(ui->SEN_ITEM->text() + ";" + ui->INS_TIPO->currentText() + ";" + ui->INS_NSerie->text() + ";;;;;;");

    if(ui->INS_S01->isChecked())
    {
        if(sig)
            Info.append("-");
        Info.append("Act Soft");
        sig = true;
    }
    if(ui->INS_S02->isChecked())
    {
        if(sig)
            Info.append("-");
        Info.append("Microtec");
        sig = true;
    }
    if(ui->INS_S03->isChecked())
    {
        if(sig)
            Info.append("-");
        Info.append("Carcaza");
        sig = true;
    }
    if(ui->INS_S04->isChecked())
    {
        if(sig)
            Info.append("-");
        Info.append("LCD");
        sig = true;
    }
    if(ui->INS_S05->isChecked())
    {
        if(sig)
            Info.append("-");
        Info.append("FCE");
    }
    Info.append(";" + ui->INS_BON->currentText());
    Info.append(";" + ui->INS_COM->toPlainText());
    ui->Datos->append(Info);
    Item ++;
    ui->SEN_ITEM->setText(QString::number(Item,10));

}

void MainWindow::on_INS_BBorrar_clicked()
{
    BorraINS();
}



void MainWindow::on_CAU_Guardar_clicked()
{
    if (!ui->CAU_BON->currentIndex())
    {
        BonificacionMsg();
        return;
    }
    Guardar = true;
    Siguiente = false;
}

void MainWindow::on_MON_TIPO_activated(int index)
{
    switch (index)
    {

    case 1:
        // CAS 4500
        ui->MON_VSOFT->setInputMask("0.00R00");
        break;
    case 2:
        //CAS 2500
        ui->MON_VSOFT->setInputMask("0.00");
        break;
     case 3:
        // CAS 1000
        ui->MON_VSOFT->setInputMask("0.00");
        break;
     case 4:
        // CAS 1500
     case 5:
        // CAS 2700
        ui->MON_VSOFT->setInputMask("000rA");
        break;
    }
}

void MainWindow::on_Siguiente_clicked()
{
    Siguiente = false;
    Guardar = true;
    ui->Siguiente->setEnabled(true);
}
