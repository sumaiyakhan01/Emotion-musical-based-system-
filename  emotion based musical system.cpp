#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>  // For system() function
#include <ctime>    // For random song selection

using namespace std;

// Function to classify emotion from user input
string detectEmotion(string userInput) {
    if (userInput.find("happy") != string::npos || userInput.find("joy") != string::npos)
        return "happy";
    else if (userInput.find("sad") != string::npos || userInput.find("depressed") != string::npos)
        return "sad";
    else if (userInput.find("angry") != string::npos || userInput.find("frustrated") != string::npos)
        return "angry";
    else if (userInput.find("calm") != string::npos || userInput.find("relaxed") != string::npos)
        return "calm";
    else
        return "neutral";
}

// Function to read songs from file
vector<string> getSongsFromFile(string filename) {
    vector<string> songs;
    ifstream file(filename);
    string song;
    
    if (file.is_open()) {
        while (getline(file, song)) {
            songs.push_back(song);
        }
        file.close();
    } else {
        cout << "Error: Could not open file " << filename << endl;
    }
    
    return songs;
}

// Function to play a song
void playSong(string songPath) {
    if (songPath.empty()) {
        cout << "No song available for this mood." << endl;
        return;
    }

#ifdef _WIN32
    system(("start " + songPath).c_str());  // Windows
#else
    system(("xdg-open " + songPath).c_str());  // Linux
#endif
}

// Function to log user history
void logHistory(string emotion, string song) {
    ofstream logFile("history.txt", ios::app);
    if (logFile.is_open()) {
        logFile << "Emotion: " << emotion << " | Song Played: " << song << endl;
        logFile.close();
    }
}

int main() {
    string userInput;
    cout << "Enter how you feel: ";
    getline(cin, userInput);

    string detectedEmotion = detectEmotion(userInput);
    cout << "Detected Emotion: " << detectedEmotion << endl;

    // Map emotions to file names
    string fileMap[5][2] = { 
        {"happy", "happy_songs.txt"}, 
        {"sad", "sad_songs.txt"},
        {"angry", "angry_songs.txt"},
        {"calm", "calm_songs.txt"},
        {"neutral", "neutral_songs.txt"}
    };

    string songFile = "";
    for (auto &emotion : fileMap) {
        if (detectedEmotion == emotion[0]) {
            songFile = emotion[1];
            break;
        }
    }

    vector<string> songs = getSongsFromFile(songFile);

    if (!songs.empty()) {
        srand(time(0));
        string selectedSong = songs[rand() % songs.size()];
        cout << "Playing: " << selectedSong << endl;
        playSong(selectedSong);
        logHistory(detectedEmotion, selectedSong);
    } else {
        cout << "No songs available for this mood." << endl;
    }

    return 0;
}