TARGET=pi

n_time=10000

while [ $n_time -le 10000000000 ]
do  
    FILE="result_${n_time}.txt" 
    printf "" > $FILE;
    for n_thread in {1..10};
    do
        echo "Thread: ${n_thread}" >> $FILE;
        eval "(time ./$TARGET ${n_thread} $n_time >> $FILE) 2>&1 | grep 'real' >> $FILE "  
        echo "-------------------" >> $FILE;
    done
    (( n_time*=10 ))
done
