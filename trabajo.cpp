#include "trabajo.h"
#include "ui_trabajo.h"
#include <QDateTime>
#include <QMessageBox>
#include <QDebug>
#include <QFile>
#include <QtWidgets>
#include <variables.h>
#include <QtPrintSupport/QPrinter>
#include <QPainter>
#include <QTextCharFormat>
#include <dbmanejo.h>

QDateTime dReparacion;
dbManejo dbTrabajo;


trabajo::trabajo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::trabajo)
{
    ui->setupUi(this);
    ui->TrabFechaRep->setInputMask("00/00/0000");
    ui->TrabFechaRep->setText(dReparacion.currentDateTime().toString("ddMMyyyy"));
    CargarRecepcion();
    ui->TrabajoReparaciones->setSortingEnabled(true);
    dbTrabajo.CargarReparaciones(*ui->TrabajoReparaciones,"*");
}

trabajo::~trabajo()
{
    delete ui;
}


void trabajo::on_ReparacionesIniciar_clicked()
{
    if(ui->TrabRepID->text().isEmpty())
    {
        QMessageBox::critical(this,tr("Trabajo"),
                                  tr("Seleccionar Trabajo para cargar datos"));
        return;
    }

    if(ui->TrabajoReparaciones->item(IndexTrabajo,3)->text().isEmpty())
    {
        if(!ui->TrabajoOperario->currentIndex())
        {
            QMessageBox::critical(this,tr("Operario"),
                                  tr("Seleccionar Operario"));
            return;
        }
        QString Conf;
        Conf.append("UPDATE Reparaciones SET "
                    "frep ="
                    "'"+ui->TrabFechaRep->text()+"',"
                    "operario ="
                    "'"+ui->TrabajoOperario->currentText()+"'"
                    " WHERE id ="
                    ""+ui->TrabRepID->text()+""
                    "");
        QSqlQuery editar;
        if(!editar.prepare(Conf))
        {
            QMessageBox::critical(this,tr("Tabla Reparaciones"),
                                  tr("Falla al Actualizar la tabla\n"
                                 "%1").arg(editar.lastError().text()));
        }
        editar.exec();

    }
    IdReparacion = TrabajoID;

    int fila;
    QString AgenteText;
    fila = ui->TrabajoAgenteTabla->currentIndex().row();
    if (fila<0)
    {
       dbTrabajo.CargarReparaciones(*ui->TrabajoReparaciones,"*");
    }
    else
    {
        AgenteText.clear();
        AgenteText.append(ui->TrabajoAgenteTabla->item(IndexAgente,0)->text());

        dbTrabajo.CargarReparaciones(*ui->TrabajoReparaciones,AgenteText);
    }

}

