#include "provided.h"
#include <string>
#include <vector>
#include <cctype>
#include <unordered_map>
using namespace std;

class TranslatorImpl
{
public:
    TranslatorImpl();
    bool pushMapping(string ciphertext, string plaintext);
    bool popMapping();
    string getTranslation(const string& ciphertext) const;
private:
    unordered_map<char, char> m_defaultMap;   //empty mapping table
    unordered_map<char, char> m_currentMap;   //current mapping table
    vector<unordered_map<char, char>> m_mapStack;  //stack of mappings
    //helper functions
    string removeApostrophes(string s) const;
    bool containsNonLetter(string text) const;
    bool inconsistentMapping(string s, string t) const;
    bool inconsistentMapping(const unordered_map<char, char>& map, string s, string t) const;
};

//Public Member Function Implementations
TranslatorImpl::TranslatorImpl()
{
    for (int i = 0; i < 26; i++) {
        char c = i + 'A';
        m_defaultMap.insert({c, '?'});
    }
    m_currentMap = m_defaultMap;
}

bool TranslatorImpl::pushMapping(string ciphertext, string plaintext)
{
    ciphertext = removeApostrophes(ciphertext);
    plaintext = removeApostrophes(plaintext);
    if (ciphertext.length() != plaintext.length() || containsNonLetter(ciphertext) || containsNonLetter(plaintext) || inconsistentMapping(ciphertext, plaintext)) { //beforehand error checking
        return false;
    }
    unordered_map<char, char> newMap;
    if (m_mapStack.empty()) {
        for (int i = 0; i < ciphertext.length(); i++) {
            newMap.insert({toupper(ciphertext[i]), toupper(plaintext[i])});
        }
    }
    else{
        newMap = m_mapStack[m_mapStack.size() - 1];
        if (inconsistentMapping(newMap, ciphertext, plaintext)) {
            return false;
        }
        for (int i = 0; i < ciphertext.length(); i++) {
            newMap.insert({toupper(ciphertext[i]), toupper(plaintext[i])});
        }
    }
    m_currentMap = newMap;
    m_mapStack.push_back(newMap);
    return true;
}

bool TranslatorImpl::popMapping()
{
    if (!m_mapStack.empty()) {
        m_mapStack.pop_back();
        if (!m_mapStack.empty()) {
            m_currentMap = m_mapStack[m_mapStack.size() - 1];
        }
        else{
            m_currentMap = m_defaultMap;
        }
        return true;
    }
    return false;
}

string TranslatorImpl::getTranslation(const string& ciphertext) const
{
    string plaintext = "";
    for (int i = 0; i < ciphertext.length(); i++) {
        if (isalpha(ciphertext[i])) {
            unordered_map<char, char>::const_iterator find = m_currentMap.find(toupper(ciphertext[i]));
            if (find != m_currentMap.end()) {
                if (islower(ciphertext[i])) {   //be careful of case sensitivity
                    plaintext += tolower(find->second);
                }
                else{
                    plaintext += find->second;
                }
            }
            else{
                plaintext += '?';
            }
        }
        else{
            plaintext += ciphertext[i];
        }
    }
    return plaintext;
}

//Private Methods
string TranslatorImpl::removeApostrophes(string s) const
{
    string str = "";
    for (int i = 0; i < s.length(); i++) {
        if (s[i] != '\'') {
            str += s[i];
        }
    }
    return str;
}

bool TranslatorImpl::containsNonLetter(string text) const
{
    for (int i = 0; i < text.length(); i++) {
        if (!isalpha(text[i])) {
            return true;
        }
    }
    return false;
}

bool TranslatorImpl::inconsistentMapping(string s, string t) const //preprocess error checking
{
    unordered_map<char, char> map;
    for (int i = 0; i < s.length(); i++) {  //check to see if 1 cipertext letter maps to 2 or more different plaintext letters
        unordered_map<char, char>::iterator find = map.find(toupper(s[i]));
        if (find != map.end()) {
            if (find->second != toupper(t[i])) {
                return true;
            }
        }
        else{
            map.insert({toupper(s[i]), toupper(t[i])});
        }
    }
    map.clear();  //check the other way: if 2 or more ciphertext letters map to the same plaintext letter
    for (int j = 0; j < t.length(); j++) {
        unordered_map<char, char>::iterator find = map.find(toupper(t[j]));
        if (find != map.end()) {
            if (find->second != toupper(s[j])) {
                return true;
            }
        }
        else{
            map.insert({toupper(t[j]), toupper(s[j])});
        }
    }
    return false;
}

bool TranslatorImpl::inconsistentMapping(const unordered_map<char, char>& map, string s, string t) const//check to see if implied mapping based on s and t would conflict with a prior mapping in current map table
{
    for (int i = 0; i < s.length(); i++) {  //check to see if same ciphertext letter map would map to 2 or or more different plaintext letters
        unordered_map<char, char>::const_iterator find = map.find(toupper(s[i]));
        if (find != map.end() && find->second != toupper(t[i])) {
            return true;
        }
    }
    for (int j = 0; j < t.length(); j++) { //check to see if adding the new mapping would cause 2 or more ciphertext letters to match to the same plain text letter
        for (unordered_map<char, char>::const_iterator it = map.begin(); it != map.end(); it++) {
            if (toupper(t[j]) == it->second && toupper(s[j]) != it->first) {
                return true;
            }
        }
    }
    return false;
}

//******************** Translator functions ************************************

// These functions simply delegate to TranslatorImpl's functions.
// You probably don't want to change any of this code.

Translator::Translator()
{
    m_impl = new TranslatorImpl;
}

Translator::~Translator()
{
    delete m_impl;
}

bool Translator::pushMapping(string ciphertext, string plaintext)
{
    return m_impl->pushMapping(ciphertext, plaintext);
}

bool Translator::popMapping()
{
    return m_impl->popMapping();
}

string Translator::getTranslation(const string& ciphertext) const
{
    return m_impl->getTranslation(ciphertext);
}
