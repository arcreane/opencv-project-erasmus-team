#ifndef EDITOR_MODEL_HPP
#define EDITOR_MODEL_HPP

#include <opencv2/opencv.hpp>

class EditorModel {
public:
  cv::Mat currentImage;
  cv::Mat processedImage;

  bool loadImage(const std::string &path);
  void applyThreshold(int method, int threshValue);

  bool stitchImages(const std::vector<cv::Mat>& images, cv::Mat& panorama);
  void setCurrentImage(const cv::Mat& img);

  void applyGlobalEqualization();
  void applyCLAHE(double clipLimit, int gridSize);

  void applyMorphology(int method, int kernelSize, int shapeType);

  void applyGrabCut(cv::Rect rectangle);

  void applyWhiteBalance(int method);

  void applyKMeans(int k);

  void applyCanny(int threshold1, int threshold2, int apertureSize);
  void applyPencilSketch();
  void applyCartoonEffect();
  void applyRotate(double angle);
  void applyAffineTransform();
  void applyPerspectiveTransform();
};

#endif
