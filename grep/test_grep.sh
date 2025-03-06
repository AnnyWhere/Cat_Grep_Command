#!/bin/bash
make all

echo " "
echo "Response to wrong flag:"
./s21_grep -y aaa $test_file
echo " "
echo "Response to wrong file:"
./s21_grep aaa nothing.txt

# Массив флагов
flags=("-e" "-i" "-v" "-c" "-l" "-n" "-h" "-s" "-f" "-o")

# Тестирование обычных флагов
for flag in "-e" "-i" "-v" "-c" "-l" "-n" "-h" "-s" "-f" "-o"
do
    test_num=1
    echo " "
    echo "Flags: $flag:"
    for test_file in "test/test1.txt" "test/test2.txt" "test/test3.txt" "test/test4.txt" "test/test5.txt" "test/test6.txt"; do
        for pattern in "do", "a", "@" 
        do
            ./s21_grep $flag $pattern $test_file > 1.txt
            grep $flag $pattern $test_file > 2.txt
            if ! cmp -s 1.txt 2.txt; then
                echo "Test $test_num: Fail"
                echo "$pattern $test_file"
            else
                echo "Test $test_num: Success"
            fi
            rm 1.txt 2.txt
            test_num=$(( test_num + 1 ))
        done
    done
done

echo " "

# # Тестирование смежных флагов
# for (( i=0; i<${#flags[@]}; i++ )); do
#     for (( j=i+1; j<${#flags[@]}; j++ )); do
#         combined_flag="${flags[i]}${flags[j]}"
#         test_num=1
#         echo " "
#         echo "Combined Flags: $combined_flag:"
#         for test_file in "test/test1.txt" "test/test2.txt" "test/test3.txt" "test/test4.txt" "test/test5.txt" "test/test6.txt"; do
#             ./s21_grep $combined_flag $test_file > 1.txt
#             grep $combined_flag $test_file > 2.txt
#             if ! cmp -s 1.txt 2.txt; then
#                 echo "Test $test_num: Fail"
#             else
#                 echo "Test $test_num: Success"
#             fi
#             rm 1.txt 2.txt
#             test_num=$(( test_num + 1 ))
#         done
#     done
# done

echo " "
echo " "