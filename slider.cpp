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
