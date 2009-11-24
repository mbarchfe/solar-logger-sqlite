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
*  Errechnen von Zusätzlichen Kanälen
*
***************************************************************************
*  DateiName:	kanalrech.c
*  Status:		InArbeit
***************************************************************************/

#include "solar-logger.h"
#include "log.h"
#include "ini.h"
#include "timer.h"
#include "wrerfassen.h"
#include "wrabfragen.h"
#include "solstatus.h"



// Globale Variable
int		ErrechneteKanaeleAnzahl=0;								// Anzahl der zu errechnenten Kanäle
char	ErrechneteKanaeleName[ERRECHNETEKANAELEMAX][20]={0};	// Namen der errechneten Kanäle
int		ErrechneteKanaeleArt[ERRECHNETEKANAELEMAX]={0};			// Berechnungs Arten
char	ErrechneteKanaeleKanal[ERRECHNETEKANAELEMAX][20]={0};	// Namen der WR Kanäle die zur Berechnung nötig sind
int		ErrechneteKanaeleZeichen[ERRECHNETEKANAELEMAX]={0};		// Anzahl der Stellen vor dem Komma
int		ErrechneteKanaeleGenauigkeit[ERRECHNETEKANAELEMAX]={0};	// Anzahl der Stellen nach dem Komma
long	ErrechneteKanaeleErrechnungsIntervallZeit=0;			// Intervall zur Berechnung der Kanäle in Sekunden
long	ErrechneteKanaeleErrechnungsLetzteIntervallZeit=0;		// Zeitpunkt des letzten berechnens der Kanäle in Sekunden
char	ErrechneteKanaeleTempDateiName[100] = {0};				// Dateinamenende zum speichern der ErrechneteKanaele Temp Variablen

double	ErrechneteKanaeleWertZahl[DEVMAX][ERRECHNETEKANAELEMAX]={0};		// Wert der errechnenten Kanäle
char	ErrechneteKanaeleWert[DEVMAX][ERRECHNETEKANAELEMAX][20]={0};		// Wert der errechnenten Kanäle
char	ErrechneteKanaeleEinheit[DEVMAX][ERRECHNETEKANAELEMAX][20]={0};		// Einheit der errechnenten Kanäle
long	ErrechneteKanaeleLongTemp[DEVMAX][ERRECHNETEKANAELEMAX]={0};		// long TEMP Speicher
double	ErrechneteKanaeleDoubleTemp[DEVMAX][ERRECHNETEKANAELEMAX]={0};		// double TEMP Speicher

BOOL	ErrechneteKanaeleAktualisierenErfolgreich = TRUE;   	// TRUE wenn ErrechneteKanaeleAktualisieren Erfolgreich



/**************************************************************************
   Description   : Sichern der ErrechneteKanaeleTemp Variablen in eine Datei
   Parameter     : (none)
   Return-Value  : (none)
   Changes       : Author, 		Date, 		Version, 	Reason
                   ********************************************************
                   Dotterweich,	27.08.2007, 0.1, 		Created
**************************************************************************/
void ErrechneteKanaeleSichernTemp(void)
{
	int		i = 1;
	int		Zaehler = 1;
	DWORD	SNBuffer = 0;
    char	TypeBuffer[50] = {0};
	char	Pfad[100] = {0};
	char	DateiName[100] = {0};
	char	DateiBuffer[200] = {0};
    FILE    *Datei = NULL;

    Tools_PathExtractPath(Pfad, ErrechneteKanaeleTempDateiName, sizeof(ErrechneteKanaeleTempDateiName)-1 );
    Tools_PathExtractFile(DateiName, ErrechneteKanaeleTempDateiName, sizeof(ErrechneteKanaeleTempDateiName)-1 );

    for(Zaehler = 1; Zaehler <= AnzahlErfassterWR; Zaehler++)
    {

        GetDeviceSN(WRHandles[Zaehler - 1], &SNBuffer);
    	GetDeviceType(WRHandles[Zaehler - 1], TypeBuffer, sizeof(TypeBuffer)-1);

		sprintf(DateiBuffer, "%s/%3ld.%s", Pfad, SNBuffer, DateiName);

        Datei = fopen(DateiBuffer, "w");
        if (!Datei)
        {
            ausgabe(LOG, DEBUGMINI, "ErrechneteKanaele.TempVariablenDatei %s konnte nicht geöffnet werden!!!\n", DateiBuffer);
            return;
        }
        else
        {
        	ausgabe(LOG, DEBUGKRECH, "Sichern der ErrechneteKanaeleTemp Variablen in %s gestartet.\n", DateiName);
            for(i = 1; i <= ERRECHNETEKANAELEMAX; i++)
            {
                fwrite(&ErrechneteKanaeleLongTemp[Zaehler - 1][i - 1], sizeof(ErrechneteKanaeleLongTemp[Zaehler - 1][i - 1]), 1, Datei);
                fwrite(&ErrechneteKanaeleDoubleTemp[Zaehler - 1][i - 1], sizeof(ErrechneteKanaeleDoubleTemp[Zaehler - 1][i - 1]), 1, Datei);
        		ausgabe(LOG, DEBUGKRECH, "ErrechneteKanaeleLongTemp[Zaehler - 1][i - 1] = %u\n", ErrechneteKanaeleLongTemp[Zaehler - 1][i - 1]);
        		ausgabe(LOG, DEBUGKRECH, "ErrechneteKanaeleDoubleTemp[Zaehler - 1][i - 1] = %f\n", ErrechneteKanaeleDoubleTemp[Zaehler - 1][i - 1]);

				if(i <= ErrechneteKanaeleAnzahl)
				{	// Nur StatusDatein für Errechnete Kanäle erstellen
        			StatusDatei(SNBuffer, TypeBuffer, ErrechneteKanaeleName[i - 1], ErrechneteKanaeleWert[Zaehler - 1][i - 1]);
        		}
            }
        }
    	fclose(Datei);
    }
}



