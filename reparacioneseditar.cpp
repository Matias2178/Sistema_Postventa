#include "reparacioneseditar.h"
#include "ui_reparacioneseditar.h"
#include <QDebug>
#include <QMessageBox>
#include <QList>




reparacioneseditar::reparacioneseditar(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::reparacioneseditar)
{
    ui->setupUi(this);
    ModEditRep = new QSqlRelationalTableModel(this,dbManejo::dbRetorna());
    ModEditRep->setTable("Productos");
    ModEditRep->select();
    //    ModTrabTablaRep->setRelation();

    FilEditRep = new QSortFilterProxyModel(this);
    FilEditRep->setSourceModel(ModEditRep);
    FilEditRep->setFilterCaseSensitivity(Qt::CaseInsensitive);
    FilEditRep->setFilterKeyColumn(-1);
    // //   FilTrabTablaRep->setFilterRegExp();

    ui->Producto->setModel(FilEditRep);
    ui->Producto->hideColumn(0);
    ui->Producto->hideColumn(3);
    ui->Producto->hideColumn(4);
    ui->Producto->sortByColumn(1, Qt::AscendingOrder);
    ui->Producto->setSortingEnabled(true);
    ui->Producto->resizeColumnsToContents();

    FilEditRep2 = new QSortFilterProxyModel(this);
    FilEditRep2->setSourceModel(ModEditRep);
    FilEditRep2->setFilterCaseSensitivity(Qt::CaseInsensitive);
    FilEditRep2->setFilterKeyColumn(-1);
    // //   FilTrabTablaRep->setFilterRegExp();

    ui->Producto_2->setModel(FilEditRep2);
    ui->Producto_2->hideColumn(0);
    ui->Producto_2->hideColumn(3);
    ui->Producto_2->hideColumn(4);
    ui->Producto_2->sortByColumn(1, Qt::AscendingOrder);
    ui->Producto_2->setSortingEnabled(true);
    ui->Producto_2->resizeColumnsToContents();
   // ui->Producto->setColumnWidth(1,100);



   // ui->Producto.set->setmo->setModel(FilEditRep);
    //    ui->TrabajoTablaRep->hideColumn(0);

}

reparacioneseditar::~reparacioneseditar()
{
    delete ui;

}

void reparacioneseditar::SetDatos(int TipoProd, QString ID)
{
    QSqlQuery consultar;
    QString conf;
    QString Codigo;
    QString Fallas;
    ui->TablaID->setText(ID);

    Fallas.clear();
    Producto = TipoProd;
    if(TipoProd == 1)
    {
        conf.append("SELECT * FROM Monitores WHERE id =="  "'"  +  ID+  "'" );
        consultar.prepare(conf);
        consultar.exec();
        consultar.next();
        Codigo.clear();
        Codigo.append(consultar.value("nombre").toString());
        ui->NProductos->setText(consultar.value("nombre").toString());
        ui->NumSerie->setText(consultar.value("sn").toString());

        Fallas.append(consultar.value("falla").toString());
        ui->FechaFab->setText(consultar.value("ffab").toString());
        ui->VerSoft->setText(consultar.value("vsoft").toString());
        ui->FechaSoft->setText(consultar.value("actsoft").toString());
//        ui->FechaInst->setText(consultar.value("finst").toString());
        ui->Observ->setText(consultar.value("obs").toString());
        ui->Bonificacion->setText(consultar.value("bonif").toString());
//        ui->Configuracion->setText(consultar.value("conf").toString());
        ui->label_18->setText("Ver Act Soft");
    }
    if(TipoProd == 2)
    {
        conf.append("SELECT * FROM Perifericos WHERE id =="  "'"  +  ID+  "'" );
        consultar.prepare(conf);
        consultar.exec();
        consultar.next();

        Fallas.append(consultar.value("falla").toString());

        Codigo.clear();
        Codigo.append(consultar.value("nombre").toString());
        ui->NProductos->setText(consultar.value("nombre").toString());
        ui->NumSerie->setText(consultar.value("sn").toString());
        ui->FechaFab->setText(consultar.value("ffab").toString());
        ui->VerSoft->setText(consultar.value("vsoft").toString());
        ui->FechaSoft->setText(consultar.value("fsoft").toString());
        ui->FechaInst->setText(consultar.value("finst").toString());
        ui->Observ->setText(consultar.value("obs").toString());
        ui->Bonificacion->setText(consultar.value("bonif").toString());
        ui->Configuracion->setText(consultar.value("conf").toString());
        ui->label_18->setText("Fecha Soft");
    }
    if(TipoProd == 3)
    {
        conf.append("SELECT * FROM Instalaciones WHERE id =="  "'"  +  ID+  "'" );
        consultar.prepare(conf);
        consultar.exec();
        consultar.next();

        Fallas.append(consultar.value("falla").toString());

        Codigo.clear();
        Codigo.append(consultar.value("nombre").toString());
        ui->NProductos->setText(consultar.value("nombre").toString());
        ui->NumSerie->setText(consultar.value("sn").toString());
//        ui->FechaFab->setText(consultar.value("ffab").toString());
//        ui->VerSoft->setText(consultar.value("vsoft").toString());
        ui->FechaSoft->setText(consultar.value("fsoft").toString());
//        ui->FechaInst->setText(consultar.value("finst").toString());
        ui->Observ->setText(consultar.value("obs").toString());
        ui->Bonificacion->setText(consultar.value("bonif").toString());
//        ui->Configuracion->setText(consultar.value("conf").toString());
    }

    EditRep.CargarProd(*ui->Productos,TipoProd);
    EditRep.CargarFallas(*ui->Fallas,Codigo);

    tFallas.SetFallas(*ui->Fallas,Fallas);


   int fila;
    FilEditRep->setFilterFixedString(Codigo);
    fila = ui->Producto->currentIndex().row();
 //   qDebug () << fila;
    FilEditRep->setFilterFixedString("");
    ui->Producto->selectRow(22);
}

