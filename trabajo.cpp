#include "trabajo.h"
#include "ui_trabajo.h"
#include <QDateTime>
#include <QMessageBox>
#include <QDebug>
#include <QFile>
#include <QtWidgets>

QDateTime dReparacion;

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
    TrabajoActualizar(arg1);
}
void trabajo::TrabajoActualizar(const QString &arg1)
{
    QString Conf;
    bool todos;
    if((arg1 == "*")|| (arg1 == "Seleccionar"))
    {
        todos = true;
    }
    else
    {
        todos = false;
    }
    Conf.append("SELECT * FROM Reparaciones");

    QSqlQuery consultar;
    consultar.prepare(Conf);
    if(!consultar.exec())
    {
        qDebug() << "error:" << consultar.lastError();
    }
    else
    {
        qDebug() << "Se ejecuto bien";

    }
    int fila  = 0;

    ui->TrabajoReparaciones->setRowCount(0);
    while(consultar.next())
    {
        if((arg1 == consultar.value(1).toByteArray().constData())|| todos )
        {
            ui->TrabajoReparaciones->insertRow(fila);
            ui->TrabajoReparaciones->setRowHeight(fila,20);
            ui->TrabajoReparaciones->setItem(fila,0,new QTableWidgetItem (consultar.value(0).toByteArray().constData()));
            ui->TrabajoReparaciones->setItem(fila,1,new QTableWidgetItem (consultar.value(1).toByteArray().constData()));
            ui->TrabajoReparaciones->setItem(fila,2,new QTableWidgetItem (consultar.value(2).toByteArray().constData()));
            ui->TrabajoReparaciones->setItem(fila,3,new QTableWidgetItem (consultar.value(3).toByteArray().constData()));
            ui->TrabajoReparaciones->setItem(fila,4,new QTableWidgetItem (consultar.value(4).toByteArray().constData()));
            fila ++;
        }
    }
    ui->TrabajoReparaciones->setColumnWidth(0,25);
    ui->TrabajoReparaciones->setColumnWidth(1,100);
    ui->TrabajoReparaciones->setColumnWidth(2,80);
    ui->TrabajoReparaciones->setColumnWidth(3,80);
    ui->TrabajoReparaciones->setColumnWidth(4,100);
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
        else
        {
            qDebug() << "Se Edito el item " << IndexTrabajo;
 //           ReparacionesActualizar(ui->AgenteNombre->currentText());
        }
    }
