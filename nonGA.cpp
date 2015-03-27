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

// returns a circuit conataining a new gate combination with the passed bit operation
void newCircuitCombo(Circuit& parent, bitOp oper, vector<Circuit>& circuitList)
{
	vector<Gate>& parentGates = parent.gateList;
	// cout << "\nparentGates.size(): " << parentGates.size() << endl;
	if (parentGates.size() > 1)
	{
		if (oper == NOT)
		{
			for (int i = 0; i < parentGates.size(); ++i)
			{
				vector<Gate> newGateList = parentGates;
				bitset<8> newResult = bitOperation(oper, parentGates[i].value, parentGates[i].value);

				// cout << "\nparentGates[i]: " << parentGates[i].gateNum << endl;

				Gate nextGate = initGate(parentGates.size() + 1, oper, newResult, parentGates[i], parentGates[i]);
				newGateList.push_back(nextGate);

				string newCircString = parent.circuitry + gateToString(nextGate);
				Circuit circ = initCircuit(circuitList.size(), newResult, parent.notCount, newGateList, newCircString, parent);

				circuitList.push_back(circ);
			}
		}
		// cout << "\nLoop 1:" << endl;
		else {
			for (int i = 0; i < parentGates.size() - 1; ++i)
			{
				// cout << "i: " << i << endl;
				// cout << "parentGates.size() - 1: " << parentGates.size() - 1 << endl;
				// cout << "Loop 2:" << endl;
				for (int j = i + 1; j < parentGates.size(); ++j)
				{
					// cout << "j: " << j << endl;
					// cout << "parentGates.size(): " << parentGates.size() << endl;

					vector<Gate> newGateList = parentGates;
					bitset<8> newResult = bitOperation(oper, parentGates[i].value, parentGates[j].value);

					// cout << "\nparentGates[i]: " << parentGates[i].gateNum << endl;
					// cout << "parentGates[j]: " << parentGates[j].gateNum << endl;

					Gate nextGate = initGate(parentGates.size() + 1, oper, newResult, parentGates[i], parentGates[j]);
					
					newGateList.push_back(nextGate);

					string newCircString = parent.circuitry + gateToString(nextGate);
					Circuit circ = initCircuit(circuitList.size(), newResult, parent.notCount, newGateList, newCircString, parent);

					circuitList.push_back(circ);
				}
			}
		}
	}
	else
	{
		cerr << "ERROR: only 1 gate in the parent.";
	}
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

void printFile(Circuit c)
{
	ofstream out("output.txt");
	out << c.circuitry;
}

void populateTier(Circuit& parent, vector<Circuit>& allCircuits)
{
	bitOp ops[3] = {AND, OR, NOT};
	vector<bitOp> bitOpers(&ops[0], &ops[0] + 3);
	// cout << "\nallCircuits.size(): " < < allCircuits.size() << endl;
	// cout << "\nparent.gateList.size(): " << parent.gateList.size() << endl;

	for (int i = 0; i < bitOpers.size(); ++i)
	{
		if (bitOpers[i] == AND)	// Going through NONE operation combinations
		{
			// cout << "\n********************************************************" << endl;
			// cout << "\t\tAND Operation Gates" << endl;
			// cout << "********************************************************" << endl;

			// vector<Circuit> andCircs = allCircuits;

			// newCircuitCombo(parent, AND, andCircs);
			newCircuitCombo(parent, AND, allCircuits);
			// allCircuits.push_back(andCirc);

			// andCircs.push_back(andCirc);

			// for (int m = 0; m < andCircs.size(); ++m)
			// {
			// 	cout << "Circuit " << m << ":" << endl;
			// 	printCircuit(andCircs[m]);
			// }
		}
		else if (bitOpers[i] == OR)	// Going through OR operation combinations
		{
			// cout << "\n********************************************************" << endl;
			// cout << "\t\tOR Operation Gates" << endl;
			// cout << "********************************************************" << endl;

			// vector<Circuit> orCircs = allCircuits;

			// newCircuitCombo(parent, OR, orCircs);
			newCircuitCombo(parent, OR, allCircuits);
			// allCircuits.push_back(orCirc);

			// orCircs.push_back(orCirc);

			// for (int m = 0; m < orCircs.size(); ++m)
			// {
			// 	cout << "Circuit " << m << ":" << endl;
			// 	printCircuit(orCircs[m]);
			// }
		}
		else if (bitOpers[i] == NOT)	// Going through NOT operation combinations
		{
			// cout << "\n********************************************************" << endl;
			// cout << "\t\tNOT Operation Gates" << endl;
			// cout << "********************************************************" << endl;

			// vector<Circuit> notCircs = allCircuits;

			// newCircuitCombo(parent, NOT, notCircs);
			newCircuitCombo(parent, NOT, allCircuits);
			// allCircuits.push_back(notCirc);

			// notCircs.push_back(notCirc);

			// for (int m = 0; m < notCircs.size(); ++m)
			// {
			// 	cout << "Circuit " << m << ":" << endl;
			// 	printCircuit(notCircs[m]);
			// }
		}
		else cerr << "ERROR: 'j' is not what it should be!";
	}
}

int main(int argc, char const *argv[])
{
	Circuit initial = parentCircuit();
	vector<Circuit> circuits;
	circuits.push_back(initial);
	cout << "********************************************************" << endl;
	cout << "\t\t\tINITIAL" << endl;
	cout << "********************************************************" << endl;
	printCircuit(initial);
	vector<Gate> initialGates = initial.gateList;

	populateTier(initial, circuits);

	cout << "********************************************************" << endl;
	cout << "\t\t\tFINAL LIST" << endl;
	cout << "********************************************************" << endl;
	printCircuitList(circuits);

	return 0;
}
