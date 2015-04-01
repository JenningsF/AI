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
	Gate solution3;
	bool correctFlag1 = false;
	bool correctFlag2 = false;
	bool correctFlag3 = false;
};

bitset<8> INPUT1 = 15;		// 00001111
bitset<8> INPUT2 = 51;		// 00110011
bitset<8> INPUT3 = 85;		// 01010101

bitset<8> OUTPUT1 = 240;	//11110000
bitset<8> OUTPUT2 = 204;	//11001100
bitset<8> OUTPUT3 = 170;	//10101010

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
	Gate initOne = initGate(1, NONE, INPUT1, initOne, initOne);			// 00001111
	gates.push_back(initOne);
	circuit += gateToString(initOne);

	// Y input
	Gate initTwo = initGate(2, NONE, INPUT2, initTwo, initTwo);			//0011001100
	gates.push_back(initTwo);
	circuit += gateToString(initTwo);

	// Carry In input
	Gate initThree = initGate(3, NONE, INPUT3, initThree, initThree);	// 01010101
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
	ofstream out("part2_output.txt");
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

	cout << "\nInput 1:\t" << INPUT1 << endl;
	cout << "Input 2:\t" << INPUT2 << endl;
	cout << "Input 3:\t" << INPUT3 << endl;

	cout << "\nOutput 1:\t" << OUTPUT1 << endl;
	cout << "Output 2:\t" << OUTPUT2 << endl;
	cout << "Output 3:\t" << OUTPUT3 << endl;
	cout << "\nSolution 1:";
	printGate(sol.solution1);
	cout << "Solution 2:";
	printGate(sol.solution2);
	cout << "Solution 3:";
	printGate(sol.solution3);
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

void setInputs(int in1, int in2, int in3)
{
	INPUT1 = in1;
	INPUT2 = in2;
	INPUT3 = in3;
}

void setOutputs()
{
	OUTPUT1 = ~INPUT1;
	OUTPUT2 = ~INPUT2;
	OUTPUT3 = ~INPUT3;
}

// Check if a new gate added to a curcuit contains a solution
void checkCorrectOutput(Circuit& currentCirc, Gate toCheck)
{
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
bool isSolutionFound(Circuit checking, int& switcher)
{
	switch(switcher)
	{
		case 0:
			if (checking.correctFlag1 || checking.correctFlag2 || checking.correctFlag3)
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
		case 3:
			if (checking.correctFlag3)
			{
				cout << "Found 3rd solution " << endl;
				return true;
			}
			else return false;
			break;
		case 4:
			if (checking.correctFlag1 || checking.correctFlag2)
			{
				cout << "Found another solution " << endl;
				return true;
			}
			else return false;
			break;
		case 5:
			if (checking.correctFlag1 || checking.correctFlag3)
			{
				cout << "Found another solution " << endl;
				return true;
			}
			else return false;
			break;
		case 6:
			if (checking.correctFlag2 || checking.correctFlag3)
			{
				cout << "Found another solution " << endl;
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
		if (solTest.correctFlag1 && solTest.correctFlag2 && solTest.correctFlag3)
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
		if (solTest.correctFlag1 && solTest.correctFlag2 && solTest.correctFlag3)
		{
			return solTest;
		}
	}
}

// Creates a circuit conataining a new gate combination with the passed bit operation
void newCircuitCombo(Circuit& parent, bitOp oper, vector<Circuit>& newCircuitList, int& numOfCircs, int& tierSize, vector< bitset<8> >& outputList, bool& solutionFound, int& solSwitch, int entireSize)
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
				if (!doesGateExist(parentGates, nextGate) 
					&& !doesOutputExist(outputList, nextGate.value) 
					&& parent.notCount < 2
					&& entireSize > 4
					)
				{
					newGateList.push_back(nextGate);
					string newCircString = parent.circuitry + gateToString(nextGate);
					Circuit circ = initCircuit(tierSize + 1, newResult, parent.notCount + 1, newGateList, newCircString, parent);
					checkCorrectOutput(circ, nextGate);
					newCircuitList.push_back(circ);
					++numOfCircs;
					++tierSize;
					if (isSolutionFound(circ, solSwitch))
					{
						solutionFound = true;
						if (circ.correctFlag2 && circ.correctFlag3)
						{
							solSwitch = 1;
						}
						else if (circ.correctFlag1 && circ.correctFlag3)
						{
							solSwitch = 2;
						}
						else if (circ.correctFlag1 && circ.correctFlag2)
						{
							solSwitch = 3;
						}
						else if (circ.correctFlag3)
						{
							solSwitch = 4;
						}
						else if (circ.correctFlag2)
						{
							solSwitch = 5;
						}
						else if (circ.correctFlag1)
						{
							solSwitch = 6;
						}
						break;
					}
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
					if (!doesGateExist(parentGates, nextGate)
						 && !doesOutputExist(outputList, nextGate.value)
						 )
					{
						newGateList.push_back(nextGate);
						string newCircString = parent.circuitry + gateToString(nextGate);
						Circuit circ = initCircuit(tierSize + 1, newResult, parent.notCount, newGateList, newCircString, parent);
						checkCorrectOutput(circ, nextGate);
						newCircuitList.push_back(circ);
						++numOfCircs;
						++tierSize;
						if (isSolutionFound(circ, solSwitch))
						{
							solutionFound = true;
							if (circ.correctFlag2 && circ.correctFlag3)
							{
								solSwitch = 1;
							}
							else if (circ.correctFlag1 && circ.correctFlag3)
							{
								solSwitch = 2;
							}
							else if (circ.correctFlag1 && circ.correctFlag2)
							{
								solSwitch = 3;
							}
							else if (circ.correctFlag3)
							{
								solSwitch = 4;
							}
							else if (circ.correctFlag2)
							{
								solSwitch = 5;
							}
							else if (circ.correctFlag1)
							{
								solSwitch = 6;
							}
							break;
						}
					}
				}
				if (solutionFound) break;
			}
		}
	}
	else
	{
		cerr << "ERROR: only 1 gate in the parent." << endl;
	}
}

