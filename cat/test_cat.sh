#!/bin/bash
make all

echo " "
echo "Response to wrong flag:"
./s21_cat -w $test_file
echo " "
echo "Response to wrong file:"
./s21_cat nothing.txt

for flag in "-b" "-e" "-n" "-s" "-t" "-v" "-bv" "-ns" "-tn" "-es" "--number-nonblank" "--number" "--squeeze-blank"
do
    test_num=1
    echo " "
    echo "Flags: $flag:"
    for test_file in  "test/test1.txt" "test/test2.txt" "test/test3.txt" "test/test4.txt" "test/test5.txt" "test/test6.txt"
    do
        ./s21_cat $flag $test_file > 1.txt
        cat $flag $test_file > 2.txt
        if ! cmp -s 1.txt 2.txt ; then
            echo "Test $test_num: Fail"
        else
            echo "Test $test_num: Success"
        fi
        rm 1.txt 2.txt
        test_num=$(( $test_num + 1 ))
    done
done
echo " "
echo " "