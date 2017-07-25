#ifndef DBMANEJO_H
#define DBMANEJO_H

#include <QTableWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDialog>

class dbManejo
{
public:
    static QSqlDatabase dbRetorna();
    void dbAbrirCrear();
    void CrearProductos();
    void CrearFallas();
    void CrearAgentes();
    void CrearOperario();
    void CrearReparaciones();
    void CrearMonitores();
    void CrearPerifericos();
    void CrearCaudalimetro();
    void CrearInstalaciones();
    void CrearIngreso();

    void  LeerProductos();

    void  ActualizarMonitores(QTableWidget &PER, int ID);
    void  ActualizarPerifericos(QTableWidget &PER, int ID);
    void  ActualizarCaudalimetro(QTableWidget &SCC, int ID);
    void  ActualizarInstalaciones(QTableWidget &INS, int ID);

    void CargarFallas(QTableWidget &FALLAS,QString Tipo);
    void CargarProd(QTableWidget &PROD,int Tipo);
    QStringList CargarProductos(int Tipo);

    void CargarReparaciones(QTableWidget &FALLAS,QString Tipo);
    void CargarIngreso(QTableWidget &FALLAS,int ID);
    void BorrarItem(QString Tabla,int Item);

    static QSqlDatabase db;

private:

};

#endif // DBMANEJO_H