// Takes parent circuit from one tier and populates the tier below it
vector<Circuit> populateTier(Circuit& parent, int& numOfCircs, int& tierSize, vector< bitset<8> >& outputList, bool& solutionFound, int& solSwitch, int entireSize)
{
	bitOp ops[3] = {AND, OR, NOT};
	vector<bitOp> bitOpers(&ops[0], &ops[0] + 3);
	vector<Circuit> newTier;
	for (int i = 0; i < bitOpers.size(); ++i)
	{
		// Going through AND operation combinations
		if (bitOpers[i] == AND)
		{
			newCircuitCombo(parent, AND, newTier, numOfCircs, tierSize, outputList, solutionFound, solSwitch, entireSize);
			if (solutionFound) break;
		}
		// Going through OR operation combinations
		else if (bitOpers[i] == OR)
		{
			newCircuitCombo(parent, OR, newTier, numOfCircs, tierSize, outputList, solutionFound, solSwitch, entireSize);
			if (solutionFound) break;
		}
		// Going through NOT operation combinations
		else if (bitOpers[i] == NOT)
		{
			newCircuitCombo(parent, NOT, newTier, numOfCircs, tierSize, outputList, solutionFound, solSwitch, entireSize);
			if (solutionFound) break;
		}
		else cerr << "ERROR: 'j' is not what it should be!";
	}
	return newTier;
}

