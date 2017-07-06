#include "N_queen_GA.h"
#include <iostream>
#include <algorithm>
#include <chrono>
#include <vector>
#include <cstdlib>
#include <random>
#include <set>

using namespace std;

bool mysort(array<unsigned char, BOARD_SIZE + 1> item1, array< unsigned char, BOARD_SIZE + 1> item2) {
	return item1[BOARD_SIZE] > item2[BOARD_SIZE];
}



void N_queen_GA::Display_Solution() {
	array<unsigned char, BOARD_SIZE+1> solution = initialPopulation[0];
	cout << endl;
	for (int i = 0; i < BOARD_SIZE; i++) {
		cout << ((i < 9) ? " ": "") << i + 1;
		for (int j = 0; j < BOARD_SIZE; j++) {
			cout << ( (solution[j] == i) ? " Q" : " -" );
		}
		cout << endl;
	}
	for (int i = 0; i < BOARD_SIZE; i++) {
		cout << " "<< int(solution[i]);
	}
	cout << endl;
	cout << " fitness: " << int(solution[BOARD_SIZE]) << endl;
	cout << "average fitness: " << (double)populationFitness/initialPopulation.size() << endl;
	
}

int N_queen_GA::Run() {
	int counter = 0;
	const int mutationRate = (int) ( POPULATION_SIZE * MUTATION_RATE );
	int mutationCounter;
	Generate_Population();
	sort(initialPopulation.begin(), initialPopulation.end(), mysort);
	Display_Solution();
	int convergence = populationFitness;
	while (counter < GEN_NUMBER) {
		mutationCounter = 0;
		//if (counter % 100 == 0) {
		//	if (convergence < populationFitness) 
		//}
		sort(initialPopulation.begin(), initialPopulation.end(), mysort);
		if (initialPopulation[0][BOARD_SIZE] == BOARD_SIZE) break; // we found solution!!!
		Choose_Parents();
		int index = 0;
		while ( index + 1 < newPopulation.size() ) {
			CrossOver(&(newPopulation[index]), &(newPopulation[index + 1]));
			index += 2;
		}
		int i = 0;
		while (newPopulation.size() < POPULATION_SIZE) {
			newPopulation.push_back(CrossOver( &(newPopulation[i]), &(initialPopulation[i + 2]) ) );
			i++;
		}
		while (mutationCounter < mutationRate) {
			int step = (int) POPULATION_SIZE / mutationRate;
			for (int i = 0; i < newPopulation.size(); i = i + step) {
				Mutation(&newPopulation[i]);
				mutationCounter++;
			}
		}
		populationFitness = 0;
		for (int i = 0; i < newPopulation.size(); i++) {
			populationFitness += Fitness_Function(&(newPopulation[i]));
		}
		initialPopulation = newPopulation;
		cout << "\rgeneration number: " << counter;
		counter++;
	}
	sort(initialPopulation.begin(), initialPopulation.end(), mysort);
	Display_Solution();
	return 0;
}


void N_queen_GA::Choose_Parents(){
	
	newPopulation.clear();
	unsigned seed = chrono::system_clock::now().time_since_epoch().count();
	default_random_engine e(seed);
	uniform_real_distribution<> u(0, 1);
	double randomFitness;
	double relativeFitness;
	int maxFitness = initialPopulation.front()[BOARD_SIZE];
	for (vector_iter it = initialPopulation.begin(); it != initialPopulation.end(); ++it) {
		relativeFitness = (double) (*it)[BOARD_SIZE] / maxFitness; 
		randomFitness = u(e);
		if (relativeFitness > randomFitness) { //probability of choosing the individual depends on its relative fitness
			newPopulation.push_back(*it);
		}
	}
}

