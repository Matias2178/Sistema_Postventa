#include "filtroinformacion.h"
#include "ui_filtroinformacion.h"
#include <calendario.h>
#include <variables.h>
#include <QFileDialog>

FiltroInformacion::FiltroInformacion(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FiltroInformacion)
{
    ui->setupUi(this);
}

FiltroInformacion::~FiltroInformacion()
{
    delete ui;
}

void FiltroInformacion::on_bFechaInicio_clicked()
{
    Calendario *CalendarioVentana = new Calendario(this);
    CalendarioVentana->setModal(true);
    CalendarioVentana->show();
    connect (CalendarioVentana, SIGNAL(Enviar()),this, SLOT(ActFechaInicio()));
}

void FiltroInformacion::on_bFechaFin_clicked()
{
    Calendario *CalendarioVentana = new Calendario(this);
    CalendarioVentana->setModal(true);
    CalendarioVentana->show();
    connect (CalendarioVentana, SIGNAL(Enviar()),this, SLOT(ActFechaFin()));
}

void FiltroInformacion::ActFechaInicio()
{
    ui->lFechaInicio->setText(Fechas);
    FechaInicio.clear();
    FechaInicio.append(Fechas);
}

void FiltroInformacion::ActFechaFin()
{
    ui->lFechaFin->setText(Fechas);
    FechaFin.clear();
    FechaFin.append(Fechas);
}

void FiltroInformacion::on_Ingresos_clicked()
{
    QString Texto;

    //Llamo a la funcion para borrar los datos de la tabla y ajustar Columnas
    AjustaIngreso();

    Texto.clear();
    Texto.append("SELECT * FROM Reparaciones WHERE ingreso = 0");

    GenerarIngreso(Texto);
}

void FiltroInformacion::on_IngresoFecha_clicked()
{
    QString Texto;

//Llamo a la funcion para borrar los datos de la tabla y ajustar Columnas
    AjustaIngreso();

    Texto.clear();
    Texto.append("SELECT * FROM Reparaciones WHERE date(frep) between date('"
                 + FechaInicio + "') AND date('"+ FechaFin + "')");

    GenerarIngreso(Texto);
}

void FiltroInformacion::AjustaIngreso()
{
    QStringList Etiquetas {"F_Ing", "F_Rep", "Operario", "Estado", "Agente", "Zona", "Productos",
                           "Remito", "R Transporte", "F R Transt", "Observaciones", "ID"};

    while(ui->TrabajoPerifericos->columnCount()<12)
        ui->TrabajoPerifericos->insertColumn(0);

    while(ui->TrabajoPerifericos->columnCount()>12)
        ui->TrabajoPerifericos->removeColumn(0);

    ui->TrabajoPerifericos->setHorizontalHeaderLabels(Etiquetas);

    while (ui->TrabajoPerifericos->rowCount())
        ui->TrabajoPerifericos->removeRow(0);

}

void FiltroInformacion::ActualizaIngreso(int id)
{
    QSqlQuery Consulta;
    Consulta.prepare("UPDATE Reparaciones SET ingreso = 1 WHERE id =" + QString::number(id,10));
    Consulta.exec();

}

