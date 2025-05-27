#include "../albat.h"
#include "../../Utils/stringutils.h"

int Albat::setup_Line(std::string &str, std::string tp, std::string &typeStr, int &is_return)
{
  std::string head_tmp = "",tmp_vars = "", tmp = "", varName = "";
  int i = 0, type = (tp == "input" ? 1 : 0);
  int loop_opt = 0;
  std::string res = "";
  std::vector<std::string> vars;
  library_check(str);
  if(type || str[i] == '$' || str[i] == '@')
  {
    while(str[i] == '@' || str[i] == '$')
    {
      if(type){
        i++;
        continue;
      }
      if(str[i] == '@')
      {
        type = 1;
      }else
      {
        type = 2;
      }
      i++;
    }
    if(type == 1)
    {
      str = str.substr(i);
      head_tmp = "rd";
    }else if (type == 2){
      str = str.substr(i);
      head_tmp = (i == 1 ? "wt" : "wtln");
    }
    if(type == 1)
    {
      while (!str.empty() && str.back() == ';') {
          str.pop_back();
      }
      //ここで頑張るべきではない
      if(str.back() == ']')
      {
        std::string tmp = StringUtils::extra_input_option(str);
        if(tmp.size()){
          StringUtils::removeAllWhitespace(tmp);
          if(tmp.substr(0,5)=="size="){
            loop_opt = 1;
            std::string size = tmp.substr(5);
            varName = gen_fresh_varname();
            res += ("rep(" + varName + "0," + size + "){");
          }
        }
      }
      
      if(str[0] == '(')
      {
        StringUtils::trim_char(str, '(', ')');
        str = str.substr(1, str.size()-2);
      }
      vars = StringUtils::split_without_char(str, ',');
      std::string tmp_vars = "";
      for(int i=0;i<vars.size();i++)
      {
        std::string vardef = vars[i];
        StringUtils::trim(vardef);
        if(!vardef.empty()){
          vardef = StringUtils::extra_varname_exlit(vardef);
        }
        if(vardef[0] == '!') continue;
        //++--の方は多分使えない
        if(typeStr.substr(0, 6) == "vector") {
          loop_opt = 1;
        }
        if(vardef.find("++") != std::string::npos || vardef.find("--") != std::string::npos){
          std::vector<std::string> tmp = StringUtils::split_without_chars(vardef, "+-");
          vardef = head_tmp + "(" + tmp[0] + ");";
          if(loop_opt)
          {
            varName = gen_fresh_varname();
            vardef += "fore(" + varName + "," + tmp[0] + "){";
          }
          vardef += ((loop_opt && !varName.empty())? varName : tmp[0]) + (vardef.find("++") != std::string::npos ? "++" : "--") + ";";
        }else
        {
          //こっち主に使う
          vardef = head_tmp + "(" + vardef  + (loop_opt ? "[i]" : "" )+ ");";
        }
        res += vardef;
      }
      if(loop_opt) res += "}";
      insert(res, lines.size());
      // nextIndices.push_back(nextPrograms.size());
      // lines.push_back(res);
      // lineTypes.push_back(CODETYPES::SENTENCE);
    }else if(type == 2)
    {
      i = 0;
      while (!str.empty() && str.back() == ';') {
          str.pop_back();
      }
      StringUtils::trim(str);
      if(str[i] == '('){
        str = str.substr(1, str.size()-2);
      }
      vars = StringUtils::split_without_char(str, ',');
      for(int i=0;i<vars.size();i++)
      {
        std::string vardef = vars[i];
        StringUtils::trim(vardef);
        vardef = head_tmp + "(" + vardef + ");";
        nextindices.push_back(-1);
        lines.push_back(vardef);
        lineTypes.push_back(LINETYPES::SENTENCE);
      }
    }
    if(type){
      LibraryManager& libMan = LibraryManager::getInstance();
      libMan.switch_Library("FastIO", 1);
      return 0;
    }
  }
  else
  {
    std::pair<std::string, char> st_char = StringUtils::nextToken(str);
    if(st_char.first == "return")
    {
      is_return = 1;
    }else
    {
      is_return = 0;
    }
  }
  return 1;
}

void Albat::setup_def(std::string &str)
{
    std::string type = "";
    std::vector<std::string> vars;
    std::string res = "";
    int i = 0, j = 0, is_lambda = 0;
    library_check(str);
    for(i=1;i<str.size();i++)
    {
        std::string tmp = str.substr(0,i);
        if(isValidtypename(tmp, str[i])) j = i;
    }
    type = str.substr(0,j);
    StringUtils::trim(type);
    res += type + " ";
    str = str.substr(j,str.size()-j-1);
    StringUtils::trim(str);
    if(str[0] == '('){
      StringUtils::trim_char(str, '(', ')');
      str = str.substr(1, str.size()-2);
    }
    vars = StringUtils::split_without_char(str, ',');
    for(i=0;i<vars.size();i++)
    {
      std::string v = vars[i];
      // vector<string> tmp = StringUtils::split_without_str(v, "++");
      // if(tmp.size() > 1) assert(0);
      // tmp = StringUtils::split_without_str(tmp[0], "--");
      // string var = tmp[0];
      std::string var = v;
      // if(var[0] == '&' || var[0] == '*') var = var.substr(1);
      StringUtils::trim(var);
      // std::vector<std::string> tmp = StringUtils::split_without_char(var, '=');
      // std::cerr << tmp[0] << " " << tmp[1] << std::endl;
      // std::cerr << "var: " << var << std::endl;
      // if(tmp.size() == 2){
      //   var += tmp[0];
      // }else{
      //   var += tmp[0];
      // }
      // std::cerr << "var: " << var << std::endl;
      if(i == vars.size() - 1){
        res += var + ";";
      }else{
        res += var + ", ";
      }
      addLocalVar(var, type);
    }
    str = res; 
    StringUtils::trim(str);
    std::string strtmp = str;
    //ラムダ式 
    //[]{};
    while(str.size()){
      int si, sj;
      is_lambda = 0;
      char pre = ';';
      std::string tmpstr = "";
      for(si=0;si<str.size()-1;si++){
        if(str[si] == '[' && (pre==';' || pre==',' || pre=='=' || pre=='(' || pre==')' || pre=='{' || pre=='}')){
          std::string tmpstr = str.substr(si);
          int square_size = StringUtils::size_brackets(tmpstr);
          std::string capture_str = str.substr(si + 1, square_size - 2);
          for(sj = si + square_size; sj < str.size(); sj++){
            if(str[sj] == '{') break;
          }
          if(sj == str.size()) break;

          tmpstr = str.substr(sj);
          int block_size = StringUtils::size_block(tmpstr);
          tmpstr = str.substr(0, sj);

          lines.push_back(tmpstr);
          lineTypes.push_back(LINETYPES::SENTENCE);
          nextindices.push_back(-1);
          tmpstr = str.substr(sj, block_size);
          tmpstr += ";";
          insert(tmpstr, lines.size());
          str = str.substr(sj + block_size);
          is_lambda = 1;
          break;
        }
        if(!isspace(str[si])) pre = str[si];
      }
      if(is_lambda == 1) continue;
      str = ""; 
    }
    if(is_lambda == 0) str = strtmp;
}