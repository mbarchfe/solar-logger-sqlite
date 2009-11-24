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
*  Erstellen der Logdateien der WR
*
***************************************************************************
*  DateiName:	sollog.c
*  Status:		InArbeit
***************************************************************************/

#include "solar-logger.h"
#include "log.h"
#include "ini.h"
#include "kanalrech.h"
#include "timer.h"
#include "wrerfassen.h"
#include "wrabfragen.h"



#define LogMonatSummeUeberschrift	"Gesamt Summen"	// WR Überschrift für die Summen aller WR



// Globale Variable
int		LogGesamtOutputTypeAnzahl=0;				// Anzahl der AusgabeArten
int		LogGesamtOutputType[OUTPUTTYPEMAX]={0};		// Wie oder Wo sollen die Daten gespeichert oder Übertragen werden (Local, FTP, HTTP-Post)
char	LogGesamtOutput[OUTPUTTYPEMAX][DATEINAMEPFADMAX]={0};	// AusgabeDateiAnfang für die LogGesamt Dateien
int		LogGesamtOutputFormat[OUTPUTTYPEMAX]={0};	// Format der AusgabeDatei für die UebersichtGesamt (txt, csv, ...)
long	LogGesamtIntervallZeit = 0;					// Interfallzeit in Sekunden für LogGesamt
long	LogGesamtLetzteIntervallZeit = 0;			// Zeitpunkt des letzten LogGesamt

int		LogOutputTypeAnzahl=0;						// Anzahl der AusgabeArten
int		LogOutputType[OUTPUTTYPEMAX]={0};			// Wie oder Wo sollen die Daten gespeichert oder Übertragen werden (Local, FTP, HTTP-Post)
char	LogOutput[OUTPUTTYPEMAX][100]={0};			// AusgabeDateiAnfang für die LogGesamt Dateien
int		LogOutputFormat[OUTPUTTYPEMAX]={0};			// Format der AusgabeDatei für die UebersichtGesamt (txt, csv, ...)
long	LogIntervallZeit = 0;						// Interfallzeit in Sekunden für LogGesamt
long	LogLetzteIntervallZeit = 0;					// Zeitpunkt des letzten LogGesamt
int		LogKanalAnzahl = 0;							// Anzahl der Kanäle
char	LogKanal[KANALMAX][20]={0};					// Namen der Kanäle

int		LogMonatOutputTypeAnzahl=0;					// Anzahl der AusgabeArten
int		LogMonatOutputType[OUTPUTTYPEMAX]={0};		// Wie oder Wo sollen die Daten gespeichert oder Übertragen werden (Local, FTP, HTTP-Post)
char	LogMonatOutput[OUTPUTTYPEMAX][DATEINAMEPFADMAX]={0};	// AusgabeDateiAnfang für die LogGesamt Dateien
int		LogMonatOutputFormat[OUTPUTTYPEMAX]={0};	// Format der AusgabeDatei für die UebersichtGesamt (txt, csv, ...)
long	LogMonatLetzteIntervallZeit = 0;			// Zeitpunkt des letzten LogGesamt
int		LogMonatWRAnzahl = 0;						// Anzahl der WR
char	LogMonatWR[DEVMAX][SNMAX]={0};				// SN der WR (9999999999 für Summe aller WR)
int		LogMonatKanalAnzahl = 0;					// Anzahl der Kanäle
char	LogMonatKanal[KANALMAX][KANALNAMEMAX]={0};				// Namen der Kanäle



