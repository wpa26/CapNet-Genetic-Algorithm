#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

int numGens = 10;
int numOrganisms = 100;
float probCrossover = 0.25;
float probPoint = 0.01;
int numCaps = 5;
float caps[] = { 100, 150, 220, 330, 470, 680, 1000 }; //caps in picofarads
float goalCap = 425; //Goal capacitance in picofards



typedef struct Organisms {
	chromosome genes;
	float value; //capacitance in picofarads
	float fitness; //difference from goal
	float lowerBound;
	float upperBound;
} Organism;


Organism parents[numOrganisms];
Organism children[numOrganisms];
Organism bestSolution;

chromosome generateChromosome(){
	chromosome genes;

	return genes;
}

float fitnessFunction(float capacitance){
	float fitness;
	float difference = abs(goalCap - capacitance);
	//Write this. Remeber that linear fitness may not best fit needs
	return fitness;
}

float calculateCapacitance(chromosome network){
	float capacitance;
	// Decode genes into network and calculate value
	return capacitance;
}

void mate( Organism * parent1, Organism * parent2, Organism * child1, Organism * child2 ){
	child1->genes = ;
	child2->genes = ;
	return;
}

main(){
	int i,j;
	float rouletteWheelLength;
	//http://www.dummies.com/programming/c/how-to-generate-random-numbers-in-c-programming/
	srand((unsigned)time(NULL));
	bestSolution.fitness = 0;
	//Generate Populations
	for(i = 0; i < numOrganisms; ++i){
		//Randomly generate genes
		parents[i].genes = generateChromosome();
	}
	//Run Generations
	for (i = 0; i < numGens; ++i) {
		rouletteWheelLength = 0;
		//Each Organism has a start and end on roulette wheel
		for(j = 0; j < numOrganisms; ++j){
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
		for(j = 0; j < numOrganisms; j += 2){
			//Generate random number between 0 and roulette wheel length
			dartThrow = fmod(rand(), rouletteWheelLength);
			//Find where the dart lands
			for (int k = 0; k < count; ++k) {
				if(dartThrow >= parents[k].lowerBound && dartThrow < parents[k].upperBound){
					parent1 = &parents[k];
					break;
				}
			}
			//Generate random number between 0 and roulette wheel length
			dartThrow = fmod(rand(), rouletteWheelLength);
			//Find where the dart lands
			for (int k = 0; k < count; ++k) {
				if(dartThrow >= parents[k].lowerBound && dartThrow < parents[k].upperBound){
					parent2 = &parents[k];
					break;
				}
			}
			mate(parent1, parent2, &child[j], &child[j+1]);
		}
		//Make children next generation's parents
		//Unoptimized version
		for(j = 0; j < numOrganisms; j++){
			parents[j].genes = child[j].genes;
		}
	}



}