void trabajo::on_RepInterno_clicked()
{
    QString NArchivo;
    QString Dts;
    int Filas;
    int i, a;
    DatosArchivo.clear();
    int fila;
    QString AgenteText;

    fila = ui->TrabajoReparaciones->currentIndex().row();
    qDebug () << fila;
    if(fila<0)
    {
        QMessageBox::critical(this,tr("Selección Trabajo"),
                              tr("Seleccionar trabajo para generar el informe"));
        return;

    }

    AgenteText.clear();
    AgenteText.append(ui->TrabajoReparaciones->item(fila,0)->text());
    qDebug () << AgenteText;
    if(AgenteText.isEmpty())
    {
        QMessageBox::critical(this,tr("Selección Trabajo"),
                              tr("Seleccionar trabajo para generar el informe"));
        return;

    }
    if(ui->TrabajoReparaciones->item(IndexTrabajo,3)->text().isEmpty())
    {
        QMessageBox::information(this,tr("Trabajo"),
                              tr("Iniciar reparacion para generar el reporte"));
        return;
    }
    NArchivo.clear();
    NArchivo.append(ui->TrabajoReparaciones->item(IndexTrabajo,3)->text());
    NArchivo.replace(2,1,"_");
    NArchivo.replace(5,1,"_");
    NArchivo.prepend("_");
    NArchivo.prepend(ui->TrabajoReparaciones->item(IndexTrabajo,1)->text());
    NArchivo.prepend("user/home/");

    DatosArchivo.append("Informe de Reparaciones");
    DatosArchivo.append("Agente:;;" + ui->TrabajoReparaciones->item(IndexTrabajo,1)->text()
                         + ";;;Fecha Ingreso :;" + ui->TrabajoReparaciones->item(IndexTrabajo,2)->text());

    DatosArchivo.append("Operario:;;" + ui->TrabajoReparaciones->item(IndexTrabajo,4)->text()
                        + ";;;Fecha Control:;" + ui->TrabajoReparaciones->item(IndexTrabajo,3)->text());
    DatosArchivo.append(" ");
    DatosArchivo.append("Nombre;;Descripcion;;;;N Serie;Cant;Fact;Comentario");
    Filas = ui->TrabajoIngreso->rowCount();
    for(i=0;i<Filas;i++)
    {
       DatosArchivo.append(  ui->TrabajoIngreso->item(i,1)->text()+ ";;"
                    + ui->TrabajoIngreso->item(i,2)->text()+ ";;;;"
                    + ui->TrabajoIngreso->item(i,3)->text()+ ";"
                    + ui->TrabajoIngreso->item(i,4)->text()+ ";"
                    + ui->TrabajoIngreso->item(i,5)->text()+ ";"
                    + ui->TrabajoIngreso->item(i,6)->text());
    }
    if(ui->TrabajoCaudalimetro->rowCount())
    {
        DatosArchivo.append(" ");
        DatosArchivo.append("Nombre;SN;Movil;F Fab;F Inst;V Soft;F Soft;TMT;CPT;DESC;DESC AT;BLOC MAG;T.BLOC MAG;Falla;Bonif;Obs;F Rep");
        Filas = ui->TrabajoCaudalimetro->rowCount();
        for(i=0;i<Filas;i++)
        {
            Dts.clear();
            for(a=1;a<=17;a++)
            {
                Dts.append(ui->TrabajoCaudalimetro->item(i,a)->text() + ";");
            }
            DatosArchivo.append(Dts);
        }
    }
    if(ui->TrabajoPerifericos->rowCount())
    {
        DatosArchivo.append(" ");
        DatosArchivo.append("Nombre;SN;F Fab;F Inst;V Soft;F Soft;Configuración;Falla;Bonif;Obs;F Rep");
        Filas = ui->TrabajoPerifericos->rowCount();
        for(i=0;i<Filas;i++)
        {
            Dts.clear();
            for(a=0;a<=10;a++)
            {
                Dts.append(ui->TrabajoPerifericos->item(i,a)->text() + ";");
            }
            DatosArchivo.append(Dts);
        }
    }
    QString fileName = QFileDialog::getSaveFileName(
                this,
                "Analisis de equipo - Guardado de archivos",
                NArchivo,
                "Text Files (*.csv);;All Files (*.csv)");
    if(!fileName.isEmpty()){
        //curFile = fileName;
        if(saveFile(fileName) && ReporteOk) //Se genero el reporte, se activa la fecha del reporte
        {
            FPresupuesto();
        }
    }
}
bool trabajo::saveFile(const QString &fileName)
{
    QString info;
    QFile file(fileName);
    if(file.open(QFile::WriteOnly))
    {
        info = DatosArchivo.join(0x0d);
        file.write(info.toLatin1());
        setWindowTitle(tr("Analisis Equipos - %1[*]").arg(QFileInfo(curFile).fileName()));
        return true;
    }
    else
    {
        QMessageBox::warning(
                    this,
                    "TextEditor",
                    tr("Cannot write file %1.\nError: %2")
                    .arg(curFile)
                    .arg(file.errorString()));
        return false;
    }
}

void trabajo::on_TrabajoReparaciones_clicked(const QModelIndex &index)
{
    bool ok;
    ReporteOk = false;
    IndexTrabajo = index.row();
    TrabajoID = ui->TrabajoReparaciones->item(index.row(),0)->text().toInt(&ok,10);
    if(ui->TrabajoReparaciones->item(index.row(),5)->text().isEmpty())
        ReporteOk = true;

    dbTrabajo.CargarIngreso(*ui->TrabajoIngreso,TrabajoID);
    dbTrabajo.ActualizarCaudalimetro(*ui->TrabajoCaudalimetro,TrabajoID);
    ui->TrabRepID->setText(QString::number(TrabajoID,10));
    TrabajosActualizar();

}

