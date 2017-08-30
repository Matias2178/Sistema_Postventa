#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include "reparaciones.h"
//#include "ui_reparaciones.h"
#include <QMessageBox>
#include <QDebug>
#include <variables.h>
#include <user.h>

#include "reparaciones.h"
#include "ui_reparaciones.h"

void MainWindow::LIN_Envio()
{
    if(serial->isOpen()&& Escribir)
    {
        Escribir = false;
        DatosLin.clear();
        switch (EIndice)
        {
        default:
        case 1:
            // NUMERO DE SERIE
            serial->write("$40FF0604FA\r\n");
            LIndice = 1;
            break;
        case 2:
            //FECHA DE FABRICACION
            serial->write("$40FF0604FB\r\n");
            LIndice = 2;
            break;
        case 3:
            //FECHA DE FABRICACION
            serial->write("$40FF0604F5\r\n");
            LIndice = 3;
            break;
        case 4:
            //VERSION DEL SOFTWARE
            serial->write("$40FF0604FD\r\n");
            LIndice = 4;
            break;
        case 5:
            //ID DEL PRODUCTO
            serial->write("$40FF0701\r\n");
            LIndice = 5;
            break;
            //------------------------------------------------------
            //   Comunicacion de sensores de Semilla y Fertilizante
            //------------------------------------------------------
        case 10:
            serial->write("$40FF0002\r\n");
            LIndice = 10;
            break;
        case 11:
            serial->write("$40FF0401\r\n");
            LIndice = 11;
            break;
        case 12:
            //Sensores Tiempo de Medicion
            serial->write("$40FF0101\r\n");
            LIndice = 12;
            break;
        case 13:
            //Sensores Tiempo de deteccion
            serial->write("$40FF0201\r\n");
            LIndice = 13;
            break;
        case 14:
            //Sensores Cantidad para activacion
            serial->write("$40FF0301\r\n");
            LIndice = 14;
            break;
            //------------------------------------------------------
            //      Comunicacion Modulo GPS
            //------------------------------------------------------
        case 20:
            //Lectura datos de GPS
            serial->write("$40D6030E3B\r\n");

            //BIT0	I	Lat. y Lon.	(8) en 1/1000000 de Grados
            //BIT1	I	Velocidad	(2) en 1/10 Km/H
            //BIT2	0	Altitud         (2) en Metros
            //BIT3	I	Curso		(1)	Grados / 2
            //BIT4	I	Satelites	(1)
            //BIT5	I	Antiguedad	(2) en Segundos
            //BIT6	0	Metros Rec.     (4) en Metros
            //BIT7	0	Tiempo UTC      (6) sec-min-hour-mday-mon-year
            //DATO: B: 0011 1011 = 0x3B
            //TAMAÑO DA DATOS;                      (14) 0x0E
            LIndice = 20;
            break;
            //------------------------------------------------------
            //      Comunicacion de sensores de turbina y rotacion
            //------------------------------------------------------
        case 30:
            //Lectura de datos de sensore RPM y TURBINA
            serial->write("$40FF0002\r\n");
            LIndice = 30;
            break;
        case 31:
            //Lectura del factor K
            serial->write("$40FF060480\r\n");
            LIndice = 31;
            break;
        case 32:
            //Lectura Alarma Maxima (Solo Turbina)
            serial->write("$40FF060460\r\n");
            LIndice = 32;
            break;
        case 33:
            //Lectura Alarma minima (Solo Turbina)
            serial->write("$40FF060470\r\n");
            LIndice = 33;
            break;
        case 40:
            //Lectura de SP y KD
            serial->write("$40FF0404\r\n");
            LIndice = 40;
            break;
        case 41:
            //Lectura Pulsos por KM Moduladora
            serial->write("$40FF060480\r\n");
            LIndice = 41;
            break;
        case 42:
            //Lectura DT DD y RT
            serial->write("$40FF060481\r\n");
            LIndice = 42;
            break;
            //----------------------------------------------------
            //		Lectura de datos del caudalimetro
            //----------------------------------------------------
        case 50:
            //Tiempo de Marcha Total
            serial->write("$40FF050420\r\n");
            LIndice = 50;
            break;
        case 51:
            //Tiempo de Marcha Parcial ////FALTA HACER
            serial->write("$40FF050420\r\n");
            LIndice = 51;
            break;
        case 52:
            //Consumo de combustible Total en litros
            serial->write("$40FF0104\r\n");
            LIndice = 52;
            break;
        case 53:
            //Consumo de combustible Parcial ///FALTA HACER
            serial->write("$40FF050403\r\n");
            LIndice = 53;
            break;
        case 54:
            //Desconexiones
            serial->write("$406C050421\r\n");
            LIndice = 54;
            break;
        case 55:
            //Desconexiones Alta Tension
            serial->write("$$406C050424\r\n");
            LIndice = 55;
            break;
        case 56:
            //Cantidad Bloqueo Magnetico
            serial->write("$406C050423\r\n");
            LIndice = 56;
            break;
        case 57:
            //Tiempo de bloqueo Magnetico
            serial->write("$406C050422\r\n");
            LIndice = 57;
            break;

        case 58:
        //PATENTE PARTE 1
            serial->write("$406C0604F2\r\n");
            LIndice = 58;
            break;
        case 59:
        //PATENTE PARTE 2
            serial->write("$406C0604F3\r\n");
            LIndice = 59;
            break;
        }
    }
}


