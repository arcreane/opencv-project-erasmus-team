#include "MainWindow.hpp"
#include "ui_mainwindow.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->resize(1100, 750);
    this->setMinimumSize(900, 600);
    model = new EditorModel();

    ui->sliderThresh->setValue(127);
    ui->sliderClipLimit->setValue(20);
    ui->sliderGridSize->setValue(8);

    ui->labelThreshValue->setText("127");
    ui->labelClipValue->setText("2.0");
    ui->labelGridValue->setText("8x8");
    ui->labelMorphValue->setText("3x3");
    ui->comboKernelShape->setCurrentIndex(0);

    ui->imageLabel->installEventFilter(this);

    ui->imageLabel->setAttribute(Qt::WA_TransparentForMouseEvents);

    connect(ui->actionGrabCut_Segmnetation, &QAction::triggered, this, [this]()
        {
            isGrabCutMode = true;
            ui->imageLabel->setCursor(Qt::CrossCursor);
        });
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

            QPixmap originalPixmap = QPixmap::fromImage(img.rgbSwapped());

            int w = ui->imageLabel->contentsRect().width();
            int h = ui->imageLabel->contentsRect().height();

            if (w < 100) w = 600;
            if (h < 100) h = 500;

            QPixmap scaledPixmap = originalPixmap.scaled(w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation);

            ui->imageLabel->setAlignment(Qt::AlignCenter);
            ui->imageLabel->setScaledContents(false);
            ui->imageLabel->setPixmap(scaledPixmap);
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
        }
        else {
        }
    }
}

