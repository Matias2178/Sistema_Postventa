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
#include <QSqlQueryModel>
#include <reparacioneseditar.h>




dbManejo dbReparaciones;
QDateTime fReparaciones;

Reparaciones::Reparaciones(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Reparaciones)
{
    ui->setupUi(this);
//    ModTrabTablaRep = new QSqlRelationalTableModel(this,dbManejo::dbRetorna());
//    ModTrabTablaRep->setTable("Ingreso");
//    ModTrabTablaRep->select();
//    ModTrabTablaRep->setRelation();

//    FilTrabTablaRep = new QSortFilterProxyModel(this);
//    FilTrabTablaRep->setSourceModel(ModTrabTablaRep);
//    FilTrabTablaRep->setFilterCaseSensitivity(Qt::CaseInsensitive);
//    FilTrabTablaRep->setFilterKeyColumn(7);
// //   FilTrabTablaRep->setFilterRegExp();


//    ui->TrabajoTablaRep->setModel(FilTrabTablaRep);
//    ui->TrabajoTablaRep->hideColumn(0);


//    ModEquipos = new QSqlRelationalTableModel(this,dbManejo::dbRetorna());
//    ModEquipos->setTable("Productos");
//    ModEquipos->select();

//    FiltEquipos = new QSortFilterProxyModel(this);
//    FiltEquipos->setSourceModel(ModEquipos);
//    FiltEquipos->setFilterCaseSensitivity(Qt::CaseInsensitive);
//    FiltEquipos->setFilterKeyColumn(2); //-1 ordena por todas la columnas
//  //  FiltEquipos->filterAcceptsRow()

//    FiltEquipos->setFilterFixedString("2");
//    ui->TablaEquipo->setModel(FiltEquipos);
//    ui->TablaEquipo->hideColumn(0);
//    ui->TablaEquipo->hideColumn(3);
//    ui->TablaEquipo->sortByColumn(0,Qt::AscendingOrder);
//    ui->TablaEquipo->setSortingEnabled(true);

    ui->tabWidget->setCurrentIndex(0);
    CambioPantalla(1);
}

void Reparaciones::ActualizaDatos()
{
    dbReparaciones.CargarProd(*ui->Prod_Mon,1);
    dbReparaciones.CargarProd(*ui->Prod_Per,2);
    dbReparaciones.CargarProd(*ui->Prod_Ins,3);

    ui->SEN_FR->setInputMask("00/00/0000");
    ui->SEN_FR->setText(fReparaciones.currentDateTime().toString("ddMMyyyy"));

    ui->MON_REP_ID->setText(QString::number(IdReparacion,10));
    ui->PerRepID->setText(QString::number(IdReparacion,10));
    ui->InstRepID->setText(QString::number(IdReparacion,10));
    ui->PAgente->setText(AgenteResp);
    ui->MAgente->setText(AgenteResp);
    ui->IAgente->setText(AgenteResp);
    ui->MON_FECHA_REP->setInputMask("00/00/0000");
    ui->MON_FECHA_REP->setText(fReparaciones.currentDateTime().toString("ddMMyyyy"));
    ui->INS_FR->setInputMask("00/00/0000");
    ui->INS_FR->setText(fReparaciones.currentDateTime().toString("ddMMyyyy"));

    dbReparaciones.ActualizarMonitores(*ui->MonitoresDatos,IdReparacion);
    dbReparaciones.ActualizarCaudalimetro(*ui->CaudalimetroDatos,IdReparacion);
    dbReparaciones.ActualizarPerifericos(*ui->PerifericosDatos,IdReparacion);
    dbReparaciones.ActualizarInstalaciones(*ui->InstalacionesDatos,IdReparacion);
    BloquearBotones();
    CargarTrabajos();
}

Reparaciones::~Reparaciones()
{
    delete ui;
}


void Reparaciones::on_Prod_Mon_clicked(const QModelIndex &index)
{
    QString MonMascara;
    QString Mon;
    int fila;
    fila = index.row();
    Mon = ui->Prod_Mon->item(fila,0)->text();
    MonMascara = dbReparaciones.Mascara(Mon);

    ui->MON_VSOFT->clear();
    ui->MON_VSOFT->setInputMask(MonMascara);
    ui->MON_ACTSOFT->clear();
    ui->MON_ACTSOFT->setInputMask(MonMascara);
    dbReparaciones.CargarFallas(*ui->MON_FALLAS,Mon);
}