void trabajo::CargarRecepcion()
{
    QString Conf;
     Conf.append("SELECT * FROM Agente");

     QSqlQuery consultar;

     if(!consultar.prepare(Conf))
     {
         QMessageBox::critical(this,tr("Tabla Agente"),
                               tr("Falla al crear la tabla\n"
                              "%1").arg(consultar.lastError().text()));
     }
     consultar.exec();
     int fila  = 0;
     QStringList Lista1 ;

     Lista1.clear();
     Lista1.append("Seleccionar");
     ui->TrabajoAgenteTabla->clear();
     ui->TrabajoAgenteTabla->setHorizontalHeaderItem(0,new QTableWidgetItem("Agente"));


     while(consultar.next())
     {
         ui->TrabajoAgenteTabla->insertRow(fila);
         ui->TrabajoAgenteTabla->setRowHeight(fila,20);
         ui->TrabajoAgenteTabla->setItem(fila,0,new QTableWidgetItem (consultar.value(1).toByteArray().constData()));
         Lista1.append(consultar.value(1).toByteArray().constData());
         fila ++;
     }
     ui->TrabajoAgenteTabla->setSortingEnabled(true);
     ui->TrabajoAgenteTabla->sortByColumn(0,Qt::AscendingOrder);
     ui->TrabajoAgenteTabla->setColumnWidth(0,200);

     if(!consultar.prepare("SELECT * FROM Operario"))
     {
         QMessageBox::critical(this,tr("Tabla Operario"),
                               tr("Falla al crear la tabla\n"
                              "%1").arg(consultar.lastError().text()));
     }
     consultar.exec();

     fila  = 0;
     Lista1.clear();
     Lista1.append("Seleccionar");
     ui->TrabajoOperario->clear();

     while(consultar.next())
     {
         Lista1.append(consultar.value(1).toByteArray().constData());
         fila ++;
     }
     ui->TrabajoOperario->addItems(Lista1);
}

