#ifndef EDITOR_MODEL_HPP
#define EDITOR_MODEL_HPP

#include <opencv2/opencv.hpp>

class EditorModel {
public:
  cv::Mat currentImage;
  cv::Mat processedImage;

  bool loadImage(const std::string &path);
  void applyThreshold(int method, int threshValue);

  void applyGlobalEqualization();
  void applyCLAHE(double clipLimit, int gridSize);

  void applyMorphology(int method, int kernelSize, int shapeType);

  void applyGrabCut(cv::Rect rectangle);

  void applyWhiteBalance(int method);
};

#endif