void Reparaciones::on_MON_GUARDAR_clicked()
{
    QString Info, Grupo;
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
    if(ui->Prod_Mon->currentRow()<0)
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
    Info.clear();
    Grupo.clear();
    indice = ui->MON_FALLAS->rowCount();
    sig = false;
    for (i=0;i<indice;i++)
    {
        if(ui->MON_FALLAS->item(i,0)->checkState() == 2)
        {
            if(sig)
            {
                Info.append(" ");
                Grupo.append(" ");
            }
            Info.append(ui->MON_FALLAS->item(i,0)->text());
            Grupo.append(ui->MON_FALLAS->item(i,2)->text());

            sig = true;
        }
        ui->MON_FALLAS->item(i,0)->setCheckState(Qt::Unchecked);
    }

 //Carga datos DB
    int fila = ui->Prod_Mon->currentIndex().row();
    QString Nombre;
    Nombre = ui->Prod_Mon->item(fila,0)->text();
    QString Ingreso;
    Ingreso.clear();
        QString Conf;
        Conf.clear();
        Conf.append("INSERT INTO Monitores("
                    "nombre,"
                    "sn,"
                    "vsoft,"
                    "actsoft,"
                    "falla,"
                    "bonif,"
                    "obs,"
                    "frep,"
                    "repid,"
                    "grupo)"
                    "VALUES("
                    "'"+Nombre+                         "',"
                    "'"+ui->MON_NSerie->text()+         "',"
                    "'"+ui->MON_VSOFT->text()+          "',"
                    "'"+ui->MON_ACTSOFT->text()+        "',"
                    "'"+Info+                           "',"
                    "'"+ui->MON_BON->currentText()+     "',"
                    "'"+ui->MON_COM->toPlainText()+     "',"
                    "'"+ui->MON_FECHA_REP->text()+      "',"
                    "'"+ui->MON_REP_ID->text()+         "',"
                    "'"+Grupo+                          "'"
                    ");");

        QSqlQuery insertar;

        if(!insertar.prepare(Conf))
        {
            QMessageBox::critical(this,tr("Error en un campo"),
                                      tr("Camos incompletos no se guardaron los datos"));
        }
        insertar.exec();
    ui->MON_BON->setCurrentIndex(0);
    dbReparaciones.ActualizarMonitores(*ui->MonitoresDatos,IdReparacion);

}

void Reparaciones::on_MON_BORRAR_clicked()
{
   dbReparaciones.BorrarItem("Monitores",IndEdicion);
   dbReparaciones.ActualizarMonitores(*ui->MonitoresDatos,IdReparacion);
   BloquearBotones();
}

void Reparaciones::on_MON_EDITAR_clicked()
{
    QString Conf;
    Conf.append("UPDATE Monitores SET "
                "nombre ="
                "'"+ui->MonitoresDatos->item(IndIndex,1)->text()+"',"
                "sn ="
                "'"+ui->MonitoresDatos->item(IndIndex,2)->text()+"',"
                "vsoft ="
                "'"+ui->MonitoresDatos->item(IndIndex,3)->text()+"',"
                "actsoft ="
                "'"+ui->MonitoresDatos->item(IndIndex,4)->text()+"',"
                "falla ="
                "'"+ui->MonitoresDatos->item(IndIndex,5)->text()+"',"
                "bonif ="
                "'"+ui->MonitoresDatos->item(IndIndex,6)->text()+"',"
                "obs ="
                "'"+ui->MonitoresDatos->item(IndIndex,7)->text()+"',"
                "frep ="
                "'"+ui->MonitoresDatos->item(IndIndex,8)->text()+"'"
//                "reid ="
//                "'"+ui->MonitoresDatos->item(IndIndex,8)->text()+"'"
                " WHERE id ="
                ""+QString::number(IndEdicion,10)+""
                "");
    QSqlQuery editar;
    if(!editar.prepare(Conf))
    {
        QMessageBox::critical(this,tr("Tabla Reparaciones"),
                              tr("Falla edicion de datos\n"
                                 "%1").arg(editar.lastError().text()));
    }

    editar.exec();

    dbReparaciones.ActualizarMonitores(*ui->MonitoresDatos,IdReparacion);
    BloquearBotones();
}

void Reparaciones::on_MonitoresDatos_clicked(const QModelIndex &index)
{
    bool ok;

    IndIndex = index.row();

    IndEdicion = ui->MonitoresDatos->item(index.row(),0)->text().toInt(&ok,10);
    ui->MON_EDITAR->setEnabled(true);
    ui->MON_BORRAR->setEnabled(true);
}

