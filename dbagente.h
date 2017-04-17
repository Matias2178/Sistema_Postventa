#ifndef DBAGENTE_H
#define DBAGENTE_H

#include <QDialog>

namespace Ui {
class dbAgente;
}

class dbAgente : public QDialog
{
    Q_OBJECT

public:
    explicit dbAgente(QWidget *parent = 0);
    ~dbAgente();

private slots:

    void AgnetesActualizar();

    void on_Guardar_clicked();

    void on_Editar_clicked();

    void on_Borrar_clicked();

    void on_DatosAgentes_clicked(const QModelIndex &index);

private:
    Ui::dbAgente *ui;
    int Indice;
};

#endif // DBAGENTE_H
