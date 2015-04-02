/********************************************************
Name		: Jennings Fairchild
		    : Tyler Windham
			: Austin Hanus
			: Dey'Ana Wilson
Subject		: CSCE 315-504
Assignment	: Project 3: AI
Compile With: g++-4.7 -std=c++11 -pthread  nonGA1.cpp -lfltk_images

********************************************************/
#include <string>
#include <vector>
#include <bitset>
#include <iostream>
#include <fstream>
#include <time.h>
#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Box.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Chart.H>
#include <math.h>
#include <stdio.h>
#include <unistd.h>
#include<stdlib.h>
#include <limits.h>
#include<thread>
#define BG_COLOR   45
#define TICK_COLOR 50
#define CIRC_COLOR 0

using namespace std;
int numberCircuits =0;

int getCircuitCount(); 
class MyTimer : public Fl_Chart {
    void draw() {
        // COMPUTE NEW COORDS OF LINE
        static long start = time(NULL);
        long tick = time(NULL) - start;
        char secs[80]; sprintf(secs, "%02ld:%02ld", tick/60, tick%60);
		int val = getCircuitCount(); 
		static char val_str[20]; 
		sprintf(val_str, "%d", val); 
		Fl_Chart::add(val, val_str, FL_GREEN); 
		Fl_Chart::draw(); 
		fl_color(FL_BLACK);
		fl_font(FL_HELVETICA,16);
		fl_draw(secs, x()+4, y()+h()-4); 
    }
    static void Timer_CB(void *userdata) {
        MyTimer *o = (MyTimer*)userdata;
        o->redraw();
        Fl::repeat_timeout(.25, Timer_CB, userdata);
    }
public:
    // CONSTRUCTOR
    MyTimer(int X,int Y,int W,int H,const char*L=0) : Fl_Chart(X,Y,W,H,L) {
        type(FL_LINE_CHART); 
		bounds(0,2000); 
        Fl::add_timeout(.25, Timer_CB, (void*)this);
    }
};


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

