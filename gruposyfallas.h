#ifndef GRUPOSYFALLAS_H
#define GRUPOSYFALLAS_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRelationalTableModel>
#include <QSortFilterProxyModel>

namespace Ui {
class gruposyfallas;
}

class gruposyfallas : public QDialog
{
    Q_OBJECT

public:
    explicit gruposyfallas(QWidget *parent = 0);
    ~gruposyfallas();
    QSqlRelationalTableModel *ModGrupo;
    QSortFilterProxyModel    *FilGrupo;

    QSqlRelationalTableModel *ModFalla;
    QSortFilterProxyModel    *FilFalla;


private slots:
    void on_GrupoGuardar_clicked();

    void on_GrupoEditar_clicked();

    void on_GrupoBorrar_clicked();

    void on_FallasProductos_activated(int index);

    void on_TablaFallaGrupo_clicked(const QModelIndex &index);

    void on_FallaGuardar_clicked();

    void on_NuevoGrupo_clicked();

    bool BuscaNuevo();

    void on_TablaFallaLista_clicked(const QModelIndex &index);

    void on_FallaEditar_clicked();

private:
    int NGrupo;
    int IDGrupo;
    int IDFalla;
    Ui::gruposyfallas *ui;
};

#endif // GRUPOSYFALLAS_H
