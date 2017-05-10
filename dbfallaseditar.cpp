#include "dbfallaseditar.h"
#include "ui_dbfallaseditar.h"
#include <QMessageBox>
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include "mainwindow.h"
#include <dbmanejo.h>

dbManejo Fallasdb;

dbFallasEditar::dbFallasEditar(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dbFallasEditar)
{
    ui->setupUi(this);
    FallasCargarDatos();
    FallasActualizar("*");
    ui->Editar->setEnabled(false);
    ui->Borrar->setEnabled(false);
    Indice = 0;
    ui->DatosFallas->setColumnWidth(0,40);
    ui->DatosFallas->setColumnWidth(1,80);
    ui->DatosFallas->setColumnWidth(2,50);
    ui->DatosFallas->setColumnWidth(3,200);
    ui->DatosFallas->setColumnWidth(4,80);
    ui->DatosFallas->setColumnWidth(5,60);
}

dbFallasEditar::~dbFallasEditar()
{
    delete ui;
}

void dbFallasEditar::FallasCargarDatos()
{
    QString Conf;
    Conf.append("SELECT producto FROM Productos");

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
    QStringList Lista1;

    Lista1.clear();

    Lista1.append("Seleccionar");

    ui->FallaProducto->clear();
    while(consultar.next())
    {
        Lista1.append(consultar.value(0).toByteArray().constData());

    }
 //   qDebug()<< Lista1;
    ui->FallaProducto->addItems(Lista1);
}

void dbFallasEditar::FallasActualizar(const QString &arg1)
{
    QString Conf;
    bool todos;
    if((arg1 == "*")|| (arg1 == "Seleccionar"))
    {
//        qDebug () << " Imprime Todos " << arg1;
        todos = true;
    }
    else
    {
//        qDebug () << "Imprime Seleccion" << arg1;
        todos = false;
    }
    Conf.append("SELECT * FROM fallas");

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

    ui->DatosFallas->setRowCount(0);
    while(consultar.next())
    {
        if((arg1 == consultar.value(1).toByteArray().constData())|| todos )
        {
            ui->DatosFallas->insertRow(fila);
            ui->DatosFallas->setRowHeight(fila,20);
            ui->DatosFallas->setItem(fila,0,new QTableWidgetItem (consultar.value(0).toByteArray().constData()));
            ui->DatosFallas->setItem(fila,1,new QTableWidgetItem (consultar.value(1).toByteArray().constData()));
            ui->DatosFallas->setItem(fila,2,new QTableWidgetItem (consultar.value(2).toByteArray().constData()));
            ui->DatosFallas->setItem(fila,3,new QTableWidgetItem (consultar.value(3).toByteArray().constData()));
            ui->DatosFallas->setItem(fila,4,new QTableWidgetItem (consultar.value(4).toByteArray().constData()));
            ui->DatosFallas->setItem(fila,5,new QTableWidgetItem (consultar.value(5).toByteArray().constData()));
            fila ++;
        }
    }
    ui->DatosFallas->setColumnWidth(0,40);
    ui->DatosFallas->setColumnWidth(1,80);
    ui->DatosFallas->setColumnWidth(2,50);
    ui->DatosFallas->setColumnWidth(3,200);
    ui->DatosFallas->setColumnWidth(4,80);
    ui->DatosFallas->setColumnWidth(5,60);

}
void dbFallasEditar::on_Guardar_clicked()
{
    if (!ui->FallaProducto->currentIndex())
    {
        QMessageBox::information(this,tr("Producto"),
                                 tr("Hay que seleccionar a que producto hay que cargar la falla"));
        return;
    }
    QString Conf;

    Conf.clear();
    Conf.append("INSERT INTO Fallas("
                "producto,"
                "falla,"
                "descripcion,"
                "fpre,"
                "bonif)"
                "VALUES("
                "'"+ui->FallaProducto->currentText()+"',"
                "'"+ui->FallaEdit->text()+"',"
                "'"+ui->FallaDescripcion->text()+"',"
                "'"+ui->FallaPresupuesto->text()+"',"
                "'"+ui->FallaBonif->text()+"'"
                ");");

//    qDebug() << Conf;
    QSqlQuery insertar;
    if(!insertar.prepare(Conf))
    {
        QMessageBox::critical(this,tr("Error en un campo"),
                                  tr("Camos incompletos no se guardaron los datos\n"
                                     "%1").arg(insertar.lastError().text()));
    }
    if(!insertar.exec())
    {
//        qDebug() << "error:" << insertar.lastError();
        QMessageBox::critical(this,tr("Error en un campo"),
                                  tr("Camos incompletos no se guardaron los datos"));
    }
    else
    {
//        qDebug() << "Se Agrego Item bien";
        FallasActualizar(ui->FallaProducto->currentText());
    }

}

