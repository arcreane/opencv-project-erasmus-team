 #include "EditorModel.hpp"

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

    cv::Mat gray;
    if (currentImage.channels() == 3) {
        cv::cvtColor(currentImage, gray, cv::COLOR_BGR2GRAY);
    }
    else {
        gray = currentImage.clone();
    }

    int cvShape = cv::MORPH_RECT;
    if (shapeType == 1) cvShape = cv::MORPH_CROSS;
    else if (shapeType == 2) cvShape = cv::MORPH_ELLIPSE;

    int size = 2 * kernelSize + 1;
    cv::Mat element = cv::getStructuringElement(cvShape, cv::Size(size, size));

    if (method == 0) {
        cv::erode(gray, processedImage, element);
    }
    else if (method == 1) {
        cv::dilate(gray, processedImage, element);
    }
    else if (method == 2) {
        cv::morphologyEx(gray, processedImage, cv::MORPH_OPEN, element);
    }
    else if (method == 3) {
        cv::morphologyEx(gray, processedImage, cv::MORPH_CLOSE, element);
    }
    else if (method == 4) {
        cv::morphologyEx(gray, processedImage, cv::MORPH_GRADIENT, element);
    }
}

void EditorModel::applyGrabCut(cv::Rect rectangle) {
    if (currentImage.empty()) return;

    cv::Mat mask;
    cv::Mat bgdModel, fgdModel;

    rectangle.x = std::max(0, rectangle.x);
    rectangle.y = std::max(0, rectangle.y);
    rectangle.width = std::min(rectangle.width, currentImage.cols - rectangle.x);
    rectangle.height = std::min(rectangle.height, currentImage.rows - rectangle.y);

    if (rectangle.width <= 5 || rectangle.height <= 5) return;

    cv::grabCut(currentImage, mask, rectangle, bgdModel, fgdModel, 5, cv::GC_INIT_WITH_RECT);

    cv::Mat binaryMask = (mask == cv::GC_FGD) | (mask == cv::GC_PR_FGD);

    cv::Mat colorResult = cv::Mat::zeros(currentImage.size(), currentImage.type());
    currentImage.copyTo(colorResult, binaryMask);

    processedImage = colorResult;

}