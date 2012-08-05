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

  ui->dataTypeComboBox->addItem(QLatin1String("uint8"), uint8);
  ui->dataTypeComboBox->addItem(QLatin1String("uint16"), uint16);
  ui->dataTypeComboBox->addItem(QLatin1String("uint32"), uint32);
  ui->dataTypeComboBox->addItem(QLatin1String("int8"), int8);
  ui->dataTypeComboBox->addItem(QLatin1String("int16"), int16);
  ui->dataTypeComboBox->addItem(QLatin1String("int32"), int32);

  ui->endiannessComboBox->addItem(QLatin1String("Little"), Little);
  ui->endiannessComboBox->addItem(QLatin1String("Big"), Big);

  ui->formatComboBox->addItem(QLatin1String("Raw binary"), Binary);
  ui->formatComboBox->addItem(QLatin1String("Hexadecimal string"), Hexadecimal);

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
    case uint8:
      max = 255;
      break;

    case uint16:
      max = 65535;
      break;

    case uint32:
      max = 2147483647;
      break;

    case int8:
      min = -128;
      max = 127;
      break;

    case int16:
      min = -32768;
      max = 32767;
      break;

    case int32:
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

  QDataStream frameStream(&frame, QIODevice::ReadWrite);

  switch(ui->endiannessComboBox->itemData(ui->endiannessComboBox->currentIndex()).value<int>()) {
    case Little:
      frameStream.setByteOrder(QDataStream::LittleEndian);
      break;

    case Big:
      frameStream.setByteOrder(QDataStream::BigEndian);
      break;
  }

  switch(ui->dataTypeComboBox->itemData(ui->dataTypeComboBox->currentIndex()).value<int>()) {
    case uint8:
      for (int i = 0; i < sliderVector.size(); i++)
        frameStream << quint8(sliderVector[i]->value());
      break;

    case uint16:
      for (int i = 0; i < sliderVector.size(); i++)
        frameStream << quint16(sliderVector[i]->value());
      break;

    case uint32:
      for (int i = 0; i < sliderVector.size(); i++)
        frameStream << quint32(sliderVector[i]->value());
      break;

    case int8:
      for (int i = 0; i < sliderVector.size(); i++)
        frameStream << qint8(sliderVector[i]->value());
      break;

    case int16:
      for (int i = 0; i < sliderVector.size(); i++)
        frameStream << qint16(sliderVector[i]->value());
      break;

    case int32:
      for (int i = 0; i < sliderVector.size(); i++)
        frameStream << qint32(sliderVector[i]->value());
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
