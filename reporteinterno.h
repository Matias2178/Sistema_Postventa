#ifndef REPORTEINTERNO_H
#define REPORTEINTERNO_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QtPrintSupport/QPrinter>
#include <QPainter>
#include <QTextCharFormat>
#include <dbmanejo.h>
#include <QDir>
#include <variables.h>
#include <QDialog>
#include <QFile>

class ReporteInterno : public QObject
{
    Q_OBJECT
public:
    explicit ReporteInterno(QObject *parent = nullptr);
    void SetDatos (int lID);
    void RepInternoPDF(int Id, QString Ruta);

signals:

public slots:

private:
     int Id;
     QString Agente, Reporte, Direccion;
     void Pluma (bool tipo);
     int SaltoPagina(void);
     void DivLinea(int lin);



};

#endif // REPORTEINTERNO_H
