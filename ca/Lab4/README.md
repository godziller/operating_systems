# Compiling

After battling with the c compiler not recognizing the thread library, this worked for me.
gcc -D_GNU_SOURCE -pthread -o thread_printer tread_printer.c
