/********************************************************
Name		: Jennings Fairchild
		    : Tyler Windham
			: Austin Hanus
			: Dey'Ana Wilson
Subject		: CSCE 315-504
Assignment	: Project 3: AI
********************************************************/
#include <string>
#include <vector>
#include <bitset>
#include <iostream>
#include <fstream>
#include <time.h>
#include <stdlib.h>
#include <algorithm>
#include <sys/time.h>
#include <cmath> 
#include <limits.h>

using namespace std;


enum bitOp {AND, OR, NOT, NONE};

struct Gate
{
	int gateNum, operId1, operId2;
	bitset<8> value;
	bitset<8> operVal1;
	bitset<8> operVal2;
	bitOp operation;
	string gateLine;
};

struct Circuit
{
	int id;
	int notCount = 0;
	int andCount=0;
	int orCount = 0; 
	bitset<8> output;
	int fitnessScore =0; 
	vector<Gate> gateList;
	string circuitry;
	Circuit* parent;
	Gate solution1;
	Gate solution2;
	Gate solution3;
	bool correctFlag1 = false;
	bool correctFlag2 = false;
	bool correctFlag3 = false;
};

struct compareCircuits {
	
	inline bool operator() (const Circuit& c1, const Circuit& c2){
		return (c1.fitnessScore < c2.fitnessScore); 
	}
}; 

Circuit parentCircuit();
Gate initGate(int id, bitOp op, bitset<8> val, Gate& oper1, Gate& oper2);
string gateToString(Gate convert);
void checkCorrectOutput(Circuit& currentCirc, Gate toCheck);
void checkCircuit(Circuit &currentCirc);
bitset<8> bitOperation(int operation, bitset<8> operand1, bitset<8> operand2);
int circuitCount = 0;
int randomCircuitSize(){
	int randomInt;
	randomInt = rand()%20 + 5; 
	return randomInt; 
}

int randomGeneticOperation(){
	int randomInt; 
	randomInt = rand()%2 + 1; 
	return randomInt; 
	

}

bitOp getGateType(int gate){
	switch(gate){
		case 1: 
			return AND; 
			break; 
		case 2: 
			return OR; 
			break; 
		case 3: 
			return NOT; 
			break; 
		default: 
			cout << "Invalid input" << endl; 
			break; 
		
	}
	
}

int randomGate(){
	int random; 
	random = rand()%3 + 1; 
	return random;
}


void clearFlags(Circuit &c1) {
	c1.correctFlag1 = false;
	c1.correctFlag2 = false;
	c1.correctFlag3 = false;
}

Circuit makeRandomCircuit(int circuitSize, int gateID){
	
	int rGateType; //Random gate type 1: AND 2: OR 3: NOT
	bitOp gateToAdd; //Type of gate to add
	int rVal1; 
	int rVal2; 
	int notCount = 0; 
	Gate g1; 
	Gate g2; 
	Gate rGate; 
	Circuit randomCircuit = parentCircuit(); //Make sure each random has the 
	randomCircuit.id = gateID;
	for(int i=0; i < circuitSize; ++i){
		if (notCount < 2) {
			rGateType = randomGate();
			if (rGateType == 3){ 
				++notCount;
				randomCircuit.notCount++;
			}
		}
		else {
			//If we get a NOT gate when already at 2, find new rand num
			while ((rGateType=randomGate()) == 3) {
			}
		}
		
		if(rGateType == 1){
			randomCircuit.andCount++; 
		}else if (rGateType == 2){
			randomCircuit.orCount++; 
		}
		gateToAdd = getGateType(rGateType); 
		rVal1 = rand()%(i+3); 
		rVal2 = rand()%(i+3);
		g1 = randomCircuit.gateList[rVal1]; 
		g2 = randomCircuit.gateList[rVal2];
		bitset<8> val = bitOperation(gateToAdd, randomCircuit.gateList[i].value, randomCircuit.gateList[i].value);
		rGate = initGate(randomCircuit.gateList.size()+1,gateToAdd, val, g1, g2);
		randomCircuit.output = val;
		checkCorrectOutput(randomCircuit, rGate);
		randomCircuit.gateList.push_back(rGate); 
		string newCircString = randomCircuit.circuitry + gateToString(rGate);
		randomCircuit.circuitry = newCircString; 
	}
	
	return randomCircuit;
}


