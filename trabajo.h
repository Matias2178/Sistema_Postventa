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
    void on_TrabajoAgente_activated(const QString &arg1);

    void on_ReparacionesIniciar_clicked();

    void on_RepInterno_clicked();

    void on_TrabajoReparaciones_clicked(const QModelIndex &index);

    void on_RepInterno_2_clicked();

private:
    Ui::trabajo *ui;
    void TrabajoActualizarAgente();
    bool saveFile(const QString &fileName);
    void CargarRecepcion();
    void TrabajosActualizar();
 private:
    int IndexTrabajo;
    int TrabajoID;
    QStringList DatosArchivo;
    QString curFile;
};

#endif // TRABAJO_H
