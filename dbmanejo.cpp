#include "dbmanejo.h"

#include <QDialog>
#include <QMessageBox>
#include <QDebug>
#include <QString>
#include "mainwindow.h"
#include <QDir>
#include <QFileDialog>
#include <QFile>
#include <QLabel>
#include <QDir>


QSqlDatabase dbManejo::db = QSqlDatabase::addDatabase("QSQLITE");


bool dbManejo::dbAbrirCrear ()
{
    QDir Dir;
 //   qDebug () << Dir.currentPath();
    // Creo/abro una base de datos
    QString nombre;
    if(Direccion.isEmpty())
    {
        nombre.append(Dir.currentPath());
        nombre.append("d:/PostVenta.sqlite");
    }
    else
    {
        nombre.append(Direccion + "PostVenta.sqlite");
    }
    db = QSqlDatabase::addDatabase("QSQLITE");

    db.setDatabaseName(nombre);

    if (!db.open())
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setWindowTitle("Base de Datos");
        msgBox.setText("fallo la creacion de la db "+nombre  + " Falla:  " + db.lastError().text());
        msgBox.exec();
        return false;

    }
    return true;

}
void dbManejo::SetDirDb(QString Dir)
{
    ClrDirDb();
    Direccion.append(Dir);
}
void dbManejo::ClrDirDb(void)
{
    Direccion.clear();
}
void dbManejo::Cerrardb()
{
    db.close();

}

//Retorna la base de datos que se creo
QSqlDatabase dbManejo::dbRetorna()
{
    return db;
}

void dbManejo::CargarFallas(QTableWidget &FALLAS,QString Tipo)
{
    QString Conf;
    QSqlQuery consultar;
    QString Falla;
    int fila;

    Conf.clear();
    Conf.append("SELECT * FROM FallasProducto");
    consultar.prepare(Conf);
    if(!consultar.exec())
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setWindowTitle("Tabla Fallas");
        msgBox.setText("Falla leer la tabla\n"+consultar.lastError().text());
        msgBox.exec();
    }

    FALLAS.clear();
    while(FALLAS.columnCount())
    {
        FALLAS.removeColumn(0);
    }

    FALLAS.insertColumn(0);
    FALLAS.insertColumn(1);
    FALLAS.insertColumn(2);

    FALLAS.setRowCount(0);
    fila = FALLAS.rowCount();
    FALLAS.setHorizontalHeaderItem(0, new QTableWidgetItem("Fallas"));
    FALLAS.setHorizontalHeaderItem(1,new QTableWidgetItem("Descripcion"));
    FALLAS.setHorizontalHeaderItem(2,new QTableWidgetItem("Grupo"));
//    FALLAS.insertRow(fila);
//    FALLAS.setRowHeight(fila,20);
 //   FALLAS.setItem(fila,0,new QTableWidgetItem("Fun_OK"));
 //   FALLAS.setItem(fila,1,new QTableWidgetItem("100"));
 //   FALLAS.item(fila,0)->setCheckState(Qt::Unchecked);
//    fila++;
    while(consultar.next())
    {
        Falla.clear();
        Falla.append(consultar.value(1).toByteArray().constData());
        if(Falla == Tipo)
        {
            FALLAS.insertRow(fila);
            FALLAS.setRowHeight(fila,20);
            FALLAS.setItem(fila,0,new QTableWidgetItem (consultar.value(2).toString()));
            FALLAS.setItem(fila,1,new QTableWidgetItem (consultar.value(3).toString()));
            FALLAS.setItem(fila,2,new QTableWidgetItem (consultar.value(4).toString()));
            FALLAS.item(fila,0)->setCheckState(Qt::Unchecked);
            fila ++;
        }

    }
    FALLAS.setColumnWidth(0,80);
    FALLAS.setColumnWidth(1,120);
    FALLAS.setColumnWidth(2,40);
}

