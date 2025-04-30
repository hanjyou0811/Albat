#include "../albat.h"
#include "../../Utils/stringutils.h"

int Albat::skipPrefixElements(const std::string& str, int pos) {
    while (pos < str.size())
    {
      if (isspace(str[pos]))
      {
        pos++;
        continue;
      }

      if (StringUtils::isOperator(str[pos]))
      {
        pos++;
        continue;
      }
      
      if (str[pos] == '(') {
        std::string strCopy = str;
        int closingPos = StringUtils::find_pairBracket1(strCopy, pos);
        std::string substring = str.substr(pos + 1, closingPos - pos - 1);
        if (closingPos > pos && isValidtypename(substring, ' '))
        {
          pos = closingPos + 1;
          continue;
        }
      }
      
      break;
    }
    
    return pos;
}

int Albat::skipSuffixElements(const std::string& str, int pos)
{
    while (pos >= 0)
    {
        if (isspace(str[pos]))
        {
            pos--;
            continue;
        }
        
        if (pos > 0 && (str.substr(pos-1, 2) == "++" || str.substr(pos-1, 2) == "--"))
        {
            pos -= 2;
            continue;
        }
        
        break;
    }
    
    return pos;
}

int Albat::skipPrefixElementsBackward(const std::string& str, int pos)
{
    while (pos >= 0) {
        if (isspace(str[pos])) {
            pos--;
            continue;
        }
        
        if (pos >= 1 && (str.substr(pos-1, 2) == "++" || str.substr(pos-1, 2) == "--"))
        {
            pos -= 2;
            continue;
        }
        
        if (str[pos] == ')') {
            int openPos = findMatchingOpenBracket(str, pos);
            if (openPos >= 0 && openPos > 0 && str[openPos-1] == '(')
            {
                std::string typeName = str.substr(openPos, pos - openPos);
                if (isValidtypename(typeName, ' '))
                {
                    pos = openPos - 1;
                    continue;
                }
            }
        }
        
        break;
    }
    
    return pos;
}


int Albat::processIdentifier(const std::string& str, int pos)
{
    while (pos < str.size() && (isalnum(str[pos]) || str[pos] == '.' || str[pos] == '_'))
    {
        pos++;
    }
    
    return pos;
}

int Albat::processSuffixElements(std::string &str, int pos)
{
    while( pos < str.size())
    {
        while(pos < str.size() && isspace(str[pos]))
        {
            pos++;
        }
        if(pos == str.size()) break;

        if(str[pos] == '(')
        {
            pos = StringUtils::find_pairBracket1(str, pos) + 1;
            str = str.substr(0, pos);
            break;
        }

        if(str[pos] == '[')
        {
            pos = StringUtils::find_pairBracket1(str, pos) + 1;
            continue;
        }

        str = str.substr(0, pos);
        break;
    }
    return (pos);
}

int Albat::processExpressionBackward(const std::string& str, int pos)
{
    if (pos >= 0 && (str[pos] == ')' || str[pos] == ']'))
    {
        int bracketType = (str[pos] == ')') ? 0 : 1; 
        int depth = 1;
        pos--;
        
        while (pos >= 0 && depth > 0)
        {
            if (bracketType == 0)
            {
                if (str[pos] == ')') depth++;
                if (str[pos] == '(') depth--;
            } else
            {
                if (str[pos] == ']') depth++;
                if (str[pos] == '[') depth--;
            }
            pos--;
        }
        
        int nameEndPos = pos;
        while (pos >= 0 && (isalnum(str[pos]) || str[pos] == '_' || str[pos] == '.'))
        {
            pos--;
        }
        
        if (pos >= 5 && str.substr(pos-5, 6) == "return")
        {
            return pos - 5;
        }
        
        return pos + 1;
    }
    
    while (pos >= 0 && (isalnum(str[pos]) || str[pos] == '_' || str[pos] == '.'))
    {
        pos--;
    }
    
    return pos + 1;
}

int Albat::findMatchingOpenBracket(const std::string& str, int closePos)
{
    if (closePos < 0 || closePos >= str.size() || str[closePos] != ')') {
        return -1;
    }
    
    int depth = 1;
    int pos = closePos - 1;
    
    while (pos >= 0 && depth > 0)
    {
        if (str[pos] == ')') depth++;
        if (str[pos] == '(') depth--;
        pos--;
    }
    
    return (depth == 0) ? pos + 1 : -1;
}