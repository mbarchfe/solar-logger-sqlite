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
*  Ausgabe des Statuses der WR
*
***************************************************************************
*  DateiName:	solstatus.c
*  Status:		InArbeit
***************************************************************************/

#include "solar-logger.h"
#include "log.h"
#include "ini.h"
#include "kanalrech.h"
#include "timer.h"
#include "wrerfassen.h"
#include "wrabfragen.h"



// Globale Variable
int		StatusUebersichtGesamtOutputTypeAnzahl = 0;						// Anzahl der AusgabeArten
int 	StatusUebersichtGesamtOutputType[OUTPUTTYPEMAX]={0};			// Wie oder Wo sollen die Daten gespeichert oder �bertragen werden (Local, FTP, HTTP-Post)
char	StatusUebersichtGesamtOutput[OUTPUTTYPEMAX][100]={0};			// AusgabeDateiAnfang f�r die UebersichtGesamt Dateien
char	StatusUebersichtGesamtOutputServer[OUTPUTTYPEMAX][100]={0};		// Server f�r die UebersichtGesamt Dateien
char	StatusUebersichtGesamtOutputUser[OUTPUTTYPEMAX][100]={0};		// User f�r die UebersichtGesamt Dateien
char	StatusUebersichtGesamtOutputPW[OUTPUTTYPEMAX][100]={0};			// PW f�r die UebersichtGesamt Dateien
int		StatusUebersichtGesamtOutputFormat[OUTPUTTYPEMAX]={0};			// Format der AusgabeDatei f�r die UebersichtGesamt (txt, csv, ...)
long	StatusUebersichtGesamtIntervallZeit = 0;						// Intervall zum erstellen der StatusUebersichtGesamt Dateien in Sekunden
long	StatusUebersichtGesamtLetzteIntervallZeit = 0;					// Zeitpunkt des letzten StatusUebersichtGesamt

int		StatusUebersichtOutputTypeAnzahl = 0;							// Anzahl der AusgabeArten
int 	StatusUebersichtOutputType[OUTPUTTYPEMAX]={0};					// Wie oder Wo sollen die Daten gespeichert oder �bertragen werden (Local, FTP, HTTP-Post)
char	StatusUebersichtOutput[OUTPUTTYPEMAX][100]={0};					// AusgabeDateiAnfang f�r die UebersichtGesamt Dateien
char	StatusUebersichtOutputServer[OUTPUTTYPEMAX][100]={0};			// Server f�r die UebersichtGesamt Dateien
char	StatusUebersichtOutputUser[OUTPUTTYPEMAX][100]={0};				// User f�r die UebersichtGesamt Dateien
char	StatusUebersichtOutputPW[OUTPUTTYPEMAX][100]={0};				// PW f�r die UebersichtGesamt Dateien
int		StatusUebersichtOutputFormat[OUTPUTTYPEMAX]={0};				// Format der AusgabeDatei f�r die UebersichtGesamt (txt, csv, ...)
long	StatusUebersichtIntervallZeit = 0;								// Intervall zum erstellen der StatusUebersichtGesamt Dateien in Sekunden
long	StatusUebersichtLetzteIntervallZeit = 0;						// Zeitpunkt des letzten StatusUebersichtGesamt
int		StatusUebersichtKanalAnzahl = 0;								// Anzahl der Kan�le
char	StatusUebersichtKanal[KANALMAX][20]={0};						// Namen der Kan�le

int		StatusOutputTypeAnzahl = 0;										// Anzahl der AusgabeArten
int 	StatusOutputType[OUTPUTTYPEMAX]={0};							// Wie oder Wo sollen die Daten gespeichert oder �bertragen werden (Local, FTP, HTTP-Post)
char	StatusOutput[OUTPUTTYPEMAX][100]={0};							// AusgabeDateiAnfang f�r die UebersichtGesamt Dateien
char	StatusOutputServer[OUTPUTTYPEMAX][100]={0};						// Server f�r die UebersichtGesamt Dateien
char	StatusOutputUser[OUTPUTTYPEMAX][100]={0};						// User f�r die UebersichtGesamt Dateien
char	StatusOutputPW[OUTPUTTYPEMAX][100]={0};							// PW f�r die UebersichtGesamt Dateien
int		StatusOutputFormat[OUTPUTTYPEMAX]={0};							// Format der AusgabeDatei f�r die UebersichtGesamt (txt, csv, ...)
long	StatusIntervallZeit = 0;										// Intervall zum erstellen der StatusUebersichtGesamt Dateien in Sekunden
long	StatusLetzteIntervallZeit = 0;									// Zeitpunkt des letzten StatusUebersichtGesamt



