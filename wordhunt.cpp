#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>

// extern "C" {
//     #include "hidapi.h"
// }


const int BOARDSIZE = 4;
std::vector<std::string> wordList;
std::vector<std::string> dictionary;
char board[BOARDSIZE][BOARDSIZE];
int dictFirstLetter[27];

void search();
void search(std::string currentSearch, std::vector<int> usedY, std::vector<int> usedX, int yPos, int xPos);
void sortAnswers();
bool compare(std::string a, std::string b);
bool isValidFrag(std::string wordFrag);
bool isValidWord(std::string word);
void buildDictionary();
//void playGame();

int main(int argc, char *argv[]){

    //std::cout << "\033[2J\033[1;1H";

    if (argc == 2) {
        std::ifstream infile(argv[1]);

        //check if the file is opened
        if (!infile.is_open()) {
            std::cerr << "ERROR: Error opening file";
            exit(EXIT_FAILURE);
        }
        //turn the file into 2d array
        for (int y = 0; y < BOARDSIZE; y++) {
            for (int x = 0; x < BOARDSIZE; x++) {
                infile >> board[y][x];
            }
        }
    } else {
        std::string temp;

        std::cout << "Please input your board using the following format:\n";
        std::cout << "ABCD\n";
        std::cout << "EFGH    -----> ABCDEFGHIJKLMNOP\n";
        std::cout << "IJKL\n";
        std::cout << "MNOP\n";

        std::cin >> temp;
        transform(temp.begin(), temp.end(), temp.begin(), ::tolower);

        for (int y = 0; y < BOARDSIZE; y++) {
            for (int x = 0; x < BOARDSIZE; x++) {
                board[y][x] = temp[(y * BOARDSIZE) + x];
            }
        }
    }

    buildDictionary();

    search();

    sortAnswers();

    //playGame();

    for (int i = 0; i < wordList.size(); i++) {
        std::cout << "WordList: " << wordList[i] << std::endl;
    }
    
    return 0;
}

void search() {
    std::vector<int> temp1, temp2;

    for (int y = 0; y < BOARDSIZE; y++) {
        for (int x = 0; x < BOARDSIZE; x++) {
            search("", temp1, temp2, y, x);
        }
    }
}

void search(std::string currentSearch, std::vector<int> usedY, std::vector<int> usedX, int yPos, int xPos) {

    for (int i = 0; i < usedX.size(); i++) {
        if (yPos == usedY[i] && xPos == usedX[i]) return;
    }
    usedY.push_back(yPos);
    usedX.push_back(xPos);

    currentSearch += board[yPos][xPos];

    if (!isValidFrag(currentSearch)) {
        return;
    } else if (isValidWord(currentSearch)) {
        wordList.push_back(currentSearch + ": " + std::to_string(usedX[0]) + ", " + std::to_string(usedY[0]));
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

bool compare(std::string a, std::string b) {
    return (a.size() < b.size());
}

bool isValidFrag(std::string wordFrag) {
    char temp = wordFrag[0];
    int alphaValue = (temp - 'a');
    for (int i = dictFirstLetter[alphaValue]; i < dictFirstLetter[alphaValue + 1]; i++) {
        if (wordFrag == dictionary[i].substr(0, wordFrag.length())) {
            std::cout << wordFrag << " is a word frag\n";
            return true;
        }
    }
    std::cout << wordFrag << " is not a word frag\n";
    return false;
}

bool isValidWord(std::string word) {
    if (word.length() < 3) return false;
    char temp = word[0];
    int alphaValue = (temp - 'a');
    for (int i = dictFirstLetter[alphaValue]; i < dictFirstLetter[alphaValue + 1]; i++) {
        if (word == dictionary[i]) {
            std::cout << word << " is a word\n";
            dictionary.erase(dictionary.begin() + i);
            return true;
        }
    }
    std::cout << word << " is not a word\n";
    return false;
}

void buildDictionary() {
    std::ifstream dictIn("dictionary.txt");
    std::string temp;
    int alphaCount = 0;
    char currentChar = 'a';
    dictFirstLetter[alphaCount] = 0;
    alphaCount++;
    
    if (!dictIn.is_open()) {
        std::cerr << "ERROR: Error opening file";
        exit(EXIT_FAILURE);
    }

    dictIn >> temp;

    while(!dictIn.eof()) {
        if (temp.size() < 3) {
            dictIn >> temp;
            continue;
        }
        transform(temp.begin(), temp.end(), temp.begin(), ::tolower);
        if (temp[0] != currentChar) {
            currentChar = temp[0];
            //dictFirstLetter is used later so you don't have to check the full dictionary every time
            dictFirstLetter[alphaCount] = dictionary.size();
            alphaCount++;
        }
        dictionary.push_back(temp);
        dictIn >> temp;
    }
    dictFirstLetter[26] = dictionary.size();
}

/*
void playGame() {
    
    
    iphone
    vendor id 	0x004C
    product id    0x730D
    serial number C8PC973LN72P
    

    //hid_device *handle = hid_open(0x004C, 0x730D, NULL);

    
    mx master
    Vendor ID:	0x046D
    Product ID:	0xB019
    
    hid_device *handle = hid_open(0x046D, 0xB019, NULL);

    int res = hid_init();

    if (!handle) {
		printf("Unable to open device\n");
		hid_exit();
        exit(EXIT_FAILURE);
	} else {
        std::cout << "Opening device worked\n";
    }
    
}
*/