void FiltroInformacion::GenerarIngreso(QString comando)
{
    QSqlQuery Consulta,Prod,Desc,Conc;
    QString IDRep, Texto;
    int fila = 0;

    Consulta.prepare(comando);
    Consulta.exec();

    while(Consulta.next()){
        IDRep = Consulta.value("id").toString();
        Prod.prepare("SELECT SUM (cantidad), nombre FROM Ingreso WHERE repid = '" + IDRep + "' group by nombre" );
        Prod.exec();
        Texto.clear();
        while(Prod.next()){
            Texto.append(Prod.value(0).toString());
            Texto.append(" ");
            Texto.append(Prod.value(1).toString());
            Texto.append(", ");
        }
        ui->TrabajoPerifericos->insertRow(fila);
        ui->TrabajoPerifericos->setRowHeight(fila,20);
        ui->TrabajoPerifericos->setItem(fila,0,new QTableWidgetItem (Consulta.value("fing").toString()));
        ui->TrabajoPerifericos->setItem(fila,1,new QTableWidgetItem (Consulta.value("frep").toString()));
        ui->TrabajoPerifericos->setItem(fila,2,new QTableWidgetItem ("MM"));
        ui->TrabajoPerifericos->setItem(fila,3,new QTableWidgetItem ("En Proceso"));
        ui->TrabajoPerifericos->setItem(fila,4,new QTableWidgetItem (Consulta.value("agente").toString()));
        ui->TrabajoPerifericos->setItem(fila,5,new QTableWidgetItem (""));
        ui->TrabajoPerifericos->setItem(fila,6,new QTableWidgetItem (Texto));
        ui->TrabajoPerifericos->setItem(fila,7,new QTableWidgetItem (Consulta.value("ragente").toString()));
        ui->TrabajoPerifericos->setItem(fila,8,new QTableWidgetItem (Consulta.value("rtransp").toString()));
        ui->TrabajoPerifericos->setItem(fila,9,new QTableWidgetItem (Consulta.value("ftransp").toString()));
        ui->TrabajoPerifericos->setItem(fila,10,new QTableWidgetItem (Consulta.value("obs").toString()));
        ui->TrabajoPerifericos->setItem(fila,11,new QTableWidgetItem (Consulta.value("id").toString()));
        ui->TrabajoPerifericos->item(fila,0)->setCheckState(Qt::Unchecked);
        fila++;
    }
}

void FiltroInformacion::on_IngresoExcel_clicked()
{
    QString NArchivo;
    QString Dts;
    int Filas;
    int i, id;
    bool ok;
    QString Texto;
    DatosArchivo.clear();
    Filas = ui->TrabajoPerifericos->rowCount();

    if(Filas<0)
    {
        QMessageBox::critical(this,tr("Ingreso a Reparaciones"),
                              tr("Filtrar datos para generar el informe"));
        return;
    }

    Texto.clear();
    Texto.append("Ingreso");

    NArchivo.clear();
    NArchivo.append(Texto);
    NArchivo.prepend(RutaInfoExcel);
    QDir Dir;
    Dir.setCurrent(Dir.homePath());

    for(i=0;i<Filas;i++)
    {
        if(ui->TrabajoPerifericos->item(i,0)->checkState() == 2)
        {
            DatosArchivo.append(
                        ui->TrabajoPerifericos->item(i,0)->text()+ ";"
                        +   ui->TrabajoPerifericos->item(i,1)->text()+ ";"
                        +   ui->TrabajoPerifericos->item(i,2)->text()+ ";"
                        +   ui->TrabajoPerifericos->item(i,3)->text()+ ";"
                        +   ui->TrabajoPerifericos->item(i,4)->text()+ ";"
                        +   ui->TrabajoPerifericos->item(i,5)->text()+ ";"
                        +   ui->TrabajoPerifericos->item(i,6)->text()+ ";"
                        +   ui->TrabajoPerifericos->item(i,7)->text()+ ";;"
                        +   ui->TrabajoPerifericos->item(i,8)->text()+ ";"
                        +   ui->TrabajoPerifericos->item(i,9)->text()+ ";"
                        +   ui->TrabajoPerifericos->item(i,10)->text());

            id = ui->TrabajoPerifericos->item(i,11)->text().toInt(&ok);
            ActualizaIngreso(id);
        }

    }

    QString fileName = QFileDialog::getSaveFileName(
                this,"Ingreso a Reparaciones",
                NArchivo,"Text Files (*.csv);;All Files (*.csv)");

    int aa = fileName.lastIndexOf("/");
    QString direccion = fileName.mid(0,aa);

    QFile file(fileName);
    if(file.open(QFile::WriteOnly))
    {
        Texto.clear();
        Texto = DatosArchivo.join(0x0d);
        file.write(Texto.toLatin1());
        setWindowTitle(tr("Ingreso a Reparaciones - %1[*]").arg(QFileInfo(NArchivo).fileName()));
    }
    else
    {
        QMessageBox::warning(this,"TextEditor",
                    tr("Cannot write file %1.\nError: %2")
                    .arg(NArchivo).arg(file.errorString()));
    }
}

