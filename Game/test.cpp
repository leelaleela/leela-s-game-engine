#include <iostream>


void doCall(void(&f)(int))
{
	f(42);
}
