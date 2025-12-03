#include "Evalvisitor.h"

bool EvalVisitor::AnyToBool(std::any tmp) {
  VariableToVal(tmp);
  if (tmp.type() == typeid(bool)) {
    return std::any_cast<bool>(tmp);
  } else if (tmp.type() == typeid(std::string)) {
    return std::any_cast<std::string>(tmp) != "";
  } else if (tmp.type() == typeid(double)) {
    return std::any_cast<double>(tmp);
  } else if (tmp.type() == typeid(int2048)) {
    return !std::any_cast<int2048>(tmp).CheckZero();
  }
  return (bool)false;
}

double EvalVisitor::AnyToDouble(std::any tmp) {
  VariableToVal(tmp);
  if (tmp.type() == typeid(bool)) {
    return std::any_cast<bool>(tmp) ? 1 : 0;
  } else if (tmp.type() == typeid(std::string)) {
    return StringToDouble(std::any_cast<std::string>(tmp));
  } else if (tmp.type() == typeid(double)) {
    return std::any_cast<double>(tmp);
  } else if (tmp.type() == typeid(int2048)) {
    return std::any_cast<int2048>(tmp);
  }
  return 0;
}

int2048 EvalVisitor::AnyToInt(std::any tmp) {
  VariableToVal(tmp);
  if (tmp.type() == typeid(bool)) {
    return std::any_cast<bool>(tmp);
  } else if (tmp.type() == typeid(std::string)) {
    return int2048(std::any_cast<std::string>(tmp));
  } else if (tmp.type() == typeid(double)) {
    return std::any_cast<double>(tmp);
  } else if (tmp.type() == typeid(int2048)) {
    return std::any_cast<int2048>(tmp);
  }
  return 0;
}

std::string EvalVisitor::AnyToString(std::any tmp) {
  VariableToVal(tmp);
  if (tmp.type() == typeid(bool)) {
    return std::any_cast<bool>(tmp) ? "True" : "False";
  } else if (tmp.type() == typeid(std::string)) {
    return std::any_cast<std::string>(tmp);
  } else if (tmp.type() == typeid(double)) {
    return DoubleToString(std::any_cast<double>(tmp));
  } else if (tmp.type() == typeid(int2048)) {
    return (std::string)(std::any_cast<int2048>(tmp));
  } else if (tmp.type() == typeid(std::pair<std::string, int>)) {
    return "None";
  }
  return "";
}

double EvalVisitor::StringToDouble(const std::string &str) {
  int sz = str.size(), flag = 1, pos = 0;
  double ans = 0, val = 1;
  if (str[pos] == '+') {
    pos++;
  } else if (str[pos] == '-') {
    pos++;
    flag = -1;
  }
  bool point = 0;
  while (pos < sz) {
    if (str[pos] == '.') {
      point = 1;
    } else if (point == 0) {
      ans = ans * 10 + str[pos] - '0';
    } else if (point == 1) {
      val *= 0.1;
      ans += (str[pos] - '0') * val;
    }
    pos++;
  }
  if (ans) {
    ans *= flag;
  }
  return ans;
}

std::string EvalVisitor::DoubleToString(double val) {
  int flag = (val >= 0 ? 1 : -1);
  val = std::abs(val);
  long long tmp = (long long)val;
  std::string ans = "";
  val = val - tmp;
  while (tmp) {
    ans += tmp % 10 + '0';
    tmp /= 10;
  }
  if (ans == "") {
    ans = "0";
  }
  for (int i = 0, j = ans.size() - 1; i < j; i++, j--) {
    std::swap(ans[i], ans[j]);
  }
  ans += '.';
  tmp = val * 1000000 + 0.5;
  for (int i = 1; i <= 6; i++) {
    ans += tmp % 10 + '0';
    tmp /= 10;
  }
  for (int j = ans.size() - 1, i = j - 5; i < j; i++, j--) {
    std::swap(ans[i], ans[j]);
  }
  if (flag == -1) {
    ans = '-' + ans;
  }
  return ans;
}

void EvalVisitor::VariableToVal(std::any &tmp) {
  if (tmp.type() == typeid(std::pair<std::string, int>)) {
    // puts("this this");
    std::pair<std::string, int> temp =
        std::any_cast<std::pair<std::string, int>>(tmp);
    if (temp.second == 0) {
      return;
    }
    tmp = GetValue(temp.first);
  }
}