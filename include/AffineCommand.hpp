#ifndef AFFINE_COMMAND_HPP
#define AFFINE_COMMAND_HPP

#include "Command.hpp"
#include "EditorModel.hpp"
#include <opencv2/opencv.hpp>

class AffineCommand : public Command {
    EditorModel* model;
    cv::Mat prevImage;

public:
    AffineCommand(EditorModel* m)
        : model(m) {
        prevImage = model->currentImage.clone();
    }

    void execute() override {
        model->applyAffineTransform();
    }

    void undo() override {
        model->setCurrentImage(prevImage);
    }
};

#endif