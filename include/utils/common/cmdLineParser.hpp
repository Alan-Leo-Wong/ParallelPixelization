//
// Created by lei on 22-11-17.
//

#ifndef GPUPROGRAM_PARSER_HPP
#define GPUPROGRAM_PARSER_HPP

#ifdef WIN32
int strcasecmp(const char* c1, const char* c2);
#endif // WIN32

#include "../../infoLog/infoPrint.hpp"

class cmdLineReadable {
public:
    bool set;
    char *name;

    cmdLineReadable(const char *name);

    virtual ~cmdLineReadable();

    virtual int read(int argc, char **argv);
};

template<class Type>
void cmdLineCleanUp(Type *t);

template<class Type>
Type cmdLineInitialize();

template<class Type>
Type cmdLineCopy(Type t);

template<class Type>
Type cmdLineStringToType(const char *str);

template<class Type>
class cmdLineParameter : public cmdLineReadable {
public:
    Type value;

    cmdLineParameter(const char *name);

    cmdLineParameter(const char *name, Type v);

    ~cmdLineParameter() override;

    int read(int argc, char **argv) override;

    bool expectsArg() const { return true; }
};

void cmdLineParse(int argc, char **argv, cmdLineReadable **params);

#include "cmdLineParser.inl"

#endif //GPUPROGRAM_PARSER_HPP
