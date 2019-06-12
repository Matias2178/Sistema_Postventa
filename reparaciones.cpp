#include "mainwindow.h"
#include "reparaciones.h"
#include "ui_reparaciones.h"
#include <QMessageBox>
#include <variables.h>
#include <dbmanejo.h>
#include <QDebug>
#include <QTimer>
#include <QSqlQueryModel>

Reparaciones::Reparaciones(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Reparaciones)
{
    ui->setupUi(this);

    ModTrabTablaRep = new QSqlRelationalTableModel(this,dbManejo::dbRetorna());
    ModTrabTablaRep->setTable("Ingreso");
    ModTrabTablaRep->select();
//    ModTrabTablaRep->setRelation();

    FilTrabTablaRep = new QSortFilterProxyModel(this);
    FilTrabTablaRep->setSourceModel(ModTrabTablaRep);
    FilTrabTablaRep->setFilterCaseSensitivity(Qt::CaseInsensitive);
    FilTrabTablaRep->setFilterKeyColumn(7);


    ui->TrabajoTablaRep->setModel(FilTrabTablaRep);
    ui->TrabajoTablaRep->hideColumn(0);


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

    QSqlQuery consultar;
    consultar.prepare("SELECT * FROM Conceptos");
    if(!consultar.exec())
    {
    QMessageBox::information(this,tr("Seleccion Conceptos"),
                             tr("Seleccionar Trabajo"));
    }
    while(consultar.next()){
        ui->MonConcepto->addItem(consultar.value("concepto").toString());
        ui->PerConcepto->addItem(consultar.value("concepto").toString());
        ui->InstConcepto->addItem(consultar.value("concepto").toString());
        ui->InsumosConcepto->addItem(consultar.value("concepto").toString());
    }
    ui->MonCantidad->setText("1");
    ui->SenCantidad->setText("1");
    ui->InstCantidad->setText("1");
    ui->InsumosCantidad->setText("1");
    ui->tabWidget->setCurrentIndex(0);
    CambioPantalla(1);

    QStringList Etiquetas {"ID", "Cant","Codigo","Nombre","Falla", "Grupo","Bonif", "Conc","Observaciones","FRep","RepID"};
  //  ui->TablaNP->insertColumn(1);
    ui->InsumosDatos->setHorizontalHeaderLabels(Etiquetas);
}


void Reparaciones::ActualizaDatos()
{
    FilTrabTablaRep->setFilterFixedString(QString::number(IdReparacion,10));

 //   qDebug() << QString::number(IdReparacion,10) << IdReparacion;

    dbReparaciones.CargarProd(*ui->Prod_Mon,1);
    dbReparaciones.CargarProd(*ui->Prod_Per,2);
    dbReparaciones.CargarProd(*ui->Prod_Ins,3);
    dbReparaciones.CargarProd(*ui->ProdInsumos,4);

    ui->SEN_FR->setInputMask("00/00/0000");
    ui->SEN_FR->setText(fReparaciones.currentDateTime().toString("ddMMyyyy"));

    ui->MON_REP_ID->setText(QString::number(IdReparacion,10));
    ui->PerRepID->setText(QString::number(IdReparacion,10));
    ui->InstRepID->setText(QString::number(IdReparacion,10));
    ui->InsumosRepId->setText(QString::number(IdReparacion,10));
    ui->PAgente->setText(AgenteResp);
    ui->MAgente->setText(AgenteResp);
    ui->IAgente->setText(AgenteResp);
    ui->InsumosAgente->setText(AgenteResp);
    ui->MON_FECHA_REP->setInputMask("00/00/0000");
    ui->MON_FECHA_REP->setText(fReparaciones.currentDateTime().toString("ddMMyyyy"));
    ui->INS_FR->setInputMask("00/00/0000");
    ui->INS_FR->setText(fReparaciones.currentDateTime().toString("ddMMyyyy"));
    ui->InsumosFechaRep->setInputMask("00/00/0000");
    ui->InsumosFechaRep->setText(fReparaciones.currentDateTime().toString("ddMMyyyy"));

    dbReparaciones.ActualizarMonitores(*ui->MonitoresDatos,IdReparacion);
    dbReparaciones.ActualizarCaudalimetro(*ui->CaudalimetroDatos,IdReparacion);
    dbReparaciones.ActualizarPerifericos(*ui->PerifericosDatos,IdReparacion);
    dbReparaciones.ActualizarInstalaciones(*ui->InstalacionesDatos,IdReparacion);
    dbReparaciones.ActualizarInsumos(*ui->InsumosDatos,IdReparacion);
    BloquearBotones();
    CargarTrabajos();
}

