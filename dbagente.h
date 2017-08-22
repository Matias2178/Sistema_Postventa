#ifndef DBAGENTE_H
#define DBAGENTE_H

#include <QDialog>
#include <QSqlRelationalTableModel>
#include <QSortFilterProxyModel>

namespace Ui {
class dbAgente;
}

class dbAgente : public QDialog
{
    Q_OBJECT

public:
    explicit dbAgente(QWidget *parent = 0);
    ~dbAgente();

    QSqlRelationalTableModel    *ModDatos;
//    QSortFilterProxyModel       *FilDatos;

private slots:

    void AgentesActualizar();

    void on_Guardar_clicked();

    void on_Editar_clicked();

    void on_Borrar_clicked();

    void on_Guardar_2_clicked();

    void on_DatosAgentesTabla_clicked(const QModelIndex &index);

public:

    void SetAgenteOperario(bool arg);

private:
    Ui::dbAgente *ui;
    int Indice;
    bool AgenteOperario;
};

#endif // DBAGENTE_H
