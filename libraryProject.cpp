
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

using namespace std;

// Structure to represent a word maybe syn or antonym
struct sWord {
    char data[30];
    sWord* next;
};

// Structure to represent a word in the dictionary,data is the actual word and each word in the dictionary has syn or ant of type sWord and next for next word in dictionary and prev for prev
struct dWord {
    char data[30];
    sWord* synonym;
    sWord* antonym;
    dWord* next;
    dWord* previous;
};

// Structure to represent the dictionary as a doubly linked list
struct Dictionary {
    dWord* head;
    dWord* tail;
};
void displayDictionary(Dictionary& dictionary) {
    dWord* current = dictionary.head;
    while (current != nullptr) {
        cout << "Word: " << current->data << endl;

        // Display Synonyms
        sWord* synonym = current->synonym;
        if (synonym != nullptr) {
            cout << "  Synonyms: "<<endl;
            while (synonym != nullptr) {
                cout << synonym->data << " ";
                synonym = synonym->next;
            }
            cout << endl;
        }
        // Display Antonyms
        sWord* antonym = current->antonym;
        if (antonym != nullptr) {
            cout << "  Antonyms: ";
            while (antonym != nullptr) {
                cout << antonym->data << " ";

                antonym = antonym->next;
            }
            cout << endl;
        }

        cout << "------------------------" << endl;
        current = current->next;
    }
}

bool isEmptyList(Dictionary* D) {
	return (D->head == NULL);
}

Dictionary parseInputFile( string& filename) {
    Dictionary dictionary;
    dictionary.head = nullptr;
     dictionary.tail = nullptr; 
        ifstream inputFile(filename);

    if (!inputFile.is_open()) {
        cout << "Error opening file!" << endl;
        return dictionary;
    }
    char line[300];
    char* context;

    while (inputFile.getline(line, sizeof(line))) {
        dWord* newWord = new dWord;  //has syn and ant
        newWord->synonym = NULL;
        newWord->antonym = NULL;
        newWord->next = NULL;
        // Returns the next token in a C-string
        char* word = strtok_s(line, ":",&context);
        char* synonyms = strtok_s(nullptr, "#",&context);
        char* antonyms = strtok_s(nullptr, "",&context);
        //copy the word to newWord data
        if (word != NULL) {
            strcpy_s(newWord->data, word);
        //copy the synonym
            if (synonyms != NULL) {
                sWord* newSynonym = new sWord;
                strcpy_s(newSynonym->data, synonyms);
                newWord->synonym = newSynonym;  //make the syn of it equal to the syn
                newSynonym->next = NULL;
            }

            if (antonyms != NULL) {
                sWord* newAntonym = new sWord;
                strcpy_s(newAntonym->data, antonyms);
                newWord->antonym = newAntonym;
                newAntonym->next = NULL;
            }
            if (isEmptyList(&dictionary)) {
                dictionary.head = newWord;
                dictionary.tail = newWord;
            }
            else {
                dictionary.tail->next = newWord;
                newWord->previous = dictionary.tail;
                dictionary.tail = newWord;
            }
        }
    }
    inputFile.close();
    return dictionary;
}


bool isWordWritten(sWord*& writtenWords, char* word) {
    sWord* current = writtenWords;
    while (current != nullptr) {
        if (strcmp(current->data, word) == 0) {
            return true; // Word has been written
        }
        current = current->next;
    }
    return false; // Word has not been written
}

//add it to the linkedlist
void markWordAsWritten(sWord*& writtenWords, char* word) {
    sWord* newWrittenWord = new sWord;
    strcpy(newWrittenWord->data, word);
    newWrittenWord->next = writtenWords;
    writtenWords = newWrittenWord;
}


//function to write dictionary to file