void reparacioneseditar::on_Productos_clicked(const QModelIndex &index)
{
    QString Producto;
    int fil = index.row();
    Producto = ui->Productos->item(fil,0)->text();
    ui->NProductos->setText(Producto);
    EditRep.CargarFallas(*ui->Fallas,Producto);
}

void reparacioneseditar::on_buttonBox_accepted()
{
    QString Fallas, Grupo;
    QList <QString> lFallas;

    Fallas.clear();
    Grupo.clear();
    lFallas.clear();

    lFallas << tFallas.GetFallas(*ui->Fallas);

    Fallas.append(lFallas[0]);
    Grupo.append(lFallas[1]);

    qDebug() << lFallas;
    qDebug() << Fallas;
    qDebug() << Grupo;


    QString Conf;
    if(Producto == 1)
    {
        Conf.append("UPDATE Monitores SET "
                    "nombre ="
                    "'"+ui->NProductos->text()+"',"
                    "sn ="
                    "'"+ui->NumSerie->text()+"',"
          //          "ffab ="
          //          "'"+ui->FechaFab->text()+"',"
          //          "finst ="
          //          "'"+ui->FechaInst->text()+"',"
                    "vsoft ="
                    "'"+ui->VerSoft->text()+"',"
                    "actsoft ="
                    "'"+ui->FechaSoft->text()+"',"
                    "falla ="
                    "'"+Fallas+"',"
                    "bonif ="
                    "'"+ui->Bonificacion->text()+"',"
                    "obs ="
                    "'"+ui->Observ->toPlainText()+"',"
                    "grupo ="
                    "'"+Grupo+"'"
                    " WHERE id ="
                    "'"+ui->TablaID->text()+"'"
                    "");
    }
    else if(Producto == 2)
    {
    Conf.append("UPDATE Perifericos SET "
                "nombre ="
                "'"+ui->NProductos->text()+"',"
                "sn ="
                "'"+ui->NumSerie->text()+"',"
                "ffab ="
                "'"+ui->FechaFab->text()+"',"
                "finst ="
                "'"+ui->FechaInst->text()+"',"
                "vsoft ="
                "'"+ui->VerSoft->text()+"',"
                "fsoft ="
                "'"+ui->FechaSoft->text()+"',"
                "conf ="
                "'"+ui->Configuracion->text()+"',"
                "falla ="
                "'"+Fallas+"',"
                "bonif ="
                "'"+ui->Bonificacion->text()+"',"
                "obs ="
                "'"+ui->Observ->toPlainText()+"',"
                "grupo ="
                "'"+Grupo+"'"
                " WHERE id ="
                "'"+ui->TablaID->text()+"'"
                "");
    }
    else if(Producto == 3)
    {
    Conf.append("UPDATE Instalaciones SET "
                "nombre ="
                "'"+ui->NProductos->text()+"',"
                "sn ="
                "'"+ui->NumSerie->text()+"',"
    //            "ffab ="
    //            "'"+ui->FechaFab->text()+"',"
    //            "finst ="
    //            "'"+ui->FechaInst->text()+"',"
    //            "vsoft ="
    //            "'"+ui->VerSoft->text()+"',"
    //            "fsoft ="
    //            "'"+ui->FechaSoft->text()+"',"
    //            "conf ="
    //            "'"+ui->Configuracion->text()+"',"
                "falla ="
                "'"+Fallas+"',"
                "bonif ="
                "'"+ui->Bonificacion->text()+"',"
                "obs ="
                "'"+ui->Observ->toPlainText()+"',"
                "grupo ="
                "'"+Grupo+"'"
                " WHERE id ="
                "'"+ui->TablaID->text()+"'"
                "");
    }
    QSqlQuery editar;
    editar.prepare(Conf);
    if(!editar.exec())
    {
        QMessageBox::critical(this,tr("Tabla Perifericos"),
                              tr("Falla edicion de datos\n"
                                 "%1").arg(editar.lastError().text()));
    }
 emit finalizar();
}
