#ifndef TRABAJO_H
#define TRABAJO_H

#include <QDialog>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QTableWidget>
namespace Ui {
class trabajo;
}

class trabajo : public QDialog
{
    Q_OBJECT

public:
    explicit trabajo(QWidget *parent = 0);
    ~trabajo();

private slots:

    void on_ReparacionesIniciar_clicked();

    void on_RepInterno_clicked();

    void on_TrabajoReparaciones_clicked(const QModelIndex &index);

    void on_RepInterno_2_clicked();

    void on_ReparacionesMostrar_clicked();

    void on_TrabajoAgenteTabla_clicked(const QModelIndex &index);

    void on_buttonBox_accepted();

private:
    Ui::trabajo *ui;
    bool saveFile(const QString &fileName);
    void CargarRecepcion();
    void TrabajosActualizar();
    void FPresupuesto();

 private:
    int IndexTrabajo;
    int TrabajoID;
    int IndexAgente;
    bool ReporteOk;
    QStringList DatosArchivo;
    QString curFile;
};

#endif // TRABAJO_H
