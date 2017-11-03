#include "busqueda.h"
#include "ui_busqueda.h"
#include <dbmanejo.h>

Busqueda::Busqueda(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Busqueda)
{
    ui->setupUi(this);
    FilBusqueda = new QSortFilterProxyModel(this);

    FilBusqueda = new QSortFilterProxyModel(this);
    FilBusqueda->setFilterCaseSensitivity(Qt::CaseInsensitive);
    FilBusqueda->setFilterKeyColumn(-1); //-1 ordena por todas la columnas
    FilBusqueda->setFilterFixedString(" ");

    ui->BusquedaTbl->setModel(FilBusqueda);
    ui->BusquedaTbl->sortByColumn(0,Qt::AscendingOrder);
    ui->BusquedaTbl->setSortingEnabled(true);
}

Busqueda::~Busqueda()
{
    delete ui;
}

void Busqueda::on_Monitores_clicked()
{
    Monitores = new QSqlRelationalTableModel(this,dbManejo::dbRetorna());
    Monitores->setTable("Monitores");
    Monitores->select();

    FilBusqueda->setSourceModel(Monitores);
}

void Busqueda::on_Perifericos_clicked()
{
    Perifericos = new QSqlRelationalTableModel(this,dbManejo::dbRetorna());
    Perifericos->setTable("Perifericos");
    Perifericos->select();

    FilBusqueda->setSourceModel(Perifericos);

}

void Busqueda::on_Instalaciones_clicked()
{
    Instalaciones = new QSqlRelationalTableModel(this,dbManejo::dbRetorna());
    Instalaciones->setTable("Instalaciones");
    Instalaciones->select();

    FilBusqueda->setSourceModel(Instalaciones);

}

void Busqueda::on_BusquedaTbl_doubleClicked(const QModelIndex &index)
{
//    QString AgenteTexto;
//    AgenteTexto.clear();
//    AgenteTexto.append(ui->BusquedaTbl->model()->data(index).toString());
//    ui->FIngreso->setText(dControl.currentDateTime().toString("ddMMyyyy"));
//    FilRepDatos->setFilterFixedString(AgenteTexto);
}

void Busqueda::on_AgenteBuscar_textChanged(const QString &arg1)
{
    FilBusqueda->setFilterFixedString(arg1);
}

void Busqueda::on_BusquedaTbl_clicked(const QModelIndex &index)
{

}
