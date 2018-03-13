#ifndef VARIABLES_H
#define VARIABLES_H
#include <QByteArray>
#include <QStringList>
#include <QSqlRelationalTableModel>
#include <QSortFilterProxyModel>
#define Ver1    4
#define Ver2    248
#define Ver3    375

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



#endif // VARIABLES_H
