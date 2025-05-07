#include "../albat.h"
#include "../../Utils/stringutils.h"

void Albat::parse(std::string &code, std::string opt, int nestlevel, LINETYPES codeType)
{
    init();
    int endType = processCodeStart(code, codeType, opt);
    int extraSentence = !opt.empty() && endType == 0;
    int returnFlg = 0;
    int mainCreated = 0;

    while(!code.empty())
    {
        StringUtils::ftrim(code);
        if(code.empty()) break;
	    if(code.empty()) break;
        if(codeType != LINETYPES::PROGRAM && code[0] == '{')
        {
            code = code.substr(1);
            endType = 1;
        }
        if(codeType == LINETYPES::PROGRAM)
        {
            endType = 2;
        }
        if(code.empty() && endType == 2) break;
        if(checkEndType(code, endType, extraSentence, returnFlg))
        {
            break;
        }
        if (isValidPreprocessor(code))
        {
            continue;
        }
        if(isValidComment(code))
        {
            continue;
        }

        if(processDirectives(code))
        {
            continue;
        }
        std::string prefixStr = processTemplate_Modifiers(code);
        if(!prefixStr.empty())
        {
            processTempalteParams(prefixStr);
        }
        int typeEndpos = -1;
        int limit = std::min(100, static_cast<int>(code.size() + 1));
        for (int i = 1; i < limit; i++)
        {
            std::string tmp = code.substr(0, i);
            if (isValidtypename(tmp, code[i]))
            {
                typeEndpos = i;
            }
        }
        auto [token, nextChar] = StringUtils::nextToken(code);
        int blockType = determineBlockType(code, token, nextChar, typeEndpos);
        if (blockType || code[0] == '{')
        {
            processBlock(code, blockType, token, typeEndpos, prefixStr, codeType, 
                opt, nestlevel, mainCreated, returnFlg);
        	continue;
	    }
        processSentence(code, typeEndpos, returnFlg);
    }
    if (name == "int main()" && returnFlg == 0)
    {
        lines.push_back("return 0;");
        nextindices.push_back(-1);
        lineTypes.push_back(LINETYPES::SENTENCE);
    }
}
