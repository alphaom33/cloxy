if [ -f a.out ]; then
  rm ./a.out
fi
gcc *.c
./a.out $1