#include "filtroinformacion.h"
#include "ui_filtroinformacion.h"
#include <calendario.h>
#include <variables.h>
#include <QDebug.h>

FiltroInformacion::FiltroInformacion(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FiltroInformacion)
{
    ui->setupUi(this);
}

FiltroInformacion::~FiltroInformacion()
{
    delete ui;
}

void FiltroInformacion::on_bFechaInicio_clicked()
{
    Calendario *CalendarioVentana = new Calendario(this);
    CalendarioVentana->setModal(true);
    CalendarioVentana->show();
    connect (CalendarioVentana, SIGNAL(Enviar()),this, SLOT(ActFechaInicio()));
}

void FiltroInformacion::on_bFechaFin_clicked()
{
    Calendario *CalendarioVentana = new Calendario(this);
    CalendarioVentana->setModal(true);
    CalendarioVentana->show();
    connect (CalendarioVentana, SIGNAL(Enviar()),this, SLOT(ActFechaFin()));
}

void FiltroInformacion::ActFechaInicio()
{
    ui->lFechaInicio->setText(Fechas);
    FechaInicio.clear();
    FechaInicio.append(Fechas);
}

void FiltroInformacion::ActFechaFin()
{
    ui->lFechaFin->setText(Fechas);
    FechaFin.clear();
    FechaFin.append(Fechas);
}

