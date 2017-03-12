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
//! [0]

//Variables de uso General
int LIndice;
int EIndice;
int SenID;
int Item;
bool LecturaOk;
bool Escribir;
bool ConfInicio;
bool RPM_TRB;
QByteArray DatosLin;
QByteArray Lectura;
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
//! [1]
    serial = new QSerialPort(this);
//! [1]
    settings = new SettingsDialog;
    ui->Fecha_Control->setText(FechaActual.toString("dd/MM//yyyy"));

    QTimer *Tiempo = new QTimer(this);

    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
    ui->actionQuit->setEnabled(true);
    ui->actionConfigure->setEnabled(true);

    initActionsConnections();

    connect(serial, SIGNAL(error(QSerialPort::SerialPortError)), this,
            SLOT(handleError(QSerialPort::SerialPortError)));

//! [2]
    connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));
//! [2]
//    connect(console, SIGNAL(getData(QByteArray)), this, SLOT(writeData(QByteArray)));
    connect(ui->textEdit,SIGNAL(getData(QByteArray)), this, SLOT(writeData(QByteArray)));
//! [3]
    connect(Tiempo, SIGNAL(timeout()), this, SLOT(bucle1()));
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
void MainWindow::readData()
{
    unsigned long Valor;
    unsigned int DD, MM, AA;
    unsigned int Aux;
    bool ok;
    QString Datos;
    QByteArray CC;
    QDateTime FechaFab;

    DatosLin.append(serial->readAll());
    if(DatosLin.contains(0x0A) )
    {
         Escribir = true;

         ui->textEdit->append(DatosLin);
         if(DatosLin.contains("!"))
         {
           ui->SEN_ID->setStyleSheet("QLineEdit { background-color: red }");
           EIndice = 1;

         }
         else
         {
             ui->SEN_ID->setStyleSheet("QLineEdit { background-color: yellow }");
             Lectura[0] = DatosLin[7];
             Lectura[1] = DatosLin[8];
             Lectura[2] = DatosLin[5];
             Lectura[3] = DatosLin[6];
             Lectura[4] = DatosLin[3];
             Lectura[5] = DatosLin[4];
             Lectura[6] = DatosLin[1];
             Lectura[7] = DatosLin[2];

             switch (LIndice)
             {
               case 1:
              //NUMERO DE SERIE
                 Valor = Lectura.toLong(&ok,16);
                 ui->SEN_NSERIE->setText(QString::number(Valor,10));
                 EIndice ++;
              break;
              case 2:
              //FECHA DE FABRICACION
                 Valor = Lectura.toLong(&ok,16);
                 FechaFab.setTime_t(Valor);
             //    qDebug() << Valor;
                 ui->SEN_FF->setText(FechaFab.toString("dd/MM/yyyy"));
                 EIndice ++;
              break;
              case 3:
              //FECHA DE INSTALACION
                 Valor = Lectura.toLong(&ok,16);
            //     qDebug() << Valor;
                 FechaFab.setTime_t(Valor);
                 if(Valor)
                 {
                     ui->SEN_FI->setText(FechaFab.toString("dd/MM/yyyy"));
                 }
                 else
                 {
                     ui->SEN_FI->setText("--/--/--");
                 }

                 EIndice ++;
              break;
              case 4:
            //VERSION DEL SOFTWARE Y FECHA DE COMPILACION
                 CC.clear();
                 CC[0] = Lectura[6];
                 CC[1] = Lectura[7];
                 Datos.clear();
                 Aux = CC.toInt(&ok,16);
                 Aux = Aux / 100;
                 Datos.append(QString::number(Aux,10));

                 Aux = CC.toInt(&ok,16);
                 Aux = Aux % 100;
                 if(Aux<10)
                     Datos.append(".0");
                 else
                     Datos.append(".");
                 Datos.append(QString::number(Aux,10));

                 ui->SEN_VS->setText(Datos);

                 CC.clear();
                 CC[0] = Lectura[4];
                 CC[1] = Lectura[5];
                 DD = CC.toInt(&ok,16);
                 CC.clear();
                 CC[0] = Lectura[2];
                 CC[1] = Lectura[3];
                 MM = CC.toInt(&ok,16);
                 CC.clear();
                 CC[0] = Lectura[0];
                 CC[1] = Lectura[1];
                 AA = CC.toInt(&ok,16);
                 ui->SEN_FS->setText(QString::number(DD,10)+"/"+QString::number(MM,10)+"/"+QString::number(AA,10));
                 EIndice ++;
              break;
              case 5:
              //LECTURA DE ID
                 Lectura.clear();
                 Lectura[0] = DatosLin[1];
                 Lectura[1] = DatosLin[2];;

                 DatosLin.left(1) ;
                 Valor = Lectura.toShort(&ok,16);
                 SenID = Valor;
                 Valor ++;
                 ui->SEN_ID->setText(QString::number(Valor,10));

           //Sensor de caida
                 qDebug() << "Sensor:" << SenID;
                 if((SenID < 63) || (SenID ==240))
                 {
                     CambioPantalla(1);  //Pantalla Sensores de semilla
                     ui->SEN_TIPO->setText("Caida");
                     EIndice = 10;
                     if(SenID ==240)
                     {
                         ui->S_NOM->setText("Sensor Virgen");
                        // ui->SEN_TIPO->setText("S. Virgen");
                     }
                     else if(SenID < 31)
                     {
                        ui->S_NOM->setText("Sensor Semilla");
                       // ui->SEN_TIPO->setText("S. Semilla");
                     }
                     else if(SenID < 63)
                     {
                        ui->SEN_TIPO->setText("S. Fertilizante");
                      //  ui->S_NOM->setText("Sensor Fertilizante");
                     }
                 }
           //Modulo GPS
                 else if ((SenID == 214))
                 {
                     CambioPantalla(2);  //Pantalla Modulo GPS

                     ui->SEN_TIPO->setText("MOD. GPS");
                     EIndice = 1;
                 }
          //Sensor de Rotacion
                 else if(((SenID >= 0x40) && (SenID <= 0x47))|| (SenID == 0xF1))
                 {
                     CambioPantalla(3);  //Pantalla Sensores Turbina y Rotacion

                     ui->SEN_TIPO->setText("ROTACION");
                     RPM_TRB = true;
                     EIndice = 30;
                 }
            //Sensor de Turbina
                 else if((SenID >= 0xD3) && (SenID <= 0xD5))
                 {
                     CambioPantalla(3);  //Pantalla Sensores Turbina y Rotacion

                     ui->SEN_TIPO->setText("TURBINA");
                     RPM_TRB = false;
                     EIndice = 30;
                 }
              break;
//Lectura de datos de los sensores de semilla
              case 10:
              //MEDICION
                 bool LecSem;
                 Lectura.clear();
                 Lectura[0] = DatosLin[3];
                 Lectura[1] = DatosLin[4];
                 Lectura[2] = DatosLin[1];
                 Lectura[3] = DatosLin[2];

                 Valor = Lectura.toInt(&ok,16);
                 LecSem = Valor & 0x8000;
                 Valor = Valor & 0x0FFF;

                 if (LecSem)
                 {
                     ui->S_MED->setStyleSheet("QLineEdit { background-color: yellow }");
                 }
                 else
                 {
                     ui->S_MED->setStyleSheet("QLineEdit { background-color: green }");
                 }
                 ui->S_MED->setText(QString::number(Valor,10));
                 EIndice ++;
              break;
              case 11:
              // Lectura de tubo sucio
                 Lectura.clear();
                 Lectura[0] = DatosLin[1];
                 Valor = Lectura.toInt(&ok,16);
                 if (Valor)
                 {
                     ui->S_Tubo->setText("Tubo Ok");
                     ui->S_Tubo->setStyleSheet("QLineEdit { background-color: green }");
                 }
                 else
                 {
                     ui->S_Tubo->setText("Tubo Sucio");
                     ui->S_Tubo->setStyleSheet("QLineEdit { background-color: pink }");
                 }
                 EIndice ++;
              break;
              case 12:
                 Lectura.clear();
                 Lectura[0] = DatosLin[1];
                 Valor = Lectura.toInt(&ok,16);
                 ui->S_TM->setText(QString::number(Valor,10));
                 EIndice ++;
              break;
              case 13:
                Lectura.clear();
                Lectura[0] = DatosLin[1];
                Valor = Lectura.toInt(&ok,16);
                ui->S_TD->setText(QString::number(Valor,10));
                EIndice ++;
              break;
              case 14:
                Lectura.clear();
                Lectura[0] = DatosLin[1];
                Valor = Lectura.toInt(&ok,16);
                ui->S_CA->setText(QString::number(Valor,10));
                EIndice = 1;
              break;
 //----------------------------------------------------------------------
 //     Lectura de datos de sensores de Rotacion y de RPM
 //----------------------------------------------------------------------
              case 30:
                 Lectura.clear();
                 Lectura[0] = DatosLin[3];
                 Lectura[1] = DatosLin[4];
                 Lectura[2] = DatosLin[1];
                 Lectura[3] = DatosLin[2];
                 Valor = Lectura.toInt(&ok,16);
                 qDebug() << "Med:" << Valor;
                 qDebug() << Lectura;
                 qDebug() << DatosLin;
                 ui->RPM_MED->setText(QString::number(Valor,10));
                 EIndice = 31;
              break;
              case 31:
                 Lectura.clear();
                 Lectura[0] = DatosLin[3];
                 Lectura[1] = DatosLin[4];
                 Lectura[2] = DatosLin[1];
                 Lectura[3] = DatosLin[2];
                 Valor = Lectura.toInt(&ok,16);
                 Aux = Valor / 100;
                 Datos.clear();
                 Datos.append(QString::number(Aux,10));
                 Aux = Valor % 100;
                 if(Aux<10)
                     Datos.append(".0");
                 else
                     Datos.append(".");
                 Datos.append(QString::number(Aux,10));
                 ui->RPM_FK->setText(Datos);
                 if (RPM_TRB)
                 {
                    ui->RPM_K->setText("Pls * Rev");
                    EIndice = 1;
                 }
                 else
                 {
                    ui->RPM_K->setText("Pls * Rev");
                    EIndice = 32;
                 }
                 qDebug() << EIndice;
              break;
              case 32:
                 Lectura.clear();
                 Lectura[0] = DatosLin[3];
                 Lectura[1] = DatosLin[4];
                 Lectura[2] = DatosLin[1];
                 Lectura[3] = DatosLin[2];
                 Valor = Lectura.toInt(&ok,16);

                 qDebug() << "Max:" << Valor;
                 ui->RPM_ALMAX->setText(QString::number(Valor,10));
                 EIndice = 33;
              break;
              case 33:
                 Lectura.clear();
                 Lectura[0] = DatosLin[3];
                 Lectura[1] = DatosLin[4];
                 Lectura[2] = DatosLin[1];
                 Lectura[3] = DatosLin[2];
                 Valor = Lectura.toInt(&ok,16);

                 qDebug() << "Min:" << Valor;
                 ui->RPM_ALMIN->setText(QString::number(Valor,10));
                 EIndice = 1;
              break;
             }

         }
         DatosLin.clear();
      }
}
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
    Escribir = true;
}


