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

QT_BEGIN_NAMESPACE

namespace Ui {
class MainWindow;
}

QT_END_NAMESPACE

class Console;
class SettingsDialog;
class selecciondeeditores;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void  ProductosCrear();
    void  ProductosInsertar();
    void  ProductosLeer();
    void  FallasCrear();
    void  dbAbrirCrear();


private slots:
    void openSerialPort();
    void closeSerialPort();
    void about();
    void Version();
    void writeData(const QByteArray &data);
    void LIN_Lectura();
    void CargarDatos(QTableWidget &FALLAS, QString Tipo);

    void handleError(QSerialPort::SerialPortError error);

    void LIN_Envio();

    void on_BT_B4500_clicked();

    void on_actionGuardar_triggered();

    void on_actionClear_triggered();

    void on_Ingreso_Guardar_clicked();

    void on_actionGateWay_triggered();

    void on_BT_G4500_clicked();

    void on_S_Guardar_clicked();
    
    void on_S_Borrar_Item_clicked();

    void on_S_ANT_clicked();

    void on_S_SIG_clicked();

    void on_RPM_Guardar_clicked();

    void Moduladoras(int);

    void on_MOD_Guardar_clicked();

    void on_GPS_Guardar_clicked();

    void on_GPS_Borrar_clicked();

    void on_MOD_Borrar_clicked();

    void on_RPM_Borrar_clicked();

    void on_S_Borrar_clicked();

    void on_INS_BGuardar_clicked();

    void on_INS_BBorrar_clicked();

    void BonificacionMsg ();

    void on_CAU_Guardar_clicked();

    void Mascaras();

    void on_MON_TIPO_activated(int index);

    void on_Siguiente_clicked();

    void on_S_TIPO_activated(const QString &arg1);

    void on_INS_TIPO_activated(const QString &arg1);

private:
    void initActionsConnections();
    void CambioPantalla(int Pant);
    void BorraIngreso();
    void BorraMonitores();
    void BorraSensores();
    void BorraRMP();
    void BorraMOD();
    void BorraGPS();
    void BorraPerifericos();
    void BorraINS();

    bool saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
    void TituloColumnas();
    void EncabezadoMsg();
private:
    Ui::MainWindow *ui;
    Console *console;
    SettingsDialog *settings;
    selecciondeeditores *SelEditores;
    QSerialPort *serial;
    QDate FechaActual;
    QString curFile;
    QStringList MonMascaras;
    QSqlDatabase db;
    int PantallaActual;
    bool Columnas;
    bool EscColumnas;
    int Item;
    int LIndice;
    int EIndice;
    int SenID;
    bool LecturaOk;
    bool Escribir;
    bool ConfInicio;
    bool RPM_TRB;
    bool Guardar;
    bool Siguiente;
    unsigned long NSerie;
    QByteArray DatosLin;
    QByteArray Lectura;
};

#endif // MAINWINDOW_H
