#ifndef INGRESO_H
#define INGRESO_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRelationalTableModel>
#include <QSortFilterProxyModel>
#include <QCompleter>
#include <dbmanejo.h>
#include <QDateTime>
#include <ingresoreparaciones.h>


namespace Ui {
class Ingreso;
}

class Ingreso : public QDialog
{
    Q_OBJECT

public:
    explicit Ingreso(QWidget *parent = 0);
    ~Ingreso();

    QSqlRelationalTableModel    *ModAgentes;
    QSortFilterProxyModel       *FiltAgentes;

    QSqlRelationalTableModel    *ModEquipos;
    QSortFilterProxyModel       *FilEquipos;

    QSqlRelationalTableModel    *ModRepDatos;
    QSortFilterProxyModel       *FilRepDatos;

    QSqlRelationalTableModel    *ModProdDatos;
    QSortFilterProxyModel       *FilProdDatos;

    QCompleter *CompleterAgente;



private slots:
    void on_RepIniciar_clicked();

    void on_RepEditar_clicked();

    void on_RepBorrar_clicked();

    void on_IngGuardar_clicked();

    void on_IngEditar_clicked();

    void on_IngBorrar_clicked();

    void on_IngresoTabla_clicked(const QModelIndex &index);

    void on_RepMostrar_clicked();

    void on_AgenteBuscar_textChanged(const QString &arg1);

    void on_AgentesTabla_clicked(const QModelIndex &index);

    void on_EquipoCodigoBuscar_textChanged(const QString &arg1);

    void on_EquipoDescBuscar_textChanged(const QString &arg1);

    void on_RepTablaIng_clicked(const QModelIndex &index);

    void on_Actualizar_clicked();

    void ActualizarDatos();

    void on_Huerfanas_clicked();

private:
    Ui::Ingreso *ui;

    QDateTime dControl;
    dbManejo dbIngreso;
    QString AgenteTexto;

    int IndiceIng;
    int IngresoID;
    int Index;
    int Fila;

public:
    IngresoReparaciones *IngRep;
};

#endif // INGRESO_H
