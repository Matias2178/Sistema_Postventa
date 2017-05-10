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
}

trabajo::~trabajo()
{
    delete ui;
}

void trabajo::on_TrabajoAgente_activated(const QString &arg1)
{
    dbTrabajo.CargarReparaciones(*ui->TrabajoReparaciones,arg1);
}
void trabajo::TrabajoActualizarAgente()
{
    QString Conf;
    Conf.append("SELECT * FROM Agente");

    QSqlQuery consultar;
    consultar.prepare(Conf);
    if(!consultar.exec())
    {
        qDebug() << "Falla leer Agente:" << consultar.lastError();
    }
    else
    {
        qDebug() << "Leer Agentes Ok";

    }
    int fila  = 0;
    QStringList Lista1 ;

    Lista1.clear();
    Lista1.append("Seleccionar");
    ui->TrabajoAgente->clear();

    while(consultar.next())
    {
        Lista1.append(consultar.value(1).toByteArray().constData());
        fila ++;
    }
    ui->TrabajoAgente->addItems(Lista1);

    consultar.prepare("SELECT * FROM Operario");
    if(!consultar.exec())
    {
        qDebug() << "Falla leer Operario:" << consultar.lastError();
    }
    else
    {
        qDebug() << "Leer Operario Ok";

    }
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
        editar.prepare(Conf);
        qDebug() << Conf;
        qDebug() << "error:" << IndexTrabajo << editar.lastError();
        if(!editar.exec())
        {
            qDebug() << "error:" << editar.lastError();
            QMessageBox::critical(this,tr("Error en un campo"),
                                 tr("Camos incompletos no se guardaron los datos"));
        }

    }
    IdReparacion = TrabajoID;

    dbTrabajo.CargarReparaciones(*ui->TrabajoReparaciones,ui->TrabajoAgente->currentText());

}

void trabajo::on_RepInterno_clicked()
{
    QString NArchivo;
    QString Dts;
    int Filas;
    int i, a;
    DatosArchivo.clear();
    if(!ui->TrabajoAgente->currentIndex())
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
    DatosArchivo.append("Nombre;;Cant");
    Filas = ui->TrabajoIngreso->rowCount();
    for(i=0;i<Filas;i++)
    {
       DatosArchivo.append(  ui->TrabajoIngreso->item(i,1)->text()+ ":;;"
                    + ui->TrabajoIngreso->item(i,2)->text()+ ";"
                    + ui->TrabajoIngreso->item(i,3)->text());
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
        saveFile(fileName);
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
    IndexTrabajo = index.row();
    TrabajoID = ui->TrabajoReparaciones->item(index.row(),0)->text().toInt(&ok,10);
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
     consultar.prepare(Conf);
     if(!consultar.exec())
     {
         qDebug() << "Falla leer Agente:" << consultar.lastError();
     }
     else
     {
         qDebug() << "Leer Agentes Ok";

     }
     int fila  = 0;
     QStringList Lista1 ;

     Lista1.clear();
     Lista1.append("Seleccionar");
     ui->TrabajoAgente->clear();


     while(consultar.next())
     {
         Lista1.append(consultar.value(1).toByteArray().constData());
         fila ++;
     }
     ui->TrabajoAgente->addItems(Lista1);

     consultar.prepare("SELECT * FROM Operario");
     if(!consultar.exec())
     {
         qDebug() << "Falla leer Operario:" << consultar.lastError();
     }
     else
     {
         qDebug() << "Leer Operario Ok";

     }
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
    consultar.prepare("SELECT * FROM Monitores");
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
    consultar.prepare("SELECT * FROM Perifericos");
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
    consultar.prepare("SELECT * FROM Instalaciones");
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
            qDebug () << "Caudalimetro"<<a;
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