void Reparaciones::ActDatos()
{
    dbReparaciones.ActualizarMonitores(*ui->MonitoresDatos,IdReparacion);
    dbReparaciones.ActualizarCaudalimetro(*ui->CaudalimetroDatos,IdReparacion);
    dbReparaciones.ActualizarPerifericos(*ui->PerifericosDatos,IdReparacion);
    dbReparaciones.ActualizarInstalaciones(*ui->InstalacionesDatos,IdReparacion);
    dbReparaciones.ActualizarInsumos(*ui->InsumosDatos,IdReparacion);
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
    QList <QString> lFallas;
    QString Nombre, concep;
    bool ok;

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

    Nombre = ui->Prod_Mon->item(ui->Prod_Mon->currentIndex().row(),0)->text();
//Controla si el sensor ya fue guardado en este trabajo, compara Nombre Numero de Serie e ID del trabajo
    if(MonDuplicado(Nombre, ui->MON_NSerie->text(),ui->MON_REP_ID->text()))
        return;

    if (!ui->MON_BON->currentIndex()|| !ui->MonConcepto->currentIndex())
    {
        BonificacionMsg();
        return;
    }

    //--------------------------------------------------------------------------------
    //     Control de Fallas
    //--------------------------------------------------------------------------------
    Info.clear();
    Grupo.clear();

    lFallas << tFallas.GetFallas(*ui->MON_FALLAS);
    Info.append(lFallas[0]);
    Grupo.append(lFallas[1]);

 //Carga datos DB
    concep.clear();
    concep.append(QString::number(ui->MonConcepto->currentIndex()));
qDebug () << concep;
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
                "grupo,"
                "cantidad,"
                "concepto)"
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
                "'"+Grupo+                          "',"
                "'"+ui->MonCantidad->text()+    "',"
                "'"+concep+   "'"
                ");");

    QSqlQuery insertar;
    insertar.prepare(Conf);
    if(!insertar.exec())
    {
        QMessageBox::critical(this,tr("Error en un campo"),
                              tr("Camos incompletos no se guardaron los datos"));
    }

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
    QString Indice;
    reparacioneseditar *VentanaEdicion  = new reparacioneseditar(this);
    VentanaEdicion->setModal(true);
    VentanaEdicion->show();
    Indice.append(ui->MonitoresDatos->item(IndIndex,0)->text());

    VentanaEdicion->SetDatos(1,Indice);

    connect (VentanaEdicion, SIGNAL(finalizar()),this, SLOT(ActDatos())  );
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
    QString Fallas, Grupo, FactConf;
    QList <QString> lFallas;
    bool sig;
    QString Nombre, concep;

    if(ui->PerRepID->text().isEmpty())
    {
        MensajeTrabajo();
        return;
    }
    if (ui->Prod_Per->currentRow()<0) //Si no esta seleccionado
    {
        QMessageBox::critical(this, tr("Seleccion de Sensor"),
                              tr("Seleccionar Tipo de sensor antes de guardar"));
        return;
    }
    int fila = ui->Prod_Per->currentIndex().row();
    Nombre = ui->Prod_Per->item(fila,0)->text();
