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
*  Funktionssammlung zum Senden von Datein per FTP
*
***************************************************************************
*  DateiName:	ftp.c
*  Status:		InArbeit
***************************************************************************/



#include "solar-logger.h"
#include "log.h"
#include "wrerfassen.h"

#include "ftplib/ftplib.h"



/**************************************************************************
   Description   : Senden von Dateien per FTP
   Parameter     : (none)
   Return-Value  : -1 wenn Fehler
   Changes       : Author, 		Date, 		Version, 	Reason
                   ********************************************************
                   Dotterweich,	31.08.2007, 0.1, 		Created
**************************************************************************/
int FTPDateienSenden(char *Pfad, char *Datei, char *HOST, char *User, char *PW)
{
	int		Zaehler = 1;
	DWORD	SNBuffer = 0;
    char	TypeBuffer[50] = {0};
	char	DateiBuffer[200] = {0};
	char	FTPDateiBuffer[200] = {0};
	netbuf	*conn = NULL;

	FtpInit();

    if (!FtpConnect(HOST, &conn))
    {
    	ausgabe(LOG, DEBUGMINI, "FTP Connect mit '%s' fehlgeschlagen!!!\n", HOST);
        return -1;
    }

    if (!FtpLogin(User, PW, conn))
    {
    	ausgabe(LOG, DEBUGMINI, "FTP Login mit USER '%s' fehlgeschlagen!!!\n", User);
        return -1;
    }

    FtpOptions(FTPLIB_CALLBACK, (long) NULL, conn);

    for(Zaehler = 1; Zaehler <= AnzahlErfassterWR; Zaehler++)
    {

        GetDeviceSN(WRHandles[Zaehler - 1], &SNBuffer);
        GetDeviceType(WRHandles[Zaehler - 1], TypeBuffer, sizeof(TypeBuffer)-1);

        sprintf(DateiBuffer, "%s/%3ld.%s.%s", Pfad, SNBuffer, TypeBuffer, Datei);
        sprintf(FTPDateiBuffer, "%3ld.%s.%s", SNBuffer, TypeBuffer, Datei);

		FtpPut(DateiBuffer, FTPDateiBuffer, FTPLIB_ASCII, conn);

    	ausgabe(LOG, DEBUGFTP, "FTP Datei '%s' gesendet!!!\n", FTPDateiBuffer);
    }

    if (conn)
    {
        FtpClose(conn);
    }
}



/**************************************************************************
   Description   : Senden einer Datei per FTP
   Parameter     : (none)
   Return-Value  : -1 wenn Fehler
   Changes       : Author, 		Date, 		Version, 	Reason
                   ********************************************************
                   Dotterweich,	01.09.2007, 0.1, 		Created
**************************************************************************/
int FTPDateiSenden(char *DateiLocal, char *DateiHost, char *HOST, char *User, char *PW)
{
	int		Zaehler = 1;
	DWORD	SNBuffer = 0;
    char	TypeBuffer[50] = {0};
	char	DateiBuffer[200] = {0};
	char	FTPDateiBuffer[200] = {0};
	netbuf	*conn = NULL;

	FtpInit();

    if (!FtpConnect(HOST, &conn))
    {
    	ausgabe(LOG, DEBUGMINI, "FTP Connect mit '%s' fehlgeschlagen!!!\n", HOST);
        return -1;
    }

    if (!FtpLogin(User, PW, conn))
    {
    	ausgabe(LOG, DEBUGMINI, "FTP Login mit USER '%s' fehlgeschlagen!!!\n", User);
        return -1;
    }

    FtpOptions(FTPLIB_CALLBACK, (long) NULL, conn);

    FtpPut(DateiLocal, DateiHost, FTPLIB_ASCII, conn);

    ausgabe(LOG, DEBUGFTP, "FTP Datei '%s' gesendet!!!\n", DateiHost);

    if (conn)
    {
        FtpClose(conn);
    }
}



// Ende
