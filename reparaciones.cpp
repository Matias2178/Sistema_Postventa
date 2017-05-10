#include "mainwindow.h"
#include "reparaciones.h"
#include "ui_reparaciones.h"
#include <QMessageBox>
#include <variables.h>
#include <dbmanejo.h>
#include "user.h"
#include <QDebug>
#include <QTimer>
#include <QDateTime>

dbManejo dbReparaciones;
QDateTime fReparaciones;

Reparaciones::Reparaciones(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Reparaciones)
{
    ui->setupUi(this);
    ui->tabWidget->setCurrentIndex(0);
    CambioPantalla(1);
    QTimer *Tiempo = new QTimer(this);

    ui->PerRepID->setText(QString::number(gTrabajoIdReparacion,10));
    dbReparaciones.CargarFallas(*ui->CAU_FALLAS,"SCC");     //Cargo fallas del caudalimetro
    dbReparaciones.CargarFallas(*ui->GPS_FALLAS,"GPS");     //Cargo fallas del GPS
    dbReparaciones.CargarFallas(*ui->MOD_FALLAS,"MOD");     //Cargo fallas Moduladora
    dbReparaciones.CargarFallas(*ui->RPM_FALLAS,"RPM");     //Cargo fallas de los sensores de RPM y Velocidad
    ui->MON_TIPO->clear();
    ui->MON_TIPO->addItems(dbReparaciones.CargarProductos(1));
    ui->SEM_TIPO->clear();
    ui->SEM_TIPO->addItems(dbReparaciones.CargarProductos(2));
    ui->INS_TIPO->clear();
    ui->INS_TIPO->addItems(dbReparaciones.CargarProductos(3));
    ui->SEN_FR->setInputMask("00/00/0000");
    ui->SEN_FR->setText(fReparaciones.currentDateTime().toString("ddMMyyyy"));

    ui->MON_REP_ID->setText(QString::number(IdReparacion,10));
    ui->PerRepID->setText(QString::number(IdReparacion,10));
    ui->InstRepID->setText(QString::number(IdReparacion,10));
    ui->MON_FECHA_REP->setInputMask("00/00/0000");
    ui->MON_FECHA_REP->setText(fReparaciones.currentDateTime().toString("ddMMyyyy"));
    ui->INS_FR->setInputMask("00/00/0000");
    ui->INS_FR->setText(fReparaciones.currentDateTime().toString("ddMMyyyy"));

    dbReparaciones.ActualizarMonitores(*ui->MonitoresDatos,IdReparacion);
    dbReparaciones.ActualizarCaudalimetro(*ui->CaudalimetroDatos,IdReparacion);
    dbReparaciones.ActualizarPerifericos(*ui->PerifericosDatos,IdReparacion);
    dbReparaciones.ActualizarInstalaciones(*ui->InstalacionesDatos,IdReparacion);

 //   connect(serial, SIGNAL(readyRead()), this, SLOT(LIN_Lectura()));
    connect(Tiempo, SIGNAL(timeout()), this, SLOT(LIN_Lectura()));
    Tiempo->start(5);

}

Reparaciones::~Reparaciones()
{
    delete ui;
}

void Reparaciones::on_MON_TIPO_activated(int index)
{
    ui->MON_VSOFT->clear();
    ui->MON_VSOFT->setInputMask(MonMascaras.value(index));

    dbReparaciones.CargarFallas(*ui->MON_FALLAS,ui->MON_TIPO->currentText());
}

void Reparaciones::on_MON_GUARDAR_clicked()
{
    QString Info;
    bool sig;
    bool ok;
    int indice;
    int i;
    if(!ui->MON_REP_ID->text().toInt(&ok,10))
    {
        QMessageBox::information(this,tr("Seleccion Trabajo"),
                                 tr("Seleccionar Trabajo"));
        return;
    }
//    qDebug () << ui->MonRepID->text().toInt(&ok,10);
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
//    if(SNAnt == ui->MON_NSerie->text().toInt(&sig,10))
//    {
//        if(DobleGuardadoMsg())
//            return;
//    }

//    SNAnt = ui->MON_NSerie->text().toInt(&sig,10);
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

 //Carga datos DB
    QString Ingreso;
    Ingreso.clear();
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
                    "'"+ui->MON_FECHA_REP->text()+        "',"
                    "'"+ui->MON_REP_ID->text()+           "'"
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
        }

    ui->MON_BON->setCurrentIndex(0);
    dbReparaciones.ActualizarMonitores(*ui->MonitoresDatos,IdReparacion);

}

