//
// Created by lei on 22-11-17.
//

#ifndef GPUPROGRAM_CMDLINEPARSER_INL
#define GPUPROGRAM_CMDLINEPARSER_INL

#include <cassert>
#include <string>
#include <cstring>
#include "cmdLineParser.hpp"

#if defined( WIN32 ) || defined( _WIN64 )
inline int strcasecmp(const char* c1, const char* c2) { return _stricmp(c1, c2); }
#endif // WIN32 || _WIN64

template<>
void cmdLineCleanUp<int>(int *t) {}

template<>
void cmdLineCleanUp<float>(float *t) {}

template<>
void cmdLineCleanUp<double>(double *t) {}

template<>
void cmdLineCleanUp<char *>(char **t) {
    if (*t) free(*t);
    *t = nullptr;
}

template<>
int cmdLineInitialize<int>() { return 0; }

template<>
float cmdLineInitialize<float>() { return 0.f; }

template<>
double cmdLineInitialize<double>() { return 0.; }

template<>
char *cmdLineInitialize<char *>() { return nullptr; }

template<>
int cmdLineCopy(int t) { return t; }

template<>
float cmdLineCopy(float t) { return t; }

template<>
double cmdLineCopy(double t) { return t; }

#if defined( WIN32 ) || defined( _WIN64 )
template< > char* cmdLineCopy(char* t) { return _strdup(t); }
#else // !WIN32 && !_WIN64

template<>
char *cmdLineCopy(char *t) { return strdup(t); }

#endif // WIN32 || _WIN64

template<>
int cmdLineStringToType(const char *str) { return atoi(str); }

template<>
float cmdLineStringToType(const char *str) { return float(atof(str)); }

template<>
double cmdLineStringToType(const char *str) { return double(atof(str)); }

#if defined( WIN32 ) || defined( _WIN64 )
template< > char* cmdLineStringToType(const char* str) { return _strdup(str); }
#else // !WIN32 && !_WIN64

template<>
char *cmdLineStringToType(const char *str) { return strdup(str); }

#endif // WIN32 || _WIN64


/////////////////////
// cmdLineReadable //
/////////////////////
#if defined( WIN32 ) || defined( _WIN64 )
inline cmdLineReadable::cmdLineReadable(const char* name) : set(false) { this->name = _strdup(name); }
#else // !WIN32 && !_WIN64

inline cmdLineReadable::cmdLineReadable(const char *name) : set(false) { this->name = strdup(name); }

#endif // WIN32 || _WIN64

inline cmdLineReadable::~cmdLineReadable() {
    if (name) free(name);
    name = nullptr;
}

inline int cmdLineReadable::read(int argc, char **argv) {
    set = true;
    return 0;
}

//////////////////////
// cmdLineParameter //
//////////////////////
template<class Type>
cmdLineParameter<Type>::cmdLineParameter(const char *name) : cmdLineReadable(name) {
    value = cmdLineInitialize<Type>();
}

template<class Type>
cmdLineParameter<Type>::cmdLineParameter(const char *name, Type v) : cmdLineReadable(name) {
    value = cmdLineCopy<Type>(v);
}

template<class Type>
cmdLineParameter<Type>::~cmdLineParameter() {
    cmdLineCleanUp(&value);
}

template<class Type>
int cmdLineParameter<Type>::read(int argc, char **argv) {
    if (argc > 0) {
        cmdLineCleanUp<Type>(&value);
        value = cmdLineStringToType<Type>(argv[0]);
        set = true;
        return 1;
    } else
        return 0;
}

inline void cmdLineParse(int argc, char **argv, cmdLineReadable **params) {
    while (argc > 0) {
        if (argv[0][0] == '-' && argv[0][1] == '-') {
            cmdLineReadable *readable = nullptr;
            for (int i = 0; params[i] != nullptr && readable == nullptr; i++) {
                if (!strcasecmp(params[i]->name, argv[0] + 2))
                    readable = params[i];
                if (i == argc) break;
            }
            if (readable) {
                /*{set = true; return 1;}*/
                int j = readable->read(argc - 1, argv + 1);
                argv += j, argc -= j;
            } else {
                WARN_P("Invalid option: ", argv[0]);
                exit(1);
            }
        } else {
            WARN_P("Parameter name should be of the form --<name>: ", argv[0]);
            exit(1);
        }
        ++argv, --argc;
    }
}

#endif //GPUPROGRAM_CMDLINEPARSER_INL
