#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <float.h>
#include "tinyexpr.h"

// Parameters
#define NUM_GENS 100
#define NUM_ORGANISMS 1000

// Internal Variables and macros
#define CHROM_MASK 0xFFFFFFF
#define GENE_LENGTH 28
#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))


// Compile-time Parameters
const float probCrossover = 0.0075; // probility of any crossover is 25%
const float probPoint = 0.01; //Probability of point mutation
const int numCaps = 5; //maximum number of caps
const float caps[] = { 100, 150, 220, 330, 470, 680, 1000 }; //availible caps in picofarads
const float goalCap = 578; //Goal capacitance in picofards

// Internal Varibales
const int capIndex[] = {3,10,16,21,25}; //
const int stringIndex[] = {3, 11, 19, 27, 35};
const int opIndex[] = {6, 13, 19, 24};
const int opStringIndex[] = {7, 16, 25, 34};
char expressionString[100];

// Chromosome encoding
// (((C1 op (((C2) op ((C3)) op (C4))) op C5)))
// Only open parenthesis is actually encoded, matching parenthesis implied
typedef int chromosome; //chromosome takes up 28 bits with 5 caps

typedef struct {
	chromosome genes; //Encoded network
	float value; //capacitance in picofarads
	float fitness; //calculated fitness (higher is more fit)
	float lowerBound; //beginning of place on roulette wheel
	float upperBound; //end of place on roulete wheel
} Organism;


Organism parents[NUM_ORGANISMS];
Organism children[NUM_ORGANISMS];
Organism bestGlobalSolution;
Organism bestGenerationalSolution;

//Randomly generates a chromosome to encode a network
chromosome generateChromosome(){
	chromosome genes;
	genes = random() & CHROM_MASK;
	return genes;
}

//Returns fitness of a given organism, based on capacitance
//Note, this could be improved by taking into account number of
//caps, as the fewer the caps the better
float fitnessFunction(float capacitance){
	float fitness;
	float difference = fabs(goalCap - capacitance);
	if(difference == 0){
		printf("We found a perfect network");
		return 0;
	}
	else{
		fitness = 1.0 / difference;
	}
	return fitness;
}

// Called to pass the expression string to the interpreter to calculate
// capacitance.
float calculateCapacitance(char * expressionString){
	float capacitance;
	// Decode genes into network and calculate value
	capacitance = te_interp(expressionString,0);
	return capacitance;
}

// Takes two parents and creates two offspring, with crossover and point mutation
void mate( Organism * parent1, Organism * parent2, Organism * child1, Organism * child2 ){
	int i;
	child1->genes = parent1->genes; //just copy and then do work later
	child2->genes = parent2->genes;
	float dieRoll;
	chromosome flipper = 1;
	chromosome mask = 1;
	chromosome temp1;
	chromosome temp2;
	//Go through each bit
	for(i = 0; i < GENE_LENGTH; ++i){
		// First point mutation for both organisms
		dieRoll = (float)rand()/(float)(RAND_MAX);
		if( dieRoll <= probPoint ){
			child1->genes ^= flipper; //flip point
		}
		dieRoll = (float)rand()/(float)(RAND_MAX);
		if( dieRoll <= probPoint){
			child2->genes ^= flipper;
		}
		// Next crossover
		dieRoll = (float)rand()/(float)(RAND_MAX);
		if( dieRoll <= probCrossover){
			temp1 = child1->genes & mask;
			temp2 = child2->genes & mask;
			child1->genes += temp2;
			child2->genes += temp1;
		}
		//Update point mutation and crossover operands
		flipper = flipper << 1;
		mask = (mask << 1) + 1;
	}
	return;
}

//Creates expression string from genes.
//Kind of a bad name, more like sprintfchromosome
void printChromosome( Organism individual ){
	char temp[5];
	int length = 0;
	int longest_cap = 4;
	float cap_val;
	chromosome gene = individual.genes;
	chromosome mask = 7;
	chromosome index;
	int i,j;
	for(i = 0; i < 42;++i){
		expressionString[i] = ' ';
	}
	expressionString[42] = '\0';
	if(CHECK_BIT(gene,0)){
		expressionString[0] = '(';
		expressionString[33] = ')';
	}
	if(CHECK_BIT(gene,1)){
		expressionString[1] = '(';
		expressionString[24] = ')';
	}
	if(CHECK_BIT(gene,2)){
		expressionString[2] = '(';
		expressionString[15] = ')';
	}
	if(CHECK_BIT(gene,7)){
		expressionString[8] = '(';
		expressionString[41] = ')';
	}
	if(CHECK_BIT(gene,8)){
		expressionString[9] = '(';
		expressionString[32] = ')';
	}
	if(CHECK_BIT(gene,9)){
		expressionString[10] = '(';
		expressionString[23] = ')';
	}
	if(CHECK_BIT(gene,14)){
		expressionString[17] = '(';
		expressionString[40] = ')';
	}
	if(CHECK_BIT(gene,15)){
		expressionString[18] = '(';
		expressionString[31] = ')';
	}
	if(CHECK_BIT(gene,20)){
		expressionString[26] = '(';
		expressionString[39] = ')';
	}
	for(j = 0; j < 5; ++j){
		index = (gene & (mask << capIndex[j]))>>capIndex[j];
		if(index){
			cap_val = caps[index - 1];
			sprintf(temp,"%04.f",cap_val);
			for(i = 0;i<4;++i){
				expressionString[stringIndex[j]+i] = temp[i];
			}
		}
		else{
			for(i = 0;i<4;++i){
				expressionString[stringIndex[j]+i] = '0';
			}
		}
	}
	for(j = 0; j < 4; ++j){
		if(CHECK_BIT(gene, opIndex[j])){
			expressionString[opStringIndex[j]] = '+'; //1 is series
		}
		else{
			expressionString[opStringIndex[j]] = '|'; //0 is parallel
		}
	}
	return;
}

int main(){
	int i,j;
	float rouletteWheelLength;
	//http://www.dummies.com/programming/c/how-to-generate-random-numbers-in-c-programming/
	srand((unsigned)time(NULL));
	bestGlobalSolution.fitness = 0;
	char expressionString[100];
	
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
			printChromosome(parents[j], expressionString);
			parents[j].value = calculateCapacitance(expressionString);
			parents[j].fitness = fitnessFunction(parents[j].value);

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
		for(j = 0; j < NUM_ORGANISMS; j++){
			parents[j].genes = children[j].genes;
		}
		printChromosome(bestGenerationalSolution,expressionString);
		printf("The best chromosome this gen (%d) is %s,with capacitance %f\n",i,expressionString,calculateCapacitance(expressionString));
	
	}
	//Print out solution
	printChromosome(bestGlobalSolution,expressionString);
	printf("The best chromosome is %s,with capacitance %f\n",expressionString,calculateCapacitance(expressionString));
}