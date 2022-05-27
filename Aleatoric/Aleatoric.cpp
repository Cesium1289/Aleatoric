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
void GenerateSineWaves(float freqArray[], sf::SoundBuffer soundSamples[], float amplitude, float bpm, float frac);
void SetAmplitudes(float accent, float volume, float& squareAmp, float& sineAmp);
void RampSamples(vector<sf::Int16>& sample, float frac);
const int NUM_SAMPLES = 48000;
const int NUM_KEYS = 8;

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "Window");
	sf::SoundBuffer soundSamples[NUM_KEYS]; 
	const char* argv[] = {"--bpm[14]","--beats[10]" };
	int argc =2;
	float squareWaveAmp = 100;
	float sineWaveAmp =100;
	vector<int> scaleArray;
	int rootKey = 72;
	int sig = 8;
	float bpm = 120.0f;
	float frac = 0.25f;
	float accent = 8.0f;
	float volume = 2.0f;
	float freqArray[NUM_KEYS];
	char key = 'c';
	int random;
	
	SetAmplitudes(accent, volume, squareWaveAmp, sineWaveAmp);

	//cout << "\nSquare: " << squareWaveAmp << endl;
	//cout << "Sine: " << sineWaveAmp << endl;
	/*
	vector<sf::Int16> temp;
	for (size_t i = 0; i < 50; i++)
	{
		temp.push_back(10);
	}
	RampSamples(temp, frac);
	for (size_t i = 0; i < 50; i++)
	{
		cout << temp.at(i) << endl;
	}*/
	//ParseArguments(rootKey, sig, bpm, frac, accent, volume, argc, argv);
	fillMajorArray(scaleArray, key);
	CalculateFrequency(freqArray, scaleArray, rootKey);
	GenerateSineWaves(freqArray, soundSamples, sineWaveAmp, bpm, frac);
	for (size_t i = 0; i < NUM_KEYS; i++)
	{
		//cout << freqArray[i] << endl;
	}
	//CalculateFrequency(freqArray, scaleArray, rootKey);
	//GenerateSineWaves(freqArray, soundSamples, sineWaveAmp, bpm, frac);

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



	cout << "\n\n-_-_-_-Sine-wave Summary-_-_-_-\n";
	cout << "Number of channels: " << soundSamples[0].getChannelCount() << endl;
	cout << "Sample size: " << soundSamples[0].getSampleCount() << endl;
	cout << "Duration (in seconds): " << soundSamples[0].getDuration().asSeconds() << endl;
	cout << "Sample rate: " << soundSamples[0].getSampleRate() << endl;
	sf::Sound sound;
	int k = 0;
	//sf::Sound::Status status;
	//status = sound.getStatus();
	sound.setBuffer(soundSamples[0]);
	sound.play();
	
	while (1)
	{
		
		sound.play();
		while (sound.getStatus() == sf::Sound::Playing)
		{
			sf::sleep(sf::milliseconds(100));
		}
		random = rand() % NUM_KEYS;
		sound.setBuffer(soundSamples[random]);
		++k;
		cout << "Switching!\n";
			
		
		//cout << "There\n";
	}

	while (window.isOpen())
	{
		sf::Event event;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
	}

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

