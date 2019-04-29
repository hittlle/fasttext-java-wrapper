#include "interface.h"
#include <cstring>
#include <mutex>
#include <vector>
#include <string>
#include <sstream>
#include "fasttext.h"

using namespace fasttext;

class Interface{
private:
    Interface() = default;
public:
    void loadModel(const std::string& modelPath);
    void predict(const std::string& sentence, std::string &label, double &score);
    static Interface* getInstance();
    ~Interface();
private:
    static Interface *_instance;
    fasttext::FastText _model;
    static std::mutex _mutex;
    static bool _loaded;
}; 

Interface* Interface::_instance = nullptr;
std::mutex Interface::_mutex;
bool Interface::_loaded = false;


Interface* Interface::getInstance() {
    if (nullptr == _instance) {
        std::lock_guard<std::mutex> guard(_mutex);
        if (nullptr == _instance) {
            _instance = new Interface();
        }
    }
    return _instance;
}
void Interface::loadModel(const std::string& modelPath) {
    if (!_loaded) {
        std::lock_guard<std::mutex> guard(_mutex);
        if (!_loaded) {
            _model.loadModel(modelPath);
            _loaded = true;
        }
    }
}

void Interface::predict(
    const std::string& sentence, 
    std::string& label, 
    double& score
    ) {
        label.clear();
        score = 0.0;
        if (sentence.empty()) {
            return;
        }
        std::vector<std::pair<real, std::string>> predictions;
        std::stringstream in(sentence);
        _model.predictLine(in, predictions, 1, 0.0);
        auto iterator = predictions.begin();
        if (iterator != predictions.end()) {
            score = iterator->first;
            label = iterator->second.c_str();
        }
}


extern "C" {
    void* getInstance() {
        Interface* pInstance = Interface::getInstance();
        return pInstance;
    }

    void prepareModel(const void* handler, const char* path, int size) {
        Interface *pModel = (Interface*)handler;
        std::string modelPath(path, size);
        pModel->loadModel(modelPath);
    }
    void predictSentence(const void* handler, const char* sentence, char label[], int labelSize, double* score){
        if (nullptr == handler || nullptr == sentence || nullptr == score||labelSize<= 0) {
            return;
        }
        Interface *pModel = (Interface*)handler;
        std::string clabel;
        double cscore;
        pModel->predict(sentence, clabel, cscore);
        strncpy(label, clabel.c_str(), clabel.size()<=labelSize?clabel.size():labelSize);
        *score = cscore;
    }
}
