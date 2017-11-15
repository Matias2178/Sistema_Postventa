#include "dbagente.h"
#include "ui_dbagente.h"
#include <QMessageBox>
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include "mainwindow.h"
#include <dbmanejo.h>
#include <QSqlRecord>

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
    ModDatos = new QSqlRelationalTableModel(this,dbManejo::dbRetorna());
    if(AgenteOperario)
    {
        ModDatos->setTable("Agente");
    }
    else
    {
        ModDatos->setTable("Operario");
    }
    ModDatos->setEditStrategy(QSqlTableModel::OnManualSubmit);
    ModDatos->select();

    ui->DatosAgentesTabla->setModel(ModDatos);
    ui->DatosAgentesTabla->sortByColumn(1,Qt::AscendingOrder);
    ui->DatosAgentesTabla->setSortingEnabled(true);
    ui->DatosAgentesTabla->setColumnWidth(0,50);
    ui->DatosAgentesTabla->setColumnWidth(1,200);
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
    int fila;
    fila = ui->DatosAgentesTabla->currentIndex().row();
    ModDatos->removeRow(fila);
    ModDatos->submitAll();

    ui->Borrar->setEnabled(false);
    ui->Editar->setEnabled(false);
}

//void dbAgente::on_DatosAgentes_clicked(const QModelIndex &index)
//{
//        ui->AgenteNombre->setText(ui->DatosAgentes->item(index.row(),1)->text());
//        Indice = ui->DatosAgentes->item(index.row(),0)->text().toInt();

//        ui->Borrar->setEnabled(true);
//        ui->Editar->setEnabled(true);
//}

void dbAgente::SetAgenteOperario(bool arg)
{
    AgenteOperario = arg;
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
}

void dbAgente::on_Guardar_2_clicked()
{
    int filas;
    filas = ModDatos->rowCount();
//    qDebug () << filas;
    filas ++;
//    qDebug () << filas;
//    qDebug () << ModDatos->record(filas);
    QSqlRecord Record = ModDatos->record(filas);
    Record.setValue("Operario", QString(ui->AgenteNombre->text()));
//    qDebug ( ) << Record;
    ModDatos->setRecord(filas,Record);
    ModDatos->insertRecord(filas,Record);

}

void dbAgente::on_DatosAgentesTabla_clicked(const QModelIndex &index)
{
    bool ok;
    int fila = index.row();
    ui->AgenteNombre->setText(ui->DatosAgentesTabla->model()->data(ui->DatosAgentesTabla->model()->index(fila,1)).toString());
    Indice = ui->DatosAgentesTabla->model()->data(ui->DatosAgentesTabla->model()->index(fila,0)).toInt(&ok);
//    qDebug () << Indice;
    ui->Borrar->setEnabled(true);
    ui->Editar->setEnabled(true);
}