/**************************************************************************
   Description   : Einlesen des Status Bereiches der INI Datei
   Parameter     : (none)
   Return-Value  : (none)
   Changes       : Author, 		Date, 		Version, 	Reason
                   ********************************************************
                   Dotterweich,	23.08.2007, 0.1, 		Created
**************************************************************************/
void StatusINI(void)
{
	int		Zaehler = 1;
	char	Temp[100] = {0};

	{
        StatusUebersichtGesamtOutputTypeAnzahl = InitAnzahl("Status.StatusUebersichtGesamtOutputTypeAnzahl", OUTPUTTYPEMAX);

        InitOutputType(StatusUebersichtGesamtOutputTypeAnzahl, StatusUebersichtGesamtOutputType, "Status.StatusUebersichtGesamtOutputType");

        for(Zaehler = 1; Zaehler <= StatusUebersichtGesamtOutputTypeAnzahl; Zaehler++)
        {
            sprintf(Temp, "Status.StatusUebersichtGesamtOutput_%i", Zaehler);
            TRepository_GetElementStr(  Temp,
                                        "",
                                        StatusUebersichtGesamtOutput[Zaehler - 1],
                                        sizeof(StatusUebersichtGesamtOutput[Zaehler - 1])-1);

            if(StatusUebersichtGesamtOutputType[Zaehler - 1] == 2)          // Wenn Output Type FTP
            {
                sprintf(Temp, "Status.StatusUebersichtGesamtOutputServer_%i", Zaehler);
                TRepository_GetElementStr(  Temp,
                                            "",
                                            StatusUebersichtGesamtOutputServer[Zaehler - 1],
                                            sizeof(StatusUebersichtGesamtOutputServer[Zaehler - 1])-1);

                sprintf(Temp, "Status.StatusUebersichtGesamtOutputUser_%i", Zaehler);
                TRepository_GetElementStr(  Temp,
                                            "",
                                            StatusUebersichtGesamtOutputUser[Zaehler - 1],
                                            sizeof(StatusUebersichtGesamtOutputUser[Zaehler - 1])-1);

                sprintf(Temp, "Status.StatusUebersichtGesamtOutputPW_%i", Zaehler);
                TRepository_GetElementStr(  Temp,
                                            "",
                                            StatusUebersichtGesamtOutputPW[Zaehler - 1],
                                            sizeof(StatusUebersichtGesamtOutputPW[Zaehler - 1])-1);
            }
        }

        InitOutputFormat(StatusUebersichtGesamtOutputTypeAnzahl, StatusUebersichtGesamtOutputFormat, "Status.StatusUebersichtGesamtOutputFormat");

        StatusUebersichtGesamtIntervallZeit = TRepository_GetElementInt("Status.StatusUebersichtGesamtIntervall", 0);
    }

	{
        StatusUebersichtOutputTypeAnzahl = InitAnzahl("Status.StatusUebersichtOutputTypeAnzahl", OUTPUTTYPEMAX);

        InitOutputType(StatusUebersichtOutputTypeAnzahl, StatusUebersichtOutputType, "Status.StatusUebersichtOutputType");

        for(Zaehler = 1; Zaehler <= StatusUebersichtOutputTypeAnzahl; Zaehler++)
        {
            sprintf(Temp, "Status.StatusUebersichtOutput_%i", Zaehler);
            TRepository_GetElementStr(  Temp,
                                        "",
                                        StatusUebersichtOutput[Zaehler - 1],
                                        sizeof(StatusUebersichtOutput[Zaehler - 1])-1);

            if(StatusUebersichtOutputType[Zaehler - 1] == 2)          // Wenn Output Type FTP
            {
                sprintf(Temp, "Status.StatusUebersichtOutputServer_%i", Zaehler);
                TRepository_GetElementStr(  Temp,
                                            "",
                                            StatusUebersichtOutputServer[Zaehler - 1],
                                            sizeof(StatusUebersichtOutputServer[Zaehler - 1])-1);

                sprintf(Temp, "Status.StatusUebersichtOutputUser_%i", Zaehler);
                TRepository_GetElementStr(  Temp,
                                            "",
                                            StatusUebersichtOutputUser[Zaehler - 1],
                                            sizeof(StatusUebersichtOutputUser[Zaehler - 1])-1);

                sprintf(Temp, "Status.StatusUebersichtOutputPW_%i", Zaehler);
                TRepository_GetElementStr(  Temp,
                                            "",
                                            StatusUebersichtOutputPW[Zaehler - 1],
                                            sizeof(StatusUebersichtOutputPW[Zaehler - 1])-1);
            }
        }

        InitOutputFormat(StatusUebersichtOutputTypeAnzahl, StatusUebersichtOutputFormat, "Status.StatusUebersichtOutputFormat");

        StatusUebersichtIntervallZeit = TRepository_GetElementInt("Status.StatusUebersichtIntervall", 0);

        StatusUebersichtKanalAnzahl = InitAnzahl("Status.StatusUebersichtKanalAnzahl", KANALMAX);

        for(Zaehler = 1; Zaehler <= StatusUebersichtKanalAnzahl; Zaehler++)
        {
            sprintf(Temp, "Status.StatusUebersichtKanal_%i", Zaehler);
            TRepository_GetElementStr(  Temp,
                                        "",
                                        StatusUebersichtKanal[Zaehler - 1],
                                        sizeof(StatusUebersichtKanal[Zaehler - 1])-1);
        }
    }

	{
        StatusOutputTypeAnzahl = InitAnzahl("Status.StatusOutputTypeAnzahl", OUTPUTTYPEMAX);

        InitOutputType(StatusOutputTypeAnzahl, StatusOutputType, "Status.StatusOutputType");

        for(Zaehler = 1; Zaehler <= StatusOutputTypeAnzahl; Zaehler++)
        {
            sprintf(Temp, "Status.StatusOutput_%i", Zaehler);
            TRepository_GetElementStr(  Temp,
                                        "",
                                        StatusOutput[Zaehler - 1],
                                        sizeof(StatusOutput[Zaehler - 1])-1);

            if(StatusOutputType[Zaehler - 1] == 2)          // Wenn Output Type FTP
            {
                sprintf(Temp, "Status.StatusOutputServer_%i", Zaehler);
                TRepository_GetElementStr(  Temp,
                                            "",
                                            StatusOutputServer[Zaehler - 1],
                                            sizeof(StatusOutputServer[Zaehler - 1])-1);

                sprintf(Temp, "Status.StatusOutputUser_%i", Zaehler);
                TRepository_GetElementStr(  Temp,
                                            "",
                                            StatusOutputUser[Zaehler - 1],
                                            sizeof(StatusOutputUser[Zaehler - 1])-1);

                sprintf(Temp, "Status.StatusOutputPW_%i", Zaehler);
                TRepository_GetElementStr(  Temp,
                                            "",
                                            StatusOutputPW[Zaehler - 1],
                                            sizeof(StatusOutputPW[Zaehler - 1])-1);
            }
        }

        InitOutputFormat(StatusOutputTypeAnzahl, StatusOutputFormat, "Status.StatusOutputFormat");

        StatusIntervallZeit = TRepository_GetElementInt("Status.StatusIntervall", 0);
    }
}