// Creates an entire tier
vector< vector<Circuit> > createPopulation(vector< vector<Circuit> > tiers, int& numOfCircs, bool& solutionFound)
{
	bool allSolutionsFound = false;
	bool firstSolutionFound = solutionFound;
	bool secondSolutionFound = false;
	bool thirdSolutionFound = false;
	bool firstSolTierMade = false;
	bool secondSolTierMade = false;
	// bool emptyTier = true;
	int solSwitch = 0;
	int i = 0;
	int numOfTiers = 8;

	while (!allSolutionsFound
			// && i < numOfTiers
			)
	{
		vector<Circuit> partialNewTier;
		vector<Circuit> entireNewTier;
		vector< bitset<8> > outputList;
		vector<Circuit> currentTier;
		int tierSize = 0;
		// Creates tiers until a solution is found
		if (!firstSolutionFound && !allSolutionsFound)
		{
			partialNewTier.clear();
			entireNewTier.clear();
			outputList.clear();
			currentTier = tiers[tiers.size() - 1];
			tierSize = 0;

			cout << "\nMaking Tier " << tiers.size() + 1 << "..." << endl;
			for (int j = 0; j < currentTier.size(); ++j)
			{
				Circuit& parent = currentTier[j];
				partialNewTier = populateTier(parent, numOfCircs, tierSize, outputList, firstSolutionFound, solSwitch, tiers.size());
				entireNewTier.insert(entireNewTier.end(), partialNewTier.begin(), partialNewTier.end());
				if (firstSolutionFound)
				{
					cout << "First Solution Found!" << endl;
					break;
				}
			}
			if (entireNewTier.size() < 1)
			{
				cerr << "No more circuits being made.";
			}
			tiers.push_back(entireNewTier);
			cout << "Tier " << tiers.size() << " made!" << endl;
			cout << "There are now " << numOfCircs << " circuits total." << endl;
		}
		// Once one solution is found, use that circuit to create next tier
		if (firstSolutionFound && !firstSolTierMade &&
			!secondSolutionFound && !secondSolTierMade &&
			!thirdSolutionFound && !allSolutionsFound)
		{
			partialNewTier.clear();
			entireNewTier.clear();
			outputList.clear();
			currentTier = tiers[tiers.size() - 1];
			tierSize = 0;

			cout << "\nMaking Tier " << tiers.size() + 1 << "..." << endl;
			vector<Circuit> lastTier = tiers[tiers.size() - 1];
			
			Circuit& firstParentSolution = lastTier[lastTier.size() - 1];
			entireNewTier = populateTier(firstParentSolution, numOfCircs, tierSize, outputList, secondSolutionFound, solSwitch, tiers.size());

			tiers.push_back(entireNewTier);
			cout << "Tier " << tiers.size() << " made!" << endl;
			cout << "There are now " << numOfCircs << " circuits total." << endl;
			if (secondSolutionFound)
			{
				cout << "Second Solution Found!" << endl;
			}
			firstSolTierMade = true;
		}
		// Continue creating population as usual until second solution is found
		if (firstSolutionFound && firstSolTierMade && !secondSolutionFound && !secondSolTierMade && !thirdSolutionFound && !allSolutionsFound)
		{
			partialNewTier.clear();
			entireNewTier.clear();
			outputList.clear();
			currentTier = tiers[tiers.size() - 1];
			tierSize = 0;

			cout << "\nMaking Tier " << tiers.size() + 1 << "..." << endl;
			for (int k = 0; k < currentTier.size(); ++k)
			{
				Circuit& newParent = currentTier[k];
				partialNewTier = populateTier(newParent, numOfCircs, tierSize, outputList, secondSolutionFound, solSwitch, tiers.size());
				entireNewTier.insert(entireNewTier.end(), partialNewTier.begin(), partialNewTier.end());
				if (secondSolutionFound)
				{
					cout << "Second Solution Found!" << endl;
					break;
				}
			}
			if (entireNewTier.size() < 1)
			{
				cerr << "No more circuits being made.";
			}
			tiers.push_back(entireNewTier);
			cout << "Tier " << tiers.size() << " made!" << endl;
			cout << "There are now " << numOfCircs << " circuits total." << endl;
		}
		// Once second solution is found, use that circuit to create next tier
		if (firstSolutionFound && firstSolTierMade &&
			secondSolutionFound && !secondSolTierMade &&
			!thirdSolutionFound && !allSolutionsFound)
		{
			partialNewTier.clear();
			entireNewTier.clear();
			outputList.clear();
			currentTier = tiers[tiers.size() - 1];
			tierSize = 0;

			cout << "\nMaking Tier " << tiers.size() + 1 << "..." << endl;
			vector<Circuit> lastTier = tiers[tiers.size() - 1];
			
			Circuit& secondParentSolution = lastTier[lastTier.size() - 1];
			entireNewTier = populateTier(secondParentSolution, numOfCircs, tierSize, outputList, thirdSolutionFound, solSwitch, tiers.size());

			tiers.push_back(entireNewTier);
			cout << "Tier " << tiers.size() << " made!" << endl;
			cout << "There are now " << numOfCircs << " circuits total." << endl;
			if (thirdSolutionFound)
			{
				cout << "Second Solution Found!" << endl;
			}
			secondSolTierMade = true;
		}
		// Continue creating population as usual until third solution is found
		if (firstSolutionFound && firstSolTierMade && secondSolutionFound && secondSolTierMade && !thirdSolutionFound && !allSolutionsFound)
		{
			partialNewTier.clear();
			entireNewTier.clear();
			outputList.clear();
			currentTier = tiers[tiers.size() - 1];
			tierSize = 0;

			cout << "\nMaking Tier " << tiers.size() + 1 << "..." << endl;
			for (int l = 0; l < currentTier.size(); ++l)
			{
				Circuit& newParent = currentTier[l];
				partialNewTier = populateTier(newParent, numOfCircs, tierSize, outputList, thirdSolutionFound, solSwitch, tiers.size());
				entireNewTier.insert(entireNewTier.end(), partialNewTier.begin(), partialNewTier.end());
				if (thirdSolutionFound)
				{
					cout << "Third Solution Found!" << endl;
					break;
				}
			}
			if (entireNewTier.size() < 1)
			{
				cerr << "No more circuits being made.";
			}
			tiers.push_back(entireNewTier);
			cout << "Tier " << tiers.size() << " made!" << endl;
			cout << "There are now " << numOfCircs << " circuits total." << endl;
		}
		allSolutionsFound = allSolutionCheck(entireNewTier);
		if (allSolutionsFound)
		{
			cout << "All Solutions Found!" << endl;
			break;
		}
		++i;
	}
	cout << "***** All Solutions were found *****" << endl;
	// Circuit solution = allSolutionCircuit(tiers[tiers.size() - 1]);
	// printSolution(solution, tiers.size());
	return tiers;
}

