#include "archivos.h"
#include "ui_archivos.h"
#include <QMessageBox>
#include <QDebug>
#include <dbrutasarchivo.h>
#include <dbmanejo.h>
#include <variables.h>
#include <QFileDialog>

dbRutasArchivo dbArchivo;
dbManejo dbBase;


Archivos::Archivos(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Archivos)
{
    ui->setupUi(this);

    dbArchivo.Abrir();

    QString Texto;
    Texto = dbArchivo.Buscar("dbReparaciones");
//    qDebug () << "Texto 1 " << Texto;
    ui->BaseDatos->setText(Texto);

    Texto.clear();
    Texto = dbArchivo.Buscar("InformePDF");
//    qDebug () << "Texto 2 " << Texto;
    ui->InformeAgente->setText(Texto);

    Texto.clear();
    Texto = dbArchivo.Buscar("InformeRep");
//    qDebug () << "Texto 3 " << Texto;
    ui->InformeInterno->setText(Texto);

}

Archivos::~Archivos()
{
    delete ui;
}

void Archivos::on_pushButton_clicked()
{

    RutaBaseDatos.clear();
    RutaInfoPDF.clear();
    RutaInfoExcel.clear();

    RutaBaseDatos.append(ui->BaseDatos->text());
    RutaInfoPDF.append(ui->InformeAgente->text());
    RutaInfoExcel.append(ui->InformeInterno->text());

    dbArchivo.Cerrar();
    dbBase.SetDirDb(RutaBaseDatos);
    dbBase.dbAbrirCrear();
    this->close();

}

void Archivos::on_bBaseDatos_clicked()
{
    QString NArchivo;
    QString fileName = QFileDialog::getExistingDirectory(this, tr("Base Datos Reparaciones"),
                                                         ui->BaseDatos->text(),
                                                         QFileDialog::ShowDirsOnly
                                                         | QFileDialog::DontResolveSymlinks);

    if(fileName.isEmpty())
        return;

    if(!fileName.endsWith("/"))
    {
        fileName.append("/");
    }
//    qDebug () << fileName;
    ui->BaseDatos->setText(fileName);

    dbArchivo.Actualizar("dbReparaciones",ui->BaseDatos->text());
}

void Archivos::on_bInformeAgente_clicked()
{
    QString NArchivo;
    QString fileName = QFileDialog::getExistingDirectory(this, tr("Informes a Agentes"),
                                                         ui->InformeAgente->text(),
                                                         QFileDialog::ShowDirsOnly
                                                         | QFileDialog::DontResolveSymlinks);

    if(fileName.isEmpty())
        return;

 //   qDebug () << fileName;
    if(!fileName.endsWith("/"))
    {
        fileName.append("/");
    }
//    qDebug () << fileName;
    ui->InformeAgente->setText(fileName);

    dbArchivo.Actualizar("InformePDF",ui->InformeAgente->text());
}

void Archivos::on_bInformeInterno_clicked()
{
    QString NArchivo;
    QString fileName = QFileDialog::getExistingDirectory(this, tr("Informes Internos"),
                                                         ui->InformeInterno->text(),
                                                         QFileDialog::ShowDirsOnly
                                                         | QFileDialog::DontResolveSymlinks);
    if(fileName.isEmpty())
        return;

//   qDebug () << fileName;
    if(!fileName.endsWith("/"))
    {
        fileName.append("/");
    }
//   qDebug () << fileName;
    ui->InformeInterno->setText(fileName);
    dbArchivo.Actualizar("InformeRep",ui->InformeInterno->text());
}
