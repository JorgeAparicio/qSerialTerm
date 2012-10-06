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

#include "imagewidget.h"
#include "ui_imagewidget.h"

ImageWidget::ImageWidget(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::ImageWidget),
  image(0)
{
  ui->setupUi(this);

  ui->formatComboBox->addItem("Monochrome", MONO);
  ui->formatComboBox->addItem("RGB888", RGB888);
  ui->formatComboBox->addItem("YCbCr422", YCBCR422);
}

ImageWidget::~ImageWidget()
{
  delete ui;
}

void ImageWidget::display(QByteArray incomingData)
{
  static QByteArray storedData;

  if (image) {
    int width = image->width();
    int height = image->height();
    int numberOfBytes = width * height;

    switch (ui->formatComboBox->itemData(ui->formatComboBox->currentIndex()).value<int>()) {
      case RGB888:
        numberOfBytes *= 3;
        break;
      case YCBCR422:
        numberOfBytes *= 2;
        break;
    }

    storedData.append(incomingData);

    if (storedData.size() >= numberOfBytes) {
      int framesToSkip = storedData.size() / numberOfBytes - 1;

      if (framesToSkip > 0)
        storedData.remove(0, numberOfBytes * framesToSkip);

      QVector<QRgb> pixels = decode(storedData.left(numberOfBytes));

      storedData.remove(0, numberOfBytes);

      for (int j = 0; j < height; j++)
        for (int i = 0; i < width; i++)
          image->setPixel(i, j, pixels.at(j * width + i));

      on_fitToScreenCheckBox_toggled(ui->fitToScreenCheckBox->isChecked());
    }
  }
}

void ImageWidget::on_clearPushButton_clicked()
{
  if (image) {
    for (int j = 0; j < image->height(); j++)
      for (int i = 0; i < image->width(); i++)
        image->setPixel(i, j, qRgb(0, 0, 0));

    on_fitToScreenCheckBox_toggled(ui->fitToScreenCheckBox->isChecked());
  }
}

void ImageWidget::on_startPushButton_clicked()
{
  if (image) {
    ui->imageLabel->setText("Your image goes here.");

    ui->widthSpinBox->setEnabled(true);
    ui->heightSpinBox->setEnabled(true);
    ui->formatComboBox->setEnabled(true);

    ui->startPushButton->setText("Start");

    delete image;
    image = 0;
  } else {
    image = new QImage(ui->widthSpinBox->value(),
                       ui->heightSpinBox->value(),
                       QImage::Format_RGB888);

    ui->widthSpinBox->setDisabled(true);
    ui->heightSpinBox->setDisabled(true);
    ui->formatComboBox->setDisabled(true);

    ui->startPushButton->setText("Stop");

    on_clearPushButton_clicked();
  }
}

void ImageWidget::on_fitToScreenCheckBox_toggled(bool checked)
{
  if (image) {
    ui->imageLabel->clear();

    if (checked)
      ui->imageLabel->setPixmap(QPixmap::fromImage(*image).scaled(ui->imageLabel->size(),
                                                                  Qt::KeepAspectRatio));
    else
      ui->imageLabel->setPixmap(QPixmap::fromImage(*image));
  }
}

QVector<QRgb> ImageWidget::decode(QByteArray data)
{
  QVector<QRgb> pixels;
  int format = ui->formatComboBox->itemData(ui->formatComboBox->currentIndex()).value<int>();
  bool grayscale = ui->grayscaleCheckBox->isChecked();

  switch (format) {
    case MONO:
      for (int i = 0; i < data.size(); i++) {
        int level = quint8(data.at(i));
        pixels.append(qRgb(level, level, level));
      }
      break;

    case RGB888:
      if (grayscale) {
        for (int i = 0; i < data.size(); i += 3) {
          int red = quint8(data.at(i));
          int green = quint8(data.at(i + 1));
          int blue = quint8(data.at(i + 2));
          int gray = (red + green + blue) / 3;

          pixels.append(qRgb(gray, gray, gray));
        }
      } else {
        for (int i = 0; i < data.size(); i += 3) {
          int red = quint8(data.at(i));
          int green = quint8(data.at(i + 1));
          int blue = quint8(data.at(i + 2));

          pixels.append(qRgb(red, green, blue));
        }
      }
      break;

    case YCBCR422:
      if (grayscale) {
        for (int i = 0; i < data.size(); i += 4) {
          int y1 = quint8(data.at(i + 1));
          int y2 = quint8(data.at(i + 3));

          pixels.append(qRgb(y1, y1, y1));
          pixels.append(qRgb(y2, y2, y2));
        }
      } else {
        const int k13 = int(1.402f * (1 << 16));
        const int k22 = int(0.334f * (1 << 16));
        const int k23 = int(0.714f * (1 << 16));
        const int k32 = int(1.772f * (1 << 16));

        for (int i = 0; i < data.size(); i += 4) {
          int red, green, blue;

          int cb = quint8(data.at(i)) - 128;
          int y1 = quint8(data.at(i + 1));
          int cr = quint8(data.at(i + 2)) - 128;
          int y2 = quint8(data.at(i + 3));

          red = y1 + (k13 * cr >> 16);
          green = y1 - (k22 * cb >> 16) - (k23 * cr >> 16);
          blue = y1 + (k32 * cb >> 16);

          saturate(red, 0, 255);
          saturate(green, 0, 255);
          saturate(blue, 0, 255);

          pixels.append(qRgb(red, green, blue));

          red = y2 + (k13 * cr >> 16);
          green = y2 - (k22 * cb >> 16) - (k23 * cr >> 16);
          blue = y2 + (k32 * cb >> 16);

          saturate(red, 0, 255);
          saturate(green, 0, 255);
          saturate(blue, 0, 255);

          pixels.append(qRgb(red, green, blue));
        }
      }
      break;
  }

  return pixels;
}

void ImageWidget::saturate(int &value, int min, int max)
{
  if (value < min)
    value = min;
  else if (value > max)
    value = max;
}
