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
#include <iostream>
#include <fstream>

using namespace std;

enum bitOp {NONE, AND, OR, NOT};

struct Gate {
	int gateNum, value, operand1, operand2;
	bitOp operation;
	string gateLine;
};

// *** NEED TO ADD PARENT CIRCUIT FUNCTION FOR "TREE" ***
struct Circuit {
	int id, output, notCount;
	vector<Gate> gateList;
	string circuitry;
	Circuit* parent;
};

string opToString(int operation) {
	switch(operation) {
		case 0:
			return "NONE";
			break;
		case 1:
			return "AND";
			break;
		case 2:
			return "OR";
			break;
		case 3:
			return "NOT";
			break;
		default:
			return "NULL";
			break;
	}
}

int bitOperation(int operation, int operand1, int operand2) {
	int result;
	switch(operation) {
		case 0:
			result = operand1;
			return result;
			break;
		case 1:
			result = operand1 & operand2;
			return result;
			break;
		case 2:
			result = operand1 | operand2;
			return result;
			break;
		case 3:
			result = ~operand1;
			return result;
			break;
		default:
			return 0;
			break;
	}
}

string gateToString(Gate convert) {			// maybe need a Gate&
	string gateLine =	to_string(convert.gateNum) + " " +
						opToString(convert.operation) + " " +
						to_string(convert.operand1);
	if (convert.operand2 != 0) {
		gateLine += to_string(convert.operand2);
	}

	return gateLine;
}

Gate initGate(int id, bitOp op, int val, int oper1, int oper2) {
	Gate newGate;
	newGate.gateNum = id;
	newGate.operation = op;
	newGate.value = val;
	newGate.operand1 = oper1;	
	newGate.operand2 = oper2;
	newGate.gateLine = gateToString(newGate);

	return newGate;
}

Circuit initCircuit(int gateId, int outputVal, int nots, vector<Gate> gates, string circuit, Circuit* parentCirc) {
	Circuit newCirc;
	newCirc.id = gateId;
	newCirc.output = outputVal;
	newCirc.notCount = nots;
	newCirc.gateList = gates;
	newCirc.circuitry = circuit;
	newCirc.parent = parentCirc;

	return newCirc;
}

Circuit parentCircuit() {
	Circuit parentCircuit;
	vector<Gate> gates;
	// X input
	Gate initOne = initGate(1, NONE, 15, 1, 0);
	// Gate initOne;
	// initOne.gateNum = 1;
	// initOne.operation = NONE;
	// initOne.value = 15;
	// initOne.operand1 = 1;	
	// initOne.operand2 = 0;
	// initOne.gateLine = gateToString(initOne);
	gates.push_back(initOne);

	// Y input
	Gate initTwo = initGate(2, NONE, 51, 2, 0);
	// Gate initTwo;
	// initTwo.gateNum = 2;
	// initTwo.operation = NONE;
	// initTwo.value = 51;
	// initTwo.operand1 = 2;
	// initTwo.operand2 = 0;
	// initTwo.gateLine = gateToString(initTwo);
	gates.push_back(initTwo);

	// Carry In input
	Gate initThree = initGate(3, NONE, 85, 3, 0);
	// Gate initThree;
	// initThree.gateNum = 3;
	// initThree.operation = NONE;
	// initThree.value = 85;
	// initThree.operand1 = 3;
	// initThree.operand2 = 0;
	// initThree.gateLine = gateToString(initThree);
	gates.push_back(initThree);

	parentCircuit.id = 1;
	parentCircuit.output = 0;
	parentCircuit.notCount = 0;

	Circuit parentCircuit = initCircuit(1, 0, 0, gates, string circuit, -1);

	vector<Gate> initialGateList = parentCircuit.gateList;
	for (int i = 0; i < initialGateList.size(); ++i) {
		parentCircuit.circuitry += initialGateList[i].gateLine + "\tValue: " + to_string(initialGateList[i].value) + "\r\n";
	}
	// initialCircuit.circuitry = initOne.gateLine + "\tValue: " + to_string(initOne.value) + "\r\n" +
	// 						initTwo.gateLine + "\tValue: " + to_string(initTwo.value) + "\r\n" +
	// 						initThree.gateLine + "\tValue: " + to_string(initThree.value) + "\r\n";

	return parentCircuit;
}

// void populate(Circuit parent, vector<Circuit> allCircuits) {
// 	vector<Gate> parentGates = parent.gateList;
// 	bitOp ops[4] = {NONE, AND, OR, NOT};
// 	vector<bitOp> bitOpers(&ops[0], &ops[0] + 4);

// 	for (int i = 0; i < parentGates.size(); ++i) {
// 		for (int j = 0; j < bitOpers.size(); ++j) {
// 			Gate nextGate = initGate(parentGates.size(), bitOpers[j], 15, 1, 0);
// 			if (bitOpers[j] == NONE) {			// Going through NONE operation combinations
// 				cout << "NONE code will go here.";
// 				/* code */
// 			}
// 			else if (bitOpers[j] == AND && i != parentGates.size() - 1) {		// Going through AND operation combinations
// 				for (int k = i + 1; k < parentGates.size(); ++k) {
// 					int newResult = bitOperation(j, parentGates[i].value, parentGates[k].value);
// 					Gate nextGate = initGate(parentGates.size(), bitOpers[j], newResult, i, k);
// 					Circuit circ = initCircuit(allCircuits.size(), int outputVal, 0, vector<Gate> gates, string circuit);
// 				}
// 			}
// 			else if (bitOpers[j] == OR && i != parentGates.size() - 1) {		// Going through OR operation combinations
				
// 			}
// 			else if (bitOpers[j] == NOT) {		// Going through NOT operation combinations
				
// 			}
// 			else cerr << "ERROR: 'j' is not what it should be!";
// 		}
// 	}
// }

void printCircuit(Circuit c) {
	cout << c.circuitry;
}

void printFile(Circuit c) {
	ofstream out("output.txt");
	out << c.circuitry;
}

int main(int argc, char const *argv[]) {
	// inputs
	int x = 15;			// x input		
	int y = 51;			// y input
	int ci = 85;		// carry in
	
	// outputs
	int co = 23;		// carry out
	int sum = 105;		// sum

	Circuit initial = parentCircuit();
	vector<Circuit> circuits;
	circuits.push_back(initial);
	printCircuit(initial);

	return 0;
}