void MainWindow::bucle1()
{
    if(serial->isOpen()&& Escribir)
    {
       Escribir = false;
    switch (EIndice)
    {
        default:
        case 1:
        // NUMERO DE SERIE
            serial->write("$40FF0604FA\r\n");
            LIndice = 1;
         break;
         case 2:
        //FECHA DE FABRICACION
            serial->write("$40FF0604FB\r\n");
            LIndice = 2;
         break;
         case 3:
        //FECHA DE FABRICACION
            serial->write("$40FF0604F5\r\n");
            LIndice = 3;
         break;
         case 4:
        //VERSION DEL SOFTWARE
            serial->write("$40FF0604FD\r\n");
            LIndice = 4;
         break;
         case 5:
        //ID DEL PRODUCTO
            serial->write("$40FF0701\r\n");
            LIndice = 5;
         break;
         case 10:
            serial->write("$40FF0002\r\n");
            LIndice = 10;
         break;
         case 11:
            serial->write("$40FF0401\r\n");
            LIndice = 11;
         break;
        case 12:
        //Sensores Tiempo de Medicion
           serial->write("$40FF0101\r\n");
           LIndice = 12;
        break;
        case 13:
        //Sensores Tiempo de deteccion
           serial->write("$40FF0201\r\n");
           LIndice = 13;
        break;
        case 14:
        //Sensores Cantidad para activacion
           serial->write("$40FF0301\r\n");
           LIndice = 14;
        break;
        case 20:
        //Lectura datos de GPS
            serial->write("$40D603\r\n");
            LIndice = 20;
        break;
        case 30:
        //Lectura de datos de sensore RPM y TURBINA
            serial->write("$40FF0002\r\n");
            LIndice = 30;
        break;
        case 31:
        //Lectura del factor K
            serial->write("$40FF060480\r\n");
            LIndice = 31;
         break;
         case 32:
         //Lectura Alarma Maxima (Solo Turbina)
            serial->write("$40FF060460\r\n");
            LIndice = 32;
         break;
         case 33:
        //Lectura Alarma minima (Solo Turbina)
            serial->write("$40FF060470\r\n");
            LIndice = 33;
         break;
       }
    }
}