void dbFallasEditar::on_Editar_clicked()
{
    bool ok;
    if (!ui->FallaProducto->currentIndex())
    {
        QMessageBox::information(this,tr("Tipo de producto"),
                                 tr("Seleccionar tipo de Producto"));
        return;
    }
    QString Conf;
    Conf.append("UPDATE Fallas SET "
                "producto ="
                "'"+ui->FallaProducto->currentText()+"'"
                ",falla ="
                "'"+ui->FallaEdit->text()+"'"
                ",descripcion ="
                "'"+ui->FallaDescripcion->text()+"'"
                ",fpre ="
                "'"+ui->FallaPresupuesto->text()+"'"
                ",bonif ="
                "'"+ui->FallaBonif->text()+"'"
                " WHERE id ="
                ""+QString::number(Indice,10)+""
                "");
    QSqlQuery editar;
    qDebug () <<Conf;
    if(!editar.prepare(Conf))
    {
        QMessageBox::critical(this,tr("Error en un campo"),
                                  tr("Camos incompletos no se guardaron los datos\n"
                                     "%1").arg(editar.lastError().text()));
        qDebug () <<editar.lastError();
    }
    if(!editar.exec())
    {
//       qDebug() << "error:" << editar.lastError();
        QMessageBox::critical(this,tr("Error en un campo"),
                                  tr("Camos incompletos no se guardaron los datos"));
    }
    else
    {
//        qDebug() << "Se Edito el item " << Indice;
        FallasActualizar(ui->FallaProducto->currentText());
    }
    Indice = 0;
    ui->Editar->setEnabled(false);
    ui->Borrar->setEnabled(false);
}

void dbFallasEditar::on_Borrar_clicked()
{
    Fallasdb.BorrarItem("Fallas",Indice);
    FallasActualizar(ui->FallaProducto->currentText());
    Indice = 0;
    ui->Borrar->setEnabled(false);
    ui->Editar->setEnabled(false);

}

void dbFallasEditar::on_DatosFallas_clicked(const QModelIndex &index)
{

//    ui->ProductoEdit->setText(ui->DatosProd->item(index.row(),1)->text());
    ui->FallaEdit->setText(ui->DatosFallas->item(index.row(),2)->text());
    ui->FallaDescripcion->setText(ui->DatosFallas->item(index.row(),3)->text());
    ui->FallaPresupuesto->setText(ui->DatosFallas->item(index.row(),4)->text());
    ui->FallaBonif->setText(ui->DatosFallas->item(index.row(),5)->text());
    Indice = ui->DatosFallas->item(index.row(),0)->text().toInt();
//    qDebug () << "Index:" << index.row();//    qDebug () << Indice;
//    qDebug () << ui->DatosProd->item(index.row(),0)->text();
//    qDebug () << ui->DatosProd->item(index.row(),1)->text();
//    qDebug () << ui->DatosProd->item(index.row(),2)->text();
//    qDebug () << ui->DatosProd->item(index.row(),3)->text().toInt();

    ui->Borrar->setEnabled(true);
    ui->Editar->setEnabled(true);
}

void dbFallasEditar::on_FallaProducto_activated(const QString &arg1)
{
    FallasActualizar(arg1);

}