// Check if a new gate added to a curcuit contains a solution
void checkCorrectOutput(Circuit& currentCirc, Gate toCheck)
{
	bitset<8> OUTPUT1 = 23;						//00010111
	bitset<8> OUTPUT2 = 105;					//01101001

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

// Creates a circuit conataining a new gate combination with the passed bit operation
void newCircuitCombo(Circuit& parent, bitOp oper, vector<Circuit>& newCircuitList, int& numOfCircs, int& tierSize, vector< bitset<8> >& outputList, bool& solutionFound, int& solSwitch)
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
					&& parent.notCount < 2)
				{
					newGateList.push_back(nextGate);
					string newCircString = parent.circuitry + gateToString(nextGate);
					Circuit circ = initCircuit(tierSize + 1, newResult, parent.notCount + 1, newGateList, newCircString, parent);
					checkCorrectOutput(circ, nextGate);
					newCircuitList.push_back(circ);
					++numOfCircs;
					++numberCircuits;
					++tierSize;
					if (isSolutionFound(circ, solSwitch))
					{
						solutionFound = true;
						if (circ.correctFlag1)
						{
							solSwitch = 2;
						}
						else if (circ.correctFlag2)
						{
							solSwitch = 1;
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
						++numberCircuits;
						++tierSize;
						if (isSolutionFound(circ, solSwitch))
						{
							solutionFound = true;
							if (circ.correctFlag1)
							{
								solSwitch = 2;
							}
							else if (circ.correctFlag2)
							{
								solSwitch = 1;
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
vector<Circuit> populateTier(Circuit& parent, int& numOfCircs, int& tierSize, vector< bitset<8> >& outputList, bool& solutionFound, int& solSwitch)
{
	bitOp ops[3] = {AND, OR, NOT};
	vector<bitOp> bitOpers(&ops[0], &ops[0] + 3);
	vector<Circuit> newTier;
	for (int i = 0; i < bitOpers.size(); ++i)
	{
		// Going through AND operation combinations
		if (bitOpers[i] == AND)
		{
			newCircuitCombo(parent, AND, newTier, numOfCircs, tierSize, outputList, solutionFound, solSwitch);
			if (solutionFound) break;
		}
		// Going through OR operation combinations
		else if (bitOpers[i] == OR)
		{
			newCircuitCombo(parent, OR, newTier, numOfCircs, tierSize, outputList, solutionFound, solSwitch);
			if (solutionFound) break;
		}
		// Going through NOT operation combinations
		else if (bitOpers[i] == NOT)
		{
			newCircuitCombo(parent, NOT, newTier, numOfCircs, tierSize, outputList, solutionFound, solSwitch);
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
	bool firstSolTierMade = false;
	int solSwitch = 0;
	int i = 0;

	while (!allSolutionsFound)
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
				partialNewTier = populateTier(parent, numOfCircs, tierSize, outputList, firstSolutionFound, solSwitch);
				entireNewTier.insert(entireNewTier.end(), partialNewTier.begin(), partialNewTier.end());
				if (firstSolutionFound)
				{
					cout << "First Solution Found!" << endl;
					break;
				}
			}
			tiers.push_back(entireNewTier);
			cout << "Tier " << tiers.size() << " made!" << endl;
			cout << "There are now " << numOfCircs << " circuits total." << endl;
			cout << "outputList: " << outputList.size() << endl;
		}
		// Once one solution is found, use that circuit to create next tier
		if (firstSolutionFound && !firstSolTierMade && !secondSolutionFound && !allSolutionsFound)
		{
			partialNewTier.clear();
			entireNewTier.clear();
			outputList.clear();
			currentTier = tiers[tiers.size() - 1];
			tierSize = 0;

			cout << "\nMaking Tier " << tiers.size() + 1 << "..." << endl;
			vector<Circuit> lastTier = tiers[tiers.size() - 1];
			
			Circuit& parentSolution = lastTier[lastTier.size() - 1];
			entireNewTier = populateTier(parentSolution, numOfCircs, tierSize, outputList, secondSolutionFound, solSwitch);

			tiers.push_back(entireNewTier);
			cout << "Tier " << tiers.size() << " made!" << endl;
			cout << "There are now " << numOfCircs << " circuits total." << endl;
			cout << "outputList: " << outputList.size() << endl;
			if (secondSolutionFound)
			{
				cout << "Second Solution Found!" << endl;
			}
			firstSolTierMade = true;
		}

		// Continue creating population as usual until second solution is found
		if (firstSolutionFound && firstSolTierMade && !secondSolutionFound && !allSolutionsFound)
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
				partialNewTier = populateTier(newParent, numOfCircs, tierSize, outputList, secondSolutionFound, solSwitch);
				entireNewTier.insert(entireNewTier.end(), partialNewTier.begin(), partialNewTier.end());
				if (secondSolutionFound)
				{
					cout << "Second Solution Found!" << endl;
					break;
				}
			}
			tiers.push_back(entireNewTier);
			cout << "Tier " << tiers.size() << " made!" << endl;
			cout << "There are now " << numOfCircs << " circuits total." << endl;
			cout << "outputList: " << outputList.size() << endl;
		}
		allSolutionsFound = allSolutionCheck(entireNewTier);
		if (allSolutionsFound)
		{
			cout << "All Solutions Found!" << endl;
			break;
		}
		++i;
	}
	cout << "***** Both Solutions were found *****" << endl;
	Circuit solution = allSolutionCircuit(tiers[tiers.size() - 1]);
	printSolution(solution, tiers.size());
	return tiers;
}

int getCircuitCount(){
	return numberCircuits;
}


int graphicsFunction(){
	 Fl_Double_Window win(1000, 440);
     MyTimer tim(5, 5, win.w()-20, win.h()-20, "Non GA Graph");
     win.show();
	return(Fl::run());
}

int main(int argc, char const *argv[])
{
	Circuit initial = parentCircuit();
	vector< vector<Circuit> > tiers;
	vector< vector<Circuit> > allTiers;
	vector<Circuit> circuits;

	circuits.push_back(initial);
	tiers.push_back(circuits);
	int circuitCount = 1;
	bool firstSolutionFound = false;

	clock_t begin = clock();
	std::thread td1(graphicsFunction);
	sleep(3.2);
	std::thread td2(createPopulation, tiers, std::ref(circuitCount),std::ref(firstSolutionFound));
	//allTiers = createPopulation(tiers, circuitCount, firstSolutionFound);
	td1.join(); 
	td2.join(); 
	clock_t end = clock();
	double elapsedTime = double(end - begin) / CLOCKS_PER_SEC;
	cout << "Elapsed Time : " << elapsedTime << endl;

	printFile(allTiers);
	return 0;
}
