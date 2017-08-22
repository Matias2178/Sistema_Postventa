#include "dbfallaseditar.h"
#include "ui_dbfallaseditar.h"
#include <QMessageBox>
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include "mainwindow.h"
#include <dbmanejo.h>

dbManejo Fallasdb;

dbFallasEditar::dbFallasEditar(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dbFallasEditar)
{
    ui->setupUi(this);

    ModProdF = new QSqlRelationalTableModel(this,dbManejo::dbRetorna());
    ModProdF->setTable("Productos");
    ModProdF->select();

    FilProdF = new QSortFilterProxyModel(this);
    FilProdF->setSourceModel(ModProdF);
    FilProdF->setFilterCaseSensitivity(Qt::CaseInsensitive);
    FilProdF->setFilterKeyColumn(-1); //-1 ordena por todas la columnas

    ui->ProdTablaF->setModel(FilProdF);
    ui->ProdTablaF->hideColumn(0);
    ui->ProdTablaF->hideColumn(3);
    ui->ProdTablaF->hideColumn(4);
    ui->ProdTablaF->sortByColumn(1,Qt::AscendingOrder);
    ui->ProdTablaF->setSortingEnabled(true);
    ui->ProdTablaF->setColumnWidth(1,100);
    ui->ProdTablaF->setColumnWidth(2,180);

    ModFalla = new QSqlRelationalTableModel(this,dbManejo::dbRetorna());
    ModFalla->setTable("Fallas");
    ModFalla->select();

    FilFalla = new QSortFilterProxyModel(this);
    FilFalla->setSourceModel(ModFalla);
    FilFalla->setFilterCaseSensitivity(Qt::CaseInsensitive);
    FilFalla->setFilterKeyColumn(-1);

    ui->FallaTabla->setModel(FilFalla);
    ui->FallaTabla->hideColumn(0);
    ui->FallaTabla->sortByColumn(1,Qt::AscendingOrder);
    ui->FallaTabla->setSortingEnabled(true);
    ui->FallaTabla->setColumnWidth(1,100);
    ui->FallaTabla->setColumnWidth(2,50);
    ui->FallaTabla->setColumnWidth(3,280);
    ui->FallaTabla->setColumnWidth(4,50);
    ui->FallaTabla->setColumnWidth(5,50);

    ui->Editar->setEnabled(false);
    ui->Borrar->setEnabled(false);
    Indice = 0;
}

dbFallasEditar::~dbFallasEditar()
{
    delete ui;
}

void dbFallasEditar::on_Guardar_clicked()
{
    if (ProdFallaTx.isEmpty())
    {
        QMessageBox::information(this,tr("Producto"),
                                 tr("Hay que seleccionar a que producto hay que cargar la falla"));
        return;
    }
    QString Conf;

    Conf.clear();
    Conf.append("INSERT INTO Fallas("
                "producto,"
                "falla,"
                "descripcion,"
                "fpre,"
                "bonif)"
                "VALUES("
                "'"+ProdFallaTx+"',"
                "'"+ui->FallaEdit->text()+"',"
                "'"+ui->FallaDescripcion->text()+"',"
                "'"+ui->FallaPresupuesto->text()+"',"
                "'"+ui->FallaBonif->text()+"'"
                ");");

    QSqlQuery insertar;
    if(!insertar.prepare(Conf))
    {
        QMessageBox::critical(this,tr("Tabla Fallas"),
                              tr("Falla al crear la tabla\n"
                             "%1").arg(insertar.lastError().text()));
    }
    insertar.exec();
    FilFalla->setFilterFixedString(ProdFallaTx);
}

void dbFallasEditar::on_Editar_clicked()
{
    if (ProdFallaTx.isEmpty())
    {
        QMessageBox::information(this,tr("Tipo de producto"),
                                 tr("Seleccionar tipo de Producto"));
        return;
    }
    QString Conf;
    Conf.append("UPDATE Fallas SET "
                "producto ="
                "'"+ProdFallaTx+"'"
                ",falla ="
                "'"+ui->FallaEdit->text()+"'"
                ",descripcion ="
                "'"+ui->FallaDescripcion->text()+"'"
                ",fpre ="
                "'"+ui->FallaPresupuesto->text()+"'"
                ",bonif ="
                "'"+ui->FallaBonif->text()+"'"
                " WHERE id ="
                ""+QString::number(Indice,10)+""
                "");
    QSqlQuery editar;
    if(!editar.prepare(Conf))
    {
        QMessageBox::critical(this,tr("Tabla Fallas"),
                              tr("Falla al crear la tabla\n"
                             "%1").arg(editar.lastError().text()));
    }
    editar.exec();
    FilFalla->setFilterFixedString(ProdFallaTx);
    Indice = 0;
    ui->Editar->setEnabled(false);
    ui->Borrar->setEnabled(false);
}

void dbFallasEditar::on_Borrar_clicked()
{
    Fallasdb.BorrarItem("Fallas",Indice);
    FilFalla->setFilterFixedString(ProdFallaTx);
    Indice = 0;
    ui->Borrar->setEnabled(false);
    ui->Editar->setEnabled(false);
}



void dbFallasEditar::on_FallaBuscar_textChanged(const QString &arg1)
{
   FilProdF->setFilterFixedString(arg1);
}

void dbFallasEditar::on_ProdTablaF_clicked(const QModelIndex &index)
{
    int fila;
    fila = index.row();
    ProdFallaTx.clear();
    ProdFallaTx.append(ui->ProdTablaF->model()->data(ui->ProdTablaF->model()->index(fila,1)).toString());
    FilFalla->setFilterFixedString(ProdFallaTx);
//    qDebug () << ProdFallaTx;
}
