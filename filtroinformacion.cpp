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

void FiltroInformacion::on_FiltrarDatos_clicked()
{
    QSqlQuery Consulta,Prod,Desc,Conc;
    QString IDRep;
    QString Texto;
    int fila = 0;
    qDebug () << FechaInicio << FechaFin;
    Texto.clear();
    Texto.append("SELECT * FROM Reparaciones WHERE date(frep) between date('"
                 + FechaInicio + "') AND date('"+ FechaFin + "')");
    //qDebug () << Texto;
    Consulta.prepare(Texto);
    Consulta.exec();
    while(Consulta.next()){
        IDRep.clear();
        IDRep.append(Consulta.value(0).toString());

        for (int i=0;i<4;i++){

            if(i==0)       {        //Monitores
                Prod.prepare("Select * FROM Monitores WHERE repid = " + IDRep );
                qDebug () << "Monitores";
            }
            else if (i==1) {        //Perifericos
                Prod.prepare("Select * FROM Perifericos WHERE repid = " + IDRep );
                qDebug () << "Perifericos";
            }
            else if (i==2) {        //Instalaciones
                Prod.prepare("Select * FROM Instalaciones WHERE repid = " + IDRep );
                qDebug () << "Instalaciones";
            }
            else if (i==3) {        //Caudalimetros
                Prod.prepare("Select * FROM Caudalimetro WHERE repid = " + IDRep );
                qDebug () << "Caudalimetro";
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
                ui->TrabajoPerifericos->setItem(fila,1,new QTableWidgetItem (Consulta.value("agente").toString()));
                ui->TrabajoPerifericos->setItem(fila,0,new QTableWidgetItem (Consulta.value("frep").toString()));
                ui->TrabajoPerifericos->setItem(fila,2,new QTableWidgetItem ("1"));
                ui->TrabajoPerifericos->setItem(fila,3,new QTableWidgetItem (Desc.value(0).toString()));
                ui->TrabajoPerifericos->setItem(fila,4,new QTableWidgetItem (Prod.value("sn").toString()));
                if(i==0 || i==2) {
                    ui->TrabajoPerifericos->setItem(fila,5,new QTableWidgetItem (" "));
                }
                else {
                    ui->TrabajoPerifericos->setItem(fila,5,new QTableWidgetItem (Prod.value("ffab").toString()));
                }
                if(i==2){
                    ui->TrabajoPerifericos->setItem(fila,6,new QTableWidgetItem (Prod.value("vsoft").toString()));

                }
                else{
                    ui->TrabajoPerifericos->setItem(fila,6,new QTableWidgetItem (" "));
                }

                ui->TrabajoPerifericos->setItem(fila,7,new QTableWidgetItem (Prod.value("falla").toString()));
                ui->TrabajoPerifericos->setItem(fila,8,new QTableWidgetItem (Prod.value("bonif").toString()));
                ui->TrabajoPerifericos->setItem(fila,9,new QTableWidgetItem (Conc.value("concepto").toString()));
                ui->TrabajoPerifericos->setItem(fila,10,new QTableWidgetItem (Prod.value("obs").toString()));
         //       qDebug () << Conc.value("concepto").toString() <<  Conc.value(0).toString();
                fila++;
            }
        }
    } 
}

void FiltroInformacion::on_ExportarExcel_clicked()
{
    QString NArchivo;
    QString Dts;
    int Filas;
    int i;
    QString Texto;
    DatosArchivo.clear();
    Filas = ui->TrabajoPerifericos->rowCount();

    if(Filas<0)
    {
        QMessageBox::critical(this,tr("Informe Reparaciones"),
                              tr("Fitrar datos para generar el informe"));
        return;
    }

    Texto.clear();
    Texto.append("Repracion");

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
                        +   ui->TrabajoPerifericos->item(i,1)->text()+ ";;"
                        +   ui->TrabajoPerifericos->item(i,2)->text()+ ";"
                        +   ui->TrabajoPerifericos->item(i,3)->text()+ ";"
                        +   ui->TrabajoPerifericos->item(i,4)->text()+ ";"
                        +   ui->TrabajoPerifericos->item(i,5)->text()+ ";"
                        +   ui->TrabajoPerifericos->item(i,6)->text()+ ";"
                        +   ui->TrabajoPerifericos->item(i,7)->text()+ ";;"
                        +   ui->TrabajoPerifericos->item(i,8)->text()+ ";"
                        +   ui->TrabajoPerifericos->item(i,9)->text()+ ";"
                        +   ui->TrabajoPerifericos->item(i,10)->text());
        }
    }

    QString fileName = QFileDialog::getSaveFileName(
                this,"Analisis de equipo - Guardado de archivos",
                NArchivo,"Text Files (*.csv);;All Files (*.csv)");

    int aa = fileName.lastIndexOf("/");
    QString direccion = fileName.mid(0,aa);

    QFile file(fileName);
    if(file.open(QFile::WriteOnly))
    {
        Texto.clear();
        Texto = DatosArchivo.join(0x0d);
        file.write(Texto.toLatin1());
        setWindowTitle(tr("Analisis Equipos - %1[*]").arg(QFileInfo(NArchivo).fileName()));
    }
    else
    {
        QMessageBox::warning(this,"TextEditor",
                    tr("Cannot write file %1.\nError: %2")
                    .arg(NArchivo).arg(file.errorString()));
    }
}


