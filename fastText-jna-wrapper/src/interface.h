
#ifndef _INTERFACE_H_
#define _INTERFACE_H_

#ifndef PREDICT_LABEL_SIZE
#define PREDICT_LABEL_SIZE 15
#endif

extern "C" {
    /**
     * Retrieve FastText handler
     * */
    void *getHandler();
    /**
     * Load model
     * */
    void prepareModel(const void* handler, const char* path, int size);

    /*
    *   Predict sentence
    *   @param labelSize, should be PREDICT_LABEL_SIZE
    */
    void predictSentence(const void* handler, const char* sentence, char label[], int lableSize, double* score);
}

#endif




