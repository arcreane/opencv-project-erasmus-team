# Opencv Erasmus Project README

## Overview
Opencv Erasmus is a C++/Qt desktop image editor powered by OpenCV. It provides core image processing operations and a GUI to interactively modify images. The application supports various filters, geometric transformations, segmentation, and panorama stitching.

## Project Structure
- `main.cpp`: Program entry point and Qt application initialization.
- `EditorModel.hpp/cpp`: Core image processing functions (threshold, white balance, filters, morphology, Canny, sketch/cartoon effects, rotation, affine/perspective transforms, panorama stitching).
- `MainWindow.hpp/cpp`: GUI implementation including slots for buttons, sliders, event filters for interactive tools, and integration with EditorModel.
- `mainwindow.ui`: Qt Designer UI layout with QLabel for image display, sliders, buttons, and group boxes for different operations.

## EditorModel Functions
- `bool loadImage(const std::string& path)`: Loads an image into memory.
- `void applyThreshold(int method, int threshValue)`: Apply binary, Otsu, or adaptive threshold.
- `void applyWhiteBalance(int method)`: Applies selected white balance method (Gray World or Simple).
- `void applyGlobalEqualization()`: Perform global histogram equalization.
- `void applyCLAHE(double clipLimit, int gridSize)`: Adaptive histogram equalization.
- `void applyMorphology(int operation, int size, int shape)`: Erosion, Dilation, Opening, Closing, or Gradient morphological operations.
- `void applyCanny(int threshold1, int threshold2, int apertureSize)`: Edge detection using Canny.
- `void applyPencilSketch()`: Converts the image to a pencil sketch effect.
- `void applyCartoonEffect()`: Applies a cartoon effect to the image.
- `void applyRotate(double angle)`: Rotate image by a given angle.
- `void applyAffineTransform()`: Apply affine transformation using preset points.
- `void applyPerspectiveTransform()`: Apply perspective warp using preset points.
- `void applyGrabCut(cv::Rect rect)`: Segments foreground using GrabCut based on a rectangle.
- `bool stitchImages(const std::vector<std::string>& imagePaths, cv::Mat& outputPanorama)`: Stitch multiple overlapping images into a panorama.

## MainWindow Slots / GUI Functions
- `on_actionOpen_triggered()`: Open image from file dialog and display it.
- `on_actionSave_triggered()`: Save processed image to disk.
- `updateDisplay()`: Refresh QLabel to show `processedImage`.
- `updateDisplayWithRect()`: Show image with a temporary rectangle overlay for GrabCut.
- `on_btnApplyWB_clicked()`: Apply white balance based on combo box selection.
- `on_btnApplyKMeans_clicked()`: Apply K-Means clustering with slider value.
- `on_sliderK_valueChanged(int)`: Update label for K value.
- `on_btnBinary_clicked()`, `on_btnOtsu_clicked()`, `on_btnAdaptive_clicked()`: Apply threshold methods and update display.
- `on_sliderThresh_valueChanged(int)`: Update threshold value in label and apply threshold.
- `on_buttonGlobalEq_clicked()`: Apply global histogram equalization.
- `on_sliderClipLimit_valueChanged(int)`, `on_sliderGridSize_valueChanged(int)`: Update CLAHE parameters and apply adaptive equalization.
- `on_btnErosion_clicked()`, `on_btnDilation_clicked()`, `on_btnNoise_clicked()`, `on_btnClosing_clicked()`, `on_btnGradient_clicked()`: Apply morphological operations based on current slider and shape.
- `on_sliderMorphSize_valueChanged(int)`, `on_comboKernelShape_currentIndexChanged(int)`: Update morphology parameters and reapply operation.
- `eventFilter(QObject*, QEvent*)`: Handle interactive GrabCut mouse events on `imageLabel`. Drag rectangle for segmentation.
- `on_btnCanny_clicked()`: Apply Canny edge detection using current slider thresholds.
- `on_sliderCannyThreshold1_valueChanged(int)`, `on_sliderCannyThreshold2_valueChanged(int)`: Update labels and reapply Canny.
- `on_btnPencilSketch_clicked()`, `on_btnCartoon_clicked()`: Apply creative effects and update display.
- `on_btnRotate_clicked()`, `on_btnAffine_clicked()`, `on_btnPerspective_clicked()`: Apply geometric transformations.
- `on_btnGrabCut_clicked()`: Enable interactive GrabCut mode.
- `on_stitchButton_clicked()`: Open file dialog for multiple images, create panorama using EditorModel, and display it in `imageLabel`.

## UI Elements
- `imageLabel`: Main QLabel for displaying images and results.
- `sliders`: Control parameters for threshold, Canny, K-Means, CLAHE, morphology, etc.
- `combo boxes`: Select methods for white balance, kernel shape, etc.
- `buttons`: Trigger image processing functions (rotate, affine, perspective, Canny, threshold, sketch, cartoon, GrabCut, stitching).
- `group boxes`: Organize controls into logical sections: Geometric Transforms, Color & Contrast, Thresholding & Edges, Morphology, Advanced Segmentation, Creative Effects, Stitching.

## How to Build and Run
1. Use `qmake` or CMake to configure the project.
2. Build the project in Qt Creator or using `make`.
3. Run the application. Use the GUI buttons and sliders to load images and apply effects.