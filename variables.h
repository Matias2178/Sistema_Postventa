#ifndef VARIABLES_H
#define VARIABLES_H
#include <QByteArray>
#include <QStringList>
#include <QSqlRelationalTableModel>
#include <QSortFilterProxyModel>
#define Ver1    5
#define Ver2    100
#define Ver3    100
#define FVer    "Marzo 2019"

 extern int IDSensores;

//Variables de uso en la comunicacion lin
 extern bool Escribir;
 extern bool Garantias;
 extern int EIndice;
 extern int LIndice;

 extern unsigned long NSerie;
 extern unsigned long Antiguedad, AuxAntiguedad;

// extern QStringList MonMascaras;

 extern int IdReparacion;

 extern QByteArray DatosLin;
 extern QByteArray Lectura;
 extern QString AgenteResp;

 extern QString RutaBaseDatos;
 extern QString RutaInfoPDF;
 extern QString RutaInfoExcel;



#endif // VARIABLES_H
