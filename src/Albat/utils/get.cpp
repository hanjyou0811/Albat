#include "../albat.h"
#include "../../Utils/stringutils.h"

std::pair<int, int> Albat::getExpressionIndexAndLength(std::string expr, int pos, bool forward) {
  if (forward) {
    int originalPos = pos;
    
    std::string subExpr = expr.substr(pos);
    int currentPos = 0;

    currentPos = skipPrefixElements(subExpr, currentPos);
    
    int identifierStart = currentPos;
    currentPos = processIdentifier(subExpr, currentPos);
    currentPos = processSuffixElements(subExpr, currentPos);
    
    int exprStartIndex = originalPos + identifierStart;
    int exprLength = currentPos - identifierStart;
    
    return {exprStartIndex, exprLength};
  } else {
    std::string subExpr = expr.substr(0, pos + 1);
    int currentPos = subExpr.size() - 1;
    
    currentPos = skipSuffixElements(subExpr, currentPos);
    
    int expressionEndPos = currentPos;
    
    currentPos = processExpressionBackward(subExpr, currentPos);
    
    int expressionStartPos = currentPos;
    
    int exprStartIndex = expressionStartPos;
    int exprLength = expressionEndPos - expressionStartPos + 1;
    
    return {exprStartIndex, exprLength};
  }
}

std::vector<std::pair<std::string, std::string>> Albat::get_def_input(std::string type_str, std::string vardef_str)
{
  std::vector<std::pair<std::string,std::string>> res;
  std::vector<std::string> vardef_strs;
  StringUtils::trim(type_str);
  StringUtils::trim(vardef_str);
  int read_flg = (vardef_str[0] == '@' ? 1 : 0);
  if(read_flg)
  {
    vardef_str = vardef_str.substr(1);
    StringUtils::trim(vardef_str);
  }

  vardef_strs = StringUtils::split_without_char(vardef_str, ',');
  std::string tmp_vars = "";
  for(int i=0;i<vardef_strs.size();i++)
  {
    std::string vardef = vardef_strs[i];
    if(i == vardef_strs.size()-1){
      vardef = vardef.substr(0,vardef.size()-1);
    }
    StringUtils::trim(vardef);
    if(read_flg)
    {
      if(vardef[0] == '!'){
        vardef = vardef.substr(1);
        StringUtils::trim(vardef);
      }else
      {
        res.push_back({"input",vardef});
      }
    }
    tmp_vars += vardef + (i == vardef_strs.size()-1 ? "" : ", ");
  }
  res.insert(res.begin(), {"def", type_str + " " + tmp_vars + ';'});
  return res;
}
// std::pair<int,int> Albat::get_Expr_index_and_length_before(std::string expr, int pos)
// {
//     std::string subExpr = expr.substr(0, pos + 1);
//     int currentPos = subExpr.size() - 1;
    
//     currentPos = skipSuffixElements(subExpr, currentPos);
    
//     int expressionEndPos = currentPos;
    
//     currentPos = processExpressionBackward(subExpr, currentPos);
    
//     int expressionStartPos = currentPos;
    
//     int exprStartIndex = expressionStartPos;
//     int exprLength = expressionEndPos - expressionStartPos + 1;
    
//     return {exprStartIndex, exprLength};
// }


// std::pair<int,int> Albat::get_Expr_index_and_length_after(std::string expr, int pos)
// {
//     int originalPos = pos;
    
//     std::string subExpr = expr.substr(pos);
//     int currentPos = 0;

//     currentPos = skipPrefixElements(subExpr, currentPos);
    
//     int identifierStart = currentPos;
//     currentPos = processIdentifier(subExpr, currentPos);
//     currentPos = processSuffixElements(subExpr, currentPos);
    
//     int exprStartIndex = originalPos + identifierStart;
//     int exprLength = currentPos - identifierStart;
    
//     return {exprStartIndex, exprLength};
// }