#ifndef ARCHIVOS_H
#define ARCHIVOS_H

#include <QDialog>
#include <QFile>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRelationalTableModel>
#include <QSortFilterProxyModel>

namespace Ui {
class Archivos;
}

class Archivos : public QDialog
{
    Q_OBJECT

public:
    explicit Archivos(QWidget *parent = nullptr);
    ~Archivos();

    QSqlRelationalTableModel    *ModArchivos;
    QSortFilterProxyModel       *FilArchivos;

private slots:
    void on_pushButton_clicked();

    void on_bBaseDatos_clicked();

    void on_bInformeAgente_clicked();

    void on_bInformeInterno_clicked();

private:
    Ui::Archivos *ui;
};

#endif // ARCHIVOS_H
