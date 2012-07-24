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

#include <QFileDialog>
#include <QDateTime>

QT_USE_NAMESPACE_SERIALPORT

/* CONSTRUCTORS AND DESTRUCTORS */

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow),
  port(this),
  isThereCommunication(false),
  isThereLogging(false)

{
  // Configure the GUI
  ui->setupUi(this);

  // Initialize class members
  refreshRateTimer.setInterval(25);
  secondKeeper.setInterval(1000);

  // Add validators to the QLineEdits
  QRegExp asciiRegExp("[\\x0000-\\x007F]*");

  ui->asciiLineEdit->setValidator(new QRegExpValidator(asciiRegExp, this));
  ui->binaryLineEdit->setValidator(new QIntValidator(0, 255, this));

  // Signal-Slot connections
  connect(&refreshRateTimer, SIGNAL(timeout()),
          this,              SLOT(on_refreshRateTimer_timeout()));

  connect(&secondKeeper, SIGNAL(timeout()),
          this,          SLOT(on_secondKeep_timeout()));
  }

MainWindow::~MainWindow()
{
  delete ui;
}

/* METHODS */

void MainWindow::validateCommunicationSettings(void)
{

  if (port.isOpen() &&
      (ui->baudRateComboBox->currentIndex() != -1) &&
      (ui->dataBitsComboBox->currentIndex() != -1) &&
      (ui->stopBitsComboBox->currentIndex() != -1) &&
      (ui->parityComboBox->currentIndex() != -1) &&
      (ui->flowControlComboBox->currentIndex() != -1))
    ui->startCommunicationButton->setEnabled(true);

}

void MainWindow::enableCommunicationSettings(void)
{
  ui->baudRateComboBox->setEnabled(true);
  ui->dataBitsComboBox->setEnabled(true);
  ui->stopBitsComboBox->setEnabled(true);
  ui->parityComboBox->setEnabled(true);
  ui->flowControlComboBox->setEnabled(true);
}

void MainWindow::disableCommunicationSettings(void)
{
  ui->baudRateComboBox->setDisabled(true);
  ui->dataBitsComboBox->setDisabled(true);
  ui->stopBitsComboBox->setDisabled(true);
  ui->parityComboBox->setDisabled(true);
  ui->flowControlComboBox->setDisabled(true);
}

/* SLOTS */

void MainWindow::on_portComboBox_currentIndexChanged(int index)
{
  if (index == -1) {
    ui->openPortButton->setDisabled(true);

    ui->baudRateComboBox->clear();

    ui->pidLabel->setText("-");
    ui->vidLabel->setText("-");
    ui->portStatusLabel->setText("<font color=red>No port select");
  } else {
    ui->openPortButton->setEnabled(true);

    port.setPort(portList[index].portName());

    QList<qint32> baudRateList = portList[index].standardRates();

    ui->baudRateComboBox->clear();

    foreach(qint32 baudRate, baudRateList)
      ui->baudRateComboBox->addItem(QString::number(baudRate));

    ui->baudRateComboBox->setCurrentIndex(-1);

    QString vid = portList[index].vendorIdentifier();

    if (vid.isEmpty())
      ui->vidLabel->setText("-");
    else
      ui->vidLabel->setText("0x" + vid);

    QString pid = portList[index].productIdentifier();

    if (pid.isEmpty())
      ui->pidLabel->setText("-");
    else
      ui->pidLabel->setText("0x" + pid);

    ui->portStatusLabel->setText("<font color=red>Closed");
  }
}

void MainWindow::on_baudRateComboBox_currentIndexChanged(QString const& rate)
{
  port.setRate(qint32(rate.toLongLong()));
}

void MainWindow::on_dataBitsComboBox_currentIndexChanged(int index)
{
  bool ok = true;

  switch(index) {
    case -1:
      ui->startCommunicationButton->setDisabled(true);
      break;
    case 0:
      ok = port.setDataBits(SerialPort::Data5);
      break;
    case 1:
      ok = port.setDataBits(SerialPort::Data6);
      break;
    case 2:
      ok = port.setDataBits(SerialPort::Data7);
      break;
    case 3:
      ok = port.setDataBits(SerialPort::Data8);
      break;
  }

  validateCommunicationSettings();

  if (!ok) {
    // TODO Couldn't change the number of data bits
  }
}

void MainWindow::on_stopBitsComboBox_currentIndexChanged(int index)
{
  bool ok = true;

  switch(index) {
    case -1:
      ui->startCommunicationButton->setDisabled(true);
      break;
    case 0:
      ok = port.setStopBits(SerialPort::OneStop);
      break;
    case 1:
      ok = port.setStopBits(SerialPort::OneAndHalfStop);
      break;
    case 2:
      ok = port.setStopBits(SerialPort::TwoStop);
      break;
  }

  validateCommunicationSettings();

  if (!ok) {
    // TODO Couldn't change the number of stop bits
  }
}

