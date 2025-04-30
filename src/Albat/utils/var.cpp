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

void Albat::addLocalVar(const std::string &name, const std::string &type) {
    localVars[name] = type;
    for (auto *child : nextPtrs) {
        child->addLocalVar(name, type);
    }
}

void Albat::addGlobalVar(const std::string &name, const std::string &type) {
    globalVars[name] = type;
    for (auto *child : nextPtrs) {
        child->addGlobalVar(name, type);
    }
}

void Albat::addArgVar(const std::string &name, const std::string &type) {
    argVars[name] = type;
    for (auto *child : nextPtrs) {
        child->addArgVar(name, type);
    }
}