void FiltroInformacion::on_Reportes_clicked()
{

    QString Texto;
 //Ajuste y borrado de datos de la tabla
    AjustaReporte();

    Texto.clear();
    Texto.append("SELECT * FROM Reparaciones WHERE reporte = 0");

//Llamo a la funcion para cargar reportes en la tabla
    GeneraReporte(Texto, true);

}

void FiltroInformacion::on_ReportesFecha_clicked()
{

    QString Texto;
 //Ajuste y borrado de datos de la tabla
    AjustaReporte();

    Texto.clear();
    Texto.append("SELECT * FROM Reparaciones WHERE date(frep) between date('"
                 + FechaInicio + "') AND date('"+ FechaFin + "')");

//Llamo a la funcion para cargar reportes en la tabla
    GeneraReporte(Texto, false);
}

void FiltroInformacion::AjustaReporte()
{
    while(ui->TrabajoPerifericos->columnCount()<16)
        ui->TrabajoPerifericos->insertColumn(0);

    while(ui->TrabajoPerifericos->columnCount()>16)
        ui->TrabajoPerifericos->removeColumn(0);

    QStringList Etiquetas {"Fecha Rep", "Agente", "Cliente", "Cantidad", "Producto", "Num Serie", "Fecha Fab",
                           "Version", "Ver Act", "Falla", "Reparacion", "Bonif", "Garantia", "Observaciones", "ID", "Tabla"};
    ui->TrabajoPerifericos->setHorizontalHeaderLabels(Etiquetas);

    while (ui->TrabajoPerifericos->rowCount())
        ui->TrabajoPerifericos->removeRow(0);
}

