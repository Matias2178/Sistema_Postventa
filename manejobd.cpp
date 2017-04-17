#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>


void MainWindow::dbAbrirCrear()
{
    // Creo/abro una base de datos
        QString nombre;
        nombre.append("PostVenta.sqlite");
        db = QSqlDatabase::addDatabase("QSQLITE");

        db.setDatabaseName(nombre);
     //   qDebug () << nombre;
        if (!db.open())
        {
            qDebug() << "Error de apertura" ;
        }
        else
        {
            qDebug() << "Creacion DB OK:" ;
        }
}

void MainWindow::ProductosCrear()
{
    QString Conf;
    Conf.append("CREATE TABLE IF NOT EXISTS Productos("
                 "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                 "producto VARCHAR[30],"
                 "version VARCHAR[30],"
                 "tipo INTEGER NOT NULL"
                 ");");

    QSqlQuery crear;
    crear.prepare(Conf);
    if(!crear.exec())
    {
        qDebug() << "Error Creacion tabla Productos" << crear.lastError();
    }
    else
    {
        qDebug() << "Creacion Tablas Prod ok";

    }
}
void MainWindow::FallasCrear()
{
    QString Conf;
    Conf.append("CREATE TABLE IF NOT EXISTS Fallas("
                 "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                 "producto VARCHAR[30],"
                 "falla VARCHAR[10],"
                 "descripcion VARCHAR[50]"
                 ");");

    QSqlQuery crear;
    crear.prepare(Conf);
    if(!crear.exec())
    {
        qDebug() << "Error Creacion tabla Fallas" << crear.lastError();
    }
    else
    {
        qDebug() << "Creacion Tabla Fallas Ok";

    }
}
void MainWindow::AgentesCrear()
{
    QString Conf;
    Conf.append("CREATE TABLE IF NOT EXISTS Agente("
                 "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                 "agente VARCHAR[30]"
                 ");");

    QSqlQuery crear;
    crear.prepare(Conf);
    if(!crear.exec())
    {
        qDebug() << "Error Creacion tabla Agente" << crear.lastError();
    }
    else
    {
        qDebug() << "Creacion Tabla Agente Ok";

    }
}
void MainWindow::ReparacionesCrear()
{
    QString Conf;
    Conf.append("CREATE TABLE IF NOT EXISTS Reparaciones("
                "id     INTEGER PRIMARY KEY AUTOINCREMENT,"
                "agente VARCHAR[30],"
                "fing   VARCHAR[10],"
                "frep   VARCHAR[10]"
                 ");");

    QSqlQuery crear;
    crear.prepare(Conf);
    if(!crear.exec())
    {
        qDebug() << "Error Creacion tabla Reparacion" << crear.lastError();
    }
    else
    {
        qDebug() << "Creacion Tabla Reparacion Ok";

    }
}
void MainWindow::MonitoresCrear()
{
    QString Conf;
    Conf.append("CREATE TABLE IF NOT EXISTS Monitores("
                "id     INTEGER PRIMARY KEY AUTOINCREMENT,"
                "nombre VARCHAR[30],"
                "sn     UNSIGNED LONG,"
                "vsoft  VARCHAR[10],"
                "falla  VARCHAR[50],"
                "bonif  INTEGER,"
                "obs    VARCHAR[50],"
                "frep   VARCHAR[10],"
                "crep   INTEGER"
                 ");");

    QSqlQuery crear;
    crear.prepare(Conf);
    if(!crear.exec())
    {
        qDebug() << "Error Creacion tabla Monitores" << crear.lastError();
    }
    else
    {
        qDebug() << "Creacion Tabla Monitores Ok";

    }
}
void MainWindow::PerifericosCrear()
{
    QString Conf;
    Conf.append("CREATE TABLE IF NOT EXISTS Perifericos("
                "id     INTEGER PRIMARY KEY AUTOINCREMENT,"
                "nombre VARCHAR[30],"
                "sn     UNSIGNED LONG,"
                "ffab   VARCHAR[10],"
                "finst  VARCHAR[10],"
                "vsoft  VARCHAR[10],"
                "fsoft  VARCHAR[10],"
                "conf   VARCHAR[50],"
                "falla  VARCHAR[50],"
                "bonif  INTEGER,"
                "obs    VARCHAR[50],"
                "frep   VARCHAR[10],"
                "crep   INTEGER"
                 ");");

    QSqlQuery crear;
    crear.prepare(Conf);
    if(!crear.exec())
    {
        qDebug() << "Error Creacion tabla Perifericos" << crear.lastError();
    }
    else
    {
        qDebug() << "Creacion Tabla Perifericos Ok";

    }
}

void MainWindow::InstalacionesCrear()
{
    QString Conf;
    Conf.append("CREATE TABLE IF NOT EXISTS Instalaciones("
                "id     INTEGER PRIMARY KEY AUTOINCREMENT,"
                "nombre VARCHAR[30],"
                "sn     UNSIGNED LONG,"
                "falla  VARCHAR[50],"
                "bonif  INTEGER,"
                "obs    VARCHAR[50],"
                "frep   VARCHAR[10],"
                "crep   INGEGER"
                 ");");

    QSqlQuery crear;
    crear.prepare(Conf);
    if(!crear.exec())
    {
        qDebug() << "Error Creacion tabla Instalaciones" << crear.lastError();
    }
    else
    {
        qDebug() << "Creacion Tabla Instalaciones Ok";

    }
}

