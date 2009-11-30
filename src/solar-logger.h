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

Dieses Programm ist freie Software. Sie k�nnen es unter den Bedingungen der GNU General Public License,
wie von der Free Software Foundation ver�ffentlicht, weitergeben und/oder modifizieren,
entweder gem�� Version 3 der Lizenz oder (nach Ihrer Option) jeder sp�teren Version.

Die Ver�ffentlichung dieses Programms erfolgt in der Hoffnung, da� es Ihnen von Nutzen sein wird,
aber OHNE IRGENDEINE GARANTIE, sogar ohne die implizite Garantie der MARKTREIFE oder
der VERWENDBARKEIT F�R EINEN BESTIMMTEN ZWECK. Details finden Sie in der GNU General Public License.

Sie sollten ein Exemplar der GNU General Public License zusammen mit diesem Programm erhalten haben.
Falls nicht, siehe <http://www.gnu.org/licenses/>.

*/

/**************************************************************************
*
*  Ein Deamon zum �berwachen und Loggen der Daten von SMA Wechselrichter
*
***************************************************************************
*  DateiName:	solar-logger.h
*  Status:		InArbeit
***************************************************************************/

#include <stdio.h>					// Standartfunktionen
#include <syslog.h>					// f�r Syslogfunktionen
#include <stdarg.h>					// Arbeiten mit variablen langen Argumentlisten
#include <stdlib.h>
#include <time.h>

#include "yasdi/libyasdimaster.h"



#define ProgrammName	"solar-logger"		// Name des Programmes
#define ProgrammVersion	"0.2.1 BETA"		// Version des Programes



#define DEVMAX 10       					// Maximal 10 devices
#define MAXDRIVERS 3   						// Maximal 3 YASDI Bus drivers
#define KANALMAX 50							// Maximal 50 Kan�le ja WR
#define WERTALTERMAX 30						// Maximales Alter der Messwerte in Sekunden
#define OUTPUTTYPEMAX 5						// Maximal 5 Output Typen
#define ERRECHNETEKANAELEMAX 10				// Maximal 10 ErrechneteKanaele
#define STELLENMAX 20						// Maximal 20 Stellen vor und nach dem Komma
#define ZEICHENMAX 20						// Maximal 20 Zeichen f�r den Wert eines Text Kanales

#define DATUMMAX 10							// Maximal 10 Zeichen f�r ein Datum
#define ZEITPUNKTMAX 25						// Maximal 25 Zeichen f�r einen Zeitpunkt
#define DATEINAMEPFADMAX 200				// Maximal 200 Zeichen f�r einen Dateinamen mit Pfad
#define KANALNAMEMAX 10						// Maximal 10 Zeichen f�r einen Kanal Namen
#define KANALWERTTEXTMAX 30					// Maximal 30 Zeichen f�r einen Kanal Wert Text
#define SNMAX 20							// Maximal 20 Zeichen f�r eine SN



// Ende
