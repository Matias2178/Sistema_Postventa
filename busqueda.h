#ifndef BUSQUEDA_H
#define BUSQUEDA_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QTableWidget>
#include <QSqlRelationalTableModel>
#include <QSortFilterProxyModel>
#include <reparacioneseditar.h>

namespace Ui {
class Busqueda;
}

class Busqueda : public QDialog
{
    Q_OBJECT

public:
    explicit Busqueda(QWidget *parent = 0);
    ~Busqueda();

private slots:
    void on_Monitores_clicked();

    void on_Perifericos_clicked();

    void on_Instalaciones_clicked();

 //   void on_BusquedaTbl_doubleClicked(const QModelIndex &index);
    
    void on_AgenteBuscar_textChanged(const QString &arg1);

    void on_BusquedaTbl_clicked(const QModelIndex &index);

    void on_Instalaciones_2_clicked();

    void on_EditarEquipo_clicked();

private:
    Ui::Busqueda *ui;
    QSqlRelationalTableModel *ModBusqueda;
    QSortFilterProxyModel *FilBusqueda;
    int tipo;

};

#endif // BUSQUEDA_H
