#ifndef DBPRODUCTOSEDITAR_H
#define DBPRODUCTOSEDITAR_H

#include <QDialog>

#include <QSqlRelationalTableModel>
#include <QSortFilterProxyModel>

namespace Ui {
class dbProductosEditar;
}

class dbProductosEditar : public QDialog
{
    Q_OBJECT

public:
    explicit dbProductosEditar(QWidget *parent = 0);
    ~dbProductosEditar();

    QSqlRelationalTableModel    *ModProdEdit;
    QSortFilterProxyModel       *FilProdEdit;


private slots:

 //   void ProductosLeer();

 //   void on_DatosProd_clicked(const QModelIndex &index);

    void on_Guardar_clicked();

    void on_Editar_clicked();

    void on_Borrar_clicked();

    void on_DatosProdTabla_clicked(const QModelIndex &index);

private:
    Ui::dbProductosEditar *ui;
    int Indice;
};

#endif // DBPRODUCTOSEDITAR_H
