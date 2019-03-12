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
        qDebug () << Conf;
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
    qDebug () << IdReparacion << TrabajoID;


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
    int i;
    QSqlQuery consultar;
    QSqlQuery Auxiliar;
    QString Comando, Grupo;
    ui->TrabajoPerifericos->setRowCount(0);
    if(!consultar.prepare("SELECT * FROM Monitores WHERE repid == "+ QString::number(TrabajoID,10)))
    {
        QMessageBox::critical(this,tr("Tabla Monitores"),
                              tr("Falla al crear la tabla\n"
                             "%1").arg(consultar.lastError().text()));
    }
    consultar.exec();
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

    if(!consultar.prepare("SELECT * FROM Instalaciones WHERE repid == "+ QString::number(TrabajoID,10)))
    {
        QMessageBox::critical(this,tr("Tabla Instalaciones"),
                              tr("Falla al crear la tabla\n"
                             "%1").arg(consultar.lastError().text()));
    }
    consultar.exec();
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

    QPrinter printer;
    QString NArchivo, Comando;
    QString Texto;
    QString AgenteText;
    QDir Dir;
    QPen pen;
    QFont font("Lucida", 20);
    QSqlQuery Auxiliar;
    int Linea, i;
    int fila;

    #define klinea 18;
    #define kLDatos 15;

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
        QMessageBox::critical(this,tr("Selección Agente"),
                              tr("Seleccionar A para generar el informe"));
        return;
    }

    NArchivo.append(ui->RepTablaTrab->model()->data(ui->RepTablaTrab->model()->index(IndexTrabajo,3)).toString()+".pdf");
    NArchivo.replace(2,1,".");
    NArchivo.replace(5,1,".");
    NArchivo.prepend("_");
    NArchivo.prepend(ui->RepTablaTrab->model()->data(ui->RepTablaTrab->model()->index(IndexTrabajo,1)).toString());
    NArchivo.prepend(RutaInfoPDF);
 //   qDebug () << RutaInfoPDF;
 //   qDebug () << NArchivo;

    QString fileName = QFileDialog::getSaveFileName(
                this,
                "Analisis de equipo - Guardado de archivos",
                NArchivo,
                "Text Files (*.pdf);;All Files (*.pdf)");

    if(fileName.isEmpty())
    {
 //       qDebug () << "chau chau chauuuuu.."  << fileName;
        return;
    }
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    printer.setPageOrientation(QPageLayout::Portrait);



    if (!painter.begin(&printer)) { // failed to open file
        qWarning("failed to open file, is it writable?");
        return ;
    }

    Linea = Encabezado();
    int inicio;
    int Filas;
    int palabras;


    if((Filas = ui->TrabajoPerifericos->rowCount()))
    {
        Linea = Encabezado();
        inicio = Linea + 8;
        painter.drawLine(50,inicio, 750, inicio);
        painter.drawLine(50,inicio, 750, inicio);

       for(i=0;i<Filas;i++)
       {
           Texto.clear();
           Texto.append(ui->TrabajoPerifericos->item(i,12)->text());
           palabras = Texto.count(" ");
  //         qDebug()   << "Espacios:"  << palabras ;     //Cuento la cantidad de espacios entre las palabras
           palabras *= 15;                              //Lo multiplico para
           palabras += 55;
           if((Linea + palabras) >=1085)
           {
               inicio = Linea + 8;
               if (! printer.newPage()) {
                   qWarning("failed in flushing page to disk, disk full?");
                   return ;
               }
               Linea = Encabezado();
               inicio = Linea + 8;
               painter.drawLine(50,inicio, 750, inicio);
           }

           Linea += kLDatos;
           Linea += 10;
//qDebug()  << i << "Inicio:" <<Linea << "Inc:" << palabras << "Fin:" << Linea+palabras;

           font.setWeight(11);
           font.setPixelSize(11);
           font.setBold(true);
           font.setItalic(false);
           painter.setFont(font);

           Texto.clear();
           Texto.append("Cod: ");
           painter.drawText(55,Linea,Texto);

 //          Texto.clear();
 //          Texto.append( QString::number(i));
 //          painter.drawText(85,Linea,Texto);

           Texto.clear();
           Texto.append("Desc: ");
           painter.drawText(250,Linea,Texto);

           Texto.clear();
           Texto.append("SN: ");
           painter.drawText(620,Linea,Texto);

           Texto.clear();
           Texto.append("Bon: ");
           painter.drawText(695,Linea,Texto);

           font.setBold(false);
//           font.setUnderline(false);
           font.setItalic(true);
           painter.setFont(font);

           Texto.clear();
           Texto.append(ui->TrabajoPerifericos->item(i,0)->text()); //Codigo
           painter.drawText(100,Linea,Texto);

//************************************
           Texto.clear();
           Texto.append(ui->TrabajoPerifericos->item(i,0)->text()); //Descripcion


           Comando.clear();
           Comando.append("SELECT desc FROM Productos WHERE producto = "
                          "'" +Texto+ "'");
           Auxiliar.prepare(Comando);
           Auxiliar.exec();
           Auxiliar.next();
           Texto.clear();
           Texto.append(Auxiliar.value(0).toByteArray().constData());
           painter.drawText(285,Linea,Texto);


   //        ui->TrabajoPerifericos->setItem(fila,13,new QTableWidgetItem (Auxiliar.value(0).toByteArray().constData()));
   //        Grupo.clear();
   //        Grupo.append(Auxiliar.value(0).toByteArray());

 //*****************************************************************
           Texto.clear();
           Texto.append(ui->TrabajoPerifericos->item(i,1)->text()); //Numero de Serie
           painter.drawText(640,Linea,Texto);

           Texto.clear();
           Texto.append(ui->TrabajoPerifericos->item(i,8)->text()); //Bonificacion
           painter.drawText(720,Linea,Texto);

           font.setBold(true);
//           font.setUnderline(true);
           font.setItalic(false);
           painter.setFont(font);

           Linea += kLDatos;
           Texto.clear();
           Texto.append("Falla: ");
           painter.drawText(55,Linea,Texto);
           font.setBold(true);

           font.setBold(false);
 //          font.setUnderline(false);
           font.setItalic(true);
           painter.setFont(font);

           Texto.clear();
           Texto.append(ui->TrabajoPerifericos->item(i,12)->text()); //falla

           int eDato, ind;
           QString aFalla;

           eDato = Texto.size();
//     qDebug() << eDato;
           while(eDato > 0 )
           {
               aFalla.clear();
               ind = Texto.lastIndexOf(' ');
               aFalla = Texto.mid(ind+1,eDato);
               Texto.truncate(ind);
               eDato = Texto.size();

 //             qDebug() << aFalla;
              //Algo impensado
              Comando.clear();
              Comando.append("SELECT descrip FROM FallasGrupo WHERE codigo = "
                             "'" +aFalla+ "'");
              Auxiliar.prepare(Comando);
              Auxiliar.exec();
              Auxiliar.next();

              Comando.clear();
              Comando.append(Auxiliar.value(0).toByteArray().constData());
              painter.drawText(90,Linea,Comando);
               if(ind > 0 )
                   Linea += kLDatos;
           }


           font.setBold(true);
//           font.setUnderline(true);
           font.setItalic(false);
           painter.setFont(font);

           Linea += kLDatos;
           Texto.clear();
           Texto.append("Observaciones: ");
           painter.drawText(55,Linea,Texto);
           font.setBold(true);

           font.setBold(false);
 //          font.setUnderline(false);
           font.setItalic(true);
           painter.setFont(font);

           Texto.clear();
           Texto.append(ui->TrabajoPerifericos->item(i,9)->text()); //Observaciones
           painter.drawText(150,Linea,Texto);

           pen.setWidth(1);
           painter.setPen(pen);
 //          inicio = inicio + (int)renglon;
           inicio = Linea +8;
           painter.drawLine(50,inicio, 750, inicio);

           //Dibujamos el encabezado.

       }
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

    qDebug () << Conf;
    QSqlQuery editar;

    if(!editar.prepare(Conf))
    {
        QMessageBox::critical(this,tr("Tabla Reparaciones"),
                              tr("Falla edicion de datos"));
    }
    editar.exec();

    QString AgenteText;

    AgenteText.clear();
    AgenteText.append(ui->RepTablaTrab->model()->data(ui->RepTablaTrab->model()->index(IndexTrabajo,1)).toString());
    if(AgenteText.isEmpty())

    FilTrRep->setFilterFixedString(AgenteText);

}
void trabajo::on_ReparacionesMostrar_clicked()
{
    FilTrRep->setFilterFixedString("");
 //   FilTrRep->
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

int trabajo::Encabezado()
{
    QString Texto;
    QString Direccion;
    QPen pen;
    int Linea;
    //Dibujamos el encabezado.

    QDir Dir;
    Direccion.append(Dir.currentPath());


    pen.setColor(Qt::blue);
    pen.setWidth(2);
    pen.setStyle(Qt::SolidLine);

    painter.setPen(pen);

    QPointF points[] = {
        QPointF(50.0, 30),
        QPointF(50.0, 100.0),
        QPointF(150.0, 100.0),
        QPointF(150.0, 30.0),
        QPointF(650.0, 30.0),
        QPointF(650.0, 100.0),
        QPointF(750.0, 100.0),
        QPointF(750.0, 30.0),
        QPointF(50.0, 30.0),
        QPointF(50.0, 100.0),
        QPointF(750.0, 100.0),
        QPointF(750.0, 30.0)
    };
    painter.drawPolygon(points,12);

    painter.drawPixmap(QRect(53,40,95,50),QPixmap(Direccion + "/SIID.png"));
    painter.drawPixmap(QRect(653,31,90,40),QPixmap(Direccion + "/controlagro.png"));
    painter.drawPixmap(QRect(653,65,90,40),QPixmap(Direccion + "/telemetric.png"));

    painter.setPen(QFont::Bold);
    QFont font("Times", 20);
    font.setBold(true);
    painter.setFont(font);

    Linea = 78;
    painter.drawText( 230,Linea, "Informe de Reparaciones");

    font.setWeight(11);
    font.setPixelSize(12);
    font.setBold(true);
    painter.setFont(font);

    Linea +=45;

    Texto.clear();
    Texto.append("Agente:");
    painter.drawText(50,Linea,Texto);

    Texto.clear();
    Texto.append("Rem. Agente:");                //Operario
    painter.drawText(280,Linea,Texto);

    Texto.clear();
    Texto.append("Rem.Trasp:");
    painter.drawText(450,Linea,Texto);

    Texto.clear();
    Texto.append("Fecha Trasnp:");         //Fecha Ingreso
    painter.drawText(590,Linea,Texto);

 //------------
    font.setBold(false);
    font.setItalic(true);
    painter.setFont(font); 

//Agente
    Texto.clear();
    Texto.append(ui->RepTablaTrab->model()->data(ui->RepTablaTrab->model()->index(IndexTrabajo,1)).toString());
    painter.drawText(103,Linea,Texto);
//Remito Agente
    Texto.clear();
    Texto.append(ui->RepTablaTrab->model()->data(ui->RepTablaTrab->model()->index(IndexTrabajo,9)).toString());
    painter.drawText(370,Linea,Texto);
//Remito Transporte
    Texto.clear();
    Texto.append(ui->RepTablaTrab->model()->data(ui->RepTablaTrab->model()->index(IndexTrabajo,7)).toString());
    painter.drawText(520,Linea,Texto);
//Fecha Transporte
    Texto.clear();
    Texto.append(ui->RepTablaTrab->model()->data(ui->RepTablaTrab->model()->index(IndexTrabajo,8)).toString());
    painter.drawText(680,Linea,Texto);
//----------------

    font.setBold(true);
    font.setItalic(false);
    painter.setFont(font);

    Linea +=klinea;
    Texto.clear();
    Texto.append("ID:");
    painter.drawText(50,Linea,Texto);

    Texto.clear();
    Texto.append("Fecha Ingreso:");
    painter.drawText(280,Linea,Texto);

    Texto.clear();
    Texto.append("Fecha Control:");
    painter.drawText(590,Linea,Texto);

 //-------------------
    font.setBold(false);
    font.setItalic(true);
    painter.setFont(font);
//ID
    Texto.clear();
    Texto.append(ui->RepTablaTrab->model()->data(ui->RepTablaTrab->model()->index(IndexTrabajo,0)).toString());
    painter.drawText(80,Linea,Texto);
//Fecha Ingreso
    Texto.clear();
    Texto.append(ui->RepTablaTrab->model()->data(ui->RepTablaTrab->model()->index(IndexTrabajo,2)).toString());
    painter.drawText(370,Linea,Texto);
//Fecha Control
    Texto.clear();
    Texto.append(ui->RepTablaTrab->model()->data(ui->RepTablaTrab->model()->index(IndexTrabajo,3)).toString());
    painter.drawText(680,Linea,Texto);

    Linea +=klinea;

    return Linea;
}

void trabajo::on_RepInterno_PDF_clicked()
{
    int fila;
    int RepId;
    fila = ui->RepTablaTrab->currentIndex().row();
    if(fila<0)
    {
        QMessageBox::critical(this,tr("Selección Trabajo"),
                              tr("Seleccionar trabajo para generar el informe"));
        return;
    }
    RepId = ui->RepTablaTrab->model()->data(ui->RepTablaTrab->model()->index(fila,0)).toInt();
    ReporteInterno RepInt;
    RepInt.RepInternoPDF(RepId);
}
