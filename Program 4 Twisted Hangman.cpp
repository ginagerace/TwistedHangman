/* ------------------------------------------------
   ggerac3_mschro9Prog4.cpp
 
   Program #4: Twisted Hangman
               Program delays choosing which word is chosen,
               making the game as difficult as possible.
   Class: CS 141
   Authors: Gina Gerace & Mahal Schroeder
   Lab: Tues 11am
   
   System:  C++ Windows Dev-C++

   Grading Rubric:

   50 Execution points
         2 Displays header info and instructions on screen when run
         5 Correctly counts and displays the number of initial dictionary words, for any dictionary
        10 Correctly extracts words of the selected length and displays count for those words, < 5 seconds
         3 Move number is displayed and updates correctly.  Move number starts at 15.
         2 Handles upper and lower-case user input for the letters that are guessed
         3 Displays letters guessed so far, in alphabetical order
        10 Updates number of words left after each guess
         5 Input of '~' displays all possible words remaining
        10 Correctly displays letters once all words remaining have that letter, eliminating non-match words
         5 Displays appropriate message when word is guessed, or when it is not guessed and guesses run out
   45 Programming Style (Given only if program runs substantially correctly)
         5 Program naming convention is followed
        10 Meaningful identifier names
        10 Comments: Has header and this rubric.  Has comments on each block of code
        10 Appropriate data and control structures 
        10 Code Layout: Appropriate indentation and blank lines
   ------------------------------------------------
*/

#include <iostream>
#include <vector>
#include <cctype>       // For the letter checking functions
#include <fstream>      // For file input
using namespace std;


// Function to print all the letters of the vector containing the word to be guessed
void printWord(const vector<char>& word) {
	// if the letter has not been guessed, print a blank line
	for(int i=0; i<word.size(); i++) {
		if(word.at(i) == ' ')
		{cout << "_ ";	}
		else
		{cout << word.at(i) << " ";}
	}
	cout << endl;
}

// Function to add to the vector of guessed letters while keeping it alphabetical
void addAlpha(char adding, vector<char>& word) {
	vector<char> temp(15);
	temp = word;
	char hold = adding;
	
	for (int i=0; i<15; i++) {
		if((hold < temp.at(i)) || (temp.at(i)==' ')){
			word.at(i) = hold;
			hold = temp.at(i);
		}
	}
}
 
// Function to check if a word contains a certain letter
int contains(char find, string check) {
	for (int letter=0; letter<check.length(); letter++) {
		if (check.at(letter) == find) {
			return letter;
		}
	}
	return -1;
} 
 
// Function to get rid of any words with the letter guessed, returns the number of words left
bool shortenList(char find, vector<string>& list, vector<char>& guessWord, bool placedLetter) {
	vector<string> tempList(list.size());
	tempList = list;
	list.resize(0);
	int count = 0;
	string theWord = "";
	int uses[tempList.size()];
	int counter;
	int index = 1;
	
	// check how many words will be left
	for (int word=0; word<tempList.size(); word++) {
		if((contains(find, tempList.at(word))) == -1) {
			theWord = tempList.at(word);
			count++;
		}
	}
	
	// if there is only word left it becames the word to be guessed
	if(count == 1) {
		list.push_back(theWord);
	}
	
	// get rid of any words with the letter if there will be at least one word left
	else if(count>0) {
		for (int word=0; word<tempList.size(); word++) {
			if((contains(find, tempList.at(word))) == -1) {
				list.push_back(tempList.at(word));
			}
		}
	}
	
	// place a letter when the computer is forced to 
	else if((count == 0) && (placedLetter == false)) {
		for(int j=0; j<tempList.size(); j++) {
			theWord = tempList.at(j);
			counter = 0;
			for(int i=0; i<theWord.length(); i++) {
				if(theWord.at(i) == find) {
					counter++;
				}
			}
			uses[j] = counter;
		}
		
		// check to see how many instacnes of the letter there will be
		int num = 0;	
		while(list.size() < 1) {
			num++;
			for(int word=0; word<tempList.size(); word++) {
				if(uses[word] == num) {
					list.push_back(tempList.at(word));
				}
			}
		}
		
		// if there is only one occurence
		if (num == 1) {
			tempList = list;
			list.resize(0);
			theWord = tempList.at(0);
			for(int s=0; s<theWord.length(); s++) {
				if(theWord.at(s) == find) {
					index = s;
					break;
				}
			}
			// add all words that have the letter at the correct spot back to the list
			for(int word=0; word<tempList.size(); word++) {
				if(tempList.at(word).at(index) == find) {
					list.push_back(tempList.at(word));
				}
			}
			guessWord.at(index) = find;
		}
		
		// if there is more than one occurrence
		else {
			theWord = list.at(0);
			for(int s=0; s<theWord.length(); s++) {
				if(theWord.at(s) == find) {
					guessWord.at(s) = find;
				}
			}
		}
		
		placedLetter = true;
		return true;
	}
	
	// add letter to the word if list of words cannot shrink anymore
	else if((count == 0) && (placedLetter == true)) {
		for (int word=0; word<tempList.size(); word++) {
			if((contains(find, tempList.at(word))) != -1) {
				list.push_back(tempList.at(word));
			}
		}
		theWord = list.at(0);
		for(int f=0; f<theWord.length(); f++) {
			if(theWord.at(f) == find) {
				guessWord.at(f) = find;
			}
		}
	}
	
	return false;
} // end of shortenList function

