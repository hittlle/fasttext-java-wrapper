#copy
#cp /opt/sentiment.csv sentiment.csv
#delete comma
#sed -e "s/,/ /g" sentiment.csv > sentiment.txt
rm -rf data.train
rm -rf data.valid
rm -rf model.bin
#shuffle
cat small-training-samples.data| shuf > off.data
#split training samples; train VS valid = 0.8:0.2

declare -a labels=("label__offensive" "label__hate" "label__neutral")

for label in "${labels[@]}"
do
	cat off.data | grep ${label} > ${label}.data
	line_count=$(wc -l < ${label}.data)
	echo "$label, $line_count"
	train=$(echo "${line_count} * 0.8"|bc)
	valid=$(echo "${line_count} * 0.2"|bc)
	train=${train%.*}
	valid=${valid%.*}
	head -n $train ${label}.data >> data.train
	tail -n $valid ${label}.data >> data.valid
	#rm ${label}.data
done

./fasttext supervised -input data.train -output model -epoch 20 -lr 1.0 -loss one-vs-all
./fasttext test model.bin data.valid
