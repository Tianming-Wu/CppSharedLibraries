#pragma once

#include "stringlist.h"
//#include <cstdarg>

#ifdef PARAMPARSER_DEBUG_ENABLED
    #include <iostream>
    #define pps_dbgopt(_name) using _dbgout = std::basic_ostream<_name, std::char_traits<_name>>; 
#else
    #define pps_dbgopt(_name)
#endif

namespace std {

enum {
    //解析逻辑（字符串）
    PARAMPARSER_CASEINSENSITIVE = 0x00000001, //0: 0001
    PARAMPARSER_ALLOWPARAMPACKAGING = 0x00000002, //0: 0010

    //解析逻辑（参数）
    PARAMPARSER_ALLOWSHORTNAME = 0x00000010, //1: 0001
    PARAMPARSER_AUTOGUESSSHORTNAME = 0x00000020, //1: 0010
    PARAMPARSER_REGARDFIRSTASGENERALCOMMAND = 0x00000040, //1: 0100

    //解析逻辑（前缀）
    PARAMPARSER_PREFIX_SLASH = 0x00000100, //2: 0001
    PARAMPARSER_PREFIX_HYPHEN = 0x00000200, //2: 0010
    PARAMPARSER_PREFIX_ALL = 0x00000300, //2: 0011

    //构造逻辑（引用）
    PARAMPARSER_COPY_TABLE = 0x00001000, //3: 0001

    //交互逻辑
    PARAMPARSER_AUTO_REPLY = 0x00010000, //4: 0001
};

//The default behavior.
#define PARAMPARSER_DEFAULT PARAMPARSER_PREFIX_ALL

template <typename _tcType>
class _tParamTableObject
{
    STRINGLIST_REGISTER_TYPE(_tcType)
public:
    _tagString name, shortname;
    size_t argumentCount = 0;
    bool argumentRequired = false;

    _tParamTableObject(_tagString _name, _tagString _shortname = 0, size_t argCnt = 0, bool argRequired = true)
        :name(_name), shortname(_shortname), argumentCount(argCnt), argumentRequired(argRequired)
    {
        if(argumentCount==0) argumentRequired = false;
    }
};

template <typename _tcType>
class _tParamTable
{
    STRINGLIST_REGISTER_TYPE(_tcType)
public:
    using _tagParamTableObject = _tParamTableObject<_tcType>;
    std::vector<_tagParamTableObject> _t;
    inline _tagParamTableObject operator [] (size_t pos) { return _t[pos]; }
};

template <typename _tcType>
class _tParamObject{
    STRINGLIST_REGISTER_TYPE(_tcType)
    using _tagParamTable = _tParamTable<_tcType>;
    using _tagParamTableObject = _tParamTableObject<_tcType>;

protected:
    _tagParamTable &table;

public:
    _tcType prefix; size_t tableMatch; bool matched; 
    _tagString name;
    std::vector<_tagString> arguments;
    _tParamObject(_tagString _name, _tagString _prefix = "", bool automatch = false, std::vector<_tagString> _arguments = std::vector<_tagString>())
        :name(_name), prefix(_prefix), arguments(_arguments)
    {
        if(automatch) {
            ///TODO: Search the list and match it.
        }
    }
};

template <typename _tcType>
class _tParamParser : protected _tagStringList<_tcType>
{
    STRINGLIST_REGISTER_TYPE(_tcType)
    using _tagParamTable = _tParamTable<_tcType>;
    using _tagParamTableObject = _tParamTableObject<_tcType>;
    using _tagParamArgument = _tParamArgument<_tcType>;
    using _tagParamObject = _tParamObject<_tcType>;

    pps_dbgopt(_tcType)

protected:
    unsigned int flags;
    _tagParamTable _paramtable;
    std::vector<_tagParamObject> params;

