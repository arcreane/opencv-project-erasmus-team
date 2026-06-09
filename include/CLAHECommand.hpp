#ifndef CLAHE_COMMAND_HPP
#define CLAHE_COMMAND_HPP

#include "Command.hpp"
#include "EditorModel.hpp"
#include <opencv2/opencv.hpp>

class CLAHECommand : public Command {
    EditorModel* model;
    cv::Mat prevImage;
    double clipLimit;
    int gridSize;

public:
    CLAHECommand(EditorModel* m, double clip, int grid)
        : model(m), clipLimit(clip), gridSize(grid) {
        prevImage = model->currentImage.clone();
    }

    void execute() override {
        model->applyCLAHE(clipLimit, gridSize);
    }

    void undo() override {
        model->setCurrentImage(prevImage);
    }
};

#endif