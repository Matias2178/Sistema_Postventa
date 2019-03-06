#include "consulta.h"
#include "ui_consulta.h"
#include <dbmanejo.h>
#include <QDebug>
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