void dbManejo::CargarProd(QTableWidget &PROD,int Tipo)
{
    QString Conf;
    QSqlQuery consultar;
   // QString Producto;
    int fila;
    int columna;

    Conf.clear();
    Conf.append("SELECT producto, desc FROM Productos WHERE tipo =" +QString::number(Tipo,10));
    consultar.prepare(Conf);

    if(!consultar.exec())
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setWindowTitle("Tabla Productos");
        msgBox.setText("Falla al leer la tabla\n"+consultar.lastError().text());
        msgBox.exec();
    }

    PROD.clear();
    columna = PROD.columnCount();
    PROD.setRowCount(0);
    if(!columna)
    {
        PROD.insertColumn(0);
        PROD.insertColumn(1);
    }
    fila = PROD.rowCount();
    PROD.setHorizontalHeaderItem(0, new QTableWidgetItem("Codigo"));
    PROD.setHorizontalHeaderItem(1,new QTableWidgetItem("Descripcion"));

    while(consultar.next())
    {
  //      TAux = consultar.value("tipo").toInt(&ok);
  //      if(TAux == Tipo)
  //      {
            PROD.insertRow(fila);
            PROD.setRowHeight(fila,20);
            PROD.setItem(fila,0,new QTableWidgetItem (consultar.value("producto").toString()));
            PROD.setItem(fila,1,new QTableWidgetItem (consultar.value("desc").toString()));
            fila ++;
 //       }

    }
    PROD.setColumnWidth(0,100);
    PROD.setColumnWidth(1,235);
    PROD.setSortingEnabled(true);
  //  PROD.sortByColumn(0,Qt::AscendingOrder);
}