/**************************************************************************
   Description   : Laden der ErrechneteKanaeleTemp Variablen von einer Datei
   Parameter     : (none)
   Return-Value  : (none)
   Changes       : Author, 		Date, 		Version, 	Reason
                   ********************************************************
                   Dotterweich,	27.08.2007, 0.1, 		Created
**************************************************************************/
void ErrechneteKanaeleLadenTemp(void)
{
	int		i = 1;
	int		Zaehler = 1;
	DWORD	SNBuffer = 0;
	char	Pfad[100] = {0};
	char	DateiName[100] = {0};
	char	DateiBuffer[200] = {0};
    FILE    *Datei = NULL;

    Tools_PathExtractPath(Pfad, ErrechneteKanaeleTempDateiName, sizeof(ErrechneteKanaeleTempDateiName)-1 );
    Tools_PathExtractFile(DateiName, ErrechneteKanaeleTempDateiName, sizeof(ErrechneteKanaeleTempDateiName)-1 );

    for(Zaehler = 1; Zaehler <= AnzahlErfassterWR; Zaehler++)
    {

        GetDeviceSN(WRHandles[Zaehler - 1], &SNBuffer);

		sprintf(DateiBuffer, "%s/%3ld.%s", Pfad, SNBuffer, DateiName);

        Datei = fopen(DateiBuffer, "r");
        if (!Datei)
        {
            ausgabe(LOG, DEBUGMINI, "ErrechneteKanaele.TempVariablenDatei %s konnte nicht geöffnet werden!!!\n", DateiBuffer);
            return;
        }
        else
        {
        	ausgabe(SYSLOG, DEBUGNORMAL, "Laden der ErrechneteKanaeleTemp Variablen in %s gestartet.\n", DateiBuffer);
            for(i = 1; i <= ERRECHNETEKANAELEMAX; i++)
            {
        		ausgabe(LOG, DEBUGINIT, "vor READ ErrechneteKanaeleLongTemp[Zaehler - 1][i - 1] = %u\n", ErrechneteKanaeleLongTemp[Zaehler - 1][i - 1]);
        		ausgabe(LOG, DEBUGINIT, "vor READ ErrechneteKanaeleDoubleTemp[Zaehler - 1][i - 1] = %f\n", ErrechneteKanaeleDoubleTemp[Zaehler - 1][i - 1]);
                fread(&ErrechneteKanaeleLongTemp[Zaehler - 1][i - 1], sizeof(ErrechneteKanaeleLongTemp[Zaehler - 1][i - 1]), 1, Datei);
                fread(&ErrechneteKanaeleDoubleTemp[Zaehler - 1][i - 1], sizeof(ErrechneteKanaeleDoubleTemp[Zaehler - 1][i - 1]), 1, Datei);
        		ausgabe(LOG, DEBUGINIT, "nach READ ErrechneteKanaeleLongTemp[Zaehler - 1][i - 1] = %u\n", ErrechneteKanaeleLongTemp[Zaehler - 1][i - 1]);
        		ausgabe(LOG, DEBUGINIT, "nach READ ErrechneteKanaeleDoubleTemp[Zaehler - 1][i - 1] = %f\n", ErrechneteKanaeleDoubleTemp[Zaehler - 1][i - 1]);
            }
    		fclose(Datei);
		}
	}
}



