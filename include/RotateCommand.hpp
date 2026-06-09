#ifndef ROTATE_COMMAND_HPP
#define ROTATE_COMMAND_HPP

#include "Command.hpp"
#include "EditorModel.hpp"
#include <opencv2/opencv.hpp>

class RotateCommand : public Command {
    EditorModel* model;
    cv::Mat prevImage;
    double angle;

public:
    RotateCommand(EditorModel* m, double a)
        : model(m), angle(a) {
        prevImage = model->currentImage.clone();
    }

    void execute() override {
        model->applyRotate(angle);
    }

    void undo() override {
        model->setCurrentImage(prevImage);
    }
};

#endif