//Controla si el sensor ya fue guardado en este trabajo, compara Nombre Numero de Serie e ID del trabajo
    if(SenDuplicado(Nombre, ui->SEN_NSERIE->text(),ui->PerRepID->text()))
        return;

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
        Fallas.clear();
        Grupo.clear();
        lFallas << tFallas.GetFallas(*ui->PER_FALLAS);

        Fallas.append(lFallas[0]);
        Grupo.append(lFallas[1]);

        FactConf.append("TD:" + ui->SEM_TDES->text() + " -TM:" + ui->SEM_TMED->text()
                        + " -SA:" + ui->SEM_ACT->text());
        //Carga datos DB
        concep.clear();
        concep.append(QString::number(ui->PerConcepto->currentIndex()));
qDebug () << concep;

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
                    "grupo,"
                    "cantidad,"
                    "concepto)"
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
                    "'"+Grupo+                      "',"
                    "'"+ui->SenCantidad->text()+    "',"
                    "'"+concep+"'"
                    ");");

        QSqlQuery insertar;
        insertar.prepare(Conf);
        if(!insertar.exec())
        {
            QMessageBox::critical(this,tr("Tabla Perifericos"),
                                  tr("Falla al crear la tabla\n"
                                     "%1").arg(insertar.lastError().text()));
        }
    }

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

    connect (VentanaEdicion, SIGNAL(finalizar()),this, SLOT(ActDatos())  );
    BloquearBotones();

}

void Reparaciones::on_MOD_GUARDAR_clicked()
{
    QString Fallas, FactConf, Grupo;
    QString Nombre, concep;
    QList <QString> lFallas;
    bool sig;

    if(ui->PerRepID->text().isEmpty())
    {
        MensajeTrabajo();
        return;
    }

    if (ui->Prod_Per->currentRow()<0) //Si no esta seleccionado
    {
        QMessageBox::critical(this, tr("Seleccion de Sensor"),
                              tr("Seleccionar Tipo de sensor antes de guardar"));
        return;
    }
    int fila = ui->Prod_Per->currentIndex().row();
    Nombre = ui->Prod_Per->item(fila,0)->text();

//Controla si el sensor ya fue guardado en este trabajo, compara Nombre Numero de Serie e ID del trabajo
    if(SenDuplicado(Nombre, ui->SEN_NSERIE->text(),ui->PerRepID->text()))
        return;

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
        Fallas.clear();
        Grupo.clear();
        lFallas << tFallas.GetFallas(*ui->PER_FALLAS);
        Fallas.append(lFallas[0]);
        Grupo.append(lFallas[1]);

        FactConf.append("FK:" + ui->MOD_FK->text()+ "DT:" + ui->MOD_DT->text()+"DD:" + ui->MOD_DD->text()
                        +"RT:" + ui->MOD_RT->text());
        //Carga datos DB

        concep.clear();
        concep.append(QString::number(ui->PerConcepto->currentIndex()));
qDebug () << concep;
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
                    "grupo,"
                    "cantidad,"
                    "concepto)"
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
                    "'"+Grupo+                      "',"
                    "'"+ui->SenCantidad->text()+    "',"
                    "'"+concep+                     "'"
                    ");");

        QSqlQuery insertar;

        insertar.prepare(Conf);
        if(!insertar.exec())
        {
            QMessageBox::critical(this,tr("Tabla Prerifericos"),
                                  tr("Falla al crear la tabla\n"
                                     "%1").arg(insertar.lastError().text()));
        }

    }
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

    connect (VentanaEdicion, SIGNAL(finalizar()),this, SLOT(ActDatos())  );
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
    QList <QString> lFallas;
    QString Nombre, concep;
    bool sig;

    if(ui->PerRepID->text().isEmpty())
    {
        MensajeTrabajo();
        return;
    }
    if (ui->Prod_Per->currentRow()<0) //Si no esta seleccionado
    {
        QMessageBox::critical(this, tr("Seleccion de Sensor"),
                              tr("Seleccionar Tipo de sensor antes de guardar"));
        return;
    }
    int fila = ui->Prod_Per->currentIndex().row();
    Nombre = ui->Prod_Per->item(fila,0)->text();

