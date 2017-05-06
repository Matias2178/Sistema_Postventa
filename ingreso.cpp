#include "ingreso.h"
#include "ui_ingreso.h"
#include <QDateTime>
#include <QMessageBox>
#include <QDebug>

QDateTime dControl;

Ingreso::Ingreso(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Ingreso)
{

    ui->setupUi(this);
    ui->FIngreso->setInputMask("00/00/0000");
    ui->FIngreso->setText(dControl.currentDateTime().toString("ddMMyyyy"));
    AgenteCargar();
    ReparacionesAct("*");
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
    ReparacionesAct(arg1);
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
                "operario)"
                "VALUES("
                "'"+ui->Agente->currentText()+"',"
                "'"+ui->FIngreso->text()+"',"
                "'',"
                "''"
                ");");

        QSqlQuery insertar;
        insertar.prepare(Conf);
        qDebug() << "error:" << insertar.lastError();
        if(!insertar.exec())
        {
            QMessageBox::critical(this,tr("Tabla Reparaciones"),
                                  tr("Falla guardado de datos"));
        }

        ReparacionesAct(ui->Agente->currentText());
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
    editar.prepare(Conf);
    if(!editar.exec())
    {
        QMessageBox::critical(this,tr("Tabla Reparaciones"),
                              tr("Falla edicion de datos"));
    }
    ReparacionesAct(ui->Agente->currentText());
    ui->RepEditar->setEnabled(false);
    ui->RepBorrar->setEnabled(false);
}

void Ingreso::on_RepBorrar_clicked()
{
    QString Conf;
    qDebug () << IndiceRep;
    Conf.append("DELETE FROM Reparaciones "
                " WHERE id ="
                ""+ui->ID_Rep->text()+""
                "");

    QSqlQuery borrar;
    borrar.prepare(Conf);
    if(!borrar.exec())
    {
        QMessageBox::critical(this,tr("Tabla Reparaciones"),
                              tr("Falla borrado de datos"));
    }

    ReparacionesAct(ui->Agente->currentText());

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
    IngActualizar(IngresoID);
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
                "cant,"
                "obs,"
                "repid)"
                "VALUES("
                "'"+ui->IngEquipo->currentText()+"',"
                "'"+ui->IngCant->text()+"',"
                "'"+ui->IngCom->placeholderText()+"',"
                "'"+ui->ID_Rep->text()+"'"
                ");");

    QSqlQuery insertar;
    insertar.prepare(Conf);
    if(!insertar.exec())
    {
        QMessageBox::critical(this,tr("Tabla Reparaciones"),
                              tr("Falla guardado de datos"));
    }
    IngActualizar(IngresoID);
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
                "cant ="
                "'"+ui->IngCant->text()+"',"
                "obs ="
                "'"+ui->IngCom->toPlainText()+"',"
                "repid ="
                "'"+ui->ID_Rep->text()+"'"
                " WHERE id ="
                ""+QString::number(IndiceIng,10)+""
                "");
    QSqlQuery editar;
    editar.prepare(Conf);
    if(!editar.exec())
    {
        QMessageBox::critical(this,tr("Tabla Reparaciones"),
                              tr("Falla edicion de datos"));
    }

    IngActualizar(IngresoID);
    IndiceIng = 0;
    ui->IngEditar->setEnabled(false);
    ui->IngBorrar->setEnabled(false);
}

