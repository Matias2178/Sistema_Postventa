#ifndef SELECCIONDEEDITORES_H
#define SELECCIONDEEDITORES_H

#include <QWidget>
#include <dbagente.h>

namespace Ui {
class selecciondeeditores;
}

class selecciondeeditores : public QWidget
{
    Q_OBJECT

public:
    explicit selecciondeeditores(QWidget *parent = 0);
    ~selecciondeeditores();

private slots:

    void on_Fallas_clicked();

    void on_FallasProductos_clicked();

    void on_Agentes_clicked();

    void on_Operarios_clicked();

    void on_Productos_clicked();

private:
    Ui::selecciondeeditores *ui;
    dbAgente *AgenteVentana;
};

#endif // SELECCIONDEEDITORES_H