//## ver esto
//    ui->MonRepID->setText(ui->TrabajoReparaciones->item(IndexTrabajo,0)->text());
//    ui->PerRepID->setText(ui->TrabajoReparaciones->item(IndexTrabajo,0)->text());
//    ui->InstRepID->setText(ui->TrabajoReparaciones->item(IndexTrabajo,0)->text());
    TrabajoActualizar(ui->TrabajoAgente->currentText());
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
    if(ui->CaudalimetroTrabDatos->rowCount())
    {
        DatosArchivo.append(" ");
        DatosArchivo.append("Nombre;SN;Movil;F Fab;F Inst;V Soft;F Soft;TMT;CPT;DESC;DESC AT;BLOC MAG;T.BLOC MAG;Falla;Bonif;Obs;F Rep");
        Filas = ui->CaudalimetroTrabDatos->rowCount();
        for(i=0;i<Filas;i++)
        {
            Dts.clear();
            for(a=1;a<=17;a++)
            {
                Dts.append(ui->CaudalimetroTrabDatos->item(i,a)->text() + ";");
            }
            DatosArchivo.append(Dts);
        }
    }
    if(ui->TrabajoDatos->rowCount())
    {
        DatosArchivo.append(" ");
        DatosArchivo.append("Nombre;SN;F Fab;F Inst;V Soft;F Soft;Configuración;Falla;Bonif;Obs;F Rep");
        Filas = ui->TrabajoDatos->rowCount();
        for(i=0;i<Filas;i++)
        {
            Dts.clear();
            for(a=0;a<=10;a++)
            {
                Dts.append(ui->TrabajoDatos->item(i,a)->text() + ";");
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
    if(file.open(QFile::WriteOnly)){
//##        file.write(ui->Datos->toPlainText().toLatin1());
        info = DatosArchivo.join(0x0d);
        file.write(info.toLatin1());

        setWindowTitle(tr("Analisis Equipos - %1[*]").arg(QFileInfo(curFile).fileName()));
//###        ui->Datos->document()->setModified(false);
        return true;
    }else{
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
    QString Conf;
    QString Carga;
    IndexTrabajo = index.row();
    TrabajoID = ui->TrabajoReparaciones->item(index.row(),0)->text().toInt();
    Conf.append("SELECT * FROM Ingreso");
    QSqlQuery consultar;
    consultar.prepare(Conf);
    if(!consultar.exec())
    {
        qDebug() << "error:" << consultar.lastError();
    }
    else
    {
        qDebug() << "Se ejecuto bien";

    }
    int fila  = 0;

    ui->TrabajoIngreso->setRowCount(0);

    while(consultar.next())
    {
        if(TrabajoID == consultar.value("repid").toByteArray().toInt())
        {
            ui->TrabajoIngreso->insertRow(fila);
            ui->TrabajoIngreso->setRowHeight(fila,20);
            ui->TrabajoIngreso->setItem(fila,0,new QTableWidgetItem (consultar.value(0).toByteArray().constData()));
            ui->TrabajoIngreso->setItem(fila,1,new QTableWidgetItem (consultar.value(1).toByteArray().constData()));
            ui->TrabajoIngreso->setItem(fila,2,new QTableWidgetItem (consultar.value(2).toByteArray().constData()));
            ui->TrabajoIngreso->setItem(fila,3,new QTableWidgetItem (consultar.value(3).toByteArray().constData()));
            ui->TrabajoIngreso->setItem(fila,4,new QTableWidgetItem (consultar.value(4).toByteArray().constData()));
            Carga.append(consultar.value(1).toByteArray().constData());
            Carga.append(": ");
            Carga.append(consultar.value(2).toByteArray().constData());
            Carga.append(0x0d);
//          ui->MONCant->setText(Carga);

            fila ++;
        }
    }
    ui->TrabRepID->setText(ui->TrabajoReparaciones->item(index.row(),0)->text());
    ui->TrabajoIngreso->setColumnWidth(0,25);
    ui->TrabajoIngreso->setColumnWidth(1,100);
    ui->TrabajoIngreso->setColumnWidth(2,50);
    ui->TrabajoIngreso->setColumnWidth(3,150);
    ui->TrabajoIngreso->setColumnWidth(4,40);
//    MonitoresActualizar();
//    PerifericosActualizar();
//    InstalacionesActualizar();
    TrabajosActualizar();
    CaudalimetroActualizar(*ui->CaudalimetroTrabDatos);
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
    ui->TrabajoDatos->setRowCount(0);
    consultar.prepare("SELECT * FROM Monitores");
    consultar.exec();
    while(consultar.next())
    {
        if(TrabajoID == consultar.value("repid").toByteArray().toInt())
        {
            ui->TrabajoDatos->insertRow(fila);
            ui->TrabajoDatos->setRowHeight(fila,20);
            ui->TrabajoDatos->setItem(fila,0,new QTableWidgetItem (consultar.value(1).toByteArray().constData()));
            ui->TrabajoDatos->setItem(fila,1,new QTableWidgetItem (consultar.value(2).toByteArray().constData()));
            ui->TrabajoDatos->setItem(fila,2,new QTableWidgetItem (" "));
            ui->TrabajoDatos->setItem(fila,3,new QTableWidgetItem (" "));
            ui->TrabajoDatos->setItem(fila,4,new QTableWidgetItem (consultar.value(3).toByteArray().constData()));
            ui->TrabajoDatos->setItem(fila,5,new QTableWidgetItem (" "));
            ui->TrabajoDatos->setItem(fila,6,new QTableWidgetItem (" "));
            ui->TrabajoDatos->setItem(fila,7,new QTableWidgetItem (consultar.value(4).toByteArray().constData()));
            ui->TrabajoDatos->setItem(fila,8,new QTableWidgetItem (consultar.value(5).toByteArray().constData()));
            ui->TrabajoDatos->setItem(fila,9,new QTableWidgetItem (consultar.value(6).toByteArray().constData()));
            ui->TrabajoDatos->setItem(fila,10,new QTableWidgetItem (consultar.value(7).toByteArray().constData()));
            ui->TrabajoDatos->setItem(fila,11,new QTableWidgetItem (consultar.value(8).toByteArray().constData()));
            fila ++;
        }
    }
    consultar.prepare("SELECT * FROM Perifericos");
    consultar.exec();
    while(consultar.next())
    {
        if(TrabajoID == consultar.value("repid").toByteArray().toInt())
        {
            ui->TrabajoDatos->insertRow(fila);
            ui->TrabajoDatos->setRowHeight(fila,20);
            ui->TrabajoDatos->setItem(fila,0,new QTableWidgetItem (consultar.value(1).toByteArray().constData()));
            ui->TrabajoDatos->setItem(fila,1,new QTableWidgetItem (consultar.value(2).toByteArray().constData()));
            ui->TrabajoDatos->setItem(fila,2,new QTableWidgetItem (consultar.value(3).toByteArray().constData()));
            ui->TrabajoDatos->setItem(fila,3,new QTableWidgetItem (consultar.value(4).toByteArray().constData()));
            ui->TrabajoDatos->setItem(fila,4,new QTableWidgetItem (consultar.value(5).toByteArray().constData()));
            ui->TrabajoDatos->setItem(fila,5,new QTableWidgetItem (consultar.value(6).toByteArray().constData()));
            ui->TrabajoDatos->setItem(fila,6,new QTableWidgetItem (consultar.value(7).toByteArray().constData()));
            ui->TrabajoDatos->setItem(fila,7,new QTableWidgetItem (consultar.value(8).toByteArray().constData()));
            ui->TrabajoDatos->setItem(fila,8,new QTableWidgetItem (consultar.value(9).toByteArray().constData()));
            ui->TrabajoDatos->setItem(fila,9,new QTableWidgetItem (consultar.value(10).toByteArray().constData()));
            ui->TrabajoDatos->setItem(fila,10,new QTableWidgetItem (consultar.value(11).toByteArray().constData()));
            ui->TrabajoDatos->setItem(fila,11,new QTableWidgetItem (consultar.value(12).toByteArray().constData()));
            fila ++;

        }
    }
    consultar.prepare("SELECT * FROM Instalaciones");
    consultar.exec();
    while(consultar.next())
    {
        if(TrabajoID == consultar.value("repid").toByteArray().toInt())
        {
            ui->TrabajoDatos->insertRow(fila);
            ui->TrabajoDatos->setRowHeight(fila,20);
            ui->TrabajoDatos->setItem(fila,0,new QTableWidgetItem (consultar.value(1).toByteArray().constData()));
            ui->TrabajoDatos->setItem(fila,1,new QTableWidgetItem (consultar.value(2).toByteArray().constData()));
            ui->TrabajoDatos->setItem(fila,2,new QTableWidgetItem (" "));
            ui->TrabajoDatos->setItem(fila,3,new QTableWidgetItem (" "));
            ui->TrabajoDatos->setItem(fila,4,new QTableWidgetItem (" "));
            ui->TrabajoDatos->setItem(fila,5,new QTableWidgetItem (" "));
            ui->TrabajoDatos->setItem(fila,6,new QTableWidgetItem (" "));
            ui->TrabajoDatos->setItem(fila,7,new QTableWidgetItem (consultar.value(3).toByteArray().constData()));
            ui->TrabajoDatos->setItem(fila,8,new QTableWidgetItem (consultar.value(4).toByteArray().constData()));
            ui->TrabajoDatos->setItem(fila,9,new QTableWidgetItem (consultar.value(5).toByteArray().constData()));
            ui->TrabajoDatos->setItem(fila,10,new QTableWidgetItem (consultar.value(6).toByteArray().constData()));
            ui->TrabajoDatos->setItem(fila,11,new QTableWidgetItem (consultar.value(7).toByteArray().constData()));
            fila ++;
        }
    }
//    ui->TrabajoDatos->setColumnWidth(0,50);
    ui->TrabajoDatos->setColumnWidth(0,100);
    ui->TrabajoDatos->setColumnWidth(1,80);
    ui->TrabajoDatos->setColumnWidth(2,70);
    ui->TrabajoDatos->setColumnWidth(3,70);
    ui->TrabajoDatos->setColumnWidth(4,40);
    ui->TrabajoDatos->setColumnWidth(5,70);
    ui->TrabajoDatos->setColumnWidth(6,100);
    ui->TrabajoDatos->setColumnWidth(7,100);
    ui->TrabajoDatos->setColumnWidth(8,40);
    ui->TrabajoDatos->setColumnWidth(9,100);
    ui->TrabajoDatos->setColumnWidth(10,70);
    ui->TrabajoDatos->setColumnWidth(11,50);

}
void trabajo::CaudalimetroActualizar(QTableWidget &SCC)
{
    int fila  = 0;
    QSqlQuery consultar;
    SCC.setRowCount(0);
    consultar.prepare("SELECT * FROM Caudalimetro");
    consultar.exec();
    while(consultar.next())
    {
        if(TrabajoID == consultar.value("repid").toByteArray().toInt())
        {
            SCC.insertRow(fila);
            SCC.setRowHeight(fila,20);
            SCC.setItem(fila,0,new QTableWidgetItem (consultar.value(0).toByteArray().constData()));
            SCC.setItem(fila,1,new QTableWidgetItem (consultar.value(1).toByteArray().constData()));
            SCC.setItem(fila,2,new QTableWidgetItem (consultar.value(2).toByteArray().constData()));
            SCC.setItem(fila,3,new QTableWidgetItem (consultar.value(3).toByteArray().constData()));
            SCC.setItem(fila,4,new QTableWidgetItem (consultar.value(4).toByteArray().constData()));
            SCC.setItem(fila,5,new QTableWidgetItem (consultar.value(5).toByteArray().constData()));
            SCC.setItem(fila,6,new QTableWidgetItem (consultar.value(6).toByteArray().constData()));
            SCC.setItem(fila,7,new QTableWidgetItem (consultar.value(7).toByteArray().constData()));
            SCC.setItem(fila,8,new QTableWidgetItem (consultar.value(8).toByteArray().constData()));
            SCC.setItem(fila,9,new QTableWidgetItem (consultar.value(9).toByteArray().constData()));
            SCC.setItem(fila,10,new QTableWidgetItem (consultar.value(10).toByteArray().constData()));
            SCC.setItem(fila,11,new QTableWidgetItem (consultar.value(11).toByteArray().constData()));
            SCC.setItem(fila,12,new QTableWidgetItem (consultar.value(12).toByteArray().constData()));
            SCC.setItem(fila,13,new QTableWidgetItem (consultar.value(13).toByteArray().constData()));
            SCC.setItem(fila,14,new QTableWidgetItem (consultar.value(14).toByteArray().constData()));
            SCC.setItem(fila,15,new QTableWidgetItem (consultar.value(15).toByteArray().constData()));
            SCC.setItem(fila,16,new QTableWidgetItem (consultar.value(16).toByteArray().constData()));
            SCC.setItem(fila,17,new QTableWidgetItem (consultar.value(17).toByteArray().constData()));
            SCC.setItem(fila,18,new QTableWidgetItem (consultar.value(18).toByteArray().constData()));
            fila ++;
        }
    }

    SCC.setColumnWidth(0,50);
    SCC.setColumnWidth(1,60);
    SCC.setColumnWidth(2,50);
    SCC.setColumnWidth(3,70);
    SCC.setColumnWidth(4,70);
    SCC.setColumnWidth(5,70);
    SCC.setColumnWidth(6,50);
    SCC.setColumnWidth(7,70);
    SCC.setColumnWidth(8,80);
    SCC.setColumnWidth(9,80);
    SCC.setColumnWidth(10,50);
    SCC.setColumnWidth(11,50);
    SCC.setColumnWidth(12,80);
    SCC.setColumnWidth(13,50);
    SCC.setColumnWidth(14,100);
    SCC.setColumnWidth(15,80);
    SCC.setColumnWidth(16,100);
    SCC.setColumnWidth(17,80);
    SCC.setColumnWidth(18,50);

}
