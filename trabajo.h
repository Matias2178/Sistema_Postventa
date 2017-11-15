#ifndef TRABAJO_H
#define TRABAJO_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QTableWidget>
#include <QSqlRelationalTableModel>
#include <QSortFilterProxyModel>
namespace Ui {
class trabajo;
}

class trabajo : public QDialog
{
    Q_OBJECT

public:
    explicit trabajo(QWidget *parent = 0);
    ~trabajo();

    QSqlRelationalTableModel *TrabAgentes;
    QSortFilterProxyModel *FiltTrAgentes;

    QSqlRelationalTableModel *TrabRep;
    QSortFilterProxyModel *FilTrRep;

private slots:

    void on_ReparacionesIniciar_clicked();

    void on_RepInterno_clicked();

    void on_RepInterno_2_clicked();

    void on_ReparacionesMostrar_clicked();

    void on_buttonBox_accepted();

    void on_AgenteBuscarTrab_textChanged(const QString &arg1);

    void on_AgentesTablaTrab_clicked(const QModelIndex &index);

    void on_pushButton_clicked();

    void on_RepTablaTrab_clicked(const QModelIndex &index);


private:
    Ui::trabajo *ui;
    bool saveFile(const QString &fileName);
    void CargarRecepcion();
    void TrabajosActualizar();
    void FPresupuesto();

 private:
    int IndexTrabajo;
    int TrabajoID;
    bool ReporteOk;
    QStringList DatosArchivo;
    QString curFile;
};

#endif // TRABAJO_H
