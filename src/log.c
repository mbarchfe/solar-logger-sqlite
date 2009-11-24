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
*  Funktionssammlung um Logausgaben zu machen
*
***************************************************************************
*  DateiName:	log.c
*  Status:		InArbeit
***************************************************************************/

#include "solar-logger.h"
#include "log.h"



// Globale Variable
char	LogFile[] = "solar-logger.log";						// Log Datei
int		DebugLevel = 30;



/**************************************************************************
   Description   : Ein LOG Ausgabe machen
   Parameter     : (none)
   Return-Value  : (none)
   Changes       : Author, 		Date, 		Version, 	Reason
                   ********************************************************
                   Dotterweich,	20.08.2007, 0.1, 		Erstellt
                   Dotterweich,	10.09.2007, 0.2, 		Erweitert
**************************************************************************/
void ausgabe(int wo, int level, char *text, ...)
{
	if(level > DebugLevel)
	{
		return;
	}

    if(TERM & wo)
    {
        va_list args;
        char    buffer[150];

        va_start(args, text);
        vsprintf(buffer, text, args);
    	printf(buffer);
        va_end(args);
    }

    if((SYSLOG & wo) || (LOGDATEI & wo))
    {
        va_list args;
        char    Buffer[150];
		char	ZeitpunktBuffer[50] = {0};
		FILE	*Datei = NULL;

        GetZeitpunkt(ZeitpunktBuffer, sizeof(ZeitpunktBuffer)-1);
        
        Datei = fopen(LogFile, "a");

        va_start(args, text);
        vsprintf(Buffer, text, args);

        fprintf(Datei, "<%s> %s", ZeitpunktBuffer, Buffer);
    	syslog(LOG_NOTICE, Buffer);

        va_end(args);

		fclose(Datei);
    }
}



// Ende