void FiltroInformacion::GeneraReporte(QString comando, bool rep)
{
    QSqlQuery Consulta,Prod,Desc,Conc;
    QString IDRep, Texto, Tabla;
    int fila = 0;

    Consulta.prepare(comando);
    Consulta.exec();
    while(Consulta.next()){
        IDRep.clear();
        IDRep.append(Consulta.value(0).toString());

        for (int i=0;i<5;i++){

            Tabla.clear();
            if(i==0)       {        //Monitores
                if(rep){
                    Prod.prepare("Select * FROM Monitores WHERE reporte = 0 AND repid = " + IDRep );
                }
                else {
                    Prod.prepare("Select * FROM Monitores WHERE repid = " + IDRep );
                }
                Tabla.append("Monitores");
            }

            else if (i==1) {        //Perifericos
                if(rep) {
                    Prod.prepare("Select * FROM Perifericos WHERE reporte = 0 AND repid = " + IDRep );
                }
                else {
                    Prod.prepare("Select * FROM Perifericos WHERE repid = " + IDRep );
                }

                Tabla.append("Perifericos");
            }

            else if (i==2) {        //Instalaciones
                if(rep){
                    Prod.prepare("Select * FROM Instalaciones WHERE reporte = 0 AND repid = " + IDRep );
                }
                else {
                    Prod.prepare("Select * FROM Instalaciones WHERE repid = " + IDRep );
                }
                Tabla.append("Instalaciones");
            }

            else if (i==3) {        //Caudalimetros
                if(rep){
                    Prod.prepare("Select * FROM Caudalimetro WHERE reporte = 0 AND repid = " + IDRep );
                }
                else {
                    Prod.prepare("Select * FROM Caudalimetro WHERE repid = " + IDRep );
                }
                Tabla.append("Caudalimetro");
            }

            else if (i==4) {        //Insumos
                if(rep){
                    Prod.prepare("Select * FROM Insumos WHERE reporte = 0 AND repid = " + IDRep );
                }
                else {
                    Prod.prepare("Select * FROM Insumos WHERE repid = " + IDRep );
                }
                Tabla.append("Insumos");
            }

            Prod.exec();
            while(Prod.next()){
                Texto.clear();
                Texto.append(Prod.value("nombre").toString());
                Desc.prepare("SELECT desc FROM Productos WHERE producto ='" + Texto + "'");
                Desc.exec();
                Desc.next();
                Texto.clear();
                Texto.append(Prod.value("concepto").toString());
                //       qDebug () << Texto;
                Conc.prepare("SELECT concepto FROM Conceptos WHERE id = '" + Texto + "'" );
                Conc.exec();
                Conc.next();
                ui->TrabajoPerifericos->insertRow(fila);
                ui->TrabajoPerifericos->setRowHeight(fila,20);
                ui->TrabajoPerifericos->setItem(fila,0,new QTableWidgetItem (Consulta.value("frep").toString()));
                ui->TrabajoPerifericos->setItem(fila,1,new QTableWidgetItem (Consulta.value("agente").toString()));
                ui->TrabajoPerifericos->setItem(fila,2,new QTableWidgetItem (" "));
                ui->TrabajoPerifericos->setItem(fila,3,new QTableWidgetItem (Prod.value("cantidad").toString()));
                ui->TrabajoPerifericos->setItem(fila,4,new QTableWidgetItem (Desc.value(0).toString()));
                ui->TrabajoPerifericos->setItem(fila,5,new QTableWidgetItem (Prod.value("sn").toString()));
                if(i==0 || i==2 || i==4) {
                    ui->TrabajoPerifericos->setItem(fila,6,new QTableWidgetItem (" "));
                }

                else {
                    ui->TrabajoPerifericos->setItem(fila,6,new QTableWidgetItem (Prod.value("ffab").toString()));
                }
                if(i==2 || i==4){
                    ui->TrabajoPerifericos->setItem(fila,7,new QTableWidgetItem (" "));
                }

                else{
                    ui->TrabajoPerifericos->setItem(fila,7,new QTableWidgetItem (Prod.value("vsoft").toString()));
                }
                if(i==0){
                    ui->TrabajoPerifericos->setItem(fila,8,new QTableWidgetItem (Prod.value("actsoft").toString()));
                }
                else{

                    ui->TrabajoPerifericos->setItem(fila,8,new QTableWidgetItem (" "));
                }
                ui->TrabajoPerifericos->setItem(fila,9,new QTableWidgetItem (Prod.value("falla").toString()));
           //     ui->TrabajoPerifericos->setItem(fila,10,new QTableWidgetItem (Conc.value("concepto").toString()));
                ui->TrabajoPerifericos->setItem(fila,10,new QTableWidgetItem (""));
                ui->TrabajoPerifericos->setItem(fila,11,new QTableWidgetItem (Prod.value("bonif").toString()));
                ui->TrabajoPerifericos->setItem(fila,12,new QTableWidgetItem (Conc.value("concepto").toString()));
                ui->TrabajoPerifericos->setItem(fila,13,new QTableWidgetItem (Prod.value("obs").toString()));
                ui->TrabajoPerifericos->setItem(fila,14,new QTableWidgetItem (Prod.value("id").toString()));
                ui->TrabajoPerifericos->setItem(fila,15,new QTableWidgetItem (Tabla));
                ui->TrabajoPerifericos->item(fila,0)->setCheckState(Qt::Unchecked);
                fila++;
            }
        }
    }
}