/**************************************************************************
   Description   : Erstellen einer StatusDatei je WR mit allen Kan�len
   Parameter     : (none)
   Return-Value  : (none)
   Changes       : Author, 		Date, 		Version, 	Reason
                   ********************************************************
                   Dotterweich,	24.08.2007, 0.1, 		Created
**************************************************************************/
void StatusUebersichtGesamtDatei(char *Pfad, char *OutputDatei, int Format)
{
	int		Zaehler = 1;

	ausgabe(LOG, DEBUGALLES, "StatusUebersichtGesamtDatei gestartet!!!\n");

    for(Zaehler = 1; Zaehler <= AnzahlErfassterWR; Zaehler++)
    {
        BOOL	NeueDatei = true;
        char	NameBuffer[50] = {0};
    	char	TypeBuffer[50] = {0};
		DWORD	SNBuffer = 0;
    	char	DateiBuffer[200] = {0};
		FILE	*Datei = NULL;

        GetDeviceName(WRHandles[Zaehler - 1], NameBuffer, sizeof(NameBuffer)-1);
        GetDeviceType(WRHandles[Zaehler - 1], TypeBuffer, sizeof(TypeBuffer)-1);
        GetDeviceSN(WRHandles[Zaehler - 1], &SNBuffer);

        ausgabe(LOG, DEBUGALLES, "StatusUebersichtGesamtDatei f�r WR %3ld erstellen\n", SNBuffer);

        sprintf(DateiBuffer, "%s/%3ld.%s.%s", Pfad, SNBuffer, TypeBuffer, OutputDatei);

        Datei = fopen(DateiBuffer, "w");
        if (!Datei)
        {
            ausgabe(LOG, DEBUGMINI, "StatusDatei %s konnte nicht ge�ffnet werden!!!\n", DateiBuffer);
            return;
        }
        else
        {
        	int			i = 0;
        	int			KanalAnzahl = 0;
            DWORD		KanalHandle[KANALMAX];
            TChanType 	KanalType = ALLCHANNELS;									// SPOTCHANNELS oder ALLCHANNELS
            char		KanalName[50] = {0};
            char		KanalEinheit[50] = {0};
            char		WertText[50]={0};
			char		ZeitpunktBuffer[50] = {0};

            ausgabe(LOG, DEBUGALLES, "Ausgabe in Datei %s wird gestartet.\n", DateiBuffer);
            setbuf(Datei, 0);                      									// Pufferung abschalten

            // Ab hier Ausgaben in Datei
            KanalAnzahl = GetChannelHandlesEx(WRHandles[Zaehler - 1], KanalHandle, KANALMAX, KanalType);
            GetZeitpunkt(ZeitpunktBuffer, sizeof(ZeitpunktBuffer)-1);

            fprintf(Datei, "WR '%s' hat %d Kan�le:\r\n", NameBuffer, KanalAnzahl);

            fprintf(Datei, "--------------------------------------------------------------------\r\n");
            fprintf(Datei, " Kanal Handle  |     KanalName      |             Wert             |\r\n");
            fprintf(Datei, "--------------------------------------------------------------------\r\n");

			// Kan�le der WR
            for(i = 0; i < KanalAnzahl; i++)
            {
            	GetChannelName(KanalHandle[i], KanalName, sizeof(KanalName)-1);
                GetChannelUnit(KanalHandle[i], KanalEinheit, sizeof(KanalEinheit)-1);

            	KanalAbfragen(KanalHandle[i], WRHandles[Zaehler - 1], WertText, sizeof(WertText)-1);		// Abfragen eines Kanales

                fprintf(Datei, "     %3ld       | '%16s' | '%15s' %c%-8s%c |\r\n", KanalHandle[i], KanalName,
                        WertText,
                        strlen(KanalEinheit) > 0 ? '(' : ' ',
                        KanalEinheit,
                        strlen(KanalEinheit) > 0 ? ')' : ' ');
            }

            fprintf(Datei, "--------------------------------------------------------------------\r\n");

			// Die Errechneten Kanaele
            for(i = 0; i < ErrechneteKanaeleAnzahl; i++)
            {
                fprintf(Datei, "     %3ld       | '%16s' | '%15s' %c%-8s%c |\r\n", 0,
                		ErrechneteKanaeleName[i],
                        ErrechneteKanaeleWert[Zaehler - 1][i],
                        strlen(ErrechneteKanaeleEinheit[Zaehler - 1][i]) > 0 ? '(' : ' ',
                        ErrechneteKanaeleEinheit[Zaehler - 1][i],
                        strlen(ErrechneteKanaeleEinheit[Zaehler - 1][i]) > 0 ? ')' : ' ');
            }

            fprintf(Datei, "--------------------------------------------------------------------\r\n");
            GetZeitpunkt(ZeitpunktBuffer, sizeof(ZeitpunktBuffer)-1);
            fprintf(Datei, "erstellt am %s von %s %s\r\n\r\n", ZeitpunktBuffer, ProgrammName, ProgrammVersion);
            ausgabe(LOG, DEBUGALLES, "Ausgabe in Dateien ist beendet.\n");
		}
		fclose(Datei);
	}
}



