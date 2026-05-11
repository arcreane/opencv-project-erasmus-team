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

