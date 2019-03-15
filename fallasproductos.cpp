#include "fallasproductos.h"
#include "ui_fallasproductos.h"
#include <QMessageBox>
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include "mainwindow.h"
#include <dbmanejo.h>


dbManejo dbFallasProductos;

FallasProductos::FallasProductos(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FallasProductos)
{
    ui->setupUi(this);

    FEquipo.clear();
    FEqCod.clear();
    FEqDes.clear();
    FGrupo.clear();

    //------------------------------------------------------
    //      Lista de Productos
    //------------------------------------------------------
    ModListaProd = new QSqlRelationalTableModel(this,dbManejo::dbRetorna());
    ModListaProd->setTable("Productos");
    ModListaProd->select();

    FilListaProd = new QSortFilterProxyModel(this);
    FilListaProd->setSourceModel(ModListaProd);
    FilListaProd->setFilterCaseSensitivity(Qt::CaseInsensitive);
    FilListaProd->setFilterKeyColumn(-1);

    ui->TablaProd_FP->setModel(FilListaProd);
    ui->TablaProd_FP->hideColumn(0);
    ui->TablaProd_FP->hideColumn(3);
 //   ui->ProdTablaFalla->hideColumn(4);
    ui->TablaProd_FP->sortByColumn(1,Qt::AscendingOrder);
    ui->TablaProd_FP->setSortingEnabled(true);
    ui->TablaProd_FP->setColumnWidth(1,100);
    ui->TablaProd_FP->setColumnWidth(2,220);
    ui->TablaProd_FP->setColumnWidth(4,30);

    //------------------------------------------------------
    //      Grupo de Fallas
    //------------------------------------------------------
    ModFallaGrupo = new QSqlRelationalTableModel(this,dbManejo::dbRetorna());
    ModFallaGrupo->setTable("FallasGrupo");
    ModFallaGrupo->select();

    FilFallaGrupo = new QSortFilterProxyModel(this);
    FilFallaGrupo->setSourceModel(ModFallaGrupo);
    FilFallaGrupo->setFilterCaseSensitivity(Qt::CaseInsensitive);
    FilFallaGrupo->setFilterKeyColumn(1);

    ui->TablaGrupo_FP->setModel(FilFallaGrupo);
    ui->TablaGrupo_FP->sortByColumn(1,Qt::AscendingOrder);
    ui->TablaGrupo_FP->hideColumn(0);
 //   ui->TablaGrupo_FP->hideColumn(3);
 //   ui->TablaGrupo_FP->hideColumn(4);
    ui->TablaGrupo_FP->setColumnWidth(1,40);
    ui->TablaGrupo_FP->setColumnWidth(2,100);
    ui->TablaGrupo_FP->setColumnWidth(3,60);
    ui->TablaGrupo_FP->setColumnWidth(4,220);

    //------------------------------------------------------
    //      Lista  de Fallas
    //------------------------------------------------------
    ModFallaLista = new QSqlRelationalTableModel(this,dbManejo::dbRetorna());
    ModFallaLista->setTable("FallasLista");
    ModFallaLista->select();


    FilFallaLista = new QSortFilterProxyModel(this);
    FilFallaLista->setSourceModel(ModFallaLista);
    FilFallaLista->setFilterCaseSensitivity(Qt::CaseInsensitive);
    FilFallaLista->setFilterKeyColumn(-1);

    ui->TablaFalla_FP->setModel(FilFallaLista);
    ui->TablaFalla_FP->sortByColumn(1,Qt::AscendingOrder);
    ui->TablaFalla_FP->hideColumn(0);
    ui->TablaFalla_FP->setColumnWidth(1,60);
    ui->TablaFalla_FP->setColumnWidth(2,220);
    ui->TablaFalla_FP->setColumnWidth(3,40);

    //------------------------------------------------------
    //      Lista  de Fallas de productos
    //------------------------------------------------------
    ModFallaProd = new QSqlRelationalTableModel(this,dbManejo::dbRetorna());
    ModFallaProd->setTable("FallasProducto");
    ModFallaProd->setEditStrategy(QSqlTableModel::OnManualSubmit);
    ModFallaProd->select();


    FilFallaProd = new QSortFilterProxyModel(this);
    FilFallaProd->setSourceModel(ModFallaProd);
    FilFallaProd->setFilterCaseSensitivity(Qt::CaseInsensitive);
    FilFallaProd->setFilterKeyColumn(-1);

    ui->TablaFallaProd_FP->setModel(FilFallaProd);
    ui->TablaFallaProd_FP->sortByColumn(1,Qt::AscendingOrder);
    ui->TablaFallaProd_FP->hideColumn(0);
    ui->TablaFallaProd_FP->setColumnWidth(1,100);
    ui->TablaFallaProd_FP->setColumnWidth(2,40);
    ui->TablaFallaProd_FP->setColumnWidth(3,220);
    ui->TablaFallaProd_FP->setColumnWidth(4,40);

    ui->ProdFallaAgregar->setEnabled(false);
    ui->ProdFallaQuitar->setEnabled(false);


    FilFallaProd->setFilterFixedString("#$%");
    FilFallaGrupo->setFilterFixedString("#$%");
    FilFallaLista->setFilterFixedString("#$%");
//    FilListaProd->setFilterFixedString("#$%");
}

