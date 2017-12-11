#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define NUM_GENS 10
#define NUM_ORGANISMS 100


float probCrossover = 0.25;
float probPoint = 0.01;
int numCaps = 5;
float caps[] = { 100, 150, 220, 330, 470, 680, 1000 }; //caps in picofarads
float goalCap = 425; //Goal capacitance in picofards

typedef long int chromosome;

typedef struct Organisms {
	chromosome genes;
	float value; //capacitance in picofarads
	float fitness; //difference from goal
	float lowerBound;
	float upperBound;
} Organism;


Organism parents[NUM_ORGANISMS];
Organism children[NUM_ORGANISMS];
Organism bestSolution;

chromosome generateChromosome(){
	chromosome genes;

	return genes;
}

float fitnessFunction(float capacitance){
	float fitness;
	float difference = fabs(goalCap - capacitance);
	//Test different functions Remember that linear fitness may not best fit needs
	fitness = 1.0 / difference;
	return fitness;
}

float calculateCapacitance(chromosome network){
	float capacitance;
	// Decode genes into network and calculate value
	return capacitance;
}

void mate( Organism * parent1, Organism * parent2, Organism * child1, Organism * child2 ){
	//child1->genes = ;
	//child2->genes = ;
	return;
}

void printChromosome( Organism individual){
	return;
}

int main(){
	int i,j;
	float rouletteWheelLength;
	//http://www.dummies.com/programming/c/how-to-generate-random-numbers-in-c-programming/
	srand((unsigned)time(NULL));
	bestSolution.fitness = 0;
	//Generate Populations
	for(i = 0; i < NUM_ORGANISMS; ++i){
		//Randomly generate genes
		parents[i].genes = generateChromosome();
	}
	//Run Generations
	for (i = 0; i < NUM_GENS; ++i) {
		rouletteWheelLength = 0;
		//Each Organism has a start and end on roulette wheel
		for(j = 0; j < NUM_ORGANISMS; ++j){
			//Determine Fitness and start and end
			parents[j].value = calculateCapacitance(parents[j].genes);
			parents[j].fitness = fitnessFunction(parents[j].value);
			if(parents[j].fitness > bestSolution.fitness){
				bestSolution = parents[j];
			}
			parents[j].lowerBound = rouletteWheelLength;
			parents[j].upperBound = rouletteWheelLength + parents[j].fitness;
			rouletteWheelLength = parents[j].upperBound;
		}
		//Throw the dart to make the new population
		float dartThrow;
		Organism * parent1;
		Organism * parent2;
		// Make each of the children, two at a time
		for(j = 0; j < NUM_ORGANISMS; j += 2){
			//Get parent 1
			//Generate random number between 0 and roulette wheel length
			dartThrow = fmod(rand(), rouletteWheelLength);
			//Find where the dart lands
			for (int k = 0; k < NUM_ORGANISMS; ++k) {
				if(dartThrow >= parents[k].lowerBound && dartThrow < parents[k].upperBound){
					parent1 = &parents[k];
					break;
				}
			}
			//Get parent 2, allow for same parent twice
			//Generate random number between 0 and roulette wheel length
			dartThrow = fmod(rand(), rouletteWheelLength);
			//Find where the dart lands
			for (int k = 0; k < NUM_ORGANISMS; ++k) {
				if(dartThrow >= parents[k].lowerBound && dartThrow < parents[k].upperBound){
					parent2 = &parents[k];
					break;
				}
			}
			mate(parent1, parent2, &children[j], &children[j+1]);
		}
		//Make children next generation's parents
		//Unoptimized version
		for(j = 0; j < NUM_ORGANISMS; j++){
			parents[j].genes = children[j].genes;
		}
	}
	//Print out solution
	printChromosome(bestSolution);



}