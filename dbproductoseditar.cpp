#include "dbproductoseditar.h"
#include "ui_dbproductoseditar.h"
#include <QMessageBox>
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include "mainwindow.h"
#include <dbmanejo.h>

dbManejo dbProductos;

dbProductosEditar::dbProductosEditar(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dbProductosEditar)
{
    ui->setupUi(this);
    ProductosLeer();

    ui->Editar->setEnabled(false);
    ui->Borrar->setEnabled(false);
}

dbProductosEditar::~dbProductosEditar()
{
    delete ui;
}

void dbProductosEditar::ProductosLeer()
{
    QString Conf;
    Conf.append("SELECT * FROM Productos");

    QSqlQuery consultar;
    if(!consultar.prepare(Conf))
    {
        QMessageBox::critical(this,tr("Tabla Productos"),
                              tr("Falla al crear la tabla\n"
                             "%1").arg(consultar.lastError().text()));
    }
    consultar.exec();
    int fila  = 0;

    ui->DatosProd->setRowCount(0);
    while(consultar.next())
    {
        ui->DatosProd->insertRow(fila);
        ui->DatosProd->setRowHeight(fila,20);
        ui->DatosProd->setItem(fila,0,new QTableWidgetItem (consultar.value(0).toByteArray().constData()));
        ui->DatosProd->setItem(fila,1,new QTableWidgetItem (consultar.value(1).toByteArray().constData()));
        ui->DatosProd->setItem(fila,2,new QTableWidgetItem (consultar.value(2).toByteArray().constData()));
        ui->DatosProd->setItem(fila,3,new QTableWidgetItem (consultar.value(3).toByteArray().constData()));
        ui->DatosProd->setItem(fila,4,new QTableWidgetItem (consultar.value(4).toByteArray().constData()));
        fila ++;
    }
    ui->DatosProd->setColumnWidth(0,40);
    ui->DatosProd->setColumnWidth(1,100);
    ui->DatosProd->setColumnWidth(2,250);
    ui->DatosProd->setColumnWidth(3,80);
    ui->DatosProd->setColumnWidth(4,40);
    ui->DatosProd->sortByColumn(1,Qt::AscendingOrder);
    ui->DatosProd->setSortingEnabled(true);

}

void dbProductosEditar::on_Guardar_clicked()
{
    if (!ui->TipoEdit->currentIndex())
    {
        QMessageBox::information(this,tr("Tipo de producto"),
                                 tr("Seleccionar tipo de Producto"));
        return;
    }
    QString Conf;
    Conf.append("INSERT INTO Productos("
                "producto,"
                "desc,"
                "version,"
                "tipo)"
                "VALUES("
                "'"+ui->ProductoEdit->text()+"',"
                "'"+ui->DescripcionEdit->text()+"',"
                "'"+ui->VersionEdit->text()+"',"
                ""+QString::number(ui->TipoEdit->currentIndex(),10)+""
                ");");
    QSqlQuery insertar;
    insertar.seek(2,true);
    if(!insertar.prepare(Conf))
    {
        QMessageBox::critical(this,tr("Tabla Productos"),
                              tr("Falla al crear la tabla\n"
                             "%1").arg(insertar.lastError().text()));
    }
    insertar.exec();
    ProductosLeer();
}

void dbProductosEditar::on_Editar_clicked()
{
    if (!ui->TipoEdit->currentIndex())
    {
        QMessageBox::information(this,tr("Tipo de producto"),
                                 tr("Seleccionar tipo de Producto"));
        return;
    }
    QString Conf;
    Conf.append("UPDATE Productos SET "
                "producto ="
                "'"+ui->ProductoEdit->text()+"'"
                ",desc ="
                "'"+ui->DescripcionEdit->text()+"'"
                ",version ="
                "'"+ui->VersionEdit->text()+"'"
                ",tipo="
                "'"+QString::number(ui->TipoEdit->currentIndex(),10)+"'"
                " WHERE id ="
                ""+QString::number(Indice,10)+""
                "");

   QSqlQuery editar;
   if(!editar.prepare(Conf))
   {
       QMessageBox::critical(this,tr("Tabla Productos"),
                             tr("Falla al crear la tabla\n"
                            "%1").arg(editar.lastError().text()));
   }
   editar.exec();
   ProductosLeer();

    Indice = 0;
    ui->Editar->setEnabled(false);
    ui->Borrar->setEnabled(false);

}

void dbProductosEditar::on_Borrar_clicked()
{
    dbProductos.BorrarItem("Productos",Indice);
    ProductosLeer();

    Indice = 0;
    ui->Borrar->setEnabled(false);
    ui->Editar->setEnabled(false);
}

void dbProductosEditar::on_DatosProd_clicked(const QModelIndex &index)
{
    ui->ProductoEdit->setText(ui->DatosProd->item(index.row(),1)->text());
    ui->DescripcionEdit->setText(ui->DatosProd->item(index.row(),2)->text());
    ui->VersionEdit->setText(ui->DatosProd->item(index.row(),3)->text());
    ui->TipoEdit->setCurrentIndex(ui->DatosProd->item(index.row(),4)->text().toInt());
    Indice = ui->DatosProd->item(index.row(),0)->text().toInt();
    ui->Borrar->setEnabled(true);
    ui->Editar->setEnabled(true);
}
