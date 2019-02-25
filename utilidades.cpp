
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include "reparaciones.h"
#include "ui_reparaciones.h"



void Reparaciones::BorraMonitores()
{
  // QMessageBox::information(this,tr("Borra Datos Monitores"),tr("aca borro los datos de los monitores"));
//    ui->MON_NSerie->clear();
//    ui->MON_COM->clear();
//    ui->MON_BON->setCurrentIndex(0);
}

void Reparaciones::BorraPerifericos()
{
    QMessageBox::about(this,tr("Borrado Datos Sensores"), tr("borramos los datos de los sensores de semilla"));
//    Item = 1;
 //   ui->SEN_ID->clear();
//    ui->SEN_FF->clear();
//    ui->SEN_FI->clear();
//    ui->SEN_FS->clear();
//    ui->SEN_ID->clear();
////    ui->SEN_ITEM->setText(QString::number(Item,10));
//    ui->SEN_NSERIE->clear();
//    ui->SEN_TIPO->clear();
//    ui->SEN_VS->clear();

}
void Reparaciones::BorraSensores()
{
//    ui->S_TIPO->setCurrentIndex(0);
////    ui->S_BON->setCurrentIndex(0);
//    ui->S_MED->clear();
////    ui->S_CAD->setChecked(false);
////    ui->S_CCD->setChecked(false);
////    ui->S_PRS->setChecked(false);
////    ui->S_PES->setChecked(false);
////    ui->S_FOK->setChecked(false);
//    ui->S_TD->clear();
//    ui->S_TM->clear();
//    ui->S_CA->clear();
//    ui->S_COM->clear();

}
void Reparaciones::BorraRMP()
{
    QMessageBox::information(this,tr("Datos RPM Turbina"), tr("Borra los datos de los sensores de RPM y turbina"));
//    ui->RPM_ALMAX->clear();
//    ui->RPM_ALMIN->clear();
//    ui->RPM_MED->clear();
//    ui->RPM_FK->clear();
//    ui->RPM_COM->clear();
//    ui->RPM_UNIDAD->setText("__ * __");

}
void Reparaciones::BorraMOD()
{
//    ui->MOD_COM->clear();
//    ui->MOD_FK->clear();
//    ui->MOD_MED->clear();
//    ui->MOD_DD->clear();
//    ui->MOD_DT->clear();
//    ui->MOD_RT->clear();
//    ui->MOD_SP->clear();
//    ui->MOD_KD->clear();
}
void Reparaciones::BorraGPS()
{
//    ui->GPS_ANT->clear();
//    ui->GPS_COM->clear();
//    ui->GPS_LAT->clear();
//    ui->GPS_LON->clear();
//    ui->GPS_CUR->clear();
//    ui->GPS_SAT->clear();
//    ui->GPS_VEL->clear();

}


void Reparaciones::MensajeTrabajo()
{
    QMessageBox::information(this, tr("Trabajo"),
                          tr("Seleccionar Trabajo para cargar datos"));
}

bool Reparaciones::DobleGuardadoMsg()
{
    QMessageBox msgBox;
    msgBox.setText("Este equipo ya se guardo ");
    msgBox.setInformativeText("Deseas guardar de todas formas?");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    int elegido = msgBox.exec();
    switch (elegido) {
       case QMessageBox::Ok:
            return false;
           break;
       case QMessageBox::Cancel:
           return true;
           break;
     }
    return false;
}

