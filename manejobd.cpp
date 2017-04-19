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
                "sn     INTEGER,"
                "vsoft  VARCHAR[10],"
                "falla  VARCHAR[50],"
                "bonif  INTEGER,"
                "obs    VARCHAR[50],"
                "frep   VARCHAR[10],"
                "repid   INTEGER"
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
                "sn     INTEGER,"
                "falla  VARCHAR[50],"
                "bonif  INTEGER,"
                "obs    VARCHAR[50],"
                "frep   VARCHAR[10],"
                "repid  INGEGER"
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
                "repid  INGEGER"
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
    ui->IngresoEquipo->clear();
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
           qDebug () << "MonMascaras"<< MonMascaras;
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


    ui->Ingreso_Equipo->addItems(Lista4);
    ui->IngresoEquipo->addItems(Lista4);
    ui->MON_TIPO->addItems(Lista1);
    ui->S_TIPO->addItems(Lista2);
    ui->INS_TIPO->addItems(Lista3);

    CargarDatos(*ui->CAU_FALLAS,"SCC");     //Cargo fallas del caudalimetro
    CargarDatos(*ui->GPS_FALLAS,"GPS");     //Cargo fallas del GPS
    CargarDatos(*ui->MOD_FALLAS,"MOD");     //Cargo fallas Moduladora
    CargarDatos(*ui->RPM_FALLAS,"RPM");     //Cargo fallas de los sensores de RPM y Velocidad
    CargarRecepcion();

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
       FALLAS.setRowHeight(fila,20);
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
               FALLAS.setRowHeight(fila,20);
               FALLAS.setItem(fila,0,new QTableWidgetItem(Falla) );
               FALLAS.item(fila,0)->setCheckState(Qt::Unchecked);
             //  ui->MON_FALLAS->setColumnWidth(0,100);
           }
           fila ++;
       }
}

void MainWindow::CargarRecepcion()
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
    ui->AgenteNombre->clear();
    ui->TrabajoAgente->clear();

    while(consultar.next())
    {
        Lista1.append(consultar.value(1).toByteArray().constData());
        fila ++;
    }

    ui->AgenteNombre->addItems(Lista1);
    ui->TrabajoAgente->addItems(Lista1);
}

void MainWindow::IngresoActualizar(int ID)
{
    QString Conf;

    qDebug()<< "ID:" << ID;
    if (!ID)
        return;
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

    ui->DatosIngreso->setRowCount(0);

    while(consultar.next())
    {
   //     qDebug () << "ID consultaRRR:" << consultar.value("repid").toByteArray().toInt();
   //     qDebug () << "ID consulta:" << consultar.value(4).toByteArray().toInt();
        if(ID == consultar.value("repid").toByteArray().toInt())
        {
            ui->DatosIngreso->insertRow(fila);
            ui->DatosIngreso->setRowHeight(fila,20);
            ui->DatosIngreso->setItem(fila,0,new QTableWidgetItem (consultar.value(0).toByteArray().constData()));
            ui->DatosIngreso->setItem(fila,1,new QTableWidgetItem (consultar.value(1).toByteArray().constData()));
            ui->DatosIngreso->setItem(fila,2,new QTableWidgetItem (consultar.value(2).toByteArray().constData()));
            ui->DatosIngreso->setItem(fila,3,new QTableWidgetItem (consultar.value(3).toByteArray().constData()));
            ui->DatosIngreso->setItem(fila,4,new QTableWidgetItem (consultar.value(4).toByteArray().constData()));
            fila ++;
        }
    }
    ui->DatosIngreso->setColumnWidth(0,50);
    ui->DatosIngreso->setColumnWidth(1,100);
    ui->DatosIngreso->setColumnWidth(2,50);
    ui->DatosIngreso->setColumnWidth(3,200);
    ui->DatosIngreso->setColumnWidth(4,50);
}

void MainWindow::ReparacionesActualizar(const QString &arg1)
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

    ui->DatosReparaciones->setRowCount(0);
    while(consultar.next())
    {
        if((arg1 == consultar.value(1).toByteArray().constData())|| todos )
        {
            ui->DatosReparaciones->insertRow(fila);
            ui->DatosReparaciones->setRowHeight(fila,20);
            ui->DatosReparaciones->setItem(fila,0,new QTableWidgetItem (consultar.value(0).toByteArray().constData()));
            ui->DatosReparaciones->setItem(fila,1,new QTableWidgetItem (consultar.value(1).toByteArray().constData()));
            ui->DatosReparaciones->setItem(fila,2,new QTableWidgetItem (consultar.value(2).toByteArray().constData()));
            ui->DatosReparaciones->setItem(fila,3,new QTableWidgetItem (consultar.value(3).toByteArray().constData()));
            fila ++;
        }
    }
    ui->DatosReparaciones->setColumnWidth(0,25);
    ui->DatosReparaciones->setColumnWidth(1,200);
    ui->DatosReparaciones->setColumnWidth(2,80);
    ui->DatosReparaciones->setColumnWidth(3,80);
}

void MainWindow::MonitoresActualizar()
{
    QString Conf;

//    qDebug()<< "ID:" << ID;
//    if (!ID)
//        return;
    Conf.append("SELECT * FROM Monitores");

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

    ui->MonitoresDatos->setRowCount(0);

    while(consultar.next())
    {
        qDebug () << "ID consulta:" << consultar.value("repid").toByteArray().toInt();

        if(RepID == consultar.value("repid").toByteArray().toInt())
        {
            ui->MonitoresDatos->insertRow(fila);
            ui->MonitoresDatos->setRowHeight(fila,20);
            ui->MonitoresDatos->setItem(fila,0,new QTableWidgetItem (consultar.value(0).toByteArray().constData()));
            ui->MonitoresDatos->setItem(fila,1,new QTableWidgetItem (consultar.value(1).toByteArray().constData()));
            ui->MonitoresDatos->setItem(fila,2,new QTableWidgetItem (consultar.value(2).toByteArray().constData()));
            ui->MonitoresDatos->setItem(fila,3,new QTableWidgetItem (consultar.value(3).toByteArray().constData()));
            ui->MonitoresDatos->setItem(fila,4,new QTableWidgetItem (consultar.value(4).toByteArray().constData()));
            ui->MonitoresDatos->setItem(fila,5,new QTableWidgetItem (consultar.value(5).toByteArray().constData()));
            ui->MonitoresDatos->setItem(fila,6,new QTableWidgetItem (consultar.value(6).toByteArray().constData()));
            ui->MonitoresDatos->setItem(fila,7,new QTableWidgetItem (consultar.value(7).toByteArray().constData()));
            ui->MonitoresDatos->setItem(fila,8,new QTableWidgetItem (consultar.value(8).toByteArray().constData()));
            fila ++;
        }
    }
    ui->MonitoresDatos->setColumnWidth(0,50);
    ui->MonitoresDatos->setColumnWidth(1,100);
    ui->MonitoresDatos->setColumnWidth(2,50);
    ui->MonitoresDatos->setColumnWidth(3,50);
    ui->MonitoresDatos->setColumnWidth(4,100);
    ui->MonitoresDatos->setColumnWidth(5,50);
    ui->MonitoresDatos->setColumnWidth(6,50);
    ui->MonitoresDatos->setColumnWidth(7,100);
    ui->MonitoresDatos->setColumnWidth(8,50);
}

