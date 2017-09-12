#ifndef RANDOM_HPP_INCLUDED
#define RANDOM_HPP_INCLUDED

void quicksort(int *data, int left, int right);
int myrand();
void mysrand(unsigned int seed);

#define random(n) (myrand()%n)
#define random2(a,b) (a+myrand()%(b-a))


#endif // RANDOM_HPP_INCLUDED
