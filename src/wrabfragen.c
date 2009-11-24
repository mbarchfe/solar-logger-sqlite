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
*  Abfragen der Momentanwerte der Wechselrichter
*
***************************************************************************
*  DateiName:	wrabfragen.c
*  Status:		InArbeit
***************************************************************************/

#include "solar-logger.h"
#include "log.h"
#include "wrerfassen.h"
#include "solstatus.h"



// Globale Variable
int		WRKanaeleAnzahl = 0;							// Anzahl der konfigurierten WR Kanäle
char	WRKanaeleName[KANALMAX][20]={0};				// Namen der Kanäle
int		WRKanaeleArt[KANALMAX]={0};						// Art der Kanäle
int		WRKanaeleZeichen[KANALMAX]={0};					// Anzahl der Zeichen String mindestlänge
int		WRKanaeleGenauigkeit[KANALMAX]={0};				// Anzahl der Stellen hinter dem Komma bei Zahlen



/**************************************************************************
   Description   : Einlesen des WRKanaele Bereiches der INI Datei
   Parameter     : (none)
   Return-Value  : (none)
   Changes       : Author, 		Date, 		Version, 	Reason
                   ********************************************************
                   Dotterweich,	25.08.2007, 0.1, 		Created
**************************************************************************/
void WRKanaeleINI(void)
{
	int		Zaehler = 1;
	char	Temp[100] = {0};
	char	TempText[20] = {0};
	int		TempZahl = 0;

	WRKanaeleAnzahl = InitAnzahl("WRKanaele.Anzahl", KANALMAX);

	for(Zaehler = 1; Zaehler <= WRKanaeleAnzahl; Zaehler++)
	{
		sprintf(Temp, "WRKanaele.Name_%i", Zaehler);
		TRepository_GetElementStr(	Temp,
        							"",
        							WRKanaeleName[Zaehler - 1],
        							sizeof(WRKanaeleName[Zaehler - 1])-1);
	}

	for(Zaehler = 1; Zaehler <= WRKanaeleAnzahl; Zaehler++)
	{
		sprintf(Temp, "WRKanaele.Art_%i", Zaehler);
		TRepository_GetElementStr(	Temp,
        							"",
        							TempText,
        							sizeof(TempText)-1);

		if(strncmp(TempText, "Zahl", sizeof(TempText)-1) == 0)
		{
            WRKanaeleArt[Zaehler - 1] = 1;

            sprintf(Temp, "WRKanaele.StellenNach_%i", Zaehler);
			TempZahl = InitAnzahl(Temp , STELLENMAX);
			WRKanaeleGenauigkeit[Zaehler - 1] = TempZahl;
            if (TempZahl > 0)
            {
                TempZahl++;
            }

            sprintf(Temp, "WRKanaele.StellenVor_%i", Zaehler);
			WRKanaeleZeichen[Zaehler - 1] = TempZahl + InitAnzahl(Temp , STELLENMAX);
		}
		else if(strncmp(TempText, "Text", sizeof(TempText)-1) == 0)
		{
            WRKanaeleArt[Zaehler - 1] = 2;

            sprintf(Temp, "WRKanaele.Zeichen_%i", Zaehler);
			WRKanaeleZeichen[Zaehler - 1] = InitAnzahl(Temp , ZEICHENMAX);
		}
		else
		{
            WRKanaeleArt[Zaehler - 1] = 0;
		}
	}
}



/**************************************************************************
   Description   : Ermitteln der Maximalen Anzahl von Zeichen eines Kanal Wertes
   Parameter     : (none)
   Return-Value  : (none)
   Changes       : Author, 		Date, 		Version, 	Reason
                   ********************************************************
                   Dotterweich,	26.08.2007, 0.1, 		Created
**************************************************************************/
int KanalWertZeichenAnzahlMAX(DWORD KanalHandle)
{
	int		Temp = 0;
	int		Zaehler = 1;
	char	KanalName[50] = {0};

    GetChannelName(KanalHandle, KanalName, sizeof(KanalName)-1);

	// Überprüfen ob Kanal konfiguriert ist???
	for(Zaehler = 1; Zaehler <= WRKanaeleAnzahl; Zaehler++)
	{
		if (strncmp(KanalName, WRKanaeleName[Zaehler - 1], sizeof(KanalName)-1) == 0)
		{
            return WRKanaeleZeichen[Zaehler - 1];
		}
	}

	// Wenn Kanal nicht oder falsche Art konfiguriert ist dann Standartwerte verwenden
	// Für Zahl Kanäle 10 Stellen vor und 4 Stellen nach dem Komme
	// Und für Text Kanäle 15 Zeichen
	return 15;
}