void MainWindow::on_actionGuardar_triggered()
{
    QString NArchivo;
 //   if(!ui->Agente->isHidden())
 //   {
 //       QMessageBox::information(this,tr("Nombre Agente"),tr("Cargar el nombre del agente para generar"
 //                                                            "el nombre del archivo"));
 //       return;
 //    }
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
    ui->Agente->clear();
    ui->Operario->clear();
    ui->Fecha_Ingreso->clear();
    ui->Fecha_Control->clear();

 //Borrado de la planilla de 4500
    ui->MON_NSerie->clear();
    ui->Com_4500->clear();
    ui->MON_PB->setChecked(false);
    ui->MON_ACT->setChecked(false);
    ui->MON_MIC->setChecked(false);
    ui->MON_CC->setChecked(false);
    ui->MON_LCD->setChecked(false);
    ui->MON_CC->setChecked(false);
    ui->MON_FE->setChecked(false);

//Cargo la fecha actual al campo de prueba
    ui->Fecha_Control->setText(FechaActual.toString("dd/MM//yyyy"));

//Borrado de la planilla de sensores

}


void MainWindow::on_Ingreso_Guardar_clicked()
{
    QString Ingreso;
//    if(!ui->Agente->t || !ui->Fecha_Ingreso->isHidden() || !ui->Operario->isHidden() || !ui->Fecha_Control->isHidden())
//    {
 //       QMessageBox::information(this,tr("Cargar Datos"),tr("Faltan completar Campos"));
//        return;
//    }
    if(!ConfInicio)
    {
        ui->Datos->setText("Informe de Reparaciones");
        ui->Datos->append("Agente:;;" + ui->Agente->text() + ";;;Fecha Ingreso :;" + ui->Fecha_Ingreso->text());
        ui->Datos->append("Operario:;;" + ui->Operario->text() + ";;;Fecha Control:;" + ui->Fecha_Control->text());
        ConfInicio = true;
        Columnas = false;
    }

    if(!ui->Ingreso_Cantidad->value())
    {
        QMessageBox::critical(this, tr("Ingreso Cantidad"),
                              tr("<b>Presta Atencion:</b> Ingresar Cantidad de equipos"));
    }
    else
    {
        Ingreso.append(ui->Ingreso_Equipo->currentText());
        Ingreso.append(";");
        Ingreso.append(ui->Ingreso_Cantidad->text());
        Ingreso.append(";");
        Ingreso.append(ui->Ingreso_Comentario->toPlainText());
        ui->Datos->append( Ingreso );
        ui->Ingreso_Comentario->clear();
        ui->Ingreso_Cantidad->clear();
        ui->Ingreso_Cantidad->setValue(0);
    }
}


