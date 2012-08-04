#include "messagewidget.h"
#include "ui_messagewidget.h"

MessageWidget::MessageWidget(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::MessageWidget)
{
  ui->setupUi(this);

  ui->asciiLineEdit->setValidator(new QRegExpValidator(QRegExp(QLatin1String("[\\x0000-\\x007F]+")), this));
  ui->byteLineEdit->setValidator(new QIntValidator(0, 255, this));
}

MessageWidget::~MessageWidget()
{
  delete ui;
}

void MessageWidget::on_asciiLineEdit_textChanged(const QString &)
{
  ui->sendAsciiPushButton->setEnabled(ui->asciiLineEdit->hasAcceptableInput());
}

void MessageWidget::on_byteLineEdit_textChanged(const QString &)
{
  ui->sendBytePushButton->setEnabled(ui->byteLineEdit->hasAcceptableInput());
}

void MessageWidget::on_sendAsciiPushButton_clicked()
{
  QByteArray message;

  message.append(ui->asciiLineEdit->text());

  ui->asciiLineEdit->clear();

  emit send(message);

  if (ui->echoCheckBox->isChecked()) {
    message.prepend("\n");
    message.append("\n");

    emit echo(message);
  }

  ui->asciiLineEdit->setFocus();
}

void MessageWidget::on_sendBytePushButton_clicked()
{
  QByteArray message;

  message.append(ui->byteLineEdit->text().toInt());

  ui->byteLineEdit->clear();

  emit send(message);

  if (ui->echoCheckBox->isChecked()) {
    message.prepend("\n");
    message.append("\n");

    emit echo(message);
  }

  ui->byteLineEdit->setFocus();
}

void MessageWidget::on_asciiLineEdit_returnPressed()
{
  on_sendAsciiPushButton_clicked();
}

void MessageWidget::on_byteLineEdit_returnPressed()
{
  on_sendBytePushButton_clicked();
}
