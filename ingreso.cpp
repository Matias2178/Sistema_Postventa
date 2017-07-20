#include "ingreso.h"
#include "ui_ingreso.h"
#include <QDateTime>
#include <QMessageBox>
#include <QDebug>
#include <dbmanejo.h>
QDateTime dControl;

dbManejo dbIngreso;
Ingreso::Ingreso(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Ingreso)
{

    ui->setupUi(this);
    ui->FIngreso->setInputMask("00/00/0000");
    ui->FIngreso->setText(dControl.currentDateTime().toString("ddMMyyyy"));
    AgenteCargar();
    dbIngreso.CargarReparaciones(*ui->RepTabla,"*");
    IngresoProductos();
    ui->RepEditar->setEnabled(false);
    ui->RepBorrar->setEnabled(false);
    ui->IngEditar->setEnabled(false);
    ui->IngBorrar->setEnabled(false);
 //   ui->RepTabla->sortByColumn(1, Qt::AscendingOrder);
    ui->IngresoTabla->setSortingEnabled(true);
    ui->RepTabla->setSortingEnabled(true);
}

Ingreso::~Ingreso()
{
    delete ui;
}

void Ingreso::on_RepGuardar_clicked()
{
    int fila;
    QString AgenteText;
    fila = ui->AgenteTabla->currentIndex().row();
    AgenteText.clear();
    AgenteText.append(ui->AgenteTabla->item(fila,0)->text());
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
    dbIngreso.CargarReparaciones(*ui->RepTabla,AgenteText);
}

void Ingreso::on_RepEditar_clicked()
{
    int fila;
    QString AgenteText;
    fila = ui->AgenteTabla->currentIndex().row();
    AgenteText.clear();
    AgenteText.append(ui->AgenteTabla->item(fila,0)->text());
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
    dbIngreso.CargarReparaciones(*ui->RepTabla,AgenteText);
    ui->RepEditar->setEnabled(false);
    ui->RepBorrar->setEnabled(false);
}

void Ingreso::on_RepBorrar_clicked()
{
    int Item;
    bool ok;
    int fila;
    QString AgenteText;
    fila = ui->AgenteTabla->currentIndex().row();
    AgenteText.clear();
    AgenteText.append(ui->AgenteTabla->item(fila,0)->text());

    Item = ui->ID_Rep->text().toInt(&ok,10);
    dbIngreso.BorrarItem("Reparaciones",Item);
    dbIngreso.CargarReparaciones(*ui->RepTabla,AgenteText);
    ui->RepBorrar->setEnabled(false);
    ui->RepEditar->setEnabled(false);
}

void Ingreso::on_RepTabla_clicked(const QModelIndex &index)
{
    ui->FIngreso->setText(ui->RepTabla->item(index.row(),2)->text());
    ui->ID_Rep->setText(ui->RepTabla->item(index.row(),0)->text());
    IngresoID = ui->RepTabla->item(index.row(),0)->text().toInt();

    ui->RepBorrar->setEnabled(true);
    ui->RepEditar->setEnabled(true);
    dbIngreso.CargarIngreso(*ui->IngresoTabla,IngresoID);
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
    fila = ui->IngEquipoTabla->currentIndex().row();
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
                "'"+ui->IngEquipoTabla->item(fila,0)->text()+"',"
                "'"+ui->IngEquipoTabla->item(fila,1)->text()+"',"
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
}

void Ingreso::on_IngEditar_clicked()
{
    int fila;
    fila = ui->IngEquipoTabla->currentIndex().row();
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
                "'"+ui->IngEquipoTabla->item(fila,0)->text()+"',"
                "desc ="
                "'"+ui->IngEquipoTabla->item(fila,1)->text()+"',"
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
    qDebug() << "paso por aca";
    fila = ui->IngresoTabla->currentIndex().row();
    qDebug () << fila;
    Item = ui->IngresoTabla->item(fila,0)->text().toInt(&ok,10);
    qDebug () << Item;
    dbIngreso.BorrarItem("Ingreso",Item);
    dbIngreso.CargarIngreso(*ui->IngresoTabla,IngresoID);
    IndiceIng = 0;
    ui->IngEditar->setEnabled(false);
    ui->IngBorrar->setEnabled(false);
}

void Ingreso::on_IngresoTabla_clicked(const QModelIndex &index)
{
    bool ok;
    ui->IngCant->setValue(ui->IngresoTabla->item(index.row(),4)->text().toInt(&ok,10));
    IndiceIng = ui->IngresoTabla->item(index.row(),0)->text().toInt();

    ui->IngBorrar->setEnabled(true);
    ui->IngEditar->setEnabled(true);
}

void Ingreso::AgenteCargar()
{
    QString Conf;
    Conf.append("SELECT * FROM Agente");

    QSqlQuery consultar;

    if(!consultar.prepare(Conf))
    {
        QMessageBox::critical(this,tr("Tabla Agente"),
                              tr("Falla guardado de datos"));
    }
    consultar.exec();
    int fila  = 0;
    ui->AgenteTabla->setHorizontalHeaderItem(0,new QTableWidgetItem("Agente"));
    ui->AgenteTabla->setColumnWidth(0,260);
    while(consultar.next())
    {
        ui->AgenteTabla->insertRow(fila);
        ui->AgenteTabla->setRowHeight(fila,20);
        ui->AgenteTabla->setItem(fila,0,new QTableWidgetItem (consultar.value(1).toByteArray().constData()));
        fila ++;
    }
    ui->AgenteTabla->sortByColumn(0,Qt::AscendingOrder);
}



void Ingreso::IngresoProductos()
{
    QString Conf;
    Conf.append("SELECT * FROM Productos");

    QSqlQuery consultar;
    if(!consultar.prepare(Conf))
    {
        QMessageBox::critical(this,tr("Tabla Productos"),
                              tr("Falla guardado de datos"));
    }
    consultar.exec();
    ui->IngEquipoTabla->clear();

    ui->IngEquipoTabla->setHorizontalHeaderItem(0,new QTableWidgetItem("Codigo"));
    ui->IngEquipoTabla->setHorizontalHeaderItem(1,new QTableWidgetItem("Descripcion"));
    int fila  = 0;
    while(consultar.next())
    {
        ui->IngEquipoTabla->insertRow(fila);
        ui->IngEquipoTabla->setRowHeight(fila,20);
        ui->IngEquipoTabla->setItem(fila,0,new QTableWidgetItem (consultar.value(1).toByteArray().constData()));
        ui->IngEquipoTabla->setItem(fila,1,new QTableWidgetItem (consultar.value(2).toByteArray().constData()));
        fila ++;
    }
    ui->IngEquipoTabla->setColumnWidth(0,100);
    ui->IngEquipoTabla->setColumnWidth(1,250);
    ui->IngEquipoTabla->sortByColumn(0,Qt::AscendingOrder);
}

void Ingreso::on_AgenteTabla_clicked(const QModelIndex &index)
{
    QString AgenteTexto;
    AgenteTexto.clear();
    AgenteTexto.append(ui->AgenteTabla->item(index.row(),0)->text());
    ui->FIngreso->setText(dControl.currentDateTime().toString("ddMMyyyy"));
    dbIngreso.CargarReparaciones(*ui->RepTabla,AgenteTexto);
}

void Ingreso::on_RepMostrar_clicked()
{
    dbIngreso.CargarReparaciones(*ui->RepTabla,"*");
}

