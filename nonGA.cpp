/********************************************************
Name		: Jennings Fairchild
		    : 
			: 
			: 
Subject		: CSCE 315-504
Assignment	: Project 3: AI
********************************************************/
#include <string>
#include <vector>
#include <bitset>
#include <iostream>
#include <fstream>

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

string gateToString(Gate convert)	// maybe need a Gate&
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

Circuit initCircuit(int gateId, bitset<8> outputVal, int nots, vector<Gate> gates, string circuit, Circuit& parentCirc)
{
	Circuit newCirc;
	newCirc.id = gateId;
	newCirc.output = outputVal;
	newCirc.notCount = nots;
	newCirc.gateList = gates;
	newCirc.circuitry = circuit;
	newCirc.parent = &parentCirc;

	return newCirc;
}

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

void printGate(Gate g)
{
	cout << "gateNum: " << g.gateNum << endl;
	cout << "value: " << g.value << endl;
	cout << "operId1: " << g.operId1 << endl;
	cout << "operId2: " << g.operId2 << endl;
	cout << "operVal1: " << g.operVal1 << endl;
	cout << "operVal2: " << g.operVal2 << endl;
	cout << "operation: " << g.operation << endl;
	cout << "gateLine: " << g.gateLine << endl;
}

void printCircuit(Circuit c)
{
	cout << c.circuitry;
}

void printCircuitList(vector<Circuit> circuitList)
{
	for (int i = 0; i < circuitList.size(); ++i)
	{
		cout << "Circuit " << i + 1 << ":" << endl;
		printCircuit(circuitList[i]);
	}
}

void printTiers(vector< vector<Circuit> > tiers)
{
	cout << "tiers.size(): " << tiers.size() << endl;
	for (int i = 0; i < tiers.size(); ++i)
	{
		cout << "\n********************************************************" << endl;
		cout << "\t\t\tTIER: " << i + 1 << endl;
		cout << "********************************************************" << endl;
		printCircuitList(tiers[i]);
	}
}

void printFile(vector< vector<Circuit> > tiers)
{
	ofstream out("output.txt");
	cout << "Printing file...\t";
	for (int i = 0; i < tiers.size(); ++i)
	{
		out << "\n********************************************************" << endl;
		out << "\t\t\tTIER: " << i + 1 << endl;
		out << "********************************************************" << endl;
		vector<Circuit> circuitList = tiers[i];
		for (int j = 0; j < circuitList.size(); ++j)
		{
			Circuit c = circuitList[j];
			out << "Circuit " << j + 1 << ":" << endl;
			out << c.circuitry;
		}
	}
	cout << "Done!" << endl;
}

void printSolution(Circuit& sol, int tierNum)
{
	cout << "\n********************************************************" << endl;
	cout << "\t\t\tSOLUTION" << endl;
	cout << "********************************************************" << endl;
	
	cout << "Tier " << tierNum << endl;
	cout << "Circuit " << sol.id << endl;
	printCircuit(sol);
}

