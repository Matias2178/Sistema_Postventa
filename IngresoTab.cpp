#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDebug>

void MainWindow::on_ReparacionesGuardar_clicked()
{
    QString Ingreso;

    Ingreso.clear();
    if(!ConfInicio)
    {
        QString Conf;
        Conf.clear();
        Conf.append("INSERT INTO Reparaciones("
                    "agente,"
                    "fing,"
                    "frep)"
                    "VALUES("
                    "'"+ui->AgenteNombre->currentText()+"',"
                    "'"+ui->FechaIngreso->text()+"',"
                    "''"
                    ");");

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
            qDebug() << "Se Agrego Item bien" << insertar.value(0).toByteArray().constData();
            ReparacionesActualizar(ui->AgenteNombre->currentText());
        }
    }
}
void MainWindow::on_ReparacionesEditar_clicked()
{
    QString Conf;
    Conf.append("UPDATE Reparaciones SET "
                "agente ="
                "'"+ui->AgenteNombre->currentText()+"'"
                ",fing ="
                "'"+ui->FechaIngreso->text()+"'"
                " WHERE id ="
                ""+QString::number(RepIndice,10)+""
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
        qDebug() << "Se Edito el item " << RepIndice;
        ReparacionesActualizar(ui->AgenteNombre->currentText());
    }
    RepIndice = 0;
    ui->ReparacionesEditar->setEnabled(false);
    ui->ReparacionesBorrar->setEnabled(false);
}

void MainWindow::on_ReparacionesBorrar_clicked()
{
    QString Conf;
    Conf.append("DELETE FROM Reparaciones "
                " WHERE id ="
                ""+QString::number(RepIndice,10)+""
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
        qDebug() << "se borro un item" << RepIndice ;
        ReparacionesActualizar(ui->AgenteNombre->currentText());
    }
    RepIndice = 0;
    ui->ReparacionesBorrar->setEnabled(false);
    ui->ReparacionesEditar->setEnabled(false);
}

void MainWindow::on_DatosReparaciones_clicked(const QModelIndex &index)
{
    //    ui->AgenteNombre->setText(ui->DatosReparaciones->item(index.row(),1)->text());
        ui->FechaIngreso->setText(ui->DatosReparaciones->item(index.row(),2)->text());
        ui->RepID->setText(ui->DatosReparaciones->item(index.row(),0)->text());
        ui->TrabRepID->setText(ui->DatosReparaciones->item(index.row(),0)->text());
        ui->MonRepID->setText(ui->DatosReparaciones->item(index.row(),0)->text());
        ui->PerRepID->setText(ui->DatosReparaciones->item(index.row(),0)->text());
        ui->InstRepID->setText(ui->DatosReparaciones->item(index.row(),0)->text());
        RepID = ui->DatosReparaciones->item(index.row(),0)->text().toInt();

        ui->ReparacionesBorrar->setEnabled(true);
        ui->ReparacionesEditar->setEnabled(true);
        IngresoActualizar(RepID);
}
void MainWindow::on_AgenteNombre_activated(const QString &arg1)
{
    ui->Agente->setText(arg1);
    ReparacionesActualizar(arg1);

}

void MainWindow::on_IngresoGuardar_2_clicked()
{
    QString Ingreso;

    Ingreso.clear();
    if(!ConfInicio)
    {
        QString Conf;
        Conf.clear();
        Conf.append("INSERT INTO Ingreso("
                    "nombre,"
                    "cant,"
                    "obs,"
                    "repid)"
                    "VALUES("
                    "'"+ui->IngresoEquipo->currentText()+"',"
                    "'"+ui->IngresoCantidad->text()+"',"
                    "'"+ui->IngresoComentario->placeholderText()+"',"
                    "'"+ui->RepID->text()+"'"
                    ");");

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
            ReparacionesActualizar(ui->AgenteNombre->currentText());
        }
    }
    IngresoActualizar(RepID);
}

void MainWindow::on_IngresoEditar_clicked()
{
    QString Conf;
    Conf.append("UPDATE Reparaciones SET "
                "nombre ="
                "'"+ui->IngresoEquipo->currentText()+"',"
                ",cant,"
                "'"+ui->IngresoCantidad->text()+"',"
                ",obs,"
                "'"+ui->IngresoComentario->placeholderText()+"',"
                ",repid"
                "'"+ui->RepID->text()+"'"
                " WHERE id ="
                ""+QString::number(IngIndice,10)+""
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
        qDebug() << "Se Edito el item " << IngIndice;
        ReparacionesActualizar(ui->AgenteNombre->currentText());
    }
    IngresoActualizar(RepID);
    IngIndice = 0;
    ui->IngresoEditar->setEnabled(false);
    ui->IngresoBorrar->setEnabled(false);
}

void MainWindow::on_IngresoBorrar_clicked()
{
    QString Conf;
    Conf.append("DELETE FROM Agente "
                " WHERE id ="
                ""+QString::number(IngIndice,10)+""
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
        qDebug() << "se borro un item" << IngIndice ;
    //    AgnetesActualizar();
    }
    IngresoActualizar(RepID);
    IngIndice = 0;
    ui->IngresoBorrar->setEnabled(false);
    ui->IngresoEditar->setEnabled(false);
}

void MainWindow::on_DatosIngreso_clicked(const QModelIndex &index)
{
    //    ui->AgenteNombre->setText(ui->DatosIngreso->item(index.row(),1)->text());
        ui->FechaIngreso->setText(ui->DatosIngreso->item(index.row(),2)->text());
        ui->RepID->setText(ui->DatosIngreso->item(index.row(),0)->text());
        ui->TrabRepID->setText(ui->DatosReparaciones->item(index.row(),0)->text());
        ui->MonRepID->setText(ui->DatosReparaciones->item(index.row(),0)->text());
        ui->PerRepID->setText(ui->DatosReparaciones->item(index.row(),0)->text());
        ui->InstRepID->setText(ui->DatosReparaciones->item(index.row(),0)->text());
        IngIndice = ui->DatosIngreso->item(index.row(),0)->text().toInt();

        ui->ReparacionesBorrar->setEnabled(true);
        ui->ReparacionesEditar->setEnabled(true);
}
