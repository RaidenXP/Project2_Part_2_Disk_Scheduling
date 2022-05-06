/*
	Tran, Nathan

	April 30, 2022

	CS 4310.02
	Task Two Disk-Scheduling Algorithm
*/

#include <iostream>
#include <vector>
#include <stdlib.h>
#include <algorithm>
#include <fstream>
#include <time.h>

using namespace std;

void fCFS(int previousService, int firstService, vector<int>* processes);

void sSTF(int previousService, int firstService, vector<int>* processes);

void scan(int previousService, int firstService, vector<int>* processes);

void cScan(int previousService, int firstService, vector<int>* processes);

int main()
{
	//initilize random seed
	srand(time(NULL));

	//just testing code don't worry about this
	/*vector<int>* processes = new vector<int>{70, 115, 130, 110, 80, 20, 25};
	int previousService = 90;
	int firstService = 120;*/

	//just testing code don't worry about this
	/*vector<int>* processes = new vector<int>{2069, 1212, 2296, 2800, 544, 1618, 356, 1523, 4965, 3681};
	int previousService = 1805;
	int firstService = 2150;*/

	//initialization of values
	vector<int>* processes = new vector<int>{};
	int previousService = 0;
	int firstService = 0;

	cout << "Welcome to the simple simulation for disk-scheduling!" << endl << endl;
	cout << "Any values you put for the cylinders must be in range of 0 - 4999" << endl << endl;
	cout << "Would you like to use 1000 random entries or use an input.txt: " << endl;

	int option = 0;
	bool flag = false;

	//Validates to make sure the input is valid
	do
	{
		cout << "Please select an option by typing in the number that is next to it to begin the process for the simulation." << endl;
		cout << "1) RandomNums" << endl;
		cout << "2) InputFile" << endl;
		cin >> option;

		if (option == 1)
		{
			flag = true;
		}
		else if (option == 2)
		{
			flag = true;
		}
		else
		{
			cerr << "Invalid choice please try again." << endl;
		}

	} while (!flag);

	//chooose either a random list of 1000 cylinders or a input.txt
	if (option == 1)
	{
		cout << "Please input the current servicing cylinder: " << endl;
		cin >> firstService;
		cout << "Please input the previous servicing cylinder before the current one: " << endl;
		cin >> previousService;

		cout << endl;
		cout << "This will help us know the direction the head is already going." << endl;
		cout << endl;

		int items = 1000;
		for (int i = 0; i < items; ++i)
		{
			processes->push_back(rand() % 5000);
		}
	}
	else if (option == 2)
	{
		cout << "Please input the current servicing cylinder: " << endl;
		cin >> firstService;
		cout << "Please input the previous servicing cylinder before the current one: " << endl;
		cin >> previousService;

		cout << endl;
		cout << "This will help us know the direction the head is already going." << endl;
		cout << endl;

		string fileName;
		cout << "Please input a file name and make sure its in the same directory: " << endl;
		cin >> fileName;

		ifstream input;
		input.open(fileName);

		//validates the file
		if (!input)
		{
			cerr << "The file was not found for some reason" << endl;
			cout << endl;
			system("Pause");
			return -1;
		}

		//reads our file
		//please make sure the file follow the given format in report and the read me
		int cylinder;
		while (input >> cylinder)
		{
			processes->push_back(cylinder);
		}

		input.close();

		cout << endl;
	}

	/*for (const auto& elem : *processes)
	{
		cout << elem << " ";
	}*/

	cout << endl;
	cout << "FCFS: ";
	fCFS(previousService, firstService, processes);

	cout << endl;
	cout << "SSTF: ";
	sSTF(previousService, firstService, processes);
	
	cout << endl;
	cout << "SCAN: ";
	scan(previousService, firstService, processes);
	
	cout << endl;
	cout << "CSCAN: ";
	cScan(previousService, firstService, processes);

	cout << endl;

	delete processes;
	processes = nullptr;

	cout << endl;
	system("Pause");
	return 0;
}

