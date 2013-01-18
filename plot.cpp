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

#include "plot.h"
#include "ui_plot.h"

#include "circularbuffer.h"
#include "qwt_plot_curve.h"
#include "qwt_plot_marker.h"
#include "qwt_plot_grid.h"

#include <QTime>

Q_DECLARE_METATYPE(Qt::GlobalColor)

Plot::Plot(size_t numberOfSamples, qreal minY, qreal maxY, QWidget *parent) :
  QWidget(parent),
  ui(new Ui::Plot)
{
  ui->setupUi(this);

  circularBuffer = new CircularBuffer(numberOfSamples,
                                      ui->minYSpinBox->value(),
                                      ui->maxYSpinBox->value());

  ui->minYSpinBox->setValue(minY);
  ui->maxYSpinBox->setValue(maxY);

  curve = new QwtPlotCurve();
  curve->setData(circularBuffer);
  curve->attach(ui->plot);

  ui->colorComboBox->addItem("Black", Qt::black);
  ui->colorComboBox->addItem("Dark gray", Qt::darkGray);
  ui->colorComboBox->addItem("Gray", Qt::gray);
  ui->colorComboBox->addItem("Light gray", Qt::lightGray);
  ui->colorComboBox->addItem("Red", Qt::red);
  ui->colorComboBox->addItem("Green", Qt::green);
  ui->colorComboBox->addItem("Blue", Qt::blue);
  ui->colorComboBox->addItem("Cyan", Qt::cyan);
  ui->colorComboBox->addItem("Magenta", Qt::magenta);
  ui->colorComboBox->addItem("Yellow", Qt::yellow);
  ui->colorComboBox->addItem("Dark red", Qt::darkRed);
  ui->colorComboBox->addItem("Dark green", Qt::darkGreen);
  ui->colorComboBox->addItem("Dark blue", Qt::darkBlue);
  ui->colorComboBox->addItem("Dark cyan", Qt::darkCyan);
  ui->colorComboBox->addItem("Dark magenta", Qt::darkMagenta);
  ui->colorComboBox->addItem("Dark yellow", Qt::darkYellow);

  qsrand(QTime::currentTime().msec());
  ui->colorComboBox->setCurrentIndex(qrand() % ui->colorComboBox->count());
}

Plot::~Plot()
{
  delete ui;
}

void Plot::setId(QString const& str)
{
  ui->idLineEdit->setText(str);
}

void Plot::setNumberOfSamples(size_t size)
{
  circularBuffer->resize(size);
}

void Plot::pushBack(qreal sample)
{
  circularBuffer->pushBack(sample);
}

void Plot::replot()
{
  ui->plot->replot();
}

void Plot::on_maxYSpinBox_valueChanged(int value)
{
  ui->minYSpinBox->setMaximum(value - 1);

  circularBuffer->setMaxY(value);

  ui->plot->replot();
}

void Plot::on_minYSpinBox_valueChanged(int value)
{
  ui->maxYSpinBox->setMinimum(value + 1);

  circularBuffer->setMinY(value);

  ui->plot->replot();
}

void Plot::on_xAxisCheckBox_toggled(bool checked)
{
  if (checked) {
    xAxis = new QwtPlotMarker;

    xAxis->setLineStyle(QwtPlotMarker::HLine);
    xAxis->setYValue(0);
    xAxis->attach(ui->plot);

    ui->plot->replot();
  } else {
    delete xAxis;
    xAxis = 0;

    ui->plot->replot();
  }
}

void Plot::on_colorComboBox_currentIndexChanged(int index)
{
  curve->setPen(QPen(ui->colorComboBox->itemData(index).value<QColor>()));

  ui->plot->replot();
}

void Plot::on_gridCheckBox_toggled(bool checked)
{
  if (checked) {
    grid = new QwtPlotGrid;

    grid->setMajPen(QPen(Qt::black, 0, Qt::DashLine));
    grid->attach(ui->plot);

    ui->plot->replot();
  } else {
    delete grid;
    grid = 0;

    ui->plot->replot();
  }
}
