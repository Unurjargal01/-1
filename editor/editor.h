#pragma once

#include <cstddef>
#include <memory>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

class Command;  // Forward declaration

class CommandHistory {
public:
    CommandHistory(){};

    void Push(std::shared_ptr<Command>& command) {
        history_.push_back(command);
    }

    std::shared_ptr<Command> Pop() {
        std::shared_ptr<Command> tail = history_.back();
        history_.pop_back();
        return tail;
    }

    size_t Size() {
        return history_.size();
    }

private:
    std::vector<std::shared_ptr<Command>> history_;
};

class Command : public std::enable_shared_from_this<Command> {
public:
    virtual bool Execute() const = 0;
    virtual bool Undo() const = 0;
    virtual ~Command() = default;
};

class Editor {
public:
    friend class TypeCommand;
    friend class ShiftLeftCommand;
    friend class ShiftRightCommand;
    friend class BackSpaceCommand;
    // Iterator
    class TextView {
    public:
        const std::string& before_cursor_;
        const std::string& after_cursor_reversed_;
        size_t pos_, cnt_;

        TextView(const std::string& before_cursor, const std::string& after_cursor, size_t pos,
                 size_t cnt)
            : before_cursor_(before_cursor),
              after_cursor_reversed_(after_cursor),
              pos_(pos),
              cnt_(cnt){};

        class Iterator {
        public:
            Iterator(const TextView& textview, size_t pos) : textview_(textview), pos_(pos) {
            }

            bool operator!=(const Iterator& other) const {
                return pos_ != other.pos_;
            }

            char operator*() const {
                if (pos_ < textview_.before_cursor_.size()) {
                    return textview_.before_cursor_[pos_];
                } else {
                    return textview_
                        .after_cursor_reversed_[textview_.after_cursor_reversed_.size() - pos_ +
                                                textview_.before_cursor_.size() - 1];
                }
            }

            void operator++() {
                ++pos_;
            }

        private:
            const TextView& textview_;
            size_t pos_;
        };

        Iterator begin() const {
            return Iterator(*this, pos_);
        }

        Iterator end() const {
            return Iterator(*this, pos_ + cnt_);
        }
    };

    void Type(char symbol);

    void Undo();
    void ShiftLeft();
    void ShiftRight();
    void Backspace();
    void Redo();

    void Print() {
        std::cout << before_cursor_ << std::endl;
        std::cout << after_cursor_reversed_ << std::endl;
    }

    size_t Size() const {
        return before_cursor_.size() + after_cursor_reversed_.size();
    }

    TextView GetText(size_t pos, size_t count) const {
        return TextView{before_cursor_, after_cursor_reversed_, pos, count};
    }

    size_t GetPosition() const {
        return before_cursor_.size();
    }

private:
    std::string before_cursor_;
    std::string after_cursor_reversed_;
    CommandHistory history_;
    size_t pos_;
    std::vector<std::shared_ptr<Command>> redo_;
};

// ConcreteCommand
class TypeCommand : public Command {
public:
    TypeCommand(Editor& editor, char c) : editor_(editor), c_(c){};

    bool Execute() const override {
        editor_.before_cursor_ += c_;
        return true;
    }

    bool Undo() const override {
        editor_.before_cursor_.pop_back();
        return true;
    }

private:
    Editor& editor_;
    char c_;
};

class ShiftLeftCommand : public Command {
public:
    ShiftLeftCommand(Editor& editor) : editor_(editor){};

    bool Execute() const override {
        if (!editor_.before_cursor_.empty()) {
            char tail = editor_.before_cursor_.back();
            editor_.before_cursor_.pop_back();
            editor_.after_cursor_reversed_.push_back(tail);
            return true;
        }
        return false;
    }

    bool Undo() const override {
        char tail = editor_.after_cursor_reversed_.back();
        editor_.after_cursor_reversed_.pop_back();
        editor_.before_cursor_.push_back(tail);
        return true;
    }

private:
    Editor& editor_;
};

class ShiftRightCommand : public Command {
public:
    ShiftRightCommand(Editor& editor) : editor_(editor){};

    bool Execute() const override {
        if (!editor_.after_cursor_reversed_.empty()) {
            char tail = editor_.after_cursor_reversed_.back();
            editor_.after_cursor_reversed_.pop_back();
            editor_.before_cursor_.push_back(tail);
            return true;
        }
        return false;
    }

    bool Undo() const override {
        char tail = editor_.before_cursor_.back();
        editor_.before_cursor_.pop_back();
        editor_.after_cursor_reversed_.push_back(tail);
        return true;
    }

private:
    Editor& editor_;
};

class BackSpaceCommand : public Command {
public:
    BackSpaceCommand(Editor& editor) : editor_(editor) {
        if (!editor_.before_cursor_.empty()) {
            c_ = editor_.before_cursor_.back();
        }
    };

    bool Execute() const override {
        if (!editor_.before_cursor_.empty()) {
            editor_.before_cursor_.pop_back();
            return true;
        }
        return false;
    };

    bool Undo() const override {
        editor_.before_cursor_.push_back(c_);
        return true;
    }

private:
    Editor& editor_;
    char c_;
};

// Editor Functions
void Editor::Type(char symbol) {
    std::shared_ptr<Command> cmd = std::make_shared<TypeCommand>(*this, symbol);
    if (cmd->Execute()) {
        history_.Push(cmd);
        redo_.clear();
    }
}

void Editor::ShiftLeft() {
    std::shared_ptr<Command> cmd = std::make_shared<ShiftLeftCommand>(*this);
    if (cmd->Execute()) {
        history_.Push(cmd);
        redo_.clear();
    }
}

void Editor::ShiftRight() {
    std::shared_ptr<Command> cmd = std::make_shared<ShiftRightCommand>(*this);
    if (cmd->Execute()) {
        history_.Push(cmd);
        redo_.clear();
    }
}

void Editor::Undo() {
    if (history_.Size() != 0) {
        std::shared_ptr<Command> cmd = history_.Pop();
        redo_.push_back(cmd);
        cmd->Undo();
    }
}

void Editor::Backspace() {
    std::shared_ptr<Command> cmd = std::make_shared<BackSpaceCommand>(*this);
    if (cmd->Execute()) {
        history_.Push(cmd);
        redo_.clear();
    }
}

void Editor::Redo() {
    if (!redo_.empty()) {
        std::shared_ptr<Command> cmd = redo_.back();
        cmd->Execute();
        redo_.pop_back();
    }
}