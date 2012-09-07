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

#ifndef SLIDER_H
#define SLIDER_H

#include <QWidget>

namespace Ui {
  class Slider;
}

class Slider : public QWidget
{
    Q_OBJECT
    
  public:
    explicit Slider(QWidget *parent = 0);
    ~Slider();
    void setMinimumValue(int);
    void setMaximumValue(int);
    int value();
    void setId(QString const&);
    
  private slots:
    void on_valueSlider_sliderMoved(int);
    void on_valueSpinBox_valueChanged(int);

  private:
    Ui::Slider *ui;
};

#endif // SLIDER_H
