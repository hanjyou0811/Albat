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
        child->parse(str,"", 0, LINETYPES::BLOCKS);
      } else {
        child->parse(str,"", 0, LINETYPES::LIBRARY);
      }
      nextindices.insert(nextindices.begin()+pos, nextPtrs.size());
      nextPtrs.push_back(child);
      lines.insert(lines.begin()+pos, (std::string)"");
      if(pure){
        lineTypes.insert(lineTypes.begin()+pos, LINETYPES::BLOCKS);
      } else {
        lineTypes.insert(lineTypes.begin()+pos, LINETYPES::LIBRARY);
      }
      pos++;
    }
  }