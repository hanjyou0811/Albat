#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <cassert>
#include "../src/Albat/albat.h"
#include "../src/Utils/stringutils.h"

std::string update_operator(Albat* albat, std::string &code, const std::string &operator_symbol) {
  struct OperatorInfo {
      std::string prefix; 
      std::string funcName;
  };

  static const std::map<std::string, OperatorInfo> operatorMap = {
      {"**", {" (_pow(", "_pow"}},
      {"/+", {" (_div(", "_div"}},
      {"%+", {" (_mod(", "_mod"}}
  };

  if (operatorMap.find(operator_symbol) == operatorMap.end()) {
      return code;
  }

  const std::string &prefix = operatorMap.at(operator_symbol).prefix;
  const std::string suffix = ")) "; 
  
  int searchPos = 0;
  
  while (true) {
      int operatorPos = StringUtils::strpos_exlit(code, operator_symbol, searchPos);
      if (operatorPos < 0) {
          break;
      }
      
      int leftExprEndPos = operatorPos - 1;
      
      while (leftExprEndPos > 0 && isspace(code[leftExprEndPos])) {
          leftExprEndPos--;
      }
      
      if (leftExprEndPos < 0 || StringUtils::isOperator(code[leftExprEndPos])) {
          searchPos = operatorPos + 1;
          continue;
      }
      
      bool isTypeName = false;
      for (int j = leftExprEndPos; j >= 0 && j >= leftExprEndPos - 100; j--) {
          std::string potentialType = code.substr(j, leftExprEndPos - j + 1);
          if (albat->isValidtypename(potentialType, code[leftExprEndPos + 1])) {
              isTypeName = true;
              break;
          }
      }
      
      if (isTypeName) {
          searchPos = operatorPos + 1;
          continue;
      }
      
      int rightExprStartPos = operatorPos + operator_symbol.length();
      
      while (rightExprStartPos < code.size() && isspace(code[rightExprStartPos])) {
          rightExprStartPos++;
      }
      
      if (rightExprStartPos >= code.size()) {
          break;
      }
      
      auto [leftIndex, leftLen] = albat->getExpressionIndexAndLength(code, leftExprEndPos, false);
      auto [rightIndex, rightLen] = albat->getExpressionIndexAndLength(code, rightExprStartPos, true);
      
      int leftExprBegin = leftExprEndPos - leftLen + 1;
      int rightExprEnd = rightExprStartPos + rightLen - 1;
      
      std::string before = code.substr(0, leftExprBegin);
      std::string leftExpr = code.substr(leftExprBegin, leftLen);
      std::string rightExpr = code.substr(rightExprStartPos, rightLen);
      std::string after = code.substr(rightExprEnd + 1);
      
      StringUtils::trim(leftExpr);
      StringUtils::trim(rightExpr);
      
      code = before + prefix + leftExpr + ", " + rightExpr + suffix + after;
      
      searchPos = 0;
  }
  
  return code;
}

