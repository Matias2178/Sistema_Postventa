/****************************************************************************
**
** Copyright (C) 2012 Denis Shienkov <denis.shienkov@gmail.com>
** Copyright (C) 2012 Laszlo Papp <lpapp@kde.org>
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtSerialPort module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore/QtGlobal>
#include <QMainWindow>
#include <QTableWidget>
#include <QtSerialPort/QSerialPort>
#include <QDate>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include  <reparaciones.h>


QT_BEGIN_NAMESPACE

namespace Ui {
class MainWindow;
}

QT_END_NAMESPACE

class Reparaciones;
class SettingsDialog;
class selecciondeeditores;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


    void ProductosLeer();
    void  MonitoresActualizar();
    void  PerifericosActualizar();
    void  InstalacionesActualizar();


private slots:
    void openSerialPort();
    void closeSerialPort();
    void about();
    void Version();
    void writeData(const QByteArray &data);
    void readData();
    void CargarDatos(QTableWidget &FALLAS, QString Tipo);
    void handleError(QSerialPort::SerialPortError error);

    void LIN_Envio();

    void Mascaras();

 //   void on_Siguiente_clicked();

    void on_actionActualizar_triggered();

    void on_actionGuardar_triggered();

    void on_actionClear_triggered();

    void on_PantallaIngreso_clicked();

    void on_PantallaTrabajos_clicked();

    void on_PantallaReparaciones_clicked();

private:
    void initActionsConnections();
    bool saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
private:
    Ui::MainWindow *ui;
    SettingsDialog *settings;
    selecciondeeditores *SelEditores;
    QSerialPort *serial;
    QDate FechaActual;
    QString curFile;
    QSqlDatabase db;
    Reparaciones *Reparacion;

    bool LecturaOk;

    int RepIndice;
    int IngIndice;
    int RepID;
    QStringList DatosArchivo;

};

#endif // MAINWINDOW_H
