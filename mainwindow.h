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
#include <QMessageBox>

#include <QtAddOnSerialPort/serialport.h>
#include <QtAddOnSerialPort/serialportinfo.h>

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT
    
  public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
  private:
    Ui::MainWindow *ui;
    QtAddOn::SerialPort::SerialPort *port;
    QList<QtAddOn::SerialPort::SerialPortInfo> portList;
    bool communicationOngoing;

    void validateCommunicationSettings(void);
    void enableCommunicationSettings(void);
    void disableCommunicationSettings(void);

  private slots:
    // Automatically connected
    void on_portComboBox_currentIndexChanged(int);
    void on_baudRateComboBox_currentIndexChanged(QString const&);
    void on_dataBitsComboBox_currentIndexChanged(int);
    void on_stopBitsComboBox_currentIndexChanged(int);
    void on_parityComboBox_currentIndexChanged(int);
    void on_flowControlComboBox_currentIndexChanged(int);

    void on_asciiLineEdit_textEdited();
    void on_binaryLineEdit_textEdited();

    void on_asciiLineEdit_returnPressed();
    void on_binaryLineEdit_returnPressed();

    void on_getPortsButton_clicked();
    void on_openPortButton_clicked();
    void on_startCommunicationButton_clicked();
    void on_sendAsciiButton_clicked();
    void on_sendBinaryButton_clicked();
    void on_clearButton_clicked();

    void on_actionAbout_triggered();
    void on_actionSerial_Port_toggled(bool);

    void on_serialPortSettingsDock_visibilityChanged(bool visible);

    // Need manual connection
    void port_readyRead();
};

#endif // MAINWINDOW_H
