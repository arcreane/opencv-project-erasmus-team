#ifndef PERSPECTIVE_COMMAND_HPP
#define PERSPECTIVE_COMMAND_HPP

#include "Command.hpp"
#include "EditorModel.hpp"
#include <opencv2/opencv.hpp>

class PerspectiveCommand : public Command {
    EditorModel* model;
    cv::Mat prevImage;

public:
    PerspectiveCommand(EditorModel* m)
        : model(m) {
        prevImage = model->currentImage.clone();
    }

    void execute() override {
        model->applyPerspectiveTransform();
    }

    void undo() override {
        model->setCurrentImage(prevImage);
    }
};

#endif