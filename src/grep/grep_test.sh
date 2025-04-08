#!/bin/bash
cp s21_grep testfiles
cd testfiles
FLAGS=("-e" "-i" "-v" "-c" "-l" "-n" "-h" "-s" "-f" "-o")
PAIRFLAGS=( "-iv" "-ic" "-ln" "-ov" "-nv" "-in" "-hs" "-io")
FILELIST=( "1.txt" "empty.txt"  "2.txt" )
extrafile="extra.txt"
regex="a"
regexfromfile="empty.txt"
count_of_success=0
count_of_failure=0
for flag in "${FLAGS[@]}"
do
  for file in "${FILELIST[@]}"
  do
    echo "Running test with flag: $flag, file: $file"
    if [ "$flag" == "-f" ]; then
      ./s21_grep "$flag" "$regexfromfile" "$file" > res1.txt
      grep "$flag" "$regexfromfile" "$file" > res2.txt
    else
      ./s21_grep "$flag" "$regex" "$file" >res1.txt
      grep "$flag" "$regex" "$file" > res2.txt
    fi
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
    echo "Running test with flag: $flag, file: $file $extrafile"
    if [ "$flag" == "-f" ]; then
      ./s21_grep "$flag" "$regexfromfile" "$file" "$extrafile" > res1.txt
      grep "$flag" "$regexfromfile" "$file" "$extrafile"  > res2.txt
    else
      ./s21_grep "$flag" "$regex" "$file" "$extrafile"  >res1.txt
      grep "$flag" "$regex" "$file" "$extrafile"  > res2.txt
    fi
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

for flag in "${PAIRFLAGS[@]}"
do
  for file in "${FILELIST[@]}"
  do
    echo "Running test with flag: $flag, file: $file"
      ./s21_grep "$flag" "$regex" "$file"   >res1.txt
      grep "$flag" "$regex" "$file"  > res2.txt
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

for flag in "${PAIRFLAGS[@]}"
do
  for file in "${FILELIST[@]}"
  do
    echo "Running test with flag: $flag, file: $file $extrafile"
      ./s21_grep "$flag" "$regex" "$file" "$extrafile"  >res1.txt
      grep "$flag" "$regex" "$file" "$extrafile"  > res2.txt
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
rm -rf s21_grep res1.txt res2.txt