/**************************************************************************
   Description   : Einlesen des Loggen Bereiches der INI Datei
   Parameter     : (none)
   Return-Value  : (none)
   Changes       : Author, 		Date, 		Version, 	Reason
                   ********************************************************
                   Dotterweich,	21.08.2007, 0.1, 		Created
**************************************************************************/
void LoggenINI(void)
{
    int     Zaehler = 1;
    char    Temp[100] = {0};

	{
        LogGesamtOutputTypeAnzahl = InitAnzahl("Loggen.LogGesamtOutputTypeAnzahl", OUTPUTTYPEMAX);

        InitOutputType(LogGesamtOutputTypeAnzahl, LogGesamtOutputType, "Loggen.LogGesamtOutputType");

        for(Zaehler = 1; Zaehler <= LogGesamtOutputTypeAnzahl; Zaehler++)
        {
            sprintf(Temp, "Loggen.LogGesamtOutput_%i", Zaehler);
            TRepository_GetElementStr(  Temp,
                                        "",
                                        LogGesamtOutput[Zaehler - 1],
                                        sizeof(LogGesamtOutput[Zaehler - 1])-1);
        }

        InitOutputFormat(LogGesamtOutputTypeAnzahl, LogGesamtOutputFormat, "Loggen.LogGesamtOutputFormat");

        LogGesamtIntervallZeit = TRepository_GetElementInt("Loggen.LogGesamtIntervall", 0);
	}

	{
        LogOutputTypeAnzahl = InitAnzahl("Loggen.LogOutputTypeAnzahl", OUTPUTTYPEMAX);

        InitOutputType(LogOutputTypeAnzahl, LogOutputType, "Loggen.LogOutputType");

        for(Zaehler = 1; Zaehler <= LogOutputTypeAnzahl; Zaehler++)
        {
            sprintf(Temp, "Loggen.LogOutput_%i", Zaehler);
            TRepository_GetElementStr(  Temp,
                                        "",
                                        LogOutput[Zaehler - 1],
                                        sizeof(LogOutput[Zaehler - 1])-1);
        }

        InitOutputFormat(LogOutputTypeAnzahl, LogOutputFormat, "Loggen.LogOutputFormat");

        LogIntervallZeit = TRepository_GetElementInt("Loggen.LogIntervall", 0);

        LogKanalAnzahl = InitAnzahl("Loggen.LogKanalAnzahl", KANALMAX);

        for(Zaehler = 1; Zaehler <= LogKanalAnzahl; Zaehler++)
        {
            sprintf(Temp, "Loggen.LogKanal_%i", Zaehler);
            TRepository_GetElementStr(  Temp,
                                        "",
                                        LogKanal[Zaehler - 1],
                                        sizeof(LogKanal[Zaehler - 1])-1);
        }
	}

	{
        LogMonatOutputTypeAnzahl = InitAnzahl("Loggen.LogMonatOutputTypeAnzahl", OUTPUTTYPEMAX);

        InitOutputType(LogMonatOutputTypeAnzahl, LogMonatOutputType, "Loggen.LogMonatOutputType");

        for(Zaehler = 1; Zaehler <= LogMonatOutputTypeAnzahl; Zaehler++)
        {
            sprintf(Temp, "Loggen.LogMonatOutput_%i", Zaehler);
            TRepository_GetElementStr(  Temp,
                                        "",
                                        LogMonatOutput[Zaehler - 1],
                                        sizeof(LogMonatOutput[Zaehler - 1])-1);
        }

        InitOutputFormat(LogMonatOutputTypeAnzahl, LogMonatOutputFormat, "Loggen.LogMonatOutputFormat");

		LogMonatLetzteIntervallZeit = TagZeit;							// LogMonat erst um 0:00 wieder starten

        LogMonatWRAnzahl = InitAnzahl("Loggen.LogMonatWRAnzahl", KANALMAX);

        for(Zaehler = 1; Zaehler <= LogMonatWRAnzahl; Zaehler++)
        {
            sprintf(Temp, "Loggen.LogMonatWR_%i", Zaehler);
            TRepository_GetElementStr(  Temp,
                                        "",
                                        LogMonatWR[Zaehler - 1],
                                        sizeof(LogMonatWR[Zaehler - 1])-1);
        }

        LogMonatKanalAnzahl = InitAnzahl("Loggen.LogMonatKanalAnzahl", KANALMAX);

        for(Zaehler = 1; Zaehler <= LogMonatKanalAnzahl; Zaehler++)
        {
            sprintf(Temp, "Loggen.LogMonatKanal_%i", Zaehler);
            TRepository_GetElementStr(  Temp,
                                        "",
                                        LogMonatKanal[Zaehler - 1],
                                        sizeof(LogMonatKanal[Zaehler - 1])-1);
        }
	}
}



