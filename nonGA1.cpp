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
	int id, notCount;
	bitset<8> output;
	vector<Gate> gateList;
	string circuitry;
	Circuit* parent;
	Gate solution1;
	Gate solution2;
	bool correctFlag1 = false;
	bool correctFlag2 = false;
};

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

	if (convert.operation == NOT)
	{
		gateLine += "\t";
	}
	gateLine += "\tValue: " + convert.value.to_string();

	gateLine += "\r\n";

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
	newCirc.correctFlag1 = parentCirc.correctFlag1;
	newCirc.correctFlag2 = parentCirc.correctFlag2;

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
		cout << "Circuit " << circuitList[i].id << ":" << endl;
		printCircuit(circuitList[i]);
	}
}

// Prints all the tiers of circuits
void printTiers(vector< vector<Circuit> > tiers)
{
	// cout << "tiers.size(): " << tiers.size() << endl;
	for (int i = 0; i < tiers.size(); ++i)
	{
		cout << "********************************************************" << endl;
		cout << "\t\t\tTIER: " << i + 1 << endl;
		cout << "********************************************************" << endl;
		printCircuitList(tiers[i]);
	}
}

// Prints all of the tiers to a txt file
void printFile(vector< vector<Circuit> > tiers)
{
	ofstream out("output.txt");
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

// Prints the solution once it's found
void printSolution(Circuit& sol, int tierNum)
{
	cout << "\n********************************************************" << endl;
	cout << "\t\t\tSOLUTION" << endl;
	cout << "********************************************************" << endl;
	
	cout << "Tier " << tierNum << endl;
	printCircuit(sol);
	cout << "\nSolution 1:";
	printGate(sol.solution1);
	cout << "Solution 2:";
	printGate(sol.solution2);
}

// Checks if the gate already exits
bool doesGateExist(vector<Gate> currentGateList, Gate checkFor)
{
	// vector<Gate> currentGateList = current.gateList;
	for (int i = 0; i < currentGateList.size(); ++i)
	{
		if (checkFor.operId1 == currentGateList[i].operId1 &&
			checkFor.operId2 == currentGateList[i].operId2 &&
			checkFor.value == currentGateList[i].value)
		{
			return true;
		}
	}
	return false;
}

// Check if a new gate added to a curcuit contains a solution
void checkCorrectOutput(Circuit& currentCirc, Gate toCheck)
{
	bitset<8> OUTPUT1 = 23;
	bitset<8> OUTPUT2 = 105;

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
}

// Traverses a tier and checks if a circuit is a full solution
bool solutionCheck(vector<Circuit>& solTestVect)
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

// Returns solution circuit 
Circuit solutionCircuit(vector<Circuit>& solTestVect)
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

// Creates a circuit conataining a new gate combination with the passed bit operation
void newCircuitCombo(Circuit& parent, bitOp oper, vector<Circuit>& newCircuitList, int& numOfCircs, int& tierSize)
{
	vector<Gate>& parentGates = parent.gateList;
	if (parentGates.size() > 1)
	{
		if (oper == NOT)
		{
			for (int i = 0; i < parentGates.size(); ++i)
			{
				vector<Gate> newGateList = parentGates;
				bitset<8> newResult = bitOperation(oper, parentGates[i].value, parentGates[i].value);

				Gate nextGate = initGate(newGateList.size() + 1, oper, newResult, parentGates[i], parentGates[i]);
				// check if gate already exists, so gates aren't duplicated
				if (!doesGateExist(parentGates, nextGate) && parent.notCount < 2)
				{
					newGateList.push_back(nextGate);
					string newCircString = parent.circuitry + gateToString(nextGate);
					Circuit circ = initCircuit(tierSize + 1, newResult, parent.notCount + 1, newGateList, newCircString, parent);
					checkCorrectOutput(circ, nextGate);
					newCircuitList.push_back(circ);
					++numOfCircs;
					++tierSize;
					cout << "Total # of Circuits: " << numOfCircs << endl;
				}
			}
		}
		else
		{
			for (int i = 0; i < parentGates.size() - 1; ++i)
			{
				for (int j = i + 1; j < parentGates.size(); ++j)
				{
					vector<Gate> newGateList = parentGates;
					bitset<8> newResult = bitOperation(oper, parentGates[i].value, parentGates[j].value);

					Gate nextGate = initGate(newGateList.size() + 1, oper, newResult, parentGates[i], parentGates[j]);
					// check if gate already exists, so gates aren't duplicated
					if (!doesGateExist(parentGates, nextGate))
					{
						newGateList.push_back(nextGate);
						string newCircString = parent.circuitry + gateToString(nextGate);
						Circuit circ = initCircuit(tierSize + 1, newResult, parent.notCount, newGateList, newCircString, parent);
						checkCorrectOutput(circ, nextGate);
						newCircuitList.push_back(circ);
						++numOfCircs;
						++tierSize;
						cout << "Total # of Circuits: " << numOfCircs << endl;
					}
				}
			}
		}
	}
	else
	{
		cerr << "ERROR: only 1 gate in the parent." << endl;
	}
}

// Takes parent circuit from one tier and populates the tier below it
vector<Circuit> populateTier(Circuit& parent, int& numOfCircs, int& tierSize)
{
	bitOp ops[3] = {AND, OR, NOT};
	vector<bitOp> bitOpers(&ops[0], &ops[0] + 3);
	vector<Circuit> newTier;
	for (int i = 0; i < bitOpers.size(); ++i)
	{
		// Going through AND operation combinations
		if (bitOpers[i] == AND)
		{
			newCircuitCombo(parent, AND, newTier, numOfCircs, tierSize);
		}
		// Going through OR operation combinations
		else if (bitOpers[i] == OR)
		{
			newCircuitCombo(parent, OR, newTier, numOfCircs, tierSize);
		}
		// Going through NOT operation combinations
		else if (bitOpers[i] == NOT)
		{
			newCircuitCombo(parent, NOT, newTier, numOfCircs, tierSize);
		}
		else cerr << "ERROR: 'j' is not what it should be!";
	}
	// cout << "newTier Size: " << newTier.size() << endl;
	return newTier;
}

// Creates an entire tier
void createPopulation(vector< vector<Circuit> >& tiers, int numOfTiers, int& numOfCircs)
{
	bool solutionFound = false;
	vector<Circuit> entireNewTier;
		for (int i = 1; ; ++i)
		{
			vector<Circuit> currentTier = tiers[i - 1];
			int tierSize = 0;
			for (int j = 0; j < currentTier.size(); ++j)
			{
				Circuit& parent = currentTier[j];
				vector<Circuit> partialNewTier = populateTier(parent, numOfCircs, tierSize);
				entireNewTier.insert(entireNewTier.end(), partialNewTier.begin(), partialNewTier.end());
			}
			tiers.push_back(entireNewTier);
			cout << "Tier " << tiers.size() << " made!" << endl;
			cout << "There are now " << entireNewTier.size() + 1 << " circuits total." << endl;
			solutionFound = solutionCheck(entireNewTier);
			if (solutionFound) 
			{
				cout << "***** Solution was found *****\nBreaking now..." << endl;
				break;
			}
		}
	Circuit solution = solutionCircuit(entireNewTier);
	printSolution(solution, tiers.size());
}

int main(int argc, char const *argv[])
{
	// Change to determine how many tiers you would like
	int NUMOFTIERS = 5;
	Circuit initial = parentCircuit();
	vector< vector<Circuit> > tiers;
	vector<Circuit> circuits;
	circuits.push_back(initial);
	tiers.push_back(circuits);
	int circuitCount = 1;

	clock_t begin = clock();
	createPopulation(tiers, NUMOFTIERS, circuitCount);
	clock_t end = clock();
	double elapsedTime = double(end - begin) / CLOCKS_PER_SEC;
	cout << "Elapsed Time : " << elapsedTime << endl;

	printFile(tiers);

	return 0;
}