/**************************************************************************
   Description   : Erstellen einer StatusDatei je WR
   Parameter     : (none)
   Return-Value  : (none)
   Changes       : Author, 		Date, 		Version, 	Reason
                   ********************************************************
                   Dotterweich,	31.08.2007, 0.1, 		Created
**************************************************************************/
void StatusUebersichtDatei(char *Pfad, char *OutputDatei, int Format)
{
	int		Zaehler = 1;

	ausgabe(LOG, DEBUGALLES, "StatusUebersichtDatei gestartet!!!\n");

    for(Zaehler = 1; Zaehler <= AnzahlErfassterWR; Zaehler++)
    {
        BOOL	NeueDatei = true;
        char	NameBuffer[50] = {0};
    	char	TypeBuffer[50] = {0};
		DWORD	SNBuffer = 0;
    	char	DateiBuffer[200] = {0};
		FILE	*Datei = NULL;

        GetDeviceName(WRHandles[Zaehler - 1], NameBuffer, sizeof(NameBuffer)-1);
        GetDeviceType(WRHandles[Zaehler - 1], TypeBuffer, sizeof(TypeBuffer)-1);
        GetDeviceSN(WRHandles[Zaehler - 1], &SNBuffer);

        ausgabe(LOG, DEBUGALLES, "StatusUebersichtDatei f�r WR %3ld erstellen\n", SNBuffer);

        sprintf(DateiBuffer, "%s/%3ld.%s.%s", Pfad, SNBuffer, TypeBuffer, OutputDatei);

        Datei = fopen(DateiBuffer, "w");
        if (!Datei)
        {
            ausgabe(LOG, DEBUGMINI, "StatusDatei %s konnte nicht ge�ffnet werden!!!\n", DateiBuffer);
            return;
        }
        else
        {
        	int			i = 0;
        	int			j = 0;
            DWORD		KanalHandle;
            char		KanalName[50] = {0};
            char		KanalEinheit[50] = {0};
            char		WertText[50]={0};
			char		ZeitpunktBuffer[50] = {0};

            ausgabe(LOG, DEBUGALLES, "Ausgabe in Datei %s wird gestartet.\n", DateiBuffer);
            setbuf(Datei, 0);                      									// Pufferung abschalten

            fprintf(Datei, "WR '%s' Status �bersicht\r\n", NameBuffer);

            fprintf(Datei, "--------------------------------------------------------------------\r\n");
            fprintf(Datei, " Kanal Handle  |     KanalName      |             Wert             |\r\n");
            fprintf(Datei, "--------------------------------------------------------------------\r\n");

            for(i = 1; i <= StatusUebersichtKanalAnzahl; i++)
            {
                KanalHandle = FindChannelName(WRHandles[Zaehler - 1], StatusUebersichtKanal[i - 1]);

                if(KanalHandle == 0)
                {			// Kanal nicht vom WR
                    		// Die Errechneten Kanaele durchsuchen
                    for(j = 1; j <= ErrechneteKanaeleAnzahl; j++)
                    {
                    	if(strncmp(StatusUebersichtKanal[i - 1], ErrechneteKanaeleName[j - 1], sizeof(StatusUebersichtKanal[i - 1])-1) == 0)
						{
							KanalHandle = j;
							break;
						}
                    }
                    if(KanalHandle == 0)
                    {           // Kanal nicht gefunden
                        fprintf(Datei, "     %3ld       | '%16s' | '%15s' %c%-8s%c |\r\n", 0,
                                StatusUebersichtKanal[i - 1],
                                "Kanal Fehler",
                                ' ',
                                "",
                                ' ');
					}
					else
					{			// Errechneter Kanal
                        fprintf(Datei, "     %3ld       | '%16s' | '%15s' %c%-8s%c |\r\n", 0,
                                ErrechneteKanaeleName[KanalHandle - 1],
                                ErrechneteKanaeleWert[Zaehler - 1][KanalHandle - 1],
                                strlen(ErrechneteKanaeleEinheit[Zaehler - 1][KanalHandle - 1]) > 0 ? '(' : ' ',
                                ErrechneteKanaeleEinheit[Zaehler - 1][KanalHandle - 1],
                                strlen(ErrechneteKanaeleEinheit[Zaehler - 1][KanalHandle - 1]) > 0 ? ')' : ' ');
                    }
                }
                else
                {			// Kanal vom WR
                    GetChannelName(KanalHandle, KanalName, sizeof(KanalName)-1);
                    GetChannelUnit(KanalHandle, KanalEinheit, sizeof(KanalEinheit)-1);

                    KanalAbfragen(KanalHandle, WRHandles[Zaehler - 1], WertText, sizeof(WertText)-1);        // Abfragen eines Kanales

                    fprintf(Datei, "     %3ld       | '%16s' | '%15s' %c%-8s%c |\r\n", KanalHandle, KanalName,
                            WertText,
                            strlen(KanalEinheit) > 0 ? '(' : ' ',
                            KanalEinheit,
                            strlen(KanalEinheit) > 0 ? ')' : ' ');
				}
            }

            fprintf(Datei, "--------------------------------------------------------------------\r\n");

            GetZeitpunkt(ZeitpunktBuffer, sizeof(ZeitpunktBuffer)-1);
            fprintf(Datei, "erstellt am %s von %s %s\r\n\r\n", ZeitpunktBuffer, ProgrammName, ProgrammVersion);
            ausgabe(LOG, DEBUGALLES, "Ausgabe in Dateien ist beendet.\n");
		}
		fclose(Datei);
	}
}



