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