bool doesGateExist(Circuit current, Gate checkFor)
{
	vector<Gate> currentGateList = current.gateList;
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
	}
	else if (toCheck.value == OUTPUT2)
	{
		currentCirc.correctFlag2 = true;
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
void newCircuitCombo(Circuit& parent, bitOp oper, vector<Circuit>& circuitList)
{
	// cout << "---------Inside newCircuitCombo---------" << endl;
	// cout << "circuitList: " << circuitList.size() << endl;
	vector<Gate>& parentGates = parent.gateList;
	if (parentGates.size() > 1)
	{
		if (oper == NOT)
		{
			for (int i = 0; i < parentGates.size(); ++i)
			{
				// cout << "i: " << i << "\t"; 
				// cout << "size: " << parentGates.size() << endl;
				// cout << "Inside NOT loop." << endl;
				vector<Gate> newGateList = parentGates;
				bitset<8> newResult = bitOperation(oper, parentGates[i].value, parentGates[i].value);

				Gate nextGate = initGate(parentGates.size() + 1, oper, newResult, parentGates[i], parentGates[i]);
				// check if gate already exists, so gates aren't duplicated
				if (!doesGateExist(parent, nextGate) && parent.notCount < 2)
				{
					newGateList.push_back(nextGate);
					string newCircString = parent.circuitry + gateToString(nextGate);
					Circuit circ = initCircuit(circuitList.size(), newResult, parent.notCount + 1, newGateList, newCircString, parent);
					circuitList.push_back(circ);
					checkCorrectOutput(circ, nextGate);
					// cout << "Circ pushed back." << endl;
					// printCircuit(circ);
				}
			}
		}
		else
		{
			for (int i = 0; i < parentGates.size() - 1; ++i)
			{
				// cout << "i: " << i << "\t"; 
				// cout << "size() - 1: " << parentGates.size() - 1 << endl;
				// cout << "Inside OTHER loop 1." << endl;
				for (int j = i + 1; j < parentGates.size(); ++j)
				{
					// cout << "j: " << j << "\t"; 
					// cout << "size(): " << parentGates.size() << endl;
					// cout << "Inside OTHER loop 2." << endl;
					vector<Gate> newGateList = parentGates;
					bitset<8> newResult = bitOperation(oper, parentGates[i].value, parentGates[j].value);

					Gate nextGate = initGate(parentGates.size() + 1, oper, newResult, parentGates[i], parentGates[j]);
					// check if gate already exists, so gates aren't duplicated
					if (!doesGateExist(parent, nextGate))
					{
						newGateList.push_back(nextGate);
						string newCircString = parent.circuitry + gateToString(nextGate);
						Circuit circ = initCircuit(circuitList.size(), newResult, parent.notCount, newGateList, newCircString, parent);
						circuitList.push_back(circ);
						// cout << "Circ pushed back." << endl;
						// printCircuit(circ);
					}
					// cout << "End of loop2." << endl;
				}
				// cout << "End of loop1." << endl;
			}
		}
	}
	else
	{
		cerr << "ERROR: only 1 gate in the parent.";
	}
	// cout << "---------Inside newCircuitCombo---------" << endl;
}

// Takes parent circuit from one tier and populates the tier below it
vector<Circuit> populateTier(Circuit& parent)
{
	// cout << "-------Inside populateTier-------" << endl;
	bitOp ops[3] = {AND, OR, NOT};
	vector<bitOp> bitOpers(&ops[0], &ops[0] + 3);
	vector<Circuit> newTier;

	// cout << "bitOpers.size(): " << bitOpers.size() << endl;
	for (int i = 0; i < bitOpers.size(); ++i)
	{
		// Going through AND operation combinations
		if (bitOpers[i] == AND)
		{
			// cout << "*****Going through AND operation combinations*****" << endl;
			newCircuitCombo(parent, AND, newTier);
		}
		// Going through OR operation combinations
		else if (bitOpers[i] == OR)
		{
			// cout << "*****Going through OR operation combinations*****" << endl;
			newCircuitCombo(parent, OR, newTier);
		}
		// Going through NOT operation combinations
		else if (bitOpers[i] == NOT)
		{
			// cout << "*****Going through NOT operation combinations*****" << endl;
			newCircuitCombo(parent, NOT, newTier);
		}
		// else cerr << "ERROR: 'j' is not what it should be!";
	}
	return newTier;
	// cout << "-------Ending populateTier-------" << endl;
}

void createPopulation(vector< vector<Circuit> >& tiers, int numOfTiers)
{
	// cout << "-----Inside createPopulation-----" << endl;
	// cout << "\t\t Initial tiers.size(): " << tiers.size() << endl;
	// cout << "numOfTiers: " << numOfTiers << endl;
	bool solutionFound = false;
	vector<Circuit> entireNewTier;
		for (int i = 1; ; ++i)
		{
			// cout << "***1st for loop***" << endl;
			// cout << "i: " << i << "\tnumOfTiers: " << numOfTiers << endl;
			// vector<Circuit> entireNewTier;
			vector<Circuit> currentTier = tiers[i - 1];
			// cout << "currentTier.size(): " << currentTier.size() << endl;
			for (int j = 0; j < currentTier.size(); ++j)
			{
				// cout << "\t******2nd for loop******" << endl;
				Circuit& parent = currentTier[j];
				// cout << "\tj: " << j << "\tcurrentTier: " << currentTier.size() << endl;
				vector<Circuit> partialNewTier = populateTier(parent);
				entireNewTier.insert(entireNewTier.end(), partialNewTier.begin(), partialNewTier.end());
				// cout << "\n********************************************************" << endl;
				// cout << "\t\t\tOld 1st tier" << endl;
				// cout << "********************************************************" << endl;
				// printCircuitList(tiers[0]);
				// cout << "\n********************************************************" << endl;
				// cout << "\t\t\tnewTier" << endl;
				// cout << "********************************************************" << endl;
				// printCircuitList(newTier);
				// cout << "Old Tiers Size: " << tiers.size() << endl;
				// tiers.push_back(newTier);
				// cout << "New Tiers Size: " << tiers.size() << endl;
				// cout << "\n********************************************************" << endl;
				// cout << "\t\t\tNew 1st tier" << endl;
				// cout << "********************************************************" << endl;
				// printCircuitList(tiers[0]);
				// cout << "\t******End of 2nd for loop******" << endl;
			}
			tiers.push_back(entireNewTier);
			cout << "Tier " << tiers.size() << " made!" << endl;
			solutionFound = solutionCheck(entireNewTier);
			if (solutionFound) break;
			// cout << "***End of 1st for loop***" << endl;
		}
	Circuit solution = solutionCircuit(entireNewTier);
	printSolution(solution, tiers.size());
}

int main(int argc, char const *argv[])
{
	// Change to determine how many tiers you would like
	int NUMOFTIERS = 4;
	Circuit initial = parentCircuit();
	vector< vector<Circuit> > tiers;
	vector<Circuit> circuits;
	circuits.push_back(initial);
	tiers.push_back(circuits);

	// Gate temp = initGate(2, NONE, 15, temp, temp);
	// cout << doesGateExist(initial, temp);

	// printTiers(tiers);

	// cout << "Tiers Size: " << tiers.size() << endl;
	createPopulation(tiers, NUMOFTIERS);
	// cout << "Tiers Size: " << tiers.size() << endl;

	// populateTier(initial, circuits);

	// cout << "********************************************************" << endl;
	// cout << "\t\t\tFINAL LIST" << endl;
	// cout << "********************************************************" << endl;
	// printCircuitList(circuits);

	// printTiers(tiers);
	printFile(tiers);


	return 0;
}