/**************************************************************************
   Description   : Erstellen der StatusUebersichtGesamt Dateien
   Parameter     : (none)
   Return-Value  : (none)
   Changes       : Author, 		Date, 		Version, 	Reason
                   ********************************************************
                   Dotterweich,	23.08.2007, 0.1, 		Created
**************************************************************************/
void StatusUebersichtGesamt(void)
{
	int		Zaehler = 1;
	int		Ergebnis = 0;

    char	Pfad[100]={0};
    char	OutputDatei[100]={0};

	// Alle Momentanwerte Loggen ???
	if(	StatusUebersichtGesamtIntervallZeit == 0 ||													// Wenn Intervallzeit = 0 ODER
		(StatusUebersichtGesamtLetzteIntervallZeit + StatusUebersichtGesamtIntervallZeit) > SekundenZeit ||		// Wenn Intervallzeit nicht abgelaufen
		AnzahlErfassterWR == 0 ||																	// ODER Wenn keine WR erfasst
		!(AnzahlSollWR == AnzahlErfassterWR) ||														// ODER Wenn nicht alle WR erfasst
		!ErrechneteKanaeleAktualisierenErfolgreich ||												// ODER Wenn ErrechneteKanaeleAktualisieren nicht erfolgreich
		(KanalAbfragenMoeglich() == -1))															// ODER Wenn WR Abfrage nicht m�glich
	{																								// Dann Status�bersichtGesamt Abbrechen
		return;
	}

	ausgabe(LOG, DEBUGALLES, "StatusUebersichtGesamt gestartet!!!\n");
 	ausgabe(LOG, DEBUGALLES, "Sekunden: %u\n", SekundenZeit);

	StatusUebersichtGesamtLetzteIntervallZeit = SekundenZeit;					// Status�bersichtGesamt erst nach ablauf der n�chsten Intervallzeit wieder starten

	for(Zaehler = 1; Zaehler <= StatusUebersichtGesamtOutputTypeAnzahl; Zaehler++)
	{
		ausgabe(LOG, DEBUGALLES, "Zaehler: %i\n", Zaehler);
		ausgabe(LOG, DEBUGALLES, "StatusUebersichtGesamtOutputType[Zaehler]: %i\n", StatusUebersichtGesamtOutputType[Zaehler - 1]);
		switch( StatusUebersichtGesamtOutputType[Zaehler - 1])
		{
			case 1:																		// "Local"
                Tools_PathExtractPath(Pfad, StatusUebersichtGesamtOutput[Zaehler - 1], sizeof(StatusUebersichtGesamtOutput[Zaehler - 1])-1 );
                Tools_PathExtractFile(OutputDatei, StatusUebersichtGesamtOutput[Zaehler - 1], sizeof(StatusUebersichtGesamtOutput[Zaehler - 1])-1 );
				ausgabe(LOG, DEBUGALLES, "StatusUebersichtGesamt 'Local' gestartet!!!\n");
				StatusUebersichtGesamtDatei(Pfad, OutputDatei, StatusUebersichtGesamtOutputFormat[Zaehler - 1]);
				break;
			case 2:																		// "FTP"
                Tools_PathExtractPath(Pfad, StatusUebersichtGesamtOutput[Zaehler - 1], sizeof(StatusUebersichtGesamtOutput[Zaehler - 1])-1 );
                Tools_PathExtractFile(OutputDatei, StatusUebersichtGesamtOutput[Zaehler - 1], sizeof(StatusUebersichtGesamtOutput[Zaehler - 1])-1 );
				ausgabe(LOG, DEBUGALLES, "StatusUebersichtGesamt 'FTP' gestartet!!!\n");
				StatusUebersichtGesamtDatei(Pfad, OutputDatei, StatusUebersichtGesamtOutputFormat[Zaehler - 1]);
//				Ergebnis = FTPDateienSenden(Pfad, OutputDatei,
//											StatusUebersichtGesamtOutputServer[Zaehler - 1],
//											StatusUebersichtGesamtOutputUser[Zaehler - 1],
//											StatusUebersichtGesamtOutputPW[Zaehler - 1]);
				break;
		}
	}
}



