#ifndef VARIABLES_H
#define VARIABLES_H
#include <QByteArray>
#include <QStringList>

#define Ver1    3
#define Ver2    10
#define Ver3 17

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
