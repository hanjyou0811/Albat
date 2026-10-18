#include "../albat.h"
#include "../../Utils/stringutils.h"

void Albat::insert(std::string &str, int pos){
    int pure;
    // int cnt = 0;
    for(;;){
      StringUtils::ftrim(str);
      if(str.size()==0) break;
      if(str[0]=='{') pure = 1; else  pure = 0;
  
      // cnt++;
      Albat *child = new Albat;
      child->mergeParent(this);
      if(pure){
        child->parse(str,"", 0, LINETYPES::BLOCKS, -1, sourceFile);
      } else {
        child->parse(str,"", 0, LINETYPES::LIBRARY, -1, sourceFile);
      }
      nextPtrs.push_back(child);
      if(pure){
        insertOutputLine(pos, "", LINETYPES::BLOCKS, nextPtrs.size() - 1, -1);
      } else {
        insertOutputLine(pos, "", LINETYPES::LIBRARY, nextPtrs.size() - 1, -1);
      }
      pos++;
    }
  }
