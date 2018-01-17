#ifndef FALLASPRODUCTOS_H
#define FALLASPRODUCTOS_H

#include <QDialog>

namespace Ui {
class FallasProductos;
}

class FallasProductos : public QDialog
{
    Q_OBJECT

public:
    explicit FallasProductos(QWidget *parent = 0);
    ~FallasProductos();

private:
    Ui::FallasProductos *ui;
};

#endif // FALLASPRODUCTOS_H
