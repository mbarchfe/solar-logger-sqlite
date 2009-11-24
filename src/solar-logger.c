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
*  DateiName:	solar-logger.c
*  Status:		InArbeit
***************************************************************************/

#include "solar-logger.h"
#include "daemon.h"
#include "log.h"
#include "sqlite3common.h"
#include "logsqlite3.h"
#include "logsunmeter.h"
#include "timer.h"
#include "wrerfassen.h"
#include "kanalrech.h"
#include "sollog.h"
#include "solstatus.h"


/**************************************************************************
   Description   : Main Funktion
   Parameter     : (none)
   Return-Value  : (none)
   Changes       : Author, 		Date, 		Version, 	Reason
                   ********************************************************
                   Dotterweich,	24.08.2007, 0.1, 		Aktualisiert
**************************************************************************/
int main(int argv, char **argc)
{
    ausgabe(TERM, DEBUGMINI, "******************************************************************************\n");
    ausgabe(TERM, DEBUGMINI, "\n");
    ausgabe(TERM, DEBUGMINI, "                Solar-Logger ein Solar Daten Logger Deamon      \n");
    ausgabe(TERM, DEBUGMINI, "\n");
    ausgabe(TERM, DEBUGMINI, "          Copyright (C) 2007  Klaus Dotterweich (dr.dotti@gmx.de)\n");
    ausgabe(TERM, DEBUGMINI, "\n");
    ausgabe(TERM, DEBUGMINI, " This program comes with ABSOLUTELY NO WARRANTY. This is free software, \n");
    ausgabe(TERM, DEBUGMINI, " and you are welcome to redistribute it under certain conditions.\n");
    ausgabe(TERM, DEBUGMINI, " For details see 'LICENSE.ENG.TXT'\n");
    ausgabe(TERM, DEBUGMINI, "\n");
    ausgabe(TERM, DEBUGMINI, " F�r dieses Programm besteht KEINERLEI GARANTIE. Dies ist freie Software, \n");
    ausgabe(TERM, DEBUGMINI, " die Sie unter bestimmten Bedingungen weitergeben d�rfen;\n");
    ausgabe(TERM, DEBUGMINI, " Details k�nnen sie in der Datei 'LICENSE.DE.TXT' finden.\n");
    ausgabe(TERM, DEBUGMINI, "\n");
    ausgabe(TERM, DEBUGMINI, "******************************************************************************\n");

    start_daemon("solar-logger", LOG_LOCAL0);

    if(!theonly_daemon())
    {										// Wenn Daemon bereits gestartet dann diesen sofort beenden.
        exit (EXIT_FAILURE);
    }

	// Ab hier ist das Programm ein Daemon

	timer();								// Zeiten aktualisieren
	WRInit();								// WR Initialisieren
	WRKanaeleINI();							// WR Kan�le Einstellen
	LoggenINI();							// Einlesen des Loggen Bereiches der INI Datei
	StatusINI();							// Einlesen des Status Bereiches der INI Datei

    ausgabe(LOG, DEBUGMINI, "Grundinitialisierung abgeschlossen\n");

	while(!(AnzahlErfassterWR == AnzahlSollWR))
	{
 		WRErfassenWiederholen();			// WR Erfassung so lange wiederholen bis alle WR erfasst
 	}

	ErrechneteKanaeleINI();					// Einlesen des ErrechneteKanaele Bereiches der INI Datei

	init_sqlite3common("/tmp/test1.sqlite3");
	init_logsqlite3();
	init_logsunmeter();


    ausgabe(LOG, DEBUGMINI, "Initialisierung abgeschlossen\n");

    while(1)
    {
    	ausgabe(LOG, DEBUGALLES, "Eine neue Runde! :-) \n");
    	
 		timer();							// Zeiten aktualisieren
 		WRErfassenWiederholen();			// WR Erfassung wiederholen wenn n�tig
 		//ErrechneteKanaeleAktualisieren();	// Aktualisieren der Errechneten Kan�le
 		//LogGesamt();						// Erstellen der Log Gesamt Dateien
 		//Log();								// Erstellen der Log Dateien
 		log_sqlite();
 		log_sunmeter();
 		//LogMonat();							// Erstellen der Log Monat Dateien
 		//StatusUebersichtGesamt();			// Erstellen der Staus Uebersicht Gesamt Dateien
 		//StatusUebersicht();					// Erstellen der Staus Uebersicht Dateien
 		
    	ausgabe(LOG, DEBUGALLES, "Jetzt schlafe ich 1 Sekunde.\n");
 		sleep(5);							// leg dich 1 Sekunde schlafen keine gute L�sung wird noch Variabel gestaltet
    }

    ausgabe(LOG, DEBUGMINI, "Logger beenden.\n");

	WRShutdown();
    stop_daemon();
    return EXIT_SUCCESS;
}



// Ende