void FiltroInformacion::ActualizarReporte()
{
    QSqlQuery Actualizar, Busca;
    QString id;
    int contador;

    Actualizar.prepare("SELECT id FROM Reparaciones WHERE Reporte = 0");
    Actualizar.exec();
    while (Actualizar.next()) {
        contador = 0;
        id.clear();
        id.append(Actualizar.value(0).toString());
        Busca.prepare("SELECT COUNT(*) FROM Monitores WHERE reporte = 0 AND repid = '" + id + "'");
        Busca.exec();
        Busca.next();
        contador = contador + Busca.value(0).toInt(); // devuelve la cantidad
//        qDebug () << "Monitores"<< contador;

        Busca.prepare("SELECT COUNT(*) FROM Perifericos WHERE reporte = 0 AND  repid = '" + id + "'");
        Busca.exec();
        Busca.next();
        contador = contador + Busca.value(0).toInt(); // devuelve la cantidad
 //       qDebug () << "Perifericos "<< Busca.value(0).toInt();

        Busca.prepare("SELECT COUNT(*) FROM Instalaciones WHERE reporte = 0 AND  repid = '" + id + "'");
        Busca.exec();
        Busca.next();
        contador = contador + Busca.value(0).toInt(); // devuelve la cantidad
//        qDebug () << "Instalaciones "<< Busca.value(0).toInt();

        Busca.prepare("SELECT COUNT(*) FROM Insumos WHERE reporte = 0 AND  repid = '" + id + "'");
        Busca.exec();
        Busca.next();
        contador = contador + Busca.value(0).toInt(); // devuelve la cantidad
 //       qDebug () << "Insumos "<< Busca.value(0).toInt();

        Busca.prepare("SELECT COUNT(*) FROM Caudalimetro WHERE reporte = 0 AND  repid = '" + id + "'");
        Busca.exec();
        Busca.next();
        contador = contador + Busca.value(0).toInt(); // devuelve la cantidad
//        qDebug () << id << "Caudalimetro "<< Busca.value(0).toInt() << contador;

        qDebug () << id <<  contador;

        if(!contador){
            Busca.prepare("UPDATE Reparaciones SET reporte = 1 WHERE id ='" + id + "'");
            Busca.exec();
            Busca.next();
            qDebug () << id;
        }

    }
}

void FiltroInformacion::on_ReportesExcel_clicked()
{
    QString NArchivo;
    QString ID, Texto;
    int Filas;
    int i;
    QSqlQuery Actualiza;

    DatosArchivo.clear();
    Filas = ui->TrabajoPerifericos->rowCount();

    if(Filas<0)
    {
        QMessageBox::critical(this,tr("Informe Reparaciones"),
                              tr("Filtrar datos para generar el informe"));
        return;
    }

    Texto.clear();
    Texto.append("Reporte");

    NArchivo.clear();
    NArchivo.append(Texto);
    NArchivo.prepend(RutaInfoExcel);
    QDir Dir;
    Dir.setCurrent(Dir.homePath());

    for(i=0;i<Filas;i++)
    {
        if(ui->TrabajoPerifericos->item(i,0)->checkState() == 2)
        {
            DatosArchivo.append(
                        ui->TrabajoPerifericos->item(i,0)->text()+ ";"
                        +   ui->TrabajoPerifericos->item(i,1)->text()+ ";"
                        +   ui->TrabajoPerifericos->item(i,2)->text()+ ";"
                        +   ui->TrabajoPerifericos->item(i,3)->text()+ ";"
                        +   ui->TrabajoPerifericos->item(i,4)->text()+ ";"
                        +   ui->TrabajoPerifericos->item(i,5)->text()+ ";"
                        +   ui->TrabajoPerifericos->item(i,6)->text()+ ";"
                        +   ui->TrabajoPerifericos->item(i,7)->text()+ ";"
                        +   ui->TrabajoPerifericos->item(i,8)->text()+ ";"
                        +   ui->TrabajoPerifericos->item(i,9)->text()+ ";"
                        +   ui->TrabajoPerifericos->item(i,10)->text()+ ";"
                        +   ui->TrabajoPerifericos->item(i,11)->text()+ ";"
                        +   ui->TrabajoPerifericos->item(i,12)->text()+ ";"
                        +   ui->TrabajoPerifericos->item(i,13)->text()+ ";"
                        +   ui->TrabajoPerifericos->item(i,14)->text());
            Texto.clear();
            Texto.append(ui->TrabajoPerifericos->item(i,15)->text());  //cargo el nombre de la tabla
            ID.clear();
            ID.append(ui->TrabajoPerifericos->item(i,14)->text());  //Cargo el ID del que tengo que corregir
            Actualiza.prepare("UPDATE '" + Texto + "' SET reporte = 1 WHERE id = '" + ID + "'");
            Actualiza.exec();
            Actualiza.next();
            QString hola;
            hola.clear();
            hola.append("UPDATE '" + Texto + "' SET reporte = 1 WHERE id = '" + ID + "'");
            qDebug () << hola;
        }
    }

    QString fileName = QFileDialog::getSaveFileName(
                this,"Reportes de reparaciones",
                NArchivo,"Text Files (*.csv);;All Files (*.csv)");

    int aa = fileName.lastIndexOf("/");
    QString direccion = fileName.mid(0,aa);

    QFile file(fileName);
    if(file.open(QFile::WriteOnly))
    {
        Texto.clear();
        Texto = DatosArchivo.join(0x0d);
        file.write(Texto.toLatin1());
        setWindowTitle(tr("Reporte de Reparaciones - %1[*]").arg(QFileInfo(NArchivo).fileName()));
    }
    else
    {
        QMessageBox::warning(this,"TextEditor",
                    tr("Cannot write file %1.\nError: %2")
                    .arg(NArchivo).arg(file.errorString()));
    }
    ActualizarReporte();
}

