#include "user.h"
#include <QString>

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