array<unsigned char, BOARD_SIZE + 1>  N_queen_GA::CrossOver(array<unsigned char, BOARD_SIZE + 1> *parent1, array<unsigned char, BOARD_SIZE + 1> *parent2) {
	array<unsigned char, BOARD_SIZE + 1> child;
	unsigned seed = chrono::system_clock::now().time_since_epoch().count();
	default_random_engine e(seed);
	uniform_int_distribution<unsigned> u(0, BOARD_SIZE - 1);
	int start = u(e); //starting index of the crossover
	int end = u(e);
	while (end < start) { //making sure that the ending index of the crossover is bigger than start
		end = u(e);
	}
	unsigned char temp;
	set<unsigned char> replacement1, replacement2;
	for (int i = start; i <= end; i++) { //initial exchange
		temp = (*parent1)[i];
		(*parent1)[i] = (*parent2)[i];
		(*parent2)[i] = temp;
		if (replacement2.find(temp) == replacement2.end()) { 
			replacement1.insert(temp); 
		}
		else {
			replacement2.erase(replacement2.find(temp));
		}
		if (replacement1.find((*parent1)[i]) == replacement1.end()) {
			replacement2.insert((*parent1)[i]);
		}
		else {
			replacement1.erase(replacement1.find((*parent1)[i]));
		}
	}
	for (int j = 0; j < start; j++) { 
		for (int k = start; k <= end; k++) {
			if ((*parent1)[j] == (*parent1)[k]) {
				(*parent1)[j] = *replacement1.begin();
				replacement1.erase(*replacement1.begin());
			}
			if ((*parent2)[j] == (*parent2)[k]) {
				(*parent2)[j] = *replacement2.begin();
				replacement2.erase(*replacement2.begin());
			}
		}
	}
	for (int j = end + 1; j < BOARD_SIZE; j++) {
		for (int k = start; k <= end; k++) {
			if ((*parent1)[j] == (*parent1)[k]) {
				(*parent1)[j] = *replacement1.begin();
				replacement1.erase(*replacement1.begin());
			}
			if ((*parent2)[j] == (*parent2)[k]) {
				(*parent2)[j] = *replacement2.begin();
				replacement2.erase(*replacement2.begin());
			}
		}
	}
	child = *parent1;
	return child;
}

int N_queen_GA::Generate_Population() {
	populationFitness = 0;
	unsigned seed = chrono::system_clock::now().time_since_epoch().count();
	vector<unsigned char> vtemp;
	array<unsigned char, BOARD_SIZE+1> temp;
	for (char c = 0; c < BOARD_SIZE; c++) {
		vtemp.push_back(c);
	}
	for (int i = 0; i < POPULATION_SIZE; i++) {
		shuffle(vtemp.begin(), vtemp.end(), default_random_engine(seed) );
		for (char j = 0; j < BOARD_SIZE; j++) {
			temp[j] = vtemp[j];
		}
		initialPopulation.push_back(temp);
	}
	for (int i = 0; i < initialPopulation.size(); i++) {
		populationFitness += Fitness_Function(&(initialPopulation[i]));
	}
	return populationFitness;
}

int N_queen_GA::Fitness_Function(array<unsigned char, BOARD_SIZE+1>* ind) {
	int counter;
	int sum = 0;
	int lDiagonals[2 * BOARD_SIZE - 1];
	int rDiagonals[2 * BOARD_SIZE - 1];
	for (int i = 0; i < 2 * BOARD_SIZE - 1; i++) {
		lDiagonals[i] = 0;
		rDiagonals[i] = 0;
	}
	for (int i = 0; i < BOARD_SIZE; i++) {
		char q = (*ind)[i];
		lDiagonals[i + q]++;
		rDiagonals[BOARD_SIZE - i + q - 1]++;
	}
	for (int k = 0; k < 2 * BOARD_SIZE - 1; k++) {
		counter = 0;
		if (lDiagonals[k] > 1) {
			counter += lDiagonals[k] - 1;
		}
		if (rDiagonals[k] > 1) {
			counter += rDiagonals[k] - 1;
		}
		sum += counter;
	}
	(*ind)[BOARD_SIZE] = BOARD_SIZE - sum;
	return BOARD_SIZE - sum;
}

void N_queen_GA::Mutation(array<unsigned char, BOARD_SIZE+1>* ind) {//swaps two random values
	unsigned seed = chrono::system_clock::now().time_since_epoch().count();
	unsigned char temp;
	default_random_engine e(seed);
	uniform_int_distribution<unsigned> u(0, BOARD_SIZE-1);
	int  index1 = u(e);
	int  index2 = u(e);
	temp = (*ind)[index1];
	(*ind)[index1] = (*ind)[index2];
	(*ind)[index2] = temp;
}

N_queen_GA::N_queen_GA()
{
	populationFitness = 0;
}


N_queen_GA::~N_queen_GA()
{
}

int main(int argc, char* argv[]) {
	N_queen_GA evolution;
	return(evolution.Run());
}