/**************************************************************************
   Description   : Erstellen der StatusUebersicht Dateien
   Parameter     : (none)
   Return-Value  : (none)
   Changes       : Author, 		Date, 		Version, 	Reason
                   ********************************************************
                   Dotterweich,	31.08.2007, 0.1, 		Created
**************************************************************************/
void StatusUebersicht(void)
{
	int		Zaehler = 1;
	int		Ergebnis = 0;

    char	Pfad[100]={0};
    char	OutputDatei[100]={0};

	// Alle Momentanwerte Loggen ???
	if(	StatusUebersichtIntervallZeit == 0 ||														// Wenn Intervallzeit = 0 ODER
		(StatusUebersichtLetzteIntervallZeit + StatusUebersichtIntervallZeit) > SekundenZeit ||		// Wenn Intervallzeit nicht abgelaufen
		AnzahlErfassterWR == 0 ||																	// ODER Wenn keine WR erfasst
		!(AnzahlSollWR == AnzahlErfassterWR) ||														// ODER Wenn nicht alle WR erfasst
		!ErrechneteKanaeleAktualisierenErfolgreich ||												// ODER Wenn ErrechneteKanaeleAktualisieren nicht erfolgreich
		(KanalAbfragenMoeglich() == -1))															// ODER Wenn WR Abfrage nicht m�glich
	{																								// Dann Status�bersicht Abbrechen
		return;
	}

	ausgabe(LOG, DEBUGALLES, "StatusUebersicht gestartet!!!\n");
 	ausgabe(LOG, DEBUGALLES, "Sekunden: %u\n", SekundenZeit);

	StatusUebersichtLetzteIntervallZeit = SekundenZeit;							// StatusUebrsicht erst nach ablauf der n�chsten Intervallzeit wieder starten

	for(Zaehler = 1; Zaehler <= StatusUebersichtOutputTypeAnzahl; Zaehler++)
	{
		ausgabe(LOG, DEBUGALLES, "Zaehler: %i\n", Zaehler);
		ausgabe(LOG, DEBUGALLES, "StatusUebersichtOutputType[Zaehler]: %i\n", StatusUebersichtOutputType[Zaehler - 1]);
		switch( StatusUebersichtOutputType[Zaehler - 1])
		{
			case 1:																		// "Local"
                Tools_PathExtractPath(Pfad, StatusUebersichtOutput[Zaehler - 1], sizeof(StatusUebersichtOutput[Zaehler - 1])-1 );
                Tools_PathExtractFile(OutputDatei, StatusUebersichtOutput[Zaehler - 1], sizeof(StatusUebersichtOutput[Zaehler - 1])-1 );
				ausgabe(LOG, DEBUGALLES, "StatusUebersicht 'Local' gestartet!!!\n");
				StatusUebersichtDatei(Pfad, OutputDatei, StatusUebersichtOutputFormat[Zaehler - 1]);
				break;
			case 2:																		// "FTP"
                Tools_PathExtractPath(Pfad, StatusUebersichtOutput[Zaehler - 1], sizeof(StatusUebersichtOutput[Zaehler - 1])-1 );
                Tools_PathExtractFile(OutputDatei, StatusUebersichtOutput[Zaehler - 1], sizeof(StatusUebersichtOutput[Zaehler - 1])-1 );
				ausgabe(LOG, DEBUGALLES, "StatusUebersicht 'FTP' gestartet!!!\n");
				StatusUebersichtDatei(Pfad, OutputDatei, StatusUebersichtOutputFormat[Zaehler - 1]);
//				Ergebnis = FTPDateienSenden(Pfad, OutputDatei,
//											StatusUebersichtOutputServer[Zaehler - 1],
//											StatusUebersichtOutputUser[Zaehler - 1],
//											StatusUebersichtOutputPW[Zaehler - 1]);
				break;
		}
	}
}



