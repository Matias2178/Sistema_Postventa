#include "selecciondeeditores.h"
#include "ui_selecciondeeditores.h"
#include <gruposyfallas.h>
#include <dbproductoseditar.h>
#include <QDebug>
#include <mainwindow.h>
#include <dbagente.h>
#include <fallasproductos.h>

selecciondeeditores::selecciondeeditores(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::selecciondeeditores)
{
    ui->setupUi(this);
    AgenteVentana = new dbAgente(this);
}

selecciondeeditores::~selecciondeeditores()
{
    delete ui;
}

void selecciondeeditores::on_Fallas_clicked()
{
    gruposyfallas *GyF_Ventana = new gruposyfallas(this);
    GyF_Ventana->setModal(true);
    GyF_Ventana->show();
}

void selecciondeeditores::on_FallasProductos_clicked()
{
    FallasProductos *FProd_Ventana = new FallasProductos(this);
    FProd_Ventana->setModal(true);
    FProd_Ventana->show();
}

void selecciondeeditores::on_Agentes_clicked()
{
    AgenteVentana->SetAgenteOperario(true);
    AgenteVentana->setModal(true);
    AgenteVentana->show();
}

void selecciondeeditores::on_Operarios_clicked()
{
    AgenteVentana->SetAgenteOperario(false);
    AgenteVentana->setModal(true);
    AgenteVentana->show();
}

void selecciondeeditores::on_Productos_clicked()
{
    dbProductosEditar  *ProductosVentana = new dbProductosEditar(this);
    ProductosVentana->setModal(true);
    ProductosVentana->show();
}
