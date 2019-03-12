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


class ReporteInterno : public QObject
{
    Q_OBJECT
public:
    explicit ReporteInterno(QObject *parent = nullptr);
    void SetDatos (int lID);
    void RepInternoPDF(int Id);

signals:

public slots:

private:
     int Id;
     QString Agente, Reporte;
     void Pluma (bool tipo);
     int SaltoPagina(void);
     void DivLinea(int lin);



};

#endif // REPORTEINTERNO_H
