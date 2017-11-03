#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDebug>
#include <QString>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <variables.h>

void MainWindow::ProductosLeer()
{
    QString Conf;
    Conf.append("SELECT * FROM Productos");

    QSqlQuery consultar;
    if(!consultar.prepare(Conf))
    {
        QMessageBox::critical(this,tr("Tabla Productos"),
                              tr("Falla al leer la tabla\n"
                                 "%1").arg(consultar.lastError().text()));
    }

    consultar.exec();
    int fila  = 0;
    int tipo;
    bool ok;
    QStringList Lista1 , Lista2, Lista3;

    Lista1.clear();
    Lista2.clear();
    Lista3.clear();
//    MonMascaras.clear();
//    MonMascaras.append("");
    Lista1.append("Seleccionar");
    Lista2.append("Seleccionar");
    Lista3.append("Seleccionar");

    while(consultar.next())
    {
        tipo = consultar.value("tipo").toInt(&ok);
        if(tipo == 1)
        {
           Lista1.append(consultar.value("producto").toByteArray().constData());
  //         MonMascaras.append(consultar.value("version").toByteArray().constData());
        }
        else if (tipo == 2)
        {
            Lista2.append(consultar.value("producto").toByteArray().constData());
        }
        else if (tipo == 3)
        {
            Lista3.append(consultar.value("producto").toByteArray().constData());
        }
        fila ++;
    }
}

void MainWindow::CargarDatos(QTableWidget &FALLAS,QString Tipo)
{
       QString Conf;
       QSqlQuery consultar;
       QString Falla;
       int fila;

       Conf.clear();
       Conf.append("SELECT * FROM Fallas");


       if(!consultar.prepare(Conf))
       {
           QMessageBox::critical(this,tr("Tabla Fallas"),
                                 tr("Falla al leer la tabla\n"
                                    "%1").arg(consultar.lastError().text()));
       }
       consultar.exec();
       FALLAS.setRowCount(0);
       fila = FALLAS.rowCount();

       FALLAS.setHorizontalHeaderItem(0, new QTableWidgetItem("Fallas_1"));

       FALLAS.insertRow(fila);
       FALLAS.setRowHeight(fila,20);
       FALLAS.setItem(fila,0,new QTableWidgetItem("Fun_OK"));
       FALLAS.item(fila,0)->setCheckState(Qt::Unchecked);

       while(consultar.next())
       {
           Falla.clear();
           Falla.append(consultar.value(1).toByteArray().constData());
           if(Falla == Tipo)
           {
               Falla.clear();
               Falla.append(consultar.value(2).toByteArray().constData());
               fila = FALLAS.rowCount();
               FALLAS.setRowHeight(fila,10);
               FALLAS.insertRow(fila);
               FALLAS.setRowHeight(fila,20);
               FALLAS.setItem(fila,0,new QTableWidgetItem(Falla) );
               FALLAS.item(fila,0)->setCheckState(Qt::Unchecked);
           }
           fila ++;
       }
}

