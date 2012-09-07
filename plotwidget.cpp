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

#include "plotwidget.h"
#include "ui_plotwidget.h"

#include "hexstring.h"

PlotWidget::PlotWidget(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::PlotWidget),
  bytesPerUpdate(0)
{
  ui->setupUi(this);

  connect(ui->dataTypeComboBox, SIGNAL(currentIndexChanged(int)),
          this,                 SLOT(updateBytesPerUpdate()));

  connect(ui->endiannessComboBox, SIGNAL(currentIndexChanged(int)),
          this,                   SLOT(updateBytesPerUpdate()));

  connect(ui->formatComboBox, SIGNAL(currentIndexChanged(int)),
          this,               SLOT(updateBytesPerUpdate()));

  connect(ui->numberOfPlotsSpinBox, SIGNAL(valueChanged(int)),
          this,                     SLOT(updateBytesPerUpdate()));

  ui->dataTypeComboBox->addItem(QLatin1String("uint8"), UINT8);
  ui->dataTypeComboBox->addItem(QLatin1String("uint16"), UINT16);
  ui->dataTypeComboBox->addItem(QLatin1String("uint32"), UINT32);
  ui->dataTypeComboBox->addItem(QLatin1String("int8"), INT8);
  ui->dataTypeComboBox->addItem(QLatin1String("int16"), INT16);
  ui->dataTypeComboBox->addItem(QLatin1String("int32"), INT32);
  ui->dataTypeComboBox->addItem(QLatin1String("float"), FLOAT);

  ui->endiannessComboBox->addItem(QLatin1String("Little"), QDataStream::LittleEndian);
  ui->endiannessComboBox->addItem(QLatin1String("Big"), QDataStream::BigEndian);

  ui->formatComboBox->addItem(QLatin1String("Raw binary"), Binary);
  ui->formatComboBox->addItem(QLatin1String("Hex string"), Hexadecimal);
}

PlotWidget::~PlotWidget()
{
  delete ui;
}

void PlotWidget::display(QByteArray incomingData)
{
  static QByteArray storedData;

  if (plotVector.size()) {

    storedData.append(incomingData);

    for (int i = 0; i < storedData.size() / bytesPerUpdate; i++) {
      if (ui->formatComboBox->itemData(ui->formatComboBox->currentIndex()).value<int>() == Hexadecimal)
        decode(HexString::toRawBinary(storedData.left(bytesPerUpdate)));
      else
        decode(storedData.left(bytesPerUpdate));
      storedData.remove(0, bytesPerUpdate);
    }
  }
}

void PlotWidget::on_goPushButton_clicked()
{
  int numberOfPlots = plotVector.size();

  if (numberOfPlots) {
    for (int i = 0; i < numberOfPlots; i++) {
      delete plotVector[i];
      plotVector[i] = 0;
    }

    plotVector.clear();

    ui->goPushButton->setText("Go");

    ui->numberOfPlotsSpinBox->setEnabled(true);
    ui->dataTypeComboBox->setEnabled(true);
    ui->endiannessComboBox->setEnabled(true);
    ui->formatComboBox->setEnabled(true);
  } else {
    size_t numberOfSamples = ui->samplesShownSpinBox->value();

    switch(ui->dataTypeComboBox->itemData(ui->dataTypeComboBox->currentIndex()).value<int>()) {
      case UINT8:
        for (int i = 0; i < ui->numberOfPlotsSpinBox->value(); i++) {
          plotVector.push_back(new Plot(numberOfSamples, 0, 255));

          plotVector[i]->setId(QString::number(i + 1));

          ui->plotLayout->addWidget(plotVector[i]);
        }
        break;

      case UINT16:
        for (int i = 0; i < ui->numberOfPlotsSpinBox->value(); i++) {
          plotVector.push_back(new Plot(numberOfSamples, 0, 65535));

          plotVector[i]->setId(QString::number(i + 1));

          ui->plotLayout->addWidget(plotVector[i]);
        }
        break;

      case UINT32:
        for (int i = 0; i < ui->numberOfPlotsSpinBox->value(); i++) {
          plotVector.push_back(new Plot(numberOfSamples, 0, 2147483647));

          plotVector[i]->setId(QString::number(i + 1));

          ui->plotLayout->addWidget(plotVector[i]);
        }
        break;

      case INT8:
        for (int i = 0; i < ui->numberOfPlotsSpinBox->value(); i++) {
          plotVector.push_back(new Plot(numberOfSamples, -128, 127));

          plotVector[i]->setId(QString::number(i + 1));

          ui->plotLayout->addWidget(plotVector[i]);
        }
        break;

      case INT16:
        for (int i = 0; i < ui->numberOfPlotsSpinBox->value(); i++) {
          plotVector.push_back(new Plot(numberOfSamples, -32768, 32768));

          plotVector[i]->setId(QString::number(i + 1));

          ui->plotLayout->addWidget(plotVector[i]);
        }
        break;

      case INT32:
        for (int i = 0; i < ui->numberOfPlotsSpinBox->value(); i++) {
          plotVector.push_back(new Plot(numberOfSamples, -2147483648, 2147483647));

          plotVector[i]->setId(QString::number(i + 1));

          ui->plotLayout->addWidget(plotVector[i]);
        }
        break;

      case FLOAT:
        for (int i = 0; i < ui->numberOfPlotsSpinBox->value(); i++) {
          plotVector.push_back(new Plot(numberOfSamples, -1, 1));

          plotVector[i]->setId(QString::number(i + 1));

          ui->plotLayout->addWidget(plotVector[i]);
        }
        break;
    }

    ui->goPushButton->setText("Clear");
    ui->numberOfPlotsSpinBox->setDisabled(true);
    ui->dataTypeComboBox->setDisabled(true);
    ui->endiannessComboBox->setDisabled(true);
    ui->formatComboBox->setDisabled(true);
  }
}

