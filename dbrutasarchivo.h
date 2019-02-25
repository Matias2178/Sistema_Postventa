#ifndef DBRUTASARCHIVO_H
#define DBRUTASARCHIVO_H
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDialog>

class dbRutasArchivo
{
public:
    dbRutasArchivo();
    void Cerrar();
    bool Abrir();
    QString Buscar(QString Tipo);
    QString Ruta(QString Tipo);
    static QSqlDatabase dbRetorna();
    bool Actualizar(QString Tipo,QString Ruta1);


    static QSqlDatabase dbRuta;

};


#endif // DBRUTASARCHIVO_H