void MainWindow::updateDisplay() {
    if (model->processedImage.empty()) return;

    QImage img;

    if (model->processedImage.channels() == 3) {
        img = QImage((const unsigned char*)(model->processedImage.data),
            model->processedImage.cols, model->processedImage.rows,
            model->processedImage.step, QImage::Format_RGB888);

        img = img.rgbSwapped();
    }
    else {
        img = QImage((const unsigned char*)(model->processedImage.data),
            model->processedImage.cols, model->processedImage.rows,
            model->processedImage.step, QImage::Format_Grayscale8);
    }

    QPixmap originalPixmap = QPixmap::fromImage(img);

    int w = ui->imageLabel->contentsRect().width();
    int h = ui->imageLabel->contentsRect().height();

    if (w < 100) w = 600;
    if (h < 100) h = 500;

    QPixmap scaledPixmap = originalPixmap.scaled(w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    ui->imageLabel->setAlignment(Qt::AlignCenter);
    ui->imageLabel->setScaledContents(false);
    ui->imageLabel->setPixmap(scaledPixmap);
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
    ui->labelThreshValue->setText(QString::number(value));
    model->applyThreshold(0, value);
    updateDisplay();
}

void MainWindow::on_buttonGlobalEq_clicked()
{
    model->applyGlobalEqualization();
    updateDisplay();
}

void MainWindow::on_sliderClipLimit_valueChanged(int value)
{
    double limit = value / 10.0;
    ui->labelClipValue->setText(QString::number(limit, 'f', 1));

    int gridSize = ui->sliderGridSize->value();

    model->applyCLAHE(limit, gridSize);
    updateDisplay();
}

void MainWindow::on_sliderGridSize_valueChanged(int value)
{
    double limit = ui->sliderClipLimit->value() / 10.0;
    ui->labelGridValue->setText(QString("%1x%1").arg(value));

    model->applyCLAHE(limit, value);
    updateDisplay();
}

void MainWindow::on_btnErosion_clicked()
{
    int val = ui->sliderMorphSize->value();
    int shape = ui->comboKernelShape->currentIndex();
    model->applyMorphology(0, val, shape);
    updateDisplay();
}

void MainWindow::on_btnDilation_clicked()
{
    int val = ui->sliderMorphSize->value();
    int shape = ui->comboKernelShape->currentIndex();
    model->applyMorphology(1, val, shape);
    updateDisplay();
}

void MainWindow::on_btnNoise_clicked()
{
    int val = ui->sliderMorphSize->value();
    int shape = ui->comboKernelShape->currentIndex();
    model->applyMorphology(2, val, shape);
    updateDisplay();
}

void MainWindow::on_btnClosing_clicked()
{
    int val = ui->sliderMorphSize->value();
    int shape = ui->comboKernelShape->currentIndex();
    model->applyMorphology(3, val, shape);
    updateDisplay();
}

void MainWindow::on_btnGradient_clicked()
{
    int val = ui->sliderMorphSize->value();
    int shape = ui->comboKernelShape->currentIndex();
    model->applyMorphology(4, val, shape);
    updateDisplay();
}

void MainWindow::on_sliderMorphSize_valueChanged(int value)
{
    int actualSize = 2 * value + 1;
    ui->labelMorphValue->setText(QString("%1x%1").arg(actualSize));

    int shape = ui->comboKernelShape->currentIndex();
    model->applyMorphology(2, value, shape);
    updateDisplay();
}

void MainWindow::on_comboKernelShape_currentIndexChanged(int index)
{
    int val = ui->sliderMorphSize->value();
    model->applyMorphology(2, val, index);
    updateDisplay();
}

void MainWindow::mousePressEvent(QMouseEvent* event) {
    if (!isGrabCutMode) return;

    QPoint labelPos = ui->imageLabel->mapFromGlobal(QCursor::pos());

    if (ui->imageLabel->rect().contains(labelPos)) {
        isDrawingRect = true;
        startPoint = labelPos;
        endPoint = startPoint;
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent* event) {
    if (!isGrabCutMode || !isDrawingRect) return;

    endPoint = ui->imageLabel->mapFromGlobal(QCursor::pos());
    updateDisplayWithRect();
}

void MainWindow::mouseReleaseEvent(QMouseEvent* event) {
    if (!isGrabCutMode || !isDrawingRect) return;

    isDrawingRect = false;
    endPoint = ui->imageLabel->mapFromGlobal(QCursor::pos());

    int displayX = std::min(startPoint.x(), endPoint.x());
    int displayY = std::min(startPoint.y(), endPoint.y());
    int displayW = std::abs(startPoint.x() - endPoint.x());
    int displayH = std::abs(startPoint.y() - endPoint.y());

    if (displayW > 10 && displayH > 10) {
        double scaleX = (double)model->currentImage.cols / ui->imageLabel->width();
        double scaleY = (double)model->currentImage.rows / ui->imageLabel->height();

        int realX = displayX * scaleX;
        int realY = displayY * scaleY;
        int realW = displayW * scaleX;
        int realH = displayH * scaleY;

        grabCutRect = cv::Rect(realX, realY, realW, realH);

        model->applyGrabCut(grabCutRect);

        updateDisplay();

        isGrabCutMode = false;
    }
    else {
        on_actionOpen_triggered();
    }
}

void MainWindow::updateDisplayWithRect() {
    if (model->currentImage.empty()) return;

    QImage img((const unsigned char*)(model->currentImage.data),
        model->currentImage.cols, model->currentImage.rows,
        model->currentImage.step, QImage::Format_RGB888);

    QPixmap originalPixmap = QPixmap::fromImage(img.rgbSwapped());

    int w = ui->imageLabel->contentsRect().width();
    int h = ui->imageLabel->contentsRect().height();
    if (w < 100) w = 600;
    if (h < 100) h = 500;

    QPixmap scaledPixmap = originalPixmap.scaled(w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    QImage drawingImage = scaledPixmap.toImage();
    QPainter painter(&drawingImage);
    painter.setPen(QPen(Qt::red, 2, Qt::DashLine));

    int x = std::min(startPoint.x(), endPoint.x());
    int y = std::min(startPoint.y(), endPoint.y());
    int width = std::abs(startPoint.x() - endPoint.x());
    int height = std::abs(startPoint.y() - endPoint.y());

    painter.drawRect(x, y, width, height);

    ui->imageLabel->setPixmap(QPixmap::fromImage(drawingImage));
}