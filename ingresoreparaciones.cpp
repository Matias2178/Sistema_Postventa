#include "ingresoreparaciones.h"
#include "ui_ingresoreparaciones.h"
#include <QMessageBox>
#include <QDebug>
#include <ingreso.h>



IngresoReparaciones::IngresoReparaciones(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IngresoReparaciones)
{
    ui->setupUi(this);
    ui->fTrasnp->setInputMask("00/00/0000");
    ui->FIngreso->setInputMask("00/00/0000");
}



IngresoReparaciones::~IngresoReparaciones()
{
    delete ui;
}

void IngresoReparaciones::on_buttonBox_accepted()
{
//    Reparaciones.GuardarIngreso(ui->Agente->text(),ui->FIngreso->text(),ui->rTransp->text(),ui->fTrasnp->text(),ui->IngObs->toPlainText());

    QString Conf;
    Conf.clear();
    if(ui->ID_Rep->text().isEmpty())
    {
        Conf.append("INSERT INTO Reparaciones("
                    "agente,"
                    "fing,"
                    "frep,"
                    "operario,"
                    "pres,"
                    "finf,"
                    "obs,"
                    "rtransp,"
                    "ftransp,"
                    "ragente)"
                    "VALUES("
                    "'"+ui->Agente->text()+"',"
                    "'"+ui->FIngreso->text()+"',"
                    "'',"
                    "'',"
                    "'',"
                    "'',"
                    "'"+ui->IngObs->toPlainText()+"',"
                    "'"+ui->rTransp->text()+"',"
                    "'"+ui->fTrasnp->text()+"',"
                    "'"+ui->rAgente->text()+"'"
                    ");");
    }
    else
    {
        Conf.append("UPDATE Reparaciones SET "
                    "agente ="
                    "'"+ui->Agente->text()+"'"
                    ",fing ="
                    "'"+ui->FIngreso->text()+"'"
                    ",obs ="
                    "'"+ui->IngObs->toPlainText()+"'"
                    ",rtransp="
                    "'"+ui->rTransp->text()+"'"
                    ",ftransp="
                    "'"+ui->fTrasnp->text()+"'"
                    ",ragente="
                    "'"+ui->rAgente->text()+"'"
                    " WHERE id ="
                    ""+ui->ID_Rep->text()+""
                    "");
    }
//    qDebug() <<Conf;
    QSqlQuery insertar;
    insertar.prepare(Conf);
    if(!insertar.exec())
    {
        QMessageBox::critical(this,tr("Tabla Reparaciones"),
                              tr("Falla al cargar la tabla\n"
                                 "%1").arg(insertar.lastError().text()));
    }

    emit finalizar();
}

void IngresoReparaciones::SetDatos(QString Agente,QString Fing)
{
    ui->ID_Rep->clear();
    ui->Agente->setText(Agente);
    ui->FIngreso->setText(Fing);
    ui->rTransp->clear();
    ui->fTrasnp->clear();
    ui->IngObs->clear();
}

void IngresoReparaciones::SetDatos(int ID)
{
    QSqlQuery consultar;
    consultar.prepare("SELECT * FROM Reparaciones WHERE id ==" + QString::number(ID,10));
    consultar.exec();
    consultar.next();
    ui->ID_Rep->setText(consultar.value("id").toString());
    ui->Agente->setText(consultar.value("agente").toString());
    ui->FIngreso->setText(consultar.value("fing").toString());
    ui->rTransp->setText(consultar.value("rtransp").toString());
    ui->fTrasnp->setText(consultar.value("rtransp").toString());
    ui->IngObs->setText(consultar.value("obs").toString());
    ui->rAgente->setText(consultar.value("ragente").toString());
}
