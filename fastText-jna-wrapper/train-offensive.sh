
cat offensive-class.data | shuf > sentiment.data

head -n 19826 sentiment.data > data.train
tail -n 4956 sentiment.data > data.valid

./fasttext supervised -input data.train -output model 
./fasttext test model.bin data.valid

./fasttext supervised -input data.train -output model -epoch 4 -lr 0.35 -loss hs
./fasttext test model.bin data.valid

./fasttext supervised -input data.train -output model -epoch 4 -lr 0.35 -loss one-vs-all
./fasttext test model.bin data.valid

./fasttext supervised -input data.train -output model -epoch 4 -lr 0.35 -loss one-vs-all  -wordNgrams 1
./fasttext test model.bin data.valid

./fasttext supervised -input data.train -output model -epoch 6 -lr 0.35 -loss one-vs-all -wordNgrams 2
./fasttext test model.bin data.valid

./fasttext supervised -input data.train -output model -epoch 6 -lr 0.35 -loss one-vs-all -wordNgrams 3
./fasttext test model.bin data.valid

./fasttext supervised -input data.train -output model -epoch 4 -lr 0.35 -loss ova 
./fasttext test model.bin data.valid
