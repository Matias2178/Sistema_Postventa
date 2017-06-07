#include "ingreso.h"
#include "ui_ingreso.h"
#include <QDateTime>
#include <QMessageBox>
#include <QDebug>
#include <dbmanejo.h>
QDateTime dControl;

dbManejo dbIngreso;
Ingreso::Ingreso(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Ingreso)
{

    ui->setupUi(this);
    ui->FIngreso->setInputMask("00/00/0000");
    ui->FIngreso->setText(dControl.currentDateTime().toString("ddMMyyyy"));
    AgenteCargar();
    dbIngreso.CargarReparaciones(*ui->RepTabla,"*");
    IngresoProductos();
    ui->RepEditar->setEnabled(false);
    ui->RepBorrar->setEnabled(false);
    ui->IngEditar->setEnabled(false);
    ui->IngBorrar->setEnabled(false);
}

Ingreso::~Ingreso()
{
    delete ui;
}
void Ingreso::on_Agente_activated(const QString &arg1)
{
    ui->FIngreso->setText(dControl.currentDateTime().toString("ddMMyyyy"));
    dbIngreso.CargarReparaciones(*ui->RepTabla,arg1);

}

void Ingreso::on_RepGuardar_clicked()
{
    if(!ui->Agente->currentIndex())
    {
        QMessageBox::critical(this,tr("Datos"),
                              tr("Seleccionar Agente"));
        return;
    }
    QString Ingreso;
    Ingreso.clear();
    QString Conf;
    Conf.clear();
    Conf.append("INSERT INTO Reparaciones("
                "agente,"
                "fing,"
                "frep,"
                "operario,"
                "pres)"
                "VALUES("
                "'"+ui->Agente->currentText()+"',"
                "'"+ui->FIngreso->text()+"',"
                "'',"
                "'',"
                "''"
                ");");

        QSqlQuery insertar;
        if(!insertar.prepare(Conf))
        {
            QMessageBox::critical(this,tr("Tabla Reparaciones"),
                                  tr("Falla al crear la tabla\n"
                                 "%1").arg(insertar.lastError().text()));
        }
        insertar.exec();
        dbIngreso.CargarReparaciones(*ui->RepTabla,ui->Agente->currentText());

}

void Ingreso::on_RepEditar_clicked()
{
    if(!ui->Agente->currentIndex())
    {
        QMessageBox::critical(this,tr("Datos"),
                              tr("Seleccionar Agente"));
        return;
    }
    QString Conf;

    Conf.append("UPDATE Reparaciones SET "
                "agente ="
                "'"+ui->Agente->currentText()+"'"
                ",fing ="
                "'"+ui->FIngreso->text()+"'"
                " WHERE id ="
                ""+ui->ID_Rep->text()+""
                "");
    QSqlQuery editar;

    if(!editar.prepare(Conf))
    {
        QMessageBox::critical(this,tr("Tabla Reparaciones"),
                              tr("Falla edicion de datos"));
    }
    editar.exec();
    dbIngreso.CargarReparaciones(*ui->RepTabla,ui->Agente->currentText());
    ui->RepEditar->setEnabled(false);
    ui->RepBorrar->setEnabled(false);
}

void Ingreso::on_RepBorrar_clicked()
{
    int Item;
    bool ok;
    Item = ui->ID_Rep->text().toInt(&ok,10);
    dbIngreso.BorrarItem("Reparaciones",Item);
    dbIngreso.CargarReparaciones(*ui->RepTabla,ui->Agente->currentText());
    ui->RepBorrar->setEnabled(false);
    ui->RepEditar->setEnabled(false);
}

void Ingreso::on_RepTabla_clicked(const QModelIndex &index)
{
    ui->FIngreso->setText(ui->RepTabla->item(index.row(),2)->text());
    ui->ID_Rep->setText(ui->RepTabla->item(index.row(),0)->text());
    IngresoID = ui->RepTabla->item(index.row(),0)->text().toInt();

    ui->RepBorrar->setEnabled(true);
    ui->RepEditar->setEnabled(true);
    dbIngreso.CargarIngreso(*ui->IngresoTabla,IngresoID);
}

