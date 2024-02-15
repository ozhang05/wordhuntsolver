#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
using namespace std;

const int BOARDSIZE = 4;
vector<string> wordList;
vector<string> dictionary;
char board[BOARDSIZE][BOARDSIZE];

void search();
void search(string currentSearch, vector<int> usedY, vector<int> usedX, int yPos, int xPos);
void sortAnswers();
bool compare(string a, string b);
bool isValidFrag(string wordFrag);
bool isValidWord(string word);
void buildDictionary();

int main(int argc, char *argv[])
{
    ifstream infile(argv[1]);

    //check if the file is opened
    if (!infile.is_open()) {
        cerr << "ERROR: Error opening file";
        exit(EXIT_FAILURE);
    }

    //turn the file into 2d array
    for (int y = 0; y < BOARDSIZE; y++) {
        for (int x = 0; x < BOARDSIZE; x++) {
            infile >> board[y][x];
        }
    }

    buildDictionary();

    search();

    sortAnswers();

    for (int i = 0; i < wordList.size(); i++) {
        cout << "WordList: " << wordList[i] << endl;
    }
    
    return 0;
}

void search() {
    vector<int> temp1, temp2;

    for (int y = 0; y < BOARDSIZE; y++) {
        for (int x = 0; x < BOARDSIZE; x++) {
            search("", temp1, temp2, y, x);
        }
    }
}

void search(string currentSearch, vector<int> usedY, vector<int> usedX, int yPos, int xPos) {

    for (int i = 0; i < usedX.size(); i++) {
        if (yPos == usedY[i] && xPos == usedX[i]) return;
    }
    usedY.push_back(yPos);
    usedX.push_back(xPos);

    currentSearch += board[yPos][xPos];

    if (!isValidFrag(currentSearch)) {
        return;
    } else if (isValidWord(currentSearch)) {
        wordList.push_back(currentSearch);
    }
    if (yPos+1 < BOARDSIZE) {
        if (xPos+1 < BOARDSIZE) {
            search(currentSearch, usedY, usedX, yPos+1, xPos+1);
        }
        search(currentSearch, usedY, usedX, yPos+1, xPos);
        if (xPos-1 >= 0) {
            search(currentSearch, usedY, usedX, yPos+1, xPos-1);
        }
    }
    if (xPos+1 < BOARDSIZE) {
        search(currentSearch, usedY, usedX, yPos, xPos+1);
    }
    if (xPos-1 >= 0) {
        search(currentSearch, usedY, usedX, yPos, xPos-1);
    }
    if (yPos-1 >= 0) {
        if (xPos+1 < BOARDSIZE) {
            search(currentSearch, usedY, usedX, yPos-1, xPos+1);
        }
        search(currentSearch, usedY, usedX, yPos-1, xPos);
        if (xPos-1 >= 0) {
            search(currentSearch, usedY, usedX, yPos-1, xPos-1);
        }
    }
}

void sortAnswers() {
    sort(wordList.begin(), wordList.end(), compare);
}

bool compare(string a, string b) {
    return (a.size() < b.size());
}

bool isValidFrag(string wordFrag) {
    for (int i = 0; i < dictionary.size(); i++) {
        if (wordFrag == dictionary[i].substr(0, wordFrag.length())) {
            cout << wordFrag << " is a word frag\n";
            return true;
        }
    }
    cout << wordFrag << " is not a word frag\n";
    return false;
}

bool isValidWord(string word) {
    for (int i = 0; i < dictionary.size(); i++) {
        if (word == dictionary[i] && word.length() > 2) {
            cout << word << " is a word\n";
            dictionary.erase(dictionary.begin() - 1 + i);
            return true;
        }
    }
    cout << word << " is not a word\n";
    return false;
}

void buildDictionary() {
    ifstream dictIn("dictionary.txt");
    string temp;
    
    if (!dictIn.is_open()) {
        cerr << "ERROR: Error opening file";
        exit(EXIT_FAILURE);
    }

    dictIn >> temp;

    while(!dictIn.eof()) {
        transform(temp.begin(), temp.end(), temp.begin(), ::tolower);
        dictionary.push_back(temp);
        dictIn >> temp;
    }
}