vector<Circuit> createRandomPopulation(int populationSize) {
	vector<Circuit> population;
	int circuitSize = 0;
	for (int i = 0; i < populationSize; ++i) {
		circuitSize = rand()%40+20; //random number between 15 and 20
		Circuit cir = makeRandomCircuit(circuitSize,circuitCount);
		++circuitCount;
		population.push_back(cir);
	}
	return population;
}


void getFitnessScore(Circuit &c1){
	 
	 c1.fitnessScore = 0;
	 int differenceCount =0; 
	 bitset<8> OUTPUT1 = 240;
	 bitset<8> OUTPUT2 = 204;
	 bitset<8> OUTPUT3 = 170; 
	for(int i=0; i < 8; ++i){
		if(c1.output[i] != OUTPUT3[i]){
			differenceCount++; 
		}
	}	 
	 c1.fitnessScore += (1000000 * differenceCount); 
	c1.fitnessScore+= (c1.andCount * 10); 
	c1.fitnessScore += (c1.orCount * 10);
	c1.fitnessScore+= (c1.notCount * 100000); 
	
	// if(c1.correctFlag1){
	// 	c1.fitnessScore -=1000; 
	// }
	// if(c1.correctFlag2){
	// 	c1.fitnessScore -=1000;
	// }  
	// if(c1.correctFlag3){
	// 	c1.fitnessScore -=1000;
	// }
}

vector<Circuit> crossOver(Circuit c1, Circuit c2){
	
	int minSize=0; 
	vector<Circuit> children; 
	if(c1.gateList.size() > c2.gateList.size()){
		minSize = c2.gateList.size(); 
	}else{
		minSize = c1.gateList.size(); 
	}
	int randomPosition = rand()% (minSize - 3) + 3; 
	//randomPosition--;
	int c1Solution1Position; 
	int c1Solution2Position; 
	int c2Solution1Position;
	int c2Solution2Position; 
	int c1DistanceToEnd= 0;
	int c2DistanceToEnd = 0; 
	int minDistance; 
	
	vector<Gate> tempGateList; 
	vector<Gate> tempGateList2; 
	
	for(int i=randomPosition; i < c1.gateList.size(); ++i){
		tempGateList.push_back(c1.gateList[i]); 
	}
	
	for(int i=randomPosition; i < c2.gateList.size(); ++i){
		tempGateList2.push_back(c2.gateList[i]); 
	}
	
	c1.gateList.erase(c1.gateList.begin()+randomPosition, c1.gateList.end());
	c1.gateList.insert(c1.gateList.end(), tempGateList2.begin(), tempGateList2.end()); 
	
	c2.gateList.erase(c2.gateList.begin()+randomPosition, c2.gateList.end()); 
	c2.gateList.insert(c2.gateList.end(), tempGateList.begin(), tempGateList.end()); 

	
	for (int i = randomPosition+1; i < c1.gateList.size(); ++i) {
		bitset<8> refreshedVal = bitOperation(c1.gateList[i].operation, c1.gateList[c1.gateList[i].operId1-1].value, 
			c1.gateList[c1.gateList[i].operId2-1].value);
		Gate refreshedGate = initGate(c1.gateList[i].gateNum, c1.gateList[i].operation, refreshedVal, c1.gateList[c1.gateList[i].operId1-1], c1.gateList[c1.gateList[i].operId2-1]);
		c1.gateList[i] = refreshedGate;
		c1.output = refreshedVal; 
	}
	
	for (int i = randomPosition+1; i < c2.gateList.size(); ++i) {
		bitset<8> refreshedVal = bitOperation(c2.gateList[i].operation, c2.gateList[c2.gateList[i].operId1-1].value, 
			c2.gateList[c2.gateList[i].operId2-1].value);
		Gate refreshedGate = initGate(c2.gateList[i].gateNum, c2.gateList[i].operation, refreshedVal, c2.gateList[c2.gateList[i].operId1-1], c1.gateList[c1.gateList[i].operId2-1]);
		c2.gateList[i] = refreshedGate;
		c2.output = refreshedVal; 
	}
	
	c1.circuitry = ""; 
	c2.circuitry = ""; 
	c1.andCount =0; 
	c1.notCount =0; 
	c1.orCount = 0;
	c2.andCount = 0; 
	c2.notCount = 0;
	c2.orCount = 0;
	for(int i=0; i < c1.gateList.size(); ++i){
		
		if(c1.gateList[i].operation == AND){
			c1.andCount++;
		}else if(c1.gateList[i].operation == OR){
			c1.orCount++; 
		}else if (c1.gateList[i].operation == NOT){
			c1.notCount++; 
		}
		c1.circuitry += gateToString(c1.gateList[i]); 
	}
	
	for(int i=0; i < c2.gateList.size(); ++i){
		
		if(c2.gateList[i].operation == AND){
			c2.andCount++;
		}else if(c2.gateList[i].operation == OR){
			c2.orCount++; 
		}else if (c2.gateList[i].operation == NOT){
			c2.notCount++; 
		}
		
		c2.circuitry += gateToString(c2.gateList[i]); 
	}
	
	clearFlags(c1);
	clearFlags(c2);
	children.push_back(c1);
	children.push_back(c2);
	return children; 
	//Check if updated info is solution
	
	
	
}