/**************************************************************************
   Description   : Erstellen der Log Gesamt Dateien
   Parameter     : (none)
   Return-Value  : (none)
   Changes       : Author, 		Date, 		Version, 	Reason
                   ********************************************************
                   Dotterweich,	22.08.2007, 0.1, 		Created
**************************************************************************/
void LogGesamtDatei(char *Pfad, char *OutputDatei, int Format)
{
	int		Zaehler = 1;

	ausgabe(LOG, DEBUGMINI, "LogGesamtDatei gestartet!!!\n");

    for(Zaehler = 1; Zaehler <= AnzahlErfassterWR; Zaehler++)
    {
        BOOL	NeueDatei = true;
        char	NameBuffer[50] = {0};
    	char	TypeBuffer[50] = {0};
		DWORD	SNBuffer = 0;
		char	DatumBuffer[20] = {0};
    	char	DateiBuffer[200] = {0};
		FILE	*Datei = NULL;

        GetDeviceName(WRHandles[Zaehler - 1], NameBuffer, sizeof(NameBuffer)-1);
        GetDeviceType(WRHandles[Zaehler - 1], TypeBuffer, sizeof(TypeBuffer)-1);
        GetDeviceSN(WRHandles[Zaehler - 1], &SNBuffer);
        GetDatum(DatumBuffer, sizeof(DatumBuffer)-1);

        ausgabe(LOG, DEBUGALLES, "LogGesamtDatei für WR %3ld erstellen\n", SNBuffer);

		sprintf(DateiBuffer, "%s/%s.%3ld.%s.%s", Pfad, DatumBuffer, SNBuffer, TypeBuffer, OutputDatei);

        Datei = fopen(DateiBuffer, "r");
        ausgabe(LOG, DEBUGALLES, "Überprüfen ob LogDatei %s geöffnet werden kann!!!\n", DateiBuffer);
        if (Datei)                   			// Wenn Datei geöffnet werden konnte dann wurde sie bereits erstellt
        {                                   	// und es muss kein Dateikopf mehr erstellt werden.
            ausgabe(LOG, DEBUGALLES, "LogDatei %s konnte geöffnet werden!!!\n", DateiBuffer);
            NeueDatei = false;
            fclose(Datei);
        }

        Datei = fopen(DateiBuffer, "a");
        if (!Datei)
        {
            ausgabe(LOG, DEBUGMINI, "LogDatei %s konnte nicht geöffnet werden!!!\n", DateiBuffer);
            return;
        }
        else
        {
        	int			i = 0;
        	int			KanalAnzahl = 0;
            DWORD		KanalHandle[KANALMAX];
            TChanType 	KanalType = ALLCHANNELS;
            char		KanalName[50] = {0};
            char		WertText[50]={0};
			char		ZeitpunktBuffer[50] = {0};

            ausgabe(LOG, DEBUGALLES, "Ausgabe in Datei %s wird gestartet.\n", DateiBuffer);
            setbuf(Datei, 0);                      									// Pufferung abschalten

            // Ab hier Ausgaben in Datei
            KanalAnzahl = GetChannelHandlesEx(WRHandles[Zaehler - 1], KanalHandle, KANALMAX, KanalType);
            GetZeitpunkt(ZeitpunktBuffer, sizeof(ZeitpunktBuffer)-1);

            if(NeueDatei)
            {
                fprintf(Datei, "LogDatei für WR %3ld erstellt am %s von %s %s\r\n", SNBuffer, ZeitpunktBuffer, ProgrammName, ProgrammVersion);
                fprintf(Datei, "WR '%s' hat %d Kanäle:\r\n", NameBuffer, KanalAnzahl);
                fprintf(Datei, "---------------------------------------------------------------------------------------------------------------------------\r\n");
                fprintf(Datei, "Zeitpunkt          \t");
				// Kanäle der WR
                for(i = 0; i < KanalAnzahl; i++)
                {
                    GetChannelName(KanalHandle[i], KanalName, sizeof(KanalName)-1);

                    fprintf(Datei, "%*s\t", KanalWertZeichenAnzahlMAX(KanalHandle[i]), KanalName);
                }
                // Die Errechneten Kanaele
                for(i = 0; i < ErrechneteKanaeleAnzahl; i++)
                {
                    fprintf(Datei, "%*s\t", ErrechneteKanaeleZeichen[i], ErrechneteKanaeleName[i]);
                }
            }

            fprintf(Datei, "\r\n");
            fprintf(Datei, "%s\t", ZeitpunktBuffer);

			// Kanäle der WR
            for(i = 0; i < KanalAnzahl; i++)
            {
            	KanalAbfragen(KanalHandle[i], WRHandles[Zaehler - 1], WertText, sizeof(WertText)-1);		// Abfragen eines Kanales

				fprintf(Datei, "%s\t", WertText);
            }
            // Die Errechneten Kanaele
            for(i = 0; i < ErrechneteKanaeleAnzahl; i++)
            {
                fprintf(Datei, "%s\t", ErrechneteKanaeleWert[Zaehler - 1][i]);
            }
            ausgabe(LOG, DEBUGALLES, "Ausgabe in Dateien ist beendet.\n");
		}
		fclose(Datei);
	}
}



