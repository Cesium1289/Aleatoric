#include<SFML/Audio.hpp>
#include<SFML/Graphics.hpp>
#include<iostream>
#include<cmath>
#include<cstdlib>
using namespace std;

int ParseArguments(int& rootKey, int& sig, float& bpm, float& frac, float& accent, float& volume, int argc, const char* argv[]);
void CalculateFrequency(float freqArray[], vector<int>& scaleArray, int RootKey);
void fillMajorArray(vector<int>& scaleArray, char key);
void RemoveSubString(string& original, const char* substring);
void GenerateWaves(float freqArray[], sf::SoundBuffer soundSamples[], float sineWaveAmp, float bpm, float frac, float squareWaveAmp);
void SetAmplitudes(float accent, float volume, float& squareAmp, float& sineAmp);
void RampSamples(vector<sf::Int16>& sample, float frac);
const int NUM_SAMPLES = 48000;
const int NUM_KEYS = 8;
int main()
{
	sf::SoundBuffer soundSamples[NUM_KEYS]; 
	sf::Sound sound;
	const char* argv[] = {"--bpm[14]","--beats[10]" };
	int argc =2;
	float squareWaveAmp = 100;
	float sineWaveAmp =100;
	vector<int> scaleArray;
	int rootKey = 48;
	int sig = 8;
	float bpm = 120.0f;
	float frac = 0.5f;
	float accent = 8.0f;
	float volume = 8.0f;
	float freqArray[NUM_KEYS];
	char key = 'c';
	
	SetAmplitudes(accent, volume, squareWaveAmp, sineWaveAmp);
	fillMajorArray(scaleArray, key);
	CalculateFrequency(freqArray, scaleArray, rootKey);
	GenerateWaves(freqArray, soundSamples, sineWaveAmp, bpm, frac, squareWaveAmp);
	sound.setBuffer(soundSamples[0]);
	int currentSig = sig;
	sound.play();


	while (1)
	{
		if (currentSig = sig)
		{

		}
		sound.play();
		while (sound.getStatus() == sf::Sound::Playing)
		{
			sf::sleep(sf::milliseconds(100));
		}
		sound.setBuffer(soundSamples[rand() % NUM_KEYS]);
	}
	


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
}



cout << "\n\n-_-_-_-Sine-wave Summary-_-_-_-\n";
cout << "Number of channels: " << soundSamples[0].getChannelCount() << endl;
cout << "Sample size: " << soundSamples[0].getSampleCount() << endl;
cout << "Duration (in seconds): " << soundSamples[0].getDuration().asSeconds() << endl;
cout << "Sample rate: " << soundSamples[0].getSampleRate() << endl;

*/
}

//Calculate the freqency of the random keys that can be played
void CalculateFrequency(float freqArray[], vector<int>& scaleArray, int rootKey)
{
	int temp = 0;
	for (size_t i = 0; i < NUM_KEYS; i++)
	{
		freqArray[i] = 440 * static_cast<float>(pow(2.0, ((rootKey + static_cast<float>(scaleArray.at(i) + temp) - 69) / 12)));
		temp += scaleArray[i];
	}
		
	for (size_t i = 0; i < NUM_KEYS; i++)
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

//Generates the square and sine waves for keys of a given scale
void GenerateWaves(float freqArray[], sf::SoundBuffer soundSamples[], float sineWaveAmp, float bpm, float frac, float sqareWaveAmp)
{
	vector<sf::Int16> buffer;
	double temp;
	int a;
	for (size_t i = 0; i < 1; i++)
	{
		for (size_t j = 0; j < NUM_SAMPLES; j++)
		{
			temp = (2 * ((2 * (freqArray[i] * static_cast<float>(j))) - (2 * freqArray[i] * static_cast<float>(j)))) + 1;
			a = temp * 1000;
			buffer.push_back(a);
		}
	}
	soundSamples[0].loadFromSamples(&buffer[0], buffer.size(), 1, NUM_SAMPLES* (bpm / 60));
		

	//loop through all possible keys in a step for a given root
	for (size_t i = 1; i < NUM_KEYS; i++)
	{
		buffer.clear();

		for (size_t j = 0; j < NUM_SAMPLES; j++)
			buffer.push_back(1000 * sineWaveAmp * sin(2 * 3.14 * freqArray[i] * (static_cast<float>(j) / NUM_SAMPLES)));
			
		//ramp the sample 
		RampSamples(buffer, frac);

		//add samples to the sound samples
		soundSamples[i].loadFromSamples(&buffer[0], buffer.size(), 1, NUM_SAMPLES * (bpm / 60));
		cout << "SAMPLE : " << i << endl;
		cout << "\n\n-_-_-_-Sine-wave Summary-_-_-_-\n";
		cout << "Number of channels: " << soundSamples[i].getChannelCount() << endl;
		cout << "Sample size: " << soundSamples[i].getSampleCount() << endl;
		cout << "Duration (in seconds): " << soundSamples[i].getDuration().asSeconds() << endl;
		cout << "Sample rate: " << soundSamples[i].getSampleRate() << endl;
		
	}
}

//Set the amplitudes for the square and sine waves
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
	float n = 0;
	float temp;

	//apply the left half of a triangle window to the first 'NUM_SAMPLES * frac' samples
	for (size_t i = 0; i < rampSamples; i++)
	{ 
		temp = n - ((static_cast<float>(rampSamples)));
		temp /= (static_cast<float>(rampSamples));
		dampen = 1 - abs(temp);
		sample.at(i) *= dampen;
		++n;
	}
	
	n = rampSamples - 1;
	//apply the right half of a triangle window to last NUM_SAMPLES * frac' samples
	for (size_t i = NUM_SAMPLES - rampSamples; i < NUM_SAMPLES; i++)
	{
		temp = n - ((static_cast<float>(rampSamples)));
		temp /= (static_cast<float>(rampSamples));
		dampen = 1 - abs(temp);
		sample.at(i) *= dampen;
		--n;
	}
}

//Fill the scakeVector with the proper key offsets so a proper frequency can be aquired for 
//each key
void fillMajorArray(vector<int>& scaleArray, char key)
{
	switch (key)
	{
	case('a'):
		cout << "case a\n";
		scaleArray.insert(scaleArray.end(), { 0,2,1,2,2,1,2,2 });
		break;
	case('b'):
		cout << "case b\n";
		scaleArray.insert(scaleArray.end(), { 0,1,2,2,1,2,2,2 });
		break;
	case('c'):
		cout << "case c\n";
		scaleArray.insert(scaleArray.end(), { 0,2,2,1,2,2,2,1 });
		break;
	case('d'):
		cout << "case d\n";
		scaleArray.insert(scaleArray.end(), { 0,2,1,2,2,2,1,2 });
		break;
	case('e'):
		cout << "case e\n";
		scaleArray.insert(scaleArray.end(), { 0,1,2,2,2,1,2,2 });
		break;
	case('f'):
		cout << "case f\n";
		scaleArray.insert(scaleArray.end(), { 0,2,2,2,1,2,2,1 });
		break;
	case('g'):
		cout << "case g\n";
		scaleArray.insert(scaleArray.end(), { 0,2,2,1,2,2,1,2 });
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