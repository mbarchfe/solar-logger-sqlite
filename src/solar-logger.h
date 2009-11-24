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
*  Ein Deamon zum Überwachen und Loggen der Daten von SMA Wechselrichter
*
***************************************************************************
*  DateiName:	solar-logger.h
*  Status:		InArbeit
***************************************************************************/

#include <stdio.h>					// Standartfunktionen
#include <syslog.h>					// für Syslogfunktionen
#include <stdarg.h>					// Arbeiten mit variablen langen Argumentlisten
#include <stdlib.h>
#include <time.h>

#include "libyasdimaster.h"



#define ProgrammName	"solar-logger"		// Name des Programmes
#define ProgrammVersion	"0.2.1 BETA"		// Version des Programes



#define DEVMAX 10       					// Maximal 10 devices
#define MAXDRIVERS 3   						// Maximal 3 YASDI Bus drivers
#define KANALMAX 50							// Maximal 50 Kanäle ja WR
#define WERTALTERMAX 30						// Maximales Alter der Messwerte in Sekunden
#define OUTPUTTYPEMAX 5						// Maximal 5 Output Typen
#define ERRECHNETEKANAELEMAX 10				// Maximal 10 ErrechneteKanaele
#define STELLENMAX 20						// Maximal 20 Stellen vor und nach dem Komma
#define ZEICHENMAX 20						// Maximal 20 Zeichen für den Wert eines Text Kanales

#define DATUMMAX 10							// Maximal 10 Zeichen für ein Datum
#define ZEITPUNKTMAX 25						// Maximal 25 Zeichen für einen Zeitpunkt
#define DATEINAMEPFADMAX 200				// Maximal 200 Zeichen für einen Dateinamen mit Pfad
#define KANALNAMEMAX 10						// Maximal 10 Zeichen für einen Kanal Namen
#define KANALWERTTEXTMAX 30					// Maximal 30 Zeichen für einen Kanal Wert Text
#define SNMAX 20							// Maximal 20 Zeichen für eine SN



// Ende
