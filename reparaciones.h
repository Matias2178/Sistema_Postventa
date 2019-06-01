#ifndef REPARACIONES_H
#define REPARACIONES_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRelationalTableModel>
#include <QSortFilterProxyModel>
#include <reparacioneseditar.h>
#include <QDateTime>
#include <user.h>


namespace Ui {
class Reparaciones;
}

class Reparaciones : public QDialog
{
    Q_OBJECT

public:
    explicit Reparaciones(QWidget *parent = 0);
    ~Reparaciones();
    QSqlRelationalTableModel    *ModTrabTablaRep;
    QSortFilterProxyModel       *FilTrabTablaRep;

    QSqlRelationalTableModel    *ModEquipos;
    QSortFilterProxyModel       *FiltEquipos;


private slots:

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

    void on_INS_GUARDAR_clicked();

    QByteArray LecturaLIN(QByteArray Datos);

    QByteArray LectASCII(QByteArray Texto);

    void Garantias();

    void on_MonitoresDatos_clicked(const QModelIndex &index);

    void on_MON_EDITAR_clicked();

    void on_PerifericosDatos_clicked(const QModelIndex &index);

    void on_SEM_EDITAR_clicked();

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

    void on_RepTrabajo_clicked(const QModelIndex &index);

    void on_Prod_Per_clicked(const QModelIndex &index);

    void on_Prod_Mon_clicked(const QModelIndex &index);

    void on_Prod_Ins_clicked(const QModelIndex &index);

    void ActDatos();

    void on_ProdInsumos_clicked(const QModelIndex &index);

    void on_InsumosDatos_clicked(const QModelIndex &index);

    void on_InsumosGuardar_clicked();

    void on_InsumosEditar_clicked();

    void on_InsumosBorrar_clicked();

private:
    Ui::Reparaciones *ui;
    void CambioPantalla(int Pant);
    void BonificacionMsg ();
    bool DobleGuardadoMsg();
    void MensajeTrabajo();
    bool SenDuplicado(QString Nombre, QString SN, QString RepId);
    bool MonDuplicado(QString Nombre, QString SN, QString RepId);
    bool InsDuplicado(QString Nombre, QString SN, QString RepId);
    bool CauDuplicado(QString Nombre, QString SN, QString RepId);

    dbManejo dbReparaciones;
    QDateTime fReparaciones;
    User tFallas;


private:

    int SNAnt;

    int     PantallaActual;
    int     VSoft;
    bool    RPM_TRB;
    QString Patente;
    bool    MonEditar;
    bool    PerEditar;
    bool    InstEditar;
    int     IndEdicion;
    int     IndIndex;
    float   Factork;
    int     IndexCau;

 public:
    void LIN_Lectura();
    void ActualizaDatos();

};

#endif // REPARACIONES_H
