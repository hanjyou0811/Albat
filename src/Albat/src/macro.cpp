#include "../albat.h"
#include "../../Utils/stringutils.h"
#include <iostream>

int Albat::processMacros(std::string &code, std::string &blockName)
{
    if (blockName.substr(0, 7) == "do_perm")
    {
        processPermMacro(code, blockName);
        return 1;
    }

    if (blockName.substr(0, 3) == "rep" || blockName.substr(0, 3) == "REP" ||
        blockName.substr(0, 4) == "rrep" || blockName.substr(0, 4) == "RREP" ||
        blockName.substr(0, 4) == "fore")
    {
        processLoopMacro(blockName);
        return 0;
    }
    return 0;
}

void Albat::processPermMacro(std::string &code, std::string &macroName)
{
    std::string perm = macroName;
    while (perm[0] != '(') perm = perm.substr(1);
    perm = perm.substr(1, perm.size() - 2);

    std::string prefix = "do{";
    std::string suffix = "}while(next_permutation(" + perm + "));";
    
    int blockLength = StringUtils::size_block(code);
    code = "{" + prefix + code.substr(0, blockLength) + suffix + "}" + code.substr(blockLength);
}

void Albat::processLoopMacro(std::string &macroName)
{
    bool isRep = (macroName.substr(0, 3) == "rep" || macroName.substr(0, 3) == "REP");
    bool isRRep = (macroName.substr(0, 4) == "rrep" || macroName.substr(0, 4) == "RREP");
    bool isFore = (macroName.substr(0, 4) == "fore");
    
    // カッコ内のパラメータを取得
    while (macroName[0] != '(') macroName = macroName.substr(1);
    while (macroName.back() != ')') macroName.pop_back();
    std::string params = macroName.substr(1, macroName.size() - 2);
    
    // パラメータをカンマで分割
    std::vector<std::string> args = StringUtils::split_without_char(params, ',');
    
    // rep/rrepの2引数形式を3引数形式に変換
    if (!isFore && args.size() == 2) {
        args.push_back(args[1]);
        args[1] = "0";
    }
    
    // 各引数をトリム
    for (auto &arg : args) {
        StringUtils::trim(arg);
    }
    
    // 対応するfor文に変換
    if (args.size() == 4 && isRep) {
        macroName = "for(int " + args[0] + " = " + args[1] + "; " + 
                    args[0] + " < " + args[2] + "; " + 
                    args[0] + " += " + args[3] + ")";
    }
    else if (args.size() == 4 && isRRep) {
        macroName = "for(int " + args[0] + " = " + args[2] + "-1; " + 
                    args[0] + " >= " + args[1] + "; " + 
                    args[0] + " -= " + args[3] + ")";
    }
    else if (args.size() == 3 && isRep) {
        macroName = "for(int " + args[0] + " = " + args[1] + "; " + 
                    args[0] + " < " + args[2] + "; " + 
                    args[0] + "++)";
    }
    else if (args.size() == 3 && isRRep) {
        macroName = "for(int " + args[0] + " = " + args[2] + "-1; " + 
                    args[0] + " >= " + args[1] + "; " + 
                    args[0] + "--)";
    }
    else if (args.size() == 2 && isFore) {
        macroName = "for(auto &" + args[0] + " : " + args[1] + ")";
    }
    else {
        macroName = "Error!!!";
    }
}