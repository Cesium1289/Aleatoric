//NAME:Carson Hansen
//DATE: 5/27/2022
#include<SFML/Audio.hpp>
#include<iostream>
#include<cmath>
#include<cstdlib>
using namespace std;

bool ParseArguments(int& rootKey, int& sig, float& bpm, float& frac, float& accent, float& volume, int argc, const char* argv[]);
void CalculateFrequency(float freqArray[], vector<int>& scaleArray, int RootKey);
void fillMajorArray(vector<int>& scaleArray, char key);
void RemoveSubString(string& original, const char* substring);
void GenerateWaves(float freqArray[], sf::SoundBuffer SineWaves[], sf::SoundBuffer& squareWave, float sineWaveAmp, float bpm, float frac, float squareWaveAmp);
void SetAmplitudes(float accent, float volume, float& squareAmp, float& sineAmp);
void RampSamples(vector<sf::Int16>& sample, float frac);
int CheckKeyValue(int rootKey, char& character);
bool ValidArguments(int& rootKey, int& sig, float& bpm, float& frac, float& accent, float& volume, char& key);
const int NUM_SAMPLES = 48000;
const int NUM_KEYS = 7;

int main()
{
	sf::SoundBuffer sineSamples[NUM_KEYS]; 
	sf::SoundBuffer squareSample;
	sf::Sound sound;
	const char* argv[] = { "--bpm[180]","--beats[3]","--root[115]","--volume[8.3]","--accent[2]" };
	int argc =0;
	float squareWaveAmp;
	float sineWaveAmp;
	vector<int> scaleArray;
	int rootKey = 48;
	int sig = 8;
	float bpm = 90.0f;
	float frac = 0.5f;
	float accent = 5.0f;
	float volume = 8.0f;
	float freqArray[NUM_KEYS];
	char key = 'c';

	if (ParseArguments(rootKey, sig, bpm, frac, accent, volume, argc, argv) && ValidArguments(rootKey, sig, bpm, frac, accent, volume,key))
	{
		SetAmplitudes(accent, volume, squareWaveAmp, sineWaveAmp);
		fillMajorArray(scaleArray, key);
		CalculateFrequency(freqArray, scaleArray, rootKey);
		GenerateWaves(freqArray, sineSamples, squareSample, sineWaveAmp, bpm, frac, squareWaveAmp);

		cout << "\n-_-_-Values used-_-_-\n";
		cout << "Root: " << rootKey << endl;
		cout << "Beats per measure: " << sig << endl;
		cout << "BPM: " << bpm << endl;
		cout << "Ramp: " << frac << endl;
		cout << "Accent: " << accent << endl;
		cout << "Volume: " << volume << endl;
		int i = 1;

		while (1)
		{
			//check if sound is playing
			while (sound.getStatus() == sf::Sound::Playing)
				sf::sleep(sf::milliseconds(100));

			//check if square wave should be played
			if (i == sig)
			{
				sound.setBuffer(squareSample);
				sound.play();
				i = 0;
			}
			else
			{
				sound.setBuffer(sineSamples[rand() % NUM_KEYS]);
				sound.play();
			}
			++i;
		}
	}
}

bool ValidArguments(int& rootKey, int& sig, float& bpm, float& frac, float& accent, float& volume,char& key)
{
	if (!CheckKeyValue(rootKey, key))
		return false;
		
	else if (sig < 1)
	{
		cout << "Enter a beats value greater than 1!\n";
		return false;
	}
	else if (bpm < 0.1f)
	{
		cout << "Enter a bpm value greater than 0!\n";
		return false;
	}
	else if (frac < 0.0f || frac > 0.5f)
	{
		cout << "Enter a ramp value that is between 0.0 - 0.5!\n";
		return false;
	}
	else if (volume < 0.0f || volume > 10.0f)
	{
		cout << "Enter a volume value between 0.0 - 10.0!\n";
		return false;
	}
	else if (accent < 0.0f || accent > 10.0f)
	{
		cout << "Enter a accent value between 0.0 - 10.0!\n";
		return false;
	}
	return true;
}

