#include "MainWindow.hpp"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QApplication>

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

            model->processedImage = cv::Mat();

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

void MainWindow::on_btnApplyWB_clicked()
{
    int index = ui->comboWBMethod->currentIndex();
    model->applyWhiteBalance(index);
    updateDisplay();
}

void MainWindow::on_btnApplyKMeans_clicked()
{
    int k = ui->sliderK->value();
    model->applyKMeans(k);
    updateDisplay();
}

void MainWindow::on_sliderK_valueChanged(int value)
{
    ui->labelKValue->setText(QString::number(value));
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
    currentMorphMethod = 0;
    int val = ui->sliderMorphSize->value();
    int shape = ui->comboKernelShape->currentIndex();
    model->applyMorphology(0, val, shape);
    updateDisplay();
}

void MainWindow::on_btnDilation_clicked()
{
    currentMorphMethod = 1;
    int val = ui->sliderMorphSize->value();
    int shape = ui->comboKernelShape->currentIndex();
    model->applyMorphology(1, val, shape);
    updateDisplay();
}

void MainWindow::on_btnNoise_clicked()
{
    currentMorphMethod = 2;
    int val = ui->sliderMorphSize->value();
    int shape = ui->comboKernelShape->currentIndex();
    model->applyMorphology(2, val, shape);
    updateDisplay();
}

void MainWindow::on_btnClosing_clicked()
{
    currentMorphMethod = 3;
    int val = ui->sliderMorphSize->value();
    int shape = ui->comboKernelShape->currentIndex();
    model->applyMorphology(3, val, shape);
    updateDisplay();
}

void MainWindow::on_btnGradient_clicked()
{
    currentMorphMethod = 4;
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
    model->applyMorphology(currentMorphMethod, value, shape);
    updateDisplay();
}

void MainWindow::on_comboKernelShape_currentIndexChanged(int index)
{
    int val = ui->sliderMorphSize->value();
    model->applyMorphology(currentMorphMethod, val, index);
    updateDisplay();
}

bool MainWindow::eventFilter(QObject* watched, QEvent* event) {
    if (watched == ui->imageLabel && isGrabCutMode) {

        if (event->type() == QEvent::MouseButtonPress) {
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
            if (mouseEvent->button() == Qt::LeftButton) {
                isDrawingRect = true;
                startPoint = mouseEvent->pos();
                endPoint = startPoint;
                return true;
            }
        }

        else if (event->type() == QEvent::MouseMove && isDrawingRect) {
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
            endPoint = mouseEvent->pos();
            updateDisplayWithRect();
            return true;
        }

        else if (event->type() == QEvent::MouseButtonRelease && isDrawingRect) {
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
            isDrawingRect = false;
            endPoint = mouseEvent->pos();

            ui->imageLabel->setCursor(Qt::ArrowCursor);

            int labelW = ui->imageLabel->width();
            int labelH = ui->imageLabel->height();

            int imgW = model->currentImage.cols;
            int imgH = model->currentImage.rows;

            if (imgW == 0 || imgH == 0) return true;

            double ratioX = (double)labelW / imgW;
            double ratioY = (double)labelH / imgH;
            double actualRatio = std::min(ratioX, ratioY);

            int actualDisplayW = imgW * actualRatio;
            int actualDisplayH = imgH * actualRatio;

            int offsetX = (labelW - actualDisplayW) / 2;
            int offsetY = (labelH - actualDisplayH) / 2;

            int displayX = std::min(startPoint.x(), endPoint.x());
            int displayY = std::min(startPoint.y(), endPoint.y());
            int displayW = std::abs(startPoint.x() - endPoint.x());
            int displayH = std::abs(startPoint.y() - endPoint.y());

            displayX -= offsetX;
            displayY -= offsetY;

            if (displayW > 10 && displayH > 10) {
                int realX = displayX / actualRatio;
                int realY = displayY / actualRatio;
                int realW = displayW / actualRatio;
                int realH = displayH / actualRatio;

                realX = std::max(0, realX);
                realY = std::max(0, realY);
                realW = std::min(realW, imgW - realX);
                realH = std::min(realH, imgH - realY);

                grabCutRect = cv::Rect(realX, realY, realW, realH);

                model->applyGrabCut(grabCutRect);
                updateDisplay();

                isGrabCutMode = false;
            }
            else {
                updateDisplay();
            }
            return true;
        }
    }
    return QMainWindow::eventFilter(watched, event);
}

