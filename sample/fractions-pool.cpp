#include "fractions-pool.h"

FractionsPool::FractionsPool() :
    fraction1(this)
{
}

void FractionsPool::init(void* parentGridElement)
{
    parent = parentGridElement;
}