void trabajo::TrabajosActualizar()
{
    int fila  = 0;
    QSqlQuery consultar;
    ui->TrabajoPerifericos->setRowCount(0);
    if(!consultar.prepare("SELECT * FROM Monitores"))
    {
        QMessageBox::critical(this,tr("Tabla Monitores"),
                              tr("Falla al crear la tabla\n"
                             "%1").arg(consultar.lastError().text()));
    }
    consultar.exec();
    while(consultar.next())
    {
        if(TrabajoID == consultar.value("repid").toByteArray().toInt())
        {
            ui->TrabajoPerifericos->insertRow(fila);
            ui->TrabajoPerifericos->setRowHeight(fila,20);
            ui->TrabajoPerifericos->setItem(fila,0,new QTableWidgetItem (consultar.value(1).toByteArray().constData()));
            ui->TrabajoPerifericos->setItem(fila,1,new QTableWidgetItem (consultar.value(2).toByteArray().constData()));
            ui->TrabajoPerifericos->setItem(fila,2,new QTableWidgetItem (" "));
            ui->TrabajoPerifericos->setItem(fila,3,new QTableWidgetItem (" "));
            ui->TrabajoPerifericos->setItem(fila,4,new QTableWidgetItem (consultar.value(3).toByteArray().constData()));
            ui->TrabajoPerifericos->setItem(fila,5,new QTableWidgetItem (" "));
            ui->TrabajoPerifericos->setItem(fila,6,new QTableWidgetItem (" "));
            ui->TrabajoPerifericos->setItem(fila,7,new QTableWidgetItem (consultar.value(4).toByteArray().constData()));
            ui->TrabajoPerifericos->setItem(fila,8,new QTableWidgetItem (consultar.value(5).toByteArray().constData()));
            ui->TrabajoPerifericos->setItem(fila,9,new QTableWidgetItem (consultar.value(6).toByteArray().constData()));
            ui->TrabajoPerifericos->setItem(fila,10,new QTableWidgetItem (consultar.value(7).toByteArray().constData()));
            ui->TrabajoPerifericos->setItem(fila,11,new QTableWidgetItem (consultar.value(8).toByteArray().constData()));
            fila ++;
        }
    }
    if(!consultar.prepare("SELECT * FROM Perifericos"))
    {
        QMessageBox::critical(this,tr("Tabla Periferico"),
                              tr("Falla al crear la tabla\n"
                             "%1").arg(consultar.lastError().text()));
    }
    consultar.exec();
    while(consultar.next())
    {
        if(TrabajoID == consultar.value("repid").toByteArray().toInt())
        {
            ui->TrabajoPerifericos->insertRow(fila);
            ui->TrabajoPerifericos->setRowHeight(fila,20);
            ui->TrabajoPerifericos->setItem(fila,0,new QTableWidgetItem (consultar.value(1).toByteArray().constData()));
            ui->TrabajoPerifericos->setItem(fila,1,new QTableWidgetItem (consultar.value(2).toByteArray().constData()));
            ui->TrabajoPerifericos->setItem(fila,2,new QTableWidgetItem (consultar.value(3).toByteArray().constData()));
            ui->TrabajoPerifericos->setItem(fila,3,new QTableWidgetItem (consultar.value(4).toByteArray().constData()));
            ui->TrabajoPerifericos->setItem(fila,4,new QTableWidgetItem (consultar.value(5).toByteArray().constData()));
            ui->TrabajoPerifericos->setItem(fila,5,new QTableWidgetItem (consultar.value(6).toByteArray().constData()));
            ui->TrabajoPerifericos->setItem(fila,6,new QTableWidgetItem (consultar.value(7).toByteArray().constData()));
            ui->TrabajoPerifericos->setItem(fila,7,new QTableWidgetItem (consultar.value(8).toByteArray().constData()));
            ui->TrabajoPerifericos->setItem(fila,8,new QTableWidgetItem (consultar.value(9).toByteArray().constData()));
            ui->TrabajoPerifericos->setItem(fila,9,new QTableWidgetItem (consultar.value(10).toByteArray().constData()));
            ui->TrabajoPerifericos->setItem(fila,10,new QTableWidgetItem (consultar.value(11).toByteArray().constData()));
            ui->TrabajoPerifericos->setItem(fila,11,new QTableWidgetItem (consultar.value(12).toByteArray().constData()));
            fila ++;

        }
    }
    if(!consultar.prepare("SELECT * FROM Instalaciones"))
    {
        QMessageBox::critical(this,tr("Tabla Instalaciones"),
                              tr("Falla al crear la tabla\n"
                             "%1").arg(consultar.lastError().text()));
    }
    consultar.exec();
    while(consultar.next())
    {
        if(TrabajoID == consultar.value("repid").toByteArray().toInt())
        {
            ui->TrabajoPerifericos->insertRow(fila);
            ui->TrabajoPerifericos->setRowHeight(fila,20);
            ui->TrabajoPerifericos->setItem(fila,0,new QTableWidgetItem (consultar.value(1).toByteArray().constData()));
            ui->TrabajoPerifericos->setItem(fila,1,new QTableWidgetItem (consultar.value(2).toByteArray().constData()));
            ui->TrabajoPerifericos->setItem(fila,2,new QTableWidgetItem (" "));
            ui->TrabajoPerifericos->setItem(fila,3,new QTableWidgetItem (" "));
            ui->TrabajoPerifericos->setItem(fila,4,new QTableWidgetItem (" "));
            ui->TrabajoPerifericos->setItem(fila,5,new QTableWidgetItem (" "));
            ui->TrabajoPerifericos->setItem(fila,6,new QTableWidgetItem (" "));
            ui->TrabajoPerifericos->setItem(fila,7,new QTableWidgetItem (consultar.value(3).toByteArray().constData()));
            ui->TrabajoPerifericos->setItem(fila,8,new QTableWidgetItem (consultar.value(4).toByteArray().constData()));
            ui->TrabajoPerifericos->setItem(fila,9,new QTableWidgetItem (consultar.value(5).toByteArray().constData()));
            ui->TrabajoPerifericos->setItem(fila,10,new QTableWidgetItem (consultar.value(6).toByteArray().constData()));
            ui->TrabajoPerifericos->setItem(fila,11,new QTableWidgetItem (consultar.value(7).toByteArray().constData()));
            fila ++;
        }
    }
//    ui->TrabajoPerifericos->setColumnWidth(0,50);
    ui->TrabajoPerifericos->setColumnWidth(0,100);
    ui->TrabajoPerifericos->setColumnWidth(1,80);
    ui->TrabajoPerifericos->setColumnWidth(2,70);
    ui->TrabajoPerifericos->setColumnWidth(3,70);
    ui->TrabajoPerifericos->setColumnWidth(4,40);
    ui->TrabajoPerifericos->setColumnWidth(5,70);
    ui->TrabajoPerifericos->setColumnWidth(6,100);
    ui->TrabajoPerifericos->setColumnWidth(7,100);
    ui->TrabajoPerifericos->setColumnWidth(8,40);
    ui->TrabajoPerifericos->setColumnWidth(9,100);
    ui->TrabajoPerifericos->setColumnWidth(10,70);
    ui->TrabajoPerifericos->setColumnWidth(11,50);

}