void MainWindow::on_parityComboBox_currentIndexChanged(int index)
{
  bool ok = true;

  switch(index) {
    case -1:
      ui->startCommunicationButton->setDisabled(true);
      break;
    case 0:
      ok = port.setParity(SerialPort::NoParity);
      break;
    case 1:
      ok = port.setParity(SerialPort::EvenParity);
      break;
    case 2:
      ok = port.setParity(SerialPort::OddParity);
      break;
    case 3:
      ok = port.setParity(SerialPort::SpaceParity);
      break;
    case 4:
      ok = port.setParity(SerialPort::MarkParity);
      break;
  }

  validateCommunicationSettings();

  if (!ok) {
    // TODO Couldnt' change the parity
  }
}

void MainWindow::on_flowControlComboBox_currentIndexChanged(int index)
{
  bool ok = true;

  switch(index) {
    case -1:
      ui->startCommunicationButton->setDisabled(true);
      break;
    case 0:
      ok = port.setFlowControl(SerialPort::NoFlowControl);
      break;
    case 1:
      ok = port.setFlowControl(SerialPort::HardwareControl);
      break;
    case 2:
      ok = port.setFlowControl(SerialPort::SoftwareControl);
      break;
  }

  validateCommunicationSettings();

  if (!ok) {
    // TODO Couldn't change the flow control
  }
}

void MainWindow::on_asciiLineEdit_textEdited()
{
  if (isThereCommunication &&
      ui->asciiLineEdit->hasAcceptableInput())
    ui->sendAsciiButton->setEnabled(true);
  else
    ui->sendAsciiButton->setDisabled(true);
}

void MainWindow::on_binaryLineEdit_textEdited()
{
  if (isThereCommunication &&
      ui->binaryLineEdit->hasAcceptableInput())
    ui->sendBinaryButton->setEnabled(true);
  else
    ui->sendBinaryButton->setDisabled(true);
}

void MainWindow::on_filePathEdit_textChanged(const QString &str)
{
  ui->startLoggingButton->setEnabled(str.length() != 0);
}


void MainWindow::on_asciiLineEdit_returnPressed()
{
  on_sendAsciiButton_clicked();
}

void MainWindow::on_binaryLineEdit_returnPressed()
{
  on_sendBinaryButton_clicked();
}

void MainWindow::on_getPortsButton_clicked()
{
  portList = SerialPortInfo::availablePorts();

  ui->portComboBox->clear();

  foreach(SerialPortInfo entry, portList)
    ui->portComboBox->addItem(entry.portName());
}

void MainWindow::on_openPortButton_clicked()
{
  if (port.isOpen()) {

    if (isThereCommunication)
      on_startCommunicationButton_clicked();

    port.close();

    ui->portStatusLabel->setText("<font color=red>Closed");
    ui->openPortButton->setText("Open");

    ui->portComboBox->setEnabled(true);

    disableCommunicationSettings();

    ui->baudRateComboBox->setCurrentIndex(-1);
    ui->dataBitsComboBox->setCurrentIndex(-1);
    ui->stopBitsComboBox->setCurrentIndex(-1);
    ui->parityComboBox->setCurrentIndex(-1);
    ui->flowControlComboBox->setCurrentIndex(-1);

  } else {
    if (port.open(QIODevice::ReadWrite)) {

      ui->portStatusLabel->setText("<font color=green>Open");
      ui->openPortButton->setText("Close");

      enableCommunicationSettings();

      ui->portComboBox->setDisabled(true);

    } else {
      QMessageBox::warning(this, "Couldn't open port", "qSerialTerm couldn't "
                           "open the requested port, check that the selected "
                           "port is not already open.");
    }
  }
}

void MainWindow::on_startCommunicationButton_clicked()
{
  if (isThereCommunication) {
    enableCommunicationSettings();

    ui->sendAsciiButton->setDisabled(true);
    ui->sendBinaryButton->setDisabled(true);

    ui->startCommunicationButton->setText("Start");

    ui->communicationStatusLabel->setText("<font color=red>No communication");

    isThereCommunication = false;

    refreshRateTimer.stop();
  } else {
    disableCommunicationSettings();

    ui->startCommunicationButton->setText("Stop");

    ui->communicationStatusLabel->setText("<font color=green>Ongoing");

    isThereCommunication = true;

    on_binaryLineEdit_textEdited();
    on_asciiLineEdit_textEdited();

    refreshRateTimer.start();
  }
}

