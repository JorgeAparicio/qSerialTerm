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

#include "slider.h"
#include "ui_slider.h"

Slider::Slider(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::Slider)
{
  ui->setupUi(this);
}

Slider::~Slider()
{
  delete ui;
}

void Slider::setMinimumValue(int min)
{
  ui->valueSlider->setMinimum(min);
  ui->valueSpinBox->setMinimum(min);
}

void Slider::setMaximumValue(int max)
{
  ui->valueSlider->setMaximum(max);
  ui->valueSpinBox->setMaximum(max);
}

int Slider::value()
{
  return ui->valueSlider->value();
}

void Slider::setId(QString const& str)
{
  ui->idLabel->setText(str);
}

void Slider::on_valueSlider_sliderMoved(int value)
{
  ui->valueSpinBox->setValue(value);
}

void Slider::on_valueSpinBox_valueChanged(int value)
{
  ui->valueSlider->setValue(value);
}