//Controla si el sensor ya fue guardado en este trabajo, compara Nombre Numero de Serie e ID del trabajo
    if(SenDuplicado(Nombre, ui->SEN_NSERIE->text(),ui->PerRepID->text()))
        return;

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
        Fallas.clear();
        Grupo.clear();
        lFallas << tFallas.GetFallas(*ui->PER_FALLAS);

        Fallas.append(lFallas[0]);
        Grupo.append(lFallas[1]);

        FactConf.append(" ");
        //Carga datos DB
        concep.clear();
        concep.append(QString::number(ui->PerConcepto->currentIndex()));
qDebug () << concep;
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
                    "grupo,"
                    "cantidad,"
                    "concepto)"
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
                    "'"+Grupo+                      "',"
                    "'"+ui->SenCantidad->text()+    "',"
                    "'"+concep+"'"
                    ");");

        QSqlQuery insertar;
        insertar.prepare(Conf);
        if(!insertar.exec())
        {
            QMessageBox::critical(this,tr("Tabla Perifericos"),
                                  tr("Falla al crear la tabla\n"
                                     "%1").arg(insertar.lastError().text()));
        }

    }
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
    QString Indice;
    reparacioneseditar *VentanaEdicion  = new reparacioneseditar(this);
    VentanaEdicion->setModal(true);
    VentanaEdicion->show();
    Indice.append(ui->PerifericosDatos->item(IndIndex,0)->text());

    VentanaEdicion->SetDatos(2,Indice);

    connect (VentanaEdicion, SIGNAL(finalizar()),this, SLOT(ActDatos())  );
    BloquearBotones();
}

void Reparaciones::on_CAU_GUARDAR_clicked()
{
    QString Fallas, Grupo;
    QString Nombre, concep;
    QList <QString> lFallas;
    bool sig,ok;
    int RepId;

    if(ui->PerRepID->text().isEmpty())
    {
        MensajeTrabajo();
        return;
    }

    if (ui->Prod_Per->currentRow()<0) //Si no esta seleccionado
    {
        QMessageBox::critical(this, tr("Seleccion de Sensor"),
                              tr("Seleccionar Tipo de sensor antes de guardar"));
        return;
    }
    int fila = ui->Prod_Per->currentIndex().row();
    Nombre = ui->Prod_Per->item(fila,0)->text();

//Controla si el sensor ya fue guardado en este trabajo, compara Nombre Numero de Serie e ID del trabajo
    if(CauDuplicado(Nombre, ui->SEN_NSERIE->text(),ui->PerRepID->text()))
        return;

    RepId = ui->PerRepID->text().toInt(&ok,10);
    SNAnt = ui->SEN_NSERIE->text().toInt(&sig,10);
    //--------------------------------------------------------------------------------
    //     Control de Fallas
    //--------------------------------------------------------------------------------
    Fallas.clear();
    Grupo.clear();
    lFallas << tFallas.GetFallas(*ui->PER_FALLAS);

    Fallas.append(lFallas[0]);
    Grupo.append(lFallas[1]);

    //Carga datos DB
    concep.clear();
    concep.append(QString::number(ui->PerConcepto->currentIndex()));
qDebug () << concep;
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
                "grupo,"
                "cantidad,"
                "concepto)"
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
                "'"+Grupo+                      "',"
                "'"+ui->SenCantidad->text()+    "',"
                "'"+concep+"'"
                ");");

    QSqlQuery insertar;
    insertar.prepare(Conf);
    if(!insertar.exec())
    {
        QMessageBox::critical(this,tr("Tabla Caudalimetro"),
                              tr("Falla al crear la tabla\n"
                                 "%1").arg(insertar.lastError().text()));
    }

    dbReparaciones.ActualizarCaudalimetro(*ui->CaudalimetroDatos,RepId);
}

