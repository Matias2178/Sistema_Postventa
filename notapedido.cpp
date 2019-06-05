#include "notapedido.h"
#include "ui_notapedido.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

NotaPedido::NotaPedido(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NotaPedido)
{
    ui->setupUi(this);

}

NotaPedido::~NotaPedido()
{
    delete ui;
}

void NotaPedido::SetDatos(int id)
{
   QSqlQuery consultar, cons;
   int fila = 0;
   int Contador=0;

   cons.prepare("SELECT SUM (cantidad) FROM Ingreso WHERE repid ="+ QString::number(id,10));
// cons.prepare("SELECT * FROM Ingreso WHERE repid ='317'");
   cons.exec();
   cons.next();
   ui->ItemIngresado->setText(cons.value(0).toString());

//qDebug ()<< id <<cons.value(1).toString();

   consultar.prepare("SELECT count(*), nombre, bonif  FROM Monitores WHERE repid=" + QString::number(id,10)+" group by nombre, bonif");
   consultar.exec();
   while(consultar.next())
   {
       ui->TablaNP->insertRow(fila);
       ui->TablaNP->setItem(fila,0, new QTableWidgetItem(consultar.value(0).toString()));
       ui->TablaNP->setItem(fila,1, new QTableWidgetItem(consultar.value(1).toString()));
       ui->TablaNP->setItem(fila,3, new QTableWidgetItem(consultar.value(2).toString()));
       cons.prepare("SELECT desc FROM Productos WHERE producto = '" + consultar.value(1).toString() + "'");
       cons.exec();
       cons.next();
       ui->TablaNP->setItem(fila,2,new QTableWidgetItem(cons.value(0).toString()));
       Contador += consultar.value(0).toInt();
   }
   consultar.prepare("SELECT count(*), nombre, bonif  FROM Perifericos WHERE repid=" + QString::number(id,10)+" group by nombre, bonif");
   consultar.exec();
   while(consultar.next())
   {
       ui->TablaNP->insertRow(fila);
       ui->TablaNP->setItem(fila,0, new QTableWidgetItem(consultar.value(0).toString()));
       ui->TablaNP->setItem(fila,1, new QTableWidgetItem(consultar.value(1).toString()));
       ui->TablaNP->setItem(fila,3, new QTableWidgetItem(consultar.value(2).toString()));
       cons.prepare("SELECT desc FROM Productos WHERE producto = '" + consultar.value(1).toString() + "'");
       cons.exec();
       cons.next();
       ui->TablaNP->setItem(fila,2,new QTableWidgetItem(cons.value(0).toString()));
       Contador += consultar.value(0).toInt();
   }

   consultar.prepare("SELECT count(*), nombre, bonif  FROM Caudalimetro WHERE repid=" + QString::number(id,10)+" group by nombre, bonif order by nombre");
   consultar.exec();
   while(consultar.next())
   {
       ui->TablaNP->insertRow(fila);
       ui->TablaNP->setItem(fila,0, new QTableWidgetItem(consultar.value(0).toString()));
       ui->TablaNP->setItem(fila,1, new QTableWidgetItem(consultar.value(1).toString()));
       ui->TablaNP->setItem(fila,3, new QTableWidgetItem(consultar.value(2).toString()));
       cons.prepare("SELECT desc FROM Productos WHERE producto = '" + consultar.value(1).toString() + "'");
       cons.exec();
       cons.next();
       ui->TablaNP->setItem(fila,2,new QTableWidgetItem(cons.value(0).toString()));
       Contador += consultar.value(0).toInt();
   }

   consultar.prepare("SELECT count(*), nombre, bonif  FROM Instalaciones WHERE repid=" + QString::number(id,10)+" group by nombre, bonif");
   consultar.exec();
   while(consultar.next())
   {
       ui->TablaNP->insertRow(fila);
       ui->TablaNP->setItem(fila,0, new QTableWidgetItem(consultar.value(0).toString()));
       ui->TablaNP->setItem(fila,1, new QTableWidgetItem(consultar.value(1).toString()));
       ui->TablaNP->setItem(fila,3, new QTableWidgetItem(consultar.value(2).toString()));
       cons.prepare("SELECT desc FROM Productos WHERE producto = '" + consultar.value(1).toString() + "'");
       cons.exec();
       cons.next();
       ui->TablaNP->setItem(fila,2,new QTableWidgetItem(cons.value(0).toString()));
       Contador += consultar.value(0).toInt();
   }
   ui->ItemProcesado->setText(QString::number(Contador,10));
}

