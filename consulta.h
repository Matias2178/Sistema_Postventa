#ifndef CONSULTA_H
#define CONSULTA_H


#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QTableWidget>
#include <QSqlRelationalTableModel>
#include <QSortFilterProxyModel>

namespace Ui {
class Consulta;
}

class Consulta : public QDialog
{
    Q_OBJECT

public:
    explicit Consulta(QWidget *parent = 0);
    ~Consulta();
    void TragajoID(int Id);
private slots:
    void on_CambioFrep_clicked();
    QString CambioFecha(QString fecha);

private:
    Ui::Consulta *ui;
};

#endif // CONSULTA_H
