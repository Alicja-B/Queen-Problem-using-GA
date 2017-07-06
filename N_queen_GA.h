#pragma once

#include <string>
#include <list>
#include <array>
#include <vector>

const double MUTATION_RATE = 0.05;
const int POPULATION_SIZE = 100;
const int BOARD_SIZE = 100;
const int GEN_NUMBER = 1000;
typedef std::vector< std::array<unsigned char, BOARD_SIZE + 1> >::iterator vector_iter;

class N_queen_GA
{
private:
	std::vector< std::array<unsigned char, BOARD_SIZE + 1> > initialPopulation;
	std::vector< std::array<unsigned char, BOARD_SIZE + 1> > newPopulation;
	int populationFitness;
	

public:
	int Fitness_Function(std::array<unsigned char, BOARD_SIZE + 1> *);
	int Generate_Population();
	void Choose_Parents();
	void Display_Solution();
	void N_queen_GA::Mutation(std::array<unsigned char, BOARD_SIZE + 1>* );
	std::array<unsigned char, BOARD_SIZE + 1> CrossOver(std::array<unsigned char, BOARD_SIZE + 1>*, std::array<unsigned char, BOARD_SIZE + 1>*);
	int Run();
	N_queen_GA();
	~N_queen_GA();
};

