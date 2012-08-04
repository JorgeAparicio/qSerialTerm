#include "terminalwidget.h"
#include "ui_terminalwidget.h"

TerminalWidget::TerminalWidget(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::TerminalWidget)
{
  ui->setupUi(this);
}

TerminalWidget::~TerminalWidget()
{
  delete ui;
}

void TerminalWidget::display(QByteArray const &text)
{
  if (text.length() != 0) {
    ui->terminalTextEdit->moveCursor(QTextCursor::End,
                                     QTextCursor::MoveAnchor);

    ui->terminalTextEdit->insertPlainText(text);
  }
}

void TerminalWidget::on_cleanPushButton_clicked()
{
  ui->terminalTextEdit->clear();
}
