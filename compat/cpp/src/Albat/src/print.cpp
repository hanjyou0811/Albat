#include "../albat.h"
#include "../../Utils/stringutils.h"
#include <string>

namespace {
std::string escapeLineDirectiveFilename(const std::string &filename)
{
    std::string escaped;
    escaped.reserve(filename.size());
    for (char c : filename) {
        if (c == '\\' || c == '"') {
            escaped += '\\';
        }
        escaped += c;
    }
    return escaped;
}
}

std::string Albat::print(int depth)
{
    LibraryManager& libMan = LibraryManager::getInstance();
    std::string code = "";
    int activeSourceLine = -1;
    std::string activeSourceFile;

    auto emit = [&](const std::string& s) {
        code += s;
    };

    for (int i = 0; i < lines.size(); i++)
    {
        if (lineTypes[i] == LINETYPES::LIBRARY) {
            code += nextPtrs[nextindices[i]]->print(depth);
            activeSourceLine = -1;
            activeSourceFile.clear();
            continue;
        }
        if (sourceLines.size() > i && sourceLines[i] > 0 && !sourceFile.empty()) {
            if (activeSourceLine != sourceLines[i] || activeSourceFile != sourceFile) {
                emit("#line " + std::to_string(sourceLines[i]) + " \"" +
                     escapeLineDirectiveFilename(sourceFile) + "\"\n");
                activeSourceLine = sourceLines[i];
                activeSourceFile = sourceFile;
            }
        } else if (activeSourceLine != -1 || !activeSourceFile.empty()) {
            emit("#line 1 \"<generated>\"\n");
            activeSourceLine = -1;
            activeSourceFile.clear();
        }
        for (int j = 0; j < depth; j++)
            emit("    ");
        emit(lines[i]);
        if (sourceLines.size() > i && sourceLines[i] > 0 && !sourceFile.empty()) {
            activeSourceLine = sourceLines[i];
        }
        if (lines[i] == "using namespace std;")
        {
            emit("\n");
            if (activeSourceLine > 0 && !activeSourceFile.empty()) {
                emit("#line 1 \"<generated>\"\n");
            }
            emit(libMan.getLibraryIdentifier("prtype"));
            if (libMan.CanUseLibrary("FastIO") || libMan.CanUseLibrary("FastIO_flush")) {
                if (libMan.CanUseLibrary("FastIO_flush"))
                    emit(libMan.getLibraryIdentifier("FastIO_flush"));
                else
                    emit(libMan.getLibraryIdentifier("FastIO"));
            }
            if (activeSourceLine > 0 && !activeSourceFile.empty()) {
                emit("#line " + std::to_string(activeSourceLine + 1) + " \"" +
                     escapeLineDirectiveFilename(activeSourceFile) + "\"\n");
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
        if (activeSourceLine > 0) {
            activeSourceLine++;
        }
        if (nextindices[i] >= 0)
            emit(nextPtrs[nextindices[i]]->print(depth + 1));
        if (lineTypes[i] == LINETYPES::BLOCKS || lineTypes[i] == LINETYPES::BLOCKW)
        {
            if (activeSourceLine != -1 || !activeSourceFile.empty()) {
                emit("#line 1 \"<generated>\"\n");
                activeSourceLine = -1;
                activeSourceFile.clear();
            }
            for (int j = 0; j < depth; j++)
                emit("    ");
            emit("}\n");
        }
    }
    return code;
}

void Albat::debug(int depth)
{
    int i, j;
  
    std::set<std::string>::iterator it1;
    std::map<std::string,std::string>::iterator it2;
    printf("vartype  :");
    for(it1=typenames.begin();it1!=typenames.end();it1++) printf(" (%s)", it1->c_str()); puts("");
    printf("tvartype :");
    for(it1=STLtypenames.begin();it1!=STLtypenames.end();it1++) printf(" (%s)", it1->c_str()); puts("");
    printf("tmptypes : ");
    for(it1=tmptypenames.begin();it1!=tmptypenames.end();it1++) printf(" (%s)", it1->c_str()); puts("");
    printf("localvar :");
    for(it2=localVars.begin();it2!=localVars.end();it2++) printf(" (%s->%s)", it2->first.c_str(), it2->second.c_str()); puts("");
    printf("globalvar:");
    for(it2=globalVars.begin();it2!=globalVars.end();it2++) printf(" (%s->%s)", it2->first.c_str(), it2->second.c_str()); puts("");
    printf("argvar   :");
    for(it2=argVars.begin();it2!=argVars.end();it2++) printf(" (%s->%s)", it2->first.c_str(), it2->second.c_str()); puts("");
    for(i=0;i<lines.size();i++)
    {
      for(j=0;j<depth;j++) printf(" ");
      printf("%s\n", lines[i].c_str());
      if(nextindices[i] >= 0) nextPtrs[nextindices[i]]->debug(depth + 1);
    }
}
