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

#include "serialportwidget.h"
#include "ui_serialportwidget.h"

#include <QMessageBox>

#define REFRESH_RATE_MS 40

Q_DECLARE_METATYPE(QSerialPort::DataBits)
Q_DECLARE_METATYPE(QSerialPort::StopBits)
Q_DECLARE_METATYPE(QSerialPort::Parity)
Q_DECLARE_METATYPE(QSerialPort::FlowControl)

SerialPortWidget::SerialPortWidget(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::SerialPortWidget),
  serialPort(0),
  refreshRateTimer(0),
  isThereCommunication(false)
{
  ui->setupUi(this);

  ui->dataBitsComboBox->addItem(QLatin1String("5"),
                                QSerialPort::Data5);
  ui->dataBitsComboBox->addItem(QLatin1String("6"),
                                QSerialPort::Data6);
  ui->dataBitsComboBox->addItem(QLatin1String("7"),
                                QSerialPort::Data7);
  ui->dataBitsComboBox->addItem(QLatin1String("8"),
                                QSerialPort::Data8);
  ui->dataBitsComboBox->setCurrentIndex(-1);


  ui->stopBitsComboBox->addItem(QLatin1String("1"),
                                QSerialPort::OneStop);
  ui->stopBitsComboBox->addItem(QLatin1String("1.5"),
                                QSerialPort::OneAndHalfStop);
  ui->stopBitsComboBox->addItem(QLatin1String("2"),
                                QSerialPort::TwoStop);
  ui->stopBitsComboBox->setCurrentIndex(-1);


  ui->parityComboBox->addItem(QLatin1String("No"),
                              QSerialPort::NoParity);
  ui->parityComboBox->addItem(QLatin1String("Even"),
                              QSerialPort::EvenParity);
  ui->parityComboBox->addItem(QLatin1String("Odd"),
                              QSerialPort::OddParity);
  ui->parityComboBox->addItem(QLatin1String("Space"),
                              QSerialPort::SpaceParity);
  ui->parityComboBox->addItem(QLatin1String("Mark"),
                              QSerialPort::MarkParity);
  ui->parityComboBox->setCurrentIndex(-1);


  ui->flowControlComboBox->addItem(QLatin1String("No"),
                                   QSerialPort::NoFlowControl);
  ui->flowControlComboBox->addItem(QLatin1String("Hardware"),
                                   QSerialPort::HardwareControl);
  ui->flowControlComboBox->addItem(QLatin1String("Software"),
                                   QSerialPort::SoftwareControl);
  ui->flowControlComboBox->setCurrentIndex(-1);
}

SerialPortWidget::~SerialPortWidget()
{
  delete ui;
}

void SerialPortWidget::write(QByteArray data)
{
  if (refreshRateTimer) // There is ongoing communication
    serialPort->write(data);
}

void SerialPortWidget::on_getPortsPushButton_clicked()
{
  serialPortInfoList = QSerialPortInfo::availablePorts();

  ui->portComboBox->clear();

  foreach(QSerialPortInfo entry, serialPortInfoList)
    ui->portComboBox->addItem(entry.portName());
}

void SerialPortWidget::on_openPortPushButton_clicked()
{
  if (serialPort) {
    if (refreshRateTimer) // Communication ongoing
      on_startCommunicationPushButton_clicked();

    serialPort->close();

    delete serialPort;
    serialPort = 0;

    ui->portStatusLabel->setText(QLatin1String("<font color=red>"
                                               "Closed"
                                               "</font>"));
    ui->openPortPushButton->setText(QLatin1String("Open"));

    ui->portComboBox->setEnabled(true);
    ui->getPortsPushButton->setEnabled(true);

    disableCommunicationSettings();

    ui->baudRateComboBox->setCurrentIndex(-1);
    ui->dataBitsComboBox->setCurrentIndex(-1);
    ui->stopBitsComboBox->setCurrentIndex(-1);
    ui->parityComboBox->setCurrentIndex(-1);
    ui->flowControlComboBox->setCurrentIndex(-1);
  } else {
    serialPort = new QSerialPort(ui->portComboBox->currentText());

    if (serialPort->open(QIODevice::ReadWrite)) {
      enableCommunicationSettings();

      ui->portStatusLabel->setText(QLatin1String("<font color=green>"
                                                 "Open"
                                                 "</font>"));

      ui->openPortPushButton->setText(QLatin1String("Close"));

      ui->portComboBox->setDisabled(true);
      ui->getPortsPushButton->setDisabled(true);
    } else {
      QMessageBox::warning(this,
                           QLatin1String("Port error"),
                           QLatin1String("Couldn't open the requested port"));

      delete serialPort;
      serialPort = 0;
    }
  }
}

void SerialPortWidget::on_startCommunicationPushButton_clicked()
{
  if (refreshRateTimer) { // There is ongoing communication
    enableCommunicationSettings();
    ui->startCommunicationPushButton->setText(QLatin1String("Start"));
    ui->communicationStatusLabel->setText(QLatin1String("<font color=red>"
                                                        "No communication"
                                                        "</font>"));

    delete refreshRateTimer;
    refreshRateTimer = 0;

    emit communicationStart(false);
  } else {
    disableCommunicationSettings();

    ui->startCommunicationPushButton->setText(QLatin1String("Stop"));
    ui->communicationStatusLabel->setText(QLatin1String("<font color=green>"
                                                        "Ongoing"
                                                        "</font>"));

    refreshRateTimer = new QTimer(this);

    connect(refreshRateTimer, SIGNAL(timeout()),
            this,             SLOT(on_refreshRateTimer_timeout()));

    refreshRateTimer->start(REFRESH_RATE_MS);

    emit communicationStart(true);
  }
}

