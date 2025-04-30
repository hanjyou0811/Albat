#include "stringutils.h"

/*size.cpp*/
namespace StringUtils {
    int size_block(std::string &str)
    {
        int i = 0;
        int t1 = 0, t2 = 0, t3 = 0, t4 = 0, t5 = 0;
        for(i=0;i<str.size();i++){
            if(t4==0 && t5==0){
                if(str[i]=='(') t1++;
                if(str[i]==')') t1--;
                if(str[i]=='[') t2++;
                if(str[i]==']') t2--;
                if(str[i]=='{') t3++;
                if(str[i]=='}'){
                    t3--;
                    if(t3==0) break;
                }
            }
            if(t5==0 && str[i]=='\'') t4 ^= 1;
            if(t4==0 && str[i]=='"') t5 ^= 1;
            if(t1==0 && t2==0 && t3==0 && t4==0 && t5==0 && str[i]==';') break;
        }
        return i+1;
    }
    int size_brackets(std::string &str){
        int i = 0;
        int t1 = 0, t2 = 0, t3 = 0, t4 = 0, t5 = 0;
        for(i=0;i<str.size();i++){
            if(t4==0 && t5==0){
                if(str[i]=='(') t1++;
                if(str[i]==')') t1--;
                if(str[i]=='[') t2++;
                if(str[i]==']') {
                    t2--;
                    if(t2==0) break;
                }
                if(str[i]=='{') t3++;
                if(str[i]=='}') t3--;
            }
            if(t5==0 && str[i]=='\'') t4 ^= 1;
            if(t4==0 && str[i]=='"') t5 ^= 1;
            if(t1==0 && t2==0 && t3==0 && t4==0 && t5==0 && str[i]==';') break;
        }
        return i+1;
    }
}