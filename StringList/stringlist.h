#pragma once

#include <string>
#include <sstream>
#include <vector>

namespace std {

    template <typename _tcType>
    class _tagStringList {
        using _tagString = std::basic_string<_tcType, std::char_traits<_tcType>, std::allocator<_tcType>>;
        using _tagStringStream = std::basic_stringstream<_tcType, std::char_traits<_tcType>, std::allocator<_tcType>>;
    private:

    public:
        static const size_t npos = -1;
        struct size_t_d {
            size_t a, b;
            size_t_d(){};
            size_t_d(size_t _a,size_t _b):a(_a),b(_b){}
        };

        std::vector<_tagString> list;

        _tagStringList(_tagString __Source) {
            if (!__Source.empty()) {
                _tagStringStream slstr; _tagString tmpstr;
                slstr << __Source;
                while (slstr >> tmpstr) {
                    list.push_back(tmpstr);
                }
            }
        }

        size_t findItem(string _item, size_t beginItem = 0) {
            for(size_t i = 0; i < list.size(); i++) {
                if(list[i]==_item) return i;
            }
            return npos;
        }

        size_t_d findItemContent(string _itemc, size_t beginItem = 0, size_t beginContent = 0) {
            size_t lsp;
            for(size_t i = 0; i < list.size(); i++) {
                if((lsp = list[i].find(_itemc, beginContent)) != _tagString::npos) return size_t_d(i, lsp);
            }
            return size_t_d(npos, npos);
        }

        inline size_t size() { return list.size(); }

        inline _tagString operator [] (size_t pos) { return list[pos]; }
        inline _tagString at(size_t pos) { return list.at(pos); }

        using iterator = typename::std::vector<_tagString>::iterator;

        inline iterator begin() { return list.begin(); }
        inline iterator end() { return list.end(); }
    };

    using StringList = _tagStringList<char>;
    using wStringList = _tagStringList<wchar_t>;

} //namespace std

//local clear
#undef _tagString
#undef _tagStringStream