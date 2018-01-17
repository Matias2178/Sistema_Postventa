#include "fallasproductos.h"
#include "ui_fallasproductos.h"

FallasProductos::FallasProductos(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FallasProductos)
{
    ui->setupUi(this);
}

FallasProductos::~FallasProductos()
{
    delete ui;
}
