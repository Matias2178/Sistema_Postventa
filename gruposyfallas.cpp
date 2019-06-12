#include "gruposyfallas.h"
#include "ui_gruposyfallas.h"
#include <QMessageBox>
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <dbmanejo.h>

gruposyfallas::gruposyfallas(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::gruposyfallas)
{
    ui->setupUi(this);

    ModGrupo = new QSqlRelationalTableModel(this,dbManejo::dbRetorna() );
    ModGrupo->setTable("FallasGrupo");
    ModGrupo->setEditStrategy(QSqlTableModel::OnManualSubmit);
    ModGrupo->select();

    FilGrupo = new QSortFilterProxyModel (this);
    FilGrupo->setSourceModel(ModGrupo);
    FilGrupo->setFilterCaseSensitivity((Qt::CaseInsensitive));

    ui->TablaFallaGrupo->setModel(FilGrupo);
    ui->TablaFallaGrupo->setSortingEnabled(true);

    ModFalla = new QSqlRelationalTableModel(this,dbManejo::dbRetorna() );
    ModFalla->setTable("FallasLista");
    ModFalla->setEditStrategy(QSqlTableModel::OnManualSubmit);
    ModFalla->select();
//     qDebug () << "Paso 4 ModFalla ";
    FilFalla = new QSortFilterProxyModel (this);
    FilFalla->setSourceModel(ModFalla);
    FilFalla->setFilterCaseSensitivity((Qt::CaseInsensitive));
    FilFalla->setFilterKeyColumn(3);

    ui->TablaFallaLista->setModel(FilFalla);
    ui->TablaFallaLista->setSortingEnabled(true);

  //  ui->GrupoGuardar->setEnabled(false);
    ui->GrupoEditar->setEnabled(false);
    ui->GrupoBorrar->setEnabled(false);
    ui->FallaEditar->setEnabled(false);
    ui->FallaBorrar->setEnabled(false);

}

gruposyfallas::~gruposyfallas()
{
    delete ui;
}
void gruposyfallas::on_NuevoGrupo_clicked()
{
}

bool gruposyfallas::BuscaNuevo()
{
    QSqlQuery consultar;
    QString Conf;
    bool ok;
    int indice;

    consultar.prepare("SELECT * FROM FallasGrupo WHERE codigo = 'nuevo'");
    if(!consultar.exec())
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setWindowTitle("Tabla Fallas");
        msgBox.setText("Falla al crear la tabla\n"+consultar.lastError().text());
        msgBox.exec();
    }

    int rev;
    rev = 0;
    if(consultar.next())
    {
        rev ++;
        QString cod;
        cod = consultar.value(1).toByteArray().constData();
        cod.clear();
        indice =ui->FallasProductos->currentIndex();
        if(indice == 1)
        {
            cod.append("M-");
        }
        else if (indice == 2)
        {
            cod.append("P-");
        }
        else if (indice == 3)
        {
            cod.append ("I-");
        }
        else if (indice == 4)
        {
            cod.append ("S-");
        }
        indice = consultar.value(0).toByteArray().toInt(&ok);
        cod.append(QString::number(indice));

        ui->GrupoCodigo->setText(cod);
        IDGrupo = indice;
        ui->GrupoGuardar->setEnabled(true);
        return true;
    }
    return false;
}

void gruposyfallas::on_GrupoGuardar_clicked()
{
    QSqlQuery insertar;
    if (!ui->FallasProductos->currentIndex())
    {
        QMessageBox::information(this,tr("Tipo de producto"),
                                tr("Seleccionar tipo de Producto"));
        return;
    }
    if(!BuscaNuevo())
    {
        QString Conf;
        Conf.clear();
        Conf.append("INSERT INTO FallasGrupo("
                    "codigo, grupo, reporte,descrip)"
                    "VALES('nuevo' ,' ' ,' ' ,' ');");

        insertar.prepare(Conf);
        if(!insertar.exec())
        {
            QMessageBox::critical(this,tr("Tabla FallasGrupo"),
                                  tr("Falla al ingresar Grupo3554\n"
                                     "%1").arg(insertar.lastError().text()));
        }
    }
    insertar.prepare("UPDATE FallasGrupo SET "
                     "codigo ='"+ui->GrupoCodigo->text()+"',"
                     "grupo ='"+ui->GrupoNombre->text()+"',"
                     "reporte ='"+ui->GrupoCodigoRep->text()+"',"
                     "descrip ='"+ui->GrupoDescripcion->text()+"'"
                     " WHERE id ="+QString::number(IDGrupo,10));
    if(!insertar.exec())
    {
        QMessageBox::critical(this,tr("Tabla FallasGrupo"),
                              tr("Falla al ingresar Grupo3554\n"
                             "%1").arg(insertar.lastError().text()));
    }
}

void gruposyfallas::on_GrupoEditar_clicked()
{

    QString Edit;

    Edit.clear();
    Edit.append("UPDATE FallasGrupo SET "
                            "grupo ="
                            "'"+ui->GrupoNombre->text()+"',"
                            "reporte ="
                            "'"+ui->GrupoCodigoRep->text()+"',"
                            "descrip ="
                            "'"+ui->GrupoDescripcion->text()+"'"
                            " WHERE id ="
                            ""+QString::number(IDGrupo,10)+""
                            "");
    QSqlQuery editar;
    editar.prepare(Edit);
    if(!editar.exec())
    {
        QMessageBox::critical(this,tr("Tabla Grupo Falla"),
                              tr("Falla en la carga de datos"));
    }

    ui->GrupoEditar->setEnabled(false);
    ModGrupo->submitAll();
    ui->TablaFallaGrupo->scrollToBottom();
}

