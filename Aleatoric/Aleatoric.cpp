#include<SFML/Audio.hpp>
#include<iostream>
#include<cmath>
using namespace std;

int ParseArguments(int& rootKey, int& sig, float& bpm, float& frac, float& accent, float& volume, int argc, const char* argv[]);
void CalculateFrequency(float freqArray[], int scaleArray[], int RootKey);
void fillMajorArray(int scaleArray[], char key);
void RemoveSubString(string& original, const char* substring);
/*
template<typename T>
void RemoveSubString(basic_string<T>& s,
	const basic_string<T>& p) {
	basic_string<T>::size_type n = p.length();

	for (basic_string<T>::size_type i = s.find(p);
		i != basic_string<T>::npos;
		i = s.find(p))
		s.erase(i, n);
}*/

const int FREQ_SIZE = 10;

int main()
{
	const char* argv[] = {"--bpm[14]","--beats[10]","--invalid[2]" };
	int argc = 3;


	int rootKey = 48;
	int* scaleArray = nullptr;
	int sig = 8;
	float bpm = 90.0f;
	float frac = 0.5f;
	float accent = 5.0f;
	float volume = 8.0f;
	float freqArray[FREQ_SIZE];
	char key = 'c';
	cout << "-_-Before-_-\n";
	cout << "root: " << rootKey << endl;
	cout << "beats: " << sig << endl;
	cout << "bpm: " << bpm << endl;
	cout << "accent: " << accent << endl;
	cout << "volume: " << volume << endl;
	if (ParseArguments(rootKey, sig, bpm, frac, accent, volume, argc, argv) == 1)
	{
		fillMajorArray(scaleArray, key);
		CalculateFrequency(freqArray, scaleArray, 69);
		cout << "-_-After-_-\n";
		cout << "root: " << rootKey << endl;
		cout << "beats: " << sig << endl;
		cout << "bpm: " << bpm << endl;
		cout << "accent: " << accent << endl;
		cout << "volume: " << volume << endl;
	}
	
	if(scaleArray)
		delete[] scaleArray;
}
int ParseArguments(int& rootKey, int& sig, float& bpm, float& frac, float& accent, float& volume, int argc, const char* argv[])
{
	std::string* argvArray = new std::string[argc];
	for (size_t i = 0; i < argc; i++)
		argvArray[i] = argv[i];
	cout << "printing\n";
	for (size_t i = 0; i < argc; i++)
		cout << argvArray[i] << endl;

	for (size_t i = 0; i < argc; i++)
	{
		if (!argvArray[i].find("--root"))
		{
			RemoveSubString(argvArray[i], "--root");
			RemoveSubString(argvArray[i], "[");
			RemoveSubString(argvArray[i], "]");
			try 
			{
				rootKey = stof(argvArray[i]);
			}
			catch (invalid_argument const& error)
			{
				cout << "Invalid parameter value: " << error.what() << endl; 
				return -1;
			}
			cout << "Trimmed root!\n";
		}
		else if (!argvArray[i].find("--beats"))
		{
			RemoveSubString(argvArray[i], "--beats");
			RemoveSubString(argvArray[i], "[");
			RemoveSubString(argvArray[i], "]");
			try
			{
				sig = stoi(argvArray[i]);
			}
			catch (invalid_argument const& error)
			{
				cout << "Invalid parameter value: " << error.what() << endl;
				return -1;
			}
			cout << "Trimmed beats!\n";
		}
		else if (!argvArray[i].find("--bpm"))
		{
			RemoveSubString(argvArray[i], "--bpm");
			RemoveSubString(argvArray[i], "[");
			RemoveSubString(argvArray[i], "]");
			try
			{
				bpm = stof(argvArray[i]);
			}
			catch (invalid_argument const& error)
			{
				cout << "Invalid parameter value: " << error.what() << endl;
				return -1;
			}
			cout << "Trimmed bpm!\n";
		}
		else if (!argvArray[i].find("--accent"))
		{
			RemoveSubString(argvArray[i], "--accent");
			RemoveSubString(argvArray[i], "[");
			RemoveSubString(argvArray[i], "]");
			try
			{
				accent = stof(argvArray[i]);
			}
			catch (invalid_argument const& error)
			{
				cout << "Invalid parameter value: " << error.what() << endl;
				return -1;
			}
			cout << "Trimmed accent!\n";
		}
		else if (!argvArray[i].find("--volume"))
		{
			RemoveSubString(argvArray[i], "--volume");
			RemoveSubString(argvArray[i], "[");
			RemoveSubString(argvArray[i], "]");
			try
			{
				volume = stof(argvArray[i]);
			}
			catch (invalid_argument const& error)
			{
				cout << "Invalid parameter value: " << error.what() << endl;
				return -1;
			}
			cout << "Trimmed volume!\n";
		}
		else
		{
			cout << "ERROR: invalid argument: " << argv[i] << endl;
			cout << "Closing program...\n";
			return -1;
		}
	}
	return 1;
}

//Calculate the freqency of the random keys that can be played
void CalculateFrequency(float freqArray[], int scaleArray[], int rootKey)
{
	for (size_t i = 0; i < FREQ_SIZE; i++)
		freqArray[i] = 440 * static_cast<float>(pow(2.0, ((rootKey + static_cast<float>(i)) - 69) / 12));
		
	for (size_t i = 0; i < FREQ_SIZE; i++)
		cout << freqArray[i] << "\n";
}


void fillMajorArray(int scaleArray[], char key)
{
	switch (key)
	{
	case('a'):
			cout << "case a\n";
			scaleArray = new int[7]{ 0,2,1,2,2,1,2 };
			break;
	case('b'):
				cout << "case b\n";
			scaleArray = new int[7]{ 0,1,2,2,1,1,2 };
			break;
	case('c'):
			cout << "case c\n";
			scaleArray = new int[7]{ 0,2,2,1,2,2,2 };
			break;
	case('d'):
			cout << "case d\n";
			scaleArray = new int[7]{ 0,2,1,2,2,2,1 };
			break;
	case('e'):
			cout << "case e\n";
			scaleArray = new int[7]{ 0,1,2,2,2,1,2 };
			break;
	case('f'):
			cout << "case f\n";
			scaleArray = new int[7]{ 0,2,2,2,1,2,2 };
		break;
	case('g'):
			cout << "case g\n";
			scaleArray = new int[7]{ 0,2,2,1,2,2,1 };
			break;
		default:
			cout << "ERROR: unable to generate major array\n";
			break;
	}
}

//Removes a substring from the string
void RemoveSubString(string& original, const char* substring)
{
	std::size_t pos = original.find(substring);

	if (pos != string::npos)
		original.erase(pos, strlen(substring));
	else
		cout << "ERROR: unable to shorten command line argument\n";
}
