#include "../library.h"

std::string LibraryManager::getLibraryIdentifier(const std::string &name) {
    auto it = lib_Ident.find(name);
    if (it != lib_Ident.end()) {
        return it->second;
    }
    return "";
}