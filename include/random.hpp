#ifndef RANDOM_HPP_INCLUDED
#define RANDOM_HPP_INCLUDED

///gyors rendezesi algoritmus
void quicksort(int *data, int left, int right);

///random szamgenerator, amit erdekessegbol raktam be.
int myrand();

///a szamgenerator srand-ja.
void mysrand(unsigned int seed);

///ha a [0;n-1] intervallumban kerunk random szamot, ezt lehet hasznalni
#define random(n) (myrand()%n)
///ha a [a;b-1] intervallumban kerunk random szamot, ezt lehet hasznalni
#define random2(a,b) (a+myrand()%(b-a))


#endif // RANDOM_HPP_INCLUDED
