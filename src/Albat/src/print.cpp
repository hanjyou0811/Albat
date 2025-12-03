#include "../albat.h"
#include "../../Utils/stringutils.h"
#include <string>

std::string Albat::print(int depth)
{
    LibraryManager& libMan = LibraryManager::getInstance();
    std::string code = "";

    auto emit = [&](const std::string& s) {
        code += s;
    };

    for (int i = 0; i < lines.size(); i++)
    {
        if (lineTypes[i] == LINETYPES::LIBRARY) {
            code += nextPtrs[nextindices[i]]->print(depth);
            continue;
        }
        for (int j = 0; j < depth; j++)
            emit("    ");
        emit(lines[i]);
        if (lines[i] == "using namespace std;")
        {
            emit("\n");
            emit(libMan.getLibraryIdentifier("prtype"));
            if (libMan.CanUseLibrary("FastIO") || libMan.CanUseLibrary("FastIO_flush")) {
                if (libMan.CanUseLibrary("FastIO_flush"))
                    emit(libMan.getLibraryIdentifier("FastIO_flush"));
                else
                    emit(libMan.getLibraryIdentifier("FastIO"));
            }
        }
        if (lineTypes[i] == LINETYPES::BLOCKW &&
            nextPtrs[nextindices[i]]->isValidEmptyBlock())
        {
            emit(";\n");
            continue;
        }
        if (lineTypes[i] == LINETYPES::BLOCKS || lineTypes[i] == LINETYPES::BLOCKW)
            emit("{\n");
        else
            emit("\n");
        if (nextindices[i] >= 0)
            emit(nextPtrs[nextindices[i]]->print(depth + 1));
        if (lineTypes[i] == LINETYPES::BLOCKS || lineTypes[i] == LINETYPES::BLOCKW)
        {
            for (int j = 0; j < depth; j++)
                emit("    ");
            emit("}\n");
        }
    }
    return code;
}
