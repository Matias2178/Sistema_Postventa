#include "selecciondeeditores.h"
#include "ui_selecciondeeditores.h"
#include <dbfallaseditar.h>
#include <gruposyfallas.h>
#include <dbproductoseditar.h>
#include <QDebug>
#include <mainwindow.h>
#include <dbagente.h>

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

void selecciondeeditores::on_commandLinkButton_clicked()
{
    dbFallasEditar *FallasVentana = new dbFallasEditar(this);
    FallasVentana->setModal(true);
    FallasVentana->show();
}

void selecciondeeditores::on_commandLinkButton_2_clicked()
{

    dbProductosEditar  *ProductosVentana = new dbProductosEditar(this);
    ProductosVentana->setModal(true);
    ProductosVentana->show();
}

void selecciondeeditores::on_commandLinkButton_3_clicked()
{

    AgenteVentana->SetAgenteOperario(true);
    AgenteVentana->setModal(true);
    AgenteVentana->show();
}

void selecciondeeditores::on_commandLinkButton_4_clicked()
{
  //  dbAgente *AgenteVentana = new dbAgente(this);
    AgenteVentana->SetAgenteOperario(false);
    AgenteVentana->setModal(true);
    AgenteVentana->show();
}

void selecciondeeditores::on_pushButton_clicked()
{
    gruposyfallas *GyF_Ventana = new gruposyfallas(this);
    GyF_Ventana->setModal(true);
    GyF_Ventana->show();

}
