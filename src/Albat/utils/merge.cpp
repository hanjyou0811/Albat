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
    const std::set<std::string> &sTempVarTypes)
{
        mergeSet(typenames, sVarTypes);
        mergeSet(tmptypenames, sTempVarTypes);
        mergeMap(globalVars, globalVars);
        mergeMap(localVars, localVars);
        mergeMap(argVars, argVars);
    }

void Albat::mergeParent(Albat *parentNode)
{
    parent = parentNode;
    merges(parentNode->typenames, parentNode->tmptypenames);
}