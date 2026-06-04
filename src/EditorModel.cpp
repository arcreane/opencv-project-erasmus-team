 #include "EditorModel.hpp"
#include <opencv2/xphoto.hpp>

bool EditorModel::loadImage(const std::string& path) {
   currentImage = cv::imread(path);
   return !currentImage.empty();
}

void EditorModel::applyThreshold(int method, int threshValue) {
   if (currentImage.empty()) return;


   cv::Mat gray;
   cv::cvtColor(currentImage, gray, cv::COLOR_BGR2GRAY);


   if (method == 0)      cv::threshold(gray, processedImage, threshValue, 255, cv::THRESH_BINARY);
   else if (method == 1) cv::threshold(gray, processedImage, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
   else if (method == 2) cv::adaptiveThreshold(gray, processedImage, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 11, 2);
}

void EditorModel::applyGlobalEqualization() {
    if (currentImage.empty()) return;

    cv::Mat gray;

    if (currentImage.channels() == 3) {
        cv::cvtColor(currentImage, gray, cv::COLOR_BGR2GRAY);
    }
    else {
        gray = currentImage.clone();
    }

    cv::equalizeHist(gray, processedImage);
}

void EditorModel::applyCLAHE(double clipLimit, int gridSize) {
    if (currentImage.empty()) return;

    cv::Mat gray;
    if (currentImage.channels() == 3) {
        cv::cvtColor(currentImage, gray, cv::COLOR_BGR2GRAY);
    }
    else {
        gray = currentImage.clone();
    }

    cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE(clipLimit, cv::Size(gridSize, gridSize));

    clahe->apply(gray, processedImage);
}

void EditorModel::applyMorphology(int method, int kernelSize, int shapeType)
{
    if (currentImage.empty()) return;

    cv::Mat sourceForMorph;

    if (!processedImage.empty() && processedImage.channels() == 1) {
        sourceForMorph = processedImage.clone();
    }

    else {
        cv::cvtColor(currentImage, sourceForMorph, cv::COLOR_BGR2GRAY);
    }

    int cvShape = cv::MORPH_RECT;
    if (shapeType == 1) cvShape = cv::MORPH_CROSS;
    else if (shapeType == 2) cvShape = cv::MORPH_ELLIPSE;

    int size = 2 * kernelSize + 1;
    cv::Mat element = cv::getStructuringElement(cvShape, cv::Size(size, size));

    if (method == 0) {
        cv::erode(sourceForMorph, processedImage, element);
    }
    else if (method == 1) {
        cv::dilate(sourceForMorph, processedImage, element);
    }
    else if (method == 2) {
        cv::morphologyEx(sourceForMorph, processedImage, cv::MORPH_OPEN, element);
    }
    else if (method == 3) {
        cv::morphologyEx(sourceForMorph, processedImage, cv::MORPH_CLOSE, element);
    }
    else if (method == 4) {
        cv::morphologyEx(sourceForMorph, processedImage, cv::MORPH_GRADIENT, element);
    }
}

void EditorModel::applyGrabCut(cv::Rect rectangle) {
    if (currentImage.empty()) return;

    rectangle.x = std::max(0, rectangle.x);
    rectangle.y = std::max(0, rectangle.y);
    rectangle.width = std::min(rectangle.width, currentImage.cols - rectangle.x);
    rectangle.height = std::min(rectangle.height, currentImage.rows - rectangle.y);

    if (rectangle.width <= 10 || rectangle.height <= 10) return;

    cv::Mat mask;
    cv::Mat bgdModel, fgdModel;

    cv::grabCut(currentImage, mask, rectangle, bgdModel, fgdModel, 5, cv::GC_INIT_WITH_RECT);

    cv::Mat binaryMask = (mask == cv::GC_FGD) | (mask == cv::GC_PR_FGD);

    cv::Mat colorResult = cv::Mat::zeros(currentImage.size(), currentImage.type());

    currentImage.copyTo(colorResult, binaryMask);

    processedImage = colorResult;
}

void EditorModel::applyWhiteBalance(int method) {
    if (currentImage.empty()) return;

    if (currentImage.channels() != 3) {
        processedImage = currentImage.clone();
        return;
    }

    if (method == 0) {
        cv::Ptr<cv::xphoto::WhiteBalancer> wb = cv::xphoto::createGrayworldWB();
        wb->balanceWhite(currentImage, processedImage);
    }
    else if (method == 1) {
        cv::Ptr<cv::xphoto::WhiteBalancer> wb = cv::xphoto::createSimpleWB();
        wb->balanceWhite(currentImage, processedImage);
    }
}

void EditorModel::applyKMeans(int k) {
    if (currentImage.empty()) return;

    if (currentImage.channels() != 3) {
        processedImage = currentImage.clone();
        return;
    }

    cv::Mat data;
    currentImage.convertTo(data, CV_32F);

    cv::Mat reshaped = data.reshape(1, currentImage.rows * currentImage.cols);

    cv::TermCriteria criteria(cv::TermCriteria::EPS + cv::TermCriteria::COUNT, 10, 1.0);

    cv::Mat labels;
    cv::Mat centers;
    cv::kmeans(reshaped, k, labels, criteria, 3, cv::KMEANS_PP_CENTERS, centers);

    cv::Mat segmentedImage(currentImage.size(), currentImage.type());
    for (int i = 0; i < reshaped.rows; ++i) {
        int clusterId = labels.at<int>(i);
        int y = i / currentImage.cols;
        int x = i % currentImage.cols;
        segmentedImage.at<cv::Vec3b>(y, x) = cv::Vec3b(
            static_cast<uchar>(centers.at<float>(clusterId, 0)),
            static_cast<uchar>(centers.at<float>(clusterId, 1)),
            static_cast<uchar>(centers.at<float>(clusterId, 2))
        );
    }

    processedImage = segmentedImage;
}