void Reparaciones::on_CAU_BORRAR_clicked()
{
    dbReparaciones.BorrarItem("Caudalimetro",IndEdicion);
    dbReparaciones.ActualizarCaudalimetro(*ui->CaudalimetroDatos,IdReparacion);
    BloquearBotones();
}

void Reparaciones::on_CAU_EDITAR_clicked()
{

    QString Indice;
    reparacioneseditar *VentanaEdicion  = new reparacioneseditar(this);
    VentanaEdicion->setModal(true);
    VentanaEdicion->show();
    Indice.append(ui->CaudalimetroDatos->item(IndIndex,0)->text());

    VentanaEdicion->SetDatos(4,Indice);

    connect (VentanaEdicion, SIGNAL(finalizar()),this, SLOT(ActDatos())  );
    BloquearBotones();
    /*
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
    editar.prepare(Conf);
    if(!editar.exec())
    {
        QMessageBox::critical(this,tr("Tabla Caudalimetro"),
                              tr("Falla edicion de datos\n"
                                 "%1").arg(editar.lastError().text()));
    }
*/
    dbReparaciones.ActualizarCaudalimetro(*ui->CaudalimetroDatos,IdReparacion);
    BloquearBotones();
}

void Reparaciones::on_RPM_GUARDAR_clicked()
{
    QString Fallas, Grupo, FactConf;
    QString Nombre, concep;
    QList <QString> lFallas;
    bool sig;

    if(ui->PerRepID->text().isEmpty())
    {
        MensajeTrabajo();
        return;
    }

    if (ui->Prod_Per->currentRow()<0) //Si no esta seleccionado
    {
        QMessageBox::critical(this, tr("Seleccion de Sensor"),
                              tr("Seleccionar Tipo de sensor antes de guardar"));
        return;
    }
    int fila = ui->Prod_Per->currentIndex().row();
    Nombre = ui->Prod_Per->item(fila,0)->text();

//Controla si el sensor ya fue guardado en este trabajo, compara Nombre Numero de Serie e ID del trabajo
    if(SenDuplicado(Nombre, ui->SEN_NSERIE->text(),ui->PerRepID->text()))
        return;

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
        Fallas.clear();
        Grupo.clear();
        lFallas << tFallas.GetFallas(*ui->PER_FALLAS);

        Fallas.append(lFallas[0]);
        Grupo.append(lFallas[1]);

        FactConf.append("FK:" + ui->RPM_FK->text());
        //Carga datos DB
        concep.clear();
        concep.append(QString::number(ui->PerConcepto->currentIndex()));
qDebug () << concep;
        int fila = ui->Prod_Per->currentIndex().row();
        QString Nombre;
        Nombre = ui->Prod_Per->item(fila,0)->text();

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
                    "grupo,"
                    "cantidad,"
                    "concepto)"
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
                    "'"+Grupo+                      "',"
                    "'"+ui->SenCantidad->text()+    "',"
                    "'"+concep+"'"
                    ");");

        QSqlQuery insertar;
        insertar.prepare(Conf);
        if(!insertar.exec())
        {
            QMessageBox::critical(this,tr("Tabla Perifericos"),
                                  tr("Falla al crear la tabla\n"
                                     "%1").arg(insertar.lastError().text()));
        }

        ui->SEN_BON->setCurrentIndex(0);
        sig = false;
    }
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

    connect (VentanaEdicion, SIGNAL(finalizar()),this, SLOT(ActDatos())  );
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