//Calculate the freqency of the random keys that can be played
void CalculateFrequency(float freqArray[], vector<int>& scaleArray, int rootKey)
{
	int scaleOffset = 0;

	for (size_t i = 0; i < NUM_KEYS + 1; i++)
	{
		freqArray[i] = 440 * static_cast<float>(pow(2.0, ((rootKey + static_cast<float>(scaleArray.at(i) + scaleOffset) - 69) / 12)));
		scaleOffset += scaleArray[i];
	}
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

//Generates the square and sine waves for keys of a given scale
void GenerateWaves(float freqArray[], sf::SoundBuffer sineSamples[], sf::SoundBuffer& squareSample, float sineWaveAmp, float bpm, float frac, float squareWaveAmp)
{
	vector<sf::Int16> buffer;

	//generate square wave
	for (size_t j = 0; j < NUM_SAMPLES; j++)
	{
		if (sin(2 * 3.14 * freqArray[0] * (static_cast<float>(j) / NUM_SAMPLES)) > 0)
			buffer.push_back(10000 * squareWaveAmp);
		else
			buffer.push_back(-10000 * squareWaveAmp);
	}
	
	//ramp square wave sample
	RampSamples(buffer, frac);

	//place sample into square wave buffer
	squareSample.loadFromSamples(&buffer[0], buffer.size(), 1, NUM_SAMPLES* (bpm / 60));	

	//generate sine waves
	for (size_t i = 1; i < NUM_KEYS+1; i++)
	{
		buffer.clear();

		for (size_t j = 0; j < NUM_SAMPLES; j++)
			buffer.push_back(10000 * sineWaveAmp * sin(2 * 3.14 * freqArray[i] * (static_cast<float>(j) / NUM_SAMPLES)));
			
		//ramp the sample 
		RampSamples(buffer, frac);

		//add sine wave to wave buffer
		sineSamples[i-1].loadFromSamples(&buffer[0], buffer.size(), 1, NUM_SAMPLES * (bpm / 60));	
	}
}

//Set the amplitudes for the square and sine waves based on volume levels
void SetAmplitudes(float accent, float volume, float& squareAmp, float& sineAmp)
{
	squareAmp =  pow(10,(-6 * (10 - accent) / 20));
	sineAmp = pow(10, (-6 * (10 - volume) / 20));
}

//Calculate the beat time for the attack and release time for the note envelope
void RampSamples(vector<sf::Int16>& sample, float frac)
{
	int rampSamples = NUM_SAMPLES * frac;
	float dampen;
	float n = 0;
	float temp;

	//apply the left half of a triangle window to the first 'NUM_SAMPLES * frac' samples,
	//where n is the current sample
	for (size_t i = 0; i < rampSamples; i++)
	{ 
		//apply triangle window formula to sample and store that value into temp
		temp = n - ((static_cast<float>(rampSamples)));
		temp /= (static_cast<float>(rampSamples));

		//calculate amount to dampen
		dampen = 1 - abs(temp);

		//dampen audio sample
		sample.at(i) *= dampen;
		++n;
	}
	
	n = rampSamples - 1;

	//apply the right half of a triangle window to last 'NUM_SAMPLES * frac' samples,
	//where n is the current sample
	for (size_t i = NUM_SAMPLES - rampSamples; i < NUM_SAMPLES; i++)
	{
		//apply triangle window formula to sample and store that value into temp
		temp = n - ((static_cast<float>(rampSamples)));
		temp /= (static_cast<float>(rampSamples));

		//calculate amount to dampen
		dampen = 1 - abs(temp);

		//dampen audio sample
		sample.at(i) *= dampen;
		--n;
	}
}

//Fill the scaleVector with the proper key offsets so a proper frequency can be acquired for 
//each key
void fillMajorArray(vector<int>& scaleArray, char key)
{
	switch (key)
	{
	case('a'):
		scaleArray.insert(scaleArray.end(), { 0,2,1,2,2,1,2,2 });
		break;
	case('b'):
		scaleArray.insert(scaleArray.end(), { 0,1,2,2,1,2,2,2 });
		break;
	case('c'):
		scaleArray.insert(scaleArray.end(), { 0,2,2,1,2,2,2,1 });
		break;
	case('d'):
		scaleArray.insert(scaleArray.end(), { 0,2,1,2,2,2,1,2 });
		break;
	case('e'):
		scaleArray.insert(scaleArray.end(), { 0,1,2,2,2,1,2,2 });
		break;
	case('f'):
		scaleArray.insert(scaleArray.end(), { 0,2,2,2,1,2,2,1 });
		break;
	case('g'):
		scaleArray.insert(scaleArray.end(), { 0,2,2,1,2,2,1,2 });
		break;
	default:
		cout << "ERROR: unable to generate major array\n";
		break;
	}
}

//Parse the command line arguments that are given to run the program.
bool ParseArguments(int& rootKey, int& sig, float& bpm, float& frac, float& accent, float& volume, int argc, const char* argv[])
{
	string* argvArray = new string[argc];
	for (size_t i = 0; i < argc; i++)
		argvArray[i] = argv[i];

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
				return false;
			}
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
				return false;
			}
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
				return false;
			}
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
				return false;
			}
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
				return false;
			}
		}
		else if (!argvArray[i].find("--ramp"))
		{
			RemoveSubString(argvArray[i], "--ramp");
			RemoveSubString(argvArray[i], "[");
			RemoveSubString(argvArray[i], "]");
			try
			{
				frac = stof(argvArray[i]);
			}
			catch (invalid_argument const& error)
			{
				cout << "Invalid parameter value: " << error.what() << endl;
				return false;
			}
		}
		else
		{
			cout << "ERROR: invalid argument: " << argv[i] << endl;
			cout << "Closing program...\n";
			return false;
		}
	}
	return true;
}