/**************************************************************************
   Description   : Erstellen der Log Dateien
   Parameter     : (none)
   Return-Value  : (none)
   Changes       : Author, 		Date, 		Version, 	Reason
                   ********************************************************
                   Dotterweich,	01.09.2007, 0.1, 		Created
**************************************************************************/
void LogDatei(char *Pfad, char *OutputDatei, int Format)
{
	int		Zaehler = 1;

	ausgabe(LOG, DEBUGALLES, "LogDatei gestartet!!!\n");

    for(Zaehler = 1; Zaehler <= AnzahlErfassterWR; Zaehler++)
    {
        BOOL	NeueDatei = true;
        char	NameBuffer[50] = {0};
    	char	TypeBuffer[50] = {0};
		DWORD	SNBuffer = 0;
		char	DatumBuffer[20] = {0};
    	char	DateiBuffer[200] = {0};
		FILE	*Datei = NULL;

        GetDeviceName(WRHandles[Zaehler - 1], NameBuffer, sizeof(NameBuffer)-1);
        GetDeviceType(WRHandles[Zaehler - 1], TypeBuffer, sizeof(TypeBuffer)-1);
        GetDeviceSN(WRHandles[Zaehler - 1], &SNBuffer);
        GetDatum(DatumBuffer, sizeof(DatumBuffer)-1);

        ausgabe(LOG, DEBUGALLES, "LogDatei für WR %3ld erstellen\n", SNBuffer);

		sprintf(DateiBuffer, "%s/%s.%3ld.%s.%s", Pfad, DatumBuffer, SNBuffer, TypeBuffer, OutputDatei);

        Datei = fopen(DateiBuffer, "r");
        ausgabe(LOG, DEBUGALLES, "Überprüfen ob LogDatei %s geöffnet werden kann!!!\n", DateiBuffer);
        if (Datei)                   			// Wenn Datei geöffnet werden konnte dann wurde sie bereits erstellt
        {                                   	// und es muss kein Dateikopf mehr erstellt werden.
            ausgabe(LOG, DEBUGALLES, "LogDatei %s konnte geöffnet werden!!!\n", DateiBuffer);
            NeueDatei = false;
            fclose(Datei);
        }

        Datei = fopen(DateiBuffer, "a");
        if (!Datei)
        {
            ausgabe(LOG, DEBUGMINI, "LogDatei %s konnte nicht geöffnet werden!!!\n", DateiBuffer);
            return;
        }
        else
        {
        	int			i = 0;
        	int			j = 0;
        	int			KanalAnzahl = 0;
            DWORD		KanalHandle;
            TChanType 	KanalType = ALLCHANNELS;
            char		KanalName[50] = {0};
            char		WertText[50]={0};
			char		ZeitpunktBuffer[50] = {0};

            ausgabe(LOG, DEBUGALLES, "Ausgabe in Datei %s wird gestartet.\n", DateiBuffer);
            setbuf(Datei, 0);                      									// Pufferung abschalten

            // Ab hier Ausgaben in Datei
            GetZeitpunkt(ZeitpunktBuffer, sizeof(ZeitpunktBuffer)-1);

            if(NeueDatei)
            {
                fprintf(Datei, "LogDatei für WR %3ld erstellt am %s von %s %s\r\n", SNBuffer, ZeitpunktBuffer, ProgrammName, ProgrammVersion);
                fprintf(Datei, "---------------------------------------------------------------------------------------------------------------------------\r\n");
                fprintf(Datei, "Zeitpunkt          \t");

				ausgabe(LOG, DEBUGALLES, "LogKanalAnzahl %i\n", LogKanalAnzahl);

                for(i = 1; i <= LogKanalAnzahl; i++)
                {
                    KanalHandle = FindChannelName(WRHandles[Zaehler - 1], LogKanal[i - 1]);

					ausgabe(LOG, DEBUGALLES, "1. KanalHandle %i\n", KanalHandle);

                    if(KanalHandle == 0)
                    {           // Kanal nicht vom WR
                                // Die Errechneten Kanaele durchsuchen
                        for(j = 1; j <= ErrechneteKanaeleAnzahl; j++)
                        {
                            if(strncmp(LogKanal[i - 1], ErrechneteKanaeleName[j - 1], sizeof(LogKanal[i - 1])-1) == 0)
                            {
                                KanalHandle = j;
                                break;
                            }
                        }
                        if(KanalHandle == 0)
                        {           // Kanal nicht gefunden
                    		fprintf(Datei, "???%9s???\t", LogKanal[i - 1]);
                        }
                        else
                        {           // Errechneter Kanal
                    		fprintf(Datei, "%*s\t", ErrechneteKanaeleZeichen[KanalHandle - 1], ErrechneteKanaeleName[KanalHandle - 1]);
                        }
                    }
                    else
                    {           // Kanal vom WR
                        GetChannelName(KanalHandle, KanalName, sizeof(KanalName)-1);
                        fprintf(Datei, "%*s\t", KanalWertZeichenAnzahlMAX(KanalHandle), KanalName);
                    }
                }
            }

            fprintf(Datei, "\r\n");
            fprintf(Datei, "%s\t", ZeitpunktBuffer);

            for(i = 1; i <= LogKanalAnzahl; i++)
            {
                KanalHandle = FindChannelName(WRHandles[Zaehler - 1], LogKanal[i - 1]);

                if(KanalHandle == 0)
                {           // Kanal nicht vom WR
                            // Die Errechneten Kanaele durchsuchen
                    for(j = 1; j <= ErrechneteKanaeleAnzahl; j++)
                    {
                        if(strncmp(LogKanal[i - 1], ErrechneteKanaeleName[j - 1], sizeof(LogKanal[i - 1])-1) == 0)
                        {
                            KanalHandle = j;
                            break;
                        }
                    }
                    if(KanalHandle == 0)
                    {           // Kanal nicht gefunden
                        fprintf(Datei, "               \t");
                    }
                    else
                    {           // Errechneter Kanal
                		fprintf(Datei, "%s\t", ErrechneteKanaeleWert[Zaehler - 1][KanalHandle - 1]);
                    }
                }
                else
                {           // Kanal vom WR
            	KanalAbfragen(KanalHandle, WRHandles[Zaehler - 1], WertText, sizeof(WertText)-1);		// Abfragen eines Kanales
				fprintf(Datei, "%s\t", WertText);
                }
            }
            ausgabe(LOG, DEBUGALLES, "Ausgabe in Dateien ist beendet.\n");
		}
		fclose(Datei);
	}
}