void Reparaciones::on_SEM_GUARDAR_clicked()
{
    QString Fallas, FactConf, Grupo;
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

    if (ui->Prod_Per->currentRow()<0) //Si no esta seleccionado
    {
        QMessageBox::critical(this, tr("Seleccion de Sensor"),
                              tr("Seleccionar Tipo de sensor antes de guardar"));
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
        indice = ui->PER_FALLAS->rowCount();
        sig = false;
        Fallas.clear();
        Grupo.clear();
        for (i=0;i<indice;i++)
        {
            if(ui->PER_FALLAS->item(i,0)->checkState() == 2)
            {
                if(sig)
                {
                    Fallas.append(" ");
                    Grupo.append(" ");
                }
                Fallas.append(ui->PER_FALLAS->item(i,0)->text());
                Grupo.append(ui->PER_FALLAS->item(i,2)->text());

                sig = true;
            }
            ui->PER_FALLAS->item(i,0)->setCheckState(Qt::Unchecked);
        }
        FactConf.append("TD:" + ui->SEM_TDES->text() + " -TM:" + ui->SEM_TMED->text()
                      + " -SA:" + ui->SEM_ACT->text());
        //Carga datos DB
        int fila = ui->Prod_Per->currentIndex().row();
        QString Nombre;
        Nombre = ui->Prod_Per->item(fila,0)->text();
        QString Ingreso;
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
                        "repid,"
                        "grupo)"
                        "VALUES("
                        "'"+Nombre+                     "',"
                        "'"+ui->SEN_NSERIE->text()+     "',"
                        "'"+ui->SEN_FF->text()+         "',"
                        "'"+ui->SEN_FI->text()+         "',"
                        "'"+ui->SEN_VS->text()+         "',"
                        "'"+ui->SEN_FS->text()+         "',"
                        "'"+FactConf+                   "',"
                        "'"+Fallas+                     "',"
                        "'"+ui->SEN_BON->currentText()+ "',"
                        "'"+ui->PER_COM->toPlainText()+ "',"
                        "'"+ui->SEN_FR->text()+         "',"
                        "'"+ui->PerRepID->text()+       "',"
                        "'"+Grupo+                      "'"
                        ");");

            QSqlQuery insertar;
            if(!insertar.prepare(Conf))
            {
                QMessageBox::critical(this,tr("Tabla Perifericos"),
                                      tr("Falla al crear la tabla\n"
                                     "%1").arg(insertar.lastError().text()));
            }
            insertar.exec();
        }
        sig = false;

    Guardar = true;
    Siguiente = false;
    dbReparaciones.ActualizarPerifericos(*ui->PerifericosDatos,IdReparacion);
    ui->checkBox->setChecked(false);
}

void Reparaciones::on_SEM_BORRAR_clicked()
{
      dbReparaciones.BorrarItem("Perifericos",IndEdicion);
     dbReparaciones.ActualizarPerifericos(*ui->PerifericosDatos,IdReparacion);
     BloquearBotones();
}

void Reparaciones::on_PerifericosDatos_clicked(const QModelIndex &index)
{
    bool ok;

    IndIndex = index.row();

    IndEdicion = ui->PerifericosDatos->item(index.row(),0)->text().toInt(&ok,10);

    ui->SEM_BORRAR->setEnabled(true);
    ui->SEM_EDITAR->setEnabled(true);

    ui->MOD_BORRAR->setEnabled(true);
    ui->MOD_EDITAR->setEnabled(true);

    ui->GPS_BORRAR->setEnabled(true);
    ui->GPS_EDITAR->setEnabled(true);

    ui->RPM_BORRAR->setEnabled(true);
    ui->RPM_EDITAR->setEnabled(true);

    ui->CAU_BORRAR->setEnabled(true);
    ui->CAU_EDITAR->setEnabled(true);
}

void Reparaciones::on_SEM_EDITAR_clicked()
{
    QString Indice;
    reparacioneseditar *VentanaEdicion  = new reparacioneseditar(this);
    VentanaEdicion->setModal(true);
    VentanaEdicion->show();
    Indice.append(ui->PerifericosDatos->item(IndIndex,0)->text());

    VentanaEdicion->SetDatos(2,Indice);

  //  EditarPerifericos();
  //  dbReparaciones.ActualizarPerifericos(*ui->PerifericosDatos,IdReparacion);
    BloquearBotones();

}

void Reparaciones::on_MOD_GUARDAR_clicked()
{
    QString Fallas, FactConf, Grupo;
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
    if (ui->Prod_Per->currentRow()<0) //Si no esta seleccionado
    {
        QMessageBox::critical(this, tr("Seleccion de Sensor"),
                              tr("Seleccionar Tipo de sensor antes de guardar"));
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
        indice = ui->PER_FALLAS->rowCount();
        sig = false;
        Fallas.clear();
        Grupo.clear();
        for (i=0;i<indice;i++)
        {
            if(ui->PER_FALLAS->item(i,0)->checkState() == 2)
            {
                if(sig)
                {
                    Fallas.append(" ");
                    Grupo.append(" ");
                }
                Fallas.append(ui->PER_FALLAS->item(i,0)->text());
                Grupo.append(ui->PER_FALLAS->item(i,2)->text());

                sig = true;
            }
            ui->PER_FALLAS->item(i,0)->setCheckState(Qt::Unchecked);
        }
        FactConf.append("FK:" + ui->MOD_FK->text()+ "DT:" + ui->MOD_DT->text()+"DD:" + ui->MOD_DD->text()
                        +"RT:" + ui->MOD_RT->text());
        //Carga datos DB
        int fila = ui->Prod_Per->currentIndex().row();
        QString Nombre;
        Nombre = ui->Prod_Per->item(fila,0)->text();
        QString Ingreso;
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
                        "repid,"
                        "grupo)"
                        "VALUES("
                        "'"+Nombre+                     "',"
                        "'"+ui->SEN_NSERIE->text()+     "',"
                        "'"+ui->SEN_FF->text()+         "',"
                        "'"+ui->SEN_FI->text()+         "',"
                        "'"+ui->SEN_VS->text()+         "',"
                        "'"+ui->SEN_FS->text()+         "',"
                        "'"+FactConf+                   "',"
                        "'"+Fallas+                     "',"
                        "'"+ui->SEN_BON->currentText()+ "',"
                        "'"+ui->PER_COM->toPlainText()+ "',"
                        "'"+ui->SEN_FR->text()+         "',"
                        "'"+ui->PerRepID->text()+       "',"
                        "'"+Grupo+                      "'"
                        ");");

            QSqlQuery insertar;
            if(!insertar.prepare(Conf))
            {
                QMessageBox::critical(this,tr("Tabla Prerifericos"),
                                      tr("Falla al crear la tabla\n"
                                     "%1").arg(insertar.lastError().text()));
            }
            insertar.exec();
        sig = false;
    }
    Guardar = true;
    Siguiente = false;
    dbReparaciones.ActualizarPerifericos(*ui->PerifericosDatos,IdReparacion);
}

