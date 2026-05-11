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