Circuit mutateCircuit(Circuit c) {
	Circuit c1 = c;
	int randomGateNum = rand()%(c1.gateList.size()-3)+3; //After initial gates
	int randomInput1 = rand()%(randomGateNum);
	int randomInput2 = rand()%(randomGateNum);
	int rGateType;
	bitOp newGate;
 
	
	if (c1.gateList[randomGateNum].operation == NOT) {
		while ((rGateType = randomGate()) != 3) {
		}
		newGate = getGateType(rGateType);
		c1.notCount--;
	}
	else if (c1.gateList[randomGateNum].operation == AND) {
		newGate = getGateType(2);
	}
	else if (c1.gateList[randomGateNum].operation == OR) {
		newGate = getGateType(1);
	}
	Gate g1 = c1.gateList[randomInput1];
	Gate g2 = c1.gateList[randomInput2];
	bitset<8> val = bitOperation(newGate, c1.gateList[randomInput1].value, c1.gateList[randomInput2].value);
	Gate rGate = initGate(c1.gateList[randomGateNum].gateNum, newGate, val, g1, g2);
	c1.gateList[randomGateNum] = rGate;

	for (int i = randomGateNum+1; i < c1.gateList.size(); ++i) {
		bitset<8> refreshedVal = bitOperation(c1.gateList[i].operation, c1.gateList[c1.gateList[i].operId1-1].value, 
			c1.gateList[c1.gateList[i].operId2-1].value);
		Gate refreshedGate = initGate(c1.gateList[i].gateNum, c1.gateList[i].operation, refreshedVal, c1.gateList[c1.gateList[i].operId1-1], c1.gateList[c1.gateList[i].operId2-1]);
		c1.gateList[i] = refreshedGate;
		c1.output = refreshedVal; 
	}

	string mutatedCircuitry = "";
	c1.andCount =0; 
	c1.notCount =0; 
	c1.orCount = 0;
	for (int j = 0; j < c1.gateList.size(); ++j) {
		
		if(c1.gateList[j].operation == AND){
			c1.andCount++;
		}else if(c1.gateList[j].operation == OR){
			c1.orCount++; 
		}else if (c1.gateList[j].operation == NOT){
			c1.notCount++; 
		}
		
		mutatedCircuitry += gateToString(c1.gateList[j]);
	}
	c1.circuitry = mutatedCircuitry;
	
	
	clearFlags(c1);
	return c1;
}


void geneticOperations(vector<Circuit> &topCircuits, vector<Circuit> &offspring){
	//If there is only one circuit left to modify we need to mutate
	Circuit c1; 
	Circuit c2; 
	vector<Circuit> children; 
	
	for(int i=0; i < topCircuits.size(); ++i){
	
		int operation = randomGeneticOperation(); 
		//cout << "Top Circuit Size: " << topCircuits.size() << endl; 
	
		if(i == topCircuits.size() - 1){
			c1 = topCircuits[i];	//Can't crossover with only 1 circuit
			c2 = mutateCircuit(c1);
			//topCircuits[i] = c2; 
			offspring.push_back(c2);
			break; 
		} 
		
		if(operation == 1){
			//Mutate
			//cout << "Mutating " << endl; 
			c1 = topCircuits[i];
			c2 = mutateCircuit(c1); 
			offspring.push_back(c2);
			//topCircuits[i] = c2; 
			//checkCircuit(topCircuits[i]); 
		}else if(operation == 2){
			//Crossover  
			//cout << "Crossing over " << endl; 
			c1 = topCircuits[i]; 
			c2 = topCircuits[i+1];
			children = crossOver(c1,c2);

			for(int j=0; j < 2; ++j){
				offspring.push_back(children[j]);
				
			}

			i++; 
			if(i== topCircuits.size()-2){
				c1 = topCircuits[i];
				c2 = mutateCircuit(c1); 
				offspring.push_back(c2);
				break; //Used last two circuits in crossover
			}	
		
		}
		
		
	}

}