    static int instances = 0;

public:
    _tParamParser(_tagString src, _tagParamTable &paramtable, uint _flags = PARAMPARSER_DEFAULT)
        :flags(_flags)
    {
#       ifdef PARAMPARSER_DEBUG_ENABLED
            if(instances++) _dbgout<<"ParamParser warning: rebuilding object, which is potentially dangerous."<<std::endl; 
#       endif

        if(flags&PARAMPARSER_COPY_TABLE) _paramtable = ParamTable(paramtable);
            else _paramtable = paramtable;
        
        //write to the StringStream list first
        {
            if (!src.empty()) {
                _tagStringStream slstr; _tagString tmpstr;
                slstr << src;
                while (slstr >> tmpstr) {
                    list.push_back(tmpstr);
                }
            }
        }

        //paramTable availablity check
        {
            size_t ms = _paramtable._t.size();
            for(size_t s = 0; s < ms; s++) {
                _tagParamTableObject &ptobj = &_patamtable._t.at(s);
                if(ptobj.name.empty()) { throw std::string("ParamParser Init Error - Parsing ParamTable: property \'name\' cannot be empty"); }
                if(!flags&PARAMPARSER_ALLOWSHORTNAME) {
                    ptobj.shortname.clear();
                } /*else if(flags&PARAMPARSER_AUTOGUESSSHORTNAME) {
                    for(size_t p = s; p >= 0; --p) {
                            _tagParamTableObject &ptobjs = &_patamtable._t.at(s);
                            if(ptobjs.name[0]) {}
                    }

                }*/ //此功能暂时不打算实现
            }

        }

        //Parse CaseInsensitive Option Features.
        if(flags&PARAMPARSER_CASEINSENSITIVE) {
            size_t ms = _paramtable._t.size();
            for(size_t s = 0; s < ms; s++) {
                _tagParamTableObject &ptobj = &_patamtable._t.at(s);
                size_t strlen = ptobj.name.length();
                for(size_t stri = 0; stri < strlen; stri++) {
                    if(ptobj.name[stri]>='a'&&ptobj.name[stri]<='z') ptobj.name[stri] -= 32;
                }
                if(flags&PARAMPARSER_ALLOWSHORTNAME) {
                    size_t strlen = ptobj.shortname.length();
                    for(size_t stri = 0; stri < strlen; stri++) {
                        if(ptobj.shortname[stri]>='a'&&ptobj.shortname[stri]<='z') ptobj.shortname[stri] -= 32;
                    }
                }
            }
        }

        //The actural parsing part.
        auto fsi = ++list.begin(); //This is for the path not being parsed as an argument.
        bool flgArg = false, flgArgRequired = false; size_t flgArgRequiredNum = 0; 
        if(flags&PARAMPARSER_REGARDFIRSTASGENERALCOMMAND) {
            auto pobj = _tagParamObject(*fsi);
            params.push_back(pobj);
            fsi++; //let the parser begins at the second argument.
        }
        _tagParamObject *_ppot;
        for (size_t cnt = 0; fsi != list.end(); fsi++, cnt++) {
            _tagString content = (*fsi);
#           ifdef PARAMPARSER_DEBUG_ENABLED
                _dbgout<<"ParamParser[Debug]: current string: "<<content<<std::endl;
#           endif
            if((flags&PARAMPARSER_PREFIX_SLASH && content[0] == '/') || (flags&PARAMPARSER_PREFIX_HYPHEN && content[0] == '-')) {
                if(flgArgRequiredNum >= 0) {
                    if(flgArgRequired) {
                        delete _ppot;
                        throw std::string("ParamParser Parse Error: The argument(s) is(are) required, but not provided or not enough.");
                    } else {
                        params.push_back(*_ppot);
                        delete _ppot;
                    }
                }
                flgArgRequired = false, flgArgRequiredNum = 0, flgArg = true;
                _ppot = new _tagParamObject(content.substr(1), content[0], true);
                if(!_ppot->matched) throw (std::string("ParamParser Parse Error: Invalid option \'")+content+'\'');
                flgArgRequired = paramtable[_ppot->tableMatch].argumentRequired;
                flgArgRequiredNum = paramtable[_ppot->tableMatch].argumentCount;
            } else {
                if(!flgArg) throw std::string("ParamParser Parse Error: Invalid object(s) without option(s).");
                _ppot->arguments.push_back(content);
                if(--flgArgRequiredNum==0) { //Parsing finished, save and delete the current option.
                    params.push_back(*_ppot);
                    delete _ppot;
                    flgArg = false, flgArgRequired = false;
                }
            }
        }
    }

    //NOTE: no need to add the prefix.
    int hasArgument(_tagString argname) {
        int cnt = -1;
        for(auto ita = params.begin(); ita != params.end(); ita++) {
            if((*ita).name==argname) {
                cnt = (*ita.arguments.size());
            }
        }
        return cnt;
    }

    template<typename _argType>
    std::vector<_argType> GetArguments(_tagString argname) {
        std::vector<_argType> out;
        for(auto ita = params.begin(); ita != params.end(); ita++) {
            if((*ita).name==argname) {
                std::vector<_tagString> ift = (*ita).arguments;
                for(auto its = ift.begin(); its != ift.end(); its++) {
                    _tagStringStream istr; _argType argobj;
                    istr << ift; istr >> argobj;
                    out.push_back(argobj);
                }
                break;
            }
        }
        return out;
    }

    _tagString _AvailablePrefix() {
        return _tagString()+TEXT((flags&PARAMPARSER_PREFIX_SLASH)?("/"):(""))+TEXT((flags&PARAMPARSER_PREFIX_HYPHEN)?("-"):(""));
    }

};

using ParamParser = _tParamParser<char>;
using wParamParser = _tParamParser<wchar_t>;

using ParamTableObject = _tParamTableObject<char>;
using wParamTableObject = _tParamTableObject<wchar_t>;

using ParamTable = _tParamTable<char>;
using wParamTable = _tParamTable<wchar_t>;

template<typename _T>
_tParamTable<_T> _tMakeParamTable(initializer_list<_tParamTableObject<_T>> _l) {
    _tParamTable<_T> paramtable;
    for(auto &item : _l) {
        paramtable._t.push_back(item);
    }
    return paramtable;
}

#define makeParamTableA _tMakeParamTable<char>
#define makeParamTableW _tMakeParamTable<wchar_t>

//using makeParamTableA = _tMakeParamTable<char>;
//using makeParamTableW = _tMakeParamTable<wchar_t>;

// MINGW_NAME_AW(makeParamTable)

} //namespace std

//local clear
#undef _tagString
#undef _tagStringStream