void MainWindow:: TituloColumnas()
{
    Columnas = true;
    ui->Datos->append("Item;Equipo;NúmeroSerie;Fecha Fab.;Fecha Inst.;V.Soft;F.Soft;Conf;Fallas;Comentario");
}

void MainWindow::on_actionGateWay_triggered()
{

    QString NArchivo;

    NArchivo.clear();
    NArchivo.append(ui->Agente->text());
    NArchivo.append(FechaActual.toString("yyyyMMdd"));
    ui->Fecha_Control->setText(FechaActual.toString("dd/MM//yyyy"));

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
    ui->MON_NSerie->clear();
    ui->Com_4500->clear();
    ui->MON_TIPO->setCurrentIndex(0);

    ui->MON_PB->setChecked(false);
    ui->MON_ACT->setChecked(false);
    ui->MON_MIC->setChecked(false);
    ui->MON_CC->setChecked(false);
    ui->MON_LCD->setChecked(false);
    ui->MON_CC->setChecked(false);
    ui->MON_FE->setChecked(false);


}
void MainWindow::on_BT_G4500_clicked()
{
    QString Info;
    int con;
    con = 6;

    if(!ui->MON_TIPO->currentIndex())
    {
        QMessageBox::information(this,tr("Seleccion Tipo Monitor"),
                                 tr("Seleccionar tipo de Monitor"));
        return;
    }
    if(!Columnas)
        TituloColumnas();

    Info.append(ui->MON_TIPO->currentText() + ";" + ui->MON_NSerie->text() + ";" );
    if(ui->MON_PB->isChecked())
    {
        Info.append("4R7 + Prot Bus;");
        con--;
    }
    if(ui->MON_ACT->isChecked())
    {
        Info.append("Act Soft");
        con--;
    }
    if(ui->MON_MIC->isChecked())
    {
        Info.append("Microtec;");
        con--;
    }
    if(ui->MON_CC->isChecked())
    {
        Info.append("Carcaza;");
        con--;
    }
    if(ui->MON_LCD->isChecked())
    {
        Info.append("LCD;");
        con--;
    }
    if(ui->MON_FE->isChecked())
    {
        Info.append("FCE;");
        con--;
    }
    while(con)
    {
        con--;
        Info.append(";");
    }
    Info.append(ui->Com_4500->toPlainText());
    ui->Datos->append(Info);
}

