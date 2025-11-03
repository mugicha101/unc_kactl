[ -e ./a.out ] && rm ./a.out
[ -e ./main.txt ] && rm ./main.txt
g++ -std=c++17 -O3 -fpermissive -fassociative-math -ffast-math -ffloat-store ./main.cpp
if [ -e ./a.out ]; then
    [ -e ./output.txt ] && rm ./output.txt
    cp ./main.cpp ./main.txt
    cat ./input.txt | ./a.out | tee ./output
    [ -e ./output ] && mv ./output ./output.txt
fi