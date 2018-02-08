#ifndef FALLASPRODUCTOS_H
#define FALLASPRODUCTOS_H

#include <QDialog>
#include <QDialog>

#include <QSqlRelationalTableModel>
#include <QSortFilterProxyModel>

namespace Ui {
class FallasProductos;
}

class FallasProductos : public QDialog
{
    Q_OBJECT

public:
    explicit FallasProductos(QWidget *parent = 0);
    ~FallasProductos();

    QSqlRelationalTableModel    *ModListaProd;
    QSortFilterProxyModel       *FilListaProd;

    QSqlRelationalTableModel    *ModFallaGrupo;
    QSortFilterProxyModel       *FilFallaGrupo;

    QSqlRelationalTableModel    *ModFallaLista;
    QSortFilterProxyModel       *FilFallaLista;

    QSqlRelationalTableModel    *ModFallaProd;
    QSortFilterProxyModel       *FilFallaProd;





private slots:
    void on_FallaProdBuscar_textChanged(const QString &arg1);

    void on_TablaProd_FP_clicked(const QModelIndex &index);

    void on_TablaGrupo_FP_clicked(const QModelIndex &index);

    void on_TablaFalla_FP_clicked(const QModelIndex &index);

    void on_TablaFallaProd_FP_clicked(const QModelIndex &index);

    void on_ProdFallaAgregar_clicked();

    void on_ProdFallaQuitar_clicked();

private:
    Ui::FallasProductos *ui;
    int ID_Fallas_Productos;
    QString FEquipo, FEqCod, FEqDes, FGrupo;
};

#endif // FALLASPRODUCTOS_H