/**************************************************************************
   Description   : Ermittel der Zeichenanzahl für die erste Spalte eines WR
   Parameter     : (none)
   Return-Value  : Zeichenanzahl
   Changes       : Author, 		Date, 		Version, 	Reason
                   ********************************************************
                   Dotterweich,	06.09.2007, 0.1, 		Created
**************************************************************************/
int LogMonatDateiZeichenAnzahl(int LogMonatKanalNummer, int ZeichenAnzahlNormal, int LogMonatWRNummer)
{
    if(LogMonatKanalNummer == 1)
    {	// Wenn erste Splate
        if(strncmp(LogMonatWR[LogMonatWRNummer - 1], "9999999999", 10) == 0)
        {
            if(ZeichenAnzahlNormal < strlen(LogMonatSummeUeberschrift))
            {
            	return strlen(LogMonatSummeUeberschrift);
            }
        }

        if(ZeichenAnzahlNormal < strlen(LogMonatWR[LogMonatWRNummer - 1]))
        {
            return strlen(LogMonatWR[LogMonatWRNummer - 1]);
        }
    }
   	// Wenn nicht erste Spalte oder Wenn ZeichenAnzahlNormal >= strlen(LogMonatWR[LogMonatWRNummer - 1]
    return ZeichenAnzahlNormal;
}



