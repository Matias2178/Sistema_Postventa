#ifndef INGRESOREPARACIONES_H
#define INGRESOREPARACIONES_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>



namespace Ui {
class IngresoReparaciones;
}

class IngresoReparaciones : public QDialog
{
    Q_OBJECT

public:
    explicit IngresoReparaciones(QWidget *parent = nullptr);
    ~IngresoReparaciones();
    void SetDatos(QString tipo, QString Agente,QString Fing, QString rTransp, QString fTransp, QString Observaciones);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::IngresoReparaciones *ui;
    int Update;
};

#endif // INGRESOREPARACIONES_H
