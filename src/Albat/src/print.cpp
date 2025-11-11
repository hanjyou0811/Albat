#include "../albat.h"
#include "../../Utils/stringutils.h"
#include <string>

std::string Albat::print(int depth)
{
    LibraryManager& libMan = LibraryManager::getInstance();
    int i = 0, j = 0;
    std::string code = "";
    //ラムダ式でいい感じにやる
    auto emit = [&](const std::string &s) {
      #ifdef __EMSCRIPTEN__
        code += s;
      #else
        printf("%s", s.c_str());
      #endif
    };
    for(i = 0;i<lines.size();i++)
    {
      if(lineTypes[i] == LINETYPES::LIBRARY){
        code += nextPtrs[nextindices[i]]->print(depth);
        continue;
      }
      for(j = 0;j<depth;j++)
      {
        emit("    ");
      }
      #ifdef __EMSCRIPTEN__
      code += lines[i];
      #else
      code = lines[i];
      #endif
      if(code == "using namespace std;")
      {
        code += '\n';
        code += libMan.getLibraryIdentifier("prtype");
        if (libMan.CanUseLibrary("FastIO") || libMan.CanUseLibrary("FastIO_flush")) {
          if(libMan.CanUseLibrary("FastIO_flush")) {
            code += libMan.getLibraryIdentifier("FastIO_flush");
          }else{
            code += libMan.getLibraryIdentifier("FastIO");
          }
        }
      }
      #ifndef __EMSCRIPTEN__
      printf("%s", code.c_str());
      #endif
      if(lineTypes[i] == LINETYPES::BLOCKW){
        if(nextPtrs[nextindices[i]]->isValidEmptyBlock()){
          emit(";\n");
          continue;
        }
      }
      if(lineTypes[i] == LINETYPES::BLOCKS 
        || lineTypes[i] == LINETYPES::BLOCKW
        )
      {
        emit("{\n");
      }else
      {
        emit("\n");
      }
      if(nextindices[i] >= 0)
      {
        code += nextPtrs[nextindices[i]]->print(depth+1);
      }
      if(lineTypes[i] == LINETYPES::BLOCKS
        || lineTypes[i] == LINETYPES::BLOCKW
        )
      {
        for(j = 0;j<depth;j++)
        {
          emit("    ");
        }
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