void Reparaciones::on_MOD_BORRAR_clicked()
{
    dbReparaciones.BorrarItem("Perifericos",IndEdicion);
    dbReparaciones.ActualizarPerifericos(*ui->PerifericosDatos,IdReparacion);
    BloquearBotones();
}

void Reparaciones::on_MOD_EDITAR_clicked()
{
    QString Indice;
    reparacioneseditar *VentanaEdicion  = new reparacioneseditar(this);
    VentanaEdicion->setModal(true);
    VentanaEdicion->show();
    Indice.append(ui->PerifericosDatos->item(IndIndex,0)->text());

    VentanaEdicion->SetDatos(2,Indice);

  //  EditarPerifericos();
  //  dbReparaciones.ActualizarPerifericos(*ui->PerifericosDatos,IdReparacion);
    BloquearBotones();
}

void Reparaciones::on_CaudalimetroDatos_clicked(const QModelIndex &index)
{
    bool ok;

    IndIndex = index.row();

    IndEdicion = ui->CaudalimetroDatos->item(index.row(),0)->text().toInt(&ok,10);
    ui->CAU_BORRAR->setEnabled(true);
    ui->CAU_EDITAR->setEnabled(true);
}

void Reparaciones::on_GPS_GUARDAR_clicked()
{
    QString Fallas, FactConf, Grupo;
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
    if (ui->Prod_Per->currentRow()<0) //Si no esta seleccionado
    {
        QMessageBox::critical(this, tr("Seleccion de Sensor"),
                              tr("Seleccionar Tipo de sensor antes de guardar"));
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
        indice = ui->PER_FALLAS->rowCount();
        sig = false;
        Fallas.clear();
        Grupo.clear();
        for (i=0;i<indice;i++)
        {
            if(ui->PER_FALLAS->item(i,0)->checkState() == 2)
            {
                if(sig)
                {
                    Fallas.append(" ");
                    Grupo.append(" ");
                }
                Fallas.append(ui->PER_FALLAS->item(i,0)->text());
                Grupo.append(ui->PER_FALLAS->item(i,2)->text());

                sig = true;
            }
            ui->PER_FALLAS->item(i,0)->setCheckState(Qt::Unchecked);
        }
        FactConf.append(" ");
        //Carga datos DB
        int fila = ui->Prod_Per->currentIndex().row();
        QString Nombre;
        Nombre = ui->Prod_Per->item(fila,0)->text();
        QString Ingreso;
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
                    "repid,"
                    "grupo)"
                    "VALUES("
                    "'"+Nombre+                     "',"
                    "'"+ui->SEN_NSERIE->text()+     "',"
                    "'"+ui->SEN_FF->text()+         "',"
                    "'"+ui->SEN_FI->text()+         "',"
                    "'"+ui->SEN_VS->text()+         "',"
                    "'"+ui->SEN_FS->text()+         "',"
                    "'"+FactConf+                   "',"
                    "'"+Fallas+                     "',"
                    "'"+ui->SEN_BON->currentText()+ "',"
                    "'"+ui->PER_COM->toPlainText()+ "',"
                    "'"+ui->SEN_FR->text()+         "',"
                    "'"+ui->PerRepID->text()+       "',"
                    "'"+Grupo+                      "'"
                    ");");

        QSqlQuery insertar;
        if(!insertar.prepare(Conf))
        {
            QMessageBox::critical(this,tr("Tabla Perifericos"),
                                  tr("Falla al crear la tabla\n"
                                     "%1").arg(insertar.lastError().text()));
        }
        insertar.exec();
    }
    sig = false;

    Guardar = true;
    Siguiente = false;
    dbReparaciones.ActualizarPerifericos(*ui->PerifericosDatos,IdReparacion);
}

void Reparaciones::on_GPS_BORRAR_clicked()
{
      dbReparaciones.BorrarItem("Perifericos",IndEdicion);
      dbReparaciones.ActualizarPerifericos(*ui->PerifericosDatos,IdReparacion);
      BloquearBotones();
}

void Reparaciones::on_GPS_EDITAR_clicked()
{
    ui->GPS_EDITAR->setEnabled(false);
    EditarPerifericos();
    dbReparaciones.ActualizarPerifericos(*ui->PerifericosDatos,IdReparacion);
    BloquearBotones();
}

