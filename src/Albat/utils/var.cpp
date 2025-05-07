#include "../albat.h"
#include "../../Utils/stringutils.h"

void Albat::addVarType(const std::string &type) {
    typenames.insert(type);
    for (auto *child : nextPtrs) {
        child->addVarType(type);
    }
}

void Albat::addTempVarType(const std::string &type) {
    tmptypenames.insert(type);
    for (auto *child : nextPtrs) {
        child->addTempVarType(type);
    }
}

void Albat::addLocalVar(const std::string &name, const std::string &type)
{
    auto [Varname, val] = StringUtils::extra_VarInfo(name);
    localVars[Varname] = type + ":" + val;
    for (auto *child : nextPtrs) {
        child->addLocalVar(name, type);
    }
}

void Albat::addGlobalVar(const std::string &name, const std::string &type) {
    auto [Varname, val] = StringUtils::extra_VarInfo(name);
    globalVars[Varname] = type + ":" + val;
    for (auto *child : nextPtrs) {
        child->addGlobalVar(name, type);
    }
}

void Albat::addArgVar(const std::string &name, const std::string &type) {
    auto [Varname, val] = StringUtils::extra_VarInfo(name);
    argVars[Varname] = type + ":" + val;
    for (auto *child : nextPtrs) {
        child->addArgVar(name, type);
    }
}

std::string Albat::gen_fresh_varname(int size)
{
    std::string strs = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_";
    std::string res = "";
    for(int i = size;;i++)
    {
        for(int j=0;j<100;j++)
        {
            res = "";
            res += strs[rand() % 52];
            for(int k=1;k<i;k++)
            {
                res += strs[rand() % strs.size()];
            }
            if(localVars.find(res) == localVars.end() && globalVars.find(res) == globalVars.end() && argVars.find(res) == argVars.end())
            {
                return res;
            }
        }
    }
    return res;
}