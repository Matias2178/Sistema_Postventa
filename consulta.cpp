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
    bool ok;
    Conf.append("SELECT * FROM Reparaciones");

    QSqlQuery consultar;
    consultar.prepare(Conf);
    consultar.exec();
    while(consultar.next())
    {
        if(Id == consultar.value("id").toInt(&ok))
        {
            ui->lAgente->setText(consultar.value("agente").toByteArray());
            break;
        }
    }
}