void dbManejo::ActualizarCaudalimetro(QTableWidget &SCC, int ID)
{
    int fila  = 0;
    QSqlQuery consultar, con;
    SCC.setRowCount(0);
    consultar.prepare("SELECT * FROM Caudalimetro WHERE repid == "+QString::number(ID,10));
    if(!consultar.exec())
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setWindowTitle("Tabla Caudalimetro");
        msgBox.setText("Falla al crear la tabla\n"+consultar.lastError().text());
        msgBox.exec();
    }

    while(consultar.next())
    {
        con.prepare("SELECT * FROM Conceptos WHERE id = '" + consultar.value("concepto").toString()+ "'");
        con.exec();
        con.next();
        SCC.insertRow(fila);
        SCC.setRowHeight(fila,20);
        SCC.setItem(fila,0,new QTableWidgetItem (consultar.value(0).toString()));
        SCC.setItem(fila,1,new QTableWidgetItem (consultar.value(1).toString()));
        SCC.setItem(fila,2,new QTableWidgetItem (consultar.value(2).toString()));
        SCC.setItem(fila,3,new QTableWidgetItem (consultar.value(3).toString()));
        SCC.setItem(fila,4,new QTableWidgetItem (consultar.value(4).toString()));
        SCC.setItem(fila,5,new QTableWidgetItem (consultar.value(5).toString()));
        SCC.setItem(fila,6,new QTableWidgetItem (consultar.value(6).toString()));
        SCC.setItem(fila,7,new QTableWidgetItem (consultar.value(7).toString()));
        SCC.setItem(fila,8,new QTableWidgetItem (consultar.value(8).toString()));
        SCC.setItem(fila,9,new QTableWidgetItem (consultar.value(9).toString()));
        SCC.setItem(fila,10,new QTableWidgetItem (consultar.value(10).toString()));
        SCC.setItem(fila,11,new QTableWidgetItem (consultar.value(11).toString()));
        SCC.setItem(fila,12,new QTableWidgetItem (consultar.value(12).toString()));
        SCC.setItem(fila,13,new QTableWidgetItem (consultar.value(13).toString()));
        SCC.setItem(fila,14,new QTableWidgetItem (consultar.value(14).toString()));
        SCC.setItem(fila,15,new QTableWidgetItem (consultar.value(15).toString()));
        SCC.setItem(fila,16,new QTableWidgetItem (consultar.value(16).toString()));
        SCC.setItem(fila,17,new QTableWidgetItem (consultar.value(17).toString()));
        SCC.setItem(fila,18,new QTableWidgetItem (consultar.value(18).toString()));
        SCC.setItem(fila,19,new QTableWidgetItem (consultar.value(19).toString()));
        SCC.setItem(fila,20,new QTableWidgetItem (con.value("concepto").toString()));
        fila ++;

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



void dbManejo::CargarIngreso(QTableWidget &TABLA, int ID)
{
    QString Conf;
    if (!ID)
        return;
    Conf.append("SELECT * FROM Ingreso WHERE repid == " +QString::number(ID,10));

    QSqlQuery consultar;
    consultar.prepare(Conf);
    if(!consultar.exec())
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setWindowTitle("Tabla Ingreso");
        msgBox.setText("Falla al crear la tabla\n"+consultar.lastError().text());
        msgBox.exec();;
    }


    int fila  = 0;

    TABLA.setRowCount(0);

    while(consultar.next())
    {
 //       if(ID == consultar.value("repid").toByteArray().toInt())
//       {
            TABLA.insertRow(fila);
            TABLA.setRowHeight(fila,20);
            TABLA.setItem(fila,0,new QTableWidgetItem (consultar.value(0).toString()));
            TABLA.setItem(fila,1,new QTableWidgetItem (consultar.value(1).toString()));
            TABLA.setItem(fila,2,new QTableWidgetItem (consultar.value(2).toString()));
            TABLA.setItem(fila,3,new QTableWidgetItem (consultar.value(3).toString()));
            TABLA.setItem(fila,4,new QTableWidgetItem (consultar.value(4).toString()));
            TABLA.setItem(fila,5,new QTableWidgetItem (consultar.value(5).toString()));
            TABLA.setItem(fila,6,new QTableWidgetItem (consultar.value(6).toString()));
            TABLA.setItem(fila,7,new QTableWidgetItem (consultar.value(7).toString()));
            fila ++;
//        }
    }
    TABLA.setColumnWidth(0,40);
    TABLA.setColumnWidth(1,100);
    TABLA.setColumnWidth(2,180);
    TABLA.setColumnWidth(3,50);
    TABLA.setColumnWidth(4,50);
    TABLA.setColumnWidth(5,50);
    TABLA.setColumnWidth(6,200);
    TABLA.setColumnWidth(7,50);
    TABLA.scrollToBottom();

}

void dbManejo::BorrarItem(QString Tabla, int Item)
{
    QSqlQuery borrar;
    borrar.prepare("DELETE FROM "+Tabla+" WHERE id ="+QString::number(Item,10)+"");
    if(!borrar.exec())
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setWindowTitle("Tabla "+Tabla);
        msgBox.setText("Falla al borrar datos de la tabla\n"+borrar.lastError().text());
        msgBox.exec();
    }

}