/**************************************************************************
   Description   : Erstellen der Status Datei
   Parameter     : (none)
   Return-Value  : (none)
   Changes       : Author, 		Date, 		Version, 	Reason
                   ********************************************************
                   Dotterweich,	01.09.2007, 0.1, 		Created
**************************************************************************/
void StatusDatei(DWORD SN, char *Type, char *Name, char *WertText)
{
	int		Zaehler = 1;

    char	Pfad[100]={0};
    char	OutputDatei[100]={0};
    char	DateiBuffer[200] = {0};
    char	DateiBufferFTP[200] = {0};
 	char	ZeitpunktBuffer[50] = {0};

    FILE	*Datei = NULL;

	ausgabe(LOG, DEBUGALLES, "StatusDatei gestartet!!!\n");
 	ausgabe(LOG, DEBUGALLES, "Sekunden: %u\n", SekundenZeit);

	for(Zaehler = 1; Zaehler <= StatusOutputTypeAnzahl; Zaehler++)
	{
		ausgabe(LOG, DEBUGALLES, "Zaehler: %i\n", Zaehler);
		ausgabe(LOG, DEBUGALLES, "StatusOutputType[Zaehler]: %i\n", StatusOutputType[Zaehler - 1]);
		switch(StatusOutputType[Zaehler - 1])
		{
			case 1:																		// "Local"
                Tools_PathExtractPath(Pfad, StatusOutput[Zaehler - 1], sizeof(StatusOutput[Zaehler - 1])-1 );
                Tools_PathExtractFile(OutputDatei, StatusOutput[Zaehler - 1], sizeof(StatusOutput[Zaehler - 1])-1 );
				ausgabe(LOG, DEBUGALLES, "Status 'Local' gestartet!!!\n");
				break;
			case 2:																		// "FTP"
                Tools_PathExtractPath(Pfad, StatusOutput[Zaehler - 1], sizeof(StatusOutput[Zaehler - 1])-1 );
                Tools_PathExtractFile(OutputDatei, StatusOutput[Zaehler - 1], sizeof(StatusOutput[Zaehler - 1])-1 );
				ausgabe(LOG, DEBUGALLES, "Status 'FTP' gestartet!!!\n");
				break;
		}

		ausgabe(LOG, DEBUGALLES, "Dateinamen bauen\n");

    	sprintf(DateiBuffer, "%s/%3ld.%s.%s.%s", Pfad, SN, Type, Name, OutputDatei);
    	sprintf(DateiBufferFTP, "%3ld.%s.%s.%s", SN, Type, Name, OutputDatei);

		ausgabe(LOG, DEBUGALLES, "DateiBuffer \t%s\n", DateiBuffer);
		ausgabe(LOG, DEBUGALLES, "DateiBufferFTP \t%s\n", DateiBufferFTP);

        Datei = fopen(DateiBuffer, "w");
        if (!Datei)
        {
            ausgabe(LOG, DEBUGMINI, "Datei %s konnte nicht ge�ffnet werden!!!\n", DateiBuffer);
        }
        else
        {
        	char *Temp = WertText;

            ausgabe(LOG, DEBUGALLES, "Datei %s wurde ge�ffnet!!!\n", DateiBuffer);
            GetZeitpunkt(ZeitpunktBuffer, sizeof(ZeitpunktBuffer)-1);

            while(*Temp==' ')
            {
            	Temp++;
            }

            fprintf(Datei, "%s\r\n", Temp);
            fprintf(Datei, "%s\r\n", ZeitpunktBuffer);
            fclose(Datei);

            switch(StatusOutputType[Zaehler - 1])
            {
                case 2:                                                                     // "FTP"
                    ausgabe(LOG, DEBUGALLES, "Status FTP �bertragung starten!!!\n");
//					FTPDateiSenden(DateiBuffer, DateiBufferFTP,
//								   StatusOutputServer[Zaehler - 1],
//                                   StatusOutputUser[Zaehler - 1],
//                                   StatusOutputPW[Zaehler - 1]);
                    break;
            }
        }
	}
}