void assignFitnessScores(vector<Circuit> &population){
	
	for(int i=0; i < population.size(); ++i){
		getFitnessScore(population[i]); 
	}



}

vector<Circuit> getMostFit(vector<Circuit> population){
	
	int selectPop = population.size() * .5; 
	
	//cout << selectPop << endl;
	int randomCircuit = 0; 
	int randomPosition = 0; 
	vector<Circuit> topCircuits; 
	for(int i=0; i < population.size(); ++i){
		randomCircuit = rand()%10 + 1; 
		if(randomCircuit > 8){
			//smaller probablility choose less fit
			randomPosition = rand()%(selectPop-1) + selectPop; 
			topCircuits.push_back(population[randomPosition]);
		}else{
			//Push back more fit Circuit
			topCircuits.push_back(population[i]); 
		}
		
	}
	
	return topCircuits;
	
	
}

	

string opToString(int operation)
{
	switch(operation)
	{
		case 0:
			return "AND";
			break;
		case 1:
			return "OR";
			break;
		case 2:
			return "NOT";
			break;
		case 3:
			return "NONE";
			break;
		default:
			return "NULL";
			break;
	}
}

// 
bitset<8> bitOperation(int operation, bitset<8> operand1, bitset<8> operand2)
{
	bitset<8> result;
	switch(operation)
	{
		case 0:
			result = operand1 & operand2;
			return result;
			break;
		case 1:
			result = operand1 | operand2;
			return result;
			break;
		case 2:
			result = ~operand1;
			return result;
			break;
		case 3:
			result = operand1;
			return result;
			break;
		default:
			return 0;
			break;
	}
}

// Converts a gate string
string gateToString(Gate convert)
{
	string gateLine =	to_string(convert.gateNum) + " " +
						opToString(convert.operation) + " " +
						to_string(convert.operId1);
	if (convert.operId1 != convert.operId2)
	{
		gateLine += " " + to_string(convert.operId2);
	}
	gateLine += "\tValue: " + convert.value.to_string() + "\r\n";

	return gateLine;
}

// Gate initializer
Gate initGate(int id, bitOp op, bitset<8> val, Gate& oper1, Gate& oper2)
{
	Gate newGate;
	newGate.gateNum = id;
	newGate.operation = op;
	newGate.value = val;
	newGate.operId1 = oper1.gateNum;
	newGate.operId2 = oper2.gateNum;
	newGate.operVal1 = oper1.value;
	newGate.operVal2 = oper2.value;
	newGate.gateLine = gateToString(newGate);

	return newGate;
}

// Circuit initializer
Circuit initCircuit(int gateId, bitset<8> outputVal, int nots, vector<Gate> gates, string circuit, Circuit& parentCirc)
{
	Circuit newCirc;
	newCirc.id = gateId;
	newCirc.output = outputVal;
	newCirc.notCount = nots;
	newCirc.gateList = gates;
	newCirc.circuitry = circuit;
	newCirc.parent = &parentCirc;
	newCirc.solution1 = parentCirc.solution1;
	newCirc.solution2 = parentCirc.solution2;
	newCirc.solution3 = parentCirc.solution3;
	newCirc.correctFlag1 = parentCirc.correctFlag1;
	newCirc.correctFlag2 = parentCirc.correctFlag2;
	newCirc.correctFlag3 = parentCirc.correctFlag3;

	return newCirc;
}

// Creates the original parent circuit
Circuit parentCircuit()
{
	vector<Gate> gates;
	string circuit = "";
	
	// X input
	Gate initOne = initGate(1, NONE, 15, initOne, initOne);			// 00001111
	gates.push_back(initOne);
	circuit += gateToString(initOne);

	// Y input
	Gate initTwo = initGate(2, NONE, 51, initTwo, initTwo);			//0011001100
	gates.push_back(initTwo);
	circuit += gateToString(initTwo);

	// Carry In input
	Gate initThree = initGate(3, NONE, 85, initThree, initThree);	// 01010101
	gates.push_back(initThree);
	circuit += gateToString(initThree);

	Circuit parentCircuit = initCircuit(0, 0, 0, gates, circuit, parentCircuit);
	return parentCircuit;
}

