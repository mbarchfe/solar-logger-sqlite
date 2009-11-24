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
*  DateiName:	log.h
*  Status:		InArbeit
***************************************************************************/



#define TERM		1			// Ausgabe auf dem Terminal
#define SYSLOG		2			// Ausgabe im Syslog
#define LOGDATEI	4			// Ausgabe in Logdatei
#define LOG			6			// Ausgebe im Syslog und in Logdatei

#define DEBUGMINI	0			// Nur die wichtigsten Meldungen ausgeben
#define DEBUGNORMAL	1			// Zusätzlich alle Normalen Meldungen ausgeben
#define DEBUGINIT	10			// Zusätzlich alle Initialisierung Meldungen ausgeben
#define DEBUGFTP	20			// Zusätzlich alle FTP Meldungen ausgeben
#define DEBUGSMTP	30			// Zusätzlich alle SMTP Meldungen ausgeben
#define DEBUGSTATUS 50			// Zusätzlich alle Status Meldungen ausgeben
#define DEBUGLOG	60			// Zusätzlich alle Log Meldungen ausgeben
#define DEBUGKRECH	80			// Zusätzlich alle Meldungen für Errechnete Kanäle ausgeben
#define DEBUGWRABF	90			// Zusätzlich alle Meldungen beim WR Abfragen ausgeben
#define DEBUGALLES	99			// Alle Meldungen ausgeben



extern void ausgabe(int wo, int level, char *text, ...);



// Ende
