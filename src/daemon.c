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
*  Funktionssammlung um ein Programm zu einem Daemon zu machen
*
***************************************************************************
*  DateiName:	daemon.c
*  Status:		InArbeit
***************************************************************************/

#include "solar-logger.h"
#include "daemon.h"
#include "log.h"



#define THEONLYFILE "/tmp/solar-logger"



// Globale Variable
static int fdtheonly;					// filedescriptor zur �berpr�fung ob das der einzige Daemon seiner Art ist



/**************************************************************************
   Description   : Das Programm zu einem Daemon machen
   Parameter     : (none)
   Return-Value  : (none)
   Changes       : Author, 		Date, 		Version, 	Reason
                   ********************************************************
                   Dotterweich,	20.08.2007, 0.1, 		Created
**************************************************************************/
void start_daemon(const char *log_name, int facility)
{
    int i;
    pid_t pid;

    if ((pid = fork ()) != 0)
    {									// Hier Code f�r Elternprozess
        exit (EXIT_FAILURE);			// Elternprozess beenden und somit ist der Kindprozess ein Waise dessen sich jetzt init annimmt
    }

    if (setsid() < 0) 					// Erzeugen einer neuen Sitzung wenn der Aufrufende Prozess nicht Prozessgruppenf�hrer ist.
    {									// Wenn dies fehlschl�gt, kann es daran liegen, dass der Prozess bereits Sessionf�hrer ist.
    	ausgabe(LOG | TERM, DEBUGMINI, "%s kann nicht Session-F�hrer werden!\n", log_name);
        exit (EXIT_FAILURE);
    }

    // Da ein D�mon kein Terminal zur Verf�gung stehen hat, sollten folgende Signale ingnoriert werden.
    signal(SIGHUP, SIG_IGN);			// Signal SIGHUP ignorieren
    signal(SIGINT, SIG_IGN);			// Signal SIGINT ignorieren
    signal(SIGWINCH, SIG_IGN);			// Signal SIGWINCH ignorieren

    // Mit einem erneuten fork()-Aufruf gehen Sie sicher, dass der Elternprozess der erste Kindprozess ist, der beendet wird.
    // Dadurch sorgt man daf�r, dass ein D�mon nicht automatisch ein Steuerterminal erh�lt, falls dieser einen �ffnen w�rde.
    if ((pid = fork ()) != 0)
    {									// Hier Code f�r Elternprozess
        exit (EXIT_FAILURE);			// Elternprozess beenden und somit ist der Kindprozess ein Waise dessen sich jetzt init annimmt
    }

    // Gr�nde f�r das Arbeitsverzeichnis:	+ core-Datei wird im aktuellen Arbeitsverzeichnis hinterlegt.
    //										+ Damit bei Beendigung mit umount das Dateisystem sicher abgeh�ngt werde kann.
//    chdir ("/");

    umask (0);							// Damit wir nicht die Bitmaske vom Elternprozess erben bzw. diese bleiben, stellen wir diese auf 0

    for (i = sysconf (_SC_OPEN_MAX); i > 0; i--)		// Wir schlie�en alle ge�ffneten Filedeskriptoren....
    {
        close (i);
    }

    // Da unser D�monprozess selbst kein Terminal f�r die Ausgabe hat....
    openlog (log_name, LOG_PID | LOG_CONS| LOG_NDELAY, facility);

    ausgabe(LOG, DEBUGMINI, "Daemon gestartet.\n");
}



/**************************************************************************
   Description   : Den Daemon aufr�umen
   Parameter     : (none)
   Return-Value  : (none)
   Changes       : Author, 		Date, 		Version, 	Reason
                   ********************************************************
                   Dotterweich,	20.08.2007, 0.1, 		Created
**************************************************************************/
void stop_daemon(void)
{
    ausgabe(LOG, DEBUGMINI, "Daemon hat sich beendent.\n");
    closelog();
}



/**************************************************************************
   Description   : Das Programm zu einem Daemon machen
   Parameter     : (none)
   Return-Value  : gibt -1 zur�ck wenn dieser daemon noch nicht gestartet
   Changes       : Author, 		Date, 		Version, 	Reason
                   ********************************************************
                   Dotterweich,	23.08.2007, 0.1, 		Created
**************************************************************************/
int theonly_daemon(void)
{
  	struct flock lock;

    ausgabe(LOG, DEBUGMINI, "�berpr�fung ob Daemon bereits gestartet.\n");

	fdtheonly = open(THEONLYFILE, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if(fdtheonly == -1)
    {
    	ausgabe(LOG, DEBUGMINI, "THEONLYFILE konnte nicht ge�ffnet werden.\n");
    	return 0;
    }

    memset(&lock, 0, sizeof(struct flock));
    fcntl(fdtheonly, F_GETLK, &lock);

	switch(lock.l_type)
    {
    case F_UNLCK:
        memset(&lock, 0, sizeof(struct flock));
        lock.l_type   = F_WRLCK;  						// Schreibsperre
        lock.l_start  = 0;        						// kein offset
        lock.l_whence = SEEK_SET; 						// vom Anfang der Datei
        lock.l_len    = 0;        						// bis zum Ende der Datei
        lock.l_pid    = getpid(); 						// Prozessnummer der Sperre
        if(fcntl(fdtheonly, F_SETLK, &lock) < 0)
        {
            ausgabe(LOG, DEBUGMINI, "Fehler beim sperren von THEONLYFILE.");
            return 0;
        }
        else
        {
            ausgabe(LOG, DEBUGMINI, "Dies ist der einzige Daemon.\n");
            return -1;
        }
    case F_RDLCK:
    case F_WRLCK:
        ausgabe(LOG, DEBUGMINI, "Dieser Daemon l�uft bereits!!! (PID: %d)\n", lock.l_pid);
        return 0;
    }

	ausgabe(LOG, DEBUGMINI, "Lockstatus�berpr�fung von THEONLYFILE war ohne Ergebnis.\n");
    return 0;
}



// Ende
