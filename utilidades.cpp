
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

 void MainWindow::BorraIngreso()
{
//     QMessageBox::critical(this, tr("Error"), tr("Borra los datos del cliente"));
     ui->Agente->clear();
     ui->Operario->clear();
     ui->Fecha_Ingreso->clear();
     ui->Fecha_Control->clear();
     ui->Fecha_Control->setText(FechaActual.toString("dd/MM/yyyy"));
     ui->Ingreso_Equipo->setCurrentIndex(0);
     ui->Ingreso_Cantidad->setValue(0);
     ui->Ingreso_Comentario->clear();
}

void MainWindow::BorraMonitores()
{
  // QMessageBox::information(this,tr("Borra Datos Monitores"),tr("aca borro los datos de los monitores"));
    ui->MON_NSerie->clear();
    ui->MON_COM->clear();
    ui->MON_TIPO->setCurrentIndex(0);
    ui->MON_BON->setCurrentIndex(0);


}

void MainWindow::BorraPerifericos()
{
 //   QMessageBox::about(this,tr("Borrado Datos Sensores"), tr("borramos los datos de los sensores de semilla"));
    Item = 1;
    ui->SEN_ID->clear();
    ui->SEN_FF->clear();
    ui->SEN_FI->clear();
    ui->SEN_FS->clear();
    ui->SEN_ID->clear();
    ui->SEN_ITEM->setText(QString::number(Item,10));
    ui->SEN_NSERIE->clear();
    ui->SEN_TIPO->clear();
    ui->SEN_VS->clear();

}
void MainWindow::BorraSensores()
{
    ui->S_TIPO->setCurrentIndex(0);
    ui->S_BON->setCurrentIndex(0);
    ui->S_MED->clear();
//    ui->S_CAD->setChecked(false);
//    ui->S_CCD->setChecked(false);
//    ui->S_PRS->setChecked(false);
//    ui->S_PES->setChecked(false);
//    ui->S_FOK->setChecked(false);
    ui->S_TD->clear();
    ui->S_TM->clear();
    ui->S_CA->clear();
    ui->S_COM->clear();

}
void MainWindow::BorraRMP()
{
//    QMessageBox::information(this,tr("Datos RPM Turbina"), tr("Borra los datos de los sensores de RPM y turbina"));
    ui->RPM_BON->setCurrentIndex(0);
    ui->RPM_ALMAX->clear();
    ui->RPM_ALMIN->clear();
    ui->RPM_MED->clear();
    ui->RPM_FK->clear();
    ui->RPM_COM->clear();
    ui->RPM_UNIDAD->setText("__ * __");

}
void MainWindow::BorraMOD()
{
    ui->MOD_BON->setCurrentIndex(0);
    ui->MOD_COM->clear();
    ui->MOD_FK->clear();
    ui->MOD_MED->clear();
    ui->MOD_DD->clear();
    ui->MOD_DT->clear();
    ui->MOD_RT->clear();
    ui->MOD_SP->clear();
    ui->MOD_KD->clear();
}
void MainWindow::BorraGPS()
{
    ui->GPS_BON->setCurrentIndex(0);
    ui->GPS_ANT->clear();
    ui->GPS_COM->clear();
    ui->GPS_LAT->clear();
    ui->GPS_LON->clear();
    ui->GPS_CUR->clear();
    ui->GPS_SAT->clear();
    ui->GPS_VEL->clear();

}
void MainWindow::BorraINS()
{
    ui->INS_BON->setCurrentIndex(0);
    ui->INS_NSerie->clear();
    ui->INS_COM->clear();
    ui->INS_TIPO->setCurrentIndex(0);
}

void MainWindow:: TituloColumnas()
{
    Columnas = true;
    EscColumnas = true;
    ui->Datos->append("Item;Equipo;NúmeroSerie;Fecha Fab.;Fecha Inst.;V.Soft;F.Soft;Conf;Fallas;Bonificación;Comentario");
}

