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

#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <QWidget>

namespace Ui {
  class ImageWidget;
}

class ImageWidget : public QWidget
{
    Q_OBJECT

  public:
    enum Format {
      MONO = 0,
      RGB888 = 1,
      YCBCR422 = 2
    };

    explicit ImageWidget(QWidget *parent = 0);
    ~ImageWidget();

  public slots:
    void display(QByteArray);

  private slots:
    void on_clearPushButton_clicked();

    void on_startPushButton_clicked();

    void on_fitToScreenCheckBox_toggled(bool checked);

  private:
    Ui::ImageWidget *ui;
    QImage *image;

    QVector<QRgb> decode(QByteArray);
    void saturate(int&, int, int);
};

#endif // IMAGEWIDGET_H
