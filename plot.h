#ifndef PLOT_H
#define PLOT_H

#include <QWidget>

class CircularBuffer;
class QwtPlotMarker;
class QwtPlotCurve;
class QwtPlotGrid;

namespace Ui {
  class Plot;
}

class Plot : public QWidget
{
    Q_OBJECT
    
  public:
    explicit Plot(size_t, qreal, qreal, QWidget *parent = 0);
    ~Plot();
    void setId(QString const&);
    void setNumberOfSamples(size_t);
    void pushBack(qreal);
    void replot();
    
  private slots:
    void on_maxYSpinBox_valueChanged(int);
    void on_minYSpinBox_valueChanged(int);

    void on_xAxisCheckBox_toggled(bool checked);

    void on_colorComboBox_currentIndexChanged(int index);

    void on_gridCheckBox_toggled(bool checked);

  private:
    Ui::Plot *ui;
    CircularBuffer *circularBuffer;
    QwtPlotMarker *xAxis;
    QwtPlotCurve *curve;
    QwtPlotGrid *grid;
};

#endif // PLOT_H
