# LogFx User Manual
Cpp LogFx Modern code style remake (2023.05.01), author Tianming (2023).

A stringed version of logfx, supporting both ANSI and **Unicode** (horray!).

The library is designed for simpler logging, and is highly **configurable**.

## Introduction
Hello! Thank you for using the class std::LogFx.

As its name, this is a logger function. You can easily design its behavior without editing the file (module).

# How to use in your code

## Initliazation

The ANSI version is `LogFx` (aka `_tLogFx<char>`), and the UNICODE version is `wLogFx` (aka `_tLogFx<wchar_t>`). To simplify, the examples are using the ANSI version.

Note that you need to use `L"const char"`, `std::wstring` and `std::wstringstream` for conversation to `wLogFx`.

### If you are not able to use c++ latest module style:
```cpp
#define LOGFX_USE_CPP11 //This will remove module definition.
#include "logfx.ixx"

int main() {
    std::LogFx logfx("main.log");
}

```

### If you are using c++20 or later, and enabled expiremental `module`:
```cpp
import LogFx;

int main() {
    std::LogFx logfx("main.log");
}
```
If you want to apply `append` opening, use this:
```cpp
    std::LogFx logfx("main.log", ios::app);
```

The filename is a `std::string` object, so it is simple to modify.

you can call `LogFx::timestamp()` to get a timestamp without an initialized object, since it is *static*.
You can use this function to customize your filename and make it relative to the time.

You can also give in an initalized `std::basic_ofstream<>` object, but it must match the charater wideness (char or wchar_t) the LogFx or wLogFx uses.
In this case, the LogFx **WILL NOT** close and delete your stream, so make sure you close it manually. Do not call LogFx after you closed the stream, since I have no idea if you closed the stream.
```cpp
import LogFx;
import std.core;

int main() {
    std::ofstream ofs("inits.ect", ios::app);
    std::LogFx logfx(&ofs);
}
```

## Writing In
The library gives three ways to write in, `write()`, `msg()`, and another will be introduced later.