/**************************************************************************
   Description   : Erstellen der Log Datei der letzten ermittelten Werte
                   von ausgewählten WR in eine Datei um 0:00
   Parameter     : (none)
   Return-Value  : (none)
   Changes       : Author, 		Date, 		Version, 	Reason
                   ********************************************************
                   Dotterweich,	03.09.2007, 0.1, 		Created
**************************************************************************/
void LogMonatDatei(char *Pfad, char *OutputDatei, int Format)
{
    int     i = 0;
    int     j = 0;
    int     k = 0;
	int		Zaehler = 1;
	char	DatumBuffer[DATUMMAX] = {0};
	char	ZeitpunktBuffer[ZEITPUNKTMAX] = {0};
    char	DateiBuffer[DATEINAMEPFADMAX] = {0};
    BOOL    NeueDatei = true;
    FILE    *Datei = NULL;
    char	KanalName[KANALNAMEMAX] = {0};
    char	WertText[KANALWERTTEXTMAX]={0};
    int     Zeichen = 0;
    DWORD   KanalHandle;

	ausgabe(LOG, DEBUGALLES, "LogMonatDatei gestartet!!!\n");

    GetMonat(DatumBuffer, sizeof(DatumBuffer)-1);
    GetZeitpunkt(ZeitpunktBuffer, sizeof(ZeitpunktBuffer)-1);

    sprintf(DateiBuffer, "%s/%s.%s", Pfad, DatumBuffer, OutputDatei);

	ausgabe(LOG, DEBUGALLES, "LogDatei %s erstellen\n", DateiBuffer);

    Datei = fopen(DateiBuffer, "r");
    ausgabe(LOG, DEBUGALLES, "Überprüfen ob LogDatei %s geöffnet werden kann!!!\n", DateiBuffer);
    if (Datei)                              // Wenn Datei geöffnet werden konnte dann wurde sie bereits erstellt
    {                                       // und es muss kein Dateikopf mehr erstellt werden.
        ausgabe(LOG, DEBUGALLES, "LogDatei %s konnte geöffnet werden!!!\n", DateiBuffer);
        NeueDatei = false;
        fclose(Datei);
    }

    Datei = fopen(DateiBuffer, "a");
    if (!Datei)
    {
        ausgabe(LOG, DEBUGALLES, "LogDatei %s konnte nicht geöffnet werden!!!\n", DateiBuffer);
        return;
    }
    else
    {	// Logdatei wurde zum Schreiben geöffnet
        ausgabe(LOG, DEBUGALLES, "Ausgabe in Datei %s wird gestartet.\n", DateiBuffer);
        setbuf(Datei, 0);                                                       // Pufferung abschalten

		// Erstellen des Dateikopfes wenn nötig
        if(NeueDatei)
        {
            fprintf(Datei, "LogMonatDatei erstellt am %s von %s %s\r\n", ZeitpunktBuffer, ProgrammName, ProgrammVersion);
            fprintf(Datei, "---------------------------------------------------------------------------------------------------------------------------\r\n");
            fprintf(Datei, "                   \t");

            for(i = 1; i <= LogMonatWRAnzahl; i++)
            {
                for(j = 1; j <= LogMonatKanalAnzahl; j++)
                {
                    KanalHandle = FindChannelName(WRHandles[0], LogMonatKanal[j - 1]);

                    if(KanalHandle == 0)
                    {           // Kanal nicht vom WR
                                // Die Errechneten Kanaele durchsuchen
                        for(k = 1; k <= ErrechneteKanaeleAnzahl; k++)
                        {
                            if(strncmp(LogMonatKanal[j - 1], ErrechneteKanaeleName[k - 1], sizeof(LogMonatKanal[j - 1])-1) == 0)
                            {
                                KanalHandle = k;
                                break;
                            }
                        }
                        if(KanalHandle == 0)
                        {           // Kanal nicht gefunden
                        	Zeichen = 15;
                        }
                        else
                        {           // Errechneter Kanal
                       		Zeichen = ErrechneteKanaeleZeichen[KanalHandle - 1];
                        }
                    }
                    else
                    {           // Kanal vom WR
                        Zeichen = KanalWertZeichenAnzahlMAX(KanalHandle);
                    }

                    if(j == 1)
                    {
                    	char *Temp[20] = {0};

                    	if(strncmp(LogMonatWR[i - 1], "9999999999", 10) == 0)
                    	{
                    		strncpy(Temp, LogMonatSummeUeberschrift, sizeof(Temp)-1);
                    	}
                    	else
                    	{
                    		strncpy(Temp, LogMonatWR[i - 1], sizeof(Temp)-1);
                    	}

                    	if(Zeichen < strlen(Temp))
                    	{
                    		Zeichen = strlen(Temp);
                    	}
                        fprintf(Datei, "%-*s\t", Zeichen, Temp);
                    }
                    else
                    {
                        fprintf(Datei, "%*s\t", Zeichen, "");
                    }
                }
            }

            fprintf(Datei, "\r\nZeitpunkt          \t");

            for(i = 1; i <= LogMonatWRAnzahl; i++)
            {
                for(j = 1; j <= LogMonatKanalAnzahl; j++)
                {
                    KanalHandle = FindChannelName(WRHandles[0], LogMonatKanal[j - 1]);

                    if(KanalHandle == 0)
                    {           // Kanal nicht vom WR
                                // Die Errechneten Kanaele durchsuchen
                        for(k = 1; k <= ErrechneteKanaeleAnzahl; k++)
                        {
                            if(strncmp(LogMonatKanal[j - 1], ErrechneteKanaeleName[k - 1], sizeof(LogMonatKanal[j - 1])-1) == 0)
                            {
                                KanalHandle = k;
                                break;
                            }
                        }
                        if(KanalHandle == 0)
                        {           // Kanal nicht gefunden
                            fprintf(Datei, "???%*s???\t", LogMonatDateiZeichenAnzahl(j, 15, i)-6, LogMonatKanal[j - 1]);
                        }
                        else
                        {           // Errechneter Kanal
                        	fprintf(Datei, "%*s\t", LogMonatDateiZeichenAnzahl(j, ErrechneteKanaeleZeichen[KanalHandle - 1], i), ErrechneteKanaeleName[KanalHandle - 1]);
                        }
                    }
                    else
                    {           // Kanal vom WR
                    	GetChannelName(KanalHandle, KanalName, sizeof(KanalName)-1);
                    	fprintf(Datei, "%*s\t", LogMonatDateiZeichenAnzahl(j, KanalWertZeichenAnzahlMAX(KanalHandle), i), KanalName);
                    }
                }
            }
		}

		// AB hier wird die Datei mit Daten gefüttert
        fprintf(Datei, "\r\n");
        fprintf(Datei, "%s\t", ZeitpunktBuffer);

        for(i = 1; i <= LogMonatWRAnzahl; i++)
        {
            for(j = 1; j <= LogMonatKanalAnzahl; j++)
            {
                KanalHandle = FindChannelName(WRHandles[0], LogMonatKanal[j - 1]);

                if(KanalHandle == 0)
                {           // Kanal nicht vom WR
                    if(GetWertCalcKanalSN(LogMonatKanal[j - 1], LogMonatWR[i - 1], WertText, sizeof(WertText)-1) == -1)
                    {		// Kanal nicht gefunden
                        fprintf(Datei, "%*s\t", LogMonatDateiZeichenAnzahl(j, 15, i), "");
                    }
                    else
                    {		// Kanal gefunden
                        fprintf(Datei, "%*s\t", LogMonatDateiZeichenAnzahl(j, GetZeichenCalcKanal(LogMonatKanal[j - 1]), i), WertText);
                    }
				}
                else
                {           // Kanal vom WR
					KanalAbfragenSN(KanalHandle, LogMonatWR[i - 1], WertText, sizeof(WertText)-1);

                    fprintf(Datei, "%*s\t", LogMonatDateiZeichenAnzahl(j, KanalWertZeichenAnzahlMAX(KanalHandle), i), WertText);
                }
            }
        }
		fclose(Datei);
	}
}



