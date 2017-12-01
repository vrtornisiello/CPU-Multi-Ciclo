# code-c - Main Makefile
#
#   Copyright 2017  Monaco F. J.   <monaco@icmc.usp.br>
#
#   This file is part of Muticlycle Mips CPU (MMCPU)
#
#   MMCPU is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with this program.  If not, see <http://www.gnu.org/licenses/>.


BIN = cpu
OBJECTS = cpu.o code.o

CC = gcc


$(BIN) : $(OBJECTS) 
	$(CC) $^ -o $@


%.o : %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -g

%.d : %.c
	$(CC) $(CPPFLAGA) $(CFLAGS) -MM -MT '$(<:%.c=%.o) $@' $< -o $@

deps = $(OBJECTS:%.o=%.d)

include $(deps)

.PHONY : clean

clean:
	rm -f $(LDFLAGS) $(OBJECTS) $(BIN) *~ \#*  $(deps)