/**************************************************************************
   Description   : Einlesen des ErrechneteKanaele Bereiches der INI Datei
   Parameter     : (none)
   Return-Value  : (none)
   Changes       : Author, 		Date, 		Version, 	Reason
                   ********************************************************
                   Dotterweich,	24.08.2007, 0.1, 		Created
**************************************************************************/
void ErrechneteKanaeleINI(void)
{
	int		Zaehler = 1;
	char	Temp[100] = {0};
	char	TempText[20] = {0};
	int		TempZahl = 0;

	ErrechneteKanaeleAnzahl = InitAnzahl("ErrechneteKanaele.Anzahl", ERRECHNETEKANAELEMAX);

	for(Zaehler = 1; Zaehler <= ErrechneteKanaeleAnzahl; Zaehler++)
	{
		sprintf(Temp, "ErrechneteKanaele.Name_%i", Zaehler);
		TRepository_GetElementStr(	Temp,
        							"",
        							ErrechneteKanaeleName[Zaehler - 1],
        							sizeof(ErrechneteKanaeleName[Zaehler - 1])-1);
	}

	for(Zaehler = 1; Zaehler <= ErrechneteKanaeleAnzahl; Zaehler++)
	{
		sprintf(Temp, "ErrechneteKanaele.Art_%i", Zaehler);
		TRepository_GetElementStr(	Temp,
        							"",
        							TempText,
        							sizeof(TempText)-1);

		if(strncmp(TempText, "TagesDifferenz", sizeof(TempText)-1) == 0)
		{
            ErrechneteKanaeleArt[Zaehler - 1] = 1;

            sprintf(Temp, "ErrechneteKanaele.StellenNach_%i", Zaehler);
			TempZahl = InitAnzahl(Temp , STELLENMAX);
			ErrechneteKanaeleGenauigkeit[Zaehler - 1] = TempZahl;
            if (TempZahl > 0)
            {
                TempZahl++;
            }

            sprintf(Temp, "ErrechneteKanaele.StellenVor_%i", Zaehler);
			ErrechneteKanaeleZeichen[Zaehler - 1] = TempZahl + InitAnzahl(Temp , STELLENMAX);
		}
		else
		{
            ErrechneteKanaeleArt[Zaehler - 1] = 0;
		}
	}

	for(Zaehler = 1; Zaehler <= ErrechneteKanaeleAnzahl; Zaehler++)
	{
		sprintf(Temp, "ErrechneteKanaele.Kanal_%i", Zaehler);
		TRepository_GetElementStr(	Temp,
        							"",
        							ErrechneteKanaeleKanal[Zaehler - 1],
        							sizeof(ErrechneteKanaeleKanal[Zaehler - 1])-1);
	}

	ErrechneteKanaeleErrechnungsIntervallZeit = TRepository_GetElementInt("ErrechneteKanaele.ErrechnungsIntervall", 0);

    TRepository_GetElementStr(  "ErrechneteKanaele.TempVariablenDatei",
                                "./errechnungs.variablen.datei.tmp",
                                ErrechneteKanaeleTempDateiName,
                                sizeof(ErrechneteKanaeleTempDateiName)-1);

	ErrechneteKanaeleLadenTemp();
}



/**************************************************************************
   Description   : Einen double Wert als Text Formatieren
   Parameter     : Wert			Wert der als Text formatiert werden soll
   				   KanalName	Name des gewünschten Kanales
   				   WertText		Hier wird der Wert als Text abgelegt.
   				   WertTextMAX	Maximale Anzahl der Zeichen von WertText
   Return-Value  : -1 wenn nicht Erfolgreich
   				   sonst den übergebenen Wert
   Changes       : Author, 		Date, 		Version, 	Reason
                   ********************************************************
                   Dotterweich,	03.09.2007, 0.1, 		Created
**************************************************************************/
double GetWertTextCalcKanal(double Wert, int KanalNumme, char *WertText, int WertTextMAX)
{
    snprintf(WertText, WertTextMAX,
             "%*.*f", ErrechneteKanaeleZeichen[KanalNumme],
             ErrechneteKanaeleGenauigkeit[KanalNumme], Wert);								// Wert als Text
    return Wert;
}