//Check if the MIDI value the user gave is valid. If so, assign that value with a root key
int CheckKeyValue(int rootKey, char& character)
{
	if (rootKey == 21 || rootKey == 33 || rootKey == 45 || rootKey == 57 || rootKey == 69 || rootKey == 81 || rootKey == 93 || rootKey == 105)
		character = 'a';
	else if (rootKey == 23 || rootKey == 35 || rootKey == 47 || rootKey == 59 || rootKey == 71 || rootKey == 83 || rootKey == 95 || rootKey == 107)
		character = 'b';
	else if (rootKey == 24 || rootKey == 36 || rootKey == 48 || rootKey == 60 || rootKey == 72 || rootKey == 84 || rootKey == 96 || rootKey == 108)
		character = 'c';
	else if (rootKey == 26 || rootKey == 38 || rootKey == 50 || rootKey == 62 || rootKey == 74 || rootKey == 86 || rootKey == 98 || rootKey == 110)
		character = 'd';
	else if (rootKey == 28 || rootKey == 40 || rootKey == 52 || rootKey == 64 || rootKey == 76 || rootKey == 88 || rootKey == 100 || rootKey == 112)
		character = 'e';
	else if (rootKey == 29 || rootKey == 41 || rootKey == 53 || rootKey == 65 || rootKey == 77 || rootKey == 89 || rootKey == 101 || rootKey == 113)
		character = 'f';
	else if (rootKey == 31 || rootKey == 43 || rootKey == 55 || rootKey == 67 || rootKey == 79 || rootKey == 91 || rootKey == 103 || rootKey == 115)
		character = 'g';
	else
	{
		cout << "\n\nINVALID KEY VALUE: " << rootKey << ". Enter a valid MIDI note between 21-115! ";
		return 0;
	}
	return 1;
}
