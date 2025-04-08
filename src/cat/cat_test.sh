#!/bin/bash
cp s21_cat testfiles
cd testfiles
FLAGS=("-b" "--number-nonblank" "-e" "-E" "-n" "--number" "-s" "--squeeze-blank" "-t" "-T")
FILELIST=( "1.txt" "empty.txt"  "2.txt" )
extrafile="extra.txt"
count_of_success=0
count_of_failure=0
for flag in "${FLAGS[@]}"
do
  for file in "${FILELIST[@]}"
  do
    echo "Running test with flag: $flag, file: $file"
   
      ./s21_cat "$flag" "$file" >res1.txt
      cat "$flag" "$file" > res2.txt
    # valgrind --tool=memcheck --leak-check=yes  ./s21_cat "$flag" "$file"
    #echo Press enter to continue
    #read
    if diff res1.txt res2.txt >/dev/null; then
      echo -e Test passed
      ((count_of_success++))
    else
      echo -e Test failed
      echo "Error output for test with flag: $flag, file: $file"
      echo 
      ((count_of_failure++))
    fi
  done
done

for flag in "${FLAGS[@]}"
do
  for file in "${FILELIST[@]}"
  do
    echo "Running test with flag: $flag, files: $file $extrafile"
   
      ./s21_cat "$flag" "$file" "$extrafile"  >res1.txt
      cat "$flag" "$file" "$extrafile"  > res2.txt
    #valgrind --tool=memcheck --leak-check=yes  ./s21_cat "$flag" "$file" "$extrafile"
    #echo Press enter to continue
    #read
    if diff res1.txt res2.txt >/dev/null; then
      echo -e Test passed
      ((count_of_success++))
    else
      echo -e Test failed
      echo "Error output for test with flag: $flag, file: $file"
      echo 
      ((count_of_failure++))
    fi
  done
done



echo -e Successful tests: $count_of_success
echo -e Failed tests: $count_of_failure
rm -rf s21_cat res1.txt res2.txt
