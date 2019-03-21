#include "reporteinterno.h"
#include <QDebug>
#include <QFileDialog>

QPainter Impresora;

ReporteInterno::ReporteInterno(QObject *parent) : QObject(parent)
{

}

void ReporteInterno::SetDatos(int lID)
{
    this->Id = lID;
}


void ReporteInterno::RepInternoPDF(int Id, QString Ruta)
{  

    QPrinter printer;
    dbManejo dbReporte;   
    QString Texto, aFalla;
    QPen pen;
    QFont font;
    QSqlQuery consulta, consulta2;
    int Linea, Palabras;
    int eDato, ind;
    int i= 1;
    QDir Dir;

    Direccion.append(Dir.currentPath());

#define kSalto 18;
#define knl 11;



    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(Ruta);
    printer.setPageOrientation(QPageLayout::Portrait);

    if (!Impresora.begin(&printer)) { // failed to open file
        qWarning("failed to open file, is it writable?");
        return ;
    }

    font.setWeight(11);
    font.setPixelSize(12);
    font.setBold(true);
    Impresora.setFont(font);


    Linea = 45;

    consulta.prepare("SELECT * FROM Reparaciones WHERE id = " + QString::number(Id,10));
    consulta.exec();
    consulta.next();

    Impresora.drawText(50,Linea,"Agente:");
    Impresora.drawText(370,Linea,"ID:");


    font.setBold(false);
    font.setItalic(true);
    Impresora.setFont(font);
    Agente.clear();
    Agente.append(consulta.value("agente").toString());
    Impresora.drawText(103,Linea,Agente);

    Reporte.clear();
    Reporte.append(consulta.value("id").toString());
    Impresora.drawText(480,Linea,Reporte);

//------------
    Linea +=kSalto;
    font.setBold(true);
    font.setItalic(false);
    Impresora.setFont(font);

    Impresora.drawText(50,Linea,"Rem.Agente:");
    Impresora.drawText(200,Linea,"Rem.Trasp:");
    Impresora.drawText(370,Linea,"Fecha Trasnp:");

 //------------
    font.setBold(false);
    font.setItalic(true);
    Impresora.setFont(font);

    Impresora.drawText(140,Linea,consulta.value("ragente").toString());
    Impresora.drawText(280,Linea,consulta.value("rtransp").toString());
    Impresora.drawText(480,Linea,consulta.value("ftransp").toString());


//----------------
    Linea +=kSalto;
    font.setBold(true);
    font.setItalic(false);
    Impresora.setFont(font);

    Impresora.drawText(50,Linea,"Operario:");
    Impresora.drawText(200,Linea,"Fecha Ingreso:");
    Impresora.drawText(370,Linea,"Fecha Reparacion:");

//------------
    font.setBold(false);
    font.setItalic(true);
    Impresora.setFont(font);

    Impresora.drawText(110,Linea,consulta.value("Operario").toString());
    Impresora.drawText(290,Linea,consulta.value("fing").toString());
    Impresora.drawText(480,Linea,consulta.value("frep").toString());

//----------------
    Linea +=kSalto;
    font.setBold(true);
    font.setItalic(false);
    Impresora.setFont(font);

    Impresora.drawText(50,Linea,"Observaciones");

//------------
    font.setBold(false);
    font.setItalic(true);
    Impresora.setFont(font);

    Impresora.drawText(140,Linea,consulta.value("obs").toString());
    Linea +=kSalto;

    font.setWeight(9);
    font.setPixelSize(9);

    Impresora.drawPixmap(QRect(600,10,100,60),QPixmap(Direccion + "/SIID.png"));
//    font.setStyle(QFont::Courier);
    font.setStyleName("Courier");
    font.setBold(true);
    font.setItalic(false);
    Impresora.setFont(font);

    DivLinea(Linea);
//-------------------------------------------------------------------------
//                          Monitores
//-------------------------------------------------------------------------
    consulta.prepare("SELECT * FROM Monitores WHERE repid = " + QString::number(Id,10));
    consulta.exec();
//consulta.next();
    while(consulta.next())
    {
        Texto.clear();
        Texto.append(consulta.value("falla").toString());
        Palabras = Texto.count(" ");
//         a = Palabras;
        Palabras += 5;

        Palabras *= knl;

 //       qDebug() << Texto<< a << i<< ":" << Palabras << "*"<< "11 + " << Linea  << "="<< Palabras + Linea;

        if((Linea + Palabras) >=1065 )
        {

            if (! printer.newPage()) {
                qWarning("failed in flushing page to disk, disk full?");
                return;
            }
            Linea = SaltoPagina();
//            qDebug()<< "NP" << i<< ":" << a << "*"<< "11 + " << Linea  << "="<< Palabras + Linea;

        }
        Linea += knl;
        font.setBold(true);
        font.setItalic(false);
        Impresora.setFont(font);

        Impresora.drawText(30,Linea,QString::number(i,10));
        Impresora.drawText(50,Linea,"Cod:");
        Impresora.drawText(200,Linea,"Desc:");
        Impresora.drawText(450,Linea,"SN:");
        Impresora.drawText(500,Linea,"VerSoft:");
        Impresora.drawText(600,Linea,"ActSoft:");
        Impresora.drawText(700,Linea,"Bon:");

        font.setBold(false);
        font.setItalic(true);
        Impresora.setFont(font);

        Texto.clear();
        Texto.append(consulta.value("nombre").toString());
        consulta2.prepare("SELECT * FROM productos WHERE producto = '" + Texto + "'");
        consulta2.exec();
        consulta2.next();
        Impresora.drawText(75,Linea,Texto);
        Impresora.drawText(230,Linea,consulta2.value("desc").toString());
        Impresora.drawText(470,Linea,consulta.value("sn").toString());
        Impresora.drawText(545,Linea,consulta.value("vsoft").toString());
        Impresora.drawText(645,Linea,consulta.value("actsoft").toString());
        Impresora.drawText(725,Linea,consulta.value("bonif").toString());


        Linea +=knl;

        font.setBold(true);
        font.setItalic(false);
        Impresora.setFont(font);
        Impresora.drawText(50,Linea,"Falla:");

        font.setBold(false);
        font.setItalic(true);
        Impresora.setFont(font);

        Texto.clear();
        Texto.append(consulta.value("falla").toString());
        eDato  = Texto.size();
        if(!eDato)
            Linea += knl;
        while(eDato > 0 )
        {
            ind = Texto.lastIndexOf(' ');
            aFalla.clear();
            aFalla = Texto.mid(ind+1,eDato);
            Texto.truncate(ind);
            eDato = Texto.size();

            consulta2.prepare("SELECT * FROM FallasLista WHERE grupo LIKE 'M%' AND nombre = '" + aFalla + "'");
            consulta2.exec();
            consulta2.next();
            Impresora.drawText(80,Linea,aFalla + ":");
            Impresora.drawText(130,Linea,consulta2.value("descripcion").toString());
            Linea += knl;
       }
        font.setBold(true);
        font.setItalic(false);
        Impresora.setFont(font);
        Impresora.drawText(50,Linea,"Observaciones:");

        font.setBold(false);
        font.setItalic(true);
        Impresora.setFont(font);
        Impresora.drawText(130,Linea,consulta.value("obs").toString());
        Linea += knl;
        DivLinea(Linea);
    //    Linea += knl;
        i++;
    }

//-------------------------------------------------------------------------
//                          Perifericos
//-------------------------------------------------------------------------
        consulta.prepare("SELECT * FROM Perifericos WHERE repid = " + QString::number(Id,10));
        consulta.exec();
        while(consulta.next())
        {
            Texto.clear();
            Texto.append(consulta.value("falla").toString());
            Palabras = Texto.count(" ");
 //            a = Palabras;
            Palabras += 5;

            Palabras *= knl;

//           qDebug() << Texto<< a << i<< ":" << Palabras << "*"<< "11 + " << Linea  << "="<< Palabras + Linea;

            if((Linea + Palabras) >=1065 )
            {

                if (! printer.newPage()) {
                    qWarning("failed in flushing page to disk, disk full?");
                    return;
                }
                Linea = SaltoPagina();
//                qDebug()<< "NP" << i<< ":" << a << "*"<< "11 + " << Linea  << "="<< Palabras + Linea;

            }
            Linea += knl;
            font.setBold(true);
            font.setItalic(false);
            Impresora.setFont(font);
Impresora.drawText(30,Linea,QString::number(i,10));
            Impresora.drawText(50,Linea,"Cod:");
            Impresora.drawText(200,Linea,"Desc:");


            font.setBold(false);
            font.setItalic(true);
            Impresora.setFont(font);

            Texto.clear();
            Texto.append(consulta.value("nombre").toString());

            consulta2.prepare("SELECT * FROM productos WHERE producto = '" + Texto + "'");
            consulta2.exec();
            consulta2.next();
            Impresora.drawText(75,Linea,Texto);
            Impresora.drawText(230,Linea,consulta2.value("desc").toString());

            Linea += knl;
            font.setBold(true);
            font.setItalic(false);
            Impresora.setFont(font);

            Impresora.drawText(50,Linea,"SN:");
            Impresora.drawText(130,Linea,"V.Soft:");
            Impresora.drawText(200,Linea,"F.Soft:");
            Impresora.drawText(300,Linea,"F.Fab:");
            Impresora.drawText(400,Linea,"F.Inst:");
            Impresora.drawText(500,Linea,"Conf:");
            Impresora.drawText(700,Linea,"Bon:");

            font.setBold(false);
            font.setItalic(true);
            font.setFamily("Helvetica [Cronyx]");
            Impresora.setFont(font);

            Impresora.drawText(70,Linea,consulta.value("sn").toString());
            Impresora.drawText(170,Linea,consulta.value("vsoft").toString());
            Impresora.drawText(240,Linea,consulta.value("fsoft").toString());
            Impresora.drawText(340,Linea,consulta.value("ffab").toString());
            Impresora.drawText(440,Linea,consulta.value("finst").toString());
            Impresora.drawText(530,Linea,consulta.value("conf").toString());
            Impresora.drawText(725,Linea,consulta.value("bonif").toString());

            Linea +=knl;

            font.setBold(true);
            font.setItalic(false);
            Impresora.setFont(font);
            Impresora.drawText(50,Linea,"Falla:");

            font.setBold(false);
            font.setItalic(true);
            Impresora.setFont(font);

            Texto.clear();
            Texto.append(consulta.value("falla").toString());
            eDato  = Texto.size();
//            qDebug()<< eDato;
            if(!eDato)
                Linea += knl;
            while(eDato > 0 )
            {

//                qDebug()<< "a:" << eDato;
                ind = Texto.lastIndexOf(' ');
                aFalla.clear();
                aFalla = Texto.mid(ind+1,eDato);
                Texto.truncate(ind);
                eDato = Texto.size();

                consulta2.prepare("SELECT * FROM FallasLista WHERE grupo LIKE 'P%' AND nombre = '" + aFalla + "'");
                consulta2.exec();
                consulta2.next();
                Impresora.drawText(80,Linea,aFalla + ":");
                Impresora.drawText(130,Linea,consulta2.value("descripcion").toString());
                Linea += knl;
            }
//            qDebug()<< "e:" << eDato;
            font.setBold(true);
            font.setItalic(false);
            Impresora.setFont(font);
            Impresora.drawText(50,Linea,"Observaciones:");

            font.setBold(false);
            font.setItalic(true);
            Impresora.setFont(font);
            Impresora.drawText(130,Linea,consulta.value("obs").toString());
            Linea += knl;
            DivLinea(Linea);
           // Linea += knl;
       //     qDebug() << i << Linea;
            i++;
        }
//-------------------------------------------------------------------------
//                          Instalaciones
//-------------------------------------------------------------------------
                consulta.prepare("SELECT * FROM Instalaciones WHERE repid = " + QString::number(Id,10));
                consulta.exec();
                while(consulta.next())
                {
                    int a;
                    Texto.clear();
                    Texto.append(consulta.value("falla").toString());
                    Palabras = Texto.count(" ");
                     a = Palabras;
                    Palabras += 5;

                    Palabras *= knl;

                    qDebug() << Texto<< a << i<< ":" << Palabras << "*"<< "11 + " << Linea  << "="<< Palabras + Linea;

                    if((Linea + Palabras) >=1065 )
                    {

                        if (! printer.newPage()) {
                            qWarning("failed in flushing page to disk, disk full?");
                            return;
                        }
                        Linea = SaltoPagina();
                        qDebug()<< "NP" << i<< ":" << a << "*"<< "11 + " << Linea  << "="<< Palabras + Linea;

                    }

                    Linea += knl;
                    font.setBold(true);
                    font.setItalic(false);
                    Impresora.setFont(font);
Impresora.drawText(30,Linea,QString::number(i,10));
                    Impresora.drawText(50,Linea,"Cod:");
                    Impresora.drawText(200,Linea,"Desc:");
                    Impresora.drawText(500,Linea,"SN:");
                    Impresora.drawText(700,Linea,"Bon:");

                    font.setBold(false);
                    font.setItalic(true);
                    Impresora.setFont(font);

                    Texto.clear();
                    Texto.append(consulta.value("nombre").toString());

                    consulta2.prepare("SELECT * FROM productos WHERE producto = '" + Texto + "'");
                    consulta2.exec();
                    consulta2.next();
                    Impresora.drawText(75,Linea,Texto);
                    Impresora.drawText(230,Linea,consulta2.value("desc").toString());
                    Impresora.drawText(520,Linea,consulta.value("sn").toString());
                    Impresora.drawText(725,Linea,consulta.value("bonif").toString());

                    Linea +=knl;

                    font.setBold(true);
                    font.setItalic(false);
                    Impresora.setFont(font);
                    Impresora.drawText(50,Linea,"Falla:");

                    font.setBold(false);
                    font.setItalic(true);
                    Impresora.setFont(font);

                    Texto.clear();
                    Texto.append(consulta.value("falla").toString());
                    eDato  = Texto.size();
                    if(!eDato)
                        Linea += knl;
                    while(eDato > 0 )
                    {
                        ind = Texto.lastIndexOf(' ');
                        aFalla.clear();
                        aFalla = Texto.mid(ind+1,eDato);
                        Texto.truncate(ind);
                        eDato = Texto.size();

                        consulta2.prepare("SELECT * FROM FallasLista WHERE grupo LIKE 'P%' AND nombre = '" + aFalla + "'");
                        consulta2.exec();
                        consulta2.next();
                        Impresora.drawText(80,Linea,aFalla + ":");
                        Impresora.drawText(100,Linea,consulta2.value("descripcion").toString());
                        Linea += knl;
                    }
                    font.setBold(true);
                    font.setItalic(false);
                    Impresora.setFont(font);
                    Impresora.drawText(50,Linea,"Observaciones:");

                    font.setBold(false);
                    font.setItalic(true);
                    Impresora.setFont(font);
                    Impresora.drawText(130,Linea,consulta.value("obs").toString());

                    Linea += knl;
                    DivLinea(Linea);
             i++;
             }
     Impresora.end();

}

