#ifndef ALBATUTILS_H
#define ALBATUTILS_H

#include <vector>
#include <string>
#include <set>
#include <map>
#include "../Library/library.h"

//debug
#include <iostream>

enum class LINETYPES{
    PROGRAM,
    SENTENCE,
    INCLUDE,
    DEFINE,
    IFDEF,
    ENDIF,
    UNDEF,
    PRAGMA,
    BLOCKS, //block-
    BLOCKW, // block-while
    STRUCT,
    CLASS,
    INPUT,
    LIBRARY
};

class Albat {
    public:
        Albat();
        Albat(std::string name, Albat *parent = nullptr);


        std::string name;
        Albat *parent = nullptr;
        std::vector<std::string> lines;
        std::vector<LINETYPES> lineTypes;
        std::vector<int> nextindices;
        std::vector<Albat *> nextPtrs;

        std::set<std::string> typenames, STLtypenames, tmptypenames;
        // std::map<std::string, std::string> 

        std::map<std::string, std::string> localVars, globalVars, argVars;

        void init();

        //merge.cpp
        void mergeSet(std::set<std::string> &tgt, const std::set<std::string> &src);
        void mergeMap(std::map<std::string, std::string> &tgt,
                      const std::map<std::string, std::string> &src);
        void merges(const std::set<std::string> &sVarTypes,
                    const std::set<std::string> &sTempVarTypes,
                    const std::map<std::string, std::string> &sGlobalVars,
                    const std::map<std::string, std::string> &sLocalVars,
                    const std::map<std::string, std::string> &sArgVars);
        void mergeParent(Albat *parent);

        //var.cpp
        void addVarType(const std::string &type);
        void addTempVarType(const std::string &type);
        void addLocalVar(const std::string &name, const std::string &type);
        void addGlobalVar(const std::string &name, const std::string &type);
        void addArgVar(const std::string &name, const std::string &type);
        std::string gen_fresh_varname(int size = 5);

        //valid.cpp
        int isValidEmptyBlock();
        int isValidPreprocessor(std::string &code);
        int isValidComment(std::string &code);
        int isValidtypename(std::string &str, char nex);
        int isValidFunction(const std::string &code);

        //get.cpp
        std::pair<int, int> getExpressionIndexAndLength(std::string expr, int pos, bool forward = true);
        // std::pair<int,int> get_Expr_index_and_length_before(std::string expr, int pos);
        // std::pair<int,int> get_Expr_index_and_length_after(std::string expr, int pos);
        int get_struct_pos(std::string &code);

        //operator.cpp
        std::string update_operator(std::string &code, const std::string &operator_symbol);
        std::string update_operator_equal(std::string &code, const std::string &operator_symbol);

        //parse.cpp
        void parse(std::string &code, std::string opt, int nestlevel, LINETYPES codeType);
        std::string print(int depth);
        void debug(int depth);
        //insert.cpp
        void insert(std::string &str, int pos);
    private:
        //library.cpp
        void library_check(std::string &str);
        void library_switch(std::string &str);

        //skip.cpp
        int skipPrefixElements(const std::string& str, int pos);
        int skipSuffixElements(const std::string& str, int pos);
        int skipPrefixElementsBackward(const std::string& str, int pos);
        int processIdentifier(const std::string& str, int pos);
        int processSuffixElements(std::string& str, int pos);
        int processExpressionBackward(const std::string& str, int pos);
        int findMatchingOpenBracket(const std::string& str, int closePos);

        //find.cpp
        int findVariableStart(const std::string &code, int operatorPos);
        int findExpressionEnd(const std::string &code, int startPos);
        
        //process.cpp
        int processCodeStart(std::string &code, LINETYPES codeType, std::string &opt);
        int checkEndType(std::string &code, int &endType, int &extraSentence, int returnFlg);
        int processDirectives(std::string &code);
        void processTempalteParams(const std::string &tempstr);
        std::string processTemplate_Modifiers(std::string &code);
        int determineBlockType(const std::string &code, std::string &token, char nextChar, int typeEndpos, std::string &prefixStr);
        void processBlock(std::string &code, int blockType, const std::string &token,
            int typeEndPos, const std::string &prefixStr, 
            LINETYPES codeType, const std::string &opt, int nestLevel,
            int &mainFunctionCreated, int &returnFlag);
        int findControlBlockEnd(const std::string &code);
        int processMacros(std::string &code, std::string &blockName);
        void processPermMacro(std::string &code, std::string &macroName);
        void processLoopMacro(std::string &macroName);
        void transferTemplateInfo(Albat *childNode, const std::string &tempstr);\
        void processSentence(std::string &code, int typeEndpos, int &returnFlg);
        void processTypeDeclaration(const std::string &typeStr, std::string &statement, int &returnFlag);

        //setup.cpp
        int setup_Line(std::string &str, std::string tp, std::string &typeStr, int &is_return);
        int setup_def(std::string &str);

        //print.cpp
        std::vector<std::pair<std::string, std::string>> get_def_input(std::string type_str, std::string vardef_str);
    };
#endif 