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

#ifndef SERIALPORTWIDGET_H
#define SERIALPORTWIDGET_H

#include <QWidget>

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#include <QTimer>

namespace Ui {
  class SerialPortWidget;
}

class SerialPortWidget : public QWidget
{
    Q_OBJECT

  public:
    explicit SerialPortWidget(QWidget *parent = 0);
    ~SerialPortWidget();

  public slots:
    void write(QByteArray);

  signals:
    void read(QByteArray);
    void communicationStart(bool);

  private slots:
    void on_getPortsPushButton_clicked();
    void on_openPortPushButton_clicked();
    void on_startCommunicationPushButton_clicked();

    void on_portComboBox_currentIndexChanged(int);
    void on_baudRateComboBox_currentIndexChanged(int);
    void on_dataBitsComboBox_currentIndexChanged(int);
    void on_stopBitsComboBox_currentIndexChanged(int);
    void on_parityComboBox_currentIndexChanged(int);
    void on_flowControlComboBox_currentIndexChanged(int);

    void on_refreshRateTimer_timeout();

  private:
    void enableCommunicationSettings();
    void disableCommunicationSettings();
    void validateCommunicationSettings();

    Ui::SerialPortWidget *ui;
    QSerialPort *serialPort;
    QList<QSerialPortInfo> serialPortInfoList;
    QTimer *refreshRateTimer;
    bool isThereCommunication;
};

#endif // SERIALPORTWIDGET_H
