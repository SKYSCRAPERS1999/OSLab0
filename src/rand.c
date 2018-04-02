#include <am.h>
static unsigned int seed = 1;
void srand(int c){
	seed = c;
}
unsigned int rand(){
	/* Algorithm "xor" from p. 4 of Marsaglia, "Xorshift RNGs" */
	unsigned int x = seed;
	x ^= x << 13;
	x ^= x >> 17;
	x ^= x << 5;
	seed = x;
	return seed;
}
