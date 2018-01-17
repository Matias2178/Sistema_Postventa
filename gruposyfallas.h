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
    void on_Grupo_Guardar_clicked();

    void on_Grupo_Editar_clicked();

    void on_Grupo_Borrar_clicked();

    void on_Fallas_Productos_activated(int index);



    void on_TablaFallaGrupo_clicked(const QModelIndex &index);

    void on_Falla_Guardar_clicked();

    void on_pushButton_clicked();

    void BuscaNuevo();

    void on_TablaFallaLista_clicked(const QModelIndex &index);

    void on_Falla_Editar_clicked();

private:
    int NGrupo;
    int IDGrupo;
    int IDFalla;
    Ui::gruposyfallas *ui;
};

#endif // GRUPOSYFALLAS_H
