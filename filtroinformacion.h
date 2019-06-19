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

    void ActualizaIngreso(int id);

    void on_Reportes_clicked();

    void on_ReportesFecha_clicked();

    void on_IngresoFecha_clicked();

    void on_Ingresos_clicked();

    void GeneraReporte(QString comando, bool rep);

    void AjustaReporte();

    void ActualizarReporte();

    void GenerarIngreso(QString comando);

    void AjustaIngreso();

    void on_ReportesExcel_clicked();

    void on_IngresoExcel_clicked();

    void on_AjusteFechas_clicked();

    QString CambioFecha(QString fecha);


private:
    Ui::FiltroInformacion *ui;
    QString FechaInicio;
    QString FechaFin;
    QStringList DatosArchivo;
};

#endif // FILTROINFORMACION_H