void Reparaciones::on_CAU_GUARDAR_clicked()
{
    QString Fallas, Grupo;
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
    if (ui->Prod_Per->currentRow()<0) //Si no esta seleccionado
    {
        QMessageBox::critical(this, tr("Seleccion de Sensor"),
                              tr("Seleccionar Tipo de sensor antes de guardar"));
    }
    RepId = ui->PerRepID->text().toInt(&ok,10);
    SNAnt = ui->SEN_NSERIE->text().toInt(&sig,10);
    //--------------------------------------------------------------------------------
    //     Control de Fallas
    //--------------------------------------------------------------------------------
    indice = ui->PER_FALLAS->rowCount();
    sig = false;
    Fallas.clear();
    Grupo.clear();
    for (i=0;i<indice;i++)
    {
        if(ui->PER_FALLAS->item(i,0)->checkState() == 2)
        {
            if(sig)
            {
                Fallas.append(" ");
                Grupo.append(" ");
            }
            Fallas.append(ui->PER_FALLAS->item(i,0)->text());
            Grupo.append(ui->PER_FALLAS->item(i,2)->text());

            sig = true;
            qDebug () << "grupo:" << Grupo;
        }
        ui->PER_FALLAS->item(i,0)->setCheckState(Qt::Unchecked);
    }

    //Carga datos DB
    int fila = ui->Prod_Per->currentIndex().row();
    QString Nombre;
    Nombre = ui->Prod_Per->item(fila,0)->text();
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
                    "repid,"
                    "grupo)"
                    "VALUES("
                    "'"+Nombre+                     "',"
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
                    "'"+ui->PER_COM->toPlainText()+ "',"
                    "'"+ui->SEN_FR->text()+         "',"
                    "'"+ui->PerRepID->text()+       "',"
                    "'"+Grupo+                      "'"
                    ");");

        QSqlQuery insertar;
        if(!insertar.prepare(Conf))
        {
            QMessageBox::critical(this,tr("Tabla Caudalimetro"),
                                  tr("Falla al crear la tabla\n"
                                 "%1").arg(insertar.lastError().text()));
        }
        insertar.exec();
    dbReparaciones.ActualizarCaudalimetro(*ui->CaudalimetroDatos,RepId);
    //   ui->SEN_BON->setCurrentIndex(0);
    //    sig = false;
    Guardar = true;
    Siguiente = false;
//    NSerie = 0;

}

void Reparaciones::on_CAU_BORRAR_clicked()
{
    dbReparaciones.BorrarItem("Caudalimetro",IndEdicion);
    dbReparaciones.ActualizarCaudalimetro(*ui->CaudalimetroDatos,IdReparacion);
    BloquearBotones();
}

void Reparaciones::on_CAU_EDITAR_clicked()
{
    QString Conf;
    QString Cong;
    Conf.append("UPDATE Caudalimetro SET "
                "nombre ="
                "'"+ui->CaudalimetroDatos->item(IndIndex,1)->text()+"',"
                "sn ="
                "'"+ui->CaudalimetroDatos->item(IndIndex,2)->text()+"',"
                "movil ="
                "'"+ui->CaudalimetroDatos->item(IndIndex,3)->text()+"',"
                "ffab ="
                "'"+ui->CaudalimetroDatos->item(IndIndex,4)->text()+"',"
                "finst ="
                "'"+ui->CaudalimetroDatos->item(IndIndex,5)->text()+"',"
                "vsoft ="
                "'"+ui->CaudalimetroDatos->item(IndIndex,6)->text()+"',"
                "fsoft ="
                "'"+ui->CaudalimetroDatos->item(IndIndex,7)->text()+"',"
                "tmt ="
                "'"+ui->CaudalimetroDatos->item(IndIndex,8)->text()+"',"
                "cct ="
                "'"+ui->CaudalimetroDatos->item(IndIndex,9)->text()+"',"
                "desc ="
                "'"+ui->CaudalimetroDatos->item(IndIndex,10)->text()+"',"
                "descat ="
                "'"+ui->CaudalimetroDatos->item(IndIndex,11)->text()+"',"
                "cbmag ="
                "'"+ui->CaudalimetroDatos->item(IndIndex,12)->text()+"',");
    Cong.append("tbmag ="
                "'"+ui->CaudalimetroDatos->item(IndIndex,13)->text()+"',"
                "falla ="
                "'"+ui->CaudalimetroDatos->item(IndIndex,14)->text()+"',"
                "bonif ="
                "'"+ui->CaudalimetroDatos->item(IndIndex,15)->text()+"',"
                "obs ="
                "'"+ui->CaudalimetroDatos->item(IndIndex,16)->text()+"',"
                "frep ="
                "'"+ui->CaudalimetroDatos->item(IndIndex,17)->text()+"'"
                " WHERE id ="
                ""+QString::number(IndEdicion,10)+"");

    QSqlQuery editar;
    if(!editar.prepare(Conf))
    {
        QMessageBox::critical(this,tr("Tabla Caudalimetro"),
                              tr("Falla edicion de datos\n"
                                 "%1").arg(editar.lastError().text()));
    }
    editar.exec();
    dbReparaciones.ActualizarCaudalimetro(*ui->CaudalimetroDatos,IdReparacion);
    BloquearBotones();
}

