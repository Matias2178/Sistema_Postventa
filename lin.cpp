#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDebug>

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
    //        qDebug () << LIndice <<  "$406C0604F2";
            break;
        case 59:
        //PATENTE PARTE 2
            serial->write("$406C0604F3\r\n");
            LIndice = 59;
            break;

        }
    }
}


void MainWindow::LIN_Lectura()
{
    unsigned long Valor;
    long Posicion;
    unsigned int DD, MM, AA;
    unsigned int Aux;
    int hrs;
    int min;
    int seg;
    bool ok;
    char letra;
    QString Datos;
    QByteArray CC;
    QDateTime FechaFab;

    DatosLin.append(serial->readAll());
    if(DatosLin.contains(0x0A) )
    {
        Escribir = true;

        ui->LectLIN->append(DatosLin);  //Muestro los datos en pantalla

        Lectura.clear();
        Lectura.append(LecturaLIN(DatosLin));
        if(DatosLin.contains("!"))
        {

//            ui->SEN_ID->setStyleSheet("QLineEdit { background-color: red }");
//            EIndice = 1;
//            Guardar = false;
//            NSerie = 0;
            //  Guardar = false;
            //  Siguiente = false;
        }
        else
        {
  //          ui->SEN_ID->setStyleSheet("QLineEdit { background-color: yellow }");
            switch (LIndice)
            {
            case 1:
                //NUMERO DE SERIE
                Valor = Lectura.toLong(&ok,16);

//                if(Siguiente)
//                {
//                    EIndice = 0;
//                    return;
//                }

//                if(NSerie &&  (NSerie != Valor) && !Guardar)
//                {
//                    QString Lectura = QString("Sensor Anterior %1 Actual %2").arg(NSerie).arg(Valor);
//                    QMessageBox::information(this, tr("Info"), Lectura);
//                    EIndice = 0;
//                    Siguiente = true;
//                    ui->Siguiente->setEnabled(true);
//                    return;
//                }
//                NSerie = Valor;
//                ui->SEN_NSERIE->setText(QString::number(Valor,10));
//                EIndice ++;
                break;
            case 2:
                //FECHA DE FABRICACION
                Valor = Lectura.toLong(&ok,16);
                FechaFab.setTime_t(Valor);
                //  qDebug() << Valor;
//                ui->SEN_FF->setText(FechaFab.toString("dd/MM/yyyy"));
                //    ui->SEN_FF->setText(QString::number(Valor,10));
                //    qDebug() << FechaFab.toString("dd/MM/yyyy");
                EIndice ++;
                break;
            case 3:
                //FECHA DE INSTALACION
                Valor = Lectura.toLong(&ok,16);
                //   qDebug() << Valor;
                FechaFab.setTime_t(Valor);
                //   qDebug() << FechaFab;
//                if(Valor)
//                {
//                    ui->SEN_FI->setText(FechaFab.toString("dd/MM/yyyy"));
//                }
//                else
//                {
//                    ui->SEN_FI->setText("--/--/--");
//                }
                // ui->SEN_FI->setText(QString::number(Valor,10));
                EIndice ++;
                break;
            case 4:
                //VERSION DEL SOFTWARE Y FECHA DE COMPILACION
//                CC.clear();
//                CC[0] = Lectura[6];
//                CC[1] = Lectura[7];
//                Valor = CC.toInt(&ok,16);
                Valor = Lectura.mid(0,2).toInt(&ok,16);
                VSoft = Valor;
//              ui->SEN_VS->setText(QString::number(Valor,10));
//qDebug () << CC << VSoft;

//                CC.clear();
//                CC[0] = Lectura[4];
//                CC[1] = Lectura[5];
//                DD = CC.toInt(&ok,16);
                DD = Lectura.mid(0,2).toInt(&ok,16);
//                CC[0] = Lectura[2];
//                CC[1] = Lectura[3];
//                MM = CC.toInt(&ok,16);
                MM = Lectura.mid(2,2).toInt(&ok,16);
//                CC[0] = Lectura[0];
//                CC[1] = Lectura[1];
//                AA = CC.toInt(&ok,16);
                AA = Lectura.mid(4,2).toInt(&ok,16);
           //     Valor = CC.toLong(&ok,10);

                CC.clear();
//                if(DD<10)
//                    CC.append("0");
                CC.append(QString::number(DD,10).rightJustified(2,'0'));
//                if(MM<10)
//                    CC.append("0");
                CC.append(QString::number(MM,10).rightJustified(2,'0'));
//                if(AA<10)
//                    CC.append("0");
                CC.append(QString::number(AA,10).rightJustified(2,'0'));

//                ui->SEN_FS->setText(CC);
                EIndice ++;
                break;
            case 5:
                //LECTURA DE ID
//                Lectura.clear();
//                Lectura[0] = DatosLin[1];
//                Lectura[1] = DatosLin[2];

//                DatosLin.left(1) ;
                Valor = Lectura.toShort(&ok,16);
                SenID = Valor;
                Valor ++;
                //   ui->SEN_ID->setText(QString::number(Valor,10));

                //Sensor de caida
 //              qDebug() << "Sensor:" << SenID;
                if((SenID < 63) || (SenID ==240))
                {
//                    if(SenID ==240)
//                        ui->SEN_ID->setText("SinID");
//                    else
//                        ui->SEN_ID->setText(QString::number(Valor,10));

//                    CambioPantalla(1);  //Pantalla Sensores de semilla
//                    ui->SEN_TIPO->setText("Caida");
//                    EIndice = 10;
                    if(SenID ==240)
                    {
//                        ui->S_NOM->setText("Sensor Virgen");
                        // ui->SEN_TIPO->setText("S. Virgen");
                    }
                    else if(SenID < 31)
                    {
//                        ui->S_NOM->setText("Sensor Semilla");
                        // ui->SEN_TIPO->setText("S. Semilla");
                    }
                    else if(SenID < 63)
                    {
//                        ui->SEN_TIPO->setText("S. Fertilizante");
                        //  ui->S_NOM->setText("Sensor Fertilizante");
                    }
                }
                //Modulo GPS
                else if ((SenID == 214))
                {
                    Valor -= 214;
//                    ui->SEN_ID->setText(QString::number(Valor,10));
//                    CambioPantalla(2);  //Pantalla Modulo GPS

//                    ui->SEN_TIPO->setText("GPS");
                    EIndice = 20;
                }
                //Sensor de Rotacion
                else if(((SenID >= 0x40) && (SenID <= 0x47))|| (SenID == 0xF1))
                {

                    Valor -=0x40;
//                    if(SenID ==0xF1)
//                        ui->SEN_ID->setText("SinID");
//                    else
//                        ui->SEN_ID->setText(QString::number(Valor,10));

//                    CambioPantalla(3);  //Pantalla Sensores Turbina y Rotacion

//                    ui->SEN_TIPO->setText("ROTACION");
                    RPM_TRB = true;
                    EIndice = 30;
                }
                //Sensor de Turbina
                else if((SenID >= 0xD3) && (SenID <= 0xD5))
                {
                    Valor -= 0xD3;
//                    ui->SEN_ID->setText(QString::number(Valor,10));
//                    CambioPantalla(3);  //Pantalla Sensores Turbina y Rotacion

//                    ui->SEN_TIPO->setText("TURBINA");
                    RPM_TRB = false;
                    EIndice = 30;
                }
                //Sensor de Turbina
                else if((SenID >= 0xD1) && (SenID <= 0xD2))
                {
                    Valor -= 0xD3;
//                    ui->SEN_ID->setText(QString::number(Valor,10));
//                    CambioPantalla(3);  //Pantalla Sensores Turbina y Rotacion

//                    ui->SEN_TIPO->setText("VELOCIDAD SECUNDARIO*+"
//                                          ""
//                                          "+-*+*");
                    RPM_TRB = false;
                    EIndice = 30;
                }
                //Moduladoras
                else if(((SenID >= 0x64) && (SenID <= 0x6B))||((SenID >= 0xC8) && (SenID <= 0xCF)))
                {
//                    ui->SEN_ID->setText(QString::number(Valor,10));
//                    CambioPantalla(4);

//                    Moduladoras(SenID);
                    EIndice = 40;
                }
                else if ((SenID >= 0x6C))
                {
                    Valor -= 0x0F;
//                    ui->SEN_ID->setText(QString::number(Valor,10));
//                    CambioPantalla(5);  //Pantalla Modulo GPS

//                    ui->SEN_TIPO->setText("SCC");
                    EIndice = 50;  
                }
                break;
                //Lectura de datos de los sensores de semilla
            case 10:
                //MEDICION
                bool LecSem;
//                Lectura.clear();
//                Lectura[0] = DatosLin[3];
//                Lectura[1] = DatosLin[4];
//                Lectura[2] = DatosLin[1];
//                Lectura[3] = DatosLin[2];

                Valor = Lectura.toInt(&ok,16);
                LecSem = Valor & 0x8000;
                Valor = Valor & 0x0FFF;

//                if (LecSem)
//                {
//                    ui->S_MED->setStyleSheet("QLineEdit { background-color: yellow }");
//                }
//                else
//                {
//                    ui->S_MED->setStyleSheet("QLineEdit { background-color: green }");
//                }
//                ui->S_MED->setText(QString::number(Valor,10));
                EIndice ++;
                break;
            case 11:
                // Lectura de tubo sucio
//                Lectura.clear();
//                Lectura[0] = DatosLin[1];
                Valor = Lectura.toInt(&ok,16);
//                if (Valor)
//                {
//                    ui->S_Tubo->setText("Tubo Ok");
//                    ui->S_Tubo->setStyleSheet("QLineEdit { background-color: green }");
//                }
//                else
//                {
//                    ui->S_Tubo->setText("Tubo Sucio");
//                    ui->S_Tubo->setStyleSheet("QLineEdit { background-color: pink }");
//                }
                EIndice ++;
                break;
            case 12:
//                Lectura.clear();
//                Lectura[0] = DatosLin[1];
                Valor = Lectura.toInt(&ok,16);
//                ui->S_TM->setText(QString::number(Valor,10));
                EIndice ++;
                break;
            case 13:
//                Lectura.clear();
//                Lectura[0] = DatosLin[1];
                Valor = Lectura.toInt(&ok,16);
//                ui->S_TD->setText(QString::number(Valor,10));
                EIndice ++;
                break;
            case 14:
//                Lectura.clear();
//                Lectura[0] = DatosLin[1];
                Valor = Lectura.toInt(&ok,16);
//                ui->S_CA->setText(QString::number(Valor,10));
                EIndice = 1;
                break;
                //----------------------------------------------------------------------
                //     Lectura de datos del GPS
                //----------------------------------------------------------------------
            case 20:
//                Lectura.clear();
//                Lectura[0] = DatosLin[7];
//                Lectura[1] = DatosLin[8];
//                Lectura[2] = DatosLin[5];
//                Lectura[3] = DatosLin[6];
//                Lectura[4] = DatosLin[4];
//                Lectura[5] = DatosLin[3];
//                Lectura[6] = DatosLin[2];
//                Lectura[7] = DatosLin[1];
                Posicion = Lectura.mid(0,8).toLongLong(&ok,16);
          //      qDebug () << "LAT" << Posicion;
                //              qDebug () << "LAT" << Lectura;
//                ui->GPS_LAT->setText(QString::number(Posicion,10));

//                Lectura.clear();
//                Lectura[0] = DatosLin[15];
//                Lectura[1] = DatosLin[16];
//                Lectura[2] = DatosLin[13];
//                Lectura[3] = DatosLin[14];
//                Lectura[4] = DatosLin[11];
//                Lectura[5] = DatosLin[12];
//                Lectura[6] = DatosLin[9];
//                Lectura[7] = DatosLin[10];
                Posicion = Lectura.mid(8,8).toLongLong(&ok,16);
          //      qDebug () << "LON:" << Posicion;
                //               qDebug () << "Lon" << Lectura;
//                ui->GPS_LON->setText(QString::number(Posicion,10));

//                Lectura.clear();
//                Lectura[0] = DatosLin[19];
//                Lectura[1] = DatosLin[20];
//                Lectura[2] = DatosLin[17];
//                Lectura[3] = DatosLin[18];
                Valor = Lectura.mid(16,4).toInt(&ok,16);

           //     qDebug () << "vel:" << Valor;
                //              qDebug () << "vel" << Lectura;

//                ui->GPS_VEL->setText(QString::number(Valor,10));

//                Lectura.clear();
//                Lectura[0] = DatosLin[21];
//                Lectura[1] = DatosLin[22];
                Valor = Lectura.mid(20,2).toInt(&ok,16);

          //     qDebug () << "CUR:" << Valor;
                //               qDebug () << "cur" << Lectura;

//                ui->GPS_CUR->setText(QString::number(Valor,10));

//                Lectura.clear();
//                Lectura[0] = DatosLin[23];
//                Lectura[1] = DatosLin[24];
                Valor = Lectura.mid(22,2).toInt(&ok,16);

          //      qDebug () << "SAT:" << Valor;
                //                qDebug () << "sAT" << Lectura;

//                ui->GPS_SAT->setText(QString::number(Valor,10));

                Lectura.clear();
//                Lectura[0] = DatosLin[27];
//                Lectura[1] = DatosLin[28];
//                Lectura[2] = DatosLin[25];
//                Lectura[3] = DatosLin[26];
                Valor = Lectura.mid(24,4).toInt(&ok,16);

           //     qDebug () << "ANT:" << Valor;
                //                qDebug () << "AnT" << Lectura;

//                ui->GPS_ANT->setText(QString::number(Valor,10));
                EIndice = 1;
                break;
                //----------------------------------------------------------------------
                //     Lectura de datos de sensores de Rotacion y de RPM
                //----------------------------------------------------------------------
            case 30:
//                Lectura.clear();
//                Lectura[0] = DatosLin[3];
//                Lectura[1] = DatosLin[4];
//                Lectura[2] = DatosLin[1];
//                Lectura[3] = DatosLin[2];
                Valor = Lectura.toInt(&ok,16);
//                qDebug() << "Med:" << Valor;
//                qDebug() << "Med L:" << Lectura;
//                qDebug() << "Med D:" << DatosLin;
//                ui->RPM_MED->setText(QString::number(Valor,10));
                EIndice = 31;
                break;
            case 31:
//                Lectura.clear();
//                Lectura[0] = DatosLin[3];
//                Lectura[1] = DatosLin[4];
//                Lectura[2] = DatosLin[1];
//                Lectura[3] = DatosLin[2];
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
//                ui->RPM_FK->setText(Datos);
//                if (RPM_TRB)
//                {
//                    ui->RPM_UNIDAD->setText("Pls/Rev");
//                    ui->RPM_NOM->setText("Sensor de Rotacion");
//                    EIndice = 1;
//                }
//                else
//                {
//                    ui->RPM_UNIDAD->setText("Pls/Rev");
//                    ui->RPM_NOM->setText("Sensor de Turbina");
//                    EIndice = 32;
//                }
//                qDebug() << EIndice;
                break;
            case 32:
//                Lectura.clear();
//                Lectura[0] = DatosLin[3];
//                Lectura[1] = DatosLin[4];
//                Lectura[2] = DatosLin[1];
//                Lectura[3] = DatosLin[2];
                Valor = Lectura.toInt(&ok,16);

//                qDebug() << "Max:" << Valor;
//                ui->RPM_ALMAX->setText(QString::number(Valor,10));
                EIndice = 33;
                break;
            case 33:
//                Lectura.clear();
//                Lectura[0] = DatosLin[3];
//                Lectura[1] = DatosLin[4];
//                Lectura[2] = DatosLin[1];
//                Lectura[3] = DatosLin[2];
                Valor = Lectura.toInt(&ok,16);

//                qDebug() << "Min:" << Valor;
//                ui->RPM_ALMIN->setText(QString::number(Valor,10));
                EIndice = 1;
                break;
                //----------------------------------------------------------------------
                //     Lectura de datos de las moduladoras
                //----------------------------------------------------------------------
            case 40:
                //Lectura del SP y KD
//                Lectura.clear();
//                Lectura[0] = DatosLin[3];
//                Lectura[1] = DatosLin[4];
//                Lectura[2] = DatosLin[1];
//                Lectura[3] = DatosLin[2];
                Valor = Lectura.mid(0,4).toInt(&ok,16);

//                qDebug() << "SP?:" << Valor;
//                ui->MOD_SP->setText(QString::number(Valor,10));

//                Lectura.clear();
//                Lectura[0] = DatosLin[7];
//                Lectura[1] = DatosLin[8];
//                Lectura[2] = DatosLin[5];
//                Lectura[3] = DatosLin[6];
                Valor = Lectura.mid(4,4).toInt(&ok,16);

//                qDebug() << "KD?:" << Valor;
//                ui->MOD_KD->setText(QString::number(Valor,10));

                EIndice = 41;
                break;
            case 41:
                //Lectura del SP y KD
//                Lectura.clear();
//                Lectura[0] = DatosLin[3];
//                Lectura[1] = DatosLin[4];
//                Lectura[2] = DatosLin[1];
//                Lectura[3] = DatosLin[2];
//                Lectura[4] = DatosLin[7];
//                Lectura[5] = DatosLin[8];
//                Lectura[6] = DatosLin[5];
//                Lectura[7] = DatosLin[6];
                Valor = Lectura.toInt(&ok,16);

//                qDebug() << "Pul KM?:" << Valor;
//                ui->MOD_FK->setText(QString::number(Valor,10));
                EIndice = 42;
                break;
            case 42:
                //Lectura del SP y KD
//                Lectura.clear();
//                Lectura[0] = DatosLin[1];
//                Lectura[1] = DatosLin[2];
                Valor = Lectura.mid(0,2).toInt(&ok,16);

//                qDebug() << "DT?:" << Valor;
//                ui->MOD_DT->setText(QString::number(Valor,10));
//                Lectura[0] = DatosLin[3];
//                Lectura[1] = DatosLin[4];
                Valor = Lectura.mid(2,2).toInt(&ok,16);

//                ui->MOD_DD->setText(QString::number(Valor,10));

//                Lectura[0] = DatosLin[5];
//                Lectura[1] = DatosLin[6];
//                Lectura[2] = DatosLin[7];
//                Lectura[3] = DatosLin[8];
                Valor = Lectura.mid(4,4).toInt(&ok,16);
//                ui->MOD_RT->setText(QString::number(Valor,10));
                EIndice = 1;
                break;
            case 50:
//                Lectura.clear();
//                Lectura[0] = DatosLin[5];
//                Lectura[1] = DatosLin[6];
//                Lectura[2] = DatosLin[3];
//                Lectura[3] = DatosLin[4];
//                Lectura[4] = DatosLin[1];
//                Lectura[5] = DatosLin[2];
                Valor = Lectura.toInt(&ok,16);
                seg = Valor % 60;
                Valor = Valor / 60;
                min = Valor % 60;
                hrs = Valor / 60;
//                ui->CAU_TMT->setText(QString::number(hrs,10) + ":" +
//                                     QString::number(min,10) + ":" +
//                                     QString::number(seg,10));
                EIndice = 52;
                break;
            case 51:
                //Falta leer los datos
//                Lectura.clear();
//                Lectura[0] = DatosLin[5];
//                Lectura[1] = DatosLin[6];
//                Lectura[2] = DatosLin[3];
//                Lectura[3] = DatosLin[4];
//                Lectura[4] = DatosLin[1];
//                Lectura[5] = DatosLin[2];
                Valor = Lectura.toInt(&ok,16);
                seg = Valor % 60;
                Valor = Valor / 60;
                min = Valor % 60;
                hrs = Valor / 60;
         //       ui->CAU_TMP->setText(QString::number(hrs,10) + ":" +
         //                            QString::number(min,10) + ":" +
         //                            QString::number(seg,10));
                EIndice = 52;
                break;
            case 52:
//                Lectura.clear();
//                Lectura[0] = DatosLin[5];
//                Lectura[1] = DatosLin[6];
//                Lectura[2] = DatosLin[3];
//                Lectura[3] = DatosLin[4];
//                Lectura[4] = DatosLin[1];
//                Lectura[5] = DatosLin[2];
                Valor = Lectura.toInt(&ok,16);
//                ui->CAU_CCT->setText(QString::number(Valor,10));
                EIndice = 54;
                break;
            case 53:
//                Lectura.clear();
//                Lectura[0] = DatosLin[5];
//                Lectura[1] = DatosLin[6];
//                Lectura[2] = DatosLin[3];
//                Lectura[3] = DatosLin[4];
//                Lectura[4] = DatosLin[1];
//                Lectura[5] = DatosLin[2];
                Valor = Lectura.toInt(&ok,16);
            //    ui->CAU_CCP->setText(QString::number(Valor,10));
                EIndice = 54;
                break;
            case 54:
//                Lectura.clear();
//                Lectura[0] = DatosLin[5];
//                Lectura[1] = DatosLin[6];
//                Lectura[2] = DatosLin[3];
//                Lectura[3] = DatosLin[4];
//                Lectura[4] = DatosLin[1];
//                Lectura[5] = DatosLin[2];
                Valor = Lectura.toInt(&ok,16);
//                ui->CAU_DESC->setText(QString::number(Valor,10));
                if(VSoft >= 200)
                    EIndice = 55;
                else
                    EIndice = 1;
                break;
            case 55:
//                Lectura.clear();
//                Lectura[0] = DatosLin[5];
//                Lectura[1] = DatosLin[6];
//                Lectura[2] = DatosLin[3];
//                Lectura[3] = DatosLin[4];
//                Lectura[4] = DatosLin[1];
//                Lectura[5] = DatosLin[2];
                Valor = Lectura.toInt(&ok,16);
//                ui->CAU_DESAT->setText(QString::number(Valor,10));
                EIndice = 56;
                break;
            case 56:
//                Lectura.clear();
//                Lectura[0] = DatosLin[5];
//                Lectura[1] = DatosLin[6];
//                Lectura[2] = DatosLin[3];
//                Lectura[3] = DatosLin[4];
//                Lectura[4] = DatosLin[1];
//                Lectura[5] = DatosLin[2];
                Valor = Lectura.toInt(&ok,16);
//                ui->CAU_BMAG->setText(QString::number(Valor,10));
                EIndice = 57;
                break;
            case 57:
//                Lectura.clear();
//                Lectura[0] = DatosLin[5];
//                Lectura[1] = DatosLin[6];
//                Lectura[2] = DatosLin[3];
//                Lectura[3] = DatosLin[4];
//                Lectura[4] = DatosLin[1];
//                Lectura[5] = DatosLin[2];
                Valor = Lectura.toInt(&ok,16);
                min = Valor % 60;
                hrs = Valor / 60;
//               ui->CAU_TBMAG->setText(QString::number(hrs,10) + ":" +
//                                     QString::number(min,10));
                EIndice = 58;
                break;
            case 58:

                Lectura.clear();
                Patente.clear();
//                Lectura[0] = DatosLin[1];
//                Lectura[1] = DatosLin[2];
//                letra = Lectura.toShort(&ok,16);
//                Patente.append(letra);
//                Lectura[0] = DatosLin[3];
//                Lectura[1] = DatosLin[4];
//                letra = Lectura.toShort(&ok,16);
//                Patente.append(letra);
//                Lectura[0] = DatosLin[5];
//                Lectura[1] = DatosLin[6];
//                letra = Lectura.toShort(&ok,16);
//                Patente.append(letra);
//                Lectura[0] = DatosLin[7];
//                Lectura[1] = DatosLin[8];
//                letra = Lectura.toShort(&ok,16);
//                Patente.append(letra);
                    Patente.append((LectASCII(Lectura)));
  //              qDebug () <<"Patente:"<< Patente;

                EIndice = 59;
                break;

            case 59:
//                Lectura.clear();
//                Lectura[0] = DatosLin[1];
//                Lectura[1] = DatosLin[2];
//                letra = Lectura.toShort(&ok,16);
//                Patente.append(letra);
//                Lectura[0] = DatosLin[3];
//                Lectura[1] = DatosLin[4];
//                letra = Lectura.toShort(&ok,16);
//                Patente.append(letra);
//                Lectura[0] = DatosLin[5];
//                Lectura[1] = DatosLin[6];
//                letra = Lectura.toShort(&ok,16);
//                Patente.append(letra);
//                Lectura[0] = DatosLin[7];
//                Lectura[1] = DatosLin[8];
//                letra = Lectura.toShort(&ok,16);
//                Patente.append(letra);

                Patente.append((LectASCII(Lectura)));
//                ui->CAU_INST->setText(Patente);
  //              qDebug () <<"Patente:"<< Patente;

                EIndice = 1;
                break;
            }
        }
        DatosLin.clear();
    }
}
QByteArray MainWindow::LecturaLIN(QByteArray Datos)
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
QByteArray MainWindow::LectASCII(QByteArray Texto)
{
    QByteArray Lectura;
    QByteArray Ax;
    int tam;
    Lectura.clear();
    Ax.clear();
    tam = Texto.size();
    do
    {
        if(tam) tam --;
        if(tam) tam --;
        Ax.append(Texto.mid(tam,2));
    }while(tam >= 1);

    Lectura.clear();
    Lectura = QByteArray::fromHex(Ax);

    return Lectura;
}