void Ingreso::on_IngGuardar_clicked()
{
    QString Ingreso;
    if(ui->ID_Rep->text().isEmpty())
    {
        QMessageBox::critical(this,tr("Trabajo"),
                                  tr("Seleccionar Trabajo para cargar datos"));
        return;
    }
    if(!ui->IngEquipo->currentIndex() || !ui->IngCant->value())
    {
        QString Equipo, Cantidad;
        Equipo.clear();
        Cantidad.clear();
        if(!ui->IngEquipo->currentIndex()) Equipo.append("seleccionar Equipo\n");
        if(!ui->IngCant->value())Cantidad.append("ingresar cantidad");
        QMessageBox::critical(this,tr("Datos"),
                              tr("Falta %1"
                                 "      %2").arg(Equipo).arg(Cantidad));
        return;
    }
    Ingreso.clear();

    QString Conf;
    Conf.clear();
    Conf.append("INSERT INTO Ingreso("
                "nombre,"
                "sn,"
                "cant,"
                "fact,"
                "obs,"
                "repid)"
                "VALUES("
                "'"+ui->IngEquipo->currentText()+   "',"
                "'"+ui->IngSN->text()+            "',"
                "'"+ui->IngCant->text()+            "',"
                "'"+ui->IngFac->text()+             "',"
                "'"+ui->IngCom->toPlainText()+      "',"
                "'"+ui->ID_Rep->text()+             "'"
                ");");

    QSqlQuery insertar;
    if(!insertar.prepare(Conf))
    {
        QMessageBox::critical(this,tr("Tabla Reparaciones"),
                              tr("Falla guardado de datos\n"
                                 "%1").arg(insertar.lastError().text()));
    }
    insertar.exec();


    dbIngreso.CargarIngreso(*ui->IngresoTabla,IngresoID);
}

void Ingreso::on_IngEditar_clicked()
{
    if(!ui->IngEquipo->currentIndex() || !ui->IngCant->value())
    {
        QString Equipo, Cantidad;
        Equipo.clear();
        Cantidad.clear();
        if(!ui->IngEquipo->currentIndex()) Equipo.append("seleccionar Equipo\n");
        if(!ui->IngCant->value())Cantidad.append("ingresar cantidad");
        QMessageBox::critical(this,tr("Datos"),
                              tr("Falta %1"
                                 "      %2").arg(Equipo).arg(Cantidad));
        return;
    }
    QString Conf;
    Conf.append("UPDATE Ingreso SET "
                "nombre ="
                "'"+ui->IngEquipo->currentText()+"',"
                "sn ="
                "'"+ui->IngSN->text()+"',"
                "cant ="
                "'"+ui->IngCant->text()+"',"
                "fact ="
                "'"+ui->IngFac->text()+"',"
                "obs ="
                "'"+ui->IngCom->toPlainText()+"',"
                "repid ="
                "'"+ui->ID_Rep->text()+"'"
                " WHERE id ="
                ""+QString::number(IndiceIng,10)+""
                "");
    QSqlQuery editar;

    if(!editar.prepare(Conf))
    {
        QMessageBox::critical(this,tr("Tabla Reparaciones"),
                              tr("Falla edicion de datos"));
    }
    editar.exec();
    dbIngreso.CargarIngreso(*ui->IngresoTabla,IngresoID);
    IndiceIng = 0;
    ui->IngEditar->setEnabled(false);
    ui->IngBorrar->setEnabled(false);
}

void Ingreso::on_IngBorrar_clicked()
{
    int Item;
    bool ok;
    Item = ui->ID_Rep->text().toInt(&ok,10);
    dbIngreso.BorrarItem("Ingreso",Item);
    dbIngreso.CargarIngreso(*ui->IngresoTabla,IngresoID);
    IndiceIng = 0;
    ui->IngEditar->setEnabled(false);
    ui->IngBorrar->setEnabled(false);
}

void Ingreso::on_IngresoTabla_clicked(const QModelIndex &index)
{
    bool ok;
    int indice;
    indice = ui->IngEquipo->findText(ui->IngresoTabla->item(index.row(),1)->text());
    ui->IngEquipo->itemText(indice);
    ui->IngCant->setValue(ui->IngresoTabla->item(index.row(),2)->text().toInt(&ok,10));
    IndiceIng = ui->IngresoTabla->item(index.row(),0)->text().toInt();

    ui->IngBorrar->setEnabled(true);
    ui->IngEditar->setEnabled(true);
}

void Ingreso::AgenteCargar()
{
    QString Conf;
    Conf.append("SELECT * FROM Agente");

    QSqlQuery consultar;

    if(!consultar.prepare(Conf))
    {
        QMessageBox::critical(this,tr("Tabla Agente"),
                              tr("Falla guardado de datos"));
    }
    consultar.exec();
    int fila  = 0;
    QStringList Lista1 ;
    Lista1.clear();
    Lista1.append("Seleccionar");
    ui->Agente->clear();
    while(consultar.next())
    {
        Lista1.append(consultar.value(1).toByteArray().constData());
        fila ++;
    }
    ui->Agente->addItems(Lista1);
}



void Ingreso::IngresoProductos()
{
    QString Conf;
    Conf.append("SELECT * FROM Productos");

    QSqlQuery consultar;
    if(!consultar.prepare(Conf))
    {
        QMessageBox::critical(this,tr("Tabla Productos"),
                              tr("Falla guardado de datos"));
    }
    consultar.exec();
    QStringList Lista4;
    Lista4.clear();
    Lista4.append("Seleccionar");
    ui->IngEquipo->clear();

    while(consultar.next())
    {
        Lista4.append(consultar.value(1).toByteArray().constData());
    }
    ui->IngEquipo->addItems(Lista4);
}

