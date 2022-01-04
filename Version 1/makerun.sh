clear
gcc -Werror -Wall main.c  -o main.out -fopenmp
if [ $? -eq 0 ]
then
    clear
    ./main.out
fi
