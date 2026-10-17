#include "../albat.h"
#include "../../Utils/stringutils.h"

int Albat::findExpressionEnd(const std::string &code, int startPos) {
    int bracketCount = 0;  
    int squareCount = 0;
    int braceCount = 0;
    bool inSingleQuote = false;
    bool inDoubleQuote = false;
    
    for (int i = startPos; i < code.length(); ++i) {
        if ((inSingleQuote || inDoubleQuote) && code[i] == '\\')
        {
            i++;
            continue;
        }
        
        if (!inSingleQuote && !inDoubleQuote)
        {
            if (code[i] == '(') bracketCount++;
            if (code[i] == ')') bracketCount--;
            if (code[i] == '[') squareCount++;
            if (code[i] == ']') squareCount--;
            if (code[i] == '{') braceCount++;
            if (code[i] == '}') braceCount--;
        }
        
        if (!inDoubleQuote && code[i] == '\'') inSingleQuote = !inSingleQuote;
        if (!inSingleQuote && code[i] == '"')  inDoubleQuote = !inDoubleQuote;
        
        if (bracketCount < 0 || squareCount < 0 || braceCount < 0)
        {
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

int Albat::findVariableStart(const std::string &code, int operatorPos) {
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

int Albat::findControlBlockEnd(const std::string &code)
{
    int pos = 0;
    int bracketCount = 0;
    bool inSingleQuote = false;
    bool inDoubleQuote = false;
    
    for (pos = 0; pos < code.size(); pos++) {
        // リテラル内のエスケープ処理
        if (inDoubleQuote || inSingleQuote) {
            if (code[pos] == '\\') {
                pos++;
                continue;
            }
        }
        
        // クオート処理
        if (!inDoubleQuote && code[pos] == '\'') inSingleQuote = !inSingleQuote;
        if (!inSingleQuote && code[pos] == '"') inDoubleQuote = !inDoubleQuote;
        
        // リテラル外での括弧のカウント
        if (!inSingleQuote && !inDoubleQuote) {
            if (code[pos] == '(') bracketCount++;
            if (code[pos] == ')') bracketCount--;
            
            // 制御構造の終了判定
            if (code[pos] == ')' && bracketCount == 0) {
                pos++;
                break;
            }
        }
    }
    
    return pos;
}