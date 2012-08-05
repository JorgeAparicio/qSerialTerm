#
# Copyright (C) 2012 Jorge Aparicio <jorge.aparicio.r@gmail.com>
#
# This file is part of qSerialTerm.
#
# qSerialTerm is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# any later version.
#
# qSerialTerm is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with qSerialTerm.  If not, see <http://www.gnu.org/licenses/>.
#

QT          +=  core gui

win32 {
    CONFIG  += serialport
}

unix {
    LIBS    +=  -lSerialPort
}

TARGET      =   qSerialTerm
TEMPLATE    =   app
SOURCES     +=  main.cpp\
              mainwindow.cpp \
    aboutdialog.cpp \
    loggerwidget.cpp \
    serialportwidget.cpp \
    messagewidget.cpp \
    terminalwidget.cpp \
    slider.cpp \
    framewidget.cpp \
    hexstring.cpp

HEADERS     +=  mainwindow.h \
    aboutdialog.h \
    loggerwidget.h \
    serialportwidget.h \
    messagewidget.h \
    terminalwidget.h \
    slider.h \
    framewidget.h \
    hexstring.h

FORMS       +=  mainwindow.ui \
    aboutdialog.ui \
    loggerwidget.ui \
    serialportwidget.ui \
    messagewidget.ui \
    terminalwidget.ui \
    slider.ui \
    framewidget.ui

OTHER_FILES +=  \
                README \
                COPYING.GPL3
