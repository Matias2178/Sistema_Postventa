#include "ingreso.h"
#include "ui_ingreso.h"
#include <QMessageBox>
#include <QDebug>
#include <variables.h>





Ingreso::Ingreso(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Ingreso)
{

    ui->setupUi(this);

    IngresoID = 0;
    ModAgentes = new QSqlRelationalTableModel(this,dbManejo::dbRetorna());
    ModAgentes->setTable("Agente"); 
    ModAgentes->select();

//Auto completado del line Edit para buscar a los agentes
    CompleterAgente = new QCompleter(ModAgentes, this);
    CompleterAgente->setCaseSensitivity(Qt::CaseInsensitive);
    CompleterAgente->setCompletionColumn(1);
    ui->AgenteBuscar->setCompleter(CompleterAgente);


    FiltAgentes = new QSortFilterProxyModel(this);
    FiltAgentes->setSourceModel(ModAgentes);
    FiltAgentes->setFilterCaseSensitivity(Qt::CaseInsensitive);
    FiltAgentes->setFilterKeyColumn(-1); //-1 ordena por todas la columnas

    ui->AgentesTabla->setModel(FiltAgentes);
    ui->AgentesTabla->hideColumn(0);
    ui->AgentesTabla->sortByColumn(1,Qt::AscendingOrder);
    ui->AgentesTabla->setSortingEnabled(true);
    ui->AgentesTabla->setColumnWidth(1,250);

    ModEquipos = new QSqlRelationalTableModel(this,dbManejo::dbRetorna());
    ModEquipos->setTable("Productos");
    ModEquipos->select();

    FilEquipos = new QSortFilterProxyModel(this);
    FilEquipos->setSourceModel(ModEquipos);
    FilEquipos->setFilterCaseSensitivity(Qt::CaseInsensitive);
    FilEquipos->setFilterKeyColumn(-1);

    ui->EquiposTablaIng->setModel(FilEquipos);
    ui->EquiposTablaIng->hideColumn(0);
    ui->EquiposTablaIng->hideColumn(3);
    ui->EquiposTablaIng->hideColumn(4);
    ui->EquiposTablaIng->sortByColumn(1,Qt::AscendingOrder);
    ui->EquiposTablaIng->setSortingEnabled(true);
    ui->EquiposTablaIng->setColumnWidth(1,100);
    ui->EquiposTablaIng->setColumnWidth(2,250);


    ModRepDatos = new QSqlRelationalTableModel(this,dbManejo::dbRetorna());
    ModRepDatos->setTable("Reparaciones");
    ModRepDatos->setEditStrategy(QSqlTableModel::OnFieldChange);
    ModRepDatos->select();

    FilRepDatos = new QSortFilterProxyModel(this);
    FilRepDatos->setSourceModel(ModRepDatos);
    FilRepDatos->setFilterCaseSensitivity(Qt::CaseInsensitive);
    FilRepDatos->setFilterKeyColumn(-1);

    ui->RepTablaIng->setModel(FilRepDatos);
    ui->RepTablaIng->sortByColumn(0,Qt::AscendingOrder);
    ui->RepTablaIng->setSortingEnabled(true);
    ui->RepTablaIng->setColumnWidth(0,50);
    ui->RepTablaIng->setColumnWidth(1,150);
    ui->RepTablaIng->scrollToBottom();


    FilRepDatos->setFilterFixedString("");

//    ModProdDatos = new QSqlRelationalTableModel(this,dbManejo::dbRetorna());
//    ModProdDatos->setTable("Ingreso");
//    ModProdDatos->setEditStrategy(QSqlTableModel::OnManualSubmit);
//    ModProdDatos->select();

//    FilProdDatos = new QSortFilterProxyModel(this);
//    FilProdDatos->setSourceModel(ModProdDatos);
//    FilProdDatos->setFilterCaseSensitivity(Qt::CaseInsensitive);
//    FilProdDatos->setFilterKeyColumn(7);


//    ui->IngresoTabla_2->setModel(FilProdDatos);
//    ui->IngresoTabla_2->sortByColumn(0,Qt::AscendingOrder);
//    ui->IngresoTabla_2->setSortingEnabled(true);

    ui->FIngreso->setInputMask("00/00/0000");
    ui->FIngreso->setText(dControl.currentDateTime().toString("ddMMyyyy"));

    ui->IngEditar->setEnabled(false);
    ui->IngBorrar->setEnabled(false);

    ui->RepEditar->setEnabled(false);
    ui->RepBorrar->setEnabled(false);
    ui->RepIniciar->setEnabled(false);

    FilRepDatos->setFilterFixedString("");

 //   connect (IngRep, SIGNAL(finalizar()), this, SLOT(ActualizarDatos()) );


}