/**************************************************************************
   Description   : Einen double Wert als Text Formatieren
   Parameter     : (none)
   Return-Value  : Gibt -1 zurück wenn nicht erfolgreich
   Changes       : Author, 		Date, 		Version, 	Reason
                   ********************************************************
                   Dotterweich,	02.09.2007, 0.1, 		Created
**************************************************************************/
double KanalWertFormatieren(double Wert, DWORD KanalHandle, char *WertText, int Zeichen)
{
	int		Zaehler = 1;
    int 	Ergebnis = 0;
	char	KanalName[KANALNAMEMAX] = {0};
	char	Temp[KANALWERTTEXTMAX] = {0};

    GetChannelName(KanalHandle, KanalName, sizeof(KanalName)-1);

	// Überprüfen ob Kanal konfiguriert ist???
	for(Zaehler = 1; Zaehler <= WRKanaeleAnzahl; Zaehler++)
	{
		if (strncmp(KanalName, WRKanaeleName[Zaehler - 1], sizeof(KanalName)-1) == 0)
		{
            switch(WRKanaeleArt[Zaehler - 1])
            {
                case 1:											// "Zahl"
                    if(Wert == -1)
                    {	// Fehler Beim Wert
                        snprintf(WertText, Zeichen, "%*s", WRKanaeleZeichen[Zaehler - 1], "ERR");
                        return -1;
                    }
                    else
                    {	// Alles OK
        				snprintf(WertText, Zeichen, "%*.*f", WRKanaeleZeichen[Zaehler - 1], WRKanaeleGenauigkeit[Zaehler - 1], Wert);
                        return Wert;
                    }

            	case 2:											// "Text"
                    if(Wert == -1)
                    {	// Fehler Beim Wert lesen
                        snprintf(WertText, Zeichen, "%*s", WRKanaeleZeichen[Zaehler - 1], "ERR");
                        return -1;
                    }
                    else
                    {	// Alles OK
						snprintf(Temp, sizeof(Temp)-1, "%*s", WRKanaeleZeichen[Zaehler - 1], WertText);
            			snprintf(WertText, Zeichen, "%s", Temp);
                        return Wert;
                    }
            }
		}
	}

	// Wenn Kanal nicht oder falsche Art konfiguriert ist dann Standartwerte verwenden
	// Für Zahl Kanäle 10 Stellen vor und 4 Stellen nach dem Komme
	// Und für Text Kanäle 15 Zeichen

	ausgabe(LOG, DEBUGALLES, "Kanal '%s' nicht konfiguriert!!! Standart Werte verwendet\n", KanalName);

    if(Wert == -1)
    {	// Fehler Beim Wert
        snprintf(WertText, Zeichen, "KanalAbfrageERR");
        return -1;
    }
    else
    {	// Alles OK
        if (strlen(WertText) == 0)             	// Wenn kein Statustext dann Wert in Text umwandeln
        {
            snprintf(WertText, Zeichen, "%10.4f", Wert);
        }
        else
        {										// WertText soll immer 15 Zeichen lang sein
            snprintf(Temp, sizeof(Temp)-1, "%15s", WertText);
            snprintf(WertText, Zeichen, "%s", Temp);
        }
        return Wert;
    }
}



/**************************************************************************
   Description   : Abfragen eines Kanales eines WR
   Parameter     : (none)
   Return-Value  : Gibt -1 zurück wenn nicht erfolgreich
   Changes       : Author, 		Date, 		Version, 	Reason
                   ********************************************************
                   Dotterweich,	03.09.2007, 0.1, 		Aktualisiert
**************************************************************************/
double KanalAbfragen(DWORD KanalHandle, DWORD WRHandle, char *WertText, int Zeichen)
{
	int		Zaehler = 1;
    int 	Ergebnis = 0;
    double	WertDouble = 0;
	char	KanalName[50] = {0};
	char	Temp[50] = {0};

    GetChannelName(KanalHandle, KanalName, sizeof(KanalName)-1);
    Ergebnis = GetChannelValue( KanalHandle, WRHandle, &WertDouble, WertText,
                                Zeichen, WERTALTERMAX);

	if(Ergebnis == 0)
	{
		return KanalWertFormatieren(WertDouble, KanalHandle, WertText, Zeichen);
	}
	else
	{
		return KanalWertFormatieren(-1, KanalHandle, WertText, Zeichen);
	}
}



/**************************************************************************
   Description   :
   Parameter     : (none)
   Return-Value  : Gibt -1 zurück wenn nicht erfolgreich
   Changes       : Author, 		Date, 		Version, 	Reason
                   ********************************************************
                   Dotterweich,	26.08.2007, 0.1, 		Created
**************************************************************************/
double KanalAbfragenSN(DWORD KanalHandle, char *SN, char *WertText, int Zeichen)
{
	int		Zaehler = 0;
	char	Temp[50] = {0};
	DWORD	SNBuffer = 0;
	double	Summe = 0;

    if(strncmp("9999999999", SN, strlen("9999999999")) == 0)
    {
        for(Zaehler = 1; Zaehler <= AnzahlErfassterWR; Zaehler++)
        {
            Summe = Summe + KanalAbfragen(KanalHandle, WRHandles[Zaehler - 1], WertText, Zeichen);
        }
        return KanalWertFormatieren(Summe, KanalHandle, WertText, Zeichen);
    }

    for(Zaehler = 1; Zaehler <= AnzahlErfassterWR; Zaehler++)
    {
        GetDeviceSN(WRHandles[Zaehler - 1], &SNBuffer);
        sprintf(Temp, "%3ld", SNBuffer);
        if(strncmp(Temp, SN, strlen(Temp)) == 0)
        {
			return KanalAbfragen(KanalHandle, WRHandles[Zaehler - 1], WertText, Zeichen);
        }
	}
	return -1;
}



/**************************************************************************
   Description   : Testen ob eine Kanal abfrage möglich ist
   Parameter     : (none)
   Return-Value  : Gibt -1 zurück wenn nicht erfolgreich
   Changes       : Author, 		Date, 		Version, 	Reason
                   ********************************************************
                   Dotterweich,	26.08.2007, 0.1, 		Created
**************************************************************************/
int KanalAbfragenMoeglich(void)
{
	int		Zaehler = 1;
    int 	Ergebnis = 0;
    double	WertDouble = 0;
	char	WertText[50] = {0};
	char	Temp[50] = {0};

	if (GetChannelValue (FindChannelName(WRHandles[0], WRKanaeleName[0]), WRHandles[0], &WertDouble, WertText, sizeof(WertText)-1, 0) == 0)
	{								// Wenn möglich
		return 0;
	}
	else
	{								// Wenn nicht möglich
		return -1;
	}
}



// Ende