// Prints a gate
void printGate(Gate g)
{
	cout << "\nGATE: " << g.gateNum << endl;
	cout << g.gateLine << endl;
}

// Prints a circuit
void printCircuit(Circuit c)
{
	cout << "Circuit " << c.id << ":" << endl;
	cout << c.circuitry;
}

// Prints a list of circuits
void printCircuitList(vector<Circuit> circuitList)
{
	for (int i = 0; i < circuitList.size(); ++i)
	{
		//cout << "Circuit " << circuitList[i].id << ":" << endl;
		printCircuit(circuitList[i]);
		cout << endl;
	}
}

// Prints all of the tiers to a txt file
void printFile(vector< vector<Circuit> > tiers)
{
	ofstream out("part1_output.txt");
	cout << "Printing file...";
	for (int i = 0; i < tiers.size(); ++i)
	{
		out << "********************************************************" << endl;
		out << "\t\t\tTIER: " << i + 1 << endl;
		out << "********************************************************" << endl;
		vector<Circuit> circuitList = tiers[i];
		for (int j = 0; j < circuitList.size(); ++j)
		{
			Circuit c = circuitList[j];
			out << "Circuit " << c.id << ":" << endl;
			out << c.circuitry;
		}
	}
	cout << "Done!" << endl;
}

void printOutputs(vector< bitset<8> > allOutputs)
{
	ofstream out("all_outputs.txt");
	cout << "Printing outputs...";
	for (int i = 0; i < allOutputs.size(); ++i)
	{
		out << allOutputs[i] << endl;
	}
	cout << "Done!" << endl;
}


// Checks if the gate already exits
bool doesGateExist(vector<Gate> currentGateList, Gate checkFor)
{
	for (int i = 0; i < currentGateList.size(); ++i)
	{
		if (checkFor.value == currentGateList[i].value)
		{
			return true;
		}
	}
	return false;
}

bool doesOutputExist(vector< bitset<8> >& outputList, bitset<8> outputToCheck)
{
	for (int i = 0; i < outputList.size(); ++i)
	{
		if (outputToCheck == outputList[i])
		{
			return true;
		}
	}
	outputList.push_back(outputToCheck);
	return false;
}

//Check if a circuit is a final solution

bool isSolution(Circuit c1){
	if(c1.correctFlag1 && c1.correctFlag2 && c1.correctFlag3){
		return true; 
	}else{
		return false; 
	}

}


//Check circuit that's been mutated or crossed-over
void checkCircuit(Circuit &currentCirc){
	
	bitset<8> OUTPUT1 = 240; 
	bitset<8> OUTPUT2 = 204;
	bitset<8> OUTPUT3 = 170;
	currentCirc.correctFlag1 = false;
	currentCirc.correctFlag2 = false;
	currentCirc.correctFlag3 = false; 
	for(int i=0; i < currentCirc.gateList.size(); ++i){
		
		if(currentCirc.gateList[i].value == OUTPUT1){
			currentCirc.correctFlag1 = true; 
			currentCirc.solution1 = currentCirc.gateList[i]; 
		}

		else if(currentCirc.gateList[i].value == OUTPUT2){
			currentCirc.correctFlag2 = true; 
			currentCirc.solution2 = currentCirc.gateList[i]; 
		}else if (currentCirc.gateList[i].value == OUTPUT3){
			currentCirc.correctFlag3 = true; 
			currentCirc.solution3 = currentCirc.gateList[i]; 	
		}
	
	
	}


}

// Check if a new gate added to a curcuit contains a solution
void checkCorrectOutput(Circuit& currentCirc, Gate toCheck)
{
	bitset<8> OUTPUT1 = 240;						
	bitset<8> OUTPUT2 = 204;					
	bitset<8> OUTPUT3 = 170;
	if (toCheck.value == OUTPUT1)
	{
		currentCirc.correctFlag1 = true;
		currentCirc.solution1 = toCheck;
	}
	else if (toCheck.value == OUTPUT2)
	{
		currentCirc.correctFlag2 = true;
		currentCirc.solution2 = toCheck;
	}
	else if (toCheck.value == OUTPUT3)
	{
		currentCirc.correctFlag3 = true;
		currentCirc.solution3 = toCheck;
	}

}