//Generates the sinewave for keys of a given scale
void GenerateSineWaves(float freqArray[], sf::SoundBuffer soundSamples[], float amplitude, float bpm, float frac)
{
	float time = 60 / bpm;
	sf::SoundBuffer temp;
	
	/*
	for (size_t i = 0; i < NUM_SAMPLES; i++)
		buffer.push_back(amplitude * sin(2 * 3.14 * freqArray[0] * (time / NUM_SAMPLES)));

	//temp.loadFromSamples(&buffer[0], buffer.size(),1, (60/bpm) * NUM_SAMPLES);
	temp.loadFromSamples(&buffer[0], buffer.size(), 1,NUM_SAMPLES * (bpm/60));
	cout << "Number of channels: " << temp.getChannelCount() << endl;
	cout << "Sample size: " << temp.getSampleCount() << endl;
	cout << "Duration (in seconds): " << temp.getDuration().asSeconds() << endl;
	cout << "Sample rate: " << temp.getSampleRate() << endl;

	*/

	sf::SoundBuffer sineBuffer;
	vector<sf::Int16> buffer;
	cout << freqArray[0] << endl;
	double test;
	//loop through all possible keys in a step for a given root
	for (size_t i = 0; i < NUM_KEYS; i++)
	{
		
		//sf::Int16 buffer[NUM_SAMPLES];
		//generate the sinewave for a given key
		float t = 0;
		float a = 0;
		float b;
		float d = 0.0f;
		buffer.clear();
		for (size_t j = 0; j < NUM_SAMPLES; j++)
		{ 
			b = 2 * 3.14 * freqArray[i] * static_cast<float>(d/ NUM_SAMPLES);
			a = sin(b);
			t = amplitude * a;
			test = t * 1000;
			buffer.push_back(test);
			d += 1.0f;
			//	cout << t << endl;
			//buffer.push_back(amplitude * sin(2 * 3.14 * freqArray[i] * (time / NUM_SAMPLES)));
		}
			
			

		//ramp the sample 
		//RampSamples(buffer, frac);

		//add ramped sample to the sound samples
		//soundSamples[i].loadFromSamples(&buffer[0], buffer.size(), 60 / bpm, NUM_SAMPLES);	
		soundSamples[i].loadFromSamples(&buffer[0], buffer.size(), 1, NUM_SAMPLES * (bpm / 60));
		cout << "SAMPLE : " << i << endl;
		cout << "\n\n-_-_-_-Sine-wave Summary-_-_-_-\n";
		cout << "Number of channels: " << soundSamples[i].getChannelCount() << endl;
		cout << "Sample size: " << soundSamples[i].getSampleCount() << endl;
		cout << "Duration (in seconds): " << soundSamples[i].getDuration().asSeconds() << endl;
		cout << "Sample rate: " << soundSamples[i].getSampleRate() << endl;
		
	}
	
	//sf::Sound sound;
	//sound.setBuffer(soundSamples[0]);
	//sound.play();
	

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
	//int rampSamples = NUM_SAMPLES * frac;
	int rampSamples = 50;
	float dampen;
	cout <<"\n\n\nthignsd: " << rampSamples * frac << endl;
	float a =rampSamples * frac;
	float temp ;

	//apply the left half of a triangle window to the ramp samples
	for (size_t i = 0; i < rampSamples * frac; i++)
	{ 
		temp = (static_cast<float>(i) - ((rampSamples/2) / static_cast<float>(2))) / ((rampSamples/2) / static_cast<float>(2));
		dampen = 1 - abs(temp);
		sample.at(i) *= dampen;
	}
	
	//apply the right half of a triangle window to the ramp samples
	for (size_t i = rampSamples/2; i < rampSamples; i++)
	{
		//temp = (static_cast<float>(i) - (rampSamples / static_cast<float>(2))) / (rampSamples / static_cast<float>(2));
		 temp = (static_cast<float>(i) - ((rampSamples) / static_cast<float>(2))) / ((rampSamples ) / static_cast<float>(2));
		dampen = 1 - abs(temp);
		sample.at(i) *= dampen;
	}
}


void fillMajorArray(vector<int>& scaleArray, char key)
{
	switch (key)
	{
	case('a'):
		cout << "case a\n";
		//scaleArray = new int[7]{ 0,2,1,2,2,1,2 };
		scaleArray.insert(scaleArray.end(), { 0,2,1,2,2,1,2,2 });
		break;
	case('b'):
		cout << "case b\n";
		scaleArray.insert(scaleArray.end(), { 0,1,2,2,1,2,2,2 });
		//scaleArray = new int[7]{ 0,1,2,2,1,1,2 };
		break;
	case('c'):
		cout << "case c\n";
		scaleArray.insert(scaleArray.end(), { 0,2,2,1,2,2,2,1 });
		//scaleArray = new int[7]{ 0,2,2,1,2,2,2 };
		break;
	case('d'):
		cout << "case d\n";
		scaleArray.insert(scaleArray.end(), { 0,2,1,2,2,2,1,2 });
		//scaleArray = new int[7]{ 0,2,1,2,2,2,1 };
		break;
	case('e'):
		cout << "case e\n";
		scaleArray.insert(scaleArray.end(), { 0,1,2,2,2,1,2,2 });
		//scaleArray = new int[7]{ 0,1,2,2,2,1,2 };
		break;
	case('f'):
		cout << "case f\n";
		scaleArray.insert(scaleArray.end(), { 0,2,2,2,1,2,2,1 });
		//scaleArray = new int[7]{ 0,2,2,2,1,2,2 };
		break;
	case('g'):
		cout << "case g\n";
		scaleArray.insert(scaleArray.end(), { 0,2,2,1,2,2,1,2 });
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