void PlotWidget::on_samplesShownSpinBox_valueChanged(int value)
{
  // TODO Clean up these magic numbers
  if (value <= 80) {
    if (value % 10) {
      value = ((value / 10) + 1) * 10;
      ui->samplesShownSpinBox->setValue(value);
    }
    ui->samplesShownSpinBox->setSingleStep(10);
  } else if (value <= 160) {
    if (value % 20) {
      value = ((value / 20) + 1) * 20;
      ui->samplesShownSpinBox->setValue(value);
    }
    ui->samplesShownSpinBox->setSingleStep(20);
  } else if (value <= 400) {
    if (value % 50) {
      value = ((value / 50) + 1) * 50;
      ui->samplesShownSpinBox->setValue(value);
    }
    ui->samplesShownSpinBox->setSingleStep(50);
  } else if (value <= 800) {
    if (value % 100) {
      value = ((value / 100) + 1) * 100;
      ui->samplesShownSpinBox->setValue(value);
    }
    ui->samplesShownSpinBox->setSingleStep(100);
  } else if (value <= 1600) {
    if (value % 200) {
      value = ((value / 200) + 1) * 200;
      ui->samplesShownSpinBox->setValue(value);
    }
    ui->samplesShownSpinBox->setSingleStep(200);
  } else if (value <= 4000) {
    if (value % 500) {
      value = ((value / 500) + 1) * 500;
      ui->samplesShownSpinBox->setValue(value);
    }
        ui->samplesShownSpinBox->setSingleStep(500);
  } else if (value <= 8000) {
    if (value % 1000) {
      value = ((value / 1000) + 1) * 1000;
      ui->samplesShownSpinBox->setValue(value);
    }
    ui->samplesShownSpinBox->setSingleStep(1000);
  } else {
    if (value % 2000) {
      value = ((value / 2000) + 1) * 2000;
      ui->samplesShownSpinBox->setValue(value);
    }
    ui->samplesShownSpinBox->setSingleStep(2000);
  }

  if (plotVector.size()) {
    for (int i = 0; i < plotVector.size(); i++) {
      plotVector[i]->setNumberOfSamples(value);

      plotVector[i]->replot();
    }
  }
}

void PlotWidget::updateBytesPerUpdate()
{
  bytesPerUpdate = ui->numberOfPlotsSpinBox->value();

  switch(ui->dataTypeComboBox->itemData(ui->dataTypeComboBox->currentIndex()).value<int>()) {
    case FLOAT:
    case UINT32:
    case INT32:
      bytesPerUpdate *= 4;
      break;
    case UINT16:
    case INT16:
      bytesPerUpdate *= 2;
      break;
    case UINT8:
    case INT8:
      bytesPerUpdate *= 1;
      break;
  }

  if (ui->formatComboBox->itemData(ui->formatComboBox->currentIndex()).value<int>() == Hexadecimal)
    bytesPerUpdate *= 2;

  if (bytesPerUpdate == 1)
    ui->bytesPerUpdateLabel->setText(QString::number(bytesPerUpdate) +
                                                     QLatin1String(" byte"));
  else
    ui->bytesPerUpdateLabel->setText(QString::number(bytesPerUpdate) +
                                                     QLatin1String(" bytes"));
}

void PlotWidget::decode(QByteArray data)
{
  if (plotVector.size()) {
    QDataStream decoder(&data, QIODevice::ReadOnly);

    decoder.setByteOrder(QDataStream::ByteOrder(ui->endiannessComboBox->itemData(ui->endiannessComboBox->currentIndex()).value<int>()));

    switch(ui->dataTypeComboBox->itemData(ui->dataTypeComboBox->currentIndex()).value<int>()) {
      case UINT8:
        quint8 tmpUint8;

        for (int i = 0; i < plotVector.size(); i++) {
          decoder >> tmpUint8;
          plotVector[i]->pushBack(tmpUint8);
          plotVector[i]->replot();
        }
        break;

      case UINT16:
        quint16 tmpUint16;

        for (int i = 0; i < plotVector.size(); i++) {
          decoder >> tmpUint16;
          plotVector[i]->pushBack(tmpUint16);
          plotVector[i]->replot();
        }
        break;

      case UINT32:
        quint32 tmpUint32;

        for (int i = 0; i < plotVector.size(); i++) {
          decoder >> tmpUint32;
          plotVector[i]->pushBack(tmpUint32);
          plotVector[i]->replot();
        }
        break;

      case INT8:
        qint8 tmpInt8;

        for (int i = 0; i < plotVector.size(); i++) {
          decoder >> tmpInt8;
          plotVector[i]->pushBack(tmpInt8);
          plotVector[i]->replot();
        }
        break;

      case INT16:
        qint16 tmpInt16;

        for (int i = 0; i < plotVector.size(); i++) {
          decoder >> tmpInt16;
          plotVector[i]->pushBack(tmpInt16);
          plotVector[i]->replot();
        }
        break;

      case INT32:
        qint32 tmpInt32;

        for (int i = 0; i < plotVector.size(); i++) {
          decoder >> tmpInt32;
          plotVector[i]->pushBack(tmpInt32);
          plotVector[i]->replot();
        }
        break;

      case FLOAT:
        float tmpFloat;
        decoder.setFloatingPointPrecision(QDataStream::SinglePrecision);

        for (int i = 0; i < plotVector.size(); i++) {
          decoder >> tmpFloat;
          plotVector[i]->pushBack(tmpFloat);
          plotVector[i]->replot();
        }
        break;
    }
  }
}
