#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <float.h>

#define NUM_GENS 10
#define NUM_ORGANISMS 100
#define CHROM_MASK 0xFFFFFFF
#define GENE_LENGTH 28
#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))

float probCrossover = 0.0075; // 25% of any crossover
float probPoint = 0.01;
int numCaps = 5; //maximum number of caps
float caps[] = { 100, 150, 220, 330, 470, 680, 1000 }; //caps in picofarads
int capIndex[] = {3,10,16,21,25};
int stringIndex[] = {3, 11, 19, 27, 35};
int opIndex[] = {6, 13, 19, 24};
int opStringIndex[] = {7, 16, 25, 34};
float goalCap = 425; //Goal capacitance in picofards


// (((C1 op (((C2) op ((C3)) op (C4))) op C5)))
// Ok never mind we only need to use half the parenthesis
// So only open parenthesis are encoded
typedef int chromosome; //chromosome takes up 37 bits in 5

typedef struct Organisms {
	chromosome genes;
	float value; //capacitance in picofarads
	float fitness; //difference from goal
	float lowerBound; 
	float upperBound;
} Organism;


Organism parents[NUM_ORGANISMS];
Organism children[NUM_ORGANISMS];
Organism bestGlobalSolution;
Organism bestGenerationalSolution;

chromosome generateChromosome(){
	chromosome genes;
	genes = random() & CHROM_MASK;
	return genes;
}

float fitnessFunction(float capacitance){
	float fitness;
	float difference = fabs(goalCap - capacitance);
	//Test different functions Remember that linear fitness may not best fit needs
	if(difference == 0){
		printf("Hey we found our guy!");
		return 0;
	}
	else{
		fitness = 1.0 / difference;
	}
	return fitness;
}

float calculateCapacitance(chromosome network){
	float capacitance;
	// Decode genes into network and calculate value
	// Choose unmatched parenthesis based on hamming code
	return capacitance;
}

void mate( Organism * parent1, Organism * parent2, Organism * child1, Organism * child2 ){
	int i;
	child1->genes = parent1->genes;
	child2->genes = parent2->genes;
	float dieRoll;
	chromosome flipper = 1;
	chromosome mask = 1;
	chromosome temp1;
	chromosome temp2;
	for(i = 0; i < GENE_LENGTH; ++i){
		dieRoll = (float)rand()/(float)(RAND_MAX);
		if( dieRoll <= probPoint ){
			child1->genes ^= flipper;
		}
		dieRoll = (float)rand()/(float)(RAND_MAX);
		if( dieRoll <= probPoint){
			child2->genes ^= flipper;
		}
		dieRoll = (float)rand()/(float)(RAND_MAX);
		if( dieRoll <= probCrossover){
			temp1 = child1->genes & mask;
			temp2 = child2->genes & mask;
			child1->genes += temp2;
			child2->genes += temp1;
		}

		flipper = flipper << 1;
		mask = (mask << 1) + 1;
	}
	return;
}

void printChromosome( Organism individual ){
	char string[100];
	char temp[5];
	int length = 0;
	int longest_cap = 4;
	float cap_val;
	chromosome gene = individual.genes;
	chromosome mask = 7;
	chromosome index;
	int i,j;
	for(i = 0; i < 42;++i){
		string[i] = ' ';
	}
	string[42] = '\0';
	if(CHECK_BIT(gene,0)){
		string[0] = '(';
		string[33] = ')';
	}
	if(CHECK_BIT(gene,1)){
		string[1] = '(';
		string[24] = ')';
	}
	if(CHECK_BIT(gene,2)){
		string[2] = '(';
		string[15] = ')';
	}
	if(CHECK_BIT(gene,7)){
		string[8] = '(';
		string[41] = ')';
	}
	if(CHECK_BIT(gene,8)){
		string[9] = '(';
		string[32] = ')';
	}
	if(CHECK_BIT(gene,9)){
		string[10] = '(';
		string[23] = ')';
	}
	if(CHECK_BIT(gene,14)){
		string[17] = '(';
		string[40] = ')';
	}
	if(CHECK_BIT(gene,15)){
		string[18] = '(';
		string[31] = ')';
	}
	if(CHECK_BIT(gene,20)){
		string[26] = '(';
		string[39] = ')';
	}
	for(j = 0; j < 5; ++j){
		index = (gene & (mask << capIndex[j]))>>capIndex[j];
		if(index){
			cap_val = caps[index - 1];
			sprintf(temp,"%04.f",cap_val);
			for(i = 0;i<4;++i){
				string[stringIndex[j]+i] = temp[i];
			}
		}
		else{
			for(i = 0;i<4;++i){
				string[stringIndex[j]+i] = '0';
			}
		}
	}
	for(j = 0; j < 4; ++j){
		if(CHECK_BIT(gene, opIndex[j])){
			string[opStringIndex[j]] = '+'; //1 is series
		}
		else{
			string[opStringIndex[j]] = '|'; //0 is parallel
		}
	}


	printf("%s\n",string);
	return;
}

int main(){
	Organism example;
	example.genes = 0b0101010101000111001101001110;
	printChromosome(example);
	example.genes = 0b1111111111111111111111111111;
	printChromosome(example);

	/*int i,j;
	float rouletteWheelLength;
	//http://www.dummies.com/programming/c/how-to-generate-random-numbers-in-c-programming/
	srand((unsigned)time(NULL));
	bestGlobalSolution.fitness = 0;
	//Generate Populations
	for(i = 0; i < NUM_ORGANISMS; ++i){
		//Randomly generate genes
		parents[i].genes = generateChromosome();
	}
	//Run Generations
	for (i = 0; i < NUM_GENS; ++i) {
		rouletteWheelLength = 0;
		bestGenerationalSolution.fitness = 0;
		//Each Organism has a start and end on roulette wheel
		for(j = 0; j < NUM_ORGANISMS; ++j){
			//Determine Fitness and start and end
			parents[j].value = calculateCapacitance(parents[j].genes);
			parents[j].fitness = fitnessFunction(parents[j].value);
			if(parents[j].fitness == 0){
				printChromosome(parents[j]);
			}
			if(parents[j].fitness > bestGlobalSolution.fitness){
				bestGlobalSolution = parents[j];
			}
			if(parents[j].fitness > bestGenerationalSolution.fitness){
				bestGenerationalSolution = parents[j];
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
			//https://stackoverflow.com/a/13409133
			//Possibly make this better
			dartThrow = (float)rand()/(float)(RAND_MAX/rouletteWheelLength);
			//Find where the dart lands
			for (int k = 0; k < NUM_ORGANISMS; ++k) {
				if(dartThrow >= parents[k].lowerBound && dartThrow < parents[k].upperBound){
					parent1 = &parents[k];
					break;
				}
			}
			//Get parent 2, allow for same parent twice
			//Generate random number between 0 and roulette wheel length
			dartThrow = (float)rand()/(float)(RAND_MAX/rouletteWheelLength);
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
	printChromosome(bestGlobalSolution);
	*/


}