// Checks if a solution is found
// If one is already found, it will search for the other
bool isSolutionFound(Circuit checking, int switcher)
{
	switch(switcher)
	{
		case 0:
			if (checking.correctFlag1 || checking.correctFlag2)
			{
				cout << "Found a solution " << endl; 
				return true;
			}
			else return false;
			break;
		case 1:
			if (checking.correctFlag1)
			{
				cout << "Found 1st solution " << endl; 
				return true;
			}
			else return false;
			break;
		case 2:
			if (checking.correctFlag2)
			{
				cout << "Found 2nd solution " << endl; 
				return true;
			}
			else return false;
			break;
		default:
			return false;
			break;
	}
}


// Traverses a tier and checks if a circuit is a full solution
bool allSolutionCheck(vector<Circuit>& solTestVect)
{
	for (int i = 0; i < solTestVect.size(); ++i)
	{
		Circuit solTest = solTestVect[i];
		if (solTest.correctFlag1 && solTest.correctFlag2)
		{
			return true;
		}
	}
	return false;
}

// Returns circuit with all solutions
Circuit allSolutionCircuit(vector<Circuit>& solTestVect)
{
	for (int i = 0; i < solTestVect.size(); ++i)
	{
		Circuit solTest = solTestVect[i];
		if (solTest.correctFlag1 && solTest.correctFlag2)
		{
			return solTest;
		}
	}
}





int main(int argc, char const *argv[])
{

	srand(time(NULL));
	bool allSolutionsFound = false; 
	int solutionIndex; 
	int numberOfGenerations = 0;
	vector<Circuit> offspring; 
	vector<Circuit> mostFitCircuits; 
	//1. Choose an initial random population of individuals
	vector<Circuit> population = createRandomPopulation(10000);
	//printCircuitList(population);
	
	//2. Evaluate the fitness of the individuals 
	assignFitnessScores(population); 
	 
	
	//3. Repeat 
	while(!allSolutionsFound){
	
	//4. Select the best individuals to be used by the genetic operators
	
	mostFitCircuits = getMostFit(population); 
	
	cout << endl; 
	//5. Generate new indidviduals using crossover and mutation
	
	geneticOperations(mostFitCircuits, offspring); 
	cout << "Offspring size: " << offspring.size() << endl;
	 
	
	
	//6. Evaluate the fitness of the new individuals  
	assignFitnessScores(offspring); 
	//7. Replace the worst individuals of the population by the best new individuals
	
	
	int tenPercent = population.size() * .10; 
	
	int worstPosition = population.size() - (tenPercent+1); 
	
	int index = 0; 
	
	for(int i=0; i < population.size(); ++i){
		for(int j=0; j < offspring.size(); ++j){
			if(offspring[j].fitnessScore < population[i].fitnessScore){
				population[i] = offspring[j]; 
				offspring[j].fitnessScore = INT_MAX; //Set to large number since we already used it
				break; 
			}
		}
	}
	
	index = 0; 
	
	 for(int i=0; i < population.size(); ++i){
		checkCircuit(population[i]);
		if (population[i].notCount < 3) {
			allSolutionsFound = isSolution(population[i]); 
			solutionIndex = i;
		}
		if (population[solutionIndex].notCount > 2) {
			allSolutionsFound = false;
		} 
	} 
	
	numberOfGenerations++; 
	cout << "Number of Generations: " << numberOfGenerations << endl; 
	cout << "Population Size: " << population.size() << endl; 
	
	double averageFS = 0; 
	for(int i=0; i < population.size(); ++i){
		averageFS += population[i].fitnessScore; 
	}
	
	averageFS = (averageFS / population.size()); 
	cout << "Average FS: " << averageFS << endl; 
	
	for(int i=0; i < population.size(); ++i){
		if(population[i].correctFlag1){
			//cout << "Solution 1 Found " << endl; 
		}
		
		if(population[i].correctFlag2){
			//cout << "Solution 2 Found" << endl; 
		}
	}
	sort(population.begin(), population.end(), compareCircuits());
	cout << "Best Fitness Score: " << population[0].fitnessScore << endl; 
	cout << "Best Circuit: " << endl; 
	cout << "Value: " << population[0].output.to_ulong() << endl; 
	//printCircuit(population[0]); 
	offspring.clear(); 
	mostFitCircuits.clear(); 
	}
	
	
	cout << "Solution Found: " << endl; 
	printCircuit(population[solutionIndex]); 
	
	return 0;
}


	




	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
