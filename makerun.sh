clear
gcc -Werror -Wall mainv2.c -o mainv2.out -fopenmp
if [ $? -eq 0 ]
then
    clear
    ./mainv2.out
fi