void MainWindow::on_sendAsciiButton_clicked()
{
  QString ascii(ui->asciiLineEdit->text());

  if (ui->echoCheckBox->isChecked()) {
    ui->terminalTextEdit->append(ascii);
    ui->terminalTextEdit->insertPlainText("\n");
  }
  port.write(ascii.toLocal8Bit());

  ui->asciiLineEdit->clear();
  ui->asciiLineEdit->setFocus();
}

void MainWindow::on_sendBinaryButton_clicked()
{
  QByteArray frame;

  frame.append(ui->binaryLineEdit->text().toShort());

  if (ui->echoCheckBox->isChecked()) {
    ui->terminalTextEdit->append(frame);
    ui->terminalTextEdit->insertPlainText("\n");
  }

  port.write(frame);

  ui->binaryLineEdit->clear();
  ui->binaryLineEdit->setFocus();
}

void MainWindow::on_clearButton_clicked()
{
  ui->terminalTextEdit->clear();
}

void MainWindow::on_browseButton_clicked()
{
  ui->filePathEdit->setText(QFileDialog::getSaveFileName(this, "Save file"));
}

void MainWindow::on_startLoggingButton_clicked()
{
  if (isThereLogging) {
    secondKeeper.stop();

    logFile.close();
    ui->startLoggingButton->setText("Start");

    ui->appendCheckBox->setEnabled(true);

    ui->loggingTimeLabel->setText("<font color=red>" +
                                  loggingTime.toString("hh:mm:ss"));

    isThereLogging = false;
  } else {
    bool ok;

    logFile.setFileName(ui->filePathEdit->text());

    if (ui->appendCheckBox->isChecked())
      ok = logFile.open(QIODevice::WriteOnly |
                         QIODevice::Text |
                         QIODevice::Append);
    else
      ok = logFile.open(QIODevice::WriteOnly |
                         QIODevice::Text);
    if (ok) {
      logFileStream.setDevice(&logFile);

      ui->filePathEdit->setDisabled(true);
      ui->startLoggingButton->setText("Stop");

      ui->appendCheckBox->setDisabled(true);

      loggingTime.setHMS(0, 0, 0);
      ui->loggingTimeLabel->setText("<font color=red>" +
                                    loggingTime.toString("hh:mm:ss"));
      secondKeeper.start();

      isThereLogging = true;
    } else {
      QMessageBox::warning(this, "Couldn't open file", "qSerialTerm couldn't "
                           "open the requested file, check the file path.");
    }
  }
}

void MainWindow::on_actionAbout_triggered()
{
  QMessageBox::about(this, "About",
                     "<center><h1>qSerialTerm</h1><br/>"
                     "qSerialTerm is a simple serial port terminal emulator."
                     "<br/><br/>Copyright (C) 2012 Jorge Aparicio "
                     "&lt;<a href=\"mailto:jorge.aparicio.r@gmail.com\">"
                     "jorge.aparicio.r@gmail.com</a>&gt;<br/><br/>"
                     "qserialTerm is licensed under the: <br/><br/>"
                     "<a href=\"http://www.gnu.org/licenses/\">"
                     "GNU General Public License(GPL) v3</a><br/><br/>"
                     "The qSerialTerm source code can be found in this "
                     "<a href=\"https://github.com/JorgeAparicio/qSerialTerm\">"
                     "repository</a>.<br/></center>");
}

void MainWindow::on_actionSerial_Port_toggled(bool checked)
{
  if (checked)
    ui->serialPortSettingsDock->show();
  else
    ui->serialPortSettingsDock->hide();
}

void MainWindow::on_actionLogging_toggled(bool checked)
{
  if (checked)
    ui->loggingDockWidget->show();
  else
    ui->loggingDockWidget->hide();
}

void MainWindow::on_serialPortSettingsDock_visibilityChanged(bool visible)
{
  if (!visible)
    ui->actionSerial_Port->setChecked(false);
}

void MainWindow::on_loggingDockWidget_visibilityChanged(bool visible)
{
  if (!visible)
    ui->actionLogging->setChecked(false);
}

void MainWindow::on_refreshRateTimer_timeout()
{
  QByteArray receivedCharacters = port.readAll();

  if (receivedCharacters.length() != 0) {
    ui->terminalTextEdit->moveCursor(QTextCursor::End,
                                     QTextCursor::MoveAnchor);

    ui->terminalTextEdit->insertPlainText(receivedCharacters);

    if (isThereLogging)
      logFileStream << receivedCharacters;
  }
}

void MainWindow::on_secondKeep_timeout()
{
  loggingTime = loggingTime.addSecs(1);

  ui->loggingTimeLabel->setText("<font color=green>" +
                                loggingTime.toString("hh:mm:ss"));
}