bool dbManejo::BorrarTrabajo(int id)
{
    QSqlQuery borrar;
    borrar.prepare("DELETE FROM Caudalimetro WHERE repid =" + QString::number(id,10));
    if(!borrar.exec())
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setWindowTitle("Tabla Caudalimetro");
        msgBox.setText("Falla al borrar datos de la tabla\n"+borrar.lastError().text());
        msgBox.exec();
        return false;
    }

    borrar.prepare("DELETE FROM Instalaciones WHERE repid =" + QString::number(id,10));
    if(!borrar.exec())
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setWindowTitle("Tabla Instalaciones");
        msgBox.setText("Falla al borrar datos de la tabla\n"+borrar.lastError().text());
        msgBox.exec();
        return false;
    }

    borrar.prepare("DELETE FROM Insumos WHERE repid =" + QString::number(id,10));
    if(!borrar.exec())
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setWindowTitle("Tabla Insumos");
        msgBox.setText("Falla al borrar datos de la tabla\n"+borrar.lastError().text());
        msgBox.exec();
        return false;
    }
    borrar.prepare("DELETE FROM Monitores WHERE repid =" + QString::number(id,10));
    if(!borrar.exec())
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setWindowTitle("Tabla Monitores");
        msgBox.setText("Falla al borrar datos de la tabla\n"+borrar.lastError().text());
        msgBox.exec();
        return false;
    }
    borrar.prepare("DELETE FROM Perifericos WHERE repid =" + QString::number(id,10));
    if(!borrar.exec())
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setWindowTitle("Tabla Perifericos");
        msgBox.setText("Falla al borrar datos de la tabla\n"+borrar.lastError().text());
        msgBox.exec();
        return false;
    }
    borrar.prepare("DELETE FROM Ingreso WHERE repid =" + QString::number(id,10));
    if(!borrar.exec())
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setWindowTitle("Tabla Ingreso");
        msgBox.setText("Falla al borrar datos de la tabla\n"+borrar.lastError().text());
        msgBox.exec();
        return false;
    }
    return true;


}
void dbManejo::ActualizarPerifericos(QTableWidget &PER, int ID)
{
    int fila  = 0;
    QSqlQuery consultar, con;
    PER.setRowCount(0);
    consultar.prepare("SELECT * FROM Perifericos WHERE repid == " + QString::number(ID,10));
    if(!consultar.exec())
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setWindowTitle("Tabla Perifericos");
        msgBox.setText("Falla al crear la tabla\n"+consultar.lastError().text());
        msgBox.exec();
    }

    while(consultar.next())
    {
        con.prepare("SELECT * FROM Conceptos WHERE id = '" + consultar.value("concepto").toString()+ "'");

        if(!con.exec())
        {
            QMessageBox msgBox;
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.setWindowTitle("Tabla Concepto");
            msgBox.setText("Falla al leer la tabla\n"+consultar.lastError().text());
            msgBox.exec();
        }
        con.next();
        PER.insertRow(fila);
        PER.setRowHeight(fila,20);

        PER.setItem(fila,0,new QTableWidgetItem (consultar.value("id").toString()));
        PER.setItem(fila,1,new QTableWidgetItem (consultar.value("nombre").toString()));
        PER.setItem(fila,2,new QTableWidgetItem (consultar.value("sn").toString()));
        PER.setItem(fila,3,new QTableWidgetItem (consultar.value("ffab").toString()));
        PER.setItem(fila,4,new QTableWidgetItem (consultar.value("finst").toString()));
        PER.setItem(fila,5,new QTableWidgetItem (consultar.value("vsoft").toString()));
        PER.setItem(fila,6,new QTableWidgetItem (consultar.value("fsoft").toString()));
        PER.setItem(fila,7,new QTableWidgetItem (consultar.value("conf").toString()));
        PER.setItem(fila,8,new QTableWidgetItem (consultar.value("falla").toString()));
        PER.setItem(fila,9,new QTableWidgetItem (consultar.value("bonif").toString()));
        PER.setItem(fila,10,new QTableWidgetItem (consultar.value("obs").toString()));
        PER.setItem(fila,11,new QTableWidgetItem (consultar.value("frep").toString()));
        PER.setItem(fila,12,new QTableWidgetItem (consultar.value("repid").toString()));
        PER.setItem(fila,13,new QTableWidgetItem (consultar.value("grupo").toString()));
        PER.setItem(fila,14,new QTableWidgetItem (con.value("concepto").toString()));

        fila ++;
    }

    PER.setColumnWidth(0,40);
    PER.setColumnWidth(1,90);
    PER.setColumnWidth(2,50);
    PER.setColumnWidth(3,70);
    PER.setColumnWidth(4,70);
    PER.setColumnWidth(5,40);
    PER.setColumnWidth(6,70);
    PER.setColumnWidth(7,125);
    PER.setColumnWidth(8,100);
    PER.setColumnWidth(9,40);
    PER.setColumnWidth(10,120);
    PER.setColumnWidth(11,70);
    PER.setColumnWidth(12,40);
    PER.setColumnWidth(13,40);