void NotaPedido::InfoFallas(QString Codigo)
{
    QSqlQuery consultar, cons;
    int fila = 0;
    int Contador=0;
    QString lFalla;
    QStringList Etiquetas {"Cantidad", "Codigo","Descripcion","CodFalla", "Falla"};
  //  ui->TablaNP->insertColumn(1);
    ui->TablaNP->setHorizontalHeaderLabels(Etiquetas);

    qDebug () << Codigo;
    Codigo.toUpper();
    qDebug () << Codigo;

    consultar.prepare("SELECT count(*), nombre, falla  FROM Perifericos WHERE nombre ='" + Codigo +"' group by falla");
    consultar.exec();
    while(consultar.next())
    {
        ui->TablaNP->insertRow(fila);
        ui->TablaNP->setItem(fila,0, new QTableWidgetItem(consultar.value(0).toString()));
        ui->TablaNP->setItem(fila,1, new QTableWidgetItem(consultar.value(1).toString()));
        ui->TablaNP->setItem(fila,3, new QTableWidgetItem(consultar.value(2).toString()));
        cons.prepare("SELECT desc FROM Productos WHERE producto = '" + consultar.value(1).toString() + "'");
        cons.exec();
        cons.next();
        ui->TablaNP->setItem(fila,2,new QTableWidgetItem(cons.value(0).toString()));
        lFalla.clear();
        lFalla.append(consultar.value(2).toString());
        cons.prepare("SELECT descripcion FROM FallasLista WHERE grupo LIKE 'P%' AND nombre ='" + lFalla  + "'");

        cons.exec();
        cons.next();
        qDebug () << lFalla;
        ui->TablaNP->setItem(fila,4,new QTableWidgetItem(cons.value(0).toString()));

        Contador += consultar.value(0).toInt();
    }
}

void NotaPedido::CargaDiferencia(int id)
{
    QSqlQuery consultar, cons;
    bool ok;
    int fila = 0;
    int Contador=0;
    int i,ingresos=0;
    QString Texto, Equipo;

    QStringList Etiquetas {"Codigo","Descripcion","Ingreso", "Procesado", "Falla"};
  //  ui->TablaNP->insertColumn(1);
    ui->TablaNP->setHorizontalHeaderLabels(Etiquetas);

    for(i=0;i<5;i++)
    {
        Equipo.clear();

        if (i==0)
        {
            Equipo.append("Monitores");
        }
        else if (i==1)
        {
            Equipo.append("Perifericos");
        }
        else if (i==2)
        {
            Equipo.append("Instalaciones");
        }
        else if (i==3)
        {
            Equipo.append("Insumos");
        }
        else if (i==4)
        {
            Equipo.append("Caudalimetro");
        }


    consultar.prepare("SELECT SUM (cantidad), nombre  FROM'" + Equipo + "'WHERE repid=" + QString::number(id,10)+" group by nombre");
    consultar.exec();
    while(consultar.next())
    {
        Texto.clear();
        Texto.append(consultar.value(1).toString());
  //      qDebug () <<Texto;

    //    cons.prepare("SELECT SUM (cantidad), nombre FROM Ingreso WHERE repid ="+ QString::number(id,10));
        cons.prepare("SELECT SUM (cantidad), nombre FROM Ingreso WHERE nombre ='"+ Texto + "' AND repid ="+ QString::number(id,10));
        cons.exec();
        cons.next();

        qDebug () << Texto << cons.value(0).toString()  << cons.value(1).toString();
        ingresos =  ingresos + cons.value(0).toInt(&ok);
        ui->TablaNP->insertRow(fila);
        ui->TablaNP->setItem(fila,3, new QTableWidgetItem(consultar.value(0).toString()));
        ui->TablaNP->setItem(fila,0, new QTableWidgetItem(consultar.value(1).toString()));
        ui->TablaNP->setItem(fila,2, new QTableWidgetItem(cons.value(0).toString()));
        cons.prepare("SELECT desc FROM Productos WHERE producto = '" + consultar.value(1).toString() + "'");
        cons.exec();
        cons.next();
        ui->TablaNP->setItem(fila,1,new QTableWidgetItem(cons.value(0).toString()));
        Contador += consultar.value(0).toInt();
    }
    }
    ui->ItemProcesado->setText(QString::number(Contador,10));
    ui->ItemIngresado->setText(QString::number(ingresos,10));
}

