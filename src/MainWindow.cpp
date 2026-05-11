#include "MainWindow.hpp"
#include "ui_mainwindow.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    model = new EditorModel();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete model;
}

void MainWindow::on_actionOpen_triggered()
{
   QString fileName = QFileDialog::getOpenFileName(this,
                                                   tr("Open Image"), "", tr("Image Files (*.png *.jpg *.bmp)"));


   if (!fileName.isEmpty()) {
       if (model->loadImage(fileName.toStdString())) {
           QImage img((const unsigned char*)(model->currentImage.data),
                      model->currentImage.cols, model->currentImage.rows,
                      model->currentImage.step, QImage::Format_RGB888);


           ui->imageLabel->setPixmap(QPixmap::fromImage(img.rgbSwapped()));
           ui->imageLabel->setScaledContents(true);
       }
   }
}

void MainWindow::on_actionSave_triggered()
{
   if (model->processedImage.empty()) {
       return;
   }


   QString fileName = QFileDialog::getSaveFileName(this,
       tr("Save Image"), "", tr("PNG Image (*.png);;JPG Image (*.jpg);;BMP Image (*.bmp)"));


   if (!fileName.isEmpty()) {
       if (cv::imwrite(fileName.toStdString(), model->processedImage)) {
       } else {
       }
   }
}

void MainWindow::updateDisplay() {
   if (model->processedImage.empty()) return;


   QImage img((const unsigned char*)(model->processedImage.data),
              model->processedImage.cols, model->processedImage.rows,
              model->processedImage.step, QImage::Format_Grayscale8);


   ui->imageLabel->setPixmap(QPixmap::fromImage(img));
   ui->imageLabel->setScaledContents(true);
}


void MainWindow::on_btnBinary_clicked()
{
   int val = ui->sliderThresh->value();
   model->applyThreshold(0, val);
   updateDisplay();
}


void MainWindow::on_btnOtsu_clicked()
{
   model->applyThreshold(1, 0);
   updateDisplay();
}


void MainWindow::on_btnAdaptive_clicked()
{
   int val = ui->sliderThresh->value();
   model->applyThreshold(2, val);
   updateDisplay();
}


void MainWindow::on_sliderThresh_valueChanged(int value)
{
   model->applyThreshold(0, value);
   updateDisplay();
}