//    PER.setColumnWidth(14,100);
//    PER.setColumnWidth(15,80);
//    PER.setColumnWidth(16,100);
//    PER.setColumnWidth(17,80);
//    PER.setColumnWidth(18,50);
    PER.scrollToBottom();
}

void dbManejo::ActualizarMonitores(QTableWidget &PER, int ID)
{
    int fila  = 0;
    QSqlQuery consultar,con;
    PER.setRowCount(0);
    consultar.prepare("SELECT * FROM Monitores WHERE repid ==" + QString::number(ID,10));
    if(!consultar.exec())
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setWindowTitle("Tabla Perifericos");
        msgBox.setText("Falla al crear la tabla\n"+consultar.lastError().text());
        msgBox.exec();
    }

    while(consultar.next())
    {
        con.prepare("SELECT * FROM Conceptos WHERE id = '" + consultar.value("concepto").toString()+ "'");
        con.exec();
        con.next();

        PER.insertRow(fila);
        PER.setRowHeight(fila,20);
        PER.setItem(fila,0,new QTableWidgetItem (consultar.value(0).toString()));
        PER.setItem(fila,1,new QTableWidgetItem (consultar.value(1).toString()));
        PER.setItem(fila,2,new QTableWidgetItem (consultar.value(2).toString()));
        PER.setItem(fila,3,new QTableWidgetItem (consultar.value(3).toString()));
        PER.setItem(fila,4,new QTableWidgetItem (consultar.value(4).toString()));
        PER.setItem(fila,5,new QTableWidgetItem (consultar.value(5).toString()));
        PER.setItem(fila,6,new QTableWidgetItem (consultar.value(6).toString()));
        PER.setItem(fila,7,new QTableWidgetItem (consultar.value(7).toString()));
        PER.setItem(fila,8,new QTableWidgetItem (consultar.value(8).toString()));
        PER.setItem(fila,9,new QTableWidgetItem (consultar.value(9).toString()));
        PER.setItem(fila,10,new QTableWidgetItem (consultar.value(10).toString()));
        PER.setItem(fila,11,new QTableWidgetItem (con.value("concepto").toString()));
        fila ++;
    }

    PER.setColumnWidth(0,50);
    PER.setColumnWidth(1,100);
    PER.setColumnWidth(2,50);
    PER.setColumnWidth(3,70);
    PER.setColumnWidth(4,70);
    PER.setColumnWidth(5,100);
    PER.setColumnWidth(6,50);
    PER.setColumnWidth(7,140);
    PER.setColumnWidth(8,80);
    PER.setColumnWidth(9,40);
    PER.setColumnWidth(10,40);
}

