#include "../albat.h"
#include "../../Utils/stringutils.h"
#include <iostream>

std::string Albat::update_operator(std::string &code, const std::string &operator_symbol) {
    struct OperatorInfo {
        std::string prefix; 
        std::string funcName;
    };
  
    static const std::map<std::string, OperatorInfo> operatorMap = {
        {"**", {" (_pow(", "_pow"}},
        {"/+", {" (_div(", "_div"}},
        {"%+", {" (_mod(", "_mod"}}
    };
    
    LibraryManager &libMan = LibraryManager::getInstance();

    if (operatorMap.find(operator_symbol) == operatorMap.end()) {
        return code;
    }
  
    const std::string &prefix = operatorMap.at(operator_symbol).prefix;
    const std::string &funcName = operatorMap.at(operator_symbol).funcName;
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
            if (isValidtypename(potentialType, code[leftExprEndPos + 1])) {
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
        
        auto [leftIndex, leftLen] = getExpressionIndexAndLength(code, leftExprEndPos, false);
        auto [rightIndex, rightLen] = getExpressionIndexAndLength(code, rightExprStartPos, true);
        
        int leftExprBegin = leftExprEndPos - leftLen + 1;
        int rightExprEnd = rightExprStartPos + rightLen - 1;
        
        std::string before = code.substr(0, leftExprBegin);
        std::string leftExpr = code.substr(leftExprBegin, leftLen);
        std::string rightExpr = code.substr(rightExprStartPos, rightLen);
        std::string after = code.substr(rightExprEnd + 1);
        StringUtils::trim(leftExpr);
        StringUtils::trim(rightExpr);
        code = before + prefix + leftExpr + ", " + rightExpr + suffix + after;
        libMan.requestLibrary(funcName, 0);
        searchPos = 0;
    }
    
    return code;
}

std::string Albat::update_operator_equal(std::string &code, const std::string &operatorSymbol) {
    struct OperatorInfo {
        std::string funcName; 
        std::string libName; 
    };

    static const std::map<std::string, OperatorInfo> operatorMap = {
        {"**=", {"_pow", "pow"}},
        {"/+=", {"_div", "_div"}},
        {"%+=", {"_mod", "_mod"}}
    };

    LibraryManager &libMan = LibraryManager::getInstance();

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
    std::string leftVar = code.substr(leftVarStart, operatorPos - leftVarStart);
    std::string rightExpr = code.substr(operatorPos + operatorSymbol.length());

    StringUtils::trim(before);
    StringUtils::trim(leftVar);
    StringUtils::trim(rightExpr);
    
    std::string result = before + leftVar + " = " + funcName + "(" + leftVar + ", " + rightExpr + ")" + afterOperator;
    libMan.requestLibrary(funcName, 0);
    return result;
}
