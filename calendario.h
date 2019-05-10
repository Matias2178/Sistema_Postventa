#ifndef CALENDARIO_H
#define CALENDARIO_H

#include <QDialog>

namespace Ui {
class Calendario;
}

class Calendario : public QDialog
{
    Q_OBJECT

public:
    explicit Calendario(QWidget *parent = nullptr);
    ~Calendario();

private slots:

    void on_Aniversario_clicked(const QDate &date);

private:
    Ui::Calendario *ui;

signals:
    void Enviar();

};

#endif // CALENDARIO_H
