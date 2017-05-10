#include "dbagente.h"
#include "ui_dbagente.h"
#include <QMessageBox>
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include "mainwindow.h"
#include <dbmanejo.h>

dbManejo Agentedb;
dbAgente::dbAgente(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dbAgente)
{
    ui->setupUi(this);

}

dbAgente::~dbAgente()
{
    delete ui;
}

void dbAgente::AgentesActualizar()
{
    QString Conf;
    QStringList Etiquetas;
//    qDebug () <<"Act"<< AgenteOperario;
    if(AgenteOperario)
    {
        Conf.append("SELECT * FROM Agente");
        Etiquetas <<"Id" << "Agentes";
    }
    else
    {
        Conf.append("SELECT * FROM Operario");
        Etiquetas <<"Id" << "Operarios";
    }
//    qDebug () << Etiquetas;
//    qDebug () << Conf;
    QSqlQuery consultar;
    consultar.prepare(Conf);
    if(!consultar.exec())
    {
//        qDebug() << "error:" << consultar.lastError();
    }
    int fila  = 0;
    ui->DatosAgentes->clear();
    ui->DatosAgentes->setHorizontalHeaderLabels(Etiquetas);
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
    if(AgenteOperario)
    {
        Conf.append("INSERT INTO Agente("
                    "agente)"
                    "VALUES("
                    "'"+ui->AgenteNombre->text()+"'"
                                                 ");");
    }
    else
    {
        Conf.append("INSERT INTO Operario("
                    "agente)"
                    "VALUES("
                    "'"+ui->AgenteNombre->text()+"'"
                                                 ");");
    }
 //   qDebug() << Conf;
    QSqlQuery insertar;
    insertar.prepare(Conf);
    if(!insertar.exec())
    {
 //       qDebug() << "error:" << insertar.lastError();
        QMessageBox::critical(this,tr("Error en un campo"),
                                  tr("Camos incompletos no se guardaron los datos"));
    }
    else
    {
//        qDebug() << "Se Agrego Item bien";
        AgentesActualizar();
    }

}

void dbAgente::on_Editar_clicked()
{

    QString Conf;
    if(AgenteOperario)
    {
        Conf.append("UPDATE Agente SET "
                    "agente ="
                    "'"+ui->AgenteNombre->text()+"'"
                    " WHERE id ="
                    ""+QString::number(Indice,10)+""
                    "");
    }
    else
    {
        Conf.append("UPDATE Operario SET "
                    "agente ="
                    "'"+ui->AgenteNombre->text()+"'"
                    " WHERE id ="
                    ""+QString::number(Indice,10)+""
                    "");
    }
    QSqlQuery editar;
    editar.prepare(Conf);
    if(!editar.exec())
    {
 //       qDebug() << "error:" << editar.lastError();
        QMessageBox::critical(this,tr("Error en un campo"),
                                  tr("Camos incompletos no se guardaron los datos"));
    }
    else
    {
//        qDebug() << "Se Edito el item " << Indice;
        AgentesActualizar();
    }
    Indice = 0;
    ui->Editar->setEnabled(false);
    ui->Borrar->setEnabled(false);
}

void dbAgente::on_Borrar_clicked()
{
    QString Conf;

    if(AgenteOperario)
    {
        Agentedb.BorrarItem("Agente",Indice);
    }
    else
    {
        Agentedb.BorrarItem("Operario",Indice);
    }

    AgentesActualizar();
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

void dbAgente::SetAgenteOperario(bool arg)
{
//    qDebug () <<"Antest"<< AgenteOperario;
    AgenteOperario = arg;
    if(AgenteOperario)
        forro = 45;
    else
        forro = 189;
//    qDebug ( )<< forro;
//    qDebug () <<"Despues"<< AgenteOperario;
    if(AgenteOperario)
    {
        ui->lAgenteOp->setText("DB Agente");
        AgentesActualizar();
    }
    else
    {
        ui->lAgenteOp->setText("DB Operario");
        AgentesActualizar();
    }
    ui->Editar->setEnabled(false);
    ui->Borrar->setEnabled(false);
    Indice = 0;
    ui->DatosAgentes->setColumnWidth(0,25);
    ui->DatosAgentes->setColumnWidth(1,200);
}
