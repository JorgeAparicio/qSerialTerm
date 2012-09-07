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

#include "framewidget.h"
#include "ui_framewidget.h"

#include "hexstring.h"

FrameWidget::FrameWidget(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::FrameWidget),
  refreshRateTimer(0)
{
  ui->setupUi(this);

  ui->appendLineEdit->setValidator(new QIntValidator(0, 255));
  ui->prependLineEdit->setValidator(new QIntValidator(0, 255));

  ui->dataTypeComboBox->addItem(QLatin1String("uint8"), UINT8);
  ui->dataTypeComboBox->addItem(QLatin1String("uint16"), UINT16);
  ui->dataTypeComboBox->addItem(QLatin1String("uint32"), UINT32);
  ui->dataTypeComboBox->addItem(QLatin1String("int8"), INT8);
  ui->dataTypeComboBox->addItem(QLatin1String("int16"), INT16);
  ui->dataTypeComboBox->addItem(QLatin1String("int32"), INT32);

  ui->endiannessComboBox->addItem(QLatin1String("Little"), QDataStream::LittleEndian);
  ui->endiannessComboBox->addItem(QLatin1String("Big"), QDataStream::BigEndian);

  ui->formatComboBox->addItem(QLatin1String("Raw binary"), Binary);
  ui->formatComboBox->addItem(QLatin1String("Hex string"), Hexadecimal);

  ui->payloadSpinBox->setValue(4);
}

FrameWidget::~FrameWidget()
{
  delete ui;
}

void FrameWidget::on_dataTypeComboBox_currentIndexChanged(int index)
{
  int min = 0, max = 0;

  switch(ui->dataTypeComboBox->itemData(index).value<int>()) {
    case UINT8:
      max = 255;
      break;

    case UINT16:
      max = 65535;
      break;

    case UINT32:
      max = 2147483647;
      break;

    case INT8:
      min = -128;
      max = 127;
      break;

    case INT16:
      min = -32768;
      max = 32767;
      break;

    case INT32:
      min = -2147483648;
      max = 2147483647;
      break;
  }

  ui->minimumValueSpinBox->setMinimum(min);
  ui->maximumValueSpinBox->setMaximum(max);

  ui->minimumValueSpinBox->setValue(min);
  ui->maximumValueSpinBox->setValue(max);
}

void FrameWidget::on_payloadSpinBox_valueChanged(int value)
{
  for (int i = 0; i < sliderVector.size(); i++) {
    delete sliderVector[i];
    sliderVector[i] = 0;
  }

  sliderVector.clear();

  for (int i = 0; i < value; i++) {
    sliderVector.push_back(new Slider);

    sliderVector[i]->setMinimumValue(ui->minimumValueSpinBox->value());
    sliderVector[i]->setMaximumValue(ui->maximumValueSpinBox->value());
    sliderVector[i]->setId(QLatin1String("<b>") +
                           QString::number(i + 1) +
                           QLatin1String("</b>"));

    ui->gridLayout->addWidget(sliderVector[i]);
  }
}

void FrameWidget::on_minimumValueSpinBox_valueChanged(int min)
{
  ui->maximumValueSpinBox->setMinimum(min);

  for (int i = 0; i < sliderVector.size(); i++)
    sliderVector[i]->setMinimumValue(min);
}

void FrameWidget::on_maximumValueSpinBox_valueChanged(int max)
{
  ui->minimumValueSpinBox->setMaximum(max);

  for (int i = 0; i < sliderVector.size(); i++)
    sliderVector[i]->setMaximumValue(max);
}

void FrameWidget::on_continuousCheckBox_toggled(bool checked)
{
  if (checked)
    ui->sendPushButton->setText(QLatin1String("Start"));
  else
    ui->sendPushButton->setText(QLatin1String("Send"));
}

void FrameWidget::on_sendPushButton_clicked()
{
  if (ui->continuousCheckBox->isChecked()) {
    if (refreshRateTimer) { // Communication ongoing
      delete refreshRateTimer;
      refreshRateTimer = 0;

      ui->sendPushButton->setText(QLatin1String("Start"));

      enableSettings();
    } else {
      refreshRateTimer = new QTimer(this);

      connect(refreshRateTimer, SIGNAL(timeout()),
              this,             SLOT(on_refreshRateTimer_timeout()));

      disableSettings();

      ui->sendPushButton->setText(QLatin1String("Stop"));

      refreshRateTimer->start(ui->refreshRateValueSpinBox->text().toInt());
    }
  } else {
    emit send(encode());
  }
}

void FrameWidget::on_refreshRateTimer_timeout()
{
  emit send(encode());
}

QByteArray FrameWidget::encode()
{
  QByteArray frame;

  QDataStream encoder(&frame, QIODevice::ReadWrite);

  encoder.setByteOrder(QDataStream::ByteOrder(ui->endiannessComboBox->itemData(ui->endiannessComboBox->currentIndex()).value<int>()));

  switch(ui->dataTypeComboBox->itemData(ui->dataTypeComboBox->currentIndex()).value<int>()) {
    case UINT8:
      for (int i = 0; i < sliderVector.size(); i++)
        encoder << quint8(sliderVector[i]->value());
      break;

    case UINT16:
      for (int i = 0; i < sliderVector.size(); i++)
        encoder << quint16(sliderVector[i]->value());
      break;

    case UINT32:
      for (int i = 0; i < sliderVector.size(); i++)
        encoder << quint32(sliderVector[i]->value());
      break;

    case INT8:
      for (int i = 0; i < sliderVector.size(); i++)
        encoder << qint8(sliderVector[i]->value());
      break;

    case INT16:
      for (int i = 0; i < sliderVector.size(); i++)
        encoder << qint16(sliderVector[i]->value());
      break;

    case INT32:
      for (int i = 0; i < sliderVector.size(); i++)
        encoder << qint32(sliderVector[i]->value());
      break;
  }

  if (ui->formatComboBox->itemData(ui->formatComboBox->currentIndex()).value<int>() == Hexadecimal)
    frame = HexString::fromRawBinary(frame);

  if (ui->prependLineEdit->hasAcceptableInput())
    frame.insert(0, char(ui->prependLineEdit->text().toInt()));

  if(ui->appendLineEdit->hasAcceptableInput())
    frame.append(char(ui->appendLineEdit->text().toInt()));

  return frame;
}

void FrameWidget::enableSettings()
{
  ui->payloadSpinBox->setEnabled(true);
  ui->dataTypeComboBox->setEnabled(true);
  ui->endiannessComboBox->setEnabled(true);
  ui->formatComboBox->setEnabled(true);
  ui->appendLineEdit->setEnabled(true);
  ui->prependLineEdit->setEnabled(true);
  ui->payloadSpinBox->setEnabled(true);
  ui->refreshRateValueSpinBox->setEnabled(true);
  ui->continuousCheckBox->setEnabled(true);
}


void FrameWidget::disableSettings()
{
  ui->payloadSpinBox->setDisabled(true);
  ui->dataTypeComboBox->setDisabled(true);
  ui->endiannessComboBox->setDisabled(true);
  ui->formatComboBox->setDisabled(true);
  ui->appendLineEdit->setDisabled(true);
  ui->prependLineEdit->setDisabled(true);
  ui->payloadSpinBox->setDisabled(true);
  ui->refreshRateValueSpinBox->setDisabled(true);
  ui->continuousCheckBox->setDisabled(true);
}
