#include "dbproductoseditar.h"
#include "ui_dbproductoseditar.h"
#include <QMessageBox>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include "mainwindow.h"
#include <dbmanejo.h>

dbManejo dbProductos;

dbProductosEditar::dbProductosEditar(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dbProductosEditar)
{
    ui->setupUi(this);

    ModProdEdit = new QSqlRelationalTableModel(this,dbManejo::dbRetorna());
    ModProdEdit->setTable("Productos");
    ModProdEdit->setEditStrategy(QSqlTableModel::OnManualSubmit);
    ModProdEdit->select();

    FilProdEdit = new QSortFilterProxyModel(this);
    FilProdEdit->setSourceModel(ModProdEdit);
    FilProdEdit->setFilterCaseSensitivity(Qt::CaseInsensitive);
    FilProdEdit->setFilterKeyColumn(1); //-1 ordena por todas la columnas


    //Auto completado del line Edit para buscar a los agentes
    CompProdEdit = new QCompleter(ModProdEdit, this);
    CompProdEdit->setCaseSensitivity(Qt::CaseInsensitive);
    CompProdEdit->setCompletionColumn(1);
    ui->ProductoEdit->setCompleter(CompProdEdit);

    ui->DatosProdTabla->setModel(FilProdEdit);
    ui->DatosProdTabla->sortByColumn(1,Qt::AscendingOrder);
    ui->DatosProdTabla->setSortingEnabled(true);
    ui->DatosProdTabla->setColumnWidth(0,40);
    ui->DatosProdTabla->setColumnWidth(1,100);
    ui->DatosProdTabla->setColumnWidth(2,250);
    ui->DatosProdTabla->setColumnWidth(3,80);
    ui->DatosProdTabla->setColumnWidth(4,40);

    ui->Editar->setEnabled(false);
    ui->Borrar->setEnabled(false);
}

dbProductosEditar::~dbProductosEditar()
{
    delete ui;
}

void dbProductosEditar::on_Guardar_clicked()
{
    if (!ui->TipoEdit->currentIndex())
    {
        QMessageBox::information(this,tr("Tipo de producto"),
                                 tr("Seleccionar tipo de Producto"));
        return;
    }
    QString Conf;
    Conf.append("INSERT INTO Productos("
                "producto,"
                "desc,"
                "version,"
                "tipo)"
                "VALUES("
                "'"+ui->ProductoEdit->text()+"',"
                "'"+ui->DescripcionEdit->text()+"',"
                "'"+ui->VersionEdit->text()+"',"
                ""+QString::number(ui->TipoEdit->currentIndex(),10)+""
                ");");
    QSqlQuery insertar;
    insertar.seek(2,true);
    insertar.prepare(Conf);
    if(!insertar.exec())
    {
        QMessageBox::critical(this,tr("Tabla Productos"),
                              tr("Falla al crear la tabla\n"
                             "%1").arg(insertar.lastError().text()));
    }

    ModProdEdit->submitAll();
}

void dbProductosEditar::on_Editar_clicked()
{
    if (!ui->TipoEdit->currentIndex())
    {
        QMessageBox::information(this,tr("Tipo de producto"),
                                 tr("Seleccionar tipo de Producto"));
        return;
    }
    QString Conf;
    Conf.append("UPDATE Productos SET "
                "producto ="
                "'"+ui->ProductoEdit->text()+"'"
                ",desc ="
                "'"+ui->DescripcionEdit->text()+"'"
                ",version ="
                "'"+ui->VersionEdit->text()+"'"
                ",tipo="
                "'"+QString::number(ui->TipoEdit->currentIndex(),10)+"'"
                " WHERE id ="
                ""+QString::number(Indice,10)+""
                "");

   QSqlQuery editar;
   editar.prepare(Conf);
   if(!editar.exec())
   {
       QMessageBox::critical(this,tr("Tabla Productos"),
                             tr("Falla al crear la tabla\n"
                            "%1").arg(editar.lastError().text()));
   }


    Indice = 0;
    ui->Editar->setEnabled(false);
    ui->Borrar->setEnabled(false);
    ModProdEdit->submitAll();
}

void dbProductosEditar::on_Borrar_clicked()
{

    int fila;
    fila = ui->DatosProdTabla->currentIndex().row();
    ModProdEdit->removeRow(fila);
    ModProdEdit->submitAll();
   Indice = 0;
    ui->Borrar->setEnabled(false);
    ui->Editar->setEnabled(false);
}

void dbProductosEditar::on_DatosProdTabla_clicked(const QModelIndex &index)
{
    bool ok;
    int fila = index.row();
    ui->ProductoEdit->setText(ui->DatosProdTabla->model()->data(ui->DatosProdTabla->model()->index(fila,1)).toString());
    ui->DescripcionEdit->setText(ui->DatosProdTabla->model()->data(ui->DatosProdTabla->model()->index(fila,2)).toString());
    ui->VersionEdit->setText(ui->DatosProdTabla->model()->data(ui->DatosProdTabla->model()->index(fila,3)).toString());
    ui->TipoEdit->setCurrentIndex(ui->DatosProdTabla->model()->data(ui->DatosProdTabla->model()->index(fila,4)).toInt(&ok));
    Indice = ui->DatosProdTabla->model()->data(ui->DatosProdTabla->model()->index(fila,0)).toInt(&ok);
    ui->Borrar->setEnabled(true);
    ui->Editar->setEnabled(true);
}

void dbProductosEditar::on_ProductoEdit_textChanged(const QString &arg1)
{
    FilProdEdit->setFilterFixedString(arg1);
}
