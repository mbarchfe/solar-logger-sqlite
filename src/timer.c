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
*  Funktionssammlung für die Timer
*
***************************************************************************
*  DateiName:	timer.c
*  Status:		InArbeit
***************************************************************************/



#include "solar-logger.h"
#include "log.h"



// Globale Variable
long SekundenZeit;								// wird jede Sekunde um eins erhöt
long MinutenZeit;								// wird jede Minute um eins erhöt
long StundenZeit;								// wird jede Stunde um eins erhöt
long TagZeit;									// wir jeden Tag um eins erhöt



/**************************************************************************
   Description   : Setzen der Globalen Zeit Variablen
   Parameter     : Zeiger auf einen char * Array
   Return-Value  : (none)
   Changes       : Author, 		Date, 		Version, 	Reason
                   ********************************************************
                   Dotterweich,	19.08.2007, 0.1, 		Created
**************************************************************************/
void timer(void)
{
	ausgabe(SYSLOG, DEBUGALLES, "timer Start\n", SekundenZeit);

	SekundenZeit = time(NULL);

	MinutenZeit = SekundenZeit / 60;

	StundenZeit = MinutenZeit / 60;

	TagZeit = (StundenZeit + 3) / 24;

	ausgabe(LOG, DEBUGALLES, "timer SekundenZeit: %d!!!\n", SekundenZeit);
	ausgabe(LOG, DEBUGALLES, "timer MinutenZeit: %d!!!\n", MinutenZeit);
	ausgabe(LOG, DEBUGALLES, "timer StundenZeit: %d!!!\n", StundenZeit);
	ausgabe(LOG, DEBUGALLES, "timer TagZeit: %d!!!\n", TagZeit);
}



/**************************************************************************
   Description   : Datum und Uhrzeit als Text erhalten
   Parameter     : Zeiger auf einen char * Array
   Return-Value  : (none)
   Changes       : Author, 		Date, 		Version, 	Reason
                   ********************************************************
                   Dotterweich,	29.07.2007, 0.1, 		Created
**************************************************************************/
void GetZeitpunkt(char *Buffer, int Zeichen)
{
	time_t	jetzt;

	time(&jetzt);
	strftime(Buffer, Zeichen, "%d.%m.%Y %H:%M:%S", localtime(&jetzt));
}



/**************************************************************************
   Description   : Datum als Text erhalten
   Parameter     : Zeiger auf einen char * Array
   Return-Value  : (none)
   Changes       : Author, 		Date, 		Version, 	Reason
                   ********************************************************
                   Dotterweich,	29.07.2007, 0.1, 		Created
**************************************************************************/
void GetDatum(char *Buffer, int Zeichen)
{
	time_t	jetzt;
	char	Temp[100] = {0};

	time(&jetzt);
    TRepository_GetElementStr(  "Grundeinstellungen.DatumsFormatDatei",
                                "",
                                Temp,
                                sizeof(Temp)-1);

    if(strncmp(Temp, "YYMMDD", sizeof(Temp)-1) == 0)
    {
		strftime(Buffer, Zeichen, "%y%m%d", localtime(&jetzt));
    }
    else if(strncmp(Temp, "YYYY-MM-DD", sizeof(Temp)-1) == 0)
    {
		strftime(Buffer, Zeichen, "%Y-%m-%d", localtime(&jetzt));
    }
    else
    {
		strftime(Buffer, Zeichen, "%y%m%d", localtime(&jetzt));
    }
}



/**************************************************************************
   Description   : Monat als Text erhalten
   Parameter     : Zeiger auf einen char * Array
   Return-Value  : (none)
   Changes       : Author, 		Date, 		Version, 	Reason
                   ********************************************************
                   Dotterweich,	01.09.2007, 0.1, 		Created
**************************************************************************/
void GetMonat(char *Buffer, int Zeichen)
{
	time_t	jetzt;
	char	Temp[100] = {0};

	time(&jetzt);
    TRepository_GetElementStr(  "Grundeinstellungen.DatumsFormatDatei",
                                "",
                                Temp,
                                sizeof(Temp)-1);

    if(strncmp(Temp, "YYMMDD", sizeof(Temp)-1) == 0)
    {
		strftime(Buffer, Zeichen, "%y%m", localtime(&jetzt));
    }
    else if(strncmp(Temp, "YYYY-MM-DD", sizeof(Temp)-1) == 0)
    {
		strftime(Buffer, Zeichen, "%Y-%m", localtime(&jetzt));
    }
    else
    {
		strftime(Buffer, Zeichen, "%y%m", localtime(&jetzt));
    }
}



// Ende
