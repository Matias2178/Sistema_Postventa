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


bool dbManejo::dbAbrirCrear()
{
    QDir Dir;
//    qDebug () << Dir.currentPath();
//    qDebug () << Dir.homePath();

    // Creo/abro una base de datos
    QString nombre;
    if(Direccion.isEmpty())
    {
        nombre.append(Dir.currentPath());
        nombre.append("d:/PostVenta.sqlite");
    }
    else
    {
        nombre.append(Direccion);
    }

 //   nombre.append("d:/PostVenta.sqlite");
  //  nombre.append("EstoFunciona.sqlite");
 //   qDebug() << nombre;


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
    else
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setWindowTitle("Base de Datos");
        msgBox.setText("Se abrio correctamente: " + nombre);
        msgBox.exec();
        return true;
    }
//    qDebug () << db.lastError().text();

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

void dbManejo::CrearProductos()
{
    QString Conf;
    Conf.append("CREATE TABLE IF NOT EXISTS Productos("
                "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                "producto VARCHAR[30],"
                "desc VARCHAR[80],"
                "version VARCHAR[30],"
                "tipo INTEGER NOT NULL"
                ");");

    QSqlQuery crear;
    if(!crear.prepare(Conf))
    {
//        QMessageBox::critical(this,tr("Tabla Productos"),
//                              tr("Falla al crear la tabla\n"
//                                 "%1").arg(crear.lastError().text()));

        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setWindowTitle("Tabla Productos");
        msgBox.setText("Falla al crear la tabla\n"+crear.lastError().text());
        msgBox.exec();
    }
    crear.exec();
}

void dbManejo::CrearFallas()
{
    QString Conf;
    Conf.append("CREATE TABLE IF NOT EXISTS Fallas("
                "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                "producto VARCHAR[30],"
                "falla VARCHAR[10],"
                "descripcion VARCHAR[50],"
                "fpre VARCHAR[10],"
                "bonif INTEGER"
                 ");");

    QSqlQuery crear;
    if(!crear.prepare(Conf))
    {
//        QMessageBox::critical(this,tr("Tabla Fallas"),
//                              tr("Falla al crear la tabla\n"
//                                 "%1").arg(crear.lastError().text()));
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setWindowTitle("Tabla Fallas");
        msgBox.setText("Falla al crear la tabla\n"+crear.lastError().text());
        msgBox.exec();
    }
    crear.exec();
}

void dbManejo::CrearAgentes()
{
    QString Conf;
    Conf.append("CREATE TABLE IF NOT EXISTS Agente("
                 "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                 "agente VARCHAR[30]"
                 ");");

    QSqlQuery crear;
    if(!crear.prepare(Conf))
    {
//        QMessageBox::critical(this,tr("Tabla Agente"),
//                              tr("Falla al crear la tabla\n"
//                                 "%1").arg(crear.lastError().text()));
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setWindowTitle("Tabla Agente");
        msgBox.setText("Falla al crear la tabla\n"+crear.lastError().text());
        msgBox.exec();
    }
    crear.exec();
}

void dbManejo::CrearOperario()
{
    QString Conf;
    Conf.append("CREATE TABLE IF NOT EXISTS Operario("
                 "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                 "agente VARCHAR[30]"
                 ");");

    QSqlQuery crear;
    if(!crear.prepare(Conf))
    {
//        QMessageBox::critical(this,tr("Tabla Operario"),
//                              tr("Falla al crear la tabla\n"
//                                 "%1").arg(crear.lastError().text()));
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setWindowTitle("Tabla Operario");
        msgBox.setText("Falla al crear la tabla\n"+crear.lastError().text());
        msgBox.exec();
    }
    crear.exec();
}