void Reparaciones::on_INS_GUARDAR_clicked()
{
    QString Fallas, Grupo;
    QString Nombre, concep;
    QList <QString> lFallas;
    bool sig;

    if(ui->InstRepID->text().isEmpty())
    {
        MensajeTrabajo();
        return;
    }

    if(ui->Prod_Ins->currentRow()<0)
    {
        QMessageBox::critical(this, tr("Seleccion de Instalacion"),
                              tr("Seleccionar Tipo de instalacion antes de guardar"));
        return;
    }

    Nombre = ui->Prod_Ins->item(ui->Prod_Ins->currentIndex().row(),0)->text();

    //Controla si el sensor ya fue guardado en este trabajo, compara Nombre Numero de Serie e ID del trabajo
    if(InsDuplicado(Nombre, ui->INS_NSerie->text(),ui->InstRepID->text()))
        return;


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

        Fallas.clear();
        Grupo.clear();
        lFallas << tFallas.GetFallas(*ui->INS_FALLAS);

        Fallas.append(lFallas[0]);
        Grupo.append(lFallas[1]);
//Carga datos DB


        concep.clear();
        concep.append(QString::number(ui->InstConcepto->currentIndex()));
    qDebug () << concep;

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
                    "grupo,"
                    "cantidad,"
                    "concepto)"
                    "VALUES("
                    "'"+Nombre+                     "',"
                    "'"+ui->INS_NSerie->text()+     "',"
                    "'"+Fallas+                     "',"
                    "'"+ui->INS_BON->currentText()+ "',"
                    "'"+ui->INS_COM->toPlainText()+ "',"
                    "'"+ui->INS_FR->text()+         "',"
                    "'"+ui->InstRepID->text()+      "',"
                    "'"+Grupo+                      "',"
                    "'"+ui->InstCantidad->text()+    "',"
                    "'"+concep+"'"
                    ");");

        QSqlQuery insertar;
        insertar.prepare(Conf);
        if(!insertar.exec())
        {
            QMessageBox::critical(this,tr("Tabla Instalaciones"),
                                  tr("Falla al crear la tabla\n"
                                     "%1").arg(insertar.lastError().text()));
        }

        dbReparaciones.ActualizarInstalaciones(*ui->InstalacionesDatos,IdReparacion);

    }
}

