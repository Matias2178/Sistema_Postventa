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
//! [0]

//Variables de uso General


QDateTime FControl;
//


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    Item = 1;
    memoria = 17;

//! [0]
    ui->setupUi(this);
    ui->SEN_ITEM->setText(QString::number(Item,10));
    FechaActual = QDate::currentDate();
    Mascaras();
    CambioPantalla(1);
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
    ReparacionesActualizar("*");
    IngresoActualizar(RepID);

//! [1]
    serial = new QSerialPort(this);

//! [1]
    settings = new SettingsDialog;
    SelEditores = new selecciondeeditores;

    ui->MonFechaRep->setText(FechaActual.toString("dd/MM/yyyy"));
    ui->SEN_FR->setText(FechaActual.toString("dd/MM/yyyy"));
    ui->INS_FR->setText(FechaActual.toString("dd/MM/yyyy"));
    ui->FechaIngreso->setText(FechaActual.toString("dd/MM/yyyy"));
    ui->TrabFechaRep->setText(FechaActual.toString("dd/MM/yyyy"));

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
    ui->MonFechaRep->setText(FechaActual.toString("dd/MM/yyyy"));
    ui->SEN_FR->setText(FechaActual.toString("dd/MM/yyyy"));
    ui->INS_FR->setText(FechaActual.toString("dd/MM/yyyy"));
    ui->FechaIngreso->setText(FechaActual.toString("dd/MM/yyyy"));
    ui->TrabFechaRep->setText(FechaActual.toString("dd/MM/yyyy"));
}