int findExpressionEnd(const std::string &code, int startPos);
int findVariableStart(const std::string &code, int operatorPos);
std::string update_operator_equal(std::string &code, const std::string &operatorSymbol) {
  struct OperatorInfo {
      std::string funcName; 
      std::string libName; 
  };

  static const std::map<std::string, OperatorInfo> operatorMap = {
      {"**=", {"_pow", "pow"}},
      {"/+=", {"_div", "_div"}},
      {"%+=", {"_mod", "_mod"}}
  };
  while(1)
  {
    if (operatorMap.find(operatorSymbol) == operatorMap.end()) {
        return code;
    }

    const std::string &funcName = operatorMap.at(operatorSymbol).funcName;
    const std::string &libName = operatorMap.at(operatorSymbol).libName;

    int operatorPos = StringUtils::strpos_exlit(code, operatorSymbol);
    if (operatorPos < 0) {
        return code;
    }

    std::string afterOperator = ""; 
    int endOfRightExpr = findExpressionEnd(code, operatorPos + operatorSymbol.length());
    
    if (endOfRightExpr < code.length()) {
        afterOperator = code.substr(endOfRightExpr);
        code = code.substr(0, endOfRightExpr);
    }

    int leftVarStart = findVariableStart(code, operatorPos);
    std::string before = code.substr(0, leftVarStart);
    std::string leftVar = code.substr(leftVarStart, operatorPos - (leftVarStart));
    std::string rightExpr = code.substr(operatorPos + operatorSymbol.length());
    rightExpr = rightExpr.substr(0, rightExpr.length());
    StringUtils::trim(before);
    StringUtils::trim(leftVar);
    StringUtils::trim(rightExpr);
    
    std::string result = before + leftVar + " = " + funcName + "(" + leftVar + ", " + rightExpr + ")" + afterOperator;
    
    return result;
  }
}

  int findExpressionEnd(const std::string &code, int startPos) {
    int bracketCount = 0;
    int squareCount = 0;
    int braceCount = 0;
    bool inSingleQuote = false;
    bool inDoubleQuote = false;
    
    for (int i = startPos; i < code.length(); ++i) {
        if ((inSingleQuote || inDoubleQuote) && code[i] == '\\') {
            i++;
            continue;
        }
        
        if (!inSingleQuote && !inDoubleQuote) {
            if (code[i] == '(') bracketCount++;
            if (code[i] == ')') bracketCount--;
            if (code[i] == '[') squareCount++;
            if (code[i] == ']') squareCount--;
            if (code[i] == '{') braceCount++;
            if (code[i] == '}') braceCount--;
        }
        
        if (!inDoubleQuote && code[i] == '\'') inSingleQuote = !inSingleQuote;
        if (!inSingleQuote && code[i] == '"')  inDoubleQuote = !inDoubleQuote;
        
        if (bracketCount < 0 || squareCount < 0 || braceCount < 0) {
            return i;
        }
        
        if (bracketCount == 0 && squareCount == 0 && braceCount == 0 && 
            !inSingleQuote && !inDoubleQuote && 
            (code[i] == ';' || code[i] == ',')) {
            return i;
        }
    }
    
  return code.length();
}

int findVariableStart(const std::string &code, int operatorPos) {
  int bracketCount = 0; 
  int squareCount = 0; 
  int braceCount = 0; 
  bool inSingleQuote = false;
  bool inDoubleQuote = false;
  
  for (int i = operatorPos - 1; i >= 0; --i)
  {
      if (i >= 1 && inSingleQuote && code[i-1] == '/' && code[i] == '\'')
      {
          i -= 2;
          continue;
      }
      
      if (!inDoubleQuote && code[i] == '\'') inSingleQuote = !inSingleQuote;
      if (!inSingleQuote && code[i] == '"')  inDoubleQuote = !inDoubleQuote;
      
      if (!inSingleQuote && !inDoubleQuote)
      {
          if (code[i] == ')') bracketCount++;
          if (code[i] == '(') bracketCount--;
          if (code[i] == '}') squareCount++;
          if (code[i] == '{') squareCount--;
          if (code[i] == ']') braceCount++;
          if (code[i] == '[') braceCount--;
      }
      
      if (bracketCount < 0 || squareCount < 0 || braceCount < 0)
      {
          std::string tmp = code;
          int pairPos = StringUtils::find_pairBracket1(tmp, i);
          if (pairPos >= 0)
          {
              return pairPos;
          }
      }
      
      if ((code[i] == '=' || code[i] == ',') &&
          bracketCount == 0 && squareCount == 0 && braceCount == 0 &&
          !inSingleQuote && !inDoubleQuote)
      {
          return i + 1;
      }
  }
  
  return 0;
}

// メイン関数
int main() {
    // Albatオブジェクトの初期化
    Albat* albat = new Albat();
    
    std::string code = "int a = 2 ** 3;\n";
    std::cout << "元のコード: " << code << std::endl;
    code = update_operator(albat, code, "**");
    std::cout << "更新後のコード: " << code << std::endl;
    code = "a **= 3;";
    std::cout << "元のコード: " << code << std::endl;
    code = update_operator_equal(code, "**=");
    std::cout << "更新後のコード: " << code << std::endl;
    delete albat;

    return 0;
}