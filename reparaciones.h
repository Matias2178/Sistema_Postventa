#ifndef REPARACIONES_H
#define REPARACIONES_H

#include <QDialog>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>


namespace Ui {
class Reparaciones;
}

class Reparaciones : public QDialog
{
    Q_OBJECT

public:
    explicit Reparaciones(QWidget *parent = 0);
    ~Reparaciones();


private slots:
    void on_MON_TIPO_activated(int index);

    void on_MON_GUARDAR_clicked();

    void on_MON_BORRAR_clicked();

    void on_SEM_GUARDAR_clicked();

    void on_SEM_BORRAR_clicked();

    void on_MOD_GUARDAR_clicked();

    void on_MOD_BORRAR_clicked();

    void on_GPS_GUARDAR_clicked();

    void on_GPS_BORRAR_clicked();

    void on_CAU_BORRAR_clicked();

    void on_CAU_GUARDAR_clicked();

    void on_RPM_GUARDAR_clicked();

    void on_PANT_ANT_clicked();

    void on_PANT_SIG_clicked();

    void on_SEM_TIPO_activated(const QString &arg1);

    void on_INS_TIPO_activated(const QString &arg1);

    void on_INS_GUARDAR_clicked();

    void LIN_Lectura();

    QByteArray LecturaLIN(QByteArray Datos);

    QByteArray LectASCII(QByteArray Texto);

    void on_MON_TIPO_activated(const QString &arg1);

private:
    Ui::Reparaciones *ui;
    void CambioPantalla(int Pant);
    void BonificacionMsg ();
    void BorraMonitores();
    void BorraSensores();
    void BorraRMP();
    void BorraMOD();
    void BorraGPS();
    void BorraPerifericos();
    void BorraINS();
    bool DobleGuardadoMsg();
    void MensajeTrabajo();

private:

    int SNAnt;
    bool Guardar;
    bool Siguiente;

    int PantallaActual;
    int VSoft;
    bool RPM_TRB;
    QString Patente;
 public:

};

#endif // REPARACIONES_H