//------------------------------------------------------------------
//Botones de Borrado y guardado de las diferentes pestañas
//------------------------------------------------------------------
void MainWindow::on_BT_B4500_clicked()
{
    BorraMonitores();
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


void MainWindow::on_INS_BBorrar_clicked()
{
    BorraINS();
}

void MainWindow::on_MON_TIPO_activated(int index)
{
    qDebug () << MonMascaras;
//    qDebug () << index;
//    qDebug () << MonMascaras.value(index);
    ui->MON_VSOFT->clear();
    ui->MON_VSOFT->setInputMask(MonMascaras.value(index));


 //Cargo las fallas en la tabla
    int fila;
    QString Conf;
    Conf.append("SELECT * FROM Fallas");

    QSqlQuery consultar;
    consultar.prepare(Conf);
    if(!consultar.exec())
    {
        qDebug() << "error:" << consultar.lastError();
    }
    else
    {
        qDebug() << "Se ejecuto bien";

    }
    QString Falla;

    ui->MON_FALLAS->setRowCount(0);
    ui->MON_FALLAS->setHorizontalHeaderItem(0, new QTableWidgetItem("Fallas"));
    fila = ui->MON_FALLAS->rowCount();
    ui->MON_FALLAS->insertRow(fila);
    ui->MON_FALLAS->setRowHeight(fila,20);
    ui->MON_FALLAS->setItem(fila,0,new QTableWidgetItem("Fun_OK"));
    ui->MON_FALLAS->item(fila,0)->setCheckState(Qt::Unchecked);

    while(consultar.next())
    {
        Falla.clear();
     //   qDebug () << "Por aca paso 1";
        Falla.append(consultar.value(1).toByteArray().constData());
     //   qDebug () << "db: " << Falla;
     //   qDebug () << "sel: " <<ui->MON_TIPO->itemText(index);

        if(Falla == ui->MON_TIPO->itemText(index))
        {
            Falla.clear();
            Falla.append(consultar.value(2).toByteArray().constData());
     //       qDebug () << "Falla: " << Falla;
            fila = ui->MON_FALLAS->rowCount();
            ui->MON_FALLAS->setRowHeight(fila,10);
            ui->MON_FALLAS->insertRow(fila);
            ui->MON_FALLAS->setRowHeight(fila,20);
            ui->MON_FALLAS->setItem(fila,0,new QTableWidgetItem(Falla) );
            ui->MON_FALLAS->item(fila,0)->setCheckState(Qt::Unchecked);
          //  ui->MON_FALLAS->setColumnWidth(0,100);
        }
        fila ++;
    }
}

void MainWindow::on_Siguiente_clicked()
{
    Siguiente = false;
    Guardar = true;
    NSerie = 0;
    ui->Siguiente->setEnabled(true);
}

void MainWindow::on_S_TIPO_activated(const QString &arg1)
{
    //Cargo las fallas en la tabla
       int fila;
       QString Conf;
       Conf.append("SELECT * FROM Fallas");

       QSqlQuery consultar;
       consultar.prepare(Conf);
       if(!consultar.exec())
       {
           qDebug() << "error:" << consultar.lastError();
       }
       else
       {
           qDebug() << "Se ejecuto bien";

       }
       qDebug () << arg1;
       QString Falla;

       ui->SEN_FALLAS->setRowCount(0);
       ui->SEN_FALLAS->setHorizontalHeaderItem(0, new QTableWidgetItem("Fallas"));
       fila = ui->SEN_FALLAS->rowCount();
       ui->SEN_FALLAS->insertRow(fila);
       ui->SEN_FALLAS->setRowHeight(fila,20);
       ui->SEN_FALLAS->setItem(fila,0,new QTableWidgetItem("Fun_OK"));
       ui->SEN_FALLAS->item(fila,0)->setCheckState(Qt::Unchecked);

       while(consultar.next())
       {
           Falla.clear();
           Falla.append(consultar.value(1).toByteArray().constData());
           qDebug () << "db: " << Falla;
          qDebug () << "sel: " << arg1;

           if(Falla == arg1)//ui->S_TIPO->itemText(index))
           {
               Falla.clear();
               Falla.append(consultar.value(2).toByteArray().constData());
               qDebug () << "Falla: " << Falla;
               fila = ui->SEN_FALLAS->rowCount();
               ui->SEN_FALLAS->setRowHeight(fila,10);
               ui->SEN_FALLAS->insertRow(fila);
               ui->SEN_FALLAS->setRowHeight(fila,20);
               ui->SEN_FALLAS->setItem(fila,0,new QTableWidgetItem(Falla) );
               ui->SEN_FALLAS->item(fila,0)->setCheckState(Qt::Unchecked);
             //  ui->SEN_FALLAS->setColumnWidth(0,100);
           }
           fila ++;
       }
}

void MainWindow::on_INS_TIPO_activated(const QString &arg1)
{
    //Cargo las fallas en la tabla
       int fila;
       QString Conf;
       Conf.append("SELECT * FROM Fallas");

       QSqlQuery consultar;
       consultar.prepare(Conf);
       if(!consultar.exec())
       {
           qDebug() << "error:" << consultar.lastError();
       }
       else
       {
           qDebug() << "Se ejecuto bien";

       }
       qDebug () << arg1;
       QString Falla;

       ui->INS_FALLAS->setRowCount(0);
       ui->INS_FALLAS->setHorizontalHeaderItem(0, new QTableWidgetItem("Fallas"));
       fila = ui->INS_FALLAS->rowCount();
       ui->INS_FALLAS->insertRow(fila);
       ui->INS_FALLAS->setRowHeight(fila,20);
       ui->INS_FALLAS->setItem(fila,0,new QTableWidgetItem("Fun_OK"));
       ui->INS_FALLAS->item(fila,0)->setCheckState(Qt::Unchecked);

       while(consultar.next())
       {
           Falla.clear();
           Falla.append(consultar.value(1).toByteArray().constData());
//           qDebug () << "db: " << Falla;
//          qDebug () << "sel: " << arg1;

           if(Falla == arg1)//ui->S_TIPO->itemText(index))
           {
               Falla.clear();
               Falla.append(consultar.value(2).toByteArray().constData());
               qDebug () << "Falla: " << Falla;
               fila = ui->INS_FALLAS->rowCount();
               ui->INS_FALLAS->setRowHeight(fila,10);
               ui->INS_FALLAS->insertRow(fila);
               ui->INS_FALLAS->setRowHeight(fila,20);
               ui->INS_FALLAS->setItem(fila,0,new QTableWidgetItem(Falla) );
               ui->INS_FALLAS->item(fila,0)->setCheckState(Qt::Unchecked);
             //  ui->INS_FALLAS->setColumnWidth(0,100);
           }
           fila ++;
       }
}

void MainWindow::on_actionActualizar_triggered()
{
    ProductosLeer();
}

void MainWindow::on_TrabajoAgente_activated(const QString &arg1)
{
  TrabajoActualizar(arg1);
}

void MainWindow::on_TrabajoReparaciones_clicked(const QModelIndex &index)
{
    QString Conf;
    QString Carga;
    int TrabID;
    IndexTrabajo = index.row();
    TrabID = ui->TrabajoReparaciones->item(index.row(),0)->text().toInt();
    Conf.append("SELECT * FROM Ingreso");
    RepID = TrabID;
    QSqlQuery consultar;
    consultar.prepare(Conf);
    if(!consultar.exec())
    {
        qDebug() << "error:" << consultar.lastError();
    }
    else
    {
        qDebug() << "Se ejecuto bien";

    }
    int fila  = 0;

    ui->TrabajoIngreso->setRowCount(0);

    while(consultar.next())
    {
    //    qDebug () << "ID consulta:" << consultar.value(4).toByteArray().toInt();
        if(TrabID == consultar.value("repid").toByteArray().toInt())
        {
            ui->TrabajoIngreso->insertRow(fila);
            ui->TrabajoIngreso->setRowHeight(fila,20);
            ui->TrabajoIngreso->setItem(fila,0,new QTableWidgetItem (consultar.value(0).toByteArray().constData()));
            ui->TrabajoIngreso->setItem(fila,1,new QTableWidgetItem (consultar.value(1).toByteArray().constData()));
            ui->TrabajoIngreso->setItem(fila,2,new QTableWidgetItem (consultar.value(2).toByteArray().constData()));
            ui->TrabajoIngreso->setItem(fila,3,new QTableWidgetItem (consultar.value(3).toByteArray().constData()));
            ui->TrabajoIngreso->setItem(fila,4,new QTableWidgetItem (consultar.value(4).toByteArray().constData()));
            Carga.append(consultar.value(1).toByteArray().constData());
            Carga.append(": ");
            Carga.append(consultar.value(2).toByteArray().constData());
            Carga.append(0x0d);
            ui->MONCant->setText(Carga);

            fila ++;
        }
    }
    ui->TrabRepID->setText(ui->TrabajoReparaciones->item(index.row(),0)->text());
    ui->TrabajoIngreso->setColumnWidth(0,25);
    ui->TrabajoIngreso->setColumnWidth(1,100);
    ui->TrabajoIngreso->setColumnWidth(2,50);
    ui->TrabajoIngreso->setColumnWidth(3,150);
    ui->TrabajoIngreso->setColumnWidth(4,40);
    MonitoresActualizar();
    PerifericosActualizar();
    InstalacionesActualizar();
    TrabajosActualizar();
    CaudalimetroActualizar(*ui->CaudalimetroTrabDatos);
}

void MainWindow::on_ReparacionesIniciar_clicked()
{
    if(ui->TrabRepID->text().isEmpty())
    {
        QMessageBox::critical(this,tr("Trabajo"),
                                  tr("Seleccionar Trabajo para cargar datos"));
        return;
    }

    if(ui->TrabajoReparaciones->item(IndexTrabajo,3)->text().isEmpty())
    {
        if(!ui->TrabajoOperario->currentIndex())
        {
            QMessageBox::critical(this,tr("Operario"),
                                  tr("Seleccionar Operario"));
            return;
        }
        QString Conf;
        Conf.append("UPDATE Reparaciones SET "
                    "frep ="
                    "'"+ui->TrabFechaRep->text()+"',"
                    "operario ="
                    "'"+ui->TrabajoOperario->currentText()+"'"
                    " WHERE id ="
                    ""+ui->TrabRepID->text()+""
                    "");
        QSqlQuery editar;
        editar.prepare(Conf);
        qDebug() << Conf;
        qDebug() << "error:" << IndexTrabajo << editar.lastError();
        if(!editar.exec())
        {
            qDebug() << "error:" << editar.lastError();
            QMessageBox::critical(this,tr("Error en un campo"),
                                 tr("Camos incompletos no se guardaron los datos"));
        }
        else
        {
            qDebug() << "Se Edito el item " << IndexTrabajo;
            ReparacionesActualizar(ui->AgenteNombre->currentText());
        }
    }
    ui->MonRepID->setText(ui->TrabajoReparaciones->item(IndexTrabajo,0)->text());
    ui->PerRepID->setText(ui->TrabajoReparaciones->item(IndexTrabajo,0)->text());
    ui->InstRepID->setText(ui->TrabajoReparaciones->item(IndexTrabajo,0)->text());
    TrabajoActualizar(ui->TrabajoAgente->currentText());
}

void MainWindow::on_MonitorGuardar_clicked()
{
    QString Info;
    bool sig;
    bool ok;
    int indice;
    int i;

//    if(!ui->MonRepID->text().toInt(ok,10))
//    {
//        qDebug () <<"sin datos"<< ui->MonRepID->text().toInt(ok,10);
//    }
//    else
//    {
//        qDebug () <<"con datos"<< ui->MonRepID->text().toInt(ok,10);
//    }
    if(!ui->MonRepID->text().toInt(&ok,10))
    {
        QMessageBox::information(this,tr("Seleccion Trabajo"),
                                 tr("Seleccionar Trabajo"));
        return;
    }
    qDebug () << ui->MonRepID->text().toInt(&ok,10);
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
    if(SNAnt == ui->MON_NSerie->text().toInt(&sig,10))
    {
        if(DobleGuardadoMsg())
            return;
    }

    SNAnt = ui->MON_NSerie->text().toInt(&sig,10);
    Info.clear();
    indice = ui->MON_FALLAS->rowCount();
    sig = false;
    for (i=0;i<indice;i++)
    {
        if(ui->MON_FALLAS->item(i,0)->checkState() == 2)
        {
            if(sig)
            {
                Info.append("-");
            }
            Info.append(ui->MON_FALLAS->item(i,0)->text());

            sig = true;
        }
        ui->MON_FALLAS->item(i,0)->setCheckState(Qt::Unchecked);
    }
    ui->SEN_ITEM->setText(QString::number(Item,10));

 //Carga datos DB
    QString Ingreso;
 //   qDebug() <<"ID" << ui->MonRepID->text();
 //   qDebug ()<<"Frep" <<ui->MonFechaRep->text();
    Ingreso.clear();
    if(!ConfInicio)
    {
        QString Conf;
        Conf.clear();
        Conf.append("INSERT INTO Monitores("
                    "nombre,"
                    "sn,"
                    "vsoft,"
                    "falla,"
                    "bonif,"
                    "obs,"
                    "frep,"
                    "repid)"
                    "VALUES("
                    "'"+ui->MON_TIPO->currentText()+    "',"
                    "'"+ui->MON_NSerie->text()+         "',"
                    "'"+ui->MON_VSOFT->text()+          "',"
                    "'"+Info+                           "',"
                    "'"+ui->MON_BON->currentText()+     "',"
                    "'"+ui->MON_COM->toPlainText()+     "',"
                    "'"+ui->MonFechaRep->text()+        "',"
                    "'"+ui->MonRepID->text()+           "'"
                    ");");

        QSqlQuery insertar;
        insertar.prepare(Conf);
        qDebug() << "Pre-error:" << insertar.lastError();
        if(!insertar.exec())
        {
            qDebug() << "error:" << insertar.lastError();
            QMessageBox::critical(this,tr("Error en un campo"),
                                      tr("Camos incompletos no se guardaron los datos"));
        }
        else
        {
            qDebug() << "Se Agrego Item bien" << insertar.value(0).toByteArray().constData();
            qDebug() << "error:" << insertar.lastError();
            MonitoresActualizar();
        }
    }
    ui->MON_BON->setCurrentIndex(0);
}

void MainWindow::on_SenGuardar_clicked()
{
    QString Fallas, FactConf;
    bool sig;
    int indice, i;
    if(ui->PerRepID->text().isEmpty())
    {
        MensajeTrabajo();
        return;
    }
    if(SNAnt == ui->SEN_NSERIE->text().toInt(&sig,10))
    {
        if(DobleGuardadoMsg())
            return;
    }
    if(!ui->S_TIPO->currentIndex())
    {
        QMessageBox::critical(this, tr("Seleccion de Sensor"),
                              tr("Seleccionar Tipo de sensor antes de guardar"));
    }
    else if (!ui->SEN_BON->currentIndex())
    {
        BonificacionMsg();
    }
    else
    {
        SNAnt = ui->SEN_NSERIE->text().toInt(&sig,10);
        Item ++;
        ui->SEN_ITEM->setText(QString::number(Item,10));
     //--------------------------------------------------------------------------------
     //     Control de Fallas
     //--------------------------------------------------------------------------------
        indice = ui->SEN_FALLAS->rowCount();
        sig = false;
        Fallas.clear();
        for (i=0;i<indice;i++)
        {
            if(ui->SEN_FALLAS->item(i,0)->checkState() == 2)
            {
                if(sig)
                {
                    Fallas.append("-");
                }
                Fallas.append(ui->SEN_FALLAS->item(i,0)->text());

                sig = true;
            }
            ui->SEN_FALLAS->item(i,0)->setCheckState(Qt::Unchecked);
        }
        FactConf.append("TD:" + ui->S_TD->text() + " TM:" + ui->S_TM->text()
                      + " SA:" + ui->S_CA->text());
        //Carga datos DB
        QString Ingreso;
        //   qDebug() <<"ID" << ui->MonRepID->text();
        //   qDebug ()<<"Frep" <<ui->MonFechaRep->text();
        Ingreso.clear();
        if(!ConfInicio)
        {
            QString Conf;
            Conf.clear();
            Conf.append("INSERT INTO Perifericos("
                        "nombre,"
                        "sn,"
                        "ffab,"
                        "finst,"
                        "vsoft,"
                        "fsoft,"
                        "conf,"
                        "falla,"
                        "bonif,"
                        "obs,"
                        "frep,"
                        "repid)"
                        "VALUES("
                        "'"+ui->S_TIPO->currentText()+  "',"
                        "'"+ui->SEN_NSERIE->text()+     "',"
                        "'"+ui->SEN_FF->text()+         "',"
                        "'"+ui->SEN_FI->text()+         "',"
                        "'"+ui->SEN_VS->text()+         "',"
                        "'"+ui->SEN_FS->text()+         "',"
                        "'"+FactConf+                   "',"
                        "'"+Fallas+                     "',"
                        "'"+ui->SEN_BON->currentText()+   "',"
                        "'"+ui->S_COM->toPlainText()+   "',"
                        "'"+ui->SEN_FR->text()+         "',"
                        "'"+ui->PerRepID->text()+       "'"
                        ");");

            QSqlQuery insertar;
            insertar.prepare(Conf);
            qDebug() << "error:" << insertar.lastError();
            if(!insertar.exec())
            {
                qDebug() << "error:" << insertar.lastError();
                QMessageBox::critical(this,tr("Error en un campo"),
                                      tr("Camos incompletos no se guardaron los datos"));
            }
            else
            {
                qDebug() << "Se Agrego Item bien" << insertar.value(0).toByteArray().constData();
                qDebug() << "error:" << insertar.lastError();
                PerifericosActualizar();
            }
        }
       // ui->SEN_BON->setCurrentIndex(0);
        sig = false;
    }
    Guardar = true;
    Siguiente = false;
    NSerie = 0;
}

void MainWindow::on_RPMGuardar_clicked()
{
    QString Fallas, FactConf;
    bool sig;
    int indice, i;
    if(ui->PerRepID->text().isEmpty())
    {
        MensajeTrabajo();
        return;
    }
    if(SNAnt == ui->SEN_NSERIE->text().toInt(&sig,10))
    {
        if(DobleGuardadoMsg())
            return;
    }
    if (!ui->SEN_BON->currentIndex())
    {
        BonificacionMsg();
    }
    else
    {
        SNAnt = ui->SEN_NSERIE->text().toInt(&sig,10);
        Item ++;
        ui->SEN_ITEM->setText(QString::number(Item,10));
     //--------------------------------------------------------------------------------
     //     Control de Fallas
     //--------------------------------------------------------------------------------
        indice = ui->RPM_FALLAS->rowCount();
        sig = false;
        Fallas.clear();
        for (i=0;i<indice;i++)
        {
            if(ui->RPM_FALLAS->item(i,0)->checkState() == 2)
            {
                if(sig)
                {
                    Fallas.append("-");
                }
                Fallas.append(ui->RPM_FALLAS->item(i,0)->text());

                sig = true;
            }
            ui->RPM_FALLAS->item(i,0)->setCheckState(Qt::Unchecked);
        }
        FactConf.append("FK:" + ui->RPM_FK->text());
        //Carga datos DB
        QString Ingreso;
        //   qDebug() <<"ID" << ui->MonRepID->text();
        //   qDebug ()<<"Frep" <<ui->MonFechaRep->text();
        Ingreso.clear();
        if(!ConfInicio)
        {
            QString Conf;
            Conf.clear();
            Conf.append("INSERT INTO Perifericos("
                        "nombre,"
                        "sn,"
                        "ffab,"
                        "finst,"
                        "vsoft,"
                        "fsoft,"
                        "conf,"
                        "falla,"
                        "bonif,"
                        "obs,"
                        "frep,"
                        "repid)"
                        "VALUES("
                        "'"+ui->SEN_TIPO->text()+       "',"
                        "'"+ui->SEN_NSERIE->text()+     "',"
                        "'"+ui->SEN_FF->text()+         "',"
                        "'"+ui->SEN_FI->text()+         "',"
                        "'"+ui->SEN_VS->text()+         "',"
                        "'"+ui->SEN_FS->text()+         "',"
                        "'"+FactConf+                   "',"
                        "'"+Fallas+                     "',"
                        "'"+ui->SEN_BON->currentText()+   "',"
                        "'"+ui->RPM_COM->toPlainText()+   "',"
                        "'"+ui->SEN_FR->text()+         "',"
                        "'"+ui->PerRepID->text()+       "'"
                        ");");

            QSqlQuery insertar;
            insertar.prepare(Conf);
            qDebug() << "error:" << insertar.lastError();
            if(!insertar.exec())
            {
                qDebug() << "error:" << insertar.lastError();
                QMessageBox::critical(this,tr("Error en un campo"),
                                      tr("Camos incompletos no se guardaron los datos"));
            }
            else
            {
                qDebug() << "Se Agrego Item bien" << insertar.value(0).toByteArray().constData();
                qDebug() << "error:" << insertar.lastError();
                PerifericosActualizar();
            }
        }
       // ui->SEN_BON->setCurrentIndex(0);
        sig = false;
    }
    Guardar = true;
    Siguiente = false;
    NSerie = 0;
}

void MainWindow::on_MODGuardar_clicked()
{
    QString Fallas, FactConf;
    bool sig;
    int indice, i;
    if(ui->PerRepID->text().isEmpty())
    {
        MensajeTrabajo();
        return;
    }
    if(SNAnt == ui->SEN_NSERIE->text().toInt(&sig,10))
    {
        if(DobleGuardadoMsg())
            return;
    }
    if (!ui->SEN_BON->currentIndex())
    {
        BonificacionMsg();
    }
    else
    {
        SNAnt = ui->SEN_NSERIE->text().toInt(&sig,10);
        Item ++;
        ui->SEN_ITEM->setText(QString::number(Item,10));
     //--------------------------------------------------------------------------------
     //     Control de Fallas
     //--------------------------------------------------------------------------------
        indice = ui->MOD_FALLAS->rowCount();
        sig = false;
        Fallas.clear();
        for (i=0;i<indice;i++)
        {
            if(ui->MOD_FALLAS->item(i,0)->checkState() == 2)
            {
                if(sig)
                {
                    Fallas.append("-");
                }
                Fallas.append(ui->MOD_FALLAS->item(i,0)->text());

                sig = true;
            }
            ui->MOD_FALLAS->item(i,0)->setCheckState(Qt::Unchecked);
        }
        FactConf.append("FK:" + ui->MOD_FK->text()+ "DT:" + ui->MOD_DT->text()+"DD:" + ui->MOD_DD->text()
                        +"RT:" + ui->MOD_RT->text());
        //Carga datos DB
        QString Ingreso;
        //   qDebug() <<"ID" << ui->MonRepID->text();
        //   qDebug ()<<"Frep" <<ui->MonFechaRep->text();
        Ingreso.clear();
        if(!ConfInicio)
        {
            QString Conf;
            Conf.clear();
            Conf.append("INSERT INTO Perifericos("
                        "nombre,"
                        "sn,"
                        "ffab,"
                        "finst,"
                        "vsoft,"
                        "fsoft,"
                        "conf,"
                        "falla,"
                        "bonif,"
                        "obs,"
                        "frep,"
                        "repid)"
                        "VALUES("
                        "'"+ui->SEN_TIPO->text()+       "',"
                        "'"+ui->SEN_NSERIE->text()+     "',"
                        "'"+ui->SEN_FF->text()+         "',"
                        "'"+ui->SEN_FI->text()+         "',"
                        "'"+ui->SEN_VS->text()+         "',"
                        "'"+ui->SEN_FS->text()+         "',"
                        "'"+FactConf+                   "',"
                        "'"+Fallas+                     "',"
                        "'"+ui->SEN_BON->currentText()+   "',"
                        "'"+ui->MOD_COM->toPlainText()+   "',"
                        "'"+ui->SEN_FR->text()+         "',"
                        "'"+ui->PerRepID->text()+       "'"
                        ");");

            QSqlQuery insertar;
            insertar.prepare(Conf);
            qDebug() << "error:" << insertar.lastError();
            if(!insertar.exec())
            {
                qDebug() << "error:" << insertar.lastError();
                QMessageBox::critical(this,tr("Error en un campo"),
                                      tr("Camos incompletos no se guardaron los datos"));
            }
            else
            {
                qDebug() << "Se Agrego Item bien" << insertar.value(0).toByteArray().constData();
                qDebug() << "error:" << insertar.lastError();
                PerifericosActualizar();
            }
        }
      //  ui->SEN_BON->setCurrentIndex(0);
        sig = false;
    }
    Guardar = true;
    Siguiente = false;
    NSerie = 0;
}

void MainWindow::on_GPSGuardar_clicked()
{
    QString Fallas, FactConf;
    bool sig;
    int indice, i;
    if(ui->PerRepID->text().isEmpty())
    {
        MensajeTrabajo();
        return;
    }
    if(SNAnt == ui->SEN_NSERIE->text().toInt(&sig,10))
    {
        if(DobleGuardadoMsg())
            return;
    }
    if (!ui->SEN_BON->currentIndex())
    {
        BonificacionMsg();
    }
    else
    {
        SNAnt = ui->SEN_NSERIE->text().toInt(&sig,10);
        Item ++;
        ui->SEN_ITEM->setText(QString::number(Item,10));
     //--------------------------------------------------------------------------------
     //     Control de Fallas
     //--------------------------------------------------------------------------------
        indice = ui->GPS_FALLAS->rowCount();
        sig = false;
        Fallas.clear();
        for (i=0;i<indice;i++)
        {
            if(ui->GPS_FALLAS->item(i,0)->checkState() == 2)
            {
                if(sig)
                {
                    Fallas.append("-");
                }
                Fallas.append(ui->GPS_FALLAS->item(i,0)->text());

                sig = true;
            }
            ui->GPS_FALLAS->item(i,0)->setCheckState(Qt::Unchecked);
        }
        FactConf.append(" ");
        //Carga datos DB
        QString Ingreso;
        //   qDebug() <<"ID" << ui->MonRepID->text();
        //   qDebug ()<<"Frep" <<ui->MonFechaRep->text();
        Ingreso.clear();
        if(!ConfInicio)
        {
            QString Conf;
            Conf.clear();
            Conf.append("INSERT INTO Perifericos("
                        "nombre,"
                        "sn,"
                        "ffab,"
                        "finst,"
                        "vsoft,"
                        "fsoft,"
                        "conf,"
                        "falla,"
                        "bonif,"
                        "obs,"
                        "frep,"
                        "repid)"
                        "VALUES("
                        "'"+ui->SEN_TIPO->text()+       "',"
                        "'"+ui->SEN_NSERIE->text()+     "',"
                        "'"+ui->SEN_FF->text()+         "',"
                        "'"+ui->SEN_FI->text()+         "',"
                        "'"+ui->SEN_VS->text()+         "',"
                        "'"+ui->SEN_FS->text()+         "',"
                        "'"+FactConf+                   "',"
                        "'"+Fallas+                     "',"
                        "'"+ui->SEN_BON->currentText()+   "',"
                        "'"+ui->GPS_COM->toPlainText()+   "',"
                        "'"+ui->SEN_FR->text()+         "',"
                        "'"+ui->PerRepID->text()+       "'"
                        ");");

            QSqlQuery insertar;
            insertar.prepare(Conf);
            qDebug() << "error:" << insertar.lastError();
            if(!insertar.exec())
            {
                qDebug() << "error:" << insertar.lastError();
                QMessageBox::critical(this,tr("Error en un campo"),
                                      tr("Camos incompletos no se guardaron los datos"));
            }
            else
            {
                qDebug() << "Se Agrego Item bien" << insertar.value(0).toByteArray().constData();
                qDebug() << "error:" << insertar.lastError();
                PerifericosActualizar();
            }
        }
     //   ui->SEN_BON->setCurrentIndex(0);
        sig = false;
    }
    Guardar = true;
    Siguiente = false;
    NSerie = 0;
}

void MainWindow::on_CAUGuardar_clicked()
{
    QString Fallas, FactConf;
    bool sig;
    int indice, i;
    if(ui->PerRepID->text().isEmpty())
    {
        MensajeTrabajo();
        return;
    }
    if(SNAnt == ui->SEN_NSERIE->text().toInt(&sig,10))
    {
        if(DobleGuardadoMsg())
            return;
    }

    SNAnt = ui->SEN_NSERIE->text().toInt(&sig,10);
    Item ++;
    ui->SEN_ITEM->setText(QString::number(Item,10));
    //--------------------------------------------------------------------------------
    //     Control de Fallas
    //--------------------------------------------------------------------------------
    indice = ui->CAU_FALLAS->rowCount();
    sig = false;
    Fallas.clear();
    for (i=0;i<indice;i++)
    {
        if(ui->CAU_FALLAS->item(i,0)->checkState() == 2)
        {
            if(sig)
            {
                Fallas.append("-");
            }
            Fallas.append(ui->CAU_FALLAS->item(i,0)->text());

            sig = true;
        }
        ui->CAU_FALLAS->item(i,0)->setCheckState(Qt::Unchecked);
    }
    //Carga datos DB
    QString Ingreso;
    //   qDebug() <<"ID" << ui->MonRepID->text();
    //   qDebug ()<<"Frep" <<ui->MonFechaRep->text();
    Ingreso.clear();
    if(!ConfInicio)
    {
        QString Conf;
        Conf.clear();
        Conf.append("INSERT INTO Caudalimetro("
                    "nombre,"
                    "sn,"
                    "movil,"
                    "ffab,"
                    "finst,"
                    "vsoft,"
                    "fsoft,"
                    "tmt,"
                    "cct,"
                    "desc,"
                    "descat,"
                    "cbmag,"
                    "tbmag,"
                    "falla,"
                    "bonif,"
                    "obs,"
                    "frep,"
                    "repid)"
                    "VALUES("
                    "'"+ui->SEN_TIPO->text()+       "',"
                                                    "'"+ui->SEN_NSERIE->text()+     "',"
                                                                                    "'"+ui->CAU_INST->text()+       "',"
                                                                                                                    "'"+ui->SEN_FF->text()+         "',"
                                                                                                                                                    "'"+ui->SEN_FI->text()+         "',"
                                                                                                                                                                                    "'"+ui->SEN_VS->text()+         "',"
                                                                                                                                                                                                                    "'"+ui->SEN_FS->text()+         "',"
                                                                                                                                                                                                                                                    "'"+ui->CAU_TMT->text()+        "',"
                                                                                                                                                                                                                                                                                    "'"+ui->CAU_CCT->text()+        "',"
                                                                                                                                                                                                                                                                                                                    "'"+ui->CAU_DESC->text()+       "',"
                                                                                                                                                                                                                                                                                                                                                    "'"+ui->CAU_DESAT->text()+      "',"
                                                                                                                                                                                                                                                                                                                                                                                    "'"+ui->CAU_BMAG->text()+       "',"
                                                                                                                                                                                                                                                                                                                                                                                                                    "'"+ui->CAU_TBMAG->text()+      "',"
                                                                                                                                                                                                                                                                                                                                                                                                                                                    "'"+Fallas+                     "',"
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    "'"+ui->SEN_BON->currentText()+ "',"
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    "'"+ui->CAU_COM->toPlainText()+ "',"
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    "'"+ui->SEN_FR->text()+         "',"
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    "'"+ui->PerRepID->text()+       "'"
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    ");");

        QSqlQuery insertar;
        insertar.prepare(Conf);
        qDebug() << "error:" << insertar.lastError();
        if(!insertar.exec())
        {
            qDebug() << "error:" << insertar.lastError();
            QMessageBox::critical(this,tr("Error en un campo"),
                                  tr("Camos incompletos no se guardaron los datos"));
        }
        else
        {
            //                qDebug() << "Se Agrego Item bien" << insertar.value(0).toByteArray().constData();
            //                qDebug() << "error:" << insertar.lastError();
            CaudalimetroActualizar(*ui->CaudalimetroDatos);
        }
    }
    //   ui->SEN_BON->setCurrentIndex(0);
    //    sig = false;
    Guardar = true;
    Siguiente = false;
    NSerie = 0;
}

void MainWindow::on_INSGuardar_clicked()
{
    QString Fallas;
    bool sig;
    int indice, i;

    if(ui->InstRepID->text().isEmpty())
    {
        MensajeTrabajo();
        return;
    }

    if(!ui->INS_TIPO->currentIndex())
    {

        QMessageBox::critical(this, tr("Seleccion de Instalacion"),
                              tr("Seleccionar Tipo de instalacion antes de guardar"));
        return;
    }
    if(SNAnt == ui->SEN_NSERIE->text().toInt(&sig,10))
    {
        if(DobleGuardadoMsg())
            return;
    }
    if (!ui->INS_BON->currentIndex())
    {
        BonificacionMsg();
    }
    else
    {
        SNAnt = ui->SEN_NSERIE->text().toInt(&sig,10);
        Item ++;
        ui->SEN_ITEM->setText(QString::number(Item,10));
     //--------------------------------------------------------------------------------
     //     Control de Fallas
     //--------------------------------------------------------------------------------
        indice = ui->INS_FALLAS->rowCount();
        sig = false;
        Fallas.clear();
        for (i=0;i<indice;i++)
        {
            if(ui->INS_FALLAS->item(i,0)->checkState() == 2)
            {
                if(sig)
                {
                    Fallas.append("-");
                }
                Fallas.append(ui->INS_FALLAS->item(i,0)->text());

                sig = true;
            }
            ui->INS_FALLAS->item(i,0)->setCheckState(Qt::Unchecked);
        }
//Carga datos DB
        QString Ingreso;
        //   qDebug() <<"ID" << ui->MonRepID->text();
        //   qDebug ()<<"Frep" <<ui->MonFechaRep->text();
        Ingreso.clear();
        if(!ConfInicio)
        {
            QString Conf;
            Conf.clear();
            Conf.append("INSERT INTO Instalaciones("
                        "nombre,"
                        "sn,"
                        "falla,"
                        "bonif,"
                        "obs,"
                        "frep,"
                        "repid)"
                        "VALUES("
                        "'"+ui->INS_TIPO->currentText()+"',"
                        "'"+ui->INS_NSerie->text()+     "',"
                        "'"+Fallas+                     "',"
                        "'"+ui->INS_BON->currentText()+ "',"
                        "'"+ui->INS_COM->toPlainText()+ "',"
                        "'"+ui->INS_FR->text()+         "',"
                        "'"+ui->InstRepID->text()+      "'"
                        ");");

            QSqlQuery insertar;
            insertar.prepare(Conf);
            qDebug() << "error:" << insertar.lastError();
            if(!insertar.exec())
            {
                qDebug() << "error:" << insertar.lastError();
                QMessageBox::critical(this,tr("Error en un campo"),
                                      tr("Camos incompletos no se guardaron los datos"));
            }
            else
            {
    //            qDebug() << "Se Agrego Item bien" << insertar.value(0).toByteArray().constData();
    //            qDebug() << "error:" << insertar.lastError();
                InstalacionesActualizar();
            }
        }
        ui->INS_BON->setCurrentIndex(0);
    }
}
void MainWindow::on_RepInterno_clicked()
{
    QString NArchivo;
    QString Dts;
    int Filas;
    int i, a;
    DatosArchivo.clear();
    if(!ui->TrabajoAgente->currentIndex())
    {
        QMessageBox::critical(this,tr("Selección Trabajo"),
                              tr("Seleccionar trabajo para generar el informe"));
        return;

    }
    if(ui->TrabajoReparaciones->item(IndexTrabajo,3)->text().isEmpty())
    {
        QMessageBox::information(this,tr("Trabajo"),
                              tr("Iniciar reparacion para generar el reporte"));
        return;
    }
    NArchivo.clear();
    NArchivo.append(ui->TrabajoReparaciones->item(IndexTrabajo,3)->text());
    NArchivo.replace(2,1,"_");
    NArchivo.replace(5,1,"_");
    NArchivo.prepend("_");
    NArchivo.prepend(ui->TrabajoReparaciones->item(IndexTrabajo,1)->text());
    NArchivo.prepend("user/home/");

    DatosArchivo.append("Informe de Reparaciones");
    DatosArchivo.append("Agente:;;" + ui->TrabajoReparaciones->item(IndexTrabajo,1)->text()
                         + ";;;Fecha Ingreso :;" + ui->TrabajoReparaciones->item(IndexTrabajo,2)->text());

    DatosArchivo.append("Operario:;;" + ui->TrabajoReparaciones->item(IndexTrabajo,4)->text()
                        + ";;;Fecha Control:;" + ui->TrabajoReparaciones->item(IndexTrabajo,3)->text());
    DatosArchivo.append(" ");
    DatosArchivo.append("Nombre;;Cant");
    Filas = ui->TrabajoIngreso->rowCount();
    for(i=0;i<Filas;i++)
    {
       DatosArchivo.append(  ui->TrabajoIngreso->item(i,1)->text()+ ":;;"
                    + ui->TrabajoIngreso->item(i,2)->text()+ ";"
                    + ui->TrabajoIngreso->item(i,3)->text());
    }
    if(ui->CaudalimetroTrabDatos->rowCount())
    {
        DatosArchivo.append(" ");
        DatosArchivo.append("Nombre;SN;Movil;F Fab;F Inst;V Soft;F Soft;TMT;CPT;DESC;DESC AT;BLOC MAG;T.BLOC MAG;Falla;Bonif;Obs;F Rep");
        Filas = ui->CaudalimetroTrabDatos->rowCount();
        for(i=0;i<Filas;i++)
        {
            Dts.clear();
            for(a=1;a<=17;a++)
            {
                Dts.append(ui->CaudalimetroTrabDatos->item(i,a)->text() + ";");
            }
            DatosArchivo.append(Dts);
        }
    }
    if(ui->TrabajoDatos->rowCount())
    {
        DatosArchivo.append(" ");
        DatosArchivo.append("Nombre;SN;F Fab;F Inst;V Soft;F Soft;Configuración;Falla;Bonif;Obs;F Rep");
        Filas = ui->TrabajoDatos->rowCount();
        for(i=0;i<Filas;i++)
        {
            Dts.clear();
            for(a=0;a<=10;a++)
            {
                Dts.append(ui->TrabajoDatos->item(i,a)->text() + ";");
            }
            DatosArchivo.append(Dts);
        }
    }
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

