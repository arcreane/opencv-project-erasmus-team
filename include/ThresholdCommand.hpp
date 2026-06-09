#ifndef THRESHOLD_COMMAND_HPP
#define THRESHOLD_COMMAND_HPP

#include "Command.hpp"
#include "EditorModel.hpp"
#include <opencv2/opencv.hpp>

class ThresholdCommand : public Command {
    EditorModel* model;
    cv::Mat prevImage;
    int method, value;

public:
    ThresholdCommand(EditorModel* m, int method_, int value_)
        : model(m), method(method_), value(value_) {
        prevImage = model->currentImage.clone();
    }

    void execute() override {
        model->applyThreshold(method, value);
    }

    void undo() override {
        model->setCurrentImage(prevImage);
    }
};

#endif