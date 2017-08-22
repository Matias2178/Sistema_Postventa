#ifndef VARIABLES_H
#define VARIABLES_H
#include <QByteArray>
#include <QStringList>
#include <QSqlRelationalTableModel>
#include <QSortFilterProxyModel>
#define Ver1    4
#define Ver2    156
#define Ver3    245

 extern int IDSensores;
 extern int gTrabajoIdReparacion;

//Variables de uso en la comunicacion lin
 extern bool Escribir;
 extern int EIndice;
 extern int LIndice;

 extern unsigned long NSerie;

 extern QStringList MonMascaras;

 extern int IdReparacion;

 extern QByteArray DatosLin;
 extern QByteArray Lectura;


#endif // VARIABLES_H
