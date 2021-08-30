#include <iostream>
#include <string>
#include <cstring>
#include <algorithm>
#include <cctype>
#include <ctime>
#include <conio.h>
#include <fstream>

using namespace std;

const string FILE_PATH = "C:/Users/.../words.txt"; // SET PATH HERE TO RUN THE PROGRAM

int userInt(string prompt)
{
    cout << prompt << endl;
    string input;
    cin >> input;
    return std::all_of(input.begin(), input.end(), ::isdigit) ? stoi(input) : 0;
}

int userIntInRange(string prompt, int min, int max)
{
    int res = 0;
    while (res > max || res < min)
    {
        res = userInt(prompt);
    }
    return res;
}

string toLower(string str)
{
    int i = 0;
    char lower[20] = { 0 };
    while (isalpha(str[i]))
    {
        lower[i] = tolower(str[i]);
        i++;
    }
    return lower;
}

bool isEntireCorrectGuess(string word, string guess)
{
    return toLower(word) == toLower(guess);
}

bool isCorrectGuess(string word, string *wordRef, char guess, int len)
{
    bool isCorrect = false;
    string modWord;
    for (int i = 0; i < len; i++)
    {
        if (tolower(guess) == tolower(word[i]))
        {
            isCorrect = true;
            modWord += tolower(guess);
        }
        else
        {
            modWord += wordRef->at(i);
        }
    }
    *wordRef = modWord;
    return isCorrect;
}

bool allLettersGuessed(string word, int len)
{
    bool allGuessed = true;
    for (int i = 0; i < len; i++)
    {
        if (word[i] == '*') allGuessed = false;
    }
    return allGuessed;
}

int* gameLoop(int numberOfPlayers, int numberOfRounds) 
{
    static int score[2];
    int turn;
    int guesser;
    int lives;
    int len;
    for (int i = 0; i < numberOfRounds; i++)
    {
        cout << endl << "*****" << endl << "Round " << i + 1 << endl;
        lives = 6;
        len = 0;
        char word[20] = { 0 };

        if (numberOfPlayers == 1)
        {
            turn = 0;
            string outContents;
            ifstream fileContents;
            fileContents.open(FILE_PATH);
            if (!fileContents.fail())
            {
                srand(time(0));
                int stopOn = (rand() % 30) + 1;
                cout << stopOn << endl;
                int curr = 0;
                while(fileContents >> outContents)
                {
                    if (curr == stopOn)
                    {
                        len = outContents.length();
                        strcpy(word, outContents.c_str());
                        break;
                    }
                    curr++;
                }
            }
            fileContents.close();
        }
        else {
            turn = i % 2;
            cout << "Player " << turn + 1 << "'s turn to provide a word" << endl;
            cout << "Enter word here: ";
            for (int j = 0; j < 20; j++) 
            {
                word[j] = _getch(); 
                if (word[j] == 13) break;
                else _putch('*');
                len++;
            }
        }

        cout << endl;
        string viewWord;
        for (int k = 0; k < len; k++)
        {
            viewWord += '*';
        }
        while (lives > 0)
        {
            string guess;
            cout << "Word: " << viewWord << endl;
            cout << "Lives: " << lives << endl;
            cin >> guess;
            guesser = numberOfPlayers == 1 ? 0 : abs(turn - 1);
            if (isEntireCorrectGuess(word, guess))
            {
                cout << "Player " << guesser + 1 << " guessed the entire word correctly." << endl;
                break;
            }
            else if (isCorrectGuess(word, &viewWord, guess[0], len))
            {
                cout << "Correct guess." << endl;
                if (allLettersGuessed(viewWord, len)) break;
            }
            else
            {
                cout << "Incorrect guess." << endl;
                lives--;
            }
        }
        if (lives > 0) score[guesser]++;
        else 
        {
            if (numberOfPlayers > 1) score[turn]++;
            cout << "The word was " << word << "." << endl;
        }
    }
    return score;
}

void gameOver(int numberOfPlayers, int score[2])
{
    cout << "Game Over" << endl;
    if (numberOfPlayers == 1)
    {
        cout << "Score: " << *(score) << endl;
    }
    else
    {
        cout << "Player 1 score: " << *(score) << endl;
        cout << "Player 2 score: " << *(score + 1) << endl;
    }
}

int main()
{
    cout << "Welcome to Hangman" << endl;
    int numberOfPlayers = userIntInRange("How many players? (1-2)", 1, 2);
    int numberOfRounds = userIntInRange("How many rounds? (1-10)", 1, 10);
    int *score;
    score = gameLoop(numberOfPlayers, numberOfRounds);
    gameOver(numberOfPlayers, score);
    return 0;
}
