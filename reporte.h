#ifndef REPORTE_H
#define REPORTE_H

#include <QObject>

class Reporte : public QObject
{
    Q_OBJECT
public:
    explicit Reporte(QObject *parent = nullptr);

signals:

public slots:
};

#endif // REPORTE_H