void writeDictionaryToFile(Dictionary& dictionary, string& filename) {
fstream outputFile(filename, ios::out);

    if (!outputFile) {
        cout << "Error opening file for writing: " << filename << endl;
        return;
    }
        

    sWord* writtenWords = nullptr; // To keep track of words that have been written intilize it first to nullptr
       
    dWord* current = dictionary.head;
    

    while (current != nullptr) {

        // As you process each word in your dictionary in the writeDictionaryToFile function, the isWordWritten function checks whether a word has already been written by iterating through the linked list pointed to by writtenWords. If a word has not been written, it is marked as written using the markWordAsWritten function, which adds the word to the linked list.
        if (!isWordWritten(writtenWords, current->data)) {

            // Write the word to the file
            outputFile << current->data;

            // Write synonyms 
            sWord* syn = current->synonym;
            while (syn != nullptr) {
                outputFile << ":" << syn->data;
                syn = syn->next;
            }

            // Write antonyms 
            sWord* ant = current->antonym;
            while (ant != nullptr) {
                outputFile << "#" << ant->data;
                ant = ant->next;
            }

            // Mark the word as written by taking the word and add it to writtenWords linkedlist
            markWordAsWritten(writtenWords, current->data);

            // Move to the next word in the dictionary
            current = current->next;

            // Add the separator between words
            outputFile << "\n";
        } 
        else {
            // Move to the next word in the dictionary
            current = current->next;
        }
    }

    outputFile.close();
}

void swapNodes(dWord* node1, dWord* node2) {
    char tempWord[30];
    strcpy(tempWord, node1->data);
    strcpy(node1->data, node2->data);
    strcpy(node2->data, tempWord);
}

void sortDictionary(Dictionary& dictionary) {
    dWord* current = dictionary.head;

    while (current != nullptr) {
        dWord* index = current->next;

        while (index != nullptr) {
            if (strcmp(current->data, index->data) > 0) {
                // Swap nodes if the current word should come after the next word
                swapNodes(current, index);
            }

            index = index->next;
        }

        current = current->next;
    }
}
void insertInCorrectPosition(Dictionary& dictionary, char* newWord) {
    // Check if the newWord already exists in the dictionary
    dWord* current = dictionary.head;
    while (current != nullptr) {
        if (strcmp(current->data, newWord) == 0) {
            cout << "Word '" << newWord << "' already exists in the dictionary." << endl;
            return;
        }
        current = current->next;
    }

    // Add the new word to the dictionary without any synonyms or antonyms
    dWord* newEntry = new dWord;
    strcpy(newEntry->data, newWord);
    newEntry->synonym = nullptr;
    newEntry->antonym = nullptr;
    newEntry->next = nullptr;
    newEntry->previous = nullptr;

    // Find the correct position to insert the new word
    current = dictionary.head;
    while (current != nullptr && strcmp(current->data, newWord) < 0) {  //when it is >0 we know the position 
        current = current->next;
    }

    // Insert the new word at the correct position
   if (current == nullptr) {
    // Add the new word to the end of the dictionary
    if (dictionary.head == nullptr) {
        dictionary.head = newEntry;
        dictionary.tail = newEntry;
    } else {
        newEntry->previous = dictionary.tail;
        dictionary.tail->next = newEntry;
        dictionary.tail = newEntry;
    }
} else {
    // Insert the new word before the current word
    newEntry->next = current;
    newEntry->previous = current->previous;
    if (current->previous != nullptr) {
        current->previous->next = newEntry;
    } else {
        dictionary.head = newEntry;
    }
    current->previous = newEntry;
}


    // Print the updated dictionary
    cout<<"this is the new dictionary after adding the new word to it:"<<endl;
    displayDictionary(dictionary);
        cout << "Word '" << newWord << "' has been added to the dictionary." << endl;

}
bool containsLetters(char* word,char* letters){
int wordsize = strlen(word);
          int letterssize = strlen(letters);
		 for (int i = 0; i < wordsize; i++) {
         for (int j = 0; j < letterssize; j++) {
            if (word[i] == letters[j]) {
                return true; 
            }
        }
    }
    return false;

}

//ex5:

void addSynonymOrAntonym(Dictionary& dictionary, char* existingWord, char* newWord) {

    // First, we need to check if the existing word provided by the user exists in the dictionary or not
    dWord* current = dictionary.head;
    while (current != nullptr && strcmp(current->data, existingWord) != 0) {
        current = current->next;
    }
     if (current == nullptr) {
         cout << "The word "<<existingWord <<" you provided is not in the dictionary" << endl;
         return;
     }

    cout << "Is this word a synonym or an antonym? (S/A): ";
    char SynOrAnt;
    cin >> SynOrAnt;

    if (SynOrAnt == 'S') {
        sWord* synonymList = current->synonym;
        while (synonymList != nullptr) {
            if (strcmp(synonymList->data, newWord) == 0) {
                cout << newWord << " is already a synonym for " << existingWord << endl;
                return;
            }
            synonymList = synonymList->next;
        }

        sWord* newSyn = new sWord;
        strcpy(newSyn->data, newWord);
        newSyn->next = current->synonym;
        current->synonym = newSyn;

        cout << "The word has been added as a synonym" << endl;
        displayDictionary(dictionary);
    } else if (SynOrAnt == 'A') {
        sWord* antonymList = current->antonym;
        while (antonymList != nullptr) {
            if (strcmp(antonymList->data, newWord) == 0) {
                cout << newWord << " is already an antonym for " << existingWord << endl;
                return;
            }
            antonymList = antonymList->next;
        }

        sWord* newAntonym = new sWord;
        strcpy(newAntonym->data, newWord);
        newAntonym->next = current->antonym;
        current->antonym = newAntonym;

        cout << "The word has been added as an antonym" << endl;
        displayDictionary(dictionary);
    } else {
        cout << "Invalid choice" << endl;
    }
}


//ex6:
void deleteWordsContainingLetters(Dictionary& dictionary, char* letters) {
    dWord* current = dictionary.head;

    while (current != nullptr) {
        dWord* nextNode = current->next;

        if (containsLetters(current->data, letters)) {
            // If the word is at the head
            if (current == dictionary.head) {
                dictionary.head = current->next;
                if (current->next != nullptr) {
                    current->next->previous = nullptr;
                }
            }
            // If the word is at the tail
            else if (current == dictionary.tail) {
                dictionary.tail = current->previous;
                current->previous->next = nullptr;
            } else {
                current->previous->next = current->next;
                current->next->previous = current->previous;
            }

            delete current; // Delete the node after updating pointers

            current = nextNode;  // Move to the next node without referencing the deleted node
        } else {
            current = nextNode;  // Move to the next node if no deletion occurred
        }
    }
}


//check if it starts with the specified set of letters. 

bool isStartWithSpecifiedLetters(char* word,char* letters){
    int Len = strlen(letters);

    if(strncmp(word, letters, Len)==0){
        return true;
    }
return false;

}
void searchForWords(Dictionary& dictionary,char* letters){
dWord *current=dictionary.head;
cout<<"the word that matches are: "<<endl;
while(current!=nullptr){
    if(isStartWithSpecifiedLetters(current->data,letters)){
    cout<<current->data<<endl;
    
    }
    
    current=current->next;
}
    }

void searchForSynAndAntForGivenWord(Dictionary& dictionary,char* letters){
    
    dWord *current=dictionary.head;
while(current!=nullptr){
    if(isStartWithSpecifiedLetters(current->data,letters)){
    cout<<current->data<<endl;

        // Display Synonyms
        sWord* synonym = current->synonym;
        if (synonym != nullptr) {
            cout << "  Synonyms: "<<endl;
            while (synonym != nullptr) {
                cout << synonym->data << " ";
                synonym = synonym->next;
            }
            cout << endl;
        }
        // Display Antonyms
        sWord* antonym = current->antonym;
        if (antonym != nullptr) {
            cout << "  Antonyms: ";
            while (antonym != nullptr) {
                cout << antonym->data << " ";

                antonym = antonym->next;
            }
            cout << endl;
    
    }
    
}
    current=current->next;

    }
}

void DeleteWordThatMatchesSetOfLetters(Dictionary& dictionary, char* letters) {
    dWord* current = dictionary.head;
    
    while (current != nullptr) {
        // Check if the word starts with the specified set of letters
        if (isStartWithSpecifiedLetters(current->data, letters)) {
            dWord* wordToDelete = current;

            // If the word is at the head
            if (current == dictionary.head) {
                dictionary.head = current->next;
                if (current->next != nullptr) {
                    current->next->previous = nullptr;
                }
            }
            // If the word is at the tail
            else if (current == dictionary.tail) {
                dictionary.tail = current->previous;
                current->previous->next = nullptr;
            }
            else {
                current->previous->next = current->next;
                current->next->previous = current->previous;
            }

            delete wordToDelete;
        }
        else {
            current = current->next;
        }
    }
}