void gruposyfallas::on_GrupoBorrar_clicked()
{

}

void gruposyfallas::on_FallasProductos_activated(int index)
{
    FilGrupo->setFilterKeyColumn(1);
    if (index == 1)
    {
        FilGrupo->setFilterFixedString("M");
        FilFalla->setFilterFixedString("M");
    }
    else if (index == 2)
    {
        FilGrupo->setFilterFixedString("P");
        FilFalla->setFilterFixedString("P");
    }
    else if (index == 3)
    {
        FilGrupo->setFilterFixedString("I");
        FilFalla->setFilterFixedString("I");
    }
    else
    {
        FilGrupo->setFilterFixedString("");
        FilFalla->setFilterFixedString("");
    }
    ui->TablaFallaGrupo->sortByColumn(0,Qt::AscendingOrder);
    ui->TablaFallaGrupo->scrollToBottom();

}

void gruposyfallas::on_TablaFallaGrupo_clicked(const QModelIndex &index)
{
    int fila = index.row();
    QString GCodigo, GNombre, GReporte, GDescripcion;
    bool ok;

    IDGrupo = ui->TablaFallaGrupo->model()->data(ui->TablaFallaGrupo->model()->index(fila,0)).toString().toInt(&ok);
    GCodigo = ui->TablaFallaGrupo->model()->data(ui->TablaFallaGrupo->model()->index(fila,1)).toString();
    GNombre= ui->TablaFallaGrupo->model()->data(ui->TablaFallaGrupo->model()->index(fila,2)).toString();
    GReporte = ui->TablaFallaGrupo->model()->data(ui->TablaFallaGrupo->model()->index(fila,3)).toString();
    GDescripcion= ui->TablaFallaGrupo->model()->data(ui->TablaFallaGrupo->model()->index(fila,4)).toString();

    ui->GrupoCodigo->setText(GCodigo);
    ui->GrupoNombre->setText(GNombre);
    ui->GrupoCodigoRep->setText(GReporte);
    ui->GrupoDescripcion->setText(GDescripcion);

    ui->FallaGrupoCod->setText(GCodigo);
    ui->FallaGrupoNom->setText(GNombre);
    FilFalla->setFilterFixedString(GCodigo);

    ui->GrupoEditar->setEnabled(true);
}

void gruposyfallas::on_FallaGuardar_clicked()
{
    if (ui->FallaGrupoCod->text().isEmpty())
    {
        QMessageBox::information(this,tr("Tipo de producto"),
                                 tr("Seleccionar tipo de Producto"));
        return;
    }
//    if(ui->GrupoC->text().isEmpty())
//    {
//        QMessageBox::information(this,tr("Cargar Nombre"),
//                                 tr("Cargar Nombre del Grupo"));
//        return;
//    }
    QString Conf;
    Conf.clear();
    Conf.append("INSERT INTO FallasLista("
                "nombre,"
                "descripcion,"
                "grupo)"
                "VALUES("
                "'"+ui->FallaCodigo->text()+"',"
                "'"+ui->FallaDescripcion->text()+"',"
                "'"+ui->FallaGrupoCod->text()+"'"
                ");");
    QSqlQuery guardar;
//    insertar.seek(2,true);
    guardar.prepare(Conf);
    if(!guardar.exec())
    {
        QMessageBox::critical(this,tr("Tabla FallasLista"),
                              tr("Falla al ingresar datos del error \n"
                             "%1").arg(guardar.lastError().text()));
    }

    ModFalla->submitAll();

}

void gruposyfallas::on_TablaFallaLista_clicked(const QModelIndex &index)
{
    int fila = index.row();
    QString FCodigo, FDescripcion, GCodigo, GNombre;
    bool ok;

    IDFalla = ui->TablaFallaLista->model()->data(ui->TablaFallaLista->model()->index(fila,0)).toString().toInt(&ok);
    FCodigo= ui->TablaFallaLista->model()->data(ui->TablaFallaLista->model()->index(fila,1)).toString();
    FDescripcion= ui->TablaFallaLista->model()->data(ui->TablaFallaLista->model()->index(fila,2)).toString();
    GCodigo = ui->TablaFallaLista->model()->data(ui->TablaFallaLista->model()->index(fila,3)).toString();


//    qDebug () << IDFalla;

    ui->FallaCodigo->setText(FCodigo);
    ui->FallaDescripcion->setText(FDescripcion);

    ui->FallaGrupoCod->setText(GCodigo);
  //   FilFalla->setFilterFixedString(GCodigo);

    ui->FallaEditar->setEnabled(true);
}

void gruposyfallas::on_FallaEditar_clicked()
{
//    int fila;
//    fila = ui->TablaFallaLista->currentIndex().row();
    QString Edit;

    Edit.clear();
    Edit.append("UPDATE FallasLista SET "
                            "nombre ="
                            "'"+ui->FallaCodigo->text()+"',"
                            "descripcion ="
                            "'"+ui->FallaDescripcion->text()+"',"
                            "grupo ="
                            "'"+ui->FallaGrupoCod->text()+"'"
                            " WHERE id ="
                            ""+QString::number(IDFalla,10)+""
                            "");
    QSqlQuery editar;
    editar.prepare(Edit);
    if(!editar.exec())
    {
        QMessageBox::critical(this,tr("Tabla Falla Lista"),
                              tr("Falla en la modificacion de datos"));
    }

    ui->FallaEditar->setEnabled(false);
    ModFalla->submitAll();
    ui->TablaFallaLista->scrollToBottom();
}
