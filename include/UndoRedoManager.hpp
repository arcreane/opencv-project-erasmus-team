#ifndef UNDOREDO_MANAGER_HPP
#define UNDOREDO_MANAGER_HPP

#include <stack>
#include <memory>
#include "Command.hpp"

class UndoRedoManager {
private:
    std::stack<std::shared_ptr<Command>> undoStack;
    std::stack<std::shared_ptr<Command>> redoStack;

public:
    void executeCommand(std::shared_ptr<Command> cmd);
    void undo();
    void redo();
};

#endif