void MainWindow::on_S_Guardar_clicked()
{
    QString Sensor;
    int con;
    bool sig;
     con = 4;
     sig = false;
    con = ui->S_TIPO->currentIndex();
  //  qDebug () << "Seleccionar" << con;

    con = 4;
    if(!Columnas)
        TituloColumnas();

    if(!ui->S_TIPO->currentIndex())
    {
        QMessageBox::critical(this, tr("Seleccion de Sensor"),
                              tr("Seleccionar Tipo de sensor antes de guardar"));
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
                Sensor.append("_");
            Sensor.append("PRF");
            sig = true;
        }
        if(ui->S_CCD->isChecked())
        {
            if(sig)
                Sensor.append("_");
            Sensor.append("CCD");
            sig = true;
        }
        sig = false;

        QString Testo = ui->S_COM->toPlainText();
        if(ui->S_FOK->isChecked())
            Testo.append(" Func. Ok");
         ui->Datos->append( Sensor + ";" + Testo);
    }


}



void MainWindow::on_S_Borrar_Item_clicked()
{
    Item = 1;
    ui->SEN_ITEM->setText(QString::number(Item,10));
}

void MainWindow::on_S_ANT_clicked()
{
    PantallaActual -- ;
    if((PantallaActual > 4) || (!PantallaActual))
        PantallaActual = 4;
    CambioPantalla(PantallaActual);
}

void MainWindow::on_S_SIG_clicked()
{
    PantallaActual ++ ;
    if(PantallaActual > 4)
        PantallaActual = 1;
    CambioPantalla(PantallaActual);
}

void MainWindow::CambioPantalla(int Pant)
{
    PantallaActual = Pant;
    switch(Pant)
    {
    //Pantalla de Semillas y Fertilizante
    case 1:
        ui->FSemilla->setVisible(true);
        ui->FGPS->setVisible(false);
        ui->FROT->setVisible(false);
        ui->FMOD->setVisible(false);
    break;
    case 2:
        ui->FSemilla->setVisible(false);
        ui->FGPS->setVisible(true);
        ui->FROT->setVisible(false);
        ui->FMOD->setVisible(false);
    break;
    case 3:
        ui->FSemilla->setVisible(false);
        ui->FGPS->setVisible(false);
        ui->FROT->setVisible(true);
        ui->FMOD->setVisible(false);
    break;
    case 4:
        ui->FSemilla->setVisible(false);
        ui->FGPS->setVisible(false);
        ui->FROT->setVisible(false);
        ui->FMOD->setVisible(true);
    break;
    }
}