void dbManejo::ActualizarInstalaciones(QTableWidget &INS, int ID)
{
    int fila  = 0;
    QSqlQuery consultar, con;
    INS.setRowCount(0);
    consultar.prepare("SELECT * FROM Instalaciones WHERE repid ==" + QString::number(ID,10));
    if(!consultar.exec())
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setWindowTitle("Tabla Instalaciones");
        msgBox.setText("Falla al crear la tabla\n"+consultar.lastError().text());
        msgBox.exec();
    }

    while(consultar.next())
    {

        con.prepare("SELECT * FROM Conceptos WHERE id = '" + consultar.value("concepto").toString()+ "'");
        con.exec();
        con.next();
        INS.insertRow(fila);
        INS.setRowHeight(fila,20);
        INS.setItem(fila,0,new QTableWidgetItem (consultar.value(0).toString()));
        INS.setItem(fila,1,new QTableWidgetItem (consultar.value(1).toString()));
        INS.setItem(fila,2,new QTableWidgetItem (consultar.value(2).toString()));
        INS.setItem(fila,3,new QTableWidgetItem (consultar.value(3).toString()));
        INS.setItem(fila,4,new QTableWidgetItem (consultar.value(4).toString()));
        INS.setItem(fila,5,new QTableWidgetItem (consultar.value(5).toString()));
        INS.setItem(fila,6,new QTableWidgetItem (consultar.value(6).toString()));
        INS.setItem(fila,7,new QTableWidgetItem (consultar.value(7).toString()));
        INS.setItem(fila,8,new QTableWidgetItem (consultar.value(8).toString()));
        INS.setItem(fila,9,new QTableWidgetItem (con.value("concepto").toString()));
        fila ++;
    }

    INS.setColumnWidth(0,50);
    INS.setColumnWidth(1,120);
    INS.setColumnWidth(2,90);
    INS.setColumnWidth(3,100);
    INS.setColumnWidth(4,50);
    INS.setColumnWidth(5,300);
    INS.setColumnWidth(6,80);
    INS.setColumnWidth(7,40);
    INS.setColumnWidth(8,40);
}

int dbManejo::BucaEquipo(QString Equipo )
{
    QSqlQuery consultar;
    QString DatosAux;

    consultar.prepare("SELECT * FROM Productos WHERE producto == " "'" + Equipo + "'" );
    consultar.exec();
    consultar.next();
    return consultar.value(4).toInt();
 //   while(consultar.next())
 //   {
 //       DatosAux.clear();
 //       DatosAux.append(consultar.value(1).toByteArray().constData());
 //       if(DatosAux == Equipo)
 //       {
 //           tipo = consultar.value(4).toInt();
 //           break;
 //       }
 //   }
 //   return tipo;
}

QString dbManejo::Mascara(QString Equipo)
{
    QSqlQuery consultar;
    QString DatosAux;
    QString Mascara;
    consultar.prepare("SELECT * FROM Productos WHERE producto == ""'" + Equipo + "'");
    consultar.exec();
    consultar.next();
    return consultar.value(3).toString();
 /*   while(consultar.next())
    {
        DatosAux.clear();
        DatosAux.append(consultar.value(1).toByteArray().constData());
        if(DatosAux == Equipo)
        {
            Mascara = consultar.value(3).toByteArray().constData();
            break;
        }
    }
    return Mascara;
    */
}

void dbManejo::ActualizarInsumos(QTableWidget &INSUMOS, int ID)
{
    int fila  = 0;
    QSqlQuery consultar, con;
    INSUMOS.setRowCount(0);
    consultar.prepare("SELECT * FROM Insumos WHERE repid =" + QString::number(ID,10));
    if(!consultar.exec())
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setWindowTitle("Tabla Insumos");
        msgBox.setText("Falla al leer la tabla\n"+consultar.lastError().text());
        msgBox.exec();
    }

    while(consultar.next())
    {

        con.prepare("SELECT * FROM Conceptos WHERE id = '" + consultar.value("concepto").toString()+ "'");
        con.exec();
        con.next();
        INSUMOS.insertRow(fila);
        INSUMOS.setRowHeight(fila,20);
        INSUMOS.setItem(fila,0,new QTableWidgetItem (consultar.value("id").toString()));
        INSUMOS.setItem(fila,1,new QTableWidgetItem (consultar.value("cantidad").toString()));
        INSUMOS.setItem(fila,2,new QTableWidgetItem (consultar.value("codigo").toString()));
        INSUMOS.setItem(fila,3,new QTableWidgetItem (consultar.value("nombre").toString()));
        INSUMOS.setItem(fila,4,new QTableWidgetItem (consultar.value("falla").toString()));
        INSUMOS.setItem(fila,5,new QTableWidgetItem (consultar.value("grupo").toString()));
        INSUMOS.setItem(fila,6,new QTableWidgetItem (consultar.value("bonif").toString()));
        INSUMOS.setItem(fila,7,new QTableWidgetItem (con.value("concepto").toString()));
        INSUMOS.setItem(fila,8,new QTableWidgetItem (consultar.value("obs").toString()));
        INSUMOS.setItem(fila,9,new QTableWidgetItem (consultar.value("frep").toString()));
        INSUMOS.setItem(fila,10,new QTableWidgetItem (consultar.value("repid").toString()));
        fila ++;
    }

    INSUMOS.setColumnWidth(0,50);
    INSUMOS.setColumnWidth(1,50);
    INSUMOS.setColumnWidth(2,50);
    INSUMOS.setColumnWidth(3,180);
    INSUMOS.setColumnWidth(4,80);
    INSUMOS.setColumnWidth(5,80);
    INSUMOS.setColumnWidth(6,80);
    INSUMOS.setColumnWidth(7,40);
    INSUMOS.setColumnWidth(8,240);
    INSUMOS.setColumnWidth(9,40);
    INSUMOS.setColumnWidth(10,40);
}

