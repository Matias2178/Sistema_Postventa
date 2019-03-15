#include "user.h"
#include <QString>
#include <QDebug>

User::User()
{
}

QString User::Moduladoras(int ID)
{
    QString Tipo;
    Tipo.clear ();
    switch(ID)
    {
        break;
        case 0x68:
        //Siembra Primaria 			#1
            Tipo.append("Mod Siem Prim 1");
        break;
        case 0x6A:
        //Siembra Secundaria 		#1
            Tipo.append("Mod Siem Sec 1");
        break;
        case 0x69:
        //Fertilizante Primario 	#1
            Tipo.append("Mod Fert Prim 1");
        break;
        case 0x6B:
        //Fertilizante Secundario 	#1
            Tipo.append("Mod Fert Sec 1");
        break;
        case 0xC8:
        //Siembra Primaria 			#2
            Tipo.append("Mod Siem Prim 2");
        break;
        case 0xCA:
        //Siembra Secundaria 		#2
            Tipo.append("Mod Siem Sec 2");
        break;
        case 0xC9:
        //Fertilizante Primario 	#2
            Tipo.append("Mod Fert Prim 2");
        break;
        case 0xCB:
        //Fertilizante Secundario 	#2
            Tipo.append("Mod Fert Sec 2");
        break;
        case 0xCC:
        //Siembra Primaria 			#3
            Tipo.append("Mod Siem Prim 3");
        break;
        case 0xCE:
        //Siembra Secundaria 		#3
            Tipo.append("Mod Siem Sec 3");
        break;
        case 0xCD:
        //Fertilizante Primario 	#3
            Tipo.append("Mod Fert Prim 3");
        break;
        case 0xCF:
        //Fertilizante Secundario	#3
            Tipo.append("Mod Fert Sec 3");
        break;
        case 0x64:
        //Siembra Primaria 			#4
            Tipo.append("Mod Siem Prim 4");
        break;
        case 0x66:
        //Siembra Secundaria 		#4
            Tipo.append("Mod Siem Sec 4");
        break;
        case 0x65:
        //Fertilizante Primario 	#4
            Tipo.append("Mod Fert Prim 4");
        break;
        case 0x67:
        //Fertilizante Secundario	#4
            Tipo.append("Mod Fert Sec 4");
        break;
    }
    return Tipo;
}

QList  <QString> User::GetFallas(QTableWidget &Falla)
{
    QString Fallas, Grupo;
    QList <QString> lFallaGrupo;
    int indice, i;
    bool sig,nuevo;

    indice = Falla.rowCount();
    sig = false;
    Fallas.clear();
    Grupo.clear();

 //Busco fallas que se seleccionaron en la tabla
    for (i=0;i<indice;i++)
    {

        if(Falla.item(i,0)->checkState() == 2) //si esta con el check activo
        {

            if(sig) //Si no es el primero pongo un espacio
                Fallas.append(" ");

            Fallas.append(Falla.item(i,0)->text());

            nuevo = true;
            if(lFallaGrupo.size()) //Si ya hay un elemento en la lista controlo si ya se agrego
            {
                for (int e = 0; e < lFallaGrupo.size(); ++e)
                {
                    if (lFallaGrupo.at(e) == Falla.item(i,2)->text())
                    {
                        nuevo = false;
                        break;
                    }
                }
            }

            if (nuevo)
            {
                if(sig) //Si no es el primero pongo un espacio
                   Grupo.append(" ");

                lFallaGrupo << Falla.item(i,2)->text();
                Grupo.append(Falla.item(i,2)->text());
            }
            sig = true;
        }
        Falla.item(i,0)->setCheckState(Qt::Unchecked);

    }
    lFallaGrupo.clear();
    lFallaGrupo << Fallas;
    lFallaGrupo << Grupo;
    return lFallaGrupo;
}

void User::SetFallas(QTableWidget &Falla, QString Fallas)
{

    int pdato, ind, inicio;
    QString ff;
    inicio = 0;

    pdato = Fallas.size();

    while(pdato >= 3)
    {
        ff.clear();
        ind = Fallas.lastIndexOf(' ');
        ff = Fallas.mid(ind+1,pdato);
//    qDebug () << "Fallas" << Fallas;
//    qDebug () << "Indice" << ind << "Size" << pdato;
//    qDebug () << "Falla" << ff;
        Fallas.truncate(ind);
        pdato = Fallas.size();

        for (int r=0; r<Falla.rowCount();r++)
        {
            if(Falla.item(r,0)->text() == ff)
            {
                Falla.item(r,0)->setCheckState(Qt::Checked);
                break;
            }
        }
    }
}
