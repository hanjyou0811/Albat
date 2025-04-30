#include "../albat.h"
#include "../../Utils/stringutils.h"

int Albat::checkEndType(std::string &code, int &endType, int &extraSentence, int returnFlg)
{

    if(endType == -1)
    {
        return 1;
    }

    if(endType == 0)
    {
        if (extraSentence) {
            extraSentence = 0;
        }else{
            endType = -1;
        }
    }

    if(code.empty())
    {
        return 1;
    }

    if(code[0] == '}' && endType == 1)
    {
        code = code.substr(1);
        return 1;
    }
    return 0;
}
