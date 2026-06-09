#ifndef MORPHOLOGY_COMMAND_HPP
#define MORPHOLOGY_COMMAND_HPP

#include "Command.hpp"
#include "EditorModel.hpp"
#include <opencv2/opencv.hpp>

class MorphologyCommand : public Command {
    EditorModel* model;
    cv::Mat prevImage;
    int method;
    int kernelSize;
    int shapeType;

public:
    MorphologyCommand(EditorModel* m, int meth, int kSize, int shape)
        : model(m), method(meth), kernelSize(kSize), shapeType(shape) {
        prevImage = model->currentImage.clone();
    }

    void execute() override {
        model->applyMorphology(method, kernelSize, shapeType);
    }

    void undo() override {
        model->setCurrentImage(prevImage);
    }
};

#endif