void MainWindow::IngresoCrear()
{
    QString Conf;
    Conf.append("CREATE TABLE IF NOT EXISTS Ingreso("
                "id     INTEGER PRIMARY KEY AUTOINCREMENT,"
                "nombre VARCHAR[30],"
                "cant   INTEGER,"
                "obs    VARCHAR[20],"
                "crep   INGEGER"
                 ");");

    QSqlQuery crear;
    crear.prepare(Conf);
    if(!crear.exec())
    {
        qDebug() << "Error Creacion tabla Ingreso" << crear.lastError();
    }
    else
    {
        qDebug() << "Creacion Tabla Ingreso Ok";

    }
}
void MainWindow::ProductosLeer()
{
    QString Conf;
    Conf.append("SELECT * FROM Productos");

    QSqlQuery consultar;
    consultar.prepare(Conf);
    if(!consultar.exec())
    {
        qDebug() << "Falla leer productos:" << consultar.lastError();
    }
    else
    {
        qDebug() << "Leer Productos Ok";

    }
    int fila  = 0;
    int tipo;
    bool ok;
    QStringList Lista1 , Lista2, Lista3, Lista4;

    Lista1.clear();
    Lista2.clear();
    Lista3.clear();
    Lista4.clear();
    MonMascaras.clear();
    MonMascaras.append("");
    Lista1.append("Seleccionar");
    Lista2.append("Seleccionar");
    Lista3.append("Seleccionar");
    Lista4.append("Seleccionar");
    ui->Ingreso_Equipo->clear();
    ui->MON_TIPO->clear();
    ui->S_TIPO->clear();
    ui->INS_TIPO->clear();

    while(consultar.next())
    {
        Lista4.append(consultar.value(1).toByteArray().constData());
        tipo = consultar.value(3).toInt(&ok);
        if(tipo == 1)
        {
           Lista1.append(consultar.value(1).toByteArray().constData());
           MonMascaras.append(consultar.value(2).toByteArray().constData());
        }
        else if (tipo == 2)
        {
            Lista2.append(consultar.value(1).toByteArray().constData());
        }
        else if (tipo == 3)
        {
            Lista3.append(consultar.value(1).toByteArray().constData());
        }
        fila ++;
    }
//    qDebug() << MonMascaras;
//    qDebug() << Lista4;
//    qDebug() << Lista1;
//    qDebug() << Lista2;
//    qDebug() << Lista3;

    ui->Ingreso_Equipo->addItems(Lista4);
    ui->MON_TIPO->addItems(Lista1);
    ui->S_TIPO->addItems(Lista2);
    ui->INS_TIPO->addItems(Lista3);

    CargarDatos(*ui->CAU_FALLAS,"SCC");     //Cargo fallas del caudalimetro
    CargarDatos(*ui->GPS_FALLAS,"GPS");     //Cargo fallas del GPS
    CargarDatos(*ui->MOD_FALLAS,"MOD");     //Cargo fallas Moduladora
    CargarDatos(*ui->RPM_FALLAS,"RPM");     //Cargo fallas de los sensores de RPM y Velocidad

}

void MainWindow::CargarDatos(QTableWidget &FALLAS,QString Tipo)
{
       QString Conf;
       QSqlQuery consultar;
       QString Falla;
       int fila;

       Conf.clear();
       Conf.append("SELECT * FROM Fallas");

       consultar.prepare(Conf);
       if(!consultar.exec())
       {
          qDebug() << "Falla Func leer Tabla Fallas:" << consultar.lastError();
       }
       else
       {
           qDebug() << "Lect Func Fallas ok";

       }

       FALLAS.setRowCount(0);
       fila = FALLAS.rowCount();

       FALLAS.setHorizontalHeaderItem(0, new QTableWidgetItem("Fallas_1"));

       FALLAS.insertRow(fila);
       FALLAS.setItem(fila,0,new QTableWidgetItem("Fun_OK"));
       FALLAS.item(fila,0)->setCheckState(Qt::Unchecked);

       while(consultar.next())
       {
           Falla.clear();
           Falla.append(consultar.value(1).toByteArray().constData());
           if(Falla == Tipo)
           {
               Falla.clear();
               Falla.append(consultar.value(2).toByteArray().constData());
          //     qDebug () << "Falla: " << Falla;
               fila = FALLAS.rowCount();
               FALLAS.setRowHeight(fila,10);
               FALLAS.insertRow(fila);
               FALLAS.setItem(fila,0,new QTableWidgetItem(Falla) );
               FALLAS.item(fila,0)->setCheckState(Qt::Unchecked);
             //  ui->MON_FALLAS->setColumnWidth(0,100);
           }
           fila ++;
       }
}
