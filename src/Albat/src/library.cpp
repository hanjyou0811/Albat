#include "../albat.h"
#include "../../Utils/stringutils.h"
#include "../../Library/library.h"

void Albat::library_check(std::string &str)
{
    LibraryManager &libMan = LibraryManager::getInstance();
    StringUtils::trim(str);
    //データ構造とか追加する
    StringUtils::update_macro_all(str);
    StringUtils::update_macro_len(str);
    StringUtils::update_macro_count(str);
    StringUtils::updaet_macro_st_bound(str);
  
    //演算子
    // ** -> _pow(べきじょう)
    // /+ -> _div(繰り上がりのある除算)
    // %+ -> _mod(余りを非負で返す)
    str = Albat::update_operator_equal(str, "**=");
    str = Albat::update_operator_equal(str, "/+=");
    str = Albat::update_operator_equal(str, "%+=");
    str = Albat::update_operator(str, "**");
    str = Albat::update_operator(str, "/+");
    str = Albat::update_operator(str, "%+");
    StringUtils::update_library_replace(str);
    Albat::library_switch(str);
    std::vector<std::string> fmt;
    while(1)
    {
        //ここで自作ライブラリを追加する
        fmt = StringUtils::find_Function(str, "biseek()");
        if(fmt.size() == 3) {
            libMan.requestLibrary("biseek", 0);
        }
        fmt = StringUtils::find_Function(str, "chmax()");
        if(fmt.size() == 3) {
            libMan.requestLibrary("chmax", 0);
        }
        fmt = StringUtils::find_Function(str, "chmin()");
        if(fmt.size() == 3) {
            libMan.requestLibrary("chmin", 0);
        }
        fmt = StringUtils::find_Function(str, "runLengthEncoding()");
        if(fmt.size() == 3){
            libMan.requestLibrary("runLengthEncoding", 0);
        }
        fmt = StringUtils::find_Function(str, "yesno()");
        if(fmt.size() == 3){
            libMan.requestLibrary("yesno", 0);
        }
        fmt = StringUtils::find_Function(str, "GCD_L()");
        if(fmt.size() == 3){
            libMan.requestLibrary("gcd", 0);
        }
        fmt = StringUtils::find_Function(str, "lcm()");
        if(fmt.size() == 3){
            libMan.requestLibrary("lcm", 0);
        }
        fmt = StringUtils::find_Function(str, "inversionNumber()");
        if(fmt.size() == 3){
            libMan.requestLibrary("inversionNumber", 0);
        }
        fmt = StringUtils::find_Function(str, "divNum()");
        if(fmt.size() == 3){
            libMan.requestLibrary("divnum", 0);
        }
        fmt = StringUtils::find_Function(str, "makePrimes()");
        if(fmt.size() == 3){
            libMan.requestLibrary("makePrimes", 0);
        }
        fmt = StringUtils::find_Function(str, "sum()");
        if(fmt.size() == 3){
            libMan.requestLibrary("sum", 0);
        }
        // fmt = StringUtils::find_Function(str, "runLengthDecoding_vec()");
        // if(fmt.size() == 3){
        //   libMan.insertStill.insert("runLengthDecoding_vec");
        // }
        break;
    }
}

void Albat::library_switch(std::string &str)
{
    LibraryManager &libMan = LibraryManager::getInstance();
    if(str.empty()) return;
    if(StringUtils::strpos_exlit_token(str, "cin") >= 0 || StringUtils::strpos_exlit_token(str, "scanf") >= 0)
    {
        libMan.switch_Library("FastIO", 0);
    }
    if(StringUtils::strpos_exlit_token(str, "cout") >= 0 || StringUtils::strpos_exlit_token(str, "printf") >= 0)
    {
        libMan.switch_Library("FastIO", 0);
    }
}