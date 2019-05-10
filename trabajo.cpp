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
#include <reporteinterno.h>

#include <QTextCharFormat>
#include <dbmanejo.h>
#include <QDir>

QDateTime dReparacion;
dbManejo dbTrabajo;
QDir dir;
QPainter painter;

trabajo::trabajo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::trabajo)
{
    ui->setupUi(this);
    ui->TrabFechaRep->setInputMask("00/00/0000");
    ui->TrabFechaRep->setText(dReparacion.currentDateTime().toString("ddMMyyyy"));
    CargarRecepcion();

    TrabAgentes = new QSqlRelationalTableModel(this,dbManejo::dbRetorna());
    TrabAgentes->setTable("Agente");
    TrabAgentes->select();

    FiltTrAgentes = new QSortFilterProxyModel(this);
    FiltTrAgentes->setSourceModel(TrabAgentes);
    FiltTrAgentes->setFilterCaseSensitivity(Qt::CaseInsensitive);
    FiltTrAgentes->setFilterKeyColumn(-1); //-1 ordena por todas la columnas

    ui->AgentesTablaTrab->setModel(FiltTrAgentes);
    ui->AgentesTablaTrab->hideColumn(0);
    ui->AgentesTablaTrab->sortByColumn(1,Qt::AscendingOrder);
    ui->AgentesTablaTrab->setSortingEnabled(true);
    ui->AgentesTablaTrab->setColumnWidth(1,250);


    TrabRep = new QSqlRelationalTableModel(this,dbManejo::dbRetorna());
    TrabRep->setTable("Reparaciones");
    TrabRep->select();

    FilTrRep = new QSortFilterProxyModel(this);
    FilTrRep->setSourceModel(TrabRep);
    FilTrRep->setFilterCaseSensitivity(Qt::CaseInsensitive);
    FilTrRep->setFilterKeyColumn(-1); //-1 ordena por todas la columnas


    FilTrRep->setFilterFixedString("");

    ui->RepTablaTrab->setModel(FilTrRep);
    ui->RepTablaTrab->sortByColumn(0,Qt::AscendingOrder);
    ui->RepTablaTrab->setSortingEnabled(true);
    ui->RepTablaTrab->setColumnWidth(0,60);
    ui->RepTablaTrab->setColumnWidth(1,150);
    ui->RepTablaTrab->setColumnWidth(2,80);
    ui->RepTablaTrab->setColumnWidth(3,80);
    ui->RepTablaTrab->setColumnWidth(5,80);
    ui->RepTablaTrab->scrollToBottom();

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
    QString Aux;
    Aux.clear();
    Aux.append( ui->RepTablaTrab->model()->data(ui->RepTablaTrab->model()->index(IndexTrabajo,3)).toString());

//    qDebug () << Aux;

    if(Aux.isEmpty())
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
//        qDebug () << Conf;
        QSqlQuery editar;
        editar.prepare(Conf);
        if(!editar.exec())
        {
            QMessageBox::critical(this,tr("Tabla Reparaciones"),
                                  tr("Falla al Actualizar la tabla\n"
                                 "%1").arg(editar.lastError().text()));
        }


    }
    IdReparacion = TrabajoID;
