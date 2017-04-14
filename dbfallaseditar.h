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

    void FallasActualizar();

    void on_Guardar_clicked();

    void on_Editar_clicked();

    void on_Borrar_clicked();

    void on_DatosFallas_clicked(const QModelIndex &index);

private:
    Ui::dbFallasEditar *ui;
    int Indice;
};

#endif // DBFALLASEDITAR_H