Ingreso::~Ingreso()
{
    delete ui;
}

void Ingreso::on_RepIniciar_clicked()
{
    int fila;
    IngresoID = 0;
    fila = ui->AgentesTabla->currentIndex().row();
    AgenteTexto.clear();
    AgenteTexto.append(ui->AgentesTabla->model()->data(ui->AgentesTabla->model()->index(fila,1)).toString());

    if(AgenteTexto.isEmpty())
    {
        QMessageBox::critical(this,tr("Datos"),
                              tr("Seleccionar Agente"));
        return;
    }

    IngresoReparaciones *VentanaIngreso  = new IngresoReparaciones(this);
    VentanaIngreso->setModal(true);
    VentanaIngreso->show();
    VentanaIngreso->SetDatos(AgenteTexto,ui->FIngreso->text());
    connect (VentanaIngreso, SIGNAL(finalizar()),this, SLOT(ActualizarDatos())  );

    ui->RepEditar->setEnabled(false);
    ui->RepBorrar->setEnabled(false);
    ui->RepIniciar->setEnabled(false);
}

void Ingreso::on_RepEditar_clicked()
{

    int IdRep;

    IdRep = ui->RepTablaIng->currentIndex().row();
    IdRep = ui->RepTablaIng->model()->data(ui->RepTablaIng->model()->index(IdRep,0)).toInt();

    IngresoReparaciones *VentanaIngreso  = new IngresoReparaciones(this);
    VentanaIngreso->setModal(true);
    VentanaIngreso->show();
    VentanaIngreso->SetDatos(IdRep);

  //  connect (IngRep, SIGNAL(finalizar()), this, SLOT(ActualizarDatos()) );
    connect (VentanaIngreso, SIGNAL(finalizar()),this, SLOT(ActualizarDatos())  );

 //   IngRep->setModal(true);
 //   IngRep->show();
//    IngRep->SetDatos(IdRep);

    ui->RepEditar->setEnabled(false);
    ui->RepBorrar->setEnabled(false);
    ui->RepIniciar->setEnabled(false);

}

void Ingreso::on_RepBorrar_clicked()
{
    int Item;
    bool ok;
    int fila;
    QString AgenteText;
    fila = ui->AgentesTabla->currentIndex().row();
    AgenteText.clear();
    AgenteText.append(ui->AgentesTabla->model()->data(ui->AgentesTabla->model()->index(fila,1)).toString());

    Item = ui->ID_Rep->text().toInt(&ok,10);
    if(dbIngreso.BorrarTrabajo(Item))
    {
        dbIngreso.BorrarItem("Reparaciones",Item);
    }
    ui->RepBorrar->setEnabled(false);
    ui->RepEditar->setEnabled(false);
    ui->RepIniciar->setEnabled(false);
    ModRepDatos->select();
    ModRepDatos->submitAll();
}

