# Solar-Logger - A Daemon for logging Data from a Solar Plant
# Copyright (C) 2007  Klaus Dotterweich (dr.dotti@gmx.de)
#
# This program is free software; you can redistribute it and/or modify it under the terms of the
# GNU General Public License as published by the Free Software Foundation;
# either version 3 of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
# without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along with this program;
# if not, see <http://www.gnu.org/licenses/>.
#
# Solar-Logger - Ein Daemon zum loggen der Daten einer Solaranlage
# Copyright (C) 2007  Klaus Dotterweich (dr.dotti@gmx.de)
#
# Dieses Programm ist freie Software. Sie k�nnen es unter den Bedingungen der GNU General Public License,
# wie von der Free Software Foundation ver�ffentlicht, weitergeben und/oder modifizieren,
# entweder gem�� Version 3 der Lizenz oder (nach Ihrer Option) jeder sp�teren Version.
#
# Die Ver�ffentlichung dieses Programms erfolgt in der Hoffnung, da� es Ihnen von Nutzen sein wird,
# aber OHNE IRGENDEINE GARANTIE, sogar ohne die implizite Garantie der MARKTREIFE oder
# der VERWENDBARKEIT F�R EINEN BESTIMMTEN ZWECK. Details finden Sie in der GNU General Public License.
#
# Sie sollten ein Exemplar der GNU General Public License zusammen mit diesem Programm erhalten haben.
# Falls nicht, siehe <http://www.gnu.org/licenses/>.

YASDI	= ../../yasdi-1.7.2/
QFTP	= ../../ftplib-3.1/linux/
MODBUS  = /home/ubuntu/libmodbus-2.0.0
CC		= gcc
CFLAGS	= -g -O2 -c
INC		= -I$(YASDI)include/ -I$(YASDI)smalib/ -I$(YASDI)libs/ -I$(QFTP) -I$(MODBUS)
OBJECTS	= solar-logger.o daemon.o log.o sqlite3common.o logsqlite3.o logsunmeter.o ini.o ftp.o timer.o wrerfassen.o wrabfragen.o kanalrech.o solueberwachen.o solstatus.o sollog.o
LIBS	= -L$(YASDI)/projects/linuxlib -L$(QFTP) -L$(MODBUS)/modbus/.libs -lmodbus -lyasdi -lyasdimaster -lftp -lsqlite3


all: solar-logger

solar-logger: $(OBJECTS)
	$(CC) -o $@ $(OBJECTS) $(INC) $(LIBS)


%.o: %.c
	$(CC) $(CFLAGS) $(INC) $<


clean:
	rm -v *.o
	rm -v solar-logger











#
# Beispiel
# --------
#
# Ziel:			Der Name vor dem Doppelpunkt ist das zu �berstzende Ziel
# 				In den Zeilen danach k�nnen shell befehle stehen.
#				Wichtig ist nur dass jede Zeile mit einem TAB beginnt
#				sonst dir die Zeile nicht erkannt.
#
# Nun Kann man mit
# 'make Ziel' das Ziel "Ziel" �bersetzen.
# gibt man nur
# 'make' an so wird das erste Ziel im makefile ausgef�hrt.
#
# Ziel : Abh�ngigkeitsliste
#
#	Das Ziel ist von allen Datein in der Abh�ngigkeitsliste abh�ngig.
#
#
# gcc Optionen
# ------------
#
# -g		Debug Informationen in Ausgabedatei schreiben
# -O2		Steuert die Stufen der Codeoptimierung
# -Idir		Verzeichnis f�r Include Datein definieren
# -c		nur Kompilieren, Linker nicht aktivieren
# -ldatei	Verwenden bestimmter Bibliotheken
# -o datei	Ausgabedatei definieren
#
#
