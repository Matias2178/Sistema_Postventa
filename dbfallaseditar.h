#ifndef DBFALLASEDITAR_H
#define DBFALLASEDITAR_H

#include <QDialog>

namespace Ui {
class dbFallasEditar;
}

class dbFallasEditar : public QDialog
{
    Q_OBJECT

public:
    explicit dbFallasEditar(QWidget *parent = 0);
    ~dbFallasEditar();

private slots:

    void FallasCargarDatos();

    void FallasActualizar(const QString &arg1);

    void on_Guardar_clicked();

    void on_Editar_clicked();

    void on_Borrar_clicked();

    void on_DatosFallas_clicked(const QModelIndex &index);

    void on_FallaProducto_activated(const QString &arg1);

private:
    Ui::dbFallasEditar *ui;
    int Indice;
};

#endif // DBFALLASEDITAR_H
