#include "dbagente.h"
#include "ui_dbagente.h"
#include <QMessageBox>
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include "mainwindow.h"

dbAgente::dbAgente(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dbAgente)
{
    ui->setupUi(this);
    AgnetesActualizar();
    ui->Editar->setEnabled(false);
    ui->Borrar->setEnabled(false);
    Indice = 0;
    ui->DatosAgentes->setColumnWidth(0,25);
    ui->DatosAgentes->setColumnWidth(1,200);
}

dbAgente::~dbAgente()
{
    delete ui;
}


void dbAgente::AgnetesActualizar()
{
    QString Conf;

    Conf.append("SELECT * FROM Agente");

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

    ui->DatosAgentes->setRowCount(0);
    while(consultar.next())
    {

        ui->DatosAgentes->insertRow(fila);
        ui->DatosAgentes->setRowHeight(fila,20);
        ui->DatosAgentes->setItem(fila,0,new QTableWidgetItem (consultar.value(0).toByteArray().constData()));
        ui->DatosAgentes->setItem(fila,1,new QTableWidgetItem (consultar.value(1).toByteArray().constData()));
        fila ++;

    }
}

void dbAgente::on_Guardar_clicked()
{
    QString Conf;

    Conf.clear();
    Conf.append("INSERT INTO Agente("
                "agente)"
                "VALUES("
                "'"+ui->AgenteNombre->text()+"'"
                ");");

    qDebug() << Conf;
    QSqlQuery insertar;
    insertar.prepare(Conf);
    if(!insertar.exec())
    {
        qDebug() << "error:" << insertar.lastError();
        QMessageBox::critical(this,tr("Error en un campo"),
                                  tr("Camos incompletos no se guardaron los datos"));
    }
    else
    {
        qDebug() << "Se Agrego Item bien";
        AgnetesActualizar();
    }

}

void dbAgente::on_Editar_clicked()
{

    QString Conf;
    Conf.append("UPDATE Agente SET "
                "agente ="
                "'"+ui->AgenteNombre->text()+"'"
                " WHERE id ="
                ""+QString::number(Indice,10)+""
                "");
    QSqlQuery editar;
    editar.prepare(Conf);
    if(!editar.exec())
    {
        qDebug() << "error:" << editar.lastError();
        QMessageBox::critical(this,tr("Error en un campo"),
                                  tr("Camos incompletos no se guardaron los datos"));
    }
    else
    {
        qDebug() << "Se Edito el item " << Indice;
        AgnetesActualizar();
    }
    Indice = 0;
    ui->Editar->setEnabled(false);
    ui->Borrar->setEnabled(false);
}

void dbAgente::on_Borrar_clicked()
{
    QString Conf;
    Conf.append("DELETE FROM Agente "
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
        qDebug() << "se borro un item" << Indice ;
        AgnetesActualizar();
    }
    Indice = 0;
    ui->Borrar->setEnabled(false);
    ui->Editar->setEnabled(false);
}


void dbAgente::on_DatosAgentes_clicked(const QModelIndex &index)
{
        ui->AgenteNombre->setText(ui->DatosAgentes->item(index.row(),1)->text());
        Indice = ui->DatosAgentes->item(index.row(),0)->text().toInt();

        ui->Borrar->setEnabled(true);
        ui->Editar->setEnabled(true);
}
