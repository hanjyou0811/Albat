#include "../albat.h"
#include "../../Utils/stringutils.h"
#include <iostream>

int Albat::processCodeStart(std::string &code, LINETYPES codeType, std::string &opt)
{
    StringUtils::ftrim(code);
    int endType = 0;

    if(codeType != LINETYPES::PROGRAM && code[0] == '{')
    {
        endType = 1;
        code = code.substr(1);
    }

    if(codeType == LINETYPES::PROGRAM)
    {
        endType = 2;
    }

    if(!opt.empty())
    {
        code = opt + code;
    }

    return endType;
}

int Albat::processDirectives(std::string &code)
{
    if (code.substr(0, 13) == "//interactive" && (isspace(code[13])|| code[13] == '\0'))
    {
        code = code.substr(13);
        return 1;
    }
    if(code.substr(0,9) == "//atcoder" && (isspace(code[9]) || code[9] == '\0'))
    {
        // libMan.insertStill.insert("ACL");
        code = code.substr(9);
        return 1;
    }
    return 0;
}

std::string Albat::processTemplate_Modifiers(std::string &code)
{
    std::string result;

    while(1)
    {
        auto [token, nextChar] = StringUtils::nextToken(code);
        if(token == "template")
        {
            int bracketCount = 0;
            int pos = 0;
            for(pos = 0;pos < code.size();pos++)
            {
                if (code[pos] == '<') bracketCount++;
                if (code[pos] == '>') bracketCount--;
                if (bracketCount == 0 && code[pos] == '>') {
                    pos++;
                    break;
                }
            }
            result += code.substr(0, pos) + " ";
            code = code.substr(pos);
            StringUtils::ftrim(code);
        }
        if(token == "inline")
        {
            result += "intline ";
            code = code.substr(6);
            StringUtils::ftrim(code);
            continue;
        }
        break;
    }
    return result;
}

void Albat::processTempalteParams(const std::string &tempstr)
{
    std::vector<std::string> declarations;
    std::string tmp = tempstr;

    while(1)
    {
        int temppos = -1;
        for(int i=0;i<tmp.size();i++)
        {
            if(tmp.substr(i, 8) == "template")
            {
                temppos = i;
            }
        }
        if(temppos == -1) break;
        declarations.push_back(tmp.substr(temppos));
        tmp = tmp.substr(0, temppos);
    }
    for(const std::string &decl : declarations)
    {
        std::string params = decl;
        StringUtils::trim_char(params, '<', '>');
        if(params.size() >= 2)
        {
            params = params.substr(1, params.size() - 2);
            std::vector<std::string> paramList = StringUtils::split_without_char(params, ',');
            for(int i=0;i<paramList.size();i++)
            {
                StringUtils::trim(paramList[i]);
                if(paramList[i].substr(0, 6) == "class ")
                {
                    paramList[i] = paramList[i].substr(5);
                    StringUtils::trim(paramList[i]);
                    typenames.insert(paramList[i]);
                }
                if(params.substr(0,9) == "typename ")
                {
                    paramList[i] = paramList[i].substr(8);
                    StringUtils::trim(paramList[i]);
                    typenames.insert(paramList[i]);
                }
            }
        }
    }
}

int Albat::determineBlockType(const std::string &code, std::string &token, char nextChar, int typeEndpos)
{
    int blockType = 0;
    if (typeEndpos >= 0 || code.substr(0, 8) == "operator")
    {
        if (isValidFunction(code))
        {
            blockType = 1;
            token = "function";
            return blockType;
        }
    }
    if (token == "if" && nextChar == '(') {
        blockType = 1;
    }
    else if (token == "for" && nextChar == '(') {
        blockType = 1;
    }
    else if (token == "while" && nextChar == '(') {
        blockType = 1;
    }
    else if ((token == "rep" || token == "REP") && nextChar == '(') {
        blockType = 1;
    }
    else if ((token == "rrep" || token == "RREP") && nextChar == '(') {
        blockType = 1;
    }
    else if (token == "fore" && nextChar == '(') {
        blockType = 1;
    }
    else if (token == "do_perm") {
        blockType = 1;
    }
    else if (token == "else") {
        blockType = 2;
        std::string nextPart = code.substr(4, 100);
        auto [nextToken, nextNextChar] = StringUtils::nextToken(nextPart);
        if (nextToken == "if" && nextNextChar == '(') {
            blockType = 1;
        }
    }
    else if (token == "do") {
        blockType = 2;
    }
    else if (token == "struct" || token == "class") {
        blockType = 3;
    }
    return blockType;
}

