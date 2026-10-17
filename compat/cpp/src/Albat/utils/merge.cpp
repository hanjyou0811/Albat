#include "../albat.h"

void Albat::mergeSet(std::set<std::string> &tgt, const std::set<std::string> &src)
{
    tgt.insert(src.begin(), src.end());
}

void Albat::mergeMap(std::map<std::string, std::string> &tgt,
    const std::map<std::string, std::string> &src)
{
    for (const auto &pair : src) {
        tgt[pair.first] = pair.second;
    }
}

void Albat::merges(const std::set<std::string> &sVarTypes,
    const std::set<std::string> &sTempVarTypes, const std::map<std::string, std::string> &sGlobalVars,
    const std::map<std::string, std::string> &sLocalVars,
    const std::map<std::string, std::string> &sArgVars)
    {
        mergeSet(typenames, sVarTypes);
        mergeSet(tmptypenames, sTempVarTypes);
        mergeMap(globalVars, sGlobalVars);
        mergeMap(globalVars, sLocalVars);
        mergeMap(globalVars, sArgVars);
    }

void Albat::mergeParent(Albat *parentNode)
{
    parent = parentNode;
    merges(parentNode->typenames, parentNode->tmptypenames, parentNode->globalVars,
        parentNode->localVars, parentNode->argVars);
    PrefixStrs = parentNode->PrefixStrs;
}