void Reparaciones::on_INS_EDITAR_clicked()
{
    QString Indice;
    reparacioneseditar *VentanaEdicion  = new reparacioneseditar(this);
    VentanaEdicion->setModal(true);
    VentanaEdicion->show();
    Indice.append(ui->InstalacionesDatos->item(IndIndex,0)->text());

    VentanaEdicion->SetDatos(3,Indice);
    connect (VentanaEdicion, SIGNAL(finalizar()),this, SLOT(ActDatos())  );

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

  //  qDebug () << indice << Equipo <<  tipo;
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
    else if (tipo==4)
    {
        int fil;
        int i;
        ui->tabWidget->setCurrentIndex(3);
        dbReparaciones.CargarFallas(*ui->InsumosFallas,Equipo);

        fil = ui->ProdInsumos->rowCount();

        for (i=0;i<=fil;i++)
        {
            if(Equipo == ui->ProdInsumos->item(i,0)->text())
            {
                ui->ProdInsumos->selectRow(i);

                break;
            }
        }
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
    ID = IdReparacion;

    Conf.append("SELECT * FROM Ingreso WHERE repid == "+ QString::number(ID,10));

    QSqlQuery consultar;
    consultar.prepare(Conf);
    if(!consultar.exec())
    {
        QMessageBox::critical(this,tr("Tabla Ingreso"),
                              tr("Falla carga de datos"));
    }

    int fila  = 0;

    ui->RepTrabajo->setRowCount(0);

    while(consultar.next())
    {
        ui->RepTrabajo->insertRow(fila);
        ui->RepTrabajo->setRowHeight(fila,20);
        ui->RepTrabajo->setItem(fila,0,new QTableWidgetItem (consultar.value("cantidad").toByteArray().constData()));
        ui->RepTrabajo->setItem(fila,1,new QTableWidgetItem (consultar.value("nombre").toByteArray().constData()));
        ui->RepTrabajo->setItem(fila,2,new QTableWidgetItem (consultar.value("desc").toByteArray().constData()));
        ui->RepTrabajo->setItem(fila,3,new QTableWidgetItem (consultar.value("sn").toByteArray().constData()));
        ui->RepTrabajo->setItem(fila,4,new QTableWidgetItem (consultar.value("obs").toByteArray().constData()));
        fila ++;

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
    return true;
}

bool Reparaciones::SenDuplicado(QString Nombre, QString SN, QString RepId)
{
    QSqlQuery consultar;
    QString Conf;

    Conf.clear();
    Conf.append("SELECT * FROM Perifericos WHERE nombre ='" +Nombre + "' AND sn = '" + SN + "' AND repid = '" + RepId +"'");

    qDebug() <<Conf;
    consultar.prepare(Conf);
    consultar.exec();
    consultar.next();

    Conf.clear();
    Conf.append(consultar.value(0).toString());
 //   qDebug() << consultar.lastError();
 //   qDebug() << Conf;
    if(!Conf.isEmpty())
       return DobleGuardadoMsg();

    return false;
  //  ui->IngObs->setText(consultar.value("obs").toByteArray());
}


bool Reparaciones::MonDuplicado(QString Nombre, QString SN, QString RepId)
{
    QSqlQuery consultar;
    QString Conf;

    Conf.clear();
    Conf.append("SELECT * FROM Monitores WHERE nombre ='" +Nombre + "' AND sn = '" + SN + "' AND repid = '" + RepId +"'");

 //   qDebug() <<Conf;
    consultar.prepare(Conf);
    consultar.exec();
    consultar.next();

    Conf.clear();
    Conf.append(consultar.value(0).toString());
//    qDebug() << consultar.lastError();
//    qDebug() << Conf;
    if(!Conf.isEmpty())
       return DobleGuardadoMsg();

    return false;
  //  ui->IngObs->setText(consultar.value("obs").toByteArray());
}

bool Reparaciones::InsDuplicado(QString Nombre, QString SN, QString RepId)
{
    QSqlQuery consultar;
    QString Conf;

    Conf.clear();
    Conf.append("SELECT * FROM Instalaciones WHERE nombre ='" +Nombre + "' AND sn = '" + SN + "' AND repid = '" + RepId +"'");

//    qDebug() <<Conf;
    consultar.prepare(Conf);
    consultar.exec();
    consultar.next();

    Conf.clear();
    Conf.append(consultar.value(0).toString());
//    qDebug() << consultar.lastError();
//    qDebug() << Conf;
    if(!Conf.isEmpty())
       return DobleGuardadoMsg();

    return false;
  //  ui->IngObs->setText(consultar.value("obs").toByteArray());
}


bool Reparaciones::CauDuplicado(QString Nombre, QString SN, QString RepId)
{
    QSqlQuery consultar;
    QString Conf;

    Conf.clear();
    Conf.append("SELECT * FROM Instalaciones WHERE nombre ='" +Nombre + "' AND sn = '" + SN + "' AND repid = '" + RepId +"'");

//    qDebug() <<Conf;
    consultar.prepare(Conf);
    consultar.exec();
    consultar.next();

    Conf.clear();
    Conf.append(consultar.value(0).toString());
//    qDebug() << consultar.lastError();
//    qDebug() << Conf;
    if(!Conf.isEmpty())
       return DobleGuardadoMsg();

    return false;
  //  ui->IngObs->setText(consultar.value("obs").toByteArray());
}

void Reparaciones::on_ProdInsumos_clicked(const QModelIndex &index)
{

    QString Sensor;
    int fil = index.row();
    Sensor = ui->ProdInsumos->item(fil,0)->text();
    dbReparaciones.CargarFallas(*ui->InsumosFallas,Sensor);

}




void Reparaciones::on_InsumosDatos_clicked(const QModelIndex &index)
{
    bool ok;
    IndIndex = index.row();
    IndEdicion = ui->InsumosDatos->item(index.row(),0)->text().toInt(&ok,10);
    ui->InsumosBorrar->setEnabled(true);
    ui->InsumosEditar->setEnabled(true);
}

void Reparaciones::on_InsumosGuardar_clicked()
{
    QString Fallas, Grupo;
    QString Nombre, concep;
    QList <QString> lFallas;
    int cant;
    bool ok;

    if(ui->InsumosRepId->text().isEmpty())
    {
        MensajeTrabajo();
        return;
    }

    if(ui->ProdInsumos->currentRow()<0)
    {
        QMessageBox::critical(this, tr("Seleccion de Instalacion"),
                              tr("Seleccionar Tipo de instalacion antes de guardar"));
        return;
    }

    Nombre = ui->ProdInsumos->item(ui->ProdInsumos->currentIndex().row(),0)->text();


    if (!ui->InsumosBonificacion->currentIndex())
    {
        BonificacionMsg();
    }
    else
    {
        //--------------------------------------------------------------------------------
        //     Control de Fallas
        //--------------------------------------------------------------------------------

        Fallas.clear();
        Grupo.clear();
        lFallas << tFallas.GetFallas(*ui->InsumosFallas);

        Fallas.append(lFallas[0]);
        Grupo.append(lFallas[1]);
        //Carga datos DB


        concep.clear();
        concep.append(QString::number(ui->InsumosConcepto->currentIndex()));
        cant = ui->InsumosCantidad->text().toInt(&ok);

        QString Conf;
        Conf.clear();
        Conf.append("INSERT INTO Insumos("
                    "cant,"
                    "codigo,"
                    "nombre,"
                    "falla,"
                    "grupo,"
                    "bonif,"
                    "concepto,"
                    "obs,"
                    "frep,"
                    "cantidad,"
                    "repid)"
                    "VALUES("
                    ""+QString::number(cant,10)+","
                    "'',"
                    "'"+Nombre+                     "',"
                    "'"+Fallas+                     "',"
                    "'"+Grupo+                      "',"
                    "'"+ui->InsumosBonificacion->currentText()+ "',"
                    "'"+concep+"',"
                    "'"+ui->InsumosComentario->toPlainText()+ "',"
                    "'"+ui->InsumosFechaRep->text()+         "',"
                    "'"+ui->InsumosCantidad->text()+         "',"
                    "'"+ui->InsumosRepId->text()+      "'"
                    ");");

        QSqlQuery insertar;
        insertar.prepare(Conf);
        if(!insertar.exec())
        {
            QMessageBox::critical(this,tr("Tabla Insumos"),
                                  tr("Falla al cargar la tabla :(\n"
                                     "%1").arg(insertar.lastError().text()));
        }

        dbReparaciones.ActualizarInsumos(*ui->InsumosDatos,IdReparacion);

    }

}

void Reparaciones::on_InsumosEditar_clicked()
{
    QString Indice;
    reparacioneseditar *VentanaEdicion  = new reparacioneseditar(this);
    VentanaEdicion->setModal(true);
    VentanaEdicion->show();
    Indice.append(ui->InsumosDatos->item(IndIndex,0)->text());

    VentanaEdicion->SetDatos(5,Indice);
    connect (VentanaEdicion, SIGNAL(finalizar()),this, SLOT(ActDatos())  );

    BloquearBotones();
    dbReparaciones.ActualizarInstalaciones(*ui->InstalacionesDatos,IdReparacion);

}

void Reparaciones::on_InsumosBorrar_clicked()
{
    dbReparaciones.BorrarItem("Insumos",IndEdicion);
    dbReparaciones.ActualizarInstalaciones(*ui->InsumosDatos,IdReparacion);
    BloquearBotones();
}

void Reparaciones::on_Comparacion_clicked()
{
    NotaPedido *NP = new NotaPedido (this);
    NP->setModal(true);
    NP->show();
    NP->CargaDiferencia(IdReparacion);
}
