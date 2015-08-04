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

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets serialport

QWT_ROOT = /media/Datos/Programas/libqwt6

include ( $$QWT_ROOT/features/qwt.prf )

win32 {
    CONFIG +=  serialport qwt
}

unix {
    LIBS +=  -lQt5SerialPort
    #LIBS += -lqwt
    #INCLUDEPATH += /usr/include/qwt
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
    hexstring.cpp \
    plot.cpp \
    plotwidget.cpp \
    circularbuffer.cpp \
    imagewidget.cpp

HEADERS     +=  mainwindow.h \
    aboutdialog.h \
    loggerwidget.h \
    serialportwidget.h \
    messagewidget.h \
    terminalwidget.h \
    slider.h \
    framewidget.h \
    hexstring.h \
    plot.h \
    plotwidget.h \
    circularbuffer.h \
    imagewidget.h

FORMS       +=  mainwindow.ui \
    aboutdialog.ui \
    loggerwidget.ui \
    serialportwidget.ui \
    messagewidget.ui \
    terminalwidget.ui \
    slider.ui \
    framewidget.ui \
    plot.ui \
    plotwidget.ui \
    imagewidget.ui

OTHER_FILES +=  \
                README \
                COPYING.GPL3