void Albat::processBlock(std::string &code, int blockType, const std::string &token,
    int typeEndPos, const std::string &prefixStr, 
    LINETYPES codeType, const std::string &opt, int nestLevel,
    int &mainFunctionCreated, int &returnFlag){
        returnFlag = 0;
        int blockEndpos =  0;
        if (blockType == 1 && token == "function") {
            for (blockEndpos = 0; blockEndpos < code.size(); blockEndpos++) {
                if (code[blockEndpos] == '{' || code[blockEndpos] == ';') {
                    break;
                }
            }
        }
        else if (blockType == 1) {
            blockEndpos = findControlBlockEnd(code);
        }
        else if (blockType == 2) {
            blockEndpos = token.size();
        }
        else if (blockType == 3) {
            // 構造体/クラス名を取得
            for (blockEndpos = 0; blockEndpos < code.size(); blockEndpos++) {
                if (code[blockEndpos] == '{' || code[blockEndpos] == ';') break;
            }
            
            std::string structDef = code.substr(0, blockEndpos);
            StringUtils::trim(structDef);
            std::vector<std::string> parts = StringUtils::split_without_char(structDef, ' ');
            
            // 型情報を追加
            if (prefixStr.empty()) {
                addVarType(parts.back());
            } else {
                addTempVarType(parts.back());
            }
        }
        else {
            blockEndpos = 0;
        }
        
        // ブロック名とブロック内容を取得
        std::string blockName = code.substr(0, blockEndpos);
        code = code.substr(blockEndpos);
        // ライブラリチェック
        library_check(blockName);
        
        // main関数の自動生成
        if (blockName.empty() && codeType == LINETYPES::PROGRAM && !mainFunctionCreated) {
            blockName = "int main()";
            mainFunctionCreated = true;
        }
         
        // マクロの展開処理
        if (processMacros(code, blockName)) {
            return;
        }
        // 子ノードを作成
        Albat *childNode = new Albat();
        childNode->mergeParent(this);
        
        // テンプレート情報を子ノードに渡す
        if (!prefixStr.empty()) {
            transferTemplateInfo(childNode, prefixStr);
        }
        // 子ノードの名前を設定しセットアップ
        childNode->name = prefixStr + blockName;
        childNode->parse(code, opt, nestLevel + 1, LINETYPES::BLOCKS);
        
        // 子ノードをツリーに追加
        nextindices.push_back(nextPtrs.size());
        nextPtrs.push_back(childNode);
        lines.push_back(prefixStr + blockName);
        // ブロックタイプを設定
        LINETYPES blockCodeType;
        if (token.substr(0, 5) == "while") {
            blockCodeType = LINETYPES::BLOCKW;
        } else {
            blockCodeType = LINETYPES::BLOCKS;
        }
        lineTypes.push_back(blockCodeType);
};

void Albat::transferTemplateInfo(Albat *childNode, const std::string &tempStr)
{
    std::string tmpStr = tempStr;
    std::vector<std::string> templateDecls;
    
    while (true) {
        int templatePos = -1;
        for (int i = 0; i < tmpStr.size(); i++) {
            if (tmpStr.substr(i, 8) == "template") {
                templatePos = i;
                break;
            }
        }
        
        if (templatePos == -1) break;
        
        templateDecls.push_back(tmpStr.substr(templatePos));
        tmpStr = tmpStr.substr(0, templatePos);
    }
    
    for (const std::string &decl : templateDecls) {
        std::string params = decl;
        StringUtils::trim_char(params, '<', '>');
        
        if (params.size() >= 2) {
            params = params.substr(1, params.size() - 2);
            std::vector<std::string> paramList = StringUtils::split_without_char(params, ',');
            
            for (std::string &param : paramList) {
                StringUtils::trim(param);
                
                if (param.substr(0, 6) == "class ") {
                    std::string typeName = param.substr(5);
                    StringUtils::trim(typeName);
                    childNode->tmptypenames.insert(typeName);
                }
                
                if (param.substr(0, 9) == "typename ") {
                    std::string typeName = param.substr(8);
                    StringUtils::trim(typeName);
                    childNode->tmptypenames.insert(typeName);
                }
            }
        }
    }
}

