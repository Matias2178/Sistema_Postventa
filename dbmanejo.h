#ifndef DBMANEJO_H
#define DBMANEJO_H

#include <QTableWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDialog>
#include <QList>

class dbManejo
{
public:
    static QSqlDatabase dbRetorna();
    bool dbAbrirCrear();
    void Cerrardb();

    void  ActualizarMonitores(QTableWidget &PER, int ID);
    void  ActualizarPerifericos(QTableWidget &PER, int ID);
    void  ActualizarCaudalimetro(QTableWidget &SCC, int ID);
    void  ActualizarInstalaciones(QTableWidget &INS, int ID);
    void  ActualizarInsumos(QTableWidget &INSUMOS, int ID);
    int   BucaEquipo(QString Equipo );


    void CargarFallas(QTableWidget &FALLAS,QString Tipo);
    void CargarProd(QTableWidget &PROD,int Tipo);

    void CargarIngreso(QTableWidget &FALLAS,int ID);
    void BorrarItem(QString Tabla,int Item);
    bool BorrarTrabajo(int id);
    int Diferencia (void);

 //   void SetNombreDb(QString Nombre);
    void SetDirDb(QString Dir);

   QString Mascara(QString Equipo);

    void ClrDirDb(void);

    static QSqlDatabase db;

private:
    QString Direccion;
};

#endif // DBMANEJO_H
