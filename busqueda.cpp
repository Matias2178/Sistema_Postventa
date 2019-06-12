#include "busqueda.h"
#include "ui_busqueda.h"
#include <dbmanejo.h>
#include <consulta.h>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QSqlTableModel>
#include <notapedido.h>


Busqueda::Busqueda(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Busqueda)
{
    ui->setupUi(this);

    ModBusqueda = new QSqlRelationalTableModel(this,dbManejo::dbRetorna());

    FilBusqueda = new QSortFilterProxyModel(this);

    FilBusqueda = new QSortFilterProxyModel(this);
    FilBusqueda->setFilterCaseSensitivity(Qt::CaseInsensitive);
    FilBusqueda->setFilterKeyColumn(-1); //-1 ordena por todas la columnas
    FilBusqueda->setFilterFixedString("");

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

    ModBusqueda->setTable("Monitores");
    ModBusqueda->select();

    FilBusqueda->setSourceModel(ModBusqueda);
    tipo= 1;
}

void Busqueda::on_Perifericos_clicked()
{
    ModBusqueda->setTable("Perifericos");
    ModBusqueda->select();

    FilBusqueda->setSourceModel(ModBusqueda);
    tipo = 2;

}

void Busqueda::on_Instalaciones_clicked()
{
    ModBusqueda->setTable("Instalaciones");
    ModBusqueda->select();

    FilBusqueda->setSourceModel(ModBusqueda);
    tipo = 3;

}

//void Busqueda::on_BusquedaTbl_doubleClicked(const QModelIndex &index)
//{
////    QString AgenteTexto;
////    AgenteTexto.clear();
////    AgenteTexto.append(ui->BusquedaTbl->model()->data(index).toString());
////    ui->FIngreso->setText(dControl.currentDateTime().toString("ddMMyyyy"));
////    FilRepDatos->setFilterFixedString(AgenteTexto);
//}

void Busqueda::on_AgenteBuscar_textChanged(const QString &arg1)
{
    FilBusqueda->setFilterFixedString(arg1);
}

void Busqueda::on_BusquedaTbl_clicked(const QModelIndex &index)
{
    int Rep, fila,columna ;
    fila = index.row();
    columna = ModBusqueda->fieldIndex("repid");

    bool ok;
    if(ui->EditarEquipo->isChecked()){

        columna = ModBusqueda->fieldIndex("id");
        reparacioneseditar *VentanaEdicion  = new reparacioneseditar(this);
        VentanaEdicion->setModal(true);
        VentanaEdicion->show();
        Rep = ui->BusquedaTbl->model()->data(ui->BusquedaTbl->model()->index(fila,columna)).toInt(&ok);

        VentanaEdicion->SetDatos(tipo,QString::number(Rep,10));

    }
    else {
    Consulta *ConsultaVentana = new Consulta(this);
    Rep = ui->BusquedaTbl->model()->data(ui->BusquedaTbl->model()->index(fila,columna)).toInt(&ok);
    ConsultaVentana->TragajoID(Rep);
    ConsultaVentana->setModal(true);
    ConsultaVentana->show();
    }
}

void Busqueda::on_Instalaciones_2_clicked()
{
        NotaPedido *NP = new NotaPedido (this);
        NP->setModal(true);
        NP->show();
        NP->InfoFallas(ui->AgenteBuscar->text());
}

void Busqueda::on_EditarEquipo_clicked()
{

}