void Ingreso::on_IngGuardar_clicked()
{
    QString Ingreso;

    if(ui->ID_Rep->text().isEmpty())
    {
        QMessageBox::critical(this,tr("Trabajo"),
                                  tr("Seleccionar Trabajo para cargar datos"));
        return;
    }
    int fila;

    fila = ui->EquiposTablaIng->currentIndex().row();

    if((fila<0) || !ui->IngCant->value())
    {
        QString Equipo, Cantidad;
        Equipo.clear();
        Cantidad.clear();
        if(fila < 0) Equipo.append("seleccionar Equipo\n");
        if(!ui->IngCant->value())Cantidad.append("ingresar cantidad");
        QMessageBox::critical(this,tr("Datos"),
                              tr("Falta %1"
                                 "      %2").arg(Equipo).arg(Cantidad));
        return;
    }
    Ingreso.clear();

    QString Conf;
    Conf.clear();
    Conf.append("INSERT INTO Ingreso("
                "nombre,"
                "desc,"
                "sn,"
                "cantidad,"
                "fact,"
                "obs,"
                "repid)"
                "VALUES("
                "'"+ui->EquiposTablaIng->model()->data(ui->EquiposTablaIng->model()->index(fila,1)).toString()+"',"
                "'"+ui->EquiposTablaIng->model()->data(ui->EquiposTablaIng->model()->index(fila,2)).toString()+"',"
                "'"+ui->IngSN->text()+            "',"
                "'"+ui->IngCant->text()+            "',"
                "'"+ui->IngFac->text()+             "',"
                "'"+ui->IngCom->toPlainText()+      "',"
                "'"+ui->ID_Rep->text()+             "'"
                ");");

    QSqlQuery insertar;
    insertar.prepare(Conf);
    if(!insertar.exec())
    {
        QMessageBox::critical(this,tr("Tabla Reparaciones"),
                              tr("Falla guardado de datos\n"
                                 "%1").arg(insertar.lastError().text()));
    }

    dbIngreso.CargarIngreso(*ui->IngresoTabla,IngresoID);
    ui->EquipoCodigoBuscar->clear();
    ui->EquipoDescBuscar->clear();
  //  ui->IngCom->clear();
    ui->IngSN->clear();
    ui->IngFac->clear();
    ui->IngCant->setValue(0);
}

void Ingreso::on_IngEditar_clicked()
{
    int fila;
    fila = ui->EquiposTablaIng->currentIndex().row();
    if((fila<0) || !ui->IngCant->value())
    {
        QString Equipo, Cantidad;
        Equipo.clear();
        Cantidad.clear();
        if(fila<0) Equipo.append("seleccionar Equipo\n");
        if(!ui->IngCant->value())Cantidad.append("ingresar cantidad");
        QMessageBox::critical(this,tr("Datos"),
                              tr("Falta %1"
                                 "      %2").arg(Equipo).arg(Cantidad));
        return;
    }
    QString Conf;
    Conf.append("UPDATE Ingreso SET "
                "nombre ="
                "'"+ui->EquiposTablaIng->model()->data(ui->EquiposTablaIng->model()->index(fila,1)).toString()+"',"
                "desc ="
                "'"+ui->EquiposTablaIng->model()->data(ui->EquiposTablaIng->model()->index(fila,2)).toString()+"',"
                "sn ="
                "'"+ui->IngSN->text()+"',"
                "cantidad ="
                "'"+ui->IngCant->text()+"',"
                "fact ="
                "'"+ui->IngFac->text()+"',"
                "obs ="
                "'"+ui->IngCom->toPlainText()+"',"
                "repid ="
                "'"+ui->ID_Rep->text()+"'"
                " WHERE id ="
                ""+QString::number(IndiceIng,10)+""
                "");
    QSqlQuery editar;
    editar.prepare(Conf);
    if(!editar.exec())
    {
        QMessageBox::critical(this,tr("Tabla Reparaciones"),
                              tr("Falla edicion de datos"));
    }

    dbIngreso.CargarIngreso(*ui->IngresoTabla,IngresoID);
    IndiceIng = 0;
    ui->IngEditar->setEnabled(false);
    ui->IngBorrar->setEnabled(false);
}

void Ingreso::on_IngBorrar_clicked()
{
    int Item;
    bool ok;
    int fila;
//    qDebug() << "paso por aca";
    fila = ui->IngresoTabla->currentIndex().row();
//    qDebug () << fila;
    Item = ui->IngresoTabla->item(fila,0)->text().toInt(&ok,10);
 //   qDebug () << Item;
    dbIngreso.BorrarItem("Ingreso",Item);
    dbIngreso.CargarIngreso(*ui->IngresoTabla,IngresoID);
    IndiceIng = 0;
    ui->IngEditar->setEnabled(false);
    ui->IngBorrar->setEnabled(false);


    ModRepDatos->select();
    ModRepDatos->submitAll();
}

