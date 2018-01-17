#include "ingreso.h"
#include "ui_ingreso.h"
#include <QDateTime>
#include <QMessageBox>
#include <QDebug>
#include <dbmanejo.h>
#include <variables.h>

QDateTime dControl;

dbManejo dbIngreso;
Ingreso::Ingreso(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Ingreso)
{

    ui->setupUi(this);

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
    ModRepDatos->setEditStrategy(QSqlTableModel::OnManualSubmit);
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

    ui->RepEditar->setEnabled(false);
    ui->RepBorrar->setEnabled(false);
    ui->IngEditar->setEnabled(false);
    ui->IngBorrar->setEnabled(false);

    FilRepDatos->setFilterFixedString("");
}

Ingreso::~Ingreso()
{
    delete ui;
}

void Ingreso::on_RepIniciar_clicked()
{
    int fila;
    QString AgenteText;
    fila = ui->AgentesTabla->currentIndex().row();
    AgenteText.clear();
    AgenteText.append(ui->AgentesTabla->model()->data(ui->AgentesTabla->model()->index(fila,1)).toString());

    if(AgenteText.isEmpty())
    {
        QMessageBox::critical(this,tr("Datos"),
                              tr("Seleccionar Agente"));
        return;
    }
    QString Ingreso;
    Ingreso.clear();
    QString Conf;
    Conf.clear();
    Conf.append("INSERT INTO Reparaciones("
                "agente,"
                "fing,"
                "frep,"
                "operario,"
                "pres)"
                "VALUES("
                "'"+AgenteText+"',"
                "'"+ui->FIngreso->text()+"',"
                "'',"
                "'',"
                "''"
                ");");

    QSqlQuery insertar;
    if(!insertar.prepare(Conf))
    {
        QMessageBox::critical(this,tr("Tabla Reparaciones"),
                              tr("Falla al crear la tabla\n"
                                 "%1").arg(insertar.lastError().text()));
    }
    insertar.exec();
    ModRepDatos->submitAll();
    ui->RepTablaIng->scrollToBottom();

}

void Ingreso::on_RepEditar_clicked()
{
    int fila;
    QString AgenteText;
    fila = ui->AgentesTabla->currentIndex().row();
    AgenteText.clear();
    AgenteText.append(ui->AgentesTabla->model()->data(ui->AgentesTabla->model()->index(fila,1)).toString());

    if(AgenteText.isEmpty())
    {
        QMessageBox::critical(this,tr("Datos"),
                              tr("Seleccionar Agente"));
        return;
    }
    QString Conf;

    Conf.append("UPDATE Reparaciones SET "
                "agente ="
                "'"+AgenteText+"'"
                ",fing ="
                "'"+ui->FIngreso->text()+"'"
                " WHERE id ="
                ""+ui->ID_Rep->text()+""
                "");
    QSqlQuery editar;

    if(!editar.prepare(Conf))
    {
        QMessageBox::critical(this,tr("Tabla Reparaciones"),
                              tr("Falla edicion de datos"));
    }
    editar.exec();
    ui->RepEditar->setEnabled(false);
    ui->RepBorrar->setEnabled(false);
    ModRepDatos->submitAll();
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
    dbIngreso.BorrarItem("Reparaciones",Item);
    ui->RepBorrar->setEnabled(false);
    ui->RepEditar->setEnabled(false);
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
                "cant,"
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
    if(!insertar.prepare(Conf))
    {
        QMessageBox::critical(this,tr("Tabla Reparaciones"),
                              tr("Falla guardado de datos\n"
                                 "%1").arg(insertar.lastError().text()));
    }
    insertar.exec();
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
                "cant ="
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

    if(!editar.prepare(Conf))
    {
        QMessageBox::critical(this,tr("Tabla Reparaciones"),
                              tr("Falla edicion de datos"));
    }
    editar.exec();
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
    FilRepDatos->setFilterFixedString("");
    ui->RepTablaIng->scrollToBottom();
}

void Ingreso::on_AgenteBuscar_textChanged(const QString &arg1)
{
    FiltAgentes->setFilterFixedString(arg1);
}

void Ingreso::on_AgentesTabla_clicked(const QModelIndex &index)
{
    QString AgenteTexto;
    AgenteTexto.clear();
    AgenteTexto.append(ui->AgentesTabla->model()->data(index).toString());
    ui->FIngreso->setText(dControl.currentDateTime().toString("ddMMyyyy"));
    FilRepDatos->setFilterFixedString(AgenteTexto);
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
    int fila = index.row();
    bool ok;
    ui->FIngreso->setText(ui->RepTablaIng->model()->data(ui->RepTablaIng->model()->index(fila,2)).toString());
    ui->ID_Rep->setText(ui->RepTablaIng->model()->data(ui->RepTablaIng->model()->index(fila,0)).toString());
    IngresoID = ui->RepTablaIng->model()->data(ui->RepTablaIng->model()->index(fila,0)).toInt(&ok);
    ui->RepBorrar->setEnabled(true);
    ui->RepEditar->setEnabled(true);
    dbIngreso.CargarIngreso(*ui->IngresoTabla,IngresoID);
 //   FilProdDatos->setFilterRole(IngresoID);
//    FilProdDatos->
}
