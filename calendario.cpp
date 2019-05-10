#include "calendario.h"
#include "ui_calendario.h"
#include <QDebug.h>
#include <variables.h>

Calendario::Calendario(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Calendario)
{
    ui->setupUi(this);
}

Calendario::~Calendario()
{
    delete ui;
}


void Calendario::on_Aniversario_clicked(const QDate &date)
{
    Fechas.clear();
    Fechas.append(date.toString("yyyy-MM-dd"));
    emit Enviar();
}
