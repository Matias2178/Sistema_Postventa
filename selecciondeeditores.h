#ifndef SELECCIONDEEDITORES_H
#define SELECCIONDEEDITORES_H

#include <QWidget>

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
    void on_commandLinkButton_clicked();

    void on_commandLinkButton_2_clicked();

    void on_toolButton_2_clicked();

private:
    Ui::selecciondeeditores *ui;
};

#endif // SELECCIONDEEDITORES_H
