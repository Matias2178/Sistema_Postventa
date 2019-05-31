#ifndef DBPRODUCTOSEDITAR_H
#define DBPRODUCTOSEDITAR_H

#include <QDialog>

#include <QSqlRelationalTableModel>
#include <QSortFilterProxyModel>
#include <QCompleter>

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

    QCompleter *CompProdEdit;


private slots:

    void on_Guardar_clicked();

    void on_Editar_clicked();

    void on_Borrar_clicked();

    void on_DatosProdTabla_clicked(const QModelIndex &index);

    void on_ProductoEdit_textChanged(const QString &arg1);


    void on_DescripcionEdit_textChanged(const QString &arg1);

    void on_ProductoCodigo_textChanged(const QString &arg1);

private:
    Ui::dbProductosEditar *ui;
    int Indice;
};

#endif // DBPRODUCTOSEDITAR_H
