# include "stringutils.h"

/*operator.cpp*/
namespace StringUtils{
    int isOperator(char c)
    {
        if(c=='+' || c=='-' || c=='*' || c=='/' || c=='%'|| c=='=') return 1;
        if(c=='&' || c=='|' || c=='^' || c=='~') return 1;
        return 0;
    }
}