void Ingreso::on_IngBorrar_clicked()
{

  QString Conf;
    Conf.append("DELETE FROM Ingreso "
                " WHERE id ="
                ""+QString::number(IndiceIng,10)+""
                "");
    QSqlQuery borrar;
    borrar.prepare(Conf);
    if(!borrar.exec())
    {
        QMessageBox::critical(this,tr("Tabla Ingreso"),
                              tr("Falla borrado de datos"));
    }
    IngActualizar(IngresoID);
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

void Ingreso::ReparacionesAct(const QString &arg1)
{
    QString Conf;
    bool todos;
    if((arg1 == "*")|| (arg1 == "Seleccionar"))
    {
        todos = true;
    }
    else
    {
        todos = false;
    }
    Conf.append("SELECT * FROM Reparaciones");

    QSqlQuery consultar;
    consultar.prepare(Conf);
    if(!consultar.exec())
    {
        QMessageBox::critical(this,tr("Tabla Reparaciones"),
                              tr("Falla Lectura de datos"));
    }
    int fila  = 0;

    ui->RepTabla->setRowCount(0);
    while(consultar.next())
    {
        if((arg1 == consultar.value(1).toByteArray().constData())|| todos )
        {
            ui->RepTabla->insertRow(fila);
            ui->RepTabla->setRowHeight(fila,20);
            ui->RepTabla->setItem(fila,0,new QTableWidgetItem (consultar.value(0).toByteArray().constData()));
            ui->RepTabla->setItem(fila,1,new QTableWidgetItem (consultar.value(1).toByteArray().constData()));
            ui->RepTabla->setItem(fila,2,new QTableWidgetItem (consultar.value(2).toByteArray().constData()));
            ui->RepTabla->setItem(fila,3,new QTableWidgetItem (consultar.value(3).toByteArray().constData()));
            ui->RepTabla->setItem(fila,4,new QTableWidgetItem (consultar.value(4).toByteArray().constData()));
            fila ++;
        }
    }
    ui->RepTabla->setColumnWidth(0,25);
    ui->RepTabla->setColumnWidth(1,200);
    ui->RepTabla->setColumnWidth(2,80);
    ui->RepTabla->setColumnWidth(3,80);
    ui->RepTabla->scrollToBottom();
}

void Ingreso::AgenteCargar()
{
    QString Conf;
    Conf.append("SELECT * FROM Agente");

    QSqlQuery consultar;
    consultar.prepare(Conf);
    if(!consultar.exec())
    {
        QMessageBox::critical(this,tr("Tabla Agente"),
                              tr("Falla guardado de datos"));
    }
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

void Ingreso::IngActualizar(int ID)
{
    QString Conf;
    if (!ID)
        return;
    Conf.append("SELECT * FROM Ingreso");

    QSqlQuery consultar;
    consultar.prepare(Conf);
    if(!consultar.exec())
    {
        QMessageBox::critical(this,tr("Tabla Ingreso"),
                              tr("Falla carga de datos"));
    }

    int fila  = 0;

    ui->IngresoTabla->setRowCount(0);

    while(consultar.next())
    {
        if(ID == consultar.value("repid").toByteArray().toInt())
        {
            ui->IngresoTabla->insertRow(fila);
            ui->IngresoTabla->setRowHeight(fila,20);
            ui->IngresoTabla->setItem(fila,0,new QTableWidgetItem (consultar.value(0).toByteArray().constData()));
            ui->IngresoTabla->setItem(fila,1,new QTableWidgetItem (consultar.value(1).toByteArray().constData()));
            ui->IngresoTabla->setItem(fila,2,new QTableWidgetItem (consultar.value(2).toByteArray().constData()));
            ui->IngresoTabla->setItem(fila,3,new QTableWidgetItem (consultar.value(3).toByteArray().constData()));
            ui->IngresoTabla->setItem(fila,4,new QTableWidgetItem (consultar.value(4).toByteArray().constData()));
            fila ++;
        }
    }
    ui->IngresoTabla->setColumnWidth(0,50);
    ui->IngresoTabla->setColumnWidth(1,100);
    ui->IngresoTabla->setColumnWidth(2,50);
    ui->IngresoTabla->setColumnWidth(3,200);
    ui->IngresoTabla->setColumnWidth(4,50);
    ui->IngresoTabla->scrollToBottom();
}

void Ingreso::IngresoProductos()
{
    QString Conf;
    Conf.append("SELECT * FROM Productos");

    QSqlQuery consultar;
    consultar.prepare(Conf);
    if(!consultar.exec())
    {
        QMessageBox::critical(this,tr("Tabla Productos"),
                              tr("Falla carga de datos"));
    }
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

