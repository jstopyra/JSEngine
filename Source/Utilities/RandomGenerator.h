//RandomGenerator.h

#ifndef __RANDOMGENERATOR_H__
#define __RANDOMGENERATOR_H__

#include <stdlib.h>     // srand, rand
#include <time.h>       // time
class RandomGenerator
{
public:
	RandomGenerator()
	{
		//setting a seed for the rand using the time of the computer.
		srand((unsigned int)time(nullptr));
	}
	~RandomGenerator() {};
private:

};

#define GENERATE_NEW_SEED RandomGenerator generator;


#endif // !__RANDOMGENERATOR_H__
