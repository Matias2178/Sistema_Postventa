#include "consulta.h"
#include "ui_consulta.h"
#include <dbmanejo.h>
#include <QDebug>
#include <QDateTime>
#include <QMessageBox>
dbManejo dbConsulta;


Consulta::Consulta(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Consulta)
{
    ui->setupUi(this);

}
Consulta::~Consulta()
{
    delete ui;
}
void Consulta::TragajoID(int Id)
{
    ui->RepID->setText(QString::number(Id,10));
    dbConsulta.CargarIngreso(*ui->TrabajoConsulta,Id);

    QString Conf;
    Conf.append("SELECT * FROM Reparaciones WHERE id =="+ QString::number(Id,10));

    QSqlQuery consultar;
    consultar.prepare(Conf);
    consultar.exec();
    consultar.next();
    ui->lAgente->setText(consultar.value("agente").toByteArray());
}

void Consulta::on_CambioFrep_clicked()
{
    QSqlQuery Consulta,Cambio;
    QString Fecha1, Fecha, id,Tabla;
    QString Datos;
    int i;

    for (i=0;i<6;i++)
    {
        Tabla.clear();
        if(i==1){
            Tabla.append("Reparaciones");
        }
        else if (i==2) {
            Tabla.append("Perifericos");
        }
        else if (i==3) {
            Tabla.append("Monitores");
        }
        else if (i==4) {
            Tabla.append("Instalaciones");
        }
        else if (i==5) {
            Tabla.append("Caudalimetro");
        }
        Datos.clear();
        Datos.append("SELECT id,frep FROM '" + Tabla + "'");
        Consulta.prepare(Datos);
        Consulta.exec();

  //      qDebug() << Datos;

        while(Consulta.next())
        {
            Fecha1.clear();
            Fecha.clear();
            id.clear();
            Fecha1 = Consulta.value(1).toString();
            id.append(Consulta.value(0).toString());
            Fecha.append(CambioFecha(Fecha1));
            if (Fecha.isEmpty()){
                continue;
            }
            Datos.clear();
            Datos.append("UPDATE '" + Tabla + "' SET frep = '" + Fecha + "' WHERE id ='" + id+ "'");
            Cambio.prepare(Datos);

        //    qDebug() << Datos;

            if(!Cambio.exec())
            {
                QMessageBox::critical(this,tr("Tabla Productos"),
                                      tr("Falla al crear la tabla\n"
                                         "%1").arg(Cambio.lastError().text()));
                return;
            }
        }
    }
}

QString Consulta::CambioFecha(QString fecha)
{
    int ind, dato;
    QString fecha2;
    fecha2.clear();
    ind = fecha.lastIndexOf('/');
    if(ind <1)
        return fecha2;

    dato = fecha.size();
    fecha2.append(fecha.mid(ind+1,dato)+'-');

    fecha.truncate(ind);
    ind = fecha.lastIndexOf('/');
    dato = fecha.size();
    fecha2.append(fecha.mid(ind+1,dato)+'-');
    fecha.truncate(ind);
    ind = fecha.lastIndexOf('/');
    dato = fecha.size();
    fecha2.append(fecha.mid(ind+1,dato));
  //   qDebug ()  << Fecha << Fecha2;
    return fecha2;

}