void Reparaciones::on_RPM_GUARDAR_clicked()
{
    QString Fallas, FactConf, Grupo;
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
    if (ui->Prod_Per->currentRow()<0) //Si no esta seleccionado
    {
        QMessageBox::critical(this, tr("Seleccion de Sensor"),
                              tr("Seleccionar Tipo de sensor antes de guardar"));
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
        indice = ui->PER_FALLAS->rowCount();
        sig = false;
        Fallas.clear();
        Grupo.clear();
        for (i=0;i<indice;i++)
        {
            if(ui->PER_FALLAS->item(i,0)->checkState() == 2)
            {
                if(sig)
                {
                    Fallas.append(" ");
                    Grupo.append(" ");
                }
                Fallas.append(ui->PER_FALLAS->item(i,0)->text());
                Grupo.append(ui->PER_FALLAS->item(i,2)->text());
                sig = true;
            }
            ui->PER_FALLAS->item(i,0)->setCheckState(Qt::Unchecked);
        }
        FactConf.append("FK:" + ui->RPM_FK->text());
        //Carga datos DB
        int fila = ui->Prod_Per->currentIndex().row();
        QString Nombre;
        Nombre = ui->Prod_Per->item(fila,0)->text();
        QString Ingreso;
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
                        "repid,"
                        "grupo)"
                        "VALUES("
                        "'"+Nombre+                     "',"
                        "'"+ui->SEN_NSERIE->text()+     "',"
                        "'"+ui->SEN_FF->text()+         "',"
                        "'"+ui->SEN_FI->text()+         "',"
                        "'"+ui->SEN_VS->text()+         "',"
                        "'"+ui->SEN_FS->text()+         "',"
                        "'"+FactConf+                   "',"
                        "'"+Fallas+                     "',"
                        "'"+ui->SEN_BON->currentText()+ "',"
                        "'"+ui->PER_COM->toPlainText()+ "',"
                        "'"+ui->SEN_FR->text()+         "',"
                        "'"+ui->PerRepID->text()+       "',"
                        "'"+Grupo+                      "'"
                        ");");

            QSqlQuery insertar;
            if(!insertar.prepare(Conf))
            {
                QMessageBox::critical(this,tr("Tabla Perifericos"),
                                      tr("Falla al crear la tabla\n"
                                     "%1").arg(insertar.lastError().text()));
            }
            insertar.exec();

       // ui->SEN_BON->setCurrentIndex(0);
        sig = false;
    }
    Guardar = true;
    Siguiente = false;
    dbReparaciones.ActualizarPerifericos(*ui->PerifericosDatos,IdReparacion);
}

void Reparaciones::on_RPM_BORRAR_clicked()
{
    dbReparaciones.BorrarItem("Perifericos",IndEdicion);
    dbReparaciones.ActualizarPerifericos(*ui->PerifericosDatos,IdReparacion);
    BloquearBotones();
}

void Reparaciones::on_RPM_EDITAR_clicked()
{
    QString Indice;
    reparacioneseditar *VentanaEdicion  = new reparacioneseditar(this);
    VentanaEdicion->setModal(true);
    VentanaEdicion->show();
    Indice.append(ui->PerifericosDatos->item(IndIndex,0)->text());

    VentanaEdicion->SetDatos(2,Indice);

  //  EditarPerifericos();
  //  dbReparaciones.ActualizarPerifericos(*ui->PerifericosDatos,IdReparacion);
    BloquearBotones();
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
        PantallaActual = 1;
    CambioPantalla(PantallaActual);
    ui->SEN_FS->setInputMask("00/00/00");
}

void Reparaciones::on_Prod_Per_clicked(const QModelIndex &index)
{
    QString Sensor;
    int fil = index.row();
    Sensor = ui->Prod_Per->item(fil,0)->text();
    dbReparaciones.CargarFallas(*ui->PER_FALLAS,Sensor);
}

void Reparaciones::on_Prod_Ins_clicked(const QModelIndex &index)
{
    QString Sensor;
    int fil = index.row();
    Sensor = ui->Prod_Ins->item(fil,0)->text();
    dbReparaciones.CargarFallas(*ui->INS_FALLAS,Sensor);
}

void Reparaciones::on_InstalacionesDatos_clicked(const QModelIndex &index)
{
    bool ok;

    IndIndex = index.row();

    IndEdicion = ui->InstalacionesDatos->item(index.row(),0)->text().toInt(&ok,10);
    ui->INS_BORRAR->setEnabled(true);
    ui->INS_EDITAR->setEnabled(true);
}

