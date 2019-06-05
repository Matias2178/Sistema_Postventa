#ifndef NOTAPEDIDO_H
#define NOTAPEDIDO_H

#include <QDialog>

namespace Ui {
class NotaPedido;
}

class NotaPedido : public QDialog
{
    Q_OBJECT

public:
    explicit NotaPedido(QWidget *parent = nullptr);
    ~NotaPedido();
    void SetDatos(int id);
    void InfoFallas(QString Codigo);
    void CargaDiferencia(int id);

private:
    Ui::NotaPedido *ui;
};

#endif // NOTAPEDIDO_H