void Reparaciones::on_MON_BORRAR_clicked()
{
   BorraMonitores();
}

void Reparaciones::on_SEM_GUARDAR_clicked()
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
    if(!ui->SEM_TIPO->currentIndex())
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

     //--------------------------------------------------------------------------------
     //     Control de Fallas
     //--------------------------------------------------------------------------------
        indice = ui->SEM_FALLAS->rowCount();
        sig = false;
        Fallas.clear();
        for (i=0;i<indice;i++)
        {
            if(ui->SEM_FALLAS->item(i,0)->checkState() == 2)
            {
                if(sig)
                {
                    Fallas.append("-");
                }
                Fallas.append(ui->SEM_FALLAS->item(i,0)->text());

                sig = true;
            }
            ui->SEM_FALLAS->item(i,0)->setCheckState(Qt::Unchecked);
        }
        FactConf.append("TD:" + ui->SEM_TDES->text() + " TM:" + ui->SEM_TMED->text()
                      + " SA:" + ui->SEM_ACT->text());
        //Carga datos DB
        QString Ingreso;
        //   qDebug() <<"ID" << ui->MonRepID->text();
        //   qDebug ()<<"Frep" <<ui->MonFechaRep->text();
        Ingreso.clear();


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
                        "'"+ui->SEM_TIPO->currentText()+  "',"
                        "'"+ui->SEN_NSERIE->text()+     "',"
                        "'"+ui->SEN_FF->text()+         "',"
                        "'"+ui->SEN_FI->text()+         "',"
                        "'"+ui->SEN_VS->text()+         "',"
                        "'"+ui->SEN_FS->text()+         "',"
                        "'"+FactConf+                   "',"
                        "'"+Fallas+                     "',"
                        "'"+ui->SEN_BON->currentText()+   "',"
                        "'"+ui->SEM_COM->toPlainText()+   "',"
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
//                PerifericosActualizar();
            }
        }
       // ui->SEN_BON->setCurrentIndex(0);
        sig = false;

    Guardar = true;
    Siguiente = false;
//    NSerie = 0;
   dbReparaciones.ActualizarPerifericos(*ui->PerifericosDatos,IdReparacion);

}

void Reparaciones::on_SEM_BORRAR_clicked()
{
     BorraSensores();
}

void Reparaciones::on_MOD_GUARDAR_clicked()
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

      //  ui->SEN_BON->setCurrentIndex(0);
        sig = false;
    }
    Guardar = true;
    Siguiente = false;
//    NSerie = 0;
    dbReparaciones.ActualizarPerifericos(*ui->PerifericosDatos,IdReparacion);
}

void Reparaciones::on_MOD_BORRAR_clicked()
{
//    BorraMOD();
}

void Reparaciones::on_GPS_GUARDAR_clicked()
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
//                PerifericosActualizar();
            }
        }
     //   ui->SEN_BON->setCurrentIndex(0);
        sig = false;

    Guardar = true;
    Siguiente = false;
//    NSerie = 0;
    dbReparaciones.ActualizarPerifericos(*ui->PerifericosDatos,IdReparacion);
}

void Reparaciones::on_GPS_BORRAR_clicked()
{
      BorraGPS();
}

void Reparaciones::on_CAU_GUARDAR_clicked()
{
    QString Fallas, FactConf;
    bool sig,ok;
    int indice, i;
    int RepId;
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
    RepId = ui->PerRepID->text().toInt(&ok,10);
    SNAnt = ui->SEN_NSERIE->text().toInt(&sig,10);
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
    Ingreso.clear();

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

        }
    dbReparaciones.ActualizarCaudalimetro(*ui->CaudalimetroDatos,RepId);
    //   ui->SEN_BON->setCurrentIndex(0);
    //    sig = false;
    Guardar = true;
    Siguiente = false;
