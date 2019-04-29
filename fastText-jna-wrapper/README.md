1, Why this?
Facebook fasttext is an excellent text classification tool, and I love it. You know what? I tried 
scikit-learn SVM, Random Forest and Logistic Regression algorithms to train multiclass classification
models for short texts, and fasttext turned out to be the best in my environment. After the training 
and tuning phases, I started to integrate the model into my backend service. My backend service is 
Java based, but fasttext is written in C++ 11. So I had to turn to JNI or JNA for help. I chosed JNA
for its simplicity. And I also modified the fasttext source code to expose three C-sytle functions
(extern "C") for JNA. That's it.

2, How to build the dynamic library and run the test?
Just execute

	bash build.sh

and you'll find the libfasttext.so dynamic library, and you can run the test this way

	javac -cp "jna-5.3.0.jar:." FastTextJNAWrapper.java

	java -cp "jna-5.3.0.jar:." FastTextJNAWrapper

