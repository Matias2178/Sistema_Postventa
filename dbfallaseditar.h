#ifndef DBFALLASEDITAR_H
#define DBFALLASEDITAR_H

#include <QDialog>

#include <QSqlRelationalTableModel>
#include <QSortFilterProxyModel>

namespace Ui {
class dbFallasEditar;
}

class dbFallasEditar : public QDialog
{
    Q_OBJECT

public:
    explicit dbFallasEditar(QWidget *parent = 0);
    ~dbFallasEditar();

    QSqlRelationalTableModel    *ModProdF;
    QSortFilterProxyModel       *FilProdF;

    QSqlRelationalTableModel    *ModFalla;
    QSortFilterProxyModel       *FilFalla;

private slots:

    void on_Guardar_clicked();

    void on_Editar_clicked();

    void on_Borrar_clicked();

    void on_FallaBuscar_textChanged(const QString &arg1);

    void on_ProdTablaF_clicked(const QModelIndex &index);

private:
    Ui::dbFallasEditar *ui;
    int Indice;
    QString ProdFallaTx;
};

#endif // DBFALLASEDITAR_H