void FiltroInformacion::on_Ingresos_clicked()
{
    QSqlQuery Consulta,Prod,Desc,Conc;
    QString IDRep;
    QString Texto;
    QStringList Etiquetas {"F_Ing", "F_Rep","Operario","Estado", "Agente", "Zona", "Productos", "Remito", "R Transporte", "F R Transt", "Observaciones"};
    ui->TrabajoPerifericos->setHorizontalHeaderLabels(Etiquetas);
    int fila = 0;

    Consulta.prepare("SELECT * FROM Reparaciones WHERE ingreso = 0");
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
        ui->TrabajoPerifericos->item(fila,0)->setCheckState(Qt::Unchecked);
        fila++;
    }

}

void FiltroInformacion::on_Informes_clicked()
{
    QSqlQuery Consulta,Prod,Desc,Conc;
    QString IDRep;
    QString Texto;
    int fila = 0;
    qDebug () << FechaInicio << FechaFin;

    Consulta.prepare("SELECT * FROM Reparaciones WHERE reporte = 0");
    Consulta.exec();
    while(Consulta.next()){
        IDRep.clear();
        IDRep.append(Consulta.value(0).toString());
   // qDebug () << IDRep;
        for (int i=0;i<4;i++){

            if(i==0)       {        //Monitores
                Prod.prepare("Select * FROM Monitores WHERE repid = " + IDRep );
           //     qDebug () << "Monitores";
            }
            else if (i==1) {        //Perifericos
                Prod.prepare("Select * FROM Perifericos WHERE repid = " + IDRep );
           //     qDebug () << "Perifericos";
            }
            else if (i==2) {        //Instalaciones
                Prod.prepare("Select * FROM Instalaciones WHERE repid = " + IDRep );
          //      qDebug () << "Instalaciones";
            }
            else if (i==3) {        //Caudalimetros
                Prod.prepare("Select * FROM Caudalimetro WHERE repid = " + IDRep );
         //       qDebug () << "Caudalimetro";
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
                Conc.prepare("SELECT concepto FROM Conceptos WHERE id = '" + Texto + "'" );
                Conc.exec();
                Conc.next();
                ui->TrabajoPerifericos->insertRow(fila);
                ui->TrabajoPerifericos->setRowHeight(fila,20);
                ui->TrabajoPerifericos->setItem(fila,1,new QTableWidgetItem (Consulta.value("agente").toString()));
                ui->TrabajoPerifericos->setItem(fila,0,new QTableWidgetItem (Consulta.value("frep").toString()));
                ui->TrabajoPerifericos->setItem(fila,2,new QTableWidgetItem ("1"));
                ui->TrabajoPerifericos->setItem(fila,3,new QTableWidgetItem (Desc.value(0).toString()));
                ui->TrabajoPerifericos->setItem(fila,4,new QTableWidgetItem (Prod.value("sn").toString()));
                if(i==0 || i==2) {
                    ui->TrabajoPerifericos->setItem(fila,5,new QTableWidgetItem (" "));
                }
                else {
                    ui->TrabajoPerifericos->setItem(fila,5,new QTableWidgetItem (Prod.value("ffab").toString()));
                }
                if(i==2){
                    ui->TrabajoPerifericos->setItem(fila,6,new QTableWidgetItem (Prod.value("vsoft").toString()));

                }
                else{
                    ui->TrabajoPerifericos->setItem(fila,6,new QTableWidgetItem (" "));
                }

                ui->TrabajoPerifericos->setItem(fila,7,new QTableWidgetItem (Prod.value("falla").toString()));
                ui->TrabajoPerifericos->setItem(fila,8,new QTableWidgetItem (Prod.value("bonif").toString()));
                ui->TrabajoPerifericos->setItem(fila,9,new QTableWidgetItem (Conc.value("concepto").toString()));
                ui->TrabajoPerifericos->setItem(fila,10,new QTableWidgetItem (Prod.value("obs").toString()));
                ui->TrabajoPerifericos->item(fila,0)->setCheckState(Qt::Unchecked);
                fila++;
            }
        }
    }
}