void Albat::processSentence(std::string &code, int typeEndpos, int &returnFlag)
{
    // カウンタ初期化
    int bracketCount = 0;
    int squareCount = 0;
    int braceCount = 0;
    bool inSingleQuote = false;
    bool inDoubleQuote = false;
    
    // 文の終端を見つける
    int startPos = (typeEndpos != -1) ? typeEndpos : 0;
    StringUtils::ftrim(code);
    if(code.empty()) return;
    int endPos = startPos;
    
    for (; endPos < code.size(); endPos++) {
        // リテラル内のエスケープ処理
        if ((inSingleQuote || inDoubleQuote) && code[endPos] == '\\') {
            endPos++;
            continue;
        }
        
        // クオート処理
        if (!inDoubleQuote && code[endPos] == '\'') inSingleQuote = !inSingleQuote;
        if (!inSingleQuote && code[endPos] == '"') inDoubleQuote = !inDoubleQuote;
        
        // リテラル外での括弧のカウント
        if (!inSingleQuote && !inDoubleQuote) {
            if (code[endPos] == '(') bracketCount++;
            if (code[endPos] == ')') bracketCount--;
            if (code[endPos] == '[') squareCount++;
            if (code[endPos] == ']') squareCount--;
            if (code[endPos] == '{') braceCount++;
            if (code[endPos] == '}') braceCount--;
        }
        
        // 文の終了判定
        if (bracketCount == 0 && squareCount == 0 && braceCount == 0 && 
            !inSingleQuote && !inDoubleQuote) {
            
            bool isExplicit = false;
            if (code.substr(0, 8) == "explicit" && isspace(code[8])) {
                isExplicit = true;
            }
            if ((!isExplicit && code[endPos] == ',') || code[endPos] == ';') {
                // 文を抽出
                std::string statement = code.substr(startPos, endPos + 1 - startPos);
                
                if (StringUtils::strpos_exlit(statement, "@") >= 0) {
                    while (code[endPos] != ';') endPos++;
                    statement = code.substr(startPos, endPos + 1 - startPos);
                }
                
                statement[statement.size() - 1] = ';';
                if (typeEndpos >= 0) {
                    processTypeDeclaration(code.substr(0, typeEndpos), statement, returnFlag);
                }
                else {
                    // 普通の文の処理
                    int ret = setup_Line(statement, "", returnFlag);
                    if (ret) {
                        nextindices.push_back(-1);
                        lines.push_back(statement);
                        lineTypes.push_back(LINETYPES::SENTENCE);
                    }
                }
                
                if (code[endPos] == ',')
                {
                    startPos = endPos + 1;
                    continue;
                } else
                {
                    endPos++;
                    break;
                }
            }
        }
    }
    
    code = code.substr(endPos);
}

void Albat::processTypeDeclaration(const std::string &typeStr, std::string &statement, int &returnFlg)
{
    std::vector<std::pair<std::string, std::string>> declarations = get_def_input(typeStr, statement);
    
    for (const auto &[declarationType, definition] : declarations) {
        std::string tmp = definition;
        if (declarationType == "input") {
            int ret = setup_Line(tmp, declarationType, returnFlg);
            if (ret) {
                nextindices.push_back(-1);
                lines.push_back(definition);
                lineTypes.push_back(LINETYPES::INPUT);
            }
        }
        else {
            setup_def(tmp);
            nextindices.push_back(-1);
            lines.push_back(definition);
            lineTypes.push_back(LINETYPES::SENTENCE);
        }
    }
}