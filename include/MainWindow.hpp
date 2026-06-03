#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "EditorModel.hpp"
#include <QMainWindow>
#include <QMouseEvent>
#include <QPainter>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
    
private slots:
    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_btnBinary_clicked();
    void on_btnOtsu_clicked();
    void on_btnAdaptive_clicked();
    void updateDisplay();
    void on_sliderThresh_valueChanged(int value);
    void on_buttonGlobalEq_clicked();
    void on_sliderClipLimit_valueChanged(int value);
    void on_sliderGridSize_valueChanged(int value);
    void on_btnErosion_clicked();
    void on_btnDilation_clicked();
    void on_btnNoise_clicked();
    void on_btnClosing_clicked();
    void on_btnGradient_clicked();
    void on_sliderMorphSize_valueChanged(int value);
    void on_comboKernelShape_currentIndexChanged(int index);
    void on_btnApplyWB_clicked();


private:
    Ui::MainWindow* ui;
    EditorModel* model;

    bool isGrabCutMode = false;
    bool isDrawingRect = false;
    QPoint startPoint;
    QPoint endPoint;
    cv::Rect grabCutRect;

    bool eventFilter(QObject* watched, QEvent* event) override;
    void updateDisplayWithRect();
};
#endif