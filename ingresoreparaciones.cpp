#include "ingresoreparaciones.h"
#include "ui_ingresoreparaciones.h"
#include <QMessageBox>
#include <QDebug>


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
                    "obs,"
                    "rtransp,"
                    "ftransp)"
                    "VALUES("
                    "'"+ui->Agente->text()+"',"
                    "'"+ui->FIngreso->text()+"',"
                    "'',"
                    "'',"
                    "'',"
                    "'"+ui->IngObs->toPlainText()+"',"
                    "'"+ui->rTransp->text()+"',"
                    "'"+ui->fTrasnp->text()+"'"
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
                    " WHERE id ="
                    ""+ui->ID_Rep->text()+""
                    "");
    }
    qDebug() <<Conf;
    QSqlQuery insertar;
    insertar.prepare(Conf);
    if(!insertar.exec())
    {
        QMessageBox::critical(this,tr("Tabla Reparaciones"),
                              tr("Falla al cargar la tabla\n"
                                 "%1").arg(insertar.lastError().text()));
    }
qDebug() <<insertar.lastError().text();

}

void IngresoReparaciones::SetDatos(QString tipo, QString Agente,QString Fing, QString rTransp, QString fTransp,QString Observaciones)
{
    ui->ID_Rep->setText(tipo);
    ui->Agente->setText(Agente);
    ui->FIngreso->setText(Fing);
    ui->rTransp->setText(rTransp);
    ui->fTrasnp->setText(fTransp);
    ui->IngObs->setText(Observaciones);
}