QString FiltroInformacion::CambioFecha(QString fecha)
{
    int ind, dato;
    QString fecha2;
    fecha2.clear();
    ind = fecha.lastIndexOf('/');
    if(ind <1)
        return fecha2;

    dato = fecha.size();
    fecha2.append(fecha.mid(ind+1,dato)+'-');

    fecha.truncate(ind);
    ind = fecha.lastIndexOf('/');
    dato = fecha.size();
    fecha2.append(fecha.mid(ind+1,dato)+'-');
    fecha.truncate(ind);
    ind = fecha.lastIndexOf('/');
    dato = fecha.size();
    fecha2.append(fecha.mid(ind+1,dato));
  //   qDebug ()  << Fecha << Fecha2;
    return fecha2;
}

void FiltroInformacion::on_AjusteFechas_clicked()
{
    QSqlQuery Consulta,Cambio;
    QString Fecha1, Fecha, id,Tabla;
    QString Datos;
    int i;

    for (i=0;i<6;i++)
    {
        Tabla.clear();
        if(i==1){
            Tabla.append("Reparaciones");
        }
        else if (i==2) {
            Tabla.append("Perifericos");
        }
        else if (i==3) {
            Tabla.append("Monitores");
        }
        else if (i==4) {
            Tabla.append("Instalaciones");
        }
        else if (i==5) {
            Tabla.append("Caudalimetro");
        }
        Datos.clear();
        Datos.append("SELECT id,frep FROM '" + Tabla + "'");
        Consulta.prepare(Datos);
        Consulta.exec();

        while(Consulta.next())
        {
            Fecha1.clear();
            Fecha.clear();
            id.clear();
            Fecha1 = Consulta.value(1).toString();
            id.append(Consulta.value(0).toString());
            Fecha.append(CambioFecha(Fecha1));
            if (Fecha.isEmpty()){
                continue;
            }
            Datos.clear();
            Datos.append("UPDATE '" + Tabla + "' SET frep = '" + Fecha + "' WHERE id ='" + id+ "'");
            Cambio.prepare(Datos);

        //    qDebug() << Datos;

            if(!Cambio.exec())
            {
                QMessageBox::critical(this,tr("Tabla Productos"),
                                      tr("Falla al crear la tabla\n"
                                         "%1").arg(Cambio.lastError().text()));
                return;
            }
        }
    }
}