int dbManejo::Diferencia (void)
{
    QSqlQuery consultar, ingreso, procesado;


    int ing, proc, id, i,a;
    i=a=0;

    consultar.prepare("SELECT id FROM Reparaciones");
    consultar.exec();
    while(consultar.next())
    {
        a++;
        id = consultar.value(0).toInt();
        ingreso.prepare("SELECT SUM(cantidad) FROM Ingreso WHERE RepId = "+ QString::number(id,10));
        ingreso.exec();
        ingreso.next();
        ing = ingreso.value(0).toInt();

        qDebug () <<"-------- " << id << " -----------------";
  //      qDebug () << "Ingreso:"<<ing;

        procesado.prepare("SELECT SUM(cantidad) FROM Caudalimetro WHERE RepId = "+ QString::number(id,10));
        procesado.exec();
        procesado.next();
        proc = procesado.value(0).toInt();

   //     qDebug () <<"Caudal" <<proc;

        procesado.prepare("SELECT SUM(cantidad) FROM Instalaciones WHERE RepId = "+ QString::number(id,10));
        procesado.exec();
        procesado.next();
        proc = proc + procesado.value(0).toInt();
    //    qDebug () <<"Instalaciones"<< proc;

        procesado.prepare("SELECT SUM(cantidad) FROM Insumos WHERE RepId = "+ QString::number(id,10));
        procesado.exec();
        procesado.next();
        proc = proc + procesado.value(0).toInt();
    //    qDebug () << "Insumos"<<proc;

        procesado.prepare("SELECT SUM(cantidad) FROM Monitores WHERE RepId = "+ QString::number(id,10));
        procesado.exec();
        procesado.next();
        proc = proc + procesado.value(0).toInt();
  //      qDebug () << "Monitores"<<proc;

        procesado.prepare("SELECT SUM(cantidad) FROM Perifericos WHERE RepId = "+ QString::number(id,10));
        procesado.exec();
        procesado.next();
        proc = proc + procesado.value(0).toInt();
   //     qDebug () << "Perifericos"<<proc;
        qDebug () << "Diferencia"<< (ing - proc);
        if (ing - proc >= 1)
        {
            i++;
            QMessageBox msgBox;
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.setWindowTitle("Busqueda de Diferencias");
            msgBox.setText("ID: "+QString::number(id,10) + " Ingreso: " + QString::number(ing ,10) + " Rep: " + QString::number(proc,10));
            msgBox.exec();

            return id;
        }

    }
    QMessageBox msgBox;
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setWindowTitle("Busqueda de Diferencias");
    msgBox.setText("Felicitaciones todas las reparaciones estan equilibradas");
    msgBox.exec();
    return 0;
}