//DeleteWordThatMatchesSetOfLetters

 int main() {
    string filename = "Dictionary.txt";
    Dictionary dictionary = parseInputFile(filename);
    cout << "The Original Dictionary is:" << endl;
    // Display original dictionary:
    displayDictionary(dictionary);
    cout << "You can now see the dictionary.txt" << endl;

    cout << "Do you want to have a dictionary without duplicates now? Y/N: ";
    char dup;
    cin >> dup;
    while (dup != 'Y') {
        cout << "Enter again. Do you want to have a dictionary without duplicates now? Y/N: ";
        cin >> dup;
        cin.ignore(); // Add this line
    }

    // Without duplicates:
    string outputFilenameWithoutDup = "DictionaryWithoutDuplicates.txt";
    writeDictionaryToFile(dictionary, outputFilenameWithoutDup);

    // Display without duplicates on console:
    displayDictionary(dictionary);
    cout << "You can now see the DictionaryWithoutDuplicates.txt" << endl;

    // Sort the dictionary:
    cout << "Do you want to sort the dictionary in alphabetical order? Y/N: ";
    char sort;
    cin >> sort;
    while (sort != 'Y') {
        cout << "Enter again. Do you want to sort the dictionary in alphabetical order? Y/N: ";
        cin >> sort;
        cin.ignore(); // Add this line
    }
    sortDictionary(dictionary);

    // Create a sortedDictionary txt that contains the sorted one
    string sortedFilename = "SortedDictionary.txt";
    cout << "Sorted Dictionary:" << endl;
    writeDictionaryToFile(dictionary, sortedFilename);
    displayDictionary(dictionary);
    cout << "You can now see the SortedDictionary.txt" << endl;

    cout << "What word to add to the dictionary:" << endl;
    char newW[30];
    cin.ignore(); // Add this line

    cin.getline(newW, 30);
    insertInCorrectPosition(dictionary, newW);
    cout << "And the dictionary is sorted " << endl;
    sortDictionary(dictionary);

    // Exercise 5:
    cout << "Enter the word you want to add for it a synonym or antonym: " << endl;
    char existingWord[30];
    cin.getline(existingWord, 30);
    cout << "Enter the word that is a synonym or antonym for the word: " << endl;
    char newWord[30];
    cin.getline(newWord, 30);
    addSynonymOrAntonym(dictionary, existingWord, newWord);

    //ex6:
    cout << "Enter some letters as arguments and delete all the words containing these letters in the dictionary: " << endl;
    char letters[30];

    cin.getline(letters, 30);
int MaxStreamSize = 32767;
cin.ignore(MaxStreamSize, '\n');  

    deleteWordsContainingLetters(dictionary, letters);
    cout << "The new dictionary is:" << endl;
    displayDictionary(dictionary);
    cout << "The word has been deleted from the dictionary" << endl;

    string updatedFil = "dictionary_updated.txt";
    writeDictionaryToFile(dictionary, updatedFil);
    cout << "You can now see the dictionary_updated.txt" << endl;



    cout << "Enter the word that searches in the dictionary for all the words starting with a given set of letters:" << endl;
    char SearchWord[30];
    cin.getline(SearchWord, 30);
    searchForWords(dictionary, SearchWord);

    cout << "Enter a word to give you the synonyms and antonyms for the given word: " << endl;
    char SearchSynAnt[30];
    cin.getline(SearchSynAnt, 30);
    searchForSynAndAntForGivenWord(dictionary, SearchSynAnt);

    cout << "Enter the word to delete all the words in the dictionary that start with a given set of letters: " << endl;
    char MatchesLetters[30];
    cin.getline(MatchesLetters, 30);
    DeleteWordThatMatchesSetOfLetters(dictionary, MatchesLetters);
    string updatedFilename = "dictionary_updatedAfterDeletion.txt";
    writeDictionaryToFile(dictionary, updatedFilename);
    cout << "You can now see the dictionary_updatedAfterDeletion.txt" << endl;
displayDictionary(dictionary);
    return 0;
}