void dbManejo::CrearReparaciones()
{
    QString Conf;
    Conf.append("CREATE TABLE IF NOT EXISTS Reparaciones("
                "id     INTEGER PRIMARY KEY AUTOINCREMENT,"
                "agente VARCHAR[30],"
                "fing   VARCHAR[10],"
                "frep   VARCHAR[10],"
                "operario VARCHAR[30],"
                "pres   VARCHAR[10]"
                 ");");

    QSqlQuery crear;
    if(!crear.prepare(Conf))
    {
//        QMessageBox::critical(this,tr("Tabla Reparaciones"),
//                              tr("Falla al crear la tabla\n"
//                                 "%1").arg(crear.lastError().text()));
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setWindowTitle("Tabla Reparaciones");
        msgBox.setText("Falla al crear la tabla\n"+crear.lastError().text());
        msgBox.exec();
    }
    crear.exec();
}

void dbManejo::CrearMonitores()
{
    QString Conf;
    Conf.append("CREATE TABLE IF NOT EXISTS Monitores("
                "id     INTEGER PRIMARY KEY AUTOINCREMENT,"
                "nombre VARCHAR[30],"
                "sn     INTEGER,"
                "vsoft  VARCHAR[10],"
                "falla  VARCHAR[50],"
                "bonif  INTEGER,"
                "obs    VARCHAR[50],"
                "frep   VARCHAR[10],"
                "repid   INTEGER"
                 ");");

    QSqlQuery crear;
    if(!crear.prepare(Conf))
    {
//        QMessageBox::critical(this,tr("Tabla Monitores"),
//                              tr("Falla al crear la tabla\n"
//                             "%1").arg(crear.lastError().text()));
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setWindowTitle("Tabla Monitores");
        msgBox.setText("Falla al crear la tabla\n"+crear.lastError().text());
        msgBox.exec();
    }
    crear.exec();
}

void dbManejo::CrearPerifericos()
{
    QString Conf;
    Conf.append("CREATE TABLE IF NOT EXISTS Perifericos("
                "id     INTEGER PRIMARY KEY AUTOINCREMENT,"
                "nombre VARCHAR[30],"
                "sn     INTEGER,"
                "ffab   VARCHAR[10],"
                "finst  VARCHAR[10],"
                "vsoft  VARCHAR[10],"
                "fsoft  VARCHAR[10],"
                "conf   VARCHAR[50],"
                "falla  VARCHAR[50],"
                "bonif  INTEGER,"
                "obs    VARCHAR[50],"
                "frep   VARCHAR[10],"
                "repid  INTEGER"
                 ");");

    QSqlQuery crear;
    if(!crear.prepare(Conf))
    {
//        QMessageBox::critical(this,tr("Tabla Perifericos"),
//                              tr("Falla al crear la tabla\n"
//                                 "%1").arg(crear.lastError().text()));
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setWindowTitle("Tabla Productos");
        msgBox.setText("Falla al crear la tabla\n"+crear.lastError().text());
        msgBox.exec();
    }
    crear.exec();
}

void dbManejo::CrearCaudalimetro()
{
    QString Conf;
    Conf.append("CREATE TABLE IF NOT EXISTS Caudalimetro("
                "id     INTEGER PRIMARY KEY AUTOINCREMENT,"
                "nombre VARCHAR[30],"
                "sn     INTEGER,"
                "movil  VARCHAR[10],"
                "ffab   VARCHAR[10],"
                "finst  VARCHAR[10],"
                "vsoft  VARCHAR[10],"
                "fsoft  VARCHAR[10],"
                "tmt    VARCHAR[10],"
                "cct    VARCHAR[10],"
                "desc   VARCHAR[10],"
                "descat VARCHAR[10],"
                "cbmag  VARCHAR[10],"
                "tbmag  VARCHAR[10],"
                "falla  VARCHAR[50],"
                "bonif  INTEGER,"
                "obs    VARCHAR[50],"
                "frep   VARCHAR[10],"
                "repid  INTEGER"
                 ");");

    QSqlQuery crear;
    if(!crear.prepare(Conf))
    {
//        QMessageBox::critical(this,tr("Tabla Caudalimetro"),
//                              tr("Falla al crear la tabla\n"
//                                 "%1").arg(crear.lastError().text()));
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setWindowTitle("Tabla Caudalimetro");
        msgBox.setText("Falla al crear la tabla\n"+crear.lastError().text());
        msgBox.exec();
    }
    crear.exec();
}