void SerialPortWidget::on_portComboBox_currentIndexChanged(int index)
{
  if (index == -1) {
    ui->openPortPushButton->setDisabled(true);

    ui->baudRateComboBox->clear();

    ui->pidLabel->setText(QLatin1String("-"));
    ui->vidLabel->setText(QLatin1String("-"));
    ui->portStatusLabel->setText(QLatin1String("<font color=red>"
                                               "No port selected"
                                               "</font>"));
  } else {
    ui->openPortPushButton->setEnabled(true);

    QList<qint32> baudRateList = serialPortInfoList[index].standardBaudRates();

    ui->baudRateComboBox->clear();

    foreach (qint32 baudRate, baudRateList)
      ui->baudRateComboBox->addItem(QString::number(baudRate), baudRate);

    ui->baudRateComboBox->setCurrentIndex(-1);

    QString vid = QString(serialPortInfoList[index].vendorIdentifier());

    if (vid.isEmpty())
      ui->vidLabel->setText(QLatin1String("-"));
    else
      ui->vidLabel->setText(QLatin1String("0x") + vid);

    QString pid = QString(serialPortInfoList[index].productIdentifier());

    if (pid.isEmpty())
      ui->pidLabel->setText(QLatin1String("-"));
    else
      ui->pidLabel->setText(QLatin1String("0x") + pid);

    ui->portStatusLabel->setText(QLatin1String("<font color=red>"
                                               "Closed"
                                               "</font>"));
  }
}

void SerialPortWidget::on_baudRateComboBox_currentIndexChanged(int index)
{
  if (index != -1 && serialPort != 0) {
    if(serialPort->setBaudRate(ui->baudRateComboBox->itemData(index).value<int>())) {
      validateCommunicationSettings();

      return;
    } else {
      QMessageBox::warning(this,
                           QLatin1String("Configuration error"),
                           QLatin1String("Couldn't change the baud rate."));
      ui->baudRateComboBox->setCurrentIndex(-1);
    }
  }

  ui->startCommunicationPushButton->setDisabled(true);
}

void SerialPortWidget::on_dataBitsComboBox_currentIndexChanged(int index)
{
  if (index != -1 && serialPort != 0) {
    if(serialPort->setDataBits(ui->dataBitsComboBox->itemData(index).value<QSerialPort::DataBits>())) {
      validateCommunicationSettings();

      return;
    } else {
      QMessageBox::warning(this,
                           QLatin1String("Configuration error"),
                           QLatin1String("Couldn't change the numbers of data bits."));
      ui->dataBitsComboBox->setCurrentIndex(-1);
    }
  }

  ui->startCommunicationPushButton->setDisabled(true);
}

void SerialPortWidget::on_stopBitsComboBox_currentIndexChanged(int index)
{
  if (index != -1 && serialPort != 0) {
    if(serialPort->setStopBits(ui->stopBitsComboBox->itemData(index).value<QSerialPort::StopBits>())) {
      validateCommunicationSettings();

      return;
    } else {
      QMessageBox::warning(this,
                           QLatin1String("Configuration error"),
                           QLatin1String("Couldn't change the numbers of stop bits."));
      ui->stopBitsComboBox->setCurrentIndex(-1);
    }
  }

  ui->startCommunicationPushButton->setDisabled(true);
}

void SerialPortWidget::on_parityComboBox_currentIndexChanged(int index)
{
  if (index != -1 && serialPort != 0) {
    if(serialPort->setParity(ui->parityComboBox->itemData(index).value<QSerialPort::Parity>())) {
      validateCommunicationSettings();

      return;
    } else {
      QMessageBox::warning(this,
                           QLatin1String("Configuration error"),
                           QLatin1String("Couldn't change the parity."));
      ui->parityComboBox->setCurrentIndex(-1);
    }
  }

  ui->startCommunicationPushButton->setDisabled(true);
}

void SerialPortWidget::on_flowControlComboBox_currentIndexChanged(int index)
{
  if (index != -1 && serialPort != 0) {
    if(serialPort->setFlowControl(ui->flowControlComboBox->itemData(index).value<QSerialPort::FlowControl>())) {
      validateCommunicationSettings();

      return;
    } else {
      QMessageBox::warning(this,
                           QLatin1String("Configuration error"),
                           QLatin1String("Couldn't change the flow control."));
      ui->flowControlComboBox->setCurrentIndex(-1);
    }
  }

  ui->startCommunicationPushButton->setDisabled(true);
}

void SerialPortWidget::on_refreshRateTimer_timeout()
{
  emit read(serialPort->readAll());
}

void SerialPortWidget::enableCommunicationSettings()
{
  ui->baudRateComboBox->setEnabled(true);
  ui->dataBitsComboBox->setEnabled(true);
  ui->stopBitsComboBox->setEnabled(true);
  ui->parityComboBox->setEnabled(true);
  ui->flowControlComboBox->setEnabled(true);
}

void SerialPortWidget::disableCommunicationSettings()
{
  ui->baudRateComboBox->setDisabled(true);
  ui->dataBitsComboBox->setDisabled(true);
  ui->stopBitsComboBox->setDisabled(true);
  ui->parityComboBox->setDisabled(true);
  ui->flowControlComboBox->setDisabled(true);
}

void SerialPortWidget::validateCommunicationSettings(void)
{
  if (serialPort->isOpen() &&
      (ui->baudRateComboBox->currentIndex() != -1) &&
      (ui->dataBitsComboBox->currentIndex() != -1) &&
      (ui->stopBitsComboBox->currentIndex() != -1) &&
      (ui->parityComboBox->currentIndex() != -1) &&
      (ui->flowControlComboBox->currentIndex() != -1)) {
    ui->startCommunicationPushButton->setEnabled(true);
  } else {
    ui->startCommunicationPushButton->setDisabled(true);
  }
}