void fCFS(int previousService, int firstService, vector<int>* processes)
{
	//initialization of variables
	int direction = 0;
	int totalHeadMov = 0;
	int headChangeCount = 0;
	int length = static_cast<int>(processes->size());

	//determine the direction
	if (firstService - previousService >= 0)
		direction = 1;
	else
		direction = -1;

	int previousDirect = direction;

	//go through processes
	for (int i = 0; i < length; ++i)
	{
		//if its the first value then we will use our initial cylinder
		//otherwise we will continue down the list and use consecutive values
		if (i == 0) 
		{
			totalHeadMov += abs((processes->operator[](i) - firstService));
			
			//check direction
			if (processes->operator[](i) - firstService >= 0)
				direction = 1;
			else
				direction = -1;

			//check to see if direction changed
			if (direction != previousDirect)
			{
				headChangeCount++;
				previousDirect = direction;
			}
		}
		else
		{
			totalHeadMov += abs((processes->operator[](i) - processes->operator[](i - 1)));

			//check direction
			if (processes->operator[](i) - processes->operator[](i - 1) >= 0)
				direction = 1;
			else
				direction = -1;

			//check to see if direction changed
			if (direction != previousDirect)
			{
				headChangeCount++;
				previousDirect = direction;
			}
		}
	}
	
	cout << endl;
	cout << endl;
	cout << "The total head movement: " << totalHeadMov << endl;
	cout << "The total changed direction: " << headChangeCount << endl;
}

void sSTF(int previousService, int firstService, vector<int>* processes)
{
	//initialization of the variables
	int direction = 0;
	int totalHeadMov = 0;
	int headChangeCount = 0;

	vector<int> v1 = *processes;
	
	//will be our list that will add the next closest process
	vector<int> v2 = {};

	//get our first closest cylinder to our initial cylinder
	int bestMin = abs(firstService - v1[0]);
	int bestInd = 0;
	for (int i = 1; i < static_cast<int>(v1.size()); ++i)
	{
		if (abs(firstService - v1[i]) < bestMin)
		{
			bestMin = abs(firstService - v1[i]);
			bestInd = i;
		}
	}

	v2.push_back(v1[bestInd]);
	v1.erase(v1.begin() + bestInd);

	//check for the direction
	if (firstService - previousService >= 0)
		direction = 1;
	else
		direction = -1;

	int previousDirect = direction;

	//order our v2 will the closest cylinder as it travels through the queue
	int j = 0;
	while (!(v1.empty()))
	{
		bestMin = abs(v2[j] - v1[0]);
		bestInd = 0;
		for (int i = 1; i < static_cast<int>(v1.size()); ++i)
		{
			if (abs(v2[j] - v1[i]) < bestMin)
			{
				bestMin = abs(v2[j] - v1[i]);
				bestInd = i;
			}
		}

		v2.push_back(v1[bestInd]);
		v1.erase(v1.begin() + bestInd);

		++j;
	}

	int length = static_cast<int>(v2.size());

	//use the exact same process in FCFS to check for direction change and head movement
	for (int i = 0; i < length; ++i)
	{
		if (i == 0)
		{
			totalHeadMov += abs((v2[i] - firstService));

			if (v2[i] - firstService >= 0)
				direction = 1;
			else
				direction = -1;

			if (direction != previousDirect)
			{
				headChangeCount++;
				previousDirect = direction;
			}
		}
		else
		{
			totalHeadMov += abs((v2[i] - v2[i - 1]));

			if (v2[i] - v2[i - 1] >= 0)
				direction = 1;
			else
				direction = -1;

			if (direction != previousDirect)
			{
				headChangeCount++;
				previousDirect = direction;
			}
		}
	}

	cout << endl;
	cout << endl;
	cout << "The total head movement: " << totalHeadMov << endl;
	cout << "The total changed direction: " << headChangeCount << endl;
}