void FiltroInformacion::on_FiltrarDatos_clicked()
{
    QSqlQuery Consulta,Prod,Desc;
    QString IDRep;
    QString Texto;
    int fila = 0;
    qDebug () << FechaInicio << FechaFin;
    Texto.clear();
    Texto.append("SELECT * FROM Reparaciones WHERE date(frep) between date('"
                 + FechaInicio + "') AND date('"+ FechaFin + "')");
    //qDebug () << Texto;
    Consulta.prepare(Texto);
    Consulta.exec();
    while(Consulta.next()){
        IDRep.clear();
        IDRep.append(Consulta.value(0).toString());

  //      qDebug () << Consulta.value(0).toString();

        Prod.prepare("Select * FROM Monitores WHERE repid = " + IDRep );
        Prod.exec();
        while(Prod.next()){
            Texto.clear();
            Texto.append(Prod.value("nombre").toString());
            Desc.prepare("SELECT desc FROM Productos WHERE producto ='" + Texto + "'");
            Desc.exec();
            Desc.next();
            ui->TrabajoPerifericos->insertRow(fila);
            ui->TrabajoPerifericos->setRowHeight(fila,20);
            ui->TrabajoPerifericos->setItem(fila,1,new QTableWidgetItem (Consulta.value("agente").toString()));
            ui->TrabajoPerifericos->setItem(fila,0,new QTableWidgetItem (Consulta.value("frep").toString()));
            ui->TrabajoPerifericos->setItem(fila,2,new QTableWidgetItem ("1"));
            ui->TrabajoPerifericos->setItem(fila,3,new QTableWidgetItem (Desc.value(0).toString()));
            ui->TrabajoPerifericos->setItem(fila,4,new QTableWidgetItem (Prod.value("sn").toString()));
            ui->TrabajoPerifericos->setItem(fila,5,new QTableWidgetItem (" "));
            ui->TrabajoPerifericos->setItem(fila,6,new QTableWidgetItem (Prod.value("vsoft").toString()));
            ui->TrabajoPerifericos->setItem(fila,7,new QTableWidgetItem (Prod.value("falla").toString()));
            ui->TrabajoPerifericos->setItem(fila,8,new QTableWidgetItem (" "));
            ui->TrabajoPerifericos->setItem(fila,9,new QTableWidgetItem (Prod.value("bonif").toString()));
            ui->TrabajoPerifericos->setItem(fila,10,new QTableWidgetItem (Prod.value("obs").toString()));

      //      ui->TrabajoPerifericos->setItem(fila,9,new QTableWidgetItem (Prod.value("agente").toString()));

            fila++;

        }
        Prod.prepare("Select * FROM Perifericos WHERE repid = " + IDRep );
        Prod.exec();
        while(Prod.next()){
            Texto.clear();
            Texto.append(Prod.value("nombre").toString());
            Desc.prepare("SELECT desc FROM Productos WHERE producto ='" + Texto + "'");
            Desc.exec();
            Desc.next();
            ui->TrabajoPerifericos->insertRow(fila);
            ui->TrabajoPerifericos->setRowHeight(fila,20);
            ui->TrabajoPerifericos->setItem(fila,1,new QTableWidgetItem (Consulta.value("agente").toString()));
            ui->TrabajoPerifericos->setItem(fila,0,new QTableWidgetItem (Consulta.value("frep").toString()));
            ui->TrabajoPerifericos->setItem(fila,2,new QTableWidgetItem ("1"));
            ui->TrabajoPerifericos->setItem(fila,3,new QTableWidgetItem (Desc.value(0).toString()));
            ui->TrabajoPerifericos->setItem(fila,4,new QTableWidgetItem (Prod.value("sn").toString()));
            ui->TrabajoPerifericos->setItem(fila,5,new QTableWidgetItem (Prod.value("ffab").toString()));
            ui->TrabajoPerifericos->setItem(fila,6,new QTableWidgetItem (Prod.value("vsoft").toString()));
            ui->TrabajoPerifericos->setItem(fila,7,new QTableWidgetItem (Prod.value("falla").toString()));
            ui->TrabajoPerifericos->setItem(fila,8,new QTableWidgetItem (" "));
            ui->TrabajoPerifericos->setItem(fila,9,new QTableWidgetItem (Prod.value("bonif").toString()));
            ui->TrabajoPerifericos->setItem(fila,10,new QTableWidgetItem (Prod.value("obs").toString()));

            fila++;

        }
        Prod.prepare("Select * FROM Instalaciones WHERE repid = " + IDRep );
        Prod.exec();
        while(Prod.next()){
            Texto.clear();
            Texto.append(Prod.value("nombre").toString());
            Desc.prepare("SELECT desc FROM Productos WHERE producto ='" + Texto+"'");
            Desc.exec();
            Desc.next();
            ui->TrabajoPerifericos->insertRow(fila);
            ui->TrabajoPerifericos->setRowHeight(fila,20);
            ui->TrabajoPerifericos->setItem(fila,1,new QTableWidgetItem (Consulta.value("agente").toString()));
            ui->TrabajoPerifericos->setItem(fila,0,new QTableWidgetItem (Consulta.value("frep").toString()));
            ui->TrabajoPerifericos->setItem(fila,2,new QTableWidgetItem ("1"));
            ui->TrabajoPerifericos->setItem(fila,3,new QTableWidgetItem (Desc.value(0).toString()));
            ui->TrabajoPerifericos->setItem(fila,4,new QTableWidgetItem (Prod.value("sn").toString()));
            ui->TrabajoPerifericos->setItem(fila,5,new QTableWidgetItem (Prod.value("ffab").toString()));
            ui->TrabajoPerifericos->setItem(fila,6,new QTableWidgetItem (Prod.value("vsoft").toString()));
            ui->TrabajoPerifericos->setItem(fila,7,new QTableWidgetItem (""));
            ui->TrabajoPerifericos->setItem(fila,8,new QTableWidgetItem (" "));
            ui->TrabajoPerifericos->setItem(fila,9,new QTableWidgetItem (Prod.value("bonif").toString()));
            ui->TrabajoPerifericos->setItem(fila,10,new QTableWidgetItem (Prod.value("obs").toString()));

            fila++;
        }
        Prod.prepare("Select * FROM Caudalimetro WHERE repid = " + IDRep );
        Prod.exec();
        while(Prod.next()){
            Texto.clear();
            Texto.append(Prod.value("nombre").toString());
            Desc.prepare("SELECT desc FROM Productos WHERE producto ='" + Texto + "'");
            Desc.exec();
            Desc.next();
            ui->TrabajoPerifericos->insertRow(fila);
            ui->TrabajoPerifericos->setRowHeight(fila,20);
            ui->TrabajoPerifericos->setItem(fila,1,new QTableWidgetItem (Consulta.value("agente").toString()));
            ui->TrabajoPerifericos->setItem(fila,0,new QTableWidgetItem (Consulta.value("frep").toString()));
            ui->TrabajoPerifericos->setItem(fila,2,new QTableWidgetItem ("1"));
            ui->TrabajoPerifericos->setItem(fila,3,new QTableWidgetItem (Desc.value(0).toString()));
            ui->TrabajoPerifericos->setItem(fila,4,new QTableWidgetItem (Prod.value("sn").toString()));
            ui->TrabajoPerifericos->setItem(fila,5,new QTableWidgetItem (Prod.value("ffab").toString()));
            ui->TrabajoPerifericos->setItem(fila,6,new QTableWidgetItem (Prod.value("vsoft").toString()));
            ui->TrabajoPerifericos->setItem(fila,7,new QTableWidgetItem (Prod.value("falla").toString()));
            ui->TrabajoPerifericos->setItem(fila,8,new QTableWidgetItem (Prod.value("bonif").toString()));
            ui->TrabajoPerifericos->setItem(fila,9,new QTableWidgetItem (Prod.value("obs").toString()));

            fila++;
        }
    }
    
}
