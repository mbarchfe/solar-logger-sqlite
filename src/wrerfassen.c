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
*  Funktionssammlung zum Erfassen der WR
*
***************************************************************************
*  DateiName:	wrerfassen.c
*  Status:		InArbeit
***************************************************************************/



#include "solar-logger.h"
#include "log.h"
#include "timer.h"



// Globale Variable
int		i;
int		AnzahlSollWR = 0;						// Anzahl der WR laut INI Datei
int		AnzahlErfassterWR = 0;					// Anzahl der erfassten WR (um ein erfassen der WR zu erzwingen auf 0 setzen)
DWORD	WRHandles[DEVMAX];						// Handles der WR
long	IntervallZeitWRErfassen = 0;			// Interfallzeit in Minuten nach dem Spätestens die WR neu erfasst werden sollen
long	LetzteIntervallZeitWRErfassen = 0;		// Zeitpunkt der letzten WR Erfassung
char	IniFile[]="solar-logger.ini";			// INI Datei
DWORD	DriverAnzahl;
DWORD	Driver[MAXDRIVERS];
char	DriverName[30];
BOOL	DriverOnline = FALSE;   				// Ist ein Driver online???



/**************************************************************************
   Description   : Einlesen des WR Bereiches der INI Datei
   Parameter     : (none)
   Return-Value  : (none)
   Changes       : Author, 		Date, 		Version, 	Reason
                   ********************************************************
                   Dotterweich,	20.08.2007, 0.1, 		Created
**************************************************************************/
void WRInit(void)
{
    ausgabe(LOG, DEBUGINIT, "WRInit gestartet.\n");

    if (0 > yasdiMasterInitialize(IniFile, &DriverAnzahl))
    {
		ausgabe(LOG, DEBUGMINI, "YASDI INI Datei konnte nicht gefunden werden!!!\n");
        return;
    }

    ausgabe(LOG, DEBUGINIT, "DriverAnzahl nach 'yasdiMasterInitialize' ist %d.\n", DriverAnzahl);

	AnzahlSollWR = TRepository_GetElementInt("WR.Anzahl", 1);
	IntervallZeitWRErfassen = TRepository_GetElementInt("WR.ErfassungsIntervall", 0);

    ausgabe(LOG, DEBUGINIT, "AnzahlSollWR ist %d.\n", AnzahlSollWR);
    ausgabe(LOG, DEBUGINIT, "IntervallZeitWRErfassen ist %d.\n", IntervallZeitWRErfassen);

    DriverAnzahl = yasdiMasterGetDriver(Driver, MAXDRIVERS);				// Liste der Driver ermitteln

    ausgabe(LOG, DEBUGINIT, "DriverAnzahl nach 'yasdiMasterGetDriver' ist %d.\n", DriverAnzahl);

    for(i = 0; i < DriverAnzahl; i++)										// Alle Driver Online schalten
    {
        yasdiGetDriverName(Driver[i], DriverName, sizeof(DriverName));		// Driver Name ermitteln

        if (yasdiSetDriverOnline(Driver[i]))
        {
			ausgabe(LOG, DEBUGNORMAL, "Driver '%s' online schalten!!!  Erfolgreich\n", DriverName);
            DriverOnline = TRUE;
        }
        else
        {
			ausgabe(LOG, DEBUGMINI, "Driver '%s' online schalten!!!  nicht Erfolgreich\n", DriverName);
		}
    }

    if (DriverOnline == FALSE)        										// Ist ein Driver Online???
    {
        ausgabe(LOG, DEBUGMINI, "ERROR: Kein Driver online! Datenabfrage nicht möglich!\n");
        return;
    }

	ausgabe(LOG, DEBUGNORMAL, "WR Erfassung Initialisiert!!!\n");
}



/**************************************************************************
   Description   : WR Erfassen
   Parameter     : (none)
   Return-Value  : (none)
   Changes       : Author, 		Date, 		Version, 	Reason
                   ********************************************************
                   Dotterweich,	20.08.2007, 0.1, 		Created
**************************************************************************/
void WRErfassen(void)
{
    int FehlerCode;

	ausgabe(LOG, DEBUGNORMAL, "WR Erfassung gestartet!!! (Soll %d WR)\n", AnzahlSollWR);

    if (AnzahlSollWR == 0)
    {
 		ausgabe(LOG, DEBUGMINI, "WR Erfasssung abgebrochen weil Variable 'AnzahlSollWR' ist 0 (Fehler in INI Datei oder Variable nicht Initalisiert)\n");
 		return;
    }

    FehlerCode = yasdiDoMasterCmdEx("detect", AnzahlSollWR, 0, 0);		// WR Erfassen

    if (FehlerCode < 0) 												//Fehler?
    {
 		ausgabe(LOG, DEBUGMINI, "Es wurden weniger als %d WR erfasst.\n", AnzahlSollWR);
 		return;
    }
    else
    {
    	AnzahlErfassterWR = GetDeviceHandles(WRHandles, DEVMAX);		// Anzahl der Erfassten WR
 		ausgabe(LOG, DEBUGNORMAL, "Es wurden %d WR erfasst.\n", AnzahlErfassterWR);
    }

	if(AnzahlSollWR == AnzahlErfassterWR)
	{																		// Wenn alle WR erfasst
 		ausgabe(LOG, DEBUGNORMAL, "WR Erfassung erfolgreich\n");
		LetzteIntervallZeitWRErfassen = MinutenZeit;						// Nach erfolgreicher Erfassung LetzteIntervallzeit neu setzen
		return;
	}
}



/**************************************************************************
   Description   : WR Erfassung Wiederholen wenn Konfiguriert und wenn nötig
   Parameter     : (none)
   Return-Value  : (none)
   Changes       : Author, 		Date, 		Version, 	Reason
                   ********************************************************
                   Dotterweich,	20.08.2007, 0.1, 		Created
**************************************************************************/
void WRErfassenWiederholen(void)
{
	ausgabe(LOG, DEBUGALLES, "WR Erfassung Wiederholung gestartet!!! (Soll %d, Ist %d WR)\n", AnzahlSollWR, AnzahlErfassterWR);
	if(	(!(IntervallZeitWRErfassen == 0) &&												// Wenn Intervallzeit = 0 UND
		(LetzteIntervallZeitWRErfassen + IntervallZeitWRErfassen <= MinutenZeit)) ||	// Wenn Intervallzeit abgelaufen
		AnzahlErfassterWR == 0 ||														// ODER Wenn keine WR erfasst
		!(AnzahlSollWR == AnzahlErfassterWR))											// ODER Wenn nicht alle WR erfasst
	{
		WRErfassen();																	// Dann WR neu Erfassen
		return;
	}

	ausgabe(LOG, DEBUGALLES, "WR Erfassung Wiederholung nicht nötig!!! (Soll %d, Ist %d WR)\n", AnzahlSollWR, AnzahlErfassterWR);
}



/**************************************************************************
   Description   : WR offline schalten
   Parameter     : (none)
   Return-Value  : (none)
   Changes       : Author, 		Date, 		Version, 	Reason
                   ********************************************************
                   Dotterweich,	20.08.2007, 0.1, 		Created
**************************************************************************/
void WRShutdown(void)
{
    for (i = 0; i < DriverAnzahl; i++)
    {
        yasdiGetDriverName(Driver[i], DriverName, sizeof(DriverName));
		ausgabe(LOG, DEBUGMINI, "Driver '%s' offline schalten!!!\n", DriverName);
        yasdiSetDriverOffline(Driver[i]);
    }
	yasdiMasterShutdown();																// Shutdown YASDI
}



// Ende