/**************************************************************************
   Description   : Loggen alle Momentanwerte aller WR in je eine Datei
   Parameter     : (none)
   Return-Value  : (none)
   Changes       : Author, 		Date, 		Version, 	Reason
                   ********************************************************
                   Dotterweich,	21.08.2007, 0.1, 		Created
**************************************************************************/
void LogGesamt(void)
{
	int		Zaehler = 1;
    char	Pfad[100]={0};
    char	OutputDatei[100]={0};

	if(	LogGesamtIntervallZeit == 0 ||													// Wenn Intervallzeit = 0 ODER
		(LogGesamtLetzteIntervallZeit + LogGesamtIntervallZeit) > SekundenZeit ||		// Wenn Intervallzeit nicht abgelaufen
		AnzahlErfassterWR == 0 ||														// ODER Wenn keine WR erfasst
		!(AnzahlSollWR == AnzahlErfassterWR) ||											// ODER Wenn nicht alle WR erfasst
		!ErrechneteKanaeleAktualisierenErfolgreich ||									// ODER Wenn ErrechneteKanaeleAktualisieren nicht erfolgreich
		(KanalAbfragenMoeglich() == -1))												// ODER Wenn WR Abfrage nicht möglich
	{																					// Dann LogGesamt Abbrechen
		return;
	}

	ausgabe(LOG, DEBUGALLES, "LogGesamt gestartet!!!\n");
	ausgabe(LOG, DEBUGALLES, "LogGesamtOutputTypeAnzahl: %i\n", LogGesamtOutputTypeAnzahl);
 	ausgabe(LOG, DEBUGALLES, "Sekunden: %u\n", SekundenZeit);

	LogGesamtLetzteIntervallZeit = SekundenZeit;										// Log Gesamt erst nach ablauf der nächsten Intervallzeit wieder starten

	for(Zaehler = 1; Zaehler <= LogGesamtOutputTypeAnzahl; Zaehler++)
	{
		ausgabe(LOG, DEBUGALLES, "Zaehler: %i\n", Zaehler);
		ausgabe(LOG, DEBUGALLES, "LogGesamtOutputType[Zaehler - 1]: %i\n", LogGesamtOutputType[Zaehler - 1]);
		switch(LogGesamtOutputType[Zaehler - 1])
		{
			case 1:																		// "Local"
                Tools_PathExtractPath(Pfad, LogGesamtOutput[Zaehler - 1], sizeof(LogGesamtOutput[Zaehler - 1])-1 );
                Tools_PathExtractFile(OutputDatei, LogGesamtOutput[Zaehler - 1], sizeof(LogGesamtOutput[Zaehler - 1])-1 );
				ausgabe(LOG, DEBUGALLES, "LogGesamt 'Local' gestartet!!!\n");
				LogGesamtDatei(Pfad, OutputDatei, LogGesamtOutputFormat[Zaehler - 1]);
				break;
			case 2:																		// "FTP"
                Tools_PathExtractPath(Pfad, LogGesamtOutput[Zaehler - 1], sizeof(LogGesamtOutput[Zaehler - 1])-1 );
                Tools_PathExtractFile(OutputDatei, LogGesamtOutput[Zaehler - 1], sizeof(LogGesamtOutput[Zaehler - 1])-1 );
				ausgabe(LOG, DEBUGALLES, "LogGesamt 'FTP' gestartet!!!\n");
				LogGesamtDatei(Pfad, OutputDatei, LogGesamtOutputFormat[Zaehler - 1]);
				break;
		}
	}
}



