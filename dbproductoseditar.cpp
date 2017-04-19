#include "dbproductoseditar.h"
#include "ui_dbproductoseditar.h"
#include <QMessageBox>
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include "mainwindow.h"

dbProductosEditar::dbProductosEditar(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dbProductosEditar)
{
    ui->setupUi(this);
    ProductosLeer();

    ui->Editar->setEnabled(false);
    ui->Borrar->setEnabled(false);
    ui->DatosProd->setColumnWidth(0,25);
    ui->DatosProd->setColumnWidth(3,25);
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
    consultar.prepare(Conf);
    if(!consultar.exec())
    {
        qDebug() << "error:" << consultar.lastError();
    }
    else
    {
        qDebug() << "Se ejecuto bien";

    }
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
        fila ++;
    }

}

void dbProductosEditar::on_Guardar_clicked()
{
    if (!ui->TipoEdit->currentIndex())
    {
        QMessageBox::information(this,tr("Tipo de producto"),
                                 tr("Seleccionar tipo de Producto"));
        return;
    }
 //   qDebug() << ui->ProductoEdit->text();
 //   qDebug()<< ui->VersionEdit->text();
 //   qDebug()<< ui->TipoEdit->currentIndex();
    QString Conf;
    Conf.append("INSERT INTO Productos("
                "producto,"
                "version,"
                "tipo)"
                "VALUES("
                "'"+ui->ProductoEdit->text()+"',"
                "'"+ui->VersionEdit->text()+"',"
                ""+QString::number(ui->TipoEdit->currentIndex(),10)+""
                ");");
    qDebug() << Conf;
    QSqlQuery insertar;
    insertar.seek(2,true);
    insertar.prepare(Conf);
    if(!insertar.exec())
    {
        qDebug() << "error:" << insertar.lastError();
        QMessageBox::critical(this,tr("Error en un campo"),
                                  tr("Camos incompletos no se guardaron los datos"));
    }
    else
    {
        qDebug() << "Se ejecuto bien";
        ProductosLeer();
    }

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
                ",version ="
                "'"+ui->VersionEdit->text()+"'"
                ",tipo="
                "'"+QString::number(ui->TipoEdit->currentIndex(),10)+"'"
                " WHERE id ="
                ""+QString::number(Indice,10)+""
                "");

//    qDebug() << Conf;
//    qDebug() << "UPDATE Productos SET producto = 'cas-1234', version = '0.00r0', tipo = '1' WHERE id = 2";
    QSqlQuery editar;
    editar.prepare(Conf);
//    insertar.prepare("UPDATE Productos SET producto = 'cas-1234', version = '0.00r0', tipo = '1' WHERE id = 2");
    if(!editar.exec())
    {
        qDebug() << "error:" << editar.lastError();
        QMessageBox::critical(this,tr("Error en un campo"),
                                  tr("Camos incompletos no se guardaron los datos"));
    }
    else
    {
        qDebug() << "Se ejecuto bien";
//        qDebug () << "UPDATE Productos SET producto = 'cas-1234', version = '0.00r0', tipo = '1' WHERE id = 1";
        ProductosLeer();
    }
    Indice = 0;
    ui->Editar->setEnabled(false);
    ui->Borrar->setEnabled(false);

}

void dbProductosEditar::on_Borrar_clicked()
{
    QString Conf;
    Conf.append("DELETE FROM Productos "
                " WHERE id ="
                ""+QString::number(Indice,10)+""
                "");

    QSqlQuery borrar;
    borrar.prepare(Conf);
    if(!borrar.exec())
    {
        qDebug() << "error:" << borrar.lastError();
        QMessageBox::critical(this,tr("Error en un campo"),
                                  tr("Camos incompletos no se guardaron los datos"));
    }
    else
    {
        qDebug() << "Se ejecuto bien";
 //       qDebug () << "UPDATE Productos SET producto = 'cas-1234', version = '0.00r0', tipo = '1' WHERE id = 1";
        ProductosLeer();
    }
    Indice = 0;
    ui->Borrar->setEnabled(false);
    ui->Editar->setEnabled(false);
}

void dbProductosEditar::on_DatosProd_clicked(const QModelIndex &index)
{
    ui->ProductoEdit->setText(ui->DatosProd->item(index.row(),1)->text());
    ui->VersionEdit->setText(ui->DatosProd->item(index.row(),2)->text());
    ui->TipoEdit->setCurrentIndex(ui->DatosProd->item(index.row(),3)->text().toInt());
    Indice = ui->DatosProd->item(index.row(),0)->text().toInt();
//    qDebug () << "Index:" << index.row();
//    qDebug () << Indice;
//    qDebug () << ui->DatosProd->item(index.row(),0)->text();
//    qDebug () << ui->DatosProd->item(index.row(),1)->text();
//    qDebug () << ui->DatosProd->item(index.row(),2)->text();
//    qDebug () << ui->DatosProd->item(index.row(),3)->text().toInt();

    ui->Borrar->setEnabled(true);
    ui->Editar->setEnabled(true);
}