Table of content:
 - [int **write**(std::string)](#int-writestdstring)
 - [template<typename _tp> int **write**(_tp cont)](#templatetypename-_tp-int-write_tp-cont)
 - [**\[\[Deprecated\]\]** template<class... DT> int **write_t**(const char* \_Format, DT... \_Data)](#deprecated-templateclass-dt-int-write_tconst-char-_format-dt-_data)
 - [int **msg**(std::string)](#int-msgstdstring)
 - [int **msg**(std::string,std::string)](#int-msgstdstring-stdstring)
 - [int **endl**()](#int-endl)
 - [std::stringstream* **stream**()](#stdstringstream-stream-int-writestream-and-int-writestreamstdstringstream-wistream)
   - [int **writeStream**()](#stdstringstream-stream-int-writestream-and-int-writestreamstdstringstream-wistream)
   - [int **writeStream**(std::stringstream wistream)](#stdstringstream-stream-int-writestream-and-int-writestreamstdstringstream-wistream)
 - [static std::string **timestamp**()](#static-stdstring-timestamp)
 - [static std::string **timerfilename**(std::string prefix, std::string suffix, std::string extname)](#static-stdstring-timerfilenamestdstring-prefix-stdstring-suffix-stdstring-extname)


## `int write(std::string)`
Function `int write(std::string message)` will not add a timestamp or any other thing, it just transfer your string into `std::ofstream`.
If no error occoured, the function returns `0`.
```cpp
int main() {
    std::LogFx logfx("main.log");
    logfx.write("this is a message\nlol");
    return 0;
}
```
_output (file "main.log")_
```
this is a message
lol
```

## `template<typename _tp> int write(_tp cont)`
This function calls stringstream to convert `cont` into `std::string`. The `_tp` does not require you to specify (will be recognized by compiler), but it must be available to `std::stringstream`. If it is a user defined struct/class, it must have oprtator `<<` to ostream&.
```cpp
int main() {
    std::LogFx logfx("main.log");
    logfx.write(114514);
}
```
_output (file "main.log")_
```
114514
```

## **\[\[Deprecated\]\]** template<class... DT> int write_t(const char* _Format, DT... _Data)
**\[[Deprecated]\]**
Function `template<class... DT> int write_t(const char* __restrict__ _Format, DT... _Data)` is similar to `printf()`. The function returns like `printf()`.

_This function is deprecated and it now always returns 0._
```cpp
int main() {
    double lfs = 3.1234132;
    std::LogFx logfx("main.log");
    logfx.write_t("This is a double value: %5lf\n", lfs);
}
```
_output (file "main.log")_ [no]

```
#NOTE: _Unable to work in the current version._
```

## int msg(std::string)
This function adds a timestamp in format `[YYYY/MM/DD HH:MM:SS]` before your message, and will start a new line after it. If no errors occured, the function returns `0`.
```cpp
int main() {
    std::LogFx logfx("main.log");
    logfx.msg("help!");
}
```
_output (file "main.log")_
```
[2023/05/07 21:23:49] help!

```

## int msg(std::string, std::string)
This function adds another stamp showing level after the timestamp and before your message. It also adds brackets.
```cpp
int main() {
    std::LogFx logfx("main.log");
    logfx.msg("help!","INFO");
}
```
_output (file "main.log")_
```
[2023/05/07 21:25:38] [INFO] help!

```

## int endl()
This function starts a new line. 
>It is much more readable and elegant than `write(std::endl)` :D
```cpp
int main() {
    std::LogFx logfx("main.log");
    logfx.write("orig");
    logfx.endl();
}
```
_output (file "main.log")_
```
orig

```

## "std::stringstream* stream()", "int writeStream()" and "int writeStream(std::stringstream& wistream)"
This function returns the pointer of a `std::basic_stringstream<typename _type>` object that you can write into it.

The changes you made to the stringstream will not automatically written into the file. Use `writeStream()` with no parameter to write to file and **clear the stringstream**.

If you recall this function, the stream will be flushed and cleared.
```cpp
#include <sstream>
int main() {
    std::LogFx logfx("main.log");
    std::stringstream *strstr = logfx.stream();
    (*strstr)<<"Information: "<<30342<<std::endl;
    logfx.writeStream(); //This actually clears it.
    (*strstr)<<"ID: "<<0<<std::endl<<std::endl;
    logfx.writeStream();
}
```
_output (file "main.log")_
```
Information: 30342
ID: 0


```
The function `writeStream(std::stringstream&)` writes the content user-specified stringstream object into the file. In this case, the stringstream won't be cleared.
```cpp
#include <sstream>
int main() {
    std::LogFx logfx("main.log");
    std::stringstream strstr;
    strstr<<"Userdefined"<<13234;
    logfx.writeStream(strstr);
    logfx.endl();
    strstr<<"ns"<<std::endl;
    logfx.writeStream(strstr);
}
```
_output (file "main.log")_
```
Userdefined13234
Userdefined13234ns

```

## static std::string timestamp()
This function returns a timestamp in `std::string` in style "YY/MM/DD HH:MM:SS".
```cpp
#include <iostream>
int main() {
    std::cout<<std::LogFx::timestamp()<<std::endl;
}
```
_output (terminal)_
```
2023/05/07 22:09:35
```

## static std::string timerfilename(std::string prefix, std::string suffix, std::string extname)
**[Detailed]**
This function make a logger filename with three parameters, can be empty.

time format: `"yyyymmdd_hhmmss"`. Use `setTimeFormat_timerfilename(std::string)` to change this, but it should be like `"%02d%02d%02d_%02d%02d%02d"`. 
> The actual parser will be included in the later versions, but it is likely to make the program logs much slower. This is not a threaded function and it may be much slower than other powerful libraries such as log4j.

Parameters:
 - `prefix` : If is not empty, this will be conneted before the time and is seperated with '_'.
 - `suffix` : If is not empty, this will be conneted after the time and is seperated with '_'.
 - `extname` : This will specify the extname of the file. **DO NOT** add the dot(`.`). If empty, it is `"log"`.

You can just send `""` if you are only using the parameter behind it.

Here is an example showing all the cases:
```cpp
#include <iostream>
int main() {
    std::cout<<std::LogFx::timerfilename()<<std::endl;
    std::cout<<std::LogFx::timerfilename("prefix")<<std::endl;
    std::cout<<std::LogFx::timerfilename("", "suffix")<<std::endl;
    std::cout<<std::LogFx::timerfilename("", "", "txt")<<std::endl;
    std::cout<<std::LogFx::timerfilename("prefix", "suffix")<<std::endl;
    std::cout<<std::LogFx::timerfilename("logfx", "", "tlog")<<std::endl;
}
```
_output (terminal)_
```
20230508_011331.log
prefix_20230508_011331.log
20230508_011331_suffix.log
20230508_011331.txt
prefix_20230508_011331_suffix.log
logfx_20230508_011331.tlog
```