// Function to print the remaining words if user input is '~'
string cheat(vector<string>& other) {
	string words;
	for (int i=0; i<other.size(); i++) {
		words += other.at(i) + " ";
	}
	 return words;
}


int main()
{
	
	// Display identifying information
    cout <<  "Authors: Gina Gerace & Mahal Schroeder" << endl << "Lab: Tues. 11am" << endl
         << "Program: #4, Twisted Hangman" << endl
         << endl;
       
    // Display the instuctions
    cout << "Welcome to the game of Twisted Hangman where you must try" << endl 
		 << "to correctly guess all the letters of a word. You have 15 chances." << endl
         << "You will get to choose the length of the word at the start of " << endl
         << "the game. Good luck!" << endl << endl;
	
    ifstream file;  // declare the input file stream
 	string oneWord;
 	
    // open input file and verify
    file.open("dictionary.txt");
    if (!file.is_open()) {
    	cout << "Could not find input file.  Exiting..." << endl;
    	exit( -1);
	}
	
	vector<string> list(0);		// create a vector that will store the words from dictionary
	
	// use a loop to get the size while putting all of the words into the vector
	while (file >> oneWord) {
		list.push_back(oneWord);
	}
	
	// go through each word and convert to uppercase or delete if it contains punctuation
	for (int word=list.size()-1; word>=0; word--) {
		for (int letter=0; letter<list.at(word).length(); letter++) {
			if (isalpha(list.at(word).at(letter))) {
				list.at(word).at(letter) = toupper(list.at(word).at(letter));
			}
			else if (ispunct(list.at(word).at(letter))) {
				list.erase(list.begin() + word);
				break;
			}
		}
	}
	
	// get user input to pick the length of the word to be guessed
	int length;
	cout << "Starting with " << list.size() << " words." << endl
		 << "What length word do you want? ";
	cin >> length;
	
	// create an array to store the word that will be guessed
	vector <char> guessWord(length);
	for (int i=0; i<length; i++) {
		guessWord.at(i) = ' ';
	}

	// get rid of any words that aren't the chosen length
	vector<string> tempList(list.size());
	tempList = list;
	list.resize(0);
	for (int word=tempList.size()-1; word>=0; word--) { 
		if (tempList.at(word).length() == length) {
			list.push_back(tempList.at(word));
		}
	}
	
	cout << "Now we have " << list.size() << " words of length " << length << endl << endl;
	
	// create a vector to store all the letters that have been guessed
	char guessLetter;
	vector<char> usedLetters(0);
	for(int i=0; i<15; i++) {
		usedLetters.push_back(' ');
	}
	
	// initialize variables to be used in for loop
	int counter;
	bool addToList;
	bool letterCheck = false;
	string theWord;
	int index;
	string aWord;
	int wordCount[16];
	wordCount[16] = 0;
	
	// user gets 15 guesses to figure out the word
	for (int turn = 15; turn>0; turn--) {
		addToList = true;
		cout << turn << ". Letters used so far: ";
		for (int i=0; i<15-turn; i++) {
			cout << usedLetters[i] << " ";
		}
		cout << endl
			 << "Letters found: ";
			 printWord(guessWord);
		cout << "Guess a letter: ";
		cin >> guessLetter;

		// execute the cheat function that prints all remaining words
		if(guessLetter == '~') {
			cout << cheat(list) << endl << endl;;
			turn++;
		    continue;
		}
		guessLetter = toupper(guessLetter);
		
		// calls the function to shorten list if the list can still be shrunk
		if(shortenList(guessLetter, list, guessWord, letterCheck) == true) {
			letterCheck = true;
		}
	
		if(list.size() == 1)
			{theWord = list.at(0);}	
		else
			{theWord = "";}
			
		// if all the letters have been guessed, exit the main for loop
		if(theWord.length() != 0) {
			counter = 0;		
			for(int i=0; i<theWord.length(); i++) {
				if(theWord.at(i)==guessWord.at(i)) {
					counter ++;
				}
			}
			if(counter == theWord.length())
				{break;}
		}
		cout << "Now we have " << list.size() << " words." << endl << endl;
		
		// add guessed letter to the array storing the guesses if it is not in the word
		if (addToList)
			{addAlpha(guessLetter, usedLetters);}	
	}
	
	cout << theWord << endl << endl;
	
	// print end of game message
	if((counter == theWord.length())) {
		cout << "*** Congratulations, you did it! ***" << endl;
	}
	else {
		cout << "Sorry, you didn't guess the word." << endl;
	}
	
	cout << endl << "Done." << endl;
    file.close();
    return 0;
   
}//end main()
