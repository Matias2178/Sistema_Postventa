#ifndef FILTROINFORMACION_H
#define FILTROINFORMACION_H

#include <QDialog>
#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>
#include <QFile>
#include <QDir>

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
    
    void on_ExportarExcel_clicked();
    
    void on_Ingresos_clicked();

    
private:
    Ui::FiltroInformacion *ui;
    QString FechaInicio;
    QString FechaFin;
    QStringList DatosArchivo;
};

#endif // FILTROINFORMACION_H