int ReporteInterno::SaltoPagina()
{
    QFont font;
    int NuevaLinea;


    NuevaLinea = 45;
    font.setWeight(11);
    font.setPixelSize(12);
    font.setBold(true);
    font.setItalic(false);

    Impresora.setFont(font);
    Impresora.drawText(50,NuevaLinea,"Agente:");
    Impresora.drawText(370,NuevaLinea,"ID:");


    font.setBold(false);
    font.setItalic(true);
    Impresora.setFont(font);
    Impresora.drawText(103,NuevaLinea,Agente);
    Impresora.drawText(400,NuevaLinea,Reporte);

    NuevaLinea += kSalto;
    NuevaLinea += kSalto;

    Impresora.drawLine(50,NuevaLinea, 750, NuevaLinea);

    font.setWeight(9);
    font.setPixelSize(9);
    font.setBold(true);
    font.setItalic(false);
    Impresora.setFont(font);
    Impresora.drawPixmap(QRect(600,10,100,60),QPixmap(Direccion + "/SIID.png"));
    return NuevaLinea;
}

void ReporteInterno::DivLinea(int Lin)
{
    QFont font;
    QPen Pluma;

    Pluma.setWidth(2);
    Impresora.setPen(Pluma);
    Impresora.setFont(font);
    Impresora.drawLine(50,Lin, 750, Lin);
    font.setWeight(9);
    font.setPixelSize(9);
    Impresora.setFont(font);

}
