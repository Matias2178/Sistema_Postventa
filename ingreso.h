#ifndef INGRESO_H
#define INGRESO_H

#include <QDialog>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>

namespace Ui {
class Ingreso;
}

class Ingreso : public QDialog
{
    Q_OBJECT

public:
    explicit Ingreso(QWidget *parent = 0);
    ~Ingreso();

private slots:
    void on_RepGuardar_clicked();

    void on_RepEditar_clicked();

    void on_RepBorrar_clicked();

    void on_RepTabla_clicked(const QModelIndex &index);

    void on_IngGuardar_clicked();

    void on_IngEditar_clicked();

    void on_IngBorrar_clicked();

    void on_IngresoTabla_clicked(const QModelIndex &index);

    void AgenteCargar();

    void IngresoProductos();

    void on_AgenteTabla_clicked(const QModelIndex &index);

    void on_RepMostrar_clicked();

private:
    Ui::Ingreso *ui;
    int IndiceRep;
    int IndiceIng;
    int IngresoID;
};

#endif // INGRESO_H