//    NSerie = 0;

}

void Reparaciones::on_CAU_BORRAR_clicked()
{

}

void Reparaciones::on_RPM_GUARDAR_clicked()
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

       // ui->SEN_BON->setCurrentIndex(0);
        sig = false;
    }
    Guardar = true;
    Siguiente = false;
    dbReparaciones.ActualizarPerifericos(*ui->PerifericosDatos,IdReparacion);
}

void Reparaciones::on_PANT_ANT_clicked()
{
    PantallaActual -- ;
    if((PantallaActual > 5) || (!PantallaActual))
        PantallaActual = 5;
    CambioPantalla(PantallaActual);
    ui->SEN_FS->setInputMask("00/00/00");
}

void Reparaciones::on_PANT_SIG_clicked()
{
    PantallaActual ++ ;
    if((PantallaActual > 5) || (!PantallaActual))
        PantallaActual = 0;
    CambioPantalla(PantallaActual);
    ui->SEN_FS->setInputMask("00/00/00");
}

void Reparaciones::on_SEM_TIPO_activated(const QString &arg1)
{
    QString Sensor;
    Sensor = arg1;
    qDebug () << Sensor << " - " << arg1;
    dbReparaciones.CargarFallas(*ui->SEM_FALLAS,Sensor);
}

void Reparaciones::on_INS_TIPO_activated(const QString &arg1)
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

void Reparaciones::on_INS_GUARDAR_clicked()
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
    //        qDebug() << "error:" << insertar.lastError();
            if(!insertar.exec())
            {
                qDebug() << "error:" << insertar.lastError();
                QMessageBox::critical(this,tr("Error en un campo"),
                                      tr("Camos incompletos no se guardaron los datos"));
            } 
        dbReparaciones.ActualizarInstalaciones(*ui->InstalacionesDatos,IdReparacion);
     //   ui->INS_BON->setCurrentIndex(0);
    }
}

void Reparaciones::CambioPantalla(int Pant)
{
    PantallaActual = Pant;
    switch(Pant)
    {
    //Pantalla de Semillas y Fertilizante
    case 1:
        ui->FSEM->setVisible(true);
        ui->FGPS->setVisible(false);
        ui->FROT->setVisible(false);
        ui->FMOD->setVisible(false);
        ui->FCAU->setVisible(false);
        ui->CaudalimetroDatos->setVisible(false);
        ui->PerifericosDatos->setVisible(true);
    break;
    case 2:
        ui->FSEM->setVisible(false);
        ui->FGPS->setVisible(true);
        ui->FROT->setVisible(false);
        ui->FMOD->setVisible(false);
        ui->FCAU->setVisible(false);
        ui->CaudalimetroDatos->setVisible(false);
        ui->PerifericosDatos->setVisible(true);
    break;
    case 3:
        ui->FSEM->setVisible(false);
        ui->FGPS->setVisible(false);
        ui->FROT->setVisible(true);
        ui->FMOD->setVisible(false);
        ui->FCAU->setVisible(false);
        ui->CaudalimetroDatos->setVisible(false);
        ui->PerifericosDatos->setVisible(true);
    break;
    case 4:
        ui->FSEM->setVisible(false);
        ui->FGPS->setVisible(false);
        ui->FROT->setVisible(false);
        ui->FMOD->setVisible(true);
        ui->FCAU->setVisible(false);
        ui->CaudalimetroDatos->setVisible(false);
        ui->PerifericosDatos->setVisible(true);
    break;
    case 5:
        ui->FSEM->setVisible(false);
        ui->FGPS->setVisible(false);
        ui->FROT->setVisible(false);
        ui->FMOD->setVisible(false);
        ui->FCAU->setVisible(true);
        ui->CaudalimetroDatos->setVisible(true);
        ui->PerifericosDatos->setVisible(false);
    break;
    }
}

void Reparaciones :: BonificacionMsg ()
{
    QMessageBox::information(this, tr("Bonificaciòn"),
                          tr("Falta cargar la bonificacion del trabajo"));
}