void MainWindow::updateDisplayWithRect() {
    if (model->currentImage.empty()) return;

    int labelW = ui->imageLabel->width();
    int labelH = ui->imageLabel->height();

    QImage img((const unsigned char*)(model->currentImage.data),
        model->currentImage.cols, model->currentImage.rows,
        model->currentImage.step, QImage::Format_RGB888);

    QPixmap originalPixmap = QPixmap::fromImage(img.rgbSwapped());

    QPixmap scaledPixmap = originalPixmap.scaled(labelW, labelH, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    int imgW = model->currentImage.cols;
    int imgH = model->currentImage.rows;
    double actualRatio = std::min((double)labelW / imgW, (double)labelH / imgH);
    int actualDisplayW = imgW * actualRatio;
    int actualDisplayH = imgH * actualRatio;
    int offsetX = (labelW - actualDisplayW) / 2;
    int offsetY = (labelH - actualDisplayH) / 2;

    QImage drawingImage = scaledPixmap.toImage();
    QPainter painter(&drawingImage);
    painter.setPen(QPen(Qt::red, 2, Qt::DashLine));

    int x = std::min(startPoint.x(), endPoint.x()) - offsetX;
    int y = std::min(startPoint.y(), endPoint.y()) - offsetY;
    int width = std::abs(startPoint.x() - endPoint.x());
    int height = std::abs(startPoint.y() - endPoint.y());

    x = std::max(0, x);
    y = std::max(0, y);
    width = std::min(width, actualDisplayW - x);
    height = std::min(height, actualDisplayH - y);

    painter.drawRect(x, y, width, height);

    QPixmap finalPixmap(labelW, labelH);
    finalPixmap.fill(Qt::black);
    QPainter finalPainter(&finalPixmap);
    finalPainter.drawPixmap(offsetX, offsetY, QPixmap::fromImage(drawingImage));

    ui->imageLabel->setPixmap(finalPixmap);
}

void MainWindow::on_btnCanny_clicked()
{
    int t1 = ui->sliderCannyThreshold1->value();
    int t2 = ui->sliderCannyThreshold2->value();

    model->applyCanny(t1, t2, 3);
    updateDisplay();
}
void MainWindow::on_sliderCannyThreshold1_valueChanged(int value)
{
    ui->labelCannyT1Value->setText(QString::number(value));
    on_btnCanny_clicked();
}

void MainWindow::on_sliderCannyThreshold2_valueChanged(int value)
{
    ui->labelCannyT2Value->setText(QString::number(value));
    on_btnCanny_clicked();
}
void MainWindow::on_btnPencilSketch_clicked()
{
    model->applyPencilSketch();
    updateDisplay();
}

void MainWindow::on_btnCartoon_clicked()
{
    model->applyCartoonEffect();
    updateDisplay();
}

void MainWindow::on_btnRotate_clicked()
{
    model->applyRotate(15.0);
    updateDisplay();
}

void MainWindow::on_btnAffine_clicked()
{
    model->applyAffineTransform();
    updateDisplay();
}

void MainWindow::on_btnPerspective_clicked()
{
    model->applyPerspectiveTransform();
    updateDisplay();
}

void MainWindow::on_btnGrabCut_clicked()
{
    isGrabCutMode = true;
    ui->imageLabel->setCursor(Qt::CrossCursor);
}