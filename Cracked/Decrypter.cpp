#include "provided.h"
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

class DecrypterImpl
{
public:
    DecrypterImpl();
    bool load(string filename);
    vector<string> crack(const string& ciphertext);
private:
    WordList m_wl;
    Tokenizer m_tn;
    Translator m_ts;
    int unTranslated(string s) const;
    string mostUntranslated(vector<string>& words) const;
    bool completelyTranslated(vector<string>& cipherwords) const;
    bool containsBadWord(string oldTranslation, string newTranslation) const;
    bool isCompleteTranslation(string s) const;
    bool notInList(string s) const;
    void crackHelper(string ciphertext, vector<string>& output);
};

//Public Method Implementations
DecrypterImpl::DecrypterImpl(): m_tn(" 0123456789,;:.!()[]{}-\"#$%^&")
{
    
}

bool DecrypterImpl::load(string filename)
{
    return m_wl.loadWordList(filename);
}

vector<string> DecrypterImpl::crack(const string& ciphertext)
{
    vector<string> decryptions;
    crackHelper(ciphertext, decryptions);
    sort(decryptions.begin(), decryptions.end());
    return decryptions;
}

//Private Implementations
int DecrypterImpl::unTranslated(string s) const
{
    int unknowns = 0;
    string translation = m_ts.getTranslation(s);
    for (int i = 0; i < translation.length(); i++) {
        if (translation[i] == '?') {
            unknowns++;
        }
    }
    return unknowns;
}

string DecrypterImpl::mostUntranslated(vector<string>& words) const
{
    string mostUntranslated = "";
    for (int i = 0; i < words.size(); i++) {
        if (unTranslated(words[i]) > unTranslated(mostUntranslated) /*&& visited.find(words[i]) == visited.end()*/) {
            mostUntranslated = words[i];
        }
    }
    return mostUntranslated; //if an empty string is returned, then that means we are out of options
}

bool DecrypterImpl::containsBadWord(string oldTranslation, string newTranslation)const
{
    if (oldTranslation.length() != newTranslation.length()) {
        return true;
    }
    for (int i = 0; i < oldTranslation.length(); i++) {
        if ((isalpha(oldTranslation[i]) && isalpha(newTranslation[i])) && (oldTranslation[i] != newTranslation[i])) {
            return true;
        }
    }
    return false;
}

bool DecrypterImpl::isCompleteTranslation(string s)const
{
    for (int i = 0; i < s.length(); i++) {
        if (s[i] == '?') {
            return false;
        }
    }
    return true;
}

bool DecrypterImpl::notInList(string s)const
{
    vector<string> words;
    string temp = "";
    for (int i = 0; i < s.length(); i++) {
        if (isalpha(s[i]) || s[i] == '?' || s[i] == '\'') {
            temp += s[i];
        }
        else if (!temp.empty()){
            if (isCompleteTranslation(temp)) {
                words.push_back(temp);
            }
            temp = "";
        }
    }
    if (!temp.empty() && isCompleteTranslation(temp)) {
        words.push_back(temp);
    }
    for (int i = 0; i < words.size(); i++){
        if (!m_wl.contains(words[i])) {
            return true;
        }
    }
    return false;
}

void DecrypterImpl::crackHelper(string ciphertext, vector<string>& output)
{
    vector<string> words = m_tn.tokenize(ciphertext); //tokenize ciphertext
    string mostUnknown = mostUntranslated(words); //get the word with the least known characters in its translations
    string currTranslation = m_ts.getTranslation(mostUnknown); //get current translation for chosen word
    vector<string> candidates = m_wl.findCandidates(mostUnknown, currTranslation); //obtain all candidates that can possibly match chosen word and compare each one with the word's current translation.
    for (int i = 0; i < candidates.size(); i++) {  //for each candidate
        string oldTranslation = m_ts.getTranslation(ciphertext);
        if (m_ts.pushMapping(mostUnknown, candidates[i])) {  //if progress was made
            string newTranslation = m_ts.getTranslation(ciphertext); //translate rest of text
            if (containsBadWord(oldTranslation, newTranslation) || notInList(newTranslation)) {
                m_ts.popMapping();
            }
            else if (isCompleteTranslation(newTranslation)){
                output.push_back(newTranslation);
                m_ts.popMapping();
            }
            else {
                crackHelper(ciphertext, output);
            }
        }
    }
    m_ts.popMapping();
}



//******************** Decrypter functions ************************************

// These functions simply delegate to DecrypterImpl's functions.
// You probably don't want to change any of this code.

Decrypter::Decrypter()
{
    m_impl = new DecrypterImpl;
}

Decrypter::~Decrypter()
{
    delete m_impl;
}

bool Decrypter::load(string filename)
{
    return m_impl->load(filename);
}

vector<string> Decrypter::crack(const string& ciphertext)
{
    return m_impl->crack(ciphertext);
}
