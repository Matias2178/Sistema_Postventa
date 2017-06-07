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

    QByteArray LecturaLIN(QByteArray Datos);

    QByteArray LectASCII(QByteArray Texto);

    void on_MonitoresDatos_clicked(const QModelIndex &index);

    void on_MON_EDITAR_clicked();

    void on_PerifericosDatos_clicked(const QModelIndex &index);

    void on_SEM_EDITAR_clicked();

    void EditarPerifericos();

    void on_MOD_EDITAR_clicked();

    void on_RPM_EDITAR_clicked();

    void on_GPS_EDITAR_clicked();

    void on_CAU_EDITAR_clicked();

    void on_RPM_BORRAR_clicked();

    void BloquearBotones();

    void CargarTrabajos();

    void on_CaudalimetroDatos_clicked(const QModelIndex &index);

    void on_InstalacionesDatos_clicked(const QModelIndex &index);

    void on_INS_EDITAR_clicked();

    void on_INS_BORRAR_clicked();

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
    bool    MonEditar;
    bool    PerEditar;
    bool    InstEditar;
    int     IndEdicion;
    int     IndIndex;
 public:
    void LIN_Lectura();
    void ActualizaDatos();

};

#endif // REPARACIONES_H