FallasProductos::~FallasProductos()
{
    delete ui;
}

void FallasProductos::on_FallaProdBuscar_textChanged(const QString &arg1)
{
    FilListaProd->setFilterFixedString(arg1);
}


void FallasProductos::on_TablaProd_FP_clicked(const QModelIndex &index)
{
    int fila, tipo;
    bool ok;
    QString fProductos;

    FEquipo.clear();
    FEqCod.clear();
    FEqDes.clear();
    FGrupo.clear();

    fila = index.row();
    tipo = ui->TablaProd_FP->model()->data(ui->TablaProd_FP->model()->index(fila,4)).toInt(&ok);
    fProductos.clear();
    fProductos.append(ui->TablaProd_FP->model()->data(ui->TablaProd_FP->model()->index(fila,1)).toString());
    FEquipo.clear();
    FEquipo.append(fProductos);

    if(tipo == 1)
    {
        FilFallaGrupo->setFilterFixedString("M");
    }
    else if (tipo == 2)
    {
        FilFallaGrupo->setFilterFixedString("P");
    }
    else if (tipo == 3)
    {
        FilFallaGrupo->setFilterFixedString("I");
    }
    else
    {
        FilFallaGrupo->setFilterFixedString(" ");
    }
    FilFallaProd->setFilterFixedString(fProductos);
}

void FallasProductos::on_TablaGrupo_FP_clicked(const QModelIndex &index)
{
    int fila;

    fila = index.row();
    FGrupo.clear();
    FEqCod.clear();
    FEqDes.clear();
    FGrupo.append(ui->TablaGrupo_FP->model()->data(ui->TablaGrupo_FP->model()->index(fila,1)).toString());
    FilFallaLista->setFilterFixedString(FGrupo);
}

void FallasProductos::on_TablaFalla_FP_clicked(const QModelIndex &index)
{
    int fila;
    fila = index.row();

    if(FGrupo.isEmpty() || FEquipo.isEmpty())
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setWindowTitle("Seleccionar datos");
        msgBox.setText("Falata seleccionar Producto y Grupos\n");
        msgBox.exec();
        ui->TablaFallaProd_FP->selectRow(0);
        return;
    }

    FEqCod.clear();
    FEqCod.append(ui->TablaFalla_FP->model()->data(ui->TablaFalla_FP->model()->index(fila,1)).toString());
    FEqDes.clear();
    FEqDes.append(ui->TablaFalla_FP->model()->data(ui->TablaFalla_FP->model()->index(fila,2)).toString());
    FGrupo.clear();
    FGrupo.append(ui->TablaFalla_FP->model()->data(ui->TablaFalla_FP->model()->index(fila,3)).toString());


    qDebug () << FEquipo << FEqCod << FEqDes << FGrupo;

    ui->ProdFallaAgregar->setEnabled(true);
    ui->ProdFallaQuitar->setEnabled(false);

}

void FallasProductos::on_TablaFallaProd_FP_clicked(const QModelIndex &index)
{
    int fila;
    bool ok;
    fila = index.row();
    ID_Fallas_Productos = ui->TablaFallaProd_FP->model()->data(ui->TablaFallaProd_FP->model()->index(fila,0)).toInt(&ok);
    ui->ProdFallaQuitar->setEnabled(true);
    ui->ProdFallaAgregar->setEnabled(false);
}

void FallasProductos::on_ProdFallaAgregar_clicked()
{
    QString Conf;

    if (FEqCod.isEmpty() || FEqDes.isEmpty())
    {
        QMessageBox::critical(this,tr("Seleccionar"),
                              tr("Seleccionar Falla"));
        ui->ProdFallaAgregar->setEnabled(false);
        FEqCod.clear();
        FEqDes.clear();
        return;
    }

    Conf.clear();
    Conf.append("INSERT INTO FallasProducto("
                "producto,"
                "falla,"
                "descripcion,"
                "grupo)"
                "VALUES("
                "'"+FEquipo+    "',"
                "'"+FEqCod+     "',"
                "'"+FEqDes+     "',"
                "'"+FGrupo+     "'"
                ");");

    QSqlQuery insertar;
    insertar.prepare(Conf);
    if(!insertar.exec())
    {
        QMessageBox::critical(this,tr("Tabla FallasProducto"),
                              tr("Falla al crear la tabla\n"
                             "%1").arg(insertar.lastError().text()));
    }

    FEqCod.clear();
    FEqDes.clear();
    ui->ProdFallaAgregar->setEnabled(false);
    ModFallaProd->submitAll();
}

void FallasProductos::on_ProdFallaQuitar_clicked()
{
    qDebug ( ) << ID_Fallas_Productos;
    dbFallasProductos.BorrarItem("FallasProducto",ID_Fallas_Productos);
    ui->ProdFallaQuitar->setEnabled(false);
    ID_Fallas_Productos = 0;
    ModFallaProd->submitAll();
}