int main(int argc, char const *argv[])
{
	vector< vector<Circuit> > tiers;
	vector< vector<Circuit> > allTiers;
	vector<Circuit> circuits;
	int input1, input2, input3;

	// cout << "Please input a number for output 1" << endl;
	// cout << "* Must be 255 or less *" << endl;
	// cin >> input1;
	// while (cin.fail() || input1 > 255)
	// {
	// 	cin.clear();
	// 	cin.ignore(256, '\n');
	// 	cout << "Not a valid int!" << endl;
	// 	cout << "Please input a number for output 1" << endl;
	// 	cout << "* Must be 255 or less *" << endl;
	// 	cin >> input1;
	// }
	// cout << "Please input a number for output 2" << endl;
	// cout << "* Must be 255 or less *" << endl;
	// cin >> input2;
	// while (cin.fail() || input2 > 255)
	// {
	// 	cin.clear();
	// 	cin.ignore(256, '\n');
	// 	cout << "Not a valid int!" << endl;
	// 	cout << "Please input a number for output 2" << endl;
	// 	cout << "* Must be 255 or less *" << endl;
	// 	cin >> input2;
	// }
	// cout << "Please input a number for output 3" << endl;
	// cout << "* Must be 255 or less *" << endl;
	// cin >> input3;
	// while (cin.fail() || input3 > 255)
	// {
	// 	cin.clear();
	// 	cin.ignore(256, '\n');
	// 	cout << "Not a valid int!" << endl;
	// 	cout << "Please input a number for output 3" << endl;
	// 	cout << "* Must be 255 or less *" << endl;
	// 	cin >> input3;
	// }

	// setInputs(input1, input2, input3);
	// setOutputs();

	Circuit initial = parentCircuit();
	circuits.push_back(initial);
	tiers.push_back(circuits);
	int circuitCount = 1;
	bool firstSolutionFound = false;



	clock_t begin = clock();
	allTiers = createPopulation(tiers, circuitCount, firstSolutionFound);
	clock_t end = clock();
	double elapsedTime = double(end - begin) / CLOCKS_PER_SEC;
	cout << "Elapsed Time : " << elapsedTime << endl;

	printFile(allTiers);
	return 0;
}
