#pragma once

#ifdef LOGFX_USE_CPP11
    #define export 
#else
    export module LogFx;
#endif

// ---------------------------------------------------
// Cpp LogFx Modern code style remake (2023.05.01), author Tianming (2023).
// A stringed version of logfx, supporting both ANSI and Unicode (horray!).

// ---------------------------------------------------
// Hello! Welcome to use the class LogFx.
// The library is designed for simpler logging, and is highly configurable.
// The User Manual is located in "logfx.md".

#include <string>
#include <cstdio>
#include <sstream>
#include <fstream>
#include <ctime>

namespace std {

    
    template<typename _ct>
    export class _tLogFx {
        using _tagString = class std::basic_string<_ct, std::char_traits<_ct>, std::allocator<_ct>>;
        using _tagStringStream = class std::basic_stringstream<_ct, std::char_traits<_ct>, std::allocator<_ct>>;
        using _tagOfStream = class std::basic_ofstream<_ct, char_traits<_ct>>;
#if _ct == char
#define _tagprintf printf;
#define _tagsprintf_s sprintf_s;
#define fxText(quote) quote
//#define _tagOfStream std::ofstream
#else
#define _tagprintf wprintf;
#define _tagsprintf_s swprintf_s;
#define fxText(quote) L##quote
//#define _tagOfStream std::ofwstream
#endif
    private:
        _tagOfStream* fileStream; bool fileStreamArrayed = false;
        _tagStringStream *strStream; bool strStreamAvailable = false;
        bool Available = false;
    public:
        _tLogFx(_tagString Filename) {
            fileStream = new _tagOfStream(Filename);
            fileStreamArrayed = true;
            if(!fileStream->bad()) Available = true;
        }

        _tLogFx(_tagString Filename, std::ios_base::openmode _Mode, int _Prot) {
            fileStream = new _tagOfStream(Filename, _Mode, _Prot);
            fileStreamArrayed = true;
            if(!fileStream->bad()) Available = true;
        }

        _tLogFx(_tagOfStream *targetOfStream) {
            fileStream = targetOfStream;
            if(!fileStream->bad()) Available = true;
        }

        ~_tLogFx() {
            if(fileStreamArrayed) {
                fileStream->close();
                //delete[] fileStream;
            }
            if(strStreamAvailable) {
                //delete[] strStream;
            }
        }

        int write(_tagString message) {
            (*fileStream)<<message;
            return 0;
        }

        template<typename _tp>
            int write(_tp cont) {
                _tagStringStream sst; sst<<cont;
                (*fileStream)<<_tagString(sst.str());
                return 0;
            }

        int msg(_tagString msgstr) {
            (*fileStream)<<'['<<timestamp()<<"] "<<msgstr<<std::endl;
            return 0;
        }

        int msg(_tagString msgstr, _tagString level) {
            (*fileStream)<<'['<<timestamp()<<"] ["<<level<<"] "<<msgstr;
            return 0;
        }

        int endl() {
            (*fileStream)<<std::endl;
            return 0;
        }

        _tagStringStream* stream() {
            if(!strStreamAvailable) {
                strStream = new _tagStringStream;
                strStreamAvailable = true;
            } else {
                strStream->str(""); strStream->clear();
            }
            return strStream;
        }

        int writeStream(_tagStringStream &wistream) {
            (*fileStream)<<_tagString(wistream.str());
            return 0;
        }

        int writeStream() {
            if(!strStreamAvailable) return 1;
            (*fileStream)<<_tagString(strStream->str());
            strStream->str(""); strStream->clear();
            return 0;
        }

        /*
        template<class... _args>
        int write_t(const _ct* _Format, _args... _Args) {
            _ct *tmps = new _ct[256];
            int ret = _tagsprintf_s(tmps, _Format, _Args...);
            _tagString strtmps(tmps); delete[] tmps;
            write(strtmps);
            return 0; //return ret;
        }*/

        static _tagString timestamp() {
            time_t tm0 = time(0);
            tm _tm;
            localtime_s(&_tm, &tm0);

            char _TimeBuffer[32];
            sprintf_s(_TimeBuffer, 32, "%04d/%02d/%02d %02d:%02d:%02d",
                _tm.tm_year + 1900, _tm.tm_mon + 1, _tm.tm_mday,
                _tm.tm_hour, _tm.tm_min, _tm.tm_sec
            );
            return _tagString(_TimeBuffer);
        }

        static _tagString timerfilename(_tagString prefix = "", _tagString suffix = "", _tagString extname = "") {
            time_t tm0 = time(0);
            tm _tm;
            localtime_s(&_tm, &tm0);

            char _TimeBuffer[32];
            sprintf_s(_TimeBuffer, 32, "%04d%02d%02d_%02d%02d%02d",
                _tm.tm_year + 1900, _tm.tm_mon + 1, _tm.tm_mday,
                _tm.tm_hour, _tm.tm_min, _tm.tm_sec
            );
            _tagString filename = prefix + (prefix.empty()?"":"_") + _tagString(_TimeBuffer) + (suffix.empty()?"":"_") + suffix + (extname.empty()?".log":_tagString(".")+extname);
            return filename;
        }

        bool isAvailable() const { return Available; }
        bool isfileStreamArrayed() const { return isfileStreamArrayed; }
        bool isStreamAvailable() const { return strStreamAvailable; }
        bool bad() const { return fileStream->bad(); }

        
    };

    using LogFx = _tLogFx<char>;
    using wLogFx = _tLogFx<wchar_t>;

/*
#ifndef LogFx_NoDefIO
    template <typename _cw, typename _ts>
        std::_tLogFx<_cw> operator << (std::_tLogFx<_cw> &logfx, _ts Ios) {
            std::endl;
            if(Ios == std::endl) {

            } else {
                std::_tLogFx<_cw>::_tagStringStream sts;
            sts<<
            }
            
        }
#endif
*/
    

} //namespace std