void scan(int previousService, int firstService, vector<int>* processes)
{
	//initialization of variables
	int direction = 0;
	int totalHeadMov = 0;
	int headChangeCount = 0;

	//will use to divide the processes because we will finish processes in one direction then
	//head to the next
	vector<int> lowerHalf = {};
	vector<int> upperHalf = {};

	//divide the processes in half
	for (int i = 0; i < static_cast<int>(processes->size()); ++i)
	{
		if (processes->operator[](i) >= 0 && processes->operator[](i) <= firstService)
		{
			lowerHalf.push_back(processes->operator[](i));
		}
		else if (processes->operator[](i) < 5000 && processes->operator[](i) > firstService)
		{
			upperHalf.push_back(processes->operator[](i));
		}
	}

	//sort the lists and reverse the lower half because we want the largest first
	sort(upperHalf.begin(), upperHalf.end());
	sort(lowerHalf.begin(), lowerHalf.end());
	reverse(lowerHalf.begin(), lowerHalf.end());

	//check the initial direction
	if (firstService - previousService >= 0)
		direction = 1;
	else
		direction = -1;

	int previousDirect = direction;

	//if we are heading the negative direction
	//or are we heading in the positive direction
	if (direction == -1)
	{
		int length = static_cast<int>(lowerHalf.size());

		//check if the lower half is empty or not
		if (length != 0)
		{
			for (int i = 0; i < length; ++i)
			{
				if (i == 0)
				{
					totalHeadMov += abs((lowerHalf[i] - firstService));
				}
				else
				{
					totalHeadMov += abs((lowerHalf[i] - lowerHalf[i - 1]));
				}
			}
		}

		length = static_cast<int>(upperHalf.size());
		
		//check for the upper half because we were decreasing add head change count by 1
		//then do the upper area
		if (length != 0)
		{
			//if there was no lower half
			//otherwise start where the lower half left off
			if (static_cast<int>(lowerHalf.size()) == 0)
			{
				++headChangeCount;

				for (int i = 0; i < length; ++i)
				{
					if (i == 0)
					{
						totalHeadMov += abs((upperHalf[i] - firstService));
					}
					else
					{
						totalHeadMov += abs((upperHalf[i] - upperHalf[i - 1]));
					}
				}
			}
			else
			{
				totalHeadMov += abs((0 - lowerHalf[static_cast<int>(lowerHalf.size()) - 1]));
				++headChangeCount;

				for (int i = 0; i < length; ++i)
				{
					if (i == 0)
					{
						totalHeadMov += abs((upperHalf[i] - 0));
					}
					else
					{
						totalHeadMov += abs((upperHalf[i] - upperHalf[i - 1]));
					}
				}
			}
		}
	}
	else
	{
		int length = static_cast<int>(upperHalf.size());
		
		//check the upper half
		if (length != 0)
		{
			for (int i = 0; i < length; ++i)
			{
				if (i == 0)
				{
					totalHeadMov += abs((upperHalf[i] - firstService));
				}
				else
				{
					totalHeadMov += abs((upperHalf[i] - upperHalf[i - 1]));
				}
			}
		}
		
		length = static_cast<int>(lowerHalf.size());
		
		if (length != 0)
		{
			//if the upperhalf did not exist then change direction and start from initial cylinder
			//otherwise start where the upper half left off
			if (static_cast<int>(upperHalf.size()) == 0)
			{
				++headChangeCount;

				for (int i = 0; i < length; ++i)
				{
					if (i == 0)
					{
						totalHeadMov += abs((lowerHalf[i] - firstService));
					}
					else
					{
						totalHeadMov += abs((lowerHalf[i] - lowerHalf[i - 1]));
					}
				}
			}
			else
			{
				totalHeadMov += abs((4999 - upperHalf[static_cast<int>(upperHalf.size()) - 1]));
				++headChangeCount;

				for (int i = 0; i < length; ++i)
				{
					if (i == 0)
					{
						totalHeadMov += abs((lowerHalf[i] - 4999));
					}
					else
					{
						totalHeadMov += abs((lowerHalf[i] - lowerHalf[i - 1]));
					}
				}
			}
		}
	}

	cout << endl;
	cout << endl;
	cout << "The total head movement: " << totalHeadMov << endl;
	cout << "The total changed direction: " << headChangeCount << endl;
}

