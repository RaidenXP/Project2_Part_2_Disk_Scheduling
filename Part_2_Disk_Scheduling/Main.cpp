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

	/*vector<int>* processes = new vector<int>{70, 115, 130, 110, 80, 20, 25};
	int previousService = 90;
	int firstService = 120;*/

	/*vector<int>* processes = new vector<int>{2069, 1212, 2296, 2800, 544, 1618, 356, 1523, 4965, 3681};
	int previousService = 1805;
	int firstService = 2150;*/

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

	if (option == 1)
	{
		cout << "Please input the current servicing cylinder: " << endl;
		cin >> firstService;
		cout << "Please input the previous servicing cylinder before the current one: " << endl;
		cin >> previousService;

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
	int direction = 0;
	int totalHeadMov = 0;
	int headChangeCount = 0;
	int length = static_cast<int>(processes->size());

	if (firstService - previousService >= 0)
		direction = 1;
	else
		direction = -1;

	int previousDirect = direction;

	for (int i = 0; i < length; ++i)
	{
		if (i == 0) 
		{
			totalHeadMov += abs((processes->operator[](i) - firstService));
			
			if (processes->operator[](i) - firstService >= 0)
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
			totalHeadMov += abs((processes->operator[](i) - processes->operator[](i - 1)));

			if (processes->operator[](i) - processes->operator[](i - 1) >= 0)
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

void sSTF(int previousService, int firstService, vector<int>* processes)
{
	int direction = 0;
	int totalHeadMov = 0;
	int headChangeCount = 0;

	vector<int> v1 = *processes;
	vector<int> v2 = {};

	if (firstService - previousService >= 0)
		direction = 1;
	else
		direction = -1;

	int previousDirect = direction;

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
	int direction = 0;
	int totalHeadMov = 0;
	int headChangeCount = 0;

	vector<int> lowerHalf = {};
	vector<int> upperHalf = {};

	if (firstService - previousService >= 0)
		direction = 1;
	else
		direction = -1;

	int previousDirect = direction;

	for (int i = 0; i < static_cast<int>(processes->size()); ++i)
	{
		if (processes->operator[](i) >= 0 && processes->operator[](i) <= firstService)
		{
			lowerHalf.push_back(processes->operator[](i));
		}
	}

	for (int i = 0; i < static_cast<int>(processes->size()); ++i)
	{
		if (processes->operator[](i) < 5000 && processes->operator[](i) >= firstService)
		{
			upperHalf.push_back(processes->operator[](i));
		}
	}

	sort(upperHalf.begin(), upperHalf.end());
	sort(lowerHalf.begin(), lowerHalf.end());
	reverse(lowerHalf.begin(), lowerHalf.end());

	if (direction == -1)
	{
		int length = static_cast<int>(lowerHalf.size());

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

		totalHeadMov += abs((0 - lowerHalf[length - 1]));

		length = static_cast<int>(upperHalf.size());
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
	else
	{
		int length = static_cast<int>(upperHalf.size());
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

		totalHeadMov += abs((4999 - upperHalf[length - 1]));
		
		length = static_cast<int>(lowerHalf.size());
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

	cout << endl;
	cout << endl;
	cout << "The total head movement: " << totalHeadMov << endl;
	cout << "The total changed direction: " << headChangeCount << endl;
}

void cScan(int previousService, int firstService, vector<int>* processes)
{
	int direction = 0;
	int totalHeadMov = 0;
	int headChangeCount = 0;

	vector<int> lowerHalf = {};
	vector<int> upperHalf = {};

	if (firstService - previousService >= 0)
		direction = 1;
	else
		direction = -1;

	int previousDirect = direction;

	for (int i = 0; i < static_cast<int>(processes->size()); ++i)
	{
		if (processes->operator[](i) >= 0 && processes->operator[](i) <= firstService)
		{
			lowerHalf.push_back(processes->operator[](i));
		}
	}

	for (int i = 0; i < static_cast<int>(processes->size()); ++i)
	{
		if (processes->operator[](i) < 5000 && processes->operator[](i) >= firstService)
		{
			upperHalf.push_back(processes->operator[](i));
		}
	}

	sort(upperHalf.begin(), upperHalf.end());
	sort(lowerHalf.begin(), lowerHalf.end());

	int length = static_cast<int>(upperHalf.size());
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

	totalHeadMov += abs((4999 - upperHalf[length - 1]));
	totalHeadMov += abs((0 - 4999));

	length = static_cast<int>(lowerHalf.size());
	++headChangeCount;
	++headChangeCount;

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

	cout << endl;
	cout << endl;
	cout << "The total head movement: " << totalHeadMov << endl;
	cout << "The total changed direction: " << headChangeCount << endl;
}