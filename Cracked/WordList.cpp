//
//  WordList.cpp
//  testWordList
//
//  Created by Jack Li on 3/11/18.
//  Copyright © 2018 Jack Li. All rights reserved.
//
#include "provided.h"
#include <string>
#include <vector>
#include <functional>
#include "MyHash.h"
#include <iostream>
#include <fstream>
#include <cctype>
using namespace std;

class WordListImpl
{
public:
    bool loadWordList(string filename);
    bool contains(string word) const;
    vector<string> findCandidates(string cipherWord, string currTranslation) const;
private:
    MyHash<string, vector<string>> m_wordList;
    //private helper functions
    bool shouldIgnore(string file) const;
    string allCaps(string s) const;
    string generateWordPattern(string s) const;
    bool invalidCipherWord(string s) const;
    bool invalidCurrTranslation(string s) const;
    bool invalidCorrespondingCharacters(string s, string t) const;
};

//Public Member Functions
bool WordListImpl::loadWordList(string filename) //group words in file by letter pattern
{
    m_wordList.reset(); //start fresh
    ifstream infile(filename);
    if (!infile) {  //failed to open file
        cerr << "Error! Cannot open " << filename << endl;
        return false;
    }
    string s;
    while (getline(infile, s)) {  //load the data by adding it to MyHash member
        if (!shouldIgnore(s)) {
            string S = allCaps(s);   //case insensitivity
            string pattern = generateWordPattern(S);
            vector<string>* words = m_wordList.find(pattern);
            if (words == nullptr) {   //new pattern
                m_wordList.associate(pattern, vector<string>(1, S));
            }
            else{
                words->push_back(S);   //pattern already exists
            }
        }
    }
    return true;
}

bool WordListImpl::contains(string word) const
{
    string WORD = allCaps(word);  //case insensitivty
    string pattern = generateWordPattern(WORD);
    const vector<string>* words = m_wordList.find(pattern);
    if (words != nullptr) {
        const vector<string> list = *(words);
        for (int i = 0; i < list.size(); i++) {
            if (list[i] == WORD) {
                return true;
            }
        }
    }
    return false;
}

vector<string> WordListImpl::findCandidates(string cipherWord, string currTranslation) const
{
    vector<string> candidates;
    if (cipherWord.length() != currTranslation.length() || invalidCipherWord(cipherWord) || invalidCurrTranslation(currTranslation) || invalidCorrespondingCharacters(cipherWord, currTranslation)) {
        return candidates;
    }
    string CIPHERWORD = allCaps(cipherWord);
    string CURRTRANSLATION = allCaps(currTranslation);
    string pattern = generateWordPattern(CIPHERWORD);
    const vector<string>* words = m_wordList.find(pattern);
    if (words != nullptr) {
        const vector<string> list = *words;
        for (int i = 0; i < list.size(); i++) {
            bool push = true;
            for (int j = 0; j < list[i].length(); j++) {
                if ((isalpha(CURRTRANSLATION[j]) && CURRTRANSLATION[j] != list[i][j]) || (CURRTRANSLATION[j] == '?' && !isalpha(list[i][j])) || (CURRTRANSLATION[j] == '\'' && list[i][j] != '\'')) {
                    push = false;
                }
            }
            if (push) {
                candidates.push_back(list[i]);
            }
        }
    }
    return candidates;
}

//Private Functions
bool WordListImpl::shouldIgnore(string file) const //ignore bad strings from input file
{
    for (int i = 0; i < file.length(); i++) {
        if (!isalpha(file[i]) && file[i] != '\'') {
            return true;
        }
    }
    return false;
}

string WordListImpl::allCaps(string s) const
{
    string S = s;
    for (int i = 0; i < S.length(); i++) {
        S[i] = toupper(S[i]);
    }
    return S;
}

string WordListImpl::generateWordPattern(string s) const //associate each word in the word list with a pattern
{
    MyHash<char, char> generator;
    char c = 'A';
    string pattern = "";
    for (int i = 0; i < s.length(); i++) {
        if (isalpha(s[i])) {
            char* ch = generator.find(toupper(s[i])); //case insensitivity
            if (ch == nullptr) {
                pattern += c;
                generator.associate(toupper(s[i]), c); //case insensitivity
                c++;
            }
            else{
                pattern += *ch;
            }
        }
        else if (s[i] == '\''){ //single quote
            pattern += s[i];
            generator.associate(s[i], s[i]);
        }
    }
    return pattern;
}

bool WordListImpl::invalidCipherWord(string s) const
{
    for (int i = 0; i < s.length(); i++) {
        if (!isalpha(s[i]) && s[i] != '\'') {
            return true;
        }
    }
    return false;
}

bool WordListImpl::invalidCurrTranslation(string s) const
{
    for (int i = 0; i < s.length(); i++) {
        if (!isalpha(s[i]) && s[i] != '\'' && s[i] != '?') {
            return true;
        }
    }
    return false;
}

bool WordListImpl::invalidCorrespondingCharacters(string s, string t) const
{
    for (int i = 0; i < t.length(); i++) {
        if ((isalpha(t[i]) && !isalpha(s[i])) || (t[i] == '?' && !isalpha(s[i])) || (t[i] == '\'' && s[i] != '\'')) {
            return true;
        }
    }
    return false;
}

//***** hash functions for string, int, and char *****
unsigned int hash(const std::string& s)
{
    return std::hash<std::string>()(s);
}

unsigned int hash(const int& i)
{
    return std::hash<int>()(i);
}

unsigned int hash(const char& c)
{
    return std::hash<char>()(c);
}

//******************** WordList functions ************************************
// These functions simply delegate to WordListImpl's functions.
// You probably don't want to change any of this code.

WordList::WordList()
{
    m_impl = new WordListImpl;
}

WordList::~WordList()
{
    delete m_impl;
}

bool WordList::loadWordList(string filename)
{
    return m_impl->loadWordList(filename);
}

bool WordList::contains(string word) const
{
    return m_impl->contains(word);
}

vector<string> WordList::findCandidates(string cipherWord, string currTranslation) const
{
    return m_impl->findCandidates(cipherWord, currTranslation);
}


