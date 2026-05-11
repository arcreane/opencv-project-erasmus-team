#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "EditorModel.hpp"

#include <QMainWindow>

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


private:
    Ui::MainWindow *ui;
    EditorModel *model;

};
#endif 