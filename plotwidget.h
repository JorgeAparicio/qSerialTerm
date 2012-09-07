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

#ifndef PLOTWIDGET_H
#define PLOTWIDGET_H

#include <QWidget>
#include <QSpacerItem>

#include "plot.h"

namespace Ui {
  class PlotWidget;
}

class PlotWidget : public QWidget
{
    Q_OBJECT
    
  public:
    enum DataType {
      UINT8 = 0,
      UINT16 = 1,
      UINT32 = 2,
      INT8 = 3,
      INT16 = 4,
      INT32 = 5,
      FLOAT = 6
    };

    enum Endianness {
      Little = 0,
      Big = 1
    };

    enum Format {
      Binary = 0,
      Hexadecimal = 1
    };

    explicit PlotWidget(QWidget *parent = 0);
    ~PlotWidget();
    
  public slots:
    void display(QByteArray);

  private slots:
    void on_goPushButton_clicked();

    void on_samplesShownSpinBox_valueChanged(int arg1);

    void updateBytesPerUpdate();

  private:
    Ui::PlotWidget *ui;
    int bytesPerUpdate;
    QVector<Plot*> plotVector;

    void decode(QByteArray);

};

#endif // PLOTWIDGET_H