void MainWindow::CambioPantalla(int Pant)
{
    PantallaActual = Pant;
    switch(Pant)
    {
    //Pantalla de Semillas y Fertilizante
    case 1:
        ui->FSemilla->setVisible(true);
        ui->FGPS->setVisible(false);
        ui->FROT->setVisible(false);
        ui->FMOD->setVisible(false);
        ui->FCAU->setVisible(false);
    break;
    case 2:
        ui->FSemilla->setVisible(false);
        ui->FGPS->setVisible(true);
        ui->FROT->setVisible(false);
        ui->FMOD->setVisible(false);
        ui->FCAU->setVisible(false);
    break;
    case 3:
        ui->FSemilla->setVisible(false);
        ui->FGPS->setVisible(false);
        ui->FROT->setVisible(true);
        ui->FMOD->setVisible(false);
        ui->FCAU->setVisible(false);
    break;
    case 4:
        ui->FSemilla->setVisible(false);
        ui->FGPS->setVisible(false);
        ui->FROT->setVisible(false);
        ui->FMOD->setVisible(true);
        ui->FCAU->setVisible(false);
    break;
    case 5:
        ui->FSemilla->setVisible(false);
        ui->FGPS->setVisible(false);
        ui->FROT->setVisible(false);
        ui->FMOD->setVisible(false);
        ui->FCAU->setVisible(true);
    break;
    }
}

void MainWindow::Moduladoras(int ID)
{
    switch(ID)
    {

        break;
        case 0x68:
        //Siembra Primaria 			#1
            ui->SEN_TIPO->setText("Mod Siem Prim 1");
        break;
        case 0x6A:
        //Siembra Secundaria 		#1
            ui->SEN_TIPO->setText("Mod Siem Sec 1");
        break;
        case 0x69:
        //Fertilizante Primario 	#1
            ui->SEN_TIPO->setText("Mod Fert Prim 1");
        break;
        case 0x6B:
        //Fertilizante Secundario 	#1
            ui->SEN_TIPO->setText("Mod Fert Sec 1");
        break;
        case 0xC8:
        //Siembra Primaria 			#2
            ui->SEN_TIPO->setText("Mod Siem Prim 2");
        break;
        case 0xCA:
        //Siembra Secundaria 		#2
            ui->SEN_TIPO->setText("Mod Siem Sec 2");
        break;
        case 0xC9:
        //Fertilizante Primario 	#2
            ui->SEN_TIPO->setText("Mod Fert Prim 2");
        break;
        case 0xCB:
        //Fertilizante Secundario 	#2
            ui->SEN_TIPO->setText("Mod Fert Sec 2");
        break;
        case 0xCC:
        //Siembra Primaria 			#3
            ui->SEN_TIPO->setText("Mod Siem Prim 3");
        break;
        case 0xCE:
        //Siembra Secundaria 		#3
            ui->SEN_TIPO->setText("Mod Siem Sec 3");
        break;
        case 0xCD:
        //Fertilizante Primario 	#3
            ui->SEN_TIPO->setText("Mod Fert Prim 3");
        break;
        case 0xCF:
        //Fertilizante Secundario	#3
            ui->SEN_TIPO->setText("Mod Fert Sec 3");
        break;
        case 0x64:
        //Siembra Primaria 			#4
            ui->SEN_TIPO->setText("Mod Siem Prim 4");
        break;
        case 0x66:
        //Siembra Secundaria 		#4
            ui->SEN_TIPO->setText("Mod Siem Sec 4");
        break;
        case 0x65:
        //Fertilizante Primario 	#4
            ui->SEN_TIPO->setText("Mod Fert Prim 4");
        break;
        case 0x67:
        //Fertilizante Secundario	#4
            ui->SEN_TIPO->setText("Mod Fert Sec 4");
        break;
    }
}
void MainWindow :: BonificacionMsg ()
{
    QMessageBox::information(this, tr("Bonificaciòn"),
                          tr("Falta cargar la bonificacion del trabajo"));
}

void MainWindow :: EncabezadoMsg()
{
    QMessageBox::information(this, tr("Encabezado Archivo"),
                          tr("Falta cargar datos del encavezado del archivo"));
}
void MainWindow:: Mascaras()
{
    ui->Fecha_Ingreso->setInputMask("00/00/0000");
    ui->SEN_VS->setInputMask("0.00");
   // ui->SEN_FF->setInputMask("00/00/00");
   // ui->SEN_FI->setInputMask("00/00/00");
    ui->SEN_FS->setInputMask("00/00/00");
    ui->GPS_LAT->setInputMask("#00.00000000");
    ui->GPS_LON->setInputMask("#00.00000000");
}