/**************************************************************************
   Description   : Erstellen der Status Dateien der WR Kan�le
   Parameter     : (none)
   Return-Value  : (none)
   Changes       : Author, 		Date, 		Version, 	Reason
                   ********************************************************
                   Dotterweich,	01.09.2007, 0.1, 		Created
**************************************************************************/
void Status(DWORD KanalHandle, DWORD WRHandle, char *WertText)
{
    DWORD   SNBuffer = 0;
    char    TypeBuffer[50] = {0};
	char	KanalName[50] = {0};

	ausgabe(LOG, DEBUGALLES, "Status gestartet!!!\n");
 	ausgabe(LOG, DEBUGALLES, "Sekunden: %u\n", SekundenZeit);

    GetDeviceSN(WRHandle, &SNBuffer);
	ausgabe(LOG, DEBUGALLES, "SN \t%3ld\n", SNBuffer);

    GetDeviceType(WRHandle, TypeBuffer, sizeof(TypeBuffer)-1);
	ausgabe(LOG, DEBUGALLES, "Type \t%s\n", TypeBuffer);

    GetChannelName(KanalHandle, KanalName, sizeof(KanalName)-1);
	ausgabe(LOG, DEBUGALLES, "Name \t%s\n", KanalName);

	StatusDatei(SNBuffer, TypeBuffer, KanalName, WertText);
}



void StatusMail(void);



// Ende
