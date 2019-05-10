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
    QString Fecha1, Fecha2, Fecha, id;
    QDateTime mm;

    int ind, dato;


    Consulta.prepare("SELECT id,frep FROM Reparaciones");
    Consulta.exec();
    while(Consulta.next())
    {
        Fecha1.clear();
        Fecha2.clear();
        Fecha.clear();
        id.clear();
        Fecha1 = Consulta.value(1).toString();
        id.append(Consulta.value(0).toString());

        Fecha.append(Fecha1);

        ind = Fecha1.lastIndexOf('/');
        if(ind <1)
            continue;

        dato = Fecha1.size();
        Fecha2.append(Fecha1.mid(ind+1,dato)+'-');
 //       qDebug () <<ind << dato<< Fecha1 << Fecha2;

        Fecha1.truncate(ind);
        ind = Fecha1.lastIndexOf('/');
        dato = Fecha1.size();
        Fecha2.append(Fecha1.mid(ind+1,dato)+'-');
//       qDebug () <<ind << dato<< Fecha1 << Fecha2;
        Fecha1.truncate(ind);
        ind = Fecha1.lastIndexOf('/');
        dato = Fecha1.size();
        Fecha2.append(Fecha1.mid(ind+1,dato));
         qDebug () << id << Fecha << Fecha2;
         Fecha.clear();
         Fecha.append("UPDATE Reparaciones SET frep = '" + Fecha2 + "' WHERE id ='" + id + "'");
         Cambio.prepare("UPDATE Reparaciones SET frep = '" + Fecha2 + "' WHERE id ='" + id+ "'");
         qDebug() << Fecha;
         if(!Cambio.exec())
         {
             QMessageBox::critical(this,tr("Tabla Productos"),
                                   tr("Falla al crear la tabla\n"
                                  "%1").arg(Cambio.lastError().text()));
             return;
         }
    }
}