/**************************************************************************
   Description   : Loggen von Werten aller WR in je eine Datei
   Parameter     : (none)
   Return-Value  : (none)
   Changes       : Author, 		Date, 		Version, 	Reason
                   ********************************************************
                   Dotterweich,	01.09.2007, 0.1, 		Created
**************************************************************************/
void Log(void)
{
	int		Zaehler = 1;
    char	Pfad[100]={0};
    char	OutputDatei[100]={0};

	if(	LogIntervallZeit == 0 ||												// Wenn Intervallzeit = 0 ODER
		(LogLetzteIntervallZeit + LogIntervallZeit) > SekundenZeit ||			// Wenn Intervallzeit nicht abgelaufen
		AnzahlErfassterWR == 0 ||												// ODER Wenn keine WR erfasst
		!(AnzahlSollWR == AnzahlErfassterWR) ||									// ODER Wenn nicht alle WR erfasst
		!ErrechneteKanaeleAktualisierenErfolgreich ||							// ODER Wenn ErrechneteKanaeleAktualisieren nicht erfolgreich
		(KanalAbfragenMoeglich() == -1))										// ODER Wenn WR Abfrage nicht möglich
	{																			// Dann Log Abbrechen
		return;
	}

	ausgabe(LOG, DEBUGALLES, "Log gestartet!!!\n");
	ausgabe(LOG, DEBUGALLES, "LogOutputTypeAnzahl: %i\n", LogOutputTypeAnzahl);
 	ausgabe(LOG, DEBUGALLES, "Sekunden: %u\n", SekundenZeit);

	LogLetzteIntervallZeit = SekundenZeit;										// Log erst nach ablauf der nächsten Intervallzeit wieder starten

	for(Zaehler = 1; Zaehler <= LogOutputTypeAnzahl; Zaehler++)
	{
		ausgabe(LOG, DEBUGALLES, "Zaehler: %i\n", Zaehler);
		ausgabe(LOG, DEBUGALLES, "LogOutputType[Zaehler - 1]: %i\n", LogOutputType[Zaehler - 1]);
		switch(LogOutputType[Zaehler - 1])
		{
			case 1:																		// "Local"
                Tools_PathExtractPath(Pfad, LogOutput[Zaehler - 1], sizeof(LogOutput[Zaehler - 1])-1 );
                Tools_PathExtractFile(OutputDatei, LogOutput[Zaehler - 1], sizeof(LogOutput[Zaehler - 1])-1 );
				ausgabe(LOG, DEBUGALLES, "Log 'Local' gestartet!!!\n");
				LogDatei(Pfad, OutputDatei, LogGesamtOutputFormat[Zaehler - 1]);
				break;
			case 2:																		// "FTP"
                Tools_PathExtractPath(Pfad, LogOutput[Zaehler - 1], sizeof(LogOutput[Zaehler - 1])-1 );
                Tools_PathExtractFile(OutputDatei, LogOutput[Zaehler - 1], sizeof(LogOutput[Zaehler - 1])-1 );
				ausgabe(LOG, DEBUGALLES, "Log 'FTP' gestartet!!!\n");
				LogDatei(Pfad, OutputDatei, LogOutputFormat[Zaehler - 1]);
				break;
		}
	}
}



/**************************************************************************
   Description   : Loggen der letzten ermittelten Werte von ausgewählten
                   WR in eine Datei um 0:00
   Parameter     : (none)
   Return-Value  : (none)
   Changes       : Author, 		Date, 		Version, 	Reason
                   ********************************************************
                   Dotterweich,	01.09.2007, 0.1, 		Created
**************************************************************************/
void LogMonat(void)
{
	int		Zaehler = 1;
    char	Pfad[100]={0};
    char	OutputDatei[100]={0};

	if(	LogMonatLetzteIntervallZeit >= TagZeit ||					// Wenn Intervallzeit nicht abgelaufen
		AnzahlErfassterWR == 0 ||									// ODER Wenn keine WR erfasst
		!(AnzahlSollWR == AnzahlErfassterWR))						// ODER Wenn nicht alle WR erfasst
	{																// Dann LogMonat Abbrechen
		return;
	}

	ausgabe(LOG, DEBUGALLES, "LogMonat gestartet!!!\n");
	ausgabe(LOG, DEBUGALLES, "LogMonatOutputTypeAnzahl: %i\n", LogMonatOutputTypeAnzahl);
 	ausgabe(LOG, DEBUGALLES, "Sekunden: %u\n", SekundenZeit);

	LogMonatLetzteIntervallZeit = TagZeit;							// LogMonat erst um 0:00 wieder starten

	for(Zaehler = 1; Zaehler <= LogOutputTypeAnzahl; Zaehler++)
	{
		ausgabe(LOG, DEBUGALLES, "Zaehler: %i\n", Zaehler);
		ausgabe(LOG, DEBUGALLES, "LogMonatOutputType[Zaehler - 1]: %i\n", LogMonatOutputType[Zaehler - 1]);
		switch(LogMonatOutputType[Zaehler - 1])
		{
			case 1:																		// "Local"
                Tools_PathExtractPath(Pfad, LogMonatOutput[Zaehler - 1], sizeof(LogMonatOutput[Zaehler - 1])-1 );
                Tools_PathExtractFile(OutputDatei, LogMonatOutput[Zaehler - 1], sizeof(LogMonatOutput[Zaehler - 1])-1 );
				ausgabe(LOG, DEBUGALLES, "LogMonat 'Local' gestartet!!!\n");
				LogMonatDatei(Pfad, OutputDatei, LogMonatOutputFormat[Zaehler - 1]);
				break;
			case 2:																		// "FTP"
                Tools_PathExtractPath(Pfad, LogMonatOutput[Zaehler - 1], sizeof(LogMonatOutput[Zaehler - 1])-1 );
                Tools_PathExtractFile(OutputDatei, LogMonatOutput[Zaehler - 1], sizeof(LogMonatOutput[Zaehler - 1])-1 );
				ausgabe(LOG, DEBUGALLES, "LogMonat 'FTP' gestartet!!!\n");
				LogMonatDatei(Pfad, OutputDatei, LogMonatOutputFormat[Zaehler - 1]);
				break;
		}
	}
}



// Ende