void trabajo::on_RepInterno_2_clicked()
{
    QPrinter printer;
    QString NArchivo;
    QString Texto;
    int i,a, Filas,Linea;
#define klinea 20;

    Linea = 10;
    NArchivo.clear();
    NArchivo.append(ui->TrabajoReparaciones->item(IndexTrabajo,3)->text()+".pdf");
    NArchivo.replace(2,1,"_");
    NArchivo.replace(5,1,"_");
    NArchivo.prepend("_");
    NArchivo.prepend(ui->TrabajoReparaciones->item(IndexTrabajo,1)->text());
    NArchivo.prepend("/");

    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(NArchivo);
    printer.setPageOrientation(QPageLayout::Landscape);
    QPainter painter;
//    QFont Font;
//    Font.setPointSize(18);
//    Font.setStyle("time");
    if (! painter.begin(&printer)) { // failed to open file
        qWarning("failed to open file, is it writable?");
        return ;
    }
    painter.setPen(Qt::blue);
    painter.setPen(QFont::Bold);
    painter.setPen(QFont::Times);
 //   painter.setPen(QFont::setPointSize(18));
    painter.drawText( 60,Linea, "Informe de Reparaciones");
    painter.setPen(QFont::Normal);
    painter.setPen(QFont::Courier);
 //   painter.setPen();
//    painter.setFont(QFont::Normal);
    Linea +=klinea;
    Texto.clear();
    Texto.append("Agente:" + ui->TrabajoReparaciones->item(IndexTrabajo,1)->text()
                  + "    Fecha Ingreso:" + ui->TrabajoReparaciones->item(IndexTrabajo,2)->text());
    painter.drawText(10,Linea,Texto);
    Linea +=klinea;
    Texto.clear();
    Texto.append("Operario:" + ui->TrabajoReparaciones->item(IndexTrabajo,4)->text()
                 + "   Fecha Control:" + ui->TrabajoReparaciones->item(IndexTrabajo,3)->text());

    painter.drawText(10,Linea,Texto);
    Linea +=klinea;
    Filas = ui->TrabajoIngreso->rowCount();
    painter.setPen(Qt::black);
    for(i=0;i<Filas;i++)
    {
       Texto.clear();
       Texto.append(  ui->TrabajoIngreso->item(i,1)->text()+ ":"
                    + ui->TrabajoIngreso->item(i,2)->text()+ " "
                    + ui->TrabajoIngreso->item(i,3)->text());
       painter.drawText(10,Linea,Texto);
       Linea +=klinea;
    }
    painter.setPen(Qt::blue);
    if(ui->TrabajoCaudalimetro->rowCount())
    {
        Texto.clear();
        Texto.append("Nombre SN;Movil F Fab F Inst V Soft F Soft TMT CPT DESC DESC AT BLOC MAG T.BLOC MAG Falla Bonif Obs F Rep");
        painter.drawText(10,Linea,Texto);
        Linea +=klinea;
        painter.setPen(Qt::black);
        Filas = ui->TrabajoCaudalimetro->rowCount();
        for(i=0;i<Filas;i++)
        {
            Texto.clear();
            for(a=1;a<=17;a++)
            {
                Texto.append(ui->TrabajoCaudalimetro->item(i,a)->text() + " ");
            }
            painter.drawText(10,Linea,Texto);
            Linea +=klinea;
        }
    }
    if(ui->TrabajoPerifericos->rowCount())
    {
        painter.setPen(Qt::blue);
        Texto.clear();
        Texto.append("Nombre SN F Fab F Inst V Soft F Soft Configuración Falla Bonif Obs F Rep");
        painter.drawText(10,Linea,Texto);
        Linea +=klinea;
        painter.setPen(Qt::black);
        Filas = ui->TrabajoPerifericos->rowCount();
        for(i=0;i<Filas;i++)
        {
            Texto.clear();
            for(a=0;a<=10;a++)
            {
                Texto.append(ui->TrabajoPerifericos->item(i,a)->text() + " ");
            }
            painter.drawText(10,Linea,Texto);
            Linea +=klinea;
        }
    }

    if (! printer.newPage()) {
        qWarning("failed in flushing page to disk, disk full?");
        return ;
    }
    painter.end();
}