void cScan(int previousService, int firstService, vector<int>* processes)
{
	//initialization of variables
	int direction = 0;
	int totalHeadMov = 0;
	int headChangeCount = 0;

	vector<int> lowerHalf = {};
	vector<int> upperHalf = {};

	//check the initial direction
	if (firstService - previousService >= 0)
		direction = 1;
	else
		direction = -1;

	int previousDirect = direction;

	//separate the halves
	for (int i = 0; i < static_cast<int>(processes->size()); ++i)
	{
		if (processes->operator[](i) >= 0 && processes->operator[](i) <= firstService)
		{
			lowerHalf.push_back(processes->operator[](i));
		} 
		else if (processes->operator[](i) < 5000 && processes->operator[](i) > firstService)
		{
			upperHalf.push_back(processes->operator[](i));
		}
	}

	//sort the halves
	sort(upperHalf.begin(), upperHalf.end());
	sort(lowerHalf.begin(), lowerHalf.end());

	//if we go in the positive direction
	//otherwise we go to the negative direction
	if (direction == 1)
	{
		int length = static_cast<int>(upperHalf.size());

		//check to see if the upperhalf is empty
		if (length != 0)
		{
			for (int i = 0; i < length; ++i)
			{
				if (i == 0)
				{
					totalHeadMov += abs((upperHalf[i] - firstService));
				}
				else
				{
					totalHeadMov += abs((upperHalf[i] - upperHalf[i - 1]));
				}
			}
		}

		length = static_cast<int>(lowerHalf.size());

		if (length != 0)
		{
			//if the upperhalf had nothing in it then send to end of cylinders
			//then go back to 0 and change direction twice cause we go back then forward
			//otherwise if there is a upper half we do 4999 - the last upper half value
			//then go back to 0 and change direction twice cause we go back then forward
			if (static_cast<int>(upperHalf.size()) == 0)
			{
				totalHeadMov += abs((4999 - firstService));
				totalHeadMov += abs((0 - 4999));
				headChangeCount += 2;

				for (int i = 0; i < length; ++i)
				{
					if (i == 0)
					{
						totalHeadMov += abs((lowerHalf[i] - 0));
					}
					else
					{
						totalHeadMov += abs((lowerHalf[i] - lowerHalf[i - 1]));
					}
				}
			}
			else
			{
				totalHeadMov += abs((4999 - upperHalf[static_cast<int>(upperHalf.size()) - 1]));
				totalHeadMov += abs((0 - 4999));
				headChangeCount += 2;

				for (int i = 0; i < length; ++i)
				{
					if (i == 0)
					{
						totalHeadMov += abs((lowerHalf[i] - 0));
					}
					else
					{
						totalHeadMov += abs((lowerHalf[i] - lowerHalf[i - 1]));
					}
				}
			}
		}
	}
	else
	{
		//we have to reverse here because we need the upper part of the lower
		//half first
		reverse(lowerHalf.begin(), lowerHalf.end());

		int length = static_cast<int>(lowerHalf.size());

		//check for the lower half size
		if (length != 0)
		{
			for (int i = 0; i < length; ++i)
			{
				if (i == 0)
				{
					totalHeadMov += abs((lowerHalf[i] - firstService));
				}
				else
				{
					totalHeadMov += abs((lowerHalf[i] - lowerHalf[i - 1]));
				}
			}
		}

		length = static_cast<int>(upperHalf.size());

		if (length != 0)
		{
			//if the lower half is empty then we go from the first service to 0
			//then go back to 4999 and change back again
			//reverse the upper half because we want the highest value first
			//else
			//we go to 0 from the lowest part of the lower half
			//then go back to 4999 and change back to decreasing
			//reverse the upper half because we want the highest value first
			if (static_cast<int>(lowerHalf.size()) == 0)
			{
				totalHeadMov += abs((0 - firstService));
				totalHeadMov += abs((4999 - 0));
				headChangeCount += 2;

				reverse(upperHalf.begin(), upperHalf.end());

				for (int i = 0; i < length; ++i)
				{
					if (i == 0)
					{
						totalHeadMov += abs((upperHalf[i] - 4999));
					}
					else
					{
						totalHeadMov += abs((upperHalf[i] - upperHalf[i - 1]));
					}
				}
			}
			else
			{
				totalHeadMov += abs((0 - lowerHalf[static_cast<int>(lowerHalf.size()) - 1]));
				totalHeadMov += abs((4999 - 0));
				headChangeCount += 2;

				reverse(upperHalf.begin(), upperHalf.end());

				for (int i = 0; i < length; ++i)
				{
					if (i == 0)
					{
						totalHeadMov += abs((upperHalf[i] - 4999));
					}
					else
					{
						totalHeadMov += abs((upperHalf[i] - upperHalf[i - 1]));
					}
				}
			}
		}
	}

	cout << endl;
	cout << endl;
	cout << "The total head movement: " << totalHeadMov << endl;
	cout << "The total changed direction: " << headChangeCount << endl;
}