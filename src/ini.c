/*

Solar-Logger - A Daemon for logging Data from a Solar Plant
Copyright (C) 2007  Klaus Dotterweich (dr.dotti@gmx.de)

This program is free software; you can redistribute it and/or modify it under the terms of the
GNU General Public License as published by the Free Software Foundation;
either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program;
if not, see <http://www.gnu.org/licenses/>.



Solar-Logger - Ein Daemon zum loggen der Daten einer Solaranlage
Copyright (C) 2007  Klaus Dotterweich (dr.dotti@gmx.de)

Dieses Programm ist freie Software. Sie können es unter den Bedingungen der GNU General Public License,
wie von der Free Software Foundation veröffentlicht, weitergeben und/oder modifizieren,
entweder gemäß Version 3 der Lizenz oder (nach Ihrer Option) jeder späteren Version.

Die Veröffentlichung dieses Programms erfolgt in der Hoffnung, daß es Ihnen von Nutzen sein wird,
aber OHNE IRGENDEINE GARANTIE, sogar ohne die implizite Garantie der MARKTREIFE oder
der VERWENDBARKEIT FÜR EINEN BESTIMMTEN ZWECK. Details finden Sie in der GNU General Public License.

Sie sollten ein Exemplar der GNU General Public License zusammen mit diesem Programm erhalten haben.
Falls nicht, siehe <http://www.gnu.org/licenses/>.

*/

/**************************************************************************
*
*  Funktionssammlung um Einlesen der INI Datei
*
***************************************************************************
*  DateiName:	ini.c
*  Status:		InArbeit
***************************************************************************/



#include "solar-logger.h"
#include "log.h"



/**************************************************************************
   Description   : Einlesen der OutputTypeAnzahl der INI Datei
   Parameter     : (none)
   Return-Value  : (none)
   Changes       : Author, 		Date, 		Version, 	Reason
                   ********************************************************
                   Dotterweich,	23.08.2007, 0.1, 		Created
**************************************************************************/
int InitAnzahl(char *Text, int MAX)
{
	int Temp;

	Temp = TRepository_GetElementInt(Text, 0);
	if(Temp > MAX)
	{
		Temp = MAX;
	}

	ausgabe(LOG, DEBUGINIT, "InitAnzahl von %s ist %i!!!\n", Text, Temp);

	return Temp;
}



/**************************************************************************
   Description   : Einlesen der OutputTypen der INI Datei
   Parameter     : (none)
   Return-Value  : (none)
   Changes       : Author, 		Date, 		Version, 	Reason
                   ********************************************************
                   Dotterweich,	23.08.2007, 0.1, 		Created
**************************************************************************/
void InitOutputType(int Anzahl, int *Type, char *Text)
{
	int		Zaehler;
	char	Temp[100] = {0};
	char	TempType[20] = {0};

	for(Zaehler = 1; Zaehler <= Anzahl; Zaehler++)
	{
		sprintf(Temp, "%s_%i", Text, Zaehler);

		TRepository_GetElementStr(	Temp,
        							"",
        							TempType,
        							sizeof(TempType)-1);

		if(strncmp(TempType, "Local", sizeof(TempType)-1) == 0)
		{
            *Type = 1;
		}
		else if(strncmp(TempType, "FTP", sizeof(TempType)-1) == 0)
		{
            *Type = 2;
        }
		else
		{
            *Type = 0;
		}

		ausgabe(LOG, DEBUGINIT, "InitOutputType von %s[%i] ist %i!!!\n", Text, Zaehler - 1, *Type);

		Type++;
	}
}



/**************************************************************************
   Description   : Einlesen der OutputFormate der INI Datei
   Parameter     : (none)
   Return-Value  : (none)
   Changes       : Author, 		Date, 		Version, 	Reason
                   ********************************************************
                   Dotterweich,	23.08.2007, 0.1, 		Created
**************************************************************************/
void InitOutputFormat(int Anzahl, int *Format, char *Text)
{
	int		Zaehler;
	char	Temp[100] = {0};
	char	TempType[20] = {0};

	for(Zaehler = 1; Zaehler <= Anzahl; Zaehler++)
	{
		sprintf(Temp, "%s_%i", Text, Zaehler);
		TRepository_GetElementStr(	Temp,
        							"",
        							TempType,
        							sizeof(TempType)-1);

		if(strncmp(TempType, "txt", sizeof(TempType)-1) == 0)
		{
            *Format = 1;
		}
		else if(strncmp(TempType, "csv", sizeof(TempType)-1) == 0)
		{
            *Format = 2;
        }
		else
		{
            *Format = 0;
		}

		ausgabe(LOG, DEBUGINIT, "InitOutputFormat von %s[%i] ist %i!!!\n", Text, Zaehler - 1, *Format);

		Format++;
	}
}



// Ende
