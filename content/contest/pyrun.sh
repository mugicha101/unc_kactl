[ -e ./output.txt ] && rm ./output.txt
cat ./input.txt | python3 main.py | tee ./output
[ -e ./output ] && mv ./output ./output.txt