/**************************************************************************
   Description   : Aktualisieren der Errechneten Kanäle
   Parameter     : (none)
   Return-Value  : (none)
   Changes       : Author, 		Date, 		Version, 	Reason
                   ********************************************************
                   Dotterweich,	24.08.2007, 0.1, 		Created
**************************************************************************/
void ErrechneteKanaeleAktualisieren(void)
{
	int		i = 1;
	int		Zaehler = 1;
    DWORD	KanalHandle = 0;

	// ErrechneteKanaele aktualisieren???
	if(	ErrechneteKanaeleErrechnungsIntervallZeit == 0 ||																	// Wenn Intervallzeit = 0 ODER
		(ErrechneteKanaeleErrechnungsLetzteIntervallZeit + ErrechneteKanaeleErrechnungsIntervallZeit) > SekundenZeit)		// Wenn Intervallzeit nicht abgelaufen
	{																					// Dann ErrechneteKanaeleAktualisieren Abbrechen
		return;
	}

	ErrechneteKanaeleAktualisierenErfolgreich = FALSE;

	// ErrechneteKanaele aktualisieren möglich???
	if(	AnzahlErfassterWR == 0 ||														// ODER Wenn keine WR erfasst
		!(AnzahlSollWR == AnzahlErfassterWR) ||											// ODER Wenn nicht alle WR erfasst
		(KanalAbfragenMoeglich() == -1))												// ODER Wenn WR Abfrage nicht möglich
	{																					// Dann ErrechneteKanaeleAktualisieren Abbrechen
		return;
	}

	ausgabe(LOG, DEBUGNORMAL, "ErrechneteKanaeleAktualisieren gestartet!!!\tSekunden: %u\n", SekundenZeit);

	ErrechneteKanaeleErrechnungsLetzteIntervallZeit = SekundenZeit;			// ErrechneteKanaele erst nach Ablauf der nächsten Intervallzeit wieder berechnen

	for(Zaehler = 1; Zaehler <= ErrechneteKanaeleAnzahl; Zaehler++)
	{
		switch(ErrechneteKanaeleArt[Zaehler - 1])
		{
			case 1:																			// "TagesDifferenz"
				// Für jeden WR Extra
                for(i = 1; i <= AnzahlErfassterWR; i++)
                {
					double	Wert = 0;
					char	Temp[50] = {0};

					KanalHandle = FindChannelName(WRHandles[i - 1], ErrechneteKanaeleKanal[Zaehler - 1]);

					if(KanalHandle == 0)
					{
						sprintf(ErrechneteKanaeleWert[i - 1][Zaehler - 1], "RECHERR");		// Wenn Kanal nicht gefunden
						return;
					}
					else
					{
                		GetChannelUnit(KanalHandle, ErrechneteKanaeleEinheit[i - 1][Zaehler - 1], sizeof(ErrechneteKanaeleEinheit[i - 1][Zaehler - 1])-1);
                        Wert = KanalAbfragen(KanalHandle, WRHandles[i - 1], Temp, sizeof(Temp)-1);          // Abfragen eines Kanales

						if (Wert == -1)
						{																	// Wenn Kanalabfrage nicht erfolgreich ABBRECHEN
							return;
						}

                        // TagesDifferenz Werte nach Ablauf eines Tages Zurücksetzen
                        if( ErrechneteKanaeleLongTemp[i - 1][Zaehler - 1] == 0 ||           // Wenn TagesDifferenzLetzterReset == 0 ODER
                            ErrechneteKanaeleLongTemp[i - 1][Zaehler - 1] < TagZeit)		// Wenn ein Tag abgelaufen
                        {                                                                   // Dann TagesDifferenzWert = 0
                            ErrechneteKanaeleLongTemp[i - 1][Zaehler - 1] = TagZeit;        // TagesDifferenz Kanäle erst am nächsten Tag zurücksetzen
                            ErrechneteKanaeleDoubleTemp[i - 1][Zaehler - 1] = Wert;         // TagesEndWert setzen
                        }

						ErrechneteKanaeleWertZahl[i - 1][Zaehler - 1] = Wert - ErrechneteKanaeleDoubleTemp[i - 1][Zaehler - 1];		// Wert errechnen

        				GetWertTextCalcKanal( ErrechneteKanaeleWertZahl[i - 1][Zaehler - 1], Zaehler - 1,
        									  ErrechneteKanaeleWert[i - 1][Zaehler - 1], sizeof(ErrechneteKanaeleWert[i - 1][Zaehler - 1])-1);	// Wert als Text
   					}
                }
				break;

			case 2:
				break;
		}
	}

	ErrechneteKanaeleAktualisierenErfolgreich = TRUE;										// Aktualisieren erfolgreich
	ErrechneteKanaeleSichernTemp();
}