void Reparaciones::on_INS_EDITAR_clicked()
{
    QString Conf;
    Conf.append("UPDATE Instalaciones SET "
                "nombre ="
                "'"+ui->InstalacionesDatos->item(IndIndex,1)->text()+"',"
                "sn ="
                "'"+ui->InstalacionesDatos->item(IndIndex,2)->text()+"',"
                "falla ="
                "'"+ui->InstalacionesDatos->item(IndIndex,3)->text()+"',"
                "bonif ="
                "'"+ui->InstalacionesDatos->item(IndIndex,4)->text()+"',"
                "obs ="
                "'"+ui->InstalacionesDatos->item(IndIndex,5)->text()+"',"
                "frep ="
                "'"+ui->InstalacionesDatos->item(IndIndex,6)->text()+"'"
                " WHERE id ="
                ""+QString::number(IndEdicion,10)+""
                "");
    QSqlQuery editar;
    if(!editar.prepare(Conf))
    {
        QMessageBox::critical(this,tr("Tabla Instalaciones"),
                              tr("Falla edicion de datos\n"
                                 "%1").arg(editar.lastError().text()));
    }
    editar.exec();
    BloquearBotones();
    dbReparaciones.ActualizarInstalaciones(*ui->InstalacionesDatos,IdReparacion);
}

void Reparaciones::on_INS_BORRAR_clicked()
{
    dbReparaciones.BorrarItem("Instalaciones",IndEdicion);
    dbReparaciones.ActualizarInstalaciones(*ui->InstalacionesDatos,IdReparacion);
    BloquearBotones();
}

void  Reparaciones::on_RepTrabajo_clicked(const QModelIndex &index)
{
    QString Equipo, Mascara;
    int indice;
    int tipo;
    indice = index.row();
    Equipo.clear();
    Equipo.append(ui->RepTrabajo->item(indice,1)->text());
    tipo = dbReparaciones.BucaEquipo(Equipo);
//    QString aaa = QString::number(tipo);
//    QMessageBox::critical(this,tr("Tabla Instalaciones"),
//                          tr("Forro\n %1  %2").arg(Equipo).arg(aaa));
    if(tipo == 1)
    {
        int i;
        ui->tabWidget->setCurrentIndex(0);
        ui->MON_NSerie->clear();
        ui->MON_NSerie->setText(ui->RepTrabajo->item(indice,3)->text());

        ui->MON_VSOFT->clear();
        Mascara = dbReparaciones.Mascara(Equipo);
        ui->MON_VSOFT->setInputMask(Mascara);
        ui->MON_ACTSOFT->setInputMask(Mascara);
        dbReparaciones.CargarFallas(*ui->MON_FALLAS,Equipo);

        int fil = ui->Prod_Mon->rowCount();
        for (i=0;i<=fil;i++)
        {
            if(Equipo == ui->Prod_Mon->item(i,0)->text())
            {
                ui->Prod_Mon->selectRow(i);
                break;
            }
        }
    }
    else if (tipo == 2)
    {
        int fil;
        int i;
        ui->tabWidget->setCurrentIndex(1);
        dbReparaciones.CargarFallas(*ui->PER_FALLAS,Equipo);
        fil = ui->Prod_Per->rowCount();
        for (i=0;i<=fil;i++)
        {
            if(Equipo == ui->Prod_Per->item(i,0)->text())
            {
                ui->Prod_Per->selectRow(i);

                break;
            }
        }
    }
    else if (tipo == 3)
    {
        int fil;
        int i;
        ui->tabWidget->setCurrentIndex(2);
        dbReparaciones.CargarFallas(*ui->INS_FALLAS,Equipo);

        fil = ui->Prod_Ins->rowCount();

        for (i=0;i<=fil;i++)
        {
            if(Equipo == ui->Prod_Ins->item(i,0)->text())
            {
                ui->Prod_Ins->selectRow(i);

                break;
            }
        }
    }
}

void Reparaciones::on_INS_GUARDAR_clicked()
{
    QString Fallas, Grupo;
    bool sig;
    int indice, i;

    if(ui->InstRepID->text().isEmpty())
    {
        MensajeTrabajo();
        return;
    }

    if(!ui->Prod_Ins->currentRow()<0)
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
        Grupo.clear();
        for (i=0;i<indice;i++)
        {
            if(ui->INS_FALLAS->item(i,0)->checkState() == 2)
            {
                if(sig)
                {
                    Fallas.append(" ");
                    Grupo.append(" ");
                }
                Fallas.append(ui->INS_FALLAS->item(i,0)->text());
                Grupo.append(ui->INS_FALLAS->item(i,2)->text());

                sig = true;
            }
            ui->INS_FALLAS->item(i,0)->setCheckState(Qt::Unchecked);
        }
//Carga datos DB
        int fila = ui->Prod_Ins->currentIndex().row();
        QString Nombre;
        Nombre = ui->Prod_Ins->item(fila,0)->text();
        QString Ingreso;
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
                    "repid,"
                    "grupo)"
                    "VALUES("
                    "'"+Nombre+                     "',"
                    "'"+ui->INS_NSerie->text()+     "',"
                    "'"+Fallas+                     "',"
                    "'"+ui->INS_BON->currentText()+ "',"
                    "'"+ui->INS_COM->toPlainText()+ "',"
                    "'"+ui->INS_FR->text()+         "',"
                    "'"+ui->InstRepID->text()+      "',"
                    "'"+Grupo+                      "'"
                    ");");

        QSqlQuery insertar;
        if(!insertar.prepare(Conf))
        {
            QMessageBox::critical(this,tr("Tabla Instalaciones"),
                                  tr("Falla al crear la tabla\n"
                                     "%1").arg(insertar.lastError().text()));
        }
        insertar.exec();
        dbReparaciones.ActualizarInstalaciones(*ui->InstalacionesDatos,IdReparacion);
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

