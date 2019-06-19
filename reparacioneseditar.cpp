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

    consultar.prepare("SELECT * FROM Conceptos WHERE id != 0");
    consultar.exec();
    while(consultar.next()){
        ui->Concepto->addItem(consultar.value(1).toString());
    }

    ui->TablaID->setText(ID);
    Fallas.clear();
    Producto = TipoProd;
    if(TipoProd == 1)
    {
        conf.append("SELECT * FROM Monitores WHERE id =="  "'"  +  ID+  "'" );
        consultar.prepare(conf);
        consultar.exec();
        consultar.next();

        ui->FechaFab->setText("");
        ui->VerSoft->setText(consultar.value("vsoft").toString());
        ui->FechaSoft->setText(consultar.value("actsoft").toString());
        ui->label_18->setText("Ver Act Soft");
    }
    else if(TipoProd == 2)
    {
        conf.append("SELECT * FROM Perifericos WHERE id =="  "'"  +  ID+  "'" );
        consultar.prepare(conf);
        consultar.exec();
        consultar.next();

        ui->FechaFab->setText(consultar.value("ffab").toString());
        ui->VerSoft->setText(consultar.value("vsoft").toString());
        ui->FechaSoft->setText(consultar.value("fsoft").toString());
        ui->FechaInst->setText(consultar.value("finst").toString());
        ui->Configuracion->setText(consultar.value("conf").toString());
        ui->label_18->setText("Fecha Soft");
    }
    else if(TipoProd == 3)
    {
        conf.append("SELECT * FROM Instalaciones WHERE id =="  "'"  +  ID+  "'" );
        consultar.prepare(conf);
        consultar.exec();
        consultar.next();
    }
    else if(TipoProd == 4)
    {
        conf.append("SELECT * FROM Caudalimetro WHERE id =="  "'"  +  ID+  "'" );
        consultar.prepare(conf);
        consultar.exec();
        consultar.next();

        ui->FechaFab->setText(consultar.value("ffab").toString());
        ui->VerSoft->setText(consultar.value("vsoft").toString());
        ui->FechaSoft->setText(consultar.value("fsoft").toString());
        ui->FechaInst->setText(consultar.value("finst").toString());
        ui->label_18->setText("Fecha Soft");    
    }
    else if(TipoProd == 5)
    {
        conf.append("SELECT * FROM Insumos WHERE id =="  "'"  +  ID+  "'" );
        consultar.prepare(conf);
        consultar.exec();
        consultar.next();
    }

    Codigo.clear();
    Codigo.append(consultar.value("nombre").toString());
    ui->NProductos->setText(consultar.value("nombre").toString());
    ui->NumSerie->setText(consultar.value("sn").toString());
    Fallas.append(consultar.value("falla").toString())  ;
    ui->Observ->setText(consultar.value("obs").toString());
    ui->Bonificacion->setText(consultar.value("bonif").toString());
    ui->Concepto->setCurrentIndex(consultar.value("concepto").toInt());
    ui->Cantidad->setText(consultar.value("cantidad").toString());
    ui->FechaReparacion->setText(consultar.value("frep").toString());
    EditRep.CargarProd(*ui->Productos,TipoProd);
    EditRep.CargarFallas(*ui->Fallas,Codigo);

    tFallas.SetFallas(*ui->Fallas,Fallas);


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
    QString Fallas, Grupo, concep;
    QList <QString> lFallas;

    Fallas.clear();
    Grupo.clear();
    lFallas.clear();

    lFallas << tFallas.GetFallas(*ui->Fallas);

    Fallas.append(lFallas[0]);
    Grupo.append(lFallas[1]);

    QString Conf;
    concep.clear();
    concep.append(QString::number(ui->Concepto->currentIndex()));

    qDebug () << Producto <<ui->TablaID->text();

    if(Producto == 1)
    {
        Conf.append("UPDATE Monitores SET "
                    "nombre ='"     +ui->NProductos->text()+"',"
                    "sn ='"         +ui->NumSerie->text()+"',"
                    "vsoft ='"      +ui->VerSoft->text()+"',"
                    "actsoft ='"    +ui->FechaSoft->text()+"',"
                    "frep ='"       +ui->FechaReparacion->text()+"',"
                    "falla ='"      +Fallas+"',"
                    "bonif ='"      +ui->Bonificacion->text()+"',"
                    "obs ='"        +ui->Observ->toPlainText()+"',"
                    "grupo ='"      +Grupo+"',"
                    "concepto = '"  +concep+"',"
                    "cantidad = '"  +ui->Cantidad->text()+"'"
                    " WHERE id ='"  +ui->TablaID->text()+"'");
        Grupo.clear();
        Grupo.append("Tabla Monitores");
    }
    else if(Producto == 2)
    {
    Conf.append("UPDATE Perifericos SET "
                "nombre = '"     +ui->NProductos->text()+"',"
                "sn = '"        +ui->NumSerie->text()+"',"
                "ffab = '"      +ui->FechaFab->text()+"',"
                "finst = '"     +ui->FechaInst->text()+"',"
                "vsoft = '"     +ui->VerSoft->text()+"',"
                "fsoft = '"     +ui->FechaSoft->text()+"',"
                "conf = '"      +ui->Configuracion->text()+"',"
                "falla = '"     +Fallas+"',"
                "bonif = '"     +ui->Bonificacion->text()+"',"
                "obs = '"       +ui->Observ->toPlainText()+"',"
                "grupo = '"     +Grupo+"',"
                "concepto = '"  +concep+"',"
                "cantidad = '"  +ui->Cantidad->text()+"'"
                " WHERE id ='"  +ui->TablaID->text()+"'");

        Grupo.clear();
        Grupo.append("Tabla Perifericos");
    }
    else if(Producto == 3)
    {
    Conf.append("UPDATE Instalaciones SET "
                "nombre ='"     +ui->NProductos->text()+"',"
                "sn ='"         +ui->NumSerie->text()+"',"
                "falla ='"      +Fallas+"',"
                "bonif ='"      +ui->Bonificacion->text()+"',"
                "obs ='"        +ui->Observ->toPlainText()+"',"
                "grupo ='"      +Grupo+"',"
                "concepto = '"  +concep+"',"
                "cantidad = '"  +ui->Cantidad->text()+"'"
                " WHERE id ='"  +ui->TablaID->text()+"'");

        Grupo.clear();
        Grupo.append("Tabla Instalaciones");
    }
    else if(Producto == 4)
    {
        Conf.append("UPDATE Caudalimetro SET "
                "nombre ='"     +ui->NProductos->text()+"',"
                "sn ='"         +ui->NumSerie->text()+"',"
                "ffab ='"       +ui->FechaFab->text()+"',"
                "finst ='"      +ui->FechaInst->text()+"',"
                "vsoft ='"      +ui->VerSoft->text()+"',"
                "fsoft ='"      +ui->FechaSoft->text()+"',"
                "falla ='"      +Fallas+"',"
                "bonif ='"      +ui->Bonificacion->text()+"',"
                "obs ='"        +ui->Observ->toPlainText()+"',"
                "grupo ='"      +Grupo+"',"
                "concepto = '"  +concep+"',"
                "cantidad = '"  +ui->Cantidad->text()+"'"
                "WHERE id ='"   +ui->TablaID->text()+"'");

        Grupo.clear();
        Grupo.append("Tabla Caudalimeto");

    }
    QSqlQuery editar;
    editar.prepare(Conf);
    if(!editar.exec())
    {
        QMessageBox::critical(this,tr(Grupo.toLocal8Bit()),
                              tr("Falla edicion de datos\n"
                                 "%1").arg(editar.lastError().text()));
    }
 emit finalizar();
}