/**************************************************************************
   Description   : Gibt die Nummer eines errechneten Kanals zurück
   Parameter     : KanalName	Name des gewünschten Kanales
   Return-Value  : -1 wenn nicht Erfolgreich
   Changes       : Author, 		Date, 		Version, 	Reason
                   ********************************************************
                   Dotterweich,	03.09.2007, 0.1, 		Created
**************************************************************************/
int GetNummerCalcKanal(char *KanalName)
{
	int		Zaehler = 0;

	for(Zaehler = 1; Zaehler <= ErrechneteKanaeleAnzahl; Zaehler++)
	{
		if(strncmp(KanalName, ErrechneteKanaeleName[Zaehler - 1], strlen(KanalName)) == 0)
		{
			return Zaehler - 1;
		}
	}
	return -1;
}



/**************************************************************************
   Description   : Liefert den Wert eines Errechneten Kanales eines WR
   				   oder aller WR zurück.
   Parameter     : KanalName	Name des gewünschten Kanales
   				   SN			SN des gewünschten WR
   				   				wenn "9999999999" dann Summe aller WR
   				   WertText		Hier wird der Wert als Text abgelegt.
   				   WertTextMAX	Maximale Anzahl der Zeichen von WertText
   Return-Value  : -1 wenn nicht Erfolgreich
   Changes       : Author, 		Date, 		Version, 	Reason
                   ********************************************************
                   Dotterweich,	03.09.2007, 0.1, 		Created
**************************************************************************/
double GetWertCalcKanalSN(char *KanalName, char *SN, char *WertText, int WertTextMAX)
{
	int		Nummer = -1;
	int		i = 0;
	double	Summe = 0;
	DWORD	SNBuffer = 0;
	char	*SNBufferText[SNMAX] = {0};

	Nummer = GetNummerCalcKanal(KanalName);

	if(Nummer == -1)
	{
		return -1;
	}
	else
	{
        for(i = 1; i <= AnzahlErfassterWR; i++)
        {
            GetDeviceSN(WRHandles[i - 1], &SNBuffer);
            snprintf(SNBufferText, sizeof(SNBufferText) - 1, "%3ld", SNBuffer);

            if(strncmp(SNBufferText, SN, strlen(SNBufferText)) == 0)
            {
                return GetWertTextCalcKanal(ErrechneteKanaeleWertZahl[i - 1][Nummer], Nummer, WertText, WertTextMAX);
            }
            Summe = Summe + ErrechneteKanaeleWertZahl[i - 1][Nummer];
        }

        if(strncmp("9999999999", SN, strlen("9999999999")) == 0)
        {
            return GetWertTextCalcKanal(Summe, Nummer, WertText, WertTextMAX);
        }
	}
	return -1;
}



/**************************************************************************
   Description   : Liefert die Maximale Anzahl von Zeichen eines
   				   Errechneten Kanales zurück.
   Parameter     : KanalName	Name des gewünschten Kanales
   				   SN			SN des gewünschten WR
   				   				wenn "9999999999" dann Summe aller WR
   				   WertText		Hier wird der Wert als Text abgelegt.
   				   WertTextMAX	Maximale Anzahl der Zeichen von WertText
   Return-Value  : 15 wenn nicht Erfolgreich (da 15 die Standartlänge ist)
   Changes       : Author, 		Date, 		Version, 	Reason
                   ********************************************************
                   Dotterweich,	03.09.2007, 0.1, 		Created
**************************************************************************/
int GetZeichenCalcKanal(char *KanalName)
{
	int	Nummer = -1;

	Nummer = GetNummerCalcKanal(KanalName);

	if(Nummer == -1)
	{
		return 15;
	}
	else
	{
		return ErrechneteKanaeleZeichen[Nummer];
	}
}



// Ende
