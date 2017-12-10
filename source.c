#include <stdio.h>



int numGens = 10;
int numOrganisms = 100;
float probCrossover = 0.25;
float probPoint = 0.01;
int numCaps;
float caps[] = { 100, 150, 220, 330, 470, 680, 1000 }; //caps in picofarads
float goalCap = 425; //Goal capacitance in picofards


typedef struct Organisms {
	chromosome genes;
	float Fitness; //difference from goal
	float lowerBound;
	float upperBound;
} Organism;


Organism orgs[numOrganisms];

main(){
	int i;
	//Generate Populations
	float rouletteWheelLength;
	//Run Generations
	for (i = 0; i < numGens; ++i) {
		rouletteWheelLength = 0;
		for(j = 0; j < numOrganisms; ++j){
			//Determine Fitness and start and end
			orgs[]

		}
		//Each Organism has a start and end on roulette wheel
	}



}