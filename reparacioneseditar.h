#ifndef REPARACIONESEDITAR_H
#define REPARACIONESEDITAR_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QString>
#include <QSqlRelationalTableModel>
#include <QSortFilterProxyModel>
#include <dbmanejo.h>
#include <user.h>


namespace Ui {
class reparacioneseditar;
}

class reparacioneseditar : public QDialog
{
    Q_OBJECT

public:
    explicit reparacioneseditar(QWidget *parent = nullptr);
    ~reparacioneseditar();

    void SetDatos(int TipoProd, QString ID);

private slots:
    void on_Productos_clicked(const QModelIndex &index);

    void on_buttonBox_accepted();


signals:
    void finalizar();

private:
    QSqlRelationalTableModel    *ModEditRep;
    QSortFilterProxyModel       *FilEditRep;
    QSortFilterProxyModel       *FilEditRep2;
    Ui::reparacioneseditar *ui;
    QString Tabladb;
    dbManejo EditRep;
    int Producto;
    User tFallas;
};

#endif // REPARACIONESEDITAR_H