void Reparaciones::BonificacionMsg ()
{
    QMessageBox::information(this, tr("Bonificaciòn"),
                          tr("Falta cargar la bonificacion del trabajo"));
}

void Reparaciones::EditarPerifericos()
{
    QString Conf;
    Conf.append("UPDATE Perifericos SET "
                "nombre ="
                "'"+ui->PerifericosDatos->item(IndIndex,1)->text()+"',"
                "sn ="
                "'"+ui->PerifericosDatos->item(IndIndex,2)->text()+"',"
                "ffab ="
                "'"+ui->PerifericosDatos->item(IndIndex,3)->text()+"',"
                "finst ="
                "'"+ui->PerifericosDatos->item(IndIndex,4)->text()+"',"
                "vsoft ="
                "'"+ui->PerifericosDatos->item(IndIndex,5)->text()+"',"
                "fsoft ="
                "'"+ui->PerifericosDatos->item(IndIndex,6)->text()+"',"
                "conf ="
                "'"+ui->PerifericosDatos->item(IndIndex,7)->text()+"',"
                "falla ="
                "'"+ui->PerifericosDatos->item(IndIndex,8)->text()+"',"
                "bonif ="
                "'"+ui->PerifericosDatos->item(IndIndex,9)->text()+"',"
                "obs ="
                "'"+ui->PerifericosDatos->item(IndIndex,10)->text()+"',"
                "frep ="
                "'"+ui->PerifericosDatos->item(IndIndex,11)->text()+"'"
                " WHERE id ="
                ""+QString::number(IndEdicion,10)+""
                "");
    QSqlQuery editar;
    if(!editar.prepare(Conf))
    {
        QMessageBox::critical(this,tr("Tabla Perifericos"),
                              tr("Falla edicion de datos\n"
                                 "%1").arg(editar.lastError().text()));
    }
    editar.exec();
}
void Reparaciones::BloquearBotones()
{
    ui->SEM_BORRAR->setEnabled(false);
    ui->SEM_EDITAR->setEnabled(false);

    ui->MOD_BORRAR->setEnabled(false);
    ui->MOD_EDITAR->setEnabled(false);

    ui->GPS_BORRAR->setEnabled(false);
    ui->GPS_EDITAR->setEnabled(false);

    ui->RPM_BORRAR->setEnabled(false);
    ui->RPM_EDITAR->setEnabled(false);

    ui->CAU_BORRAR->setEnabled(false);
    ui->CAU_EDITAR->setEnabled(false);

    ui->MON_BORRAR->setEnabled(false);
    ui->MON_EDITAR->setEnabled(false);

}

void Reparaciones::CargarTrabajos()
{
    QString Conf;
    int ID;
    Conf.append("SELECT * FROM Ingreso");
    ID = IdReparacion;
    QSqlQuery consultar;
    if(!consultar.prepare(Conf))
    {
        QMessageBox::critical(this,tr("Tabla Ingreso"),
                              tr("Falla carga de datos"));
    }
    consultar.exec();
    int fila  = 0;

    ui->RepTrabajo->setRowCount(0);

    while(consultar.next())
    {
        if(ID == consultar.value("repid").toByteArray().toInt())
        {
            ui->RepTrabajo->insertRow(fila);
            ui->RepTrabajo->setRowHeight(fila,20);
            ui->RepTrabajo->setItem(fila,0,new QTableWidgetItem (consultar.value("cant").toByteArray().constData()));
            ui->RepTrabajo->setItem(fila,1,new QTableWidgetItem (consultar.value("nombre").toByteArray().constData()));
            ui->RepTrabajo->setItem(fila,2,new QTableWidgetItem (consultar.value("desc").toByteArray().constData()));
            ui->RepTrabajo->setItem(fila,3,new QTableWidgetItem (consultar.value("sn").toByteArray().constData()));
            ui->RepTrabajo->setItem(fila,4,new QTableWidgetItem (consultar.value("obs").toByteArray().constData()));
            fila ++;
        }
    }
    ui->RepTrabajo->setColumnWidth(0,40);
    ui->RepTrabajo->setColumnWidth(1,100);
    ui->RepTrabajo->setColumnWidth(2,120);
    ui->RepTrabajo->setColumnWidth(3,40);
    ui->RepTrabajo->setColumnWidth(4,120);

    ui->RepTrabajo->scrollToBottom();
}

void Reparaciones::MensajeTrabajo()
{
    QMessageBox::information(this,tr("Trabajo"),
                             tr("Seleccionar trabajo para cargar datos"));
}

bool Reparaciones::DobleGuardadoMsg()
{
    QMessageBox dGuardado;
    dGuardado.setText("Este equipo ya se guardo");
    dGuardado.setInformativeText("Quiere guardar de todas formas??");
    dGuardado.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    int elegido = dGuardado.exec();
    switch (elegido)
    {
        case QMessageBox::Ok:
            return false;

    }
}