void dbManejo::CrearIngreso()
{
    QString Conf;
    Conf.append("CREATE TABLE IF NOT EXISTS Ingreso("
                "id     INTEGER PRIMARY KEY AUTOINCREMENT,"
                "nombre VARCHAR[30],"
                "desc   VARCHAR[80],"
                "sn     INTEGER,"
                "cant   INTEGER,"
                "fact   VARCHAR[20],"
                "obs    VARCHAR[60],"
                "repid  INGEGER"
                ");");

    QSqlQuery crear;
    if(!crear.prepare(Conf))
    {
//        QMessageBox::critical(this,tr("Tabla Ingeso"),
//                              tr("Falla al crear la tabla\n"
//                             "%1").arg(crear.lastError().text()));
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setWindowTitle("Tabla Ingreso");
        msgBox.setText("Falla al crear la tabla\n"+crear.lastError().text());
        msgBox.exec();
    }
    crear.exec();
}

void dbManejo::CrearInstalaciones()
{
    QString Conf;
    Conf.append("CREATE TABLE IF NOT EXISTS Instalaciones("
                "id     INTEGER PRIMARY KEY AUTOINCREMENT,"
                "nombre VARCHAR[30],"
                "sn     INTEGER,"
                "falla  VARCHAR[50],"
                "bonif  INTEGER,"
                "obs    VARCHAR[50],"
                "frep   VARCHAR[10],"
                "repid  INGEGER"
                 ");");

    QSqlQuery crear;
    if(!crear.prepare(Conf))
    {
//        QMessageBox::critical(this,tr("Tabla Instalaciones"),
//                              tr("Falla al crear la tabla\n"
//                                 "%1").arg(crear.lastError().text()));
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setWindowTitle("Tabla Instalaciones");
        msgBox.setText("Falla al crear la tabla\n"+crear.lastError().text());
        msgBox.exec();
    }
    crear.exec();
}

void dbManejo::CargarFallas(QTableWidget &FALLAS,QString Tipo)
{
    QString Conf;
    QSqlQuery consultar;
    QString Falla;
    int fila;
//    int columna;

    Conf.clear();
    Conf.append("SELECT * FROM Fallas");

    if(!consultar.prepare(Conf))
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setWindowTitle("Tabla Fallas");
        msgBox.setText("Falla al crear la tabla\n"+consultar.lastError().text());
        msgBox.exec();
    }
    consultar.exec();
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
    FALLAS.setHorizontalHeaderItem(2,new QTableWidgetItem("Bonif"));
    FALLAS.insertRow(fila);
    FALLAS.setRowHeight(fila,20);
    FALLAS.setItem(fila,0,new QTableWidgetItem("Fun_OK"));
    FALLAS.setItem(fila,1,new QTableWidgetItem("100"));
    FALLAS.item(fila,0)->setCheckState(Qt::Unchecked);
    fila++;
    while(consultar.next())
    {
        Falla.clear();
        Falla.append(consultar.value(1).toByteArray().constData());
        if(Falla == Tipo)
        {
            FALLAS.insertRow(fila);
            FALLAS.setRowHeight(fila,20);
            FALLAS.setItem(fila,0,new QTableWidgetItem (consultar.value(2).toByteArray().constData()));
            FALLAS.setItem(fila,1,new QTableWidgetItem (consultar.value(3).toByteArray().constData()));
            FALLAS.setItem(fila,2,new QTableWidgetItem (consultar.value(5).toByteArray().constData()));
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
    int TAux;
    int fila;
    int columna;
    bool ok;

    Conf.clear();
    Conf.append("SELECT * FROM Productos");


    if(!consultar.prepare(Conf))
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setWindowTitle("Tabla Fallas");
        msgBox.setText("Falla al crear la tabla\n"+consultar.lastError().text());
        msgBox.exec();
    }
    consultar.exec();
    PROD.clear();
    columna = PROD.columnCount();
    PROD.setRowCount(0);
    if(!columna)
    {
        PROD.insertColumn(0);
        PROD.insertColumn(1);
        PROD.insertColumn(2);
    }
    fila = PROD.rowCount();
    PROD.setHorizontalHeaderItem(0, new QTableWidgetItem("Codigo"));
    PROD.setHorizontalHeaderItem(1,new QTableWidgetItem("Descripcion"));
    PROD.setHorizontalHeaderItem(2,new QTableWidgetItem("Version"));

    while(consultar.next())
    {
        TAux = consultar.value("tipo").toInt(&ok);
        if(TAux == Tipo)
        {
            PROD.insertRow(fila);
            PROD.setRowHeight(fila,20);
            PROD.setItem(fila,0,new QTableWidgetItem (consultar.value(1).toByteArray().constData()));
            PROD.setItem(fila,1,new QTableWidgetItem (consultar.value(2).toByteArray().constData()));
            PROD.setItem(fila,2,new QTableWidgetItem (consultar.value(3).toByteArray().constData()));
            fila ++;
        }

    }
    PROD.setColumnWidth(0,100);
    PROD.setColumnWidth(1,235);
    PROD.setColumnWidth(1,50);

}

