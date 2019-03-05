#ifndef USER_H
#define USER_H
#include <QString>
#include <QList>
#include <QTableWidget>

class User
{
public:
    User();
    QString Moduladoras(int ID);
    QList  <QString> GetFallas(QTableWidget &Falla);
    void SetFallas(QTableWidget &Falla, QString Fallas);
};

#endif // USER_H
