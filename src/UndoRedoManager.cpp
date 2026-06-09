#include "UndoRedoManager.hpp"

void UndoRedoManager::executeCommand(std::shared_ptr<Command> cmd) {
    cmd->execute();
    undoStack.push(cmd);
    while (!redoStack.empty()) redoStack.pop();
}

void UndoRedoManager::undo() {
    if (undoStack.empty()) return;
    auto cmd = undoStack.top();
    undoStack.pop();
    cmd->undo();
    redoStack.push(cmd);
}

void UndoRedoManager::redo() {
    if (redoStack.empty()) return;
    auto cmd = redoStack.top();
    redoStack.pop();
    cmd->execute();
    undoStack.push(cmd);
}