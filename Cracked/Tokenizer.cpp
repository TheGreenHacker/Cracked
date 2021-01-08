#include "provided.h"
#include <string>
#include <vector>
#include <unordered_set>
using namespace std;

class TokenizerImpl
{
public:
    TokenizerImpl(string separators);
    vector<string> tokenize(const std::string& s) const;
private:
    unordered_set<char> m_separators; //unordered set is O(1) for insertions and searching
};

TokenizerImpl::TokenizerImpl(string separators)
{
    for (int i = 0; i < separators.length(); i++) {
        m_separators.insert(separators[i]); //convert each character in the string to a string
    }
}

vector<string> TokenizerImpl::tokenize(const std::string& s) const
{
    vector<string> tokenized;
    string currentString = "";
    for (int i = 0; i < s.length(); i++) {
        if (m_separators.find(s[i]) != m_separators.end()) {
            if (!currentString.empty()) {
                tokenized.push_back(currentString);
            }
            currentString = "";
        }
        else{
            currentString += s[i];
        }
    }
    if (!currentString.empty()){
        tokenized.push_back(currentString);
    }
    return tokenized;
}

//******************** Tokenizer functions ************************************

// These functions simply delegate to TokenizerImpl's functions.
// You probably don't want to change any of this code.

Tokenizer::Tokenizer(string separators)
{
    m_impl = new TokenizerImpl(separators);
}

Tokenizer::~Tokenizer()
{
    delete m_impl;
}

vector<string> Tokenizer::tokenize(const std::string& s) const
{
    return m_impl->tokenize(s);
}
