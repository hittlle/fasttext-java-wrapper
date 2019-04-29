#include <iostream>
#include <string>
#include <dlfcn.h>
#include "src/interface.h"

using namespace std;
typedef void* (*pFunc)();
typedef void (*prepareFunc)(void*, const char*);
typedef void (*predictFunc)(void*, const char*, char*, int, double*);
int main(int argc, char **argv) {
	const char* modelPath = "model.bin";
    const char* sentence = "this is really stupid and shity";
    void *pLib = dlopen("libfasttext.so", RTLD_LOCAL | RTLD_LAZY);
    if (nullptr == pLib) {
        std::cerr<<dlerror()<<std::endl;
        return 0;
    }
    pFunc func  = (pFunc)dlsym(pLib, "getInstance");
    if (nullptr == func) {
        std::cerr<<dlerror()<<std::endl;
        return 0;
    }
    void *handler = func();
    prepareFunc prepareModel = (prepareFunc)dlsym(pLib, "prepareModel");
    if (nullptr == prepareModel) {
        std::cerr<<dlerror()<<std::endl;
        return 0;
    }
    prepareModel(handler, modelPath);
    char label[20] = {0};
    double score;
    predictFunc predictSent = (predictFunc)dlsym(pLib, "predictSentence");
    if (nullptr == predictSent) {
        std::cerr<<dlerror()<<std::endl;
        return 0;
    }
    predictSent(handler, sentence, label,PREDICT_LABEL_SIZE, &score);
    std::cout<<label<<" "<<score<<std::endl;
    dlclose(pLib);
    return 0;
}
