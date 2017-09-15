#ifndef BUSQUEDA_H
#define BUSQUEDA_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QTableWidget>
#include <QSqlRelationalTableModel>
#include <QSortFilterProxyModel>
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

private:
    Ui::Busqueda *ui;
    QSqlRelationalTableModel *Monitores;
    QSortFilterProxyModel *FilMonitores;
    QSqlRelationalTableModel *Perifericos;
    QSortFilterProxyModel *FilPerifericos;
    QSqlRelationalTableModel *Instalaciones;
    QSortFilterProxyModel *FilInstalaciones;

};

#endif // BUSQUEDA_H
