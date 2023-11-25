#pragma once

#include <object.h>
#include <tokenizer.h>
#include <error.h>
#include <memory>

std::shared_ptr<Object> ReadList(Tokenizer* tokenizer);

std::shared_ptr<Object> Read(Tokenizer* tokenizer);

std::shared_ptr<Object> ReadCell(Tokenizer* tokenizer);

std::shared_ptr<Object> ReadQuote(Tokenizer* tokenizer, int number_open);