void Ingreso::on_IngresoTabla_clicked(const QModelIndex &index)
{
    bool ok;
    ui->EquipoCodigoBuscar->clear();
    ui->EquipoDescBuscar->clear();
    ui->IngCom->clear();
    ui->IngSN->clear();
    ui->IngFac->clear();

    ui->IngCant->setValue(ui->IngresoTabla->item(index.row(),4)->text().toInt(&ok,10));
    ui->EquipoCodigoBuscar->setText(ui->IngresoTabla->item(index.row(),1)->text());
    ui->IngCom->setText(ui->IngresoTabla->item(index.row(),6)->text());
    ui->IngSN->setText(ui->IngresoTabla->item(index.row(),3)->text());
    ui->IngFac->setText(ui->IngresoTabla->item(index.row(),5)->text());
    ui->EquiposTablaIng->selectRow(0);


    IndiceIng = ui->IngresoTabla->item(index.row(),0)->text().toInt();

    ui->IngBorrar->setEnabled(true);
    ui->IngEditar->setEnabled(true);
}



void Ingreso::on_RepMostrar_clicked()
{
    AgenteTexto.clear();
    FilRepDatos->setFilterFixedString("");
    ui->RepTablaIng->scrollToBottom();
}

void Ingreso::on_AgenteBuscar_textChanged(const QString &arg1)
{
    FiltAgentes->setFilterFixedString(arg1);
}

void Ingreso::on_AgentesTabla_clicked(const QModelIndex &index)
{
    AgenteTexto.clear();
    AgenteTexto.append(ui->AgentesTabla->model()->data(index).toString());
    ui->FIngreso->setText(dControl.currentDateTime().toString("ddMMyyyy"));
    FilRepDatos->setFilterFixedString(AgenteTexto);

    ui->ID_Rep->clear();
    ui->IngObs->clear();
    ui->RepTablaIng->scrollToBottom();

    ui->RepEditar->setEnabled(false);
    ui->RepBorrar->setEnabled(false);
    ui->RepIniciar->setEnabled(true);

}

void Ingreso::on_EquipoCodigoBuscar_textChanged(const QString &arg1)
{
    FilEquipos->setFilterKeyColumn(1);
    FilEquipos->setFilterFixedString(arg1);
    ui->EquipoDescBuscar->clear();
}

void Ingreso::on_EquipoDescBuscar_textChanged(const QString &arg1)
{
    FilEquipos->setFilterKeyColumn(2);
    FilEquipos->setFilterFixedString(arg1);
    ui->EquipoCodigoBuscar->clear();
}

void Ingreso::on_RepTablaIng_clicked(const QModelIndex &index)
{
    Fila = index.row();
    bool ok;
    QString Conf,IDIng;

    ModRepDatos->submitAll();



    ui->FIngreso->setText(ui->RepTablaIng->model()->data(ui->RepTablaIng->model()->index(Fila,2)).toString());
    ui->ID_Rep->setText(ui->RepTablaIng->model()->data(ui->RepTablaIng->model()->index(Fila,0)).toString());
    IngresoID = ui->RepTablaIng->model()->data(ui->RepTablaIng->model()->index(Fila,0)).toInt(&ok);

    dbIngreso.CargarIngreso(*ui->IngresoTabla,IngresoID);

    IDIng.append(QString::number(IngresoID,10));
    Conf.append("SELECT * FROM Reparaciones WHERE id = ""'" + IDIng + "'");
    QSqlQuery consultar;
    consultar.prepare(Conf);
    consultar.exec();
    consultar.next();
    ui->IngObs->setText(consultar.value("obs").toByteArray());
    ui->rTransp->setText(consultar.value("rTransp").toByteArray());
    ui->fTransp->setText(consultar.value("fTransp").toByteArray());
    ui->rAgente->setText(consultar.value("ragente").toString());

    ui->RepBorrar->setEnabled(true);
    ui->RepEditar->setEnabled(true);
    ui->RepIniciar->setEnabled(false);
}


void Ingreso::ActualizarDatos()
{
    QString Conf,IDIng;
    QSqlQuery consultar;

    ModRepDatos->select();
    ModRepDatos->submitAll();
    FilRepDatos->setFilterFixedString(AgenteTexto);
    if(!IngresoID)
    {
  //      qDebug () << "Hola Mama";

        Conf.append("SELECT id FROM Reparaciones WHERE agente == ""'" + AgenteTexto + "'");
        consultar.prepare(Conf);
        consultar.exec();
        consultar.last();
        IngresoID = consultar.value("id").toInt();

    }
    IDIng.append(QString::number(IngresoID,10));
    ui->ID_Rep->setText(IDIng);

  //  ui->FIngreso->setText(ui->RepTablaIng->model()->data(ui->RepTablaIng->model()->index(Fila,2)).toString());

    dbIngreso.CargarIngreso(*ui->IngresoTabla,IngresoID);

//    qDebug () << IngresoID << IDIng;
    Conf.clear();
    Conf.append("SELECT * FROM Reparaciones WHERE id = ""'" + IDIng + "'");


//    qDebug () << Conf;
    consultar.prepare(Conf);
    consultar.exec();
    consultar.next();
    ui->IngObs->setText(consultar.value("obs").toString());
    ui->rTransp->setText(consultar.value("rTransp").toString());
    ui->fTransp->setText(consultar.value("fTransp").toString());
    ui->FIngreso->setText(consultar.value("fing").toString());
    ui->rAgente->setText(consultar.value("ragente").toString());

    ModRepDatos->select();
    ModRepDatos->submitAll();
    FilRepDatos->setFilterFixedString(AgenteTexto);
}



void Ingreso::on_Actualizar_clicked()
{
    QString Conf,IDIng;
    QSqlQuery consultar;

//    qDebug() <<"1:" << Fila << IngresoID << IDIng;

    ModRepDatos->select();
    ModRepDatos->submitAll();
    FilRepDatos->setFilterFixedString(AgenteTexto);
    if(!IngresoID)
    {
        Conf.append("SELECT id FROM Reparaciones WHERE agente == ""'" + AgenteTexto + "'");
        consultar.prepare(Conf);
        consultar.exec();
        consultar.last();
        IngresoID = consultar.value("id").toInt();

    }

    IDIng.append(QString::number(IngresoID,10));
    ui->ID_Rep->setText(IDIng);



    Conf.clear();
    Conf.append("SELECT * FROM Reparaciones WHERE id = ""'" + IDIng + "'");

    consultar.prepare(Conf);
    consultar.exec();
    consultar.next();
    ui->IngObs->setText(consultar.value("obs").toByteArray());
    ui->rTransp->setText(consultar.value("rTransp").toByteArray());
    ui->fTransp->setText(consultar.value("fTransp").toByteArray());

//    qDebug() <<"4:" << Fila << IngresoID << IDIng;
    int i;
dbIngreso.CargarIngreso(*ui->IngresoTabla,IngresoID);

    ui->RepTablaIng->selectRow(0);
    ui->RepTablaIng->reset();
//    a = ui->RepTablaIng->model()->rowCount();
 //           qDebug() << a;

    for(i=0;i<IngresoID;i++)
    {

//     qDebug()<< a<<ui->RepTablaIng->model()->data(ui->RepTablaIng->model()->index(i,0)).toInt();

//        qDebug()<< i << IngresoID <<"==" <<ui->RepTablaIng->model()->index(i,0).data().toInt();
        if(!ui->RepTablaIng->model()->data(ui->RepTablaIng->model()->index(i,0)).toInt())
            break;
        if(IngresoID == ui->RepTablaIng->model()->index(i,0).data().toInt())
            break;
    }
    ui->RepTablaIng->selectRow(i);


}

void Ingreso::on_Huerfanas_clicked()
{
    int i, ant, IdRep;
    ant =0;
    for (i=0;i<=333;i++) {
        ui->RepTablaIng->selectRow(i);
        IdRep = ui->RepTablaIng->model()->data(ui->RepTablaIng->model()->index(i,0)).toInt();
        qDebug () << IdRep<<i;
        while((IdRep - ant)>1)
        {
            ant++;
            QMessageBox::critical(this,tr("Busqueda de Huerfanas"),
                                  tr("Falta el trabajo  %1  %2 %3").arg(ant).arg(IdRep).arg(IdRep - ant));

            dbIngreso.BorrarTrabajo(ant);


        }
        ant = IdRep;
    }

}