void MainWindow::MonitoresActualizar()
{
    QString Conf;
    Conf.append("SELECT * FROM Monitores");

    QSqlQuery consultar;

    if(!consultar.prepare(Conf))
    {
        QMessageBox::critical(this,tr("Tabla Monitores"),
                              tr("Falla al leer la tabla\n"
                                 "%1").arg(consultar.lastError().text()));
    }
    consultar.exec();
    while(consultar.next())
    {
        if(RepID == consultar.value("repid").toByteArray().toInt())
        {
//            ui->MonitoresDatos->insertRow(fila);
//            ui->MonitoresDatos->setRowHeight(fila,20);
//            ui->MonitoresDatos->setItem(fila,0,new QTableWidgetItem (consultar.value(0).toByteArray().constData()));
//            ui->MonitoresDatos->setItem(fila,1,new QTableWidgetItem (consultar.value(1).toByteArray().constData()));
//            ui->MonitoresDatos->setItem(fila,2,new QTableWidgetItem (consultar.value(2).toByteArray().constData()));
//            ui->MonitoresDatos->setItem(fila,3,new QTableWidgetItem (consultar.value(3).toByteArray().constData()));
//            ui->MonitoresDatos->setItem(fila,4,new QTableWidgetItem (consultar.value(4).toByteArray().constData()));
//            ui->MonitoresDatos->setItem(fila,5,new QTableWidgetItem (consultar.value(5).toByteArray().constData()));
//            ui->MonitoresDatos->setItem(fila,6,new QTableWidgetItem (consultar.value(6).toByteArray().constData()));
//            ui->MonitoresDatos->setItem(fila,7,new QTableWidgetItem (consultar.value(7).toByteArray().constData()));
//            ui->MonitoresDatos->setItem(fila,8,new QTableWidgetItem (consultar.value(8).toByteArray().constData()));
//            fila ++;
        }
    }
//    ui->MonitoresDatos->setColumnWidth(0,50);
//    ui->MonitoresDatos->setColumnWidth(1,100);
//    ui->MonitoresDatos->setColumnWidth(2,50);
//    ui->MonitoresDatos->setColumnWidth(3,80);
//    ui->MonitoresDatos->setColumnWidth(4,100);
//    ui->MonitoresDatos->setColumnWidth(5,50);
//    ui->MonitoresDatos->setColumnWidth(6,50);
//    ui->MonitoresDatos->setColumnWidth(7,100);
//    ui->MonitoresDatos->setColumnWidth(8,50);
}
void MainWindow::PerifericosActualizar()
{
    QString Conf;

//    if (!ID)
//        return;
    Conf.append("SELECT * FROM Perifericos");

    QSqlQuery consultar;

    if(!consultar.prepare(Conf))
    {
        QMessageBox::critical(this,tr("Tabla Perifericos"),
                              tr("Falla al leer la tabla\n"
                                 "%1").arg(consultar.lastError().text()));
    }
    consultar.exec();
    int fila  = 0;

//    ui->PerifericosDatos->setRowCount(0);

    while(consultar.next())
    {
        if(RepID == consultar.value("repid").toByteArray().toInt())
        {
//            ui->PerifericosDatos->insertRow(fila);
//            ui->PerifericosDatos->setRowHeight(fila,20);
//            ui->PerifericosDatos->setItem(fila,0,new QTableWidgetItem (consultar.value(0).toByteArray().constData()));
//            ui->PerifericosDatos->setItem(fila,1,new QTableWidgetItem (consultar.value(1).toByteArray().constData()));
//            ui->PerifericosDatos->setItem(fila,2,new QTableWidgetItem (consultar.value(2).toByteArray().constData()));
//            ui->PerifericosDatos->setItem(fila,3,new QTableWidgetItem (consultar.value(3).toByteArray().constData()));
//            ui->PerifericosDatos->setItem(fila,4,new QTableWidgetItem (consultar.value(4).toByteArray().constData()));
//            ui->PerifericosDatos->setItem(fila,5,new QTableWidgetItem (consultar.value(5).toByteArray().constData()));
//            ui->PerifericosDatos->setItem(fila,6,new QTableWidgetItem (consultar.value(6).toByteArray().constData()));
//            ui->PerifericosDatos->setItem(fila,7,new QTableWidgetItem (consultar.value(7).toByteArray().constData()));
//            ui->PerifericosDatos->setItem(fila,8,new QTableWidgetItem (consultar.value(8).toByteArray().constData()));
//            ui->PerifericosDatos->setItem(fila,9,new QTableWidgetItem (consultar.value(9).toByteArray().constData()));
//            ui->PerifericosDatos->setItem(fila,10,new QTableWidgetItem (consultar.value(10).toByteArray().constData()));
//            ui->PerifericosDatos->setItem(fila,11,new QTableWidgetItem (consultar.value(11).toByteArray().constData()));
//            ui->PerifericosDatos->setItem(fila,12,new QTableWidgetItem (consultar.value(12).toByteArray().constData()));
//            ui->PerifericosDatos->setItem(fila,13,new QTableWidgetItem (consultar.value(13).toByteArray().constData()));
            fila ++;
        }
    }
//    ui->PerifericosDatos->setColumnWidth(0,50);
//    ui->PerifericosDatos->setColumnWidth(1,100);
//    ui->PerifericosDatos->setColumnWidth(2,80);
//    ui->PerifericosDatos->setColumnWidth(3,70);
//    ui->PerifericosDatos->setColumnWidth(4,70);
//    ui->PerifericosDatos->setColumnWidth(5,40);
//    ui->PerifericosDatos->setColumnWidth(6,70);
//    ui->PerifericosDatos->setColumnWidth(7,100);
//    ui->PerifericosDatos->setColumnWidth(8,100);
//    ui->PerifericosDatos->setColumnWidth(9,40);
//    ui->PerifericosDatos->setColumnWidth(10,100);
//    ui->PerifericosDatos->setColumnWidth(11,70);
//    ui->PerifericosDatos->setColumnWidth(12,50);
//    ui->PerifericosDatos->setColumnWidth(13,40);
}
void MainWindow::InstalacionesActualizar()
{
    QString Conf;

    Conf.append("SELECT * FROM Instalaciones");

    QSqlQuery consultar;

    if(!consultar.prepare(Conf))
    {
        QMessageBox::critical(this,tr("Tabla Instalacones"),
                              tr("Falla al leer la tabla\n"
                                 "%1").arg(consultar.lastError().text()));
    }
    consultar.exec();
    int fila  = 0;

//    ui->InstalacionesDatos->setRowCount(0);
    while(consultar.next())
    {

        if(RepID == consultar.value("repid").toByteArray().toInt())
        {
//            ui->InstalacionesDatos->insertRow(fila);
//            ui->InstalacionesDatos->setRowHeight(fila,20);
//            ui->InstalacionesDatos->setItem(fila,0,new QTableWidgetItem (consultar.value(0).toByteArray().constData()));
//            ui->InstalacionesDatos->setItem(fila,1,new QTableWidgetItem (consultar.value(1).toByteArray().constData()));
//            ui->InstalacionesDatos->setItem(fila,2,new QTableWidgetItem (consultar.value(2).toByteArray().constData()));
//            ui->InstalacionesDatos->setItem(fila,3,new QTableWidgetItem (consultar.value(3).toByteArray().constData()));
//            ui->InstalacionesDatos->setItem(fila,4,new QTableWidgetItem (consultar.value(4).toByteArray().constData()));
//            ui->InstalacionesDatos->setItem(fila,5,new QTableWidgetItem (consultar.value(5).toByteArray().constData()));
//            ui->InstalacionesDatos->setItem(fila,6,new QTableWidgetItem (consultar.value(6).toByteArray().constData()));
//            ui->InstalacionesDatos->setItem(fila,7,new QTableWidgetItem (consultar.value(7).toByteArray().constData()));
/*            ui->PerifericosDatos->setItem(fila,8,new QTableWidgetItem (consultar.value(8).toByteArray().constData()));
            ui->PerifericosDatos->setItem(fila,9,new QTableWidgetItem (consultar.value(9).toByteArray().constData()));
            ui->PerifericosDatos->setItem(fila,10,new QTableWidgetItem (consultar.value(10).toByteArray().constData()));
            ui->PerifericosDatos->setItem(fila,11,new QTableWidgetItem (consultar.value(11).toByteArray().constData()));
            ui->PerifericosDatos->setItem(fila,12,new QTableWidgetItem (consultar.value(12).toByteArray().constData()));
*/
            fila ++;
        }
    }
//    ui->PerifericosDatos->setColumnWidth(0,50);
//    ui->PerifericosDatos->setColumnWidth(1,100);
//    ui->PerifericosDatos->setColumnWidth(2,80);
//    ui->PerifericosDatos->setColumnWidth(3,70);
//    ui->PerifericosDatos->setColumnWidth(4,70);
//    ui->PerifericosDatos->setColumnWidth(5,40);
//    ui->PerifericosDatos->setColumnWidth(6,70);
//    ui->PerifericosDatos->setColumnWidth(7,100);

//    ui->PerifericosDatos->setColumnWidth(8,100);
//    ui->PerifericosDatos->setColumnWidth(9,40);
//    ui->PerifericosDatos->setColumnWidth(10,100);
//    ui->PerifericosDatos->setColumnWidth(11,70);
//    ui->PerifericosDatos->setColumnWidth(12,50);

}


