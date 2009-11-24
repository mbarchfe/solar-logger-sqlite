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
*  DateiName:	kanalrech.h
*  Status:		InArbeit
***************************************************************************/



// Globale Variable
extern int		ErrechneteKanaeleAnzahl;								// Anzahl der zu errechnenten Kanäle
extern char		ErrechneteKanaeleName[ERRECHNETEKANAELEMAX][20];		// Namen der errechneten Kanäle
extern int		ErrechneteKanaeleArt[ERRECHNETEKANAELEMAX];				// Berechnungs Arten
extern char		ErrechneteKanaeleKanal[ERRECHNETEKANAELEMAX][20];		// Namen der WR Kanäle die zur Berechnung nötig sind
extern int		ErrechneteKanaeleZeichen[ERRECHNETEKANAELEMAX];			// Anzahl der Stellen vor dem Komma
extern int		ErrechneteKanaeleGenauigkeit[ERRECHNETEKANAELEMAX];		// Anzahl der Stellen nach dem Komma
extern long		ErrechneteKanaeleErrechnungsIntervallZeit;				// Intervall zur Berechnung der Kanäle in Sekunden
extern long		ErrechneteKanaeleErrechnungsLetzteIntervallZeit;		// Zeitpunkt des letzten berechnens der Kanäle in Sekunden

extern double	ErrechneteKanaeleWertZahl[DEVMAX][ERRECHNETEKANAELEMAX];		// Wert der errechnenten Kanäle
extern char		ErrechneteKanaeleWert[DEVMAX][ERRECHNETEKANAELEMAX][20];		// Wert der errechnenten Kanäle
extern char		ErrechneteKanaeleEinheit[DEVMAX][ERRECHNETEKANAELEMAX][20];		// Einheit der errechnenten Kanäle
extern long		ErrechneteKanaeleLongTemp[DEVMAX][ERRECHNETEKANAELEMAX];		// long TEMP Speicher
extern double	ErrechneteKanaeleDoubleTemp[DEVMAX][ERRECHNETEKANAELEMAX];		// double TEMP Speicher

extern BOOL		ErrechneteKanaeleAktualisierenErfolgreich;   			// TRUE wenn ErrechneteKanaeleAktualisieren Erfolgreich



extern void ErrechneteKanaeleINI(void);
extern void ErrechneteKanaeleAktualisieren(void);

extern double 	GetWertCalcKanalSN(char *KanalName, char *SN, char *WertText, int WertTextMAX);
extern double 	GetWertCalcKanalSN(char *KanalName, char *SN, char *WertText, int WertTextMAX);
extern int 		GetZeichenCalcKanal(char *KanalName);



// Ende