void trabajo::FPresupuesto()
{

    QString Conf;
    Conf.append("UPDATE Reparaciones SET "
                "pres ="
                "'"+ui->TrabFechaRep->text()+"'"
                " WHERE id ="
                ""+QString::number(TrabajoID,10)+""
                "");
    QSqlQuery editar;

    if(!editar.prepare(Conf))
    {
        QMessageBox::critical(this,tr("Tabla Reparaciones"),
                              tr("Falla edicion de datos"));
    }
    editar.exec();

    int fila;
    QString AgenteText;
    fila = ui->TrabajoReparaciones->currentIndex().row();
    AgenteText.clear();
    AgenteText.append(ui->TrabajoReparaciones->item(fila,0)->text());
    if(AgenteText.isEmpty())


    dbTrabajo.CargarReparaciones(*ui->TrabajoReparaciones,AgenteText);

}
void trabajo::on_ReparacionesMostrar_clicked()
{
    dbTrabajo.CargarReparaciones(*ui->TrabajoReparaciones,"*");
}

void trabajo::on_TrabajoAgenteTabla_clicked(const QModelIndex &index)
{
    QString AgenteTexto;
    IndexAgente = index.row();
    AgenteTexto.clear();
    AgenteTexto.append(ui->TrabajoAgenteTabla->item(index.row(),0)->text());
    dbTrabajo.CargarReparaciones(*ui->TrabajoReparaciones,AgenteTexto);
}


void trabajo::on_buttonBox_accepted()
{
    on_ReparacionesIniciar_clicked();
//    if(ui->TrabRepID->text().isEmpty())
//    {
//        QMessageBox::critical(this,tr("Trabajo"),
//                                  tr("Seleccionar Trabajo para cargar datos"));
//        return;
//    }

//    if(ui->TrabajoReparaciones->item(IndexTrabajo,3)->text().isEmpty())
//    {
//        if(!ui->TrabajoOperario->currentIndex())
//        {
//            QMessageBox::critical(this,tr("Operario"),
//                                  tr("Seleccionar Operario"));
//            return;
//        }
//        QString Conf;
//        Conf.append("UPDATE Reparaciones SET "
//                    "frep ="
//                    "'"+ui->TrabFechaRep->text()+"',"
//                    "operario ="
//                    "'"+ui->TrabajoOperario->currentText()+"'"
//                    " WHERE id ="
//                    ""+ui->TrabRepID->text()+""
//                    "");
//        QSqlQuery editar;
//        if(!editar.prepare(Conf))
//        {
//            QMessageBox::critical(this,tr("Tabla Reparaciones"),
//                                  tr("Falla al crear la tabla\n"
//                                 "%1").arg(editar.lastError().text()));
//        }
//        editar.exec();

//    }
//    IdReparacion = TrabajoID;
//    int fila;
//    QString AgenteText;
//    fila = ui->TrabajoAgenteTabla->currentIndex().row();
//    AgenteText.clear();
//    AgenteText.append(ui->TrabajoAgenteTabla->item(fila,0)->text());

//    dbTrabajo.CargarReparaciones(*ui->TrabajoReparaciones,AgenteText);

}
