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

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow),
  imageWidget(0),
  plotWidget(0),
  terminalWidget(0)
{
  ui->setupUi(this);

  this->showMaximized();
  ui->frameDockWidget->hide();

  connect(ui->serialPortWidget, SIGNAL(read(QByteArray)),
          ui->loggerWidget,     SLOT(append(QByteArray)));

  connect(ui->messageWidget,    SIGNAL(send(QByteArray)),
          ui->serialPortWidget, SLOT(write(QByteArray)));

  connect(ui->frameWidget,      SIGNAL(send(QByteArray)),
          ui->serialPortWidget, SLOT(write(QByteArray)));

  connect(ui->serialPortWidget, SIGNAL(communicationStart(bool)),
          ui->actionImage,   SLOT(setDisabled(bool)));

  connect(ui->serialPortWidget, SIGNAL(communicationStart(bool)),
          ui->actionPlot,   SLOT(setDisabled(bool)));

  connect(ui->serialPortWidget, SIGNAL(communicationStart(bool)),
          ui->actionTerminal,   SLOT(setDisabled(bool)));


  fromDeviceActionGroup = new QActionGroup(this);

  fromDeviceActionGroup->setExclusive(true);
  fromDeviceActionGroup->addAction(ui->actionImage);
  fromDeviceActionGroup->addAction(ui->actionPlot);
  fromDeviceActionGroup->addAction(ui->actionTerminal);
  ui->actionTerminal->setChecked(true);
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::on_actionAbout_triggered()
{
  AboutDialog *aboutDialog = new AboutDialog(this);

  aboutDialog->setAttribute(Qt::WA_DeleteOnClose);

  aboutDialog->show();
}

void MainWindow::on_actionImage_toggled(bool checked)
{
  if (checked) {
    imageWidget = new ImageWidget;

    ui->fromDeviceGridLayout->addWidget(imageWidget);

    connect(ui->serialPortWidget, SIGNAL(read(QByteArray)),
            imageWidget,          SLOT(display(QByteArray)));

    imageWidget->show();
  } else {
    delete imageWidget;
    imageWidget = 0;
  }
}

void MainWindow::on_actionPlot_toggled(bool checked)
{
  if (checked) {
    plotWidget = new PlotWidget;

    ui->fromDeviceGridLayout->addWidget(plotWidget);

    connect(ui->serialPortWidget, SIGNAL(read(QByteArray)),
            plotWidget,           SLOT(display(QByteArray)));

    plotWidget->show();
  } else {
    delete plotWidget;
    plotWidget = 0;
  }
}

void MainWindow::on_actionTerminal_toggled(bool checked)
{
  if (checked) {
    terminalWidget = new TerminalWidget;

    ui->fromDeviceGridLayout->addWidget(terminalWidget);

    terminalWidget->show();

    connect(ui->serialPortWidget, SIGNAL(read(QByteArray)),
            terminalWidget,       SLOT(display(QByteArray)));

    connect(ui->messageWidget,  SIGNAL(echo(QByteArray)),
            terminalWidget,     SLOT(display(QByteArray)));
  } else {
    delete terminalWidget;
    terminalWidget = 0;
  }
}

void MainWindow::on_frameDockWidget_visibilityChanged(bool)
{
  ui->actionFrame->setChecked(!ui->frameDockWidget->isHidden());
}

void MainWindow::on_loggerDockWidget_visibilityChanged(bool)
{
  ui->actionLogger->setChecked(!ui->loggerDockWidget->isHidden());
}

void MainWindow::on_messageDockWidget_visibilityChanged(bool)
{
  ui->actionMessage->setChecked(!ui->messageDockWidget->isHidden());
}

void MainWindow::on_serialPortDockWidget_visibilityChanged(bool)
{
  ui->actionSerial_Port->setChecked(!ui->serialPortDockWidget->isHidden());
}