QStringList dbManejo::CargarProductos(int Tipo)
{
    QStringList Lista1;
    QSqlQuery consultar;
    int fila  = 0;
    int TAux;
    bool ok;

    Lista1.clear();

    if(!consultar.prepare("SELECT * FROM Productos"))
    {
//        QMessageBox::critical(this,tr("Tabla Productos"),
//                              tr("Falla al leer la tabla\n"
//                                 "%1").arg(consultar.lastError().text()));
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setWindowTitle("Tabla Productos");
        msgBox.setText("Falla al crear la tabla\n"+consultar.lastError().text());
        msgBox.exec();;
    }
    consultar.exec();
    Lista1.append("Seleccionar");
    while(consultar.next())
    {
        TAux = consultar.value("tipo").toInt(&ok);
        if(TAux == Tipo)
        {
           Lista1.append(consultar.value(1).toByteArray().constData());
        }
        fila ++;
    }

    return Lista1;
}

void dbManejo::ActualizarCaudalimetro(QTableWidget &SCC, int ID)
{
    int fila  = 0;
    QSqlQuery consultar;
    SCC.setRowCount(0);

    if(!consultar.prepare("SELECT * FROM Caudalimetro"))
    {
//        QMessageBox::critical(this,tr("Tabla Caudalimetro"),
//                              tr("Falla al leer la tabla\n"
//                                 "%1").arg(consultar.lastError().text()));
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setWindowTitle("Tabla Caudalimetro");
        msgBox.setText("Falla al crear la tabla\n"+consultar.lastError().text());
        msgBox.exec();
    }
    consultar.exec();
    while(consultar.next())
    {
        if(ID == consultar.value("repid").toByteArray().toInt())
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

void dbManejo::CargarReparaciones(QTableWidget &TABLA, QString Tipo)
{
    QString Conf;
    bool todos;
    if((Tipo == "*")|| (Tipo == "Seleccionar"))
    {
        todos = true;
    }
    else
    {
        todos = false;
    }
    Conf.append("SELECT * FROM Reparaciones");

    QSqlQuery consultar;
    if(!consultar.prepare(Conf))
    {
//        QMessageBox::critical(this,tr("Tabla Reparaciones"),
//                              tr("Falla al leer la tabla\n"
//                                 "%1").arg(consultar.lastError().text()));
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setWindowTitle("Tabla Reparaciones");
        msgBox.setText("Falla al crear la tabla\n"+consultar.lastError().text());
        msgBox.exec();
    }
    consultar.exec();
    int fila  = 0;

    TABLA.setRowCount(0);
    while(consultar.next())
    {
        if((Tipo == consultar.value(1).toByteArray().constData())|| todos )
        {
            TABLA.insertRow(fila);
            TABLA.setRowHeight(fila,20);
            TABLA.setItem(fila,0,new QTableWidgetItem (consultar.value(0).toByteArray().constData()));
            TABLA.setItem(fila,1,new QTableWidgetItem (consultar.value(1).toByteArray().constData()));
            TABLA.setItem(fila,2,new QTableWidgetItem (consultar.value(2).toByteArray().constData()));
            TABLA.setItem(fila,3,new QTableWidgetItem (consultar.value(3).toByteArray().constData()));
            TABLA.setItem(fila,4,new QTableWidgetItem (consultar.value(4).toByteArray().constData()));
            TABLA.setItem(fila,5,new QTableWidgetItem (consultar.value(5).toByteArray().constData()));
            fila ++;
        }
    }
    TABLA.setColumnWidth(0,25);
    TABLA.setColumnWidth(1,200);
    TABLA.setColumnWidth(2,80);
    TABLA.setColumnWidth(3,80);
    TABLA.setColumnWidth(4,80);
    TABLA.setColumnWidth(5,80);
    TABLA.scrollToBottom();
}

void dbManejo::CargarIngreso(QTableWidget &TABLA, int ID)
{
    QString Conf;
    if (!ID)
        return;
    Conf.append("SELECT * FROM Ingreso");

    QSqlQuery consultar;
    if(!consultar.prepare(Conf))
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setWindowTitle("Tabla Ingreso");
        msgBox.setText("Falla al crear la tabla\n"+consultar.lastError().text());
        msgBox.exec();;
    }
    consultar.exec();

    int fila  = 0;

    TABLA.setRowCount(0);

    while(consultar.next())
    {
        if(ID == consultar.value("repid").toByteArray().toInt())
        {
            TABLA.insertRow(fila);
            TABLA.setRowHeight(fila,20);
            TABLA.setItem(fila,0,new QTableWidgetItem (consultar.value(0).toByteArray().constData()));
            TABLA.setItem(fila,1,new QTableWidgetItem (consultar.value(1).toByteArray().constData()));
            TABLA.setItem(fila,2,new QTableWidgetItem (consultar.value(2).toByteArray().constData()));
            TABLA.setItem(fila,3,new QTableWidgetItem (consultar.value(3).toByteArray().constData()));
            TABLA.setItem(fila,4,new QTableWidgetItem (consultar.value(4).toByteArray().constData()));
            TABLA.setItem(fila,5,new QTableWidgetItem (consultar.value(5).toByteArray().constData()));
            TABLA.setItem(fila,6,new QTableWidgetItem (consultar.value(6).toByteArray().constData()));
            TABLA.setItem(fila,7,new QTableWidgetItem (consultar.value(7).toByteArray().constData()));
            fila ++;
        }
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
    QString Conf;
    Conf.append("DELETE FROM "
                +Tabla+ " "
                " WHERE id ="
                ""+QString::number(Item,10)+""
                "");
    QSqlQuery borrar;
    if(!borrar.prepare(Conf))
    {
//        QMessageBox::critical(this,tr("Tabla %1").arg(Tabla),
//                              tr("Falla al leer la tabla\n"
//                                 "%1").arg(consultar.lastError().text()));
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setWindowTitle("Tabla "+Tabla);
        msgBox.setText("Falla al crear la tabla\n"+borrar.lastError().text());
        msgBox.exec();
    }
    borrar.exec();
}
void dbManejo::ActualizarPerifericos(QTableWidget &PER, int ID)
{
    int fila  = 0;
    QSqlQuery consultar;
    PER.setRowCount(0);

    if(!consultar.prepare("SELECT * FROM Perifericos"))
    {
//        QMessageBox::critical(this,tr("Tabla Perifericos"),
//                              tr("Falla al leer la tabla\n"
//                                 "%1").arg(consultar.lastError().text()));
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setWindowTitle("Tabla Perifericos");
        msgBox.setText("Falla al crear la tabla\n"+consultar.lastError().text());
        msgBox.exec();

    }
    consultar.exec();
    while(consultar.next())
    {
        if(ID == consultar.value("repid").toByteArray().toInt())
        {
            PER.insertRow(fila);
            PER.setRowHeight(fila,20);
            PER.setItem(fila,0,new QTableWidgetItem (consultar.value(0).toByteArray().constData()));
            PER.setItem(fila,1,new QTableWidgetItem (consultar.value(1).toByteArray().constData()));
            PER.setItem(fila,2,new QTableWidgetItem (consultar.value(2).toByteArray().constData()));
            PER.setItem(fila,3,new QTableWidgetItem (consultar.value(3).toByteArray().constData()));
            PER.setItem(fila,4,new QTableWidgetItem (consultar.value(4).toByteArray().constData()));
            PER.setItem(fila,5,new QTableWidgetItem (consultar.value(5).toByteArray().constData()));
            PER.setItem(fila,6,new QTableWidgetItem (consultar.value(6).toByteArray().constData()));
            PER.setItem(fila,7,new QTableWidgetItem (consultar.value(7).toByteArray().constData()));
            PER.setItem(fila,8,new QTableWidgetItem (consultar.value(8).toByteArray().constData()));
            PER.setItem(fila,9,new QTableWidgetItem (consultar.value(9).toByteArray().constData()));
            PER.setItem(fila,10,new QTableWidgetItem (consultar.value(10).toByteArray().constData()));
            PER.setItem(fila,11,new QTableWidgetItem (consultar.value(11).toByteArray().constData()));
            PER.setItem(fila,12,new QTableWidgetItem (consultar.value(12).toByteArray().constData()));
//            PER.setItem(fila,13,new QTableWidgetItem (consultar.value(13).toByteArray().constData()));
//            PER.setItem(fila,14,new QTableWidgetItem (consultar.value(14).toByteArray().constData()));
//            PER.setItem(fila,15,new QTableWidgetItem (consultar.value(15).toByteArray().constData()));
//            PER.setItem(fila,16,new QTableWidgetItem (consultar.value(16).toByteArray().constData()));
//            PER.setItem(fila,17,new QTableWidgetItem (consultar.value(17).toByteArray().constData()));
//            PER.setItem(fila,18,new QTableWidgetItem (consultar.value(18).toByteArray().constData()));
            fila ++;
        }
    }

    PER.setColumnWidth(0,40);
    PER.setColumnWidth(1,60);
    PER.setColumnWidth(2,50);
    PER.setColumnWidth(3,70);
    PER.setColumnWidth(4,70);
    PER.setColumnWidth(5,40);
    PER.setColumnWidth(6,70);
    PER.setColumnWidth(7,100);
    PER.setColumnWidth(8,100);
    PER.setColumnWidth(9,40);
    PER.setColumnWidth(10,100);
    PER.setColumnWidth(11,70);
    PER.setColumnWidth(12,40);
//    PER.setColumnWidth(13,50);
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
    QSqlQuery consultar;
    PER.setRowCount(0);

    if(!consultar.prepare("SELECT * FROM Monitores"))
    {
//        QMessageBox::critical(this,tr("Tabla Perifericos"),
//                              tr("Falla al leer la tabla\n"
//                                 "%1").arg(consultar.lastError().text()));
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setWindowTitle("Tabla Perifericos");
        msgBox.setText("Falla al crear la tabla\n"+consultar.lastError().text());
        msgBox.exec();
    }
    consultar.exec();
    while(consultar.next())
    {
        if(ID == consultar.value("repid").toByteArray().toInt())
        {
            PER.insertRow(fila);
            PER.setRowHeight(fila,20);
            PER.setItem(fila,0,new QTableWidgetItem (consultar.value(0).toByteArray().constData()));
            PER.setItem(fila,1,new QTableWidgetItem (consultar.value(1).toByteArray().constData()));
            PER.setItem(fila,2,new QTableWidgetItem (consultar.value(2).toByteArray().constData()));
            PER.setItem(fila,3,new QTableWidgetItem (consultar.value(3).toByteArray().constData()));
            PER.setItem(fila,4,new QTableWidgetItem (consultar.value(4).toByteArray().constData()));
            PER.setItem(fila,5,new QTableWidgetItem (consultar.value(5).toByteArray().constData()));
            PER.setItem(fila,6,new QTableWidgetItem (consultar.value(6).toByteArray().constData()));
            PER.setItem(fila,7,new QTableWidgetItem (consultar.value(7).toByteArray().constData()));
            PER.setItem(fila,8,new QTableWidgetItem (consultar.value(8).toByteArray().constData()));
            fila ++;
        }
    }

    PER.setColumnWidth(0,50);
    PER.setColumnWidth(1,60);
    PER.setColumnWidth(2,50);
    PER.setColumnWidth(3,70);
    PER.setColumnWidth(4,70);
    PER.setColumnWidth(5,70);
    PER.setColumnWidth(6,50);
    PER.setColumnWidth(7,70);
    PER.setColumnWidth(8,80);
}

void dbManejo::ActualizarInstalaciones(QTableWidget &INS, int ID)
{
    int fila  = 0;
    QSqlQuery consultar;
    INS.setRowCount(0);

    if(!consultar.prepare("SELECT * FROM Instalaciones"))
    {
//        QMessageBox::critical(this,tr("Tabla Instalaciones"),
//                              tr("Falla al leer la tabla\n"
//                                 "%1").arg(consultar.lastError().text()));
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setWindowTitle("Tabla Instalaciones");
        msgBox.setText("Falla al crear la tabla\n"+consultar.lastError().text());
        msgBox.exec();
    }
    consultar.exec();
    while(consultar.next())
    {
        if(ID == consultar.value("repid").toByteArray().toInt())
        {
            INS.insertRow(fila);
            INS.setRowHeight(fila,20);
            INS.setItem(fila,0,new QTableWidgetItem (consultar.value(0).toByteArray().constData()));
            INS.setItem(fila,1,new QTableWidgetItem (consultar.value(1).toByteArray().constData()));
            INS.setItem(fila,2,new QTableWidgetItem (consultar.value(2).toByteArray().constData()));
            INS.setItem(fila,3,new QTableWidgetItem (consultar.value(3).toByteArray().constData()));
            INS.setItem(fila,4,new QTableWidgetItem (consultar.value(4).toByteArray().constData()));
            INS.setItem(fila,5,new QTableWidgetItem (consultar.value(5).toByteArray().constData()));
            INS.setItem(fila,6,new QTableWidgetItem (consultar.value(6).toByteArray().constData()));
            INS.setItem(fila,7,new QTableWidgetItem (consultar.value(7).toByteArray().constData()));
//            INS.setItem(fila,8,new QTableWidgetItem (consultar.value(8).toByteArray().constData()));
            fila ++;
        }
    }

    INS.setColumnWidth(0,50);
    INS.setColumnWidth(1,120);
    INS.setColumnWidth(2,90);
    INS.setColumnWidth(3,100);
    INS.setColumnWidth(4,50);
    INS.setColumnWidth(5,300);
    INS.setColumnWidth(6,80);
    INS.setColumnWidth(7,40);
//    INS.setColumnWidth(8,80);
}
