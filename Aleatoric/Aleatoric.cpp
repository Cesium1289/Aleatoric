#include<SFML/Audio.hpp>
#include<iostream>
#include<cmath>
using namespace std;

int ParseArguments(int& rootKey, int& sig, float& bpm, float& frac, float& accent, float& volume, int argc, const char* argv[]);
void CalculateFrequency(float freqArray[], vector<int>& scaleArray, int RootKey);
void fillMajorArray(vector<int>& scaleArray, char key);
void RemoveSubString(string& original, const char* substring);
void GenerateSineWaves(int freqArray[], sf::SoundBuffer soundBuffer[], float amplitude, float bpm, float frac);
void SetAmplitudes(float accent, float volume, float& squareAmp, float& sineAmp);
void RampSamples(vector<sf::Int16>& sample, float frac);
const int NUM_SAMPLES = 48000;
const int FREQ_SIZE = 7;

int main()
{
	const char* argv[] = {"--bpm[14]","--beats[10]" };
	int argc =2;
	float squareWaveAmp;
	float sineWaveAmp ;
	vector<int> scaleArray;
	int rootKey = 76;
	int sig = 8;
	float bpm = 90.0f;
	float frac = 0.5f;
	float accent = 8.0f;
	float volume = 8.0f;
	float freqArray[FREQ_SIZE];
	
	char key = 'e';
	
	SetAmplitudes(accent, volume, squareWaveAmp, sineWaveAmp);

	cout << "\nSquare: " << squareWaveAmp << endl;
	cout << "Sine: " << sineWaveAmp << endl;
	ParseArguments(rootKey, sig, bpm, frac, accent, volume, argc, argv);
	fillMajorArray(scaleArray, key);
	CalculateFrequency(freqArray, scaleArray, rootKey);

	/*
	cout << "-_-Before-_-\n";
	cout << "root: " << rootKey << endl;
	cout << "beats: " << sig << endl;
	cout << "bpm: " << bpm << endl;
	cout << "accent: " << accent << endl;
	cout << "volume: " << volume << endl;

	if (ParseArguments(rootKey, sig, bpm, frac, accent, volume, argc, argv) )
	{
		fillMajorArray(scaleArray, key);
		CalculateFrequency(freqArray, scaleArray, rootKey);
		cout << "-_-After-_-\n";
		cout << "root: " << rootKey << endl;
		cout << "beats: " << sig << endl;
		cout << "bpm: " << bpm << endl;
		cout << "accent: " << accent << endl;
		cout << "volume: " << volume << endl;
	}*/

}

//Calculate the freqency of the random keys that can be played
void CalculateFrequency(float freqArray[], vector<int>& scaleArray, int rootKey)
{
	int temp = 0;
	for (size_t i = 0; i < FREQ_SIZE; i++)
	{
		freqArray[i] = 440 * static_cast<float>(pow(2.0, ((rootKey + static_cast<float>(scaleArray.at(i) + temp) - 69) / 12)));
		temp += scaleArray[i];
	}
		
	for (size_t i = 0; i < FREQ_SIZE; i++)
		cout << freqArray[i] << "\n";
}

//Removes a substring from the string
void RemoveSubString(string& original, const char* substring)
{
	std::size_t pos = original.find(substring);

	if (pos != string::npos)
		original.erase(pos, strlen(substring));
	else
		cout << "ERROR: unable to parse command line argument\n";
}

//Generates the sinewave for keys of a given scale
void GenerateSineWaves(int freqArray[], sf::SoundBuffer soundSamples[], float amplitude, float bpm, float frac)
{
	//loop through all possible keys in a step for a given root
	for (size_t i = 0; i < NUM_SAMPLES; i++)
	{
		vector<sf::Int16> buffer;

		//generate the sinewave for a given key
		for (size_t j = 0; j < NUM_SAMPLES; j++)
			//buffer.push_back(amplitude * sin(2 * 3.14 * freqArray[i] * ((60 / bpm) / NUM_SAMPLES)));

		//ramp the sample 
		RampSamples(buffer, frac);

		//add ramped sample to the sound samples
		//soundSamples[i].loadFromSamples(&buffer[0], buffer.size(), 60 / bpm, NUM_SAMPLES);
	}
}

void SetAmplitudes(float accent, float volume, float& squareAmp, float& sineAmp)
{
	squareAmp =  pow(10,(-6 * (10 - accent) / 20));
	sineAmp = pow(10, (-6 * (10 - volume) / 20));
}

//Calculate the beat time for the attack and release time for the note envelope
void RampSamples(vector<sf::Int16>& sample, float frac)
{
	//number of samples that take place in the ramp time
	int rampSamples = NUM_SAMPLES * frac;
	float dampen;
	float temp;

	//apply the left half of a triangle window to the ramp samples
	for (size_t i = 0; i < rampSamples; i++)	
	{ 
		//temp = (static_cast<float>(i) - (rampSamples / static_cast<float>(2))) / (rampSamples / static_cast<float>(2));
		//dampen = 1 - abs(temp);
		//sample.at(i) *= dampen;
	}

	//apply the right half of a triangle window to the ramp samples
	for (size_t i = rampSamples; i < NUM_SAMPLES; i++)
	{
		//temp = (static_cast<float>(i) - (rampSamples / static_cast<float>(2))) / (rampSamples / static_cast<float>(2));
		//dampen = 1 - abs(temp);
		//sample.at(i) *= dampen;
	}
}


void fillMajorArray(vector<int>& scaleArray, char key)
{
	switch (key)
	{
	case('a'):
		cout << "case a\n";
		//scaleArray = new int[7]{ 0,2,1,2,2,1,2 };
		scaleArray.insert(scaleArray.end(), { 0,2,1,2,2,1,2 });
		break;
	case('b'):
		cout << "case b\n";
		scaleArray.insert(scaleArray.end(), { 0,1,2,2,1,1,2 });
		//scaleArray = new int[7]{ 0,1,2,2,1,1,2 };
		break;
	case('c'):
		cout << "case c\n";
		scaleArray.insert(scaleArray.end(), { 0,2,2,1,2,2,2 });
		//scaleArray = new int[7]{ 0,2,2,1,2,2,2 };
		break;
	case('d'):
		cout << "case d\n";
		scaleArray.insert(scaleArray.end(), { 0,2,1,2,2,2,1 });
		//scaleArray = new int[7]{ 0,2,1,2,2,2,1 };
		break;
	case('e'):
		cout << "case e\n";
		scaleArray.insert(scaleArray.end(), { 0,1,2,2,2,1,2 });
		//scaleArray = new int[7]{ 0,1,2,2,2,1,2 };
		break;
	case('f'):
		cout << "case f\n";
		scaleArray.insert(scaleArray.end(), { 0,2,2,2,1,2,2 });
		//scaleArray = new int[7]{ 0,2,2,2,1,2,2 };
		break;
	case('g'):
		cout << "case g\n";
		scaleArray.insert(scaleArray.end(), { 0,2,2,1,2,2,1 });
		//scaleArray = new int[7]{ 0,2,2,1,2,2,1 };
		break;
	default:
		cout << "ERROR: unable to generate major array\n";
		break;
	}
}

//Parses the command line arguments that are given to run the program. This function takes the
int ParseArguments(int& rootKey, int& sig, float& bpm, float& frac, float& accent, float& volume, int argc, const char* argv[])
{
	string* argvArray = new string[argc];
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
				rootKey = stoi(argvArray[i]);
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