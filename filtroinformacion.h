#ifndef FILTROINFORMACION_H
#define FILTROINFORMACION_H

#include <QDialog>
#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

namespace Ui {
class FiltroInformacion;
}

class FiltroInformacion : public QDialog
{
    Q_OBJECT

public:
    explicit FiltroInformacion(QWidget *parent = nullptr);
    ~FiltroInformacion();

private slots:
    void on_bFechaInicio_clicked();

    void on_bFechaFin_clicked();

    void ActFechaInicio();
    void ActFechaFin();

    void on_FiltrarDatos_clicked();
    
private:
    Ui::FiltroInformacion *ui;
    QString FechaInicio;
    QString FechaFin;
};

#endif // FILTROINFORMACION_H