void MainWindow::PerifericosActualizar()
{
    QString Conf;

//    qDebug()<< "ID:" << ID;
//    if (!ID)
//        return;
    Conf.append("SELECT * FROM Perifericos");

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

    ui->PerifericosDatos->setRowCount(0);

    while(consultar.next())
    {
   //    qDebug () << "ID consulta:" << consultar.value("repid").toByteArray().toInt();

        if(RepID == consultar.value("repid").toByteArray().toInt())
        {
            ui->PerifericosDatos->insertRow(fila);
            ui->PerifericosDatos->setRowHeight(fila,20);
            ui->PerifericosDatos->setItem(fila,0,new QTableWidgetItem (consultar.value(0).toByteArray().constData()));
            ui->PerifericosDatos->setItem(fila,1,new QTableWidgetItem (consultar.value(1).toByteArray().constData()));
            ui->PerifericosDatos->setItem(fila,2,new QTableWidgetItem (consultar.value(2).toByteArray().constData()));
            ui->PerifericosDatos->setItem(fila,3,new QTableWidgetItem (consultar.value(3).toByteArray().constData()));
            ui->PerifericosDatos->setItem(fila,4,new QTableWidgetItem (consultar.value(4).toByteArray().constData()));
            ui->PerifericosDatos->setItem(fila,5,new QTableWidgetItem (consultar.value(5).toByteArray().constData()));
            ui->PerifericosDatos->setItem(fila,6,new QTableWidgetItem (consultar.value(6).toByteArray().constData()));
            ui->PerifericosDatos->setItem(fila,7,new QTableWidgetItem (consultar.value(7).toByteArray().constData()));
            ui->PerifericosDatos->setItem(fila,8,new QTableWidgetItem (consultar.value(8).toByteArray().constData()));
            ui->PerifericosDatos->setItem(fila,9,new QTableWidgetItem (consultar.value(9).toByteArray().constData()));
            ui->PerifericosDatos->setItem(fila,10,new QTableWidgetItem (consultar.value(10).toByteArray().constData()));
            ui->PerifericosDatos->setItem(fila,11,new QTableWidgetItem (consultar.value(11).toByteArray().constData()));
            ui->PerifericosDatos->setItem(fila,12,new QTableWidgetItem (consultar.value(12).toByteArray().constData()));
//            ui->PerifericosDatos->setItem(fila,13,new QTableWidgetItem (consultar.value(13).toByteArray().constData()));
            fila ++;
        }
    }
    ui->PerifericosDatos->setColumnWidth(0,50);
    ui->PerifericosDatos->setColumnWidth(1,100);
    ui->PerifericosDatos->setColumnWidth(2,80);
    ui->PerifericosDatos->setColumnWidth(3,70);
    ui->PerifericosDatos->setColumnWidth(4,70);
    ui->PerifericosDatos->setColumnWidth(5,40);
    ui->PerifericosDatos->setColumnWidth(6,70);
    ui->PerifericosDatos->setColumnWidth(7,100);
    ui->PerifericosDatos->setColumnWidth(8,100);
    ui->PerifericosDatos->setColumnWidth(9,40);
    ui->PerifericosDatos->setColumnWidth(10,100);
    ui->PerifericosDatos->setColumnWidth(11,70);
    ui->PerifericosDatos->setColumnWidth(12,50);
//    ui->PerifericosDatos->setColumnWidth(13,40);
}
void MainWindow::InstalacionesActualizar()
{
    QString Conf;

    Conf.append("SELECT * FROM Instalaciones");

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

    ui->InstalacionesDatos->setRowCount(0);
    qDebug () << "RepID:" << RepID;
    while(consultar.next())
    {
       qDebug () << "ID consulta:" << consultar.value("repid").toByteArray().toInt();

        if(RepID == consultar.value("repid").toByteArray().toInt())
        {
            ui->InstalacionesDatos->insertRow(fila);
            ui->InstalacionesDatos->setRowHeight(fila,20);
            ui->InstalacionesDatos->setItem(fila,0,new QTableWidgetItem (consultar.value(0).toByteArray().constData()));
            ui->InstalacionesDatos->setItem(fila,1,new QTableWidgetItem (consultar.value(1).toByteArray().constData()));
            ui->InstalacionesDatos->setItem(fila,2,new QTableWidgetItem (consultar.value(2).toByteArray().constData()));
            ui->InstalacionesDatos->setItem(fila,3,new QTableWidgetItem (consultar.value(3).toByteArray().constData()));
            ui->InstalacionesDatos->setItem(fila,4,new QTableWidgetItem (consultar.value(4).toByteArray().constData()));
            ui->InstalacionesDatos->setItem(fila,5,new QTableWidgetItem (consultar.value(5).toByteArray().constData()));
            ui->InstalacionesDatos->setItem(fila,6,new QTableWidgetItem (consultar.value(6).toByteArray().constData()));
            ui->InstalacionesDatos->setItem(fila,7,new QTableWidgetItem (consultar.value(7).toByteArray().constData()));
/*            ui->PerifericosDatos->setItem(fila,8,new QTableWidgetItem (consultar.value(8).toByteArray().constData()));
            ui->PerifericosDatos->setItem(fila,9,new QTableWidgetItem (consultar.value(9).toByteArray().constData()));
            ui->PerifericosDatos->setItem(fila,10,new QTableWidgetItem (consultar.value(10).toByteArray().constData()));
            ui->PerifericosDatos->setItem(fila,11,new QTableWidgetItem (consultar.value(11).toByteArray().constData()));
            ui->PerifericosDatos->setItem(fila,12,new QTableWidgetItem (consultar.value(12).toByteArray().constData()));
//            ui->PerifericosDatos->setItem(fila,13,new QTableWidgetItem (consultar.value(13).toByteArray().constData()));
*/
            fila ++;
        }
    }
    ui->PerifericosDatos->setColumnWidth(0,50);
    ui->PerifericosDatos->setColumnWidth(1,100);
    ui->PerifericosDatos->setColumnWidth(2,80);
    ui->PerifericosDatos->setColumnWidth(3,70);
    ui->PerifericosDatos->setColumnWidth(4,70);
    ui->PerifericosDatos->setColumnWidth(5,40);
    ui->PerifericosDatos->setColumnWidth(6,70);
    ui->PerifericosDatos->setColumnWidth(7,100);
//    ui->PerifericosDatos->setColumnWidth(8,100);
//    ui->PerifericosDatos->setColumnWidth(9,40);
//    ui->PerifericosDatos->setColumnWidth(10,100);
//    ui->PerifericosDatos->setColumnWidth(11,70);
//    ui->PerifericosDatos->setColumnWidth(12,50);
////    ui->PerifericosDatos->setColumnWidth(13,40);
}
