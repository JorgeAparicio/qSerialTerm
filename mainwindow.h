/*
 * Copyright (C) 2012 Jorge Aparicio <jorge.aparicio.r@gmail.com>
 *
 * This file is part of qSerialTerm.
 *
 * qSerialTerm is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.

 * qSerialTerm is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with qSerialTerm.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QActionGroup>

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#include "aboutdialog.h"
#include "imagewidget.h"
#include "messagewidget.h"
#include "plotwidget.h"
#include "terminalwidget.h"

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT
    
  public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
  private slots:
    void on_actionAbout_triggered();

    void on_actionImage_toggled(bool);
    void on_actionPlot_toggled(bool);
    void on_actionTerminal_toggled(bool);

    void on_frameDockWidget_visibilityChanged(bool);
    void on_loggerDockWidget_visibilityChanged(bool);
    void on_messageDockWidget_visibilityChanged(bool);
    void on_serialPortDockWidget_visibilityChanged(bool);

  private:
    Ui::MainWindow *ui;
    ImageWidget *imageWidget;
    PlotWidget *plotWidget;
    TerminalWidget *terminalWidget;
    QActionGroup *fromDeviceActionGroup;
};

#endif // MAINWINDOW_H