void Reparaciones::LIN_Lectura()
{
    User Utilidades;
    unsigned long Valor;
    long Posicion;
    unsigned int DD, MM, AA;
    unsigned int Aux;
    int hrs;
    int min;
    int seg;
    bool ok;
    QString Datos;
    QByteArray CC;
    QDateTime FechaFab;

//    DatosLin.append(serial->readAll());
    if(DatosLin.contains(0x0A) )
    {
        Escribir = true;
        Lectura.clear();
        Lectura.append(LecturaLIN(DatosLin));
        if(DatosLin.contains("!"))
        {

            ui->SEN_ID->setStyleSheet("QLineEdit { background-color: red }");
            EIndice = 1;
            Guardar = false;
            NSerie = 0;
            //  Guardar = false;
            //  Siguiente = false;
        }
        else
        {
            ui->SEN_ID->setStyleSheet("QLineEdit { background-color: yellow }");
            switch (LIndice)
            {
            case 1:
                //NUMERO DE SERIE
                Valor = Lectura.toLong(&ok,16);
                if(Siguiente)
                {
                    EIndice = 0;
                    return;
                }

                if(NSerie &&  (NSerie != Valor) && !Guardar)
                {
                    QString Lectura = QString("Sensor Anterior %1 Actual %2").arg(NSerie).arg(Valor);
                    QMessageBox::information(this, tr("Info"), Lectura);
                    EIndice = 0;
                    Siguiente = true;
                    ui->Siguiente->setEnabled(true);
                    return;
                }
                NSerie = Valor;
                ui->SEN_NSERIE->setText(QString::number(Valor,10));
                EIndice ++;
                break;
            case 2:
                //FECHA DE FABRICACION
                Valor = Lectura.toLong(&ok,16);
                FechaFab.setTime_t(Valor);
                ui->SEN_FF->setInputMask("00/00/0000");
                ui->SEN_FF->setText(FechaFab.toString("ddMMyyyy"));
                EIndice ++;
                break;
            case 3:
                //FECHA DE INSTALACION
                Valor = Lectura.toLong(&ok,16);
                FechaFab.setTime_t(Valor);
                ui->SEN_FI->setInputMask("00/00/0000");
                if(Valor)
                {
                    ui->SEN_FI->setText(FechaFab.toString("ddMMyyyy"));
                }
                else
                {
                    ui->SEN_FI->setText("        ");
                }
                EIndice ++;
                break;
            case 4:
                //VERSION DEL SOFTWARE Y FECHA DE COMPILACION
                Valor = Lectura.mid(6,2).toInt(&ok,16);
                VSoft = Valor;
                ui->SEN_VS->setInputMask("0.00");
                ui->SEN_VS->setText(QString::number(Valor,10));

                DD = Lectura.mid(4,2).toInt(&ok,16);
                MM = Lectura.mid(2,2).toInt(&ok,16);
                AA = Lectura.mid(0,2).toInt(&ok,16);

                CC.clear();
                CC.append(QString::number(DD,10).rightJustified(2,'0'));
                CC.append(QString::number(MM,10).rightJustified(2,'0'));
                CC.append(QString::number(AA,10).rightJustified(2,'0'));

                ui->SEN_FS->setInputMask("00/00/00");
                ui->SEN_FS->setText(CC);
                EIndice ++;
                break;
            case 5:
                //LECTURA DE ID
                int SenID;
                Valor = Lectura.toShort(&ok,16);
                SenID = Valor;

                ui->SEN_ID_3->setText(QString::number(Valor,10));
                ui->SEN_ID_4->setText(QString::number(Valor,16).toUpper());
                Valor ++;
                ui->SEN_ID->setText(QString::number(Valor,10));

                //Sensor de caida 0x00 0x3F Virgen 0xF0
                if((SenID < 0x3F) || (SenID == 0xF0))
                {
                    if(SenID ==0xF0)
                        ui->SEN_ID->setText("SinID");
                    else
                        ui->SEN_ID->setText(QString::number(Valor,10));

                    CambioPantalla(1);  //Pantalla Sensores de semilla
                    ui->SEN_TIPO->setText("Caida");
                    EIndice = 10;
                    if(SenID ==0xF0)
                    {
                        ui->SEM_NOM->setText("Sensor Virgen");
                        ui->SEN_TIPO->setText("S. Virgen");
                    }
                    else if(SenID < 0x1F)
                    {
                        ui->SEM_NOM->setText("Sensor Semilla");
                        ui->SEN_TIPO->setText("S. Semilla");
                    }
                    else if(SenID < 0x3F)
                    {
                        ui->SEN_TIPO->setText("S. Fertilizante");
                        ui->SEM_NOM->setText("Sensor Fertilizante");
                    }
                }
                //Modulo GPS
                else if ((SenID == 0xD6))
                {
                    Valor -= 0xD6;
                    ui->SEN_ID->setText(QString::number(Valor,10));
                    CambioPantalla(2);  //Pantalla Modulo GPS

                    ui->SEN_TIPO->setText("GPS");
                    EIndice = 20;
                }
                //Sensor de Rotacion
                else if(((SenID >= 0x40) && (SenID <= 0x47))|| (SenID == 0xF1))
                {

                    Valor -=0x40;
                    if(SenID ==0xF1)
                       ui->SEN_ID->setText("SinID");
                    else
                        ui->SEN_ID->setText(QString::number(Valor,10));

                    CambioPantalla(3);  //Pantalla Sensores Turbina y Rotacion

                    ui->SEN_TIPO->setText("ROTACION");
                    RPM_TRB = true;
                    EIndice = 30;
                }
                //Sensor de Turbina
                else if((SenID >= 0xD3) && (SenID <= 0xD5))
                {
                    Valor -= 0xD3;
                    ui->SEN_ID->setText(QString::number(Valor,10));
                    CambioPantalla(3);  //Pantalla Sensores Turbina y Rotacion

                    ui->SEN_TIPO->setText("TURBINA");
                    ui->RPM_NOM->setText("Sensor Turbina");
                    RPM_TRB = false;
                    EIndice = 30;
                }
                //Velocidad Primario
                else if((SenID == 0xD0))
                {
                    Valor -= 0xD0;
                    ui->SEN_ID->setText(QString::number(Valor,10));
                    CambioPantalla(3);  //Pantalla Sensores Turbina y Rotacion

                    ui->SEN_TIPO->setText("VEL PRIMARIO");
                    ui->RPM_NOM->setText("Velocidad Primario");
                    RPM_TRB = false;
                    EIndice = 30;
                }
                else if((SenID == 0xD1))
                {
                    Valor -= 0xD1;
                    ui->SEN_ID->setText(QString::number(Valor,10));
                    CambioPantalla(3);  //Pantalla Sensores Turbina y Rotacion

                    ui->SEN_TIPO->setText("VEL SECUNDARIO");
                    ui->RPM_NOM->setText("Velocidad Secundario");
                    RPM_TRB = false;
                    EIndice = 30;
                }
                //RPM PRIMARIO
                else if(SenID == 0xD2)
                {
                    Valor -= 0xD2;
                    ui->SEN_ID->setText(QString::number(Valor,10));
                    CambioPantalla(3);  //Pantalla Sensores Turbina y Rotacion

                    ui->SEN_TIPO->setText("RPM PRIMARIO");
                    ui->RPM_NOM->setText("RPM Primario");
                    RPM_TRB = false;
                    EIndice = 30;
                }
                //Moduladoras
                else if(((SenID >= 0x64) && (SenID <= 0x6B))||((SenID >= 0xC8) && (SenID <= 0xCF)))
                {
                    ui->SEN_ID->setText(QString::number(Valor,10));
                    CambioPantalla(4);

                   Utilidades.Moduladoras(SenID);
                    EIndice = 40;
                }
                //Sensor de Caudal
                else if ((SenID >= 0x6C)&& (SenID <= 0x6D))
                {
                    Valor -= 0x6C;
                    ui->SEN_ID->setText(QString::number(Valor,10));
                    CambioPantalla(5);  //Pantalla Modulo GPS

                    ui->SEN_TIPO->setText("SCC");
                    EIndice = 50;
                }
                break;
                //Lectura de datos de los sensores de semilla
            case 10:
                //MEDICION
                bool LecSem;
                Valor = Lectura.toInt(&ok,16);
                LecSem = Valor & 0x8000;
                Valor = Valor & 0x0FFF;

                if (LecSem)
                {
                    ui->SEM_MED->setStyleSheet("QLineEdit { background-color: yellow }");
                }
                else
                {
                    ui->SEM_MED->setStyleSheet("QLineEdit { background-color: green }");
                }
                ui->SEM_MED->setText(QString::number(Valor,10));
                EIndice ++;
                break;
            case 11:
                // Lectura de tubo sucio
                Valor = Lectura.toInt(&ok,16);
                if (Valor)
                {
                    ui->SEM_TUBO->setText("Tubo Ok");
                    ui->SEM_TUBO->setStyleSheet("QLineEdit { background-color: green }");
                }
                else
                {
                    ui->SEM_TUBO->setText("Tubo Sucio");
                    ui->SEM_TUBO->setStyleSheet("QLineEdit { background-color: pink }");
                }
                EIndice ++;
                break;
            case 12:
                Valor = Lectura.toInt(&ok,16);
                ui->SEM_TMED->setText(QString::number(Valor,10));
                EIndice ++;
                break;
            case 13:
                Valor = Lectura.toInt(&ok,16);
                ui->SEM_TDES->setText(QString::number(Valor,10));
                EIndice ++;
                break;
            case 14:
                Valor = Lectura.toInt(&ok,16);
                ui->SEM_ACT->setText(QString::number(Valor,10));
                EIndice = 1;
                break;
                //----------------------------------------------------------------------
                //     Lectura de datos del GPS
                //----------------------------------------------------------------------
            case 20:
                Posicion = Lectura.mid(20,8).toLongLong(&ok,16);
                ui->GPS_LAT->setText(QString::number(Posicion,10));

                Posicion = Lectura.mid(12,8).toLongLong(&ok,16);
                ui->GPS_LON->setText(QString::number(Posicion,10));

                Valor = Lectura.mid(8,4).toInt(&ok,16);
                ui->GPS_VEL->setText(QString::number(Valor,10));

                Valor = Lectura.mid(6,2).toInt(&ok,16);
                ui->GPS_CUR->setText(QString::number(Valor,10));

                Valor = Lectura.mid(4,2).toInt(&ok,16);
                ui->GPS_SAT->setText(QString::number(Valor,10));

                Valor = Lectura.mid(0,4).toInt(&ok,16);
                ui->GPS_ANT->setText(QString::number(Valor,10));
                EIndice = 1;
                break;
                //----------------------------------------------------------------------
                //     Lectura de datos de sensores de Rotacion y de RPM
                //----------------------------------------------------------------------
            case 30:
                Valor = Lectura.toInt(&ok,16);
                ui->RPM_MED->setText(QString::number(Valor,10));
                EIndice = 31;
                break;
            case 31:
                Valor = Lectura.toInt(&ok,16);
                Aux = Valor / 100;
                Datos.clear();
                Datos.append(QString::number(Aux,10));
                Aux = Valor % 100;
                if(Aux<10)
                    Datos.append(".0");
                else
                    Datos.append(".");
                Datos.append(QString::number(Aux,10));
                ui->RPM_FK->setText(Datos);
                if (RPM_TRB)
                {
                    ui->RPM_UNIDAD->setText("Pls/Rev");
                    EIndice = 1;
                }
                else
                {
                    ui->RPM_UNIDAD->setText("Pls/Rev");
                    EIndice = 32;
                }
                break;
            case 32:
                Valor = Lectura.toInt(&ok,16);
                ui->RPM_ALMAX->setText(QString::number(Valor,10));
                EIndice = 33;
                break;
            case 33:
                Valor = Lectura.toInt(&ok,16);
                ui->RPM_ALMIN->setText(QString::number(Valor,10));
                EIndice = 1;
                break;
                //----------------------------------------------------------------------
                //     Lectura de datos de las moduladoras
                //----------------------------------------------------------------------
            case 40:
                //Lectura del SP y KD
                Valor = Lectura.mid(0,4).toInt(&ok,16);

                ui->MOD_SP->setText(QString::number(Valor,10));
                Valor = Lectura.mid(4,4).toInt(&ok,16);

                ui->MOD_KD->setText(QString::number(Valor,10));

                EIndice = 41;
                break;
            case 41:
                //Lectura del SP y KD
                Valor = Lectura.toInt(&ok,16);
                ui->MOD_FK->setText(QString::number(Valor,10));
                EIndice = 42;
                break;
            case 42:
                //Lectura del SP y KD
                Valor = Lectura.mid(0,2).toInt(&ok,16);

                ui->MOD_DT->setText(QString::number(Valor,10));
                Valor = Lectura.mid(2,2).toInt(&ok,16);

                ui->MOD_DD->setText(QString::number(Valor,10));
                Valor = Lectura.mid(4,4).toInt(&ok,16);
                ui->MOD_RT->setText(QString::number(Valor,10));
                EIndice = 1;
                break;
            case 50:
                Valor = Lectura.toInt(&ok,16);
                seg = Valor % 60;
                Valor = Valor / 60;
                min = Valor % 60;
                hrs = Valor / 60;
                ui->CAU_TMT->setText(QString::number(hrs,10) + ":" +
                                     QString::number(min,10) + ":" +
                                     QString::number(seg,10));
                EIndice = 52;
                break;
            case 51:
                //Falta leer los datos
                Valor = Lectura.toInt(&ok,16);
                seg = Valor % 60;
                Valor = Valor / 60;
                min = Valor % 60;
                hrs = Valor / 60;
//                ui->CAU_TMP->setText(QString::number(hrs,10) + ":" +
//                                     QString::number(min,10) + ":" +
//                                     QString::number(seg,10));
                EIndice = 52;
                break;
            case 52:
                Valor = Lectura.toInt(&ok,16);
                ui->CAU_CCT->setText(QString::number(Valor,10));
                EIndice = 54;
                break;
            case 53:
                Valor = Lectura.toInt(&ok,16);
//                ui->CAU_CCP->setText(QString::number(Valor,10));
                EIndice = 54;
                break;
            case 54:
                Valor = Lectura.toInt(&ok,16);
                ui->CAU_DESC->setText(QString::number(Valor,10));
                if(VSoft >= 200)
                    EIndice = 55;
                else
                    EIndice = 1;
                break;
            case 55:
                Valor = Lectura.toInt(&ok,16);
                ui->CAU_DESAT->setText(QString::number(Valor,10));
                EIndice = 56;
                break;
            case 56:
                Valor = Lectura.toInt(&ok,16);
                ui->CAU_BMAG->setText(QString::number(Valor,10));
                EIndice = 57;
                break;
            case 57:
                Valor = Lectura.toInt(&ok,16);
                min = Valor % 60;
                hrs = Valor / 60;
               ui->CAU_TBMAG->setText(QString::number(hrs,10) + ":" +
                                     QString::number(min,10));
                EIndice = 58;
                break;
            case 58:
                Patente.append((LectASCII(Lectura)));
                EIndice = 59;
                break;

            case 59:
                Patente.append((LectASCII(Lectura)));
                ui->CAU_INST->setText(Patente);
                EIndice = 1;
                break;
            }
        }
        DatosLin.clear();
    }
}

QByteArray Reparaciones::LecturaLIN(QByteArray Datos)
{
    QByteArray Deco;
    int tam;
    int i;
    Deco.clear();
    if(Datos.contains('!'))
    {
        Deco.append("!");
        return Deco;
    }
    i   = Datos.indexOf("\n");
    tam = Datos.indexOf("\r");
    tam = tam < i ? tam : i;
    if(tam) tam --;
    if(tam) tam --;

    while(tam >= 1)
    {
        Deco.append(Datos.mid(tam,2));
        if(tam) tam --;
        if(tam) tam --;
    }
    return Deco;
}
QByteArray Reparaciones::LectASCII(QByteArray Texto)
{
    QByteArray Lect;
    QByteArray Ax;
    int tam;
    Lect.clear();
    Ax.clear();
    tam = Texto.size();
    do
    {
        if(tam) tam --;
        if(tam) tam --;
        Ax.append(Texto.mid(tam,2));
    }while(tam >= 1);

    Lect = QByteArray::fromHex(Ax);

    return Lect;
}
