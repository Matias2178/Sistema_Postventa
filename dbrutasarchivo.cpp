#include "dbrutasarchivo.h"
#include <QDir>
#include <QMessageBox>
#include <QDebug>
#include <archivos.h>

QSqlDatabase dbRutasArchivo::dbRuta = QSqlDatabase::addDatabase("QSQLITE");


dbRutasArchivo::dbRutasArchivo()
{

}


void dbRutasArchivo::Cerrar()
{
    dbRuta.close();
}

bool dbRutasArchivo::Abrir()
{
    QDir Direccion;
    QString Ruta;


    Ruta.clear();
    Ruta.append(Direccion.currentPath() + "/Info.sqlite");

    dbRuta = QSqlDatabase::addDatabase("QSQLITE");

    dbRuta.setDatabaseName(Ruta);
    dbRuta.open();
    QSqlQuery crear;
    crear.prepare("SELECT * FROM Direcciones");

    if(!crear.exec())
    {
        Ruta.clear();
        Ruta.append("CREATE TABLE IF NOT EXISTS Direcciones("
                    "id         INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,"
                    "Archivo	TEXT ,"
                    "Ruta       TEXT "
                    ");");

        crear.prepare(Ruta);
        if(!crear.exec())
        {
            QMessageBox msgBox;
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.setWindowTitle("Tabla Direcciones");
            msgBox.setText("Falla al crear la tabla\n"+crear.lastError().text());
            msgBox.exec();
        }

        crear.prepare("INSERT INTO Direcciones(Archivo) "
                      "VALUES(:Archivo)");
        crear.bindValue(":Archivo", "dbReparaciones" );
        crear.exec();

        crear.prepare("INSERT INTO Direcciones (Archivo) "
                      "VALUES(:Archivo)");
        crear.bindValue(":Archivo", "InformePDF" );
        crear.exec();

        crear.prepare("INSERT INTO Direcciones(Archivo) "
                      "VALUES(:Archivo)");
        crear.bindValue(":Archivo", "InformeRep" );
        crear.exec();

        return true;
    }
    return false;
}

QString dbRutasArchivo::Buscar(QString Tipo)
{
    QSqlQuery buscar;
    buscar.prepare("SELECT * FROM Direcciones WHERE Archivo == "
                    "'" +Tipo + "'");
//    buscar.prepare("SELECT * FROM Direcciones");
    buscar.exec();
    buscar.next();
//    qDebug () <<  buscar.value(2).toByteArray().constData();
//    qDebug () << buscar.lastError().text();
    return buscar.value(2).toString();


}

//Retorna la base de datos que se creo
QSqlDatabase dbRutasArchivo::dbRetorna()
{
    QDir Direccion;
    QString Ruta;

    Ruta.clear();
    Ruta.append(Direccion.currentPath() + "/Info.sqlite");

    dbRuta = QSqlDatabase::addDatabase("QSQLITE");

    dbRuta.setDatabaseName(Ruta);
    dbRuta.open();
    return dbRuta;
}

bool dbRutasArchivo::Actualizar(QString Tipo,QString Ruta1)
{
    QSqlQuery actualiza;
    QString Conf;
    Conf.append("UPDATE Direcciones SET "
                "Ruta ="
                "'"+Ruta1+"'"
                " WHERE Archivo ="
                "'"+Tipo+"'"
                "");
    actualiza.prepare(Conf);
    actualiza.exec();
    if(!actualiza.next())
    {
        qDebug () << Tipo << "...." << actualiza.lastError().text();
        return false;
    }
    return true;
//    qDebug () <<  buscar.value(2).toByteArray().constData();

//    return buscar.value(2).toString();

}
