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

    QSqlQuery consultar;
    if(!consultar.prepare(Conf))
    {
        QMessageBox::critical(this,tr("Tabla Agente/Operario"),
                              tr("Falla al crear la tabla\n"
                             "%1").arg(consultar.lastError().text()));
    }
    consultar.exec();

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
    QSqlQuery insertar;
    if(!insertar.prepare(Conf))
    {
        QMessageBox::critical(this,tr("Tabla Operario"),
                              tr("Falla al crear la tabla\n"
                             "%1").arg(insertar.lastError().text()));
    }
    insertar.exec();
    AgentesActualizar();

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
    if(!editar.prepare(Conf))
    {
        QMessageBox::critical(this,tr("Tabla Operario"),
                              tr("Falla al crear la tabla\n"
                             "%1").arg(editar.lastError().text()));
    }
    editar.exec();
    AgentesActualizar();
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
    AgenteOperario = arg;
    if(AgenteOperario)
        forro = 45;
    else
        forro = 189;
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
