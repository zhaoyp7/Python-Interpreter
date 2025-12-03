#include "Evalvisitor.h"

std::any EvalVisitor::GetValue(std::any variable) {
  std::string name = std::any_cast<std::string>(variable);
  if (variables_stack.back().find(name) != variables_stack.back().end()) {
    return variables_stack.back()[name];
  } else {
    return variables_stack.front()[name];
  }
}

void EvalVisitor::SetValue(const std::string &name, std::any val) {
  if (variables_stack.back().find(name) != variables_stack.back().end()) {
    variables_stack.back()[name] = val;
  } else if (variables_stack.front().find(name) !=
             variables_stack.front().end()) {
    variables_stack.front()[name] = val;
  } else {
    variables_stack.back()[name] = val;
  }
}

void EvalVisitor::AddValue(const std::string &name, std::any val) {
  variables_stack.back()[name] = val;
}