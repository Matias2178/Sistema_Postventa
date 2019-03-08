#ifndef INGRESOREPARACIONES_H
#define INGRESOREPARACIONES_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>


//Ingreso IngMod;


namespace Ui {
class IngresoReparaciones;
}

class IngresoReparaciones : public QDialog
{
    Q_OBJECT

public:
    explicit IngresoReparaciones(QWidget *parent = nullptr);
    ~IngresoReparaciones();
    void SetDatos(QString Agente,QString Fing);
    void SetDatos(int ID);

private slots:
    void on_buttonBox_accepted();

signals:
    void finalizar();

private:
    Ui::IngresoReparaciones *ui;
    int Update;
};

#endif // INGRESOREPARACIONES_H