//    qDebug () << IdReparacion << TrabajoID;


    int fila;
    QString AgenteText;
    fila = ui->AgentesTablaTrab->currentIndex().row();
    if (fila<0)
    {
       FilTrRep->setFilterFixedString("");
       ui->RepTablaTrab->scrollToBottom();
    }
    else
    {
        AgenteText.clear();
        AgenteText.append(ui->AgentesTablaTrab->model()->data(ui->AgentesTablaTrab->model()->index(fila,1)).toString());
        FilTrRep->setFilterFixedString(AgenteText);
        ui->RepTablaTrab->scrollToBottom();
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

    fila = ui->RepTablaTrab->currentIndex().row();
    if(fila<0)
    {
        QMessageBox::critical(this,tr("Selección Trabajo"),
                              tr("Seleccionar trabajo para generar el informe"));
        return;

    }

    AgenteText.clear();
    AgenteText.append(ui->RepTablaTrab->model()->data(ui->RepTablaTrab->model()->index(fila,1)).toString());

//    qDebug () << AgenteText;
    if(AgenteText.isEmpty())
    {
        QMessageBox::critical(this,tr("Selección Trabajo"),
                              tr("Seleccionar trabajo para generar el informe"));
        return;

    }
    QString Aux;
    Aux.clear();
    Aux.append( ui->RepTablaTrab->model()->data(ui->RepTablaTrab->model()->index(IndexTrabajo,3)).toString());
//    qDebug () << Aux;
    if(Aux.isEmpty())
    {
        QMessageBox::information(this,tr("Trabajo"),
                              tr("Iniciar reparacion para generar el reporte"));
        return;
    }
    NArchivo.clear();
    NArchivo.append(Aux);
    NArchivo.replace(2,1,".");
    NArchivo.replace(5,1,".");
    NArchivo.prepend("_");
    NArchivo.prepend(ui->RepTablaTrab->model()->data(ui->RepTablaTrab->model()->index(IndexTrabajo,1)).toString());
    NArchivo.prepend(RutaInfoExcel);
    QDir Dir;
    Dir.setCurrent(Dir.homePath());

    DatosArchivo.append(";;;;;Informe de Reparaciones");
    DatosArchivo.append("Agente:;" + ui->RepTablaTrab->model()->data(ui->RepTablaTrab->model()->index(IndexTrabajo,1)).toString()
                         + ";;;Fecha Ingreso :;;" + ui->RepTablaTrab->model()->data(ui->RepTablaTrab->model()->index(IndexTrabajo,2)).toString());

    DatosArchivo.append("Operario:;" + ui->RepTablaTrab->model()->data(ui->RepTablaTrab->model()->index(IndexTrabajo,4)).toString()
                        + ";;;Fecha Control:;;" + ui->RepTablaTrab->model()->data(ui->RepTablaTrab->model()->index(IndexTrabajo,3)).toString());
    DatosArchivo.append(" ");
    DatosArchivo.append("Nombre;Descripcion;;;;;N Serie;Cant;Fact;Comentario");
    Filas = ui->TrabajoIngreso->rowCount();
    for(i=0;i<Filas;i++)
    {
       DatosArchivo.append(  ui->TrabajoIngreso->item(i,1)->text()+ ";"
                    + ui->TrabajoIngreso->item(i,2)->text()+ ";;;;;"
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


//    qDebug () << "FileName: " << fileName;
    int aa = fileName.lastIndexOf("/");
//    qDebug () << aa;
    QString direccion = fileName.mid(0,aa);
//    qDebug ()<< direccion;

    if(!fileName.isEmpty()){
        //curFile = fileName;
        if(saveFile(fileName) && ReporteOk) //Se genero el reporte, se activa la fecha del reporte
        {
            FPresupuesto();
        }
//        qDebug () << "3" << dir.currentPath();
    }
}
bool trabajo::saveFile(const QString &fileName)
{
    QString info;
//    qDebug () << fileName;
    QFile file(fileName);
//    qDebug () << "1" << dir.currentPath();
    if(file.open(QFile::WriteOnly))
    {
        info = DatosArchivo.join(0x0d);
        file.write(info.toLatin1());
        setWindowTitle(tr("Analisis Equipos - %1[*]").arg(QFileInfo(curFile).fileName()));
//        qDebug () << "2" << dir.currentPath();
//        qDebug () << fileName;
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

void trabajo::CargarRecepcion()
{
    int fila  = 0;
    QStringList Lista1 ;
    QSqlQuery consultar;

    consultar.prepare("SELECT * FROM Operario");
    if(!consultar.exec())
    {
        QMessageBox::critical(this,tr("Tabla Operario"),
                              tr("Falla lectura de la tabla\n"
                                 "%1").arg(consultar.lastError().text()));
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
    QSqlQuery Auxiliar;
    QString Comando, Grupo;

    ui->TrabajoPerifericos->setRowCount(0);
    consultar.prepare("SELECT * FROM Monitores WHERE repid == "+ QString::number(TrabajoID,10));
    if(!consultar.exec())
    {
        QMessageBox::critical(this,tr("Tabla Monitores"),
                              tr("Falla al crear la tabla\n"
                             "%1").arg(consultar.lastError().text()));
    }

    while(consultar.next())
    {
        ui->TrabajoPerifericos->insertRow(fila);
        ui->TrabajoPerifericos->setRowHeight(fila,20);
        ui->TrabajoPerifericos->setItem(fila,0,new QTableWidgetItem (consultar.value(1).toByteArray().constData()));
        ui->TrabajoPerifericos->setItem(fila,1,new QTableWidgetItem (consultar.value(2).toByteArray().constData()));
        ui->TrabajoPerifericos->setItem(fila,2,new QTableWidgetItem (" "));
        ui->TrabajoPerifericos->setItem(fila,3,new QTableWidgetItem (" "));
        ui->TrabajoPerifericos->setItem(fila,4,new QTableWidgetItem (consultar.value(3).toByteArray().constData()));
        ui->TrabajoPerifericos->setItem(fila,5,new QTableWidgetItem (consultar.value(4).toByteArray().constData()));
        ui->TrabajoPerifericos->setItem(fila,6,new QTableWidgetItem (" "));
        ui->TrabajoPerifericos->setItem(fila,7,new QTableWidgetItem (consultar.value(5).toByteArray().constData()));
        ui->TrabajoPerifericos->setItem(fila,8,new QTableWidgetItem (consultar.value(6).toByteArray().constData()));
        ui->TrabajoPerifericos->setItem(fila,9,new QTableWidgetItem (consultar.value(7).toByteArray().constData()));
        ui->TrabajoPerifericos->setItem(fila,10,new QTableWidgetItem (consultar.value(8).toByteArray().constData()));
        ui->TrabajoPerifericos->setItem(fila,11,new QTableWidgetItem (consultar.value(9).toByteArray().constData()));
        ui->TrabajoPerifericos->setItem(fila,12,new QTableWidgetItem (consultar.value(10).toByteArray().constData()));

        fila ++;
    }

    consultar.prepare("SELECT * FROM Perifericos WHERE repid == "+ QString::number(TrabajoID,10));
    consultar.exec();
    while(consultar.next())
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
        ui->TrabajoPerifericos->setItem(fila,12,new QTableWidgetItem (consultar.value(13).toByteArray().constData()));

        fila ++;
    }
    consultar.prepare("SELECT * FROM Instalaciones WHERE repid == "+ QString::number(TrabajoID,10));
    if(!consultar.exec())
    {
        QMessageBox::critical(this,tr("Tabla Instalaciones"),
                              tr("Falla al crear la tabla\n"
                             "%1").arg(consultar.lastError().text()));
    }

    while(consultar.next())
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
        ui->TrabajoPerifericos->setItem(fila,12,new QTableWidgetItem (consultar.value(8).toByteArray().constData()));

        fila ++;
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
//    ui->TrabajoPerifericos->setColumnWidth(13,100);

}

void trabajo::on_RepInterno_2_clicked()
{
    int fila;
    int RepId;
    QString NArchivo;
    QSqlQuery consulta;

    fila = ui->RepTablaTrab->currentIndex().row();
    if(fila<0)
    {
        QMessageBox::critical(this,tr("Selección Trabajo"),
                              tr("Seleccionar trabajo para generar el informe"));
        return;
    }
    RepId = ui->RepTablaTrab->model()->data(ui->RepTablaTrab->model()->index(fila,0)).toInt();
    consulta.prepare("SELECT * FROM Reparaciones WHERE id = " + QString::number(RepId,10));
    consulta.exec();
    consulta.next();

    NArchivo.append(consulta.value("frep").toString() +".pdf");
    NArchivo.replace(2,1,".");
    NArchivo.replace(5,1,".");
    NArchivo.prepend("_");
    NArchivo.prepend(consulta.value("agente").toString());
    NArchivo.prepend(RutaInfoPDF);


    QString fileName = QFileDialog::getSaveFileName(
                this,
                "Analisis de equipo - Guardado de archivos",
                NArchivo,
                "Text Files (*.pdf);;All Files (*.pdf)");

    if(fileName.isEmpty())
    {
        return;
    }
//qDebug () << fileName;
    ReporteInterno ReporteAgente;
    ReporteAgente.RepAgentePDF(RepId, fileName);
    QString Conf;
    Conf.append("UPDATE Reparaciones SET "
                "finf ="
                "'"+ui->TrabFechaRep->text()+"'"
                " WHERE id ="
                ""+QString::number(TrabajoID,10)+""
                "");

 //   qDebug () << Conf;
    QSqlQuery editar;
    editar.prepare(Conf);
    if(!editar.exec())
    {
        QMessageBox::critical(this,tr("Tabla Reparaciones"),
                              tr("Falla edicion de datos"));
    }
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

 //   qDebug () << Conf;
    QSqlQuery editar;
    editar.prepare(Conf);
    if(!editar.exec())
    {
        QMessageBox::critical(this,tr("Tabla Reparaciones"),
                              tr("Falla edicion de datos1"));
    }


    QString AgenteText;

    AgenteText.clear();
    AgenteText.append(ui->RepTablaTrab->model()->data(ui->RepTablaTrab->model()->index(IndexTrabajo,1)).toString());
    if(AgenteText.isEmpty())

    TrabRep->clear();
    TrabRep->submitAll();
    FilTrRep->setFilterFixedString(AgenteText);

}

void trabajo::on_ReparacionesMostrar_clicked()
{
    FilTrRep->setFilterFixedString("");
 //   FilTrRep->
    ui->RepTablaTrab->scrollToBottom();
    ui->RepTablaTrab->scrollToBottom();
}

void trabajo::on_buttonBox_accepted()
{
    on_ReparacionesIniciar_clicked();
}

void trabajo::on_AgenteBuscarTrab_textChanged(const QString &arg1)
{
    FiltTrAgentes->setFilterFixedString(arg1);
    FilTrRep->setFilterFixedString(arg1);

}

void trabajo::on_AgentesTablaTrab_clicked(const QModelIndex &index)
{

    QString AgenteTexto;
    AgenteTexto.clear();
    AgenteTexto.append(ui->AgentesTablaTrab->model()->data(index).toString());
    FilTrRep->setFilterFixedString(AgenteTexto);
}



void trabajo::on_RepTablaTrab_clicked(const QModelIndex &index)
{
    bool ok;
    ReporteOk = false;
    IndexTrabajo = index.row();
    QModelIndex Indice = ui->RepTablaTrab->model()->index(IndexTrabajo,0);
    TrabajoID = ui->RepTablaTrab->model()->data(Indice).toInt(&ok);

    Indice = ui->RepTablaTrab->model()->index(IndexTrabajo,5);
    if(ui->RepTablaTrab->model()->data(Indice).toString().isEmpty())
        ReporteOk = true;

    Indice = ui->RepTablaTrab->model()->index(IndexTrabajo,1);
    AgenteResp = ui->RepTablaTrab->model()->data(Indice).toString();


    QDateTime T1, T2;

//    qDebug() << "Inicio: "<< dReparacion.currentDateTime().toString("hh:mm:ss.z");
    dbTrabajo.CargarIngreso(*ui->TrabajoIngreso,TrabajoID);
//
//    qDebug() << "Fin Ingreso: "<< dReparacion.currentDateTime().toString("hh:mm:ss.z");
    dbTrabajo.ActualizarCaudalimetro(*ui->TrabajoCaudalimetro,TrabajoID);

 //   qDebug() << "Fin Caudal: "<< dReparacion.currentDateTime().toString("hh:mm:ss.z");
    ui->TrabRepID->setText(QString::number(TrabajoID,10));

//    qDebug() << "Fin Reparaciones: "<< dReparacion.currentDateTime().toString("hh:mm:ss.z");
    TrabajosActualizar();
//    qDebug() << "Fin Actualizar: "<< dReparacion.currentDateTime().toString("hh:mm:ss.z");
}



void trabajo::on_RepInterno_PDF_clicked()
{
    int fila;
    int RepId;
    QString NArchivo;
    QSqlQuery consulta;

    fila = ui->RepTablaTrab->currentIndex().row();
    if(fila<0)
    {
        QMessageBox::critical(this,tr("Selección Trabajo"),
                              tr("Seleccionar trabajo para generar el informe"));
        return;
    }
    RepId = ui->RepTablaTrab->model()->data(ui->RepTablaTrab->model()->index(fila,0)).toInt();
    consulta.prepare("SELECT * FROM Reparaciones WHERE id = " + QString::number(RepId,10));
    consulta.exec();
    consulta.next();

    NArchivo.append(consulta.value("frep").toString() +".pdf");
    NArchivo.replace(2,1,".");
    NArchivo.replace(5,1,".");
    NArchivo.prepend("_");
    NArchivo.prepend(consulta.value("agente").toString());
    NArchivo.prepend(RutaInfoExcel);


    QString fileName = QFileDialog::getSaveFileName(
                this,
                "Analisis de equipo - Guardado de archivos",
                NArchivo,
                "Text Files (*.pdf);;All Files (*.pdf)");

    if(fileName.isEmpty())
    {
        return;
    }

    ReporteInterno RepInt;
    RepInt.RepInternoPDF(RepId, NArchivo);
    FPresupuesto();
}


void trabajo::on_NotaPedido_clicked()
{
    int RepID;
    int fila;
    fila = ui->RepTablaTrab->currentIndex().row();
    if(fila<0)
    {
        QMessageBox::critical(this,tr("Selección Trabajo"),
                              tr("Seleccionar trabajo para generar el informe"));
        return;
    }
    RepID = ui->RepTablaTrab->model()->data(ui->RepTablaTrab->model()->index(fila,0)).toInt();
    NotaPedido *NP = new NotaPedido (this);
    NP->setModal(true);
    NP->show();
    NP->SetDatos(RepID);
}

void trabajo::on_Pendientes_clicked()
{
    FilTrRep->setFilterKeyColumn(4); //-1 ordena por todas la columnas
    FilTrRep->setFilterFixedString("");
}
