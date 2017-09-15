#include "busqueda.h"
#include "ui_busqueda.h"
#include <dbmanejo.h>

Busqueda::Busqueda(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Busqueda)
{
    ui->setupUi(this);
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

    FilMonitores = new QSortFilterProxyModel(this);
    FilMonitores->setSourceModel(Monitores);
    FilMonitores->setFilterCaseSensitivity(Qt::CaseInsensitive);
    FilMonitores->setFilterKeyColumn(-1); //-1 ordena por todas la columnas
    FilMonitores->setFilterFixedString(" ");

    ui->BusquedaTbl->setModel(FilMonitores);
    ui->BusquedaTbl->sortByColumn(0,Qt::AscendingOrder);
    ui->BusquedaTbl->setSortingEnabled(true);
//    ui->BusquedaTbl->scrollToBottom();
}

void Busqueda::on_Perifericos_clicked()
{
    Perifericos = new QSqlRelationalTableModel(this,dbManejo::dbRetorna());
    Perifericos->setTable("Perifericos");
    Perifericos->select();

    FilPerifericos = new QSortFilterProxyModel(this);
    FilPerifericos->setSourceModel(Perifericos);
    FilPerifericos->setFilterCaseSensitivity(Qt::CaseInsensitive);
    FilPerifericos->setFilterKeyColumn(-1); //-1 ordena por todas la columnas
    FilPerifericos->setFilterFixedString(" ");

    ui->BusquedaTbl->setModel(FilPerifericos);
    ui->BusquedaTbl->sortByColumn(0,Qt::AscendingOrder);
    ui->BusquedaTbl->setSortingEnabled(true);
//    ui->BusquedaTbl->scrollToBottom();
}

void Busqueda::on_Instalaciones_clicked()
{
    Instalaciones = new QSqlRelationalTableModel(this,dbManejo::dbRetorna());
    Instalaciones->setTable("Instalaciones");
    Instalaciones->select();

    FilInstalaciones = new QSortFilterProxyModel(this);
    FilInstalaciones->setSourceModel(Instalaciones);
    FilInstalaciones->setFilterCaseSensitivity(Qt::CaseInsensitive);
    FilInstalaciones->setFilterKeyColumn(-1); //-1 ordena por todas la columnas
    FilInstalaciones->setFilterFixedString(" ");

    ui->BusquedaTbl->setModel(FilInstalaciones);
    ui->BusquedaTbl->sortByColumn(0,Qt::AscendingOrder);
    ui->BusquedaTbl->setSortingEnabled(true);
//    ui->BusquedaTbl->scrollToBottom();
}
