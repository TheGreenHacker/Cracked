#include "provided.h"
#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <cctype>
#include <random>
#include <algorithm>
#include <numeric>
#include <cassert> //testing purposes. Remember to comment out.
#include "MyHash.h"  //testing MyHash purposes only. Remember to comment out after done.
using namespace std;

/*
const string WORDLIST_FILE = "wordlist.txt";

string encrypt(string plaintext)
{
	  // Generate a to z
    char plaintextAlphabet[26+1];
	iota(plaintextAlphabet, plaintextAlphabet+26, 'a');
	plaintextAlphabet[26] = '\0';

	  // Generate permutation
	string ciphertextAlphabet(plaintextAlphabet);
	default_random_engine e((random_device()()));
	shuffle(ciphertextAlphabet.begin(), ciphertextAlphabet.end(), e);

	  // Run translator (opposite to the intended direction)
	Translator t;
	t.pushMapping(plaintextAlphabet, ciphertextAlphabet);
	return t.getTranslation(plaintext);
}

bool decrypt(string ciphertext)
{
	Decrypter d;
	if ( ! d.load(WORDLIST_FILE))
	{
		cout << "Unable to load word list file " << WORDLIST_FILE << endl;
		return false;
	}
	for (const auto& s : d.crack(ciphertext))
		cout << s << endl;
	return true;
}

int main(int argc, char* argv[])
{
	if (argc == 3  &&  argv[1][0] == '-')
	{
		switch (tolower(argv[1][1]))
		{
		  case 'e':
			cout << encrypt(argv[2]) << endl;
			return 0;
		  case 'd':
			if (decrypt(argv[2]))
				return 0;
			return 1;
		}
	}

	cout << "Usage to encrypt:  " << argv[0] << " -e \"Your message here.\"" << endl;
	cout << "Usage to decrypt:  " << argv[0] << " -d \"Uwey tirrboi miyi.\"" << endl;
	return 1;
}
*/

int main()
{
    MyHash<string, int> m_hash;
    string key = "";
    assert(m_hash.getNumItems() == 0);
    assert(m_hash.find("") == nullptr);
    for (int i = 0; i < 51; i++) {
        m_hash.associate(key, i);
        int* valuePtr = m_hash.find(key);
        assert(valuePtr != nullptr);
        assert(*valuePtr == i);
        key += "a";
    }
    assert(m_hash.getNumItems() == 51);
    assert(m_hash.getLoadFactor() == 0.255);
    int* ptr = m_hash.find("aaa");
    assert(ptr != nullptr);
    assert(*ptr == 3);
    int* ptr2 = m_hash.find("aaaaaa");
    assert(ptr2 != nullptr);
    assert(*ptr2 == 6);
    string key2 = "b";
    for (int j = 0; j < 50; j++) {
        m_hash.associate(key2, j);
        int* valuePtr = m_hash.find(key2);
        assert(valuePtr != nullptr);
        assert(*valuePtr == j);
        key2 += "b";
    }
    assert(m_hash.getNumItems() == 101);
    assert(m_hash.getLoadFactor() == 0.2525);
    m_hash.associate("", 999);
    assert(m_hash.getNumItems() == 101);
    int* ptr3 = m_hash.find("");
    assert(ptr3 != nullptr && *ptr3 == 999);
    m_hash.associate("bbb", 875);
    assert(m_hash.getNumItems() == 101);
    int* ptr4 = m_hash.find("bbb");
    assert(ptr4 != nullptr && *ptr4 == 875);
    m_hash.associate("Trump", -32);
    assert(m_hash.getNumItems() == 102);
    assert(m_hash.getLoadFactor() == 0.255);
    m_hash.associate("Kim Jong Un", -98);
    assert(m_hash.getNumItems() == 103);
    assert(m_hash.getLoadFactor() == 0.2575);
    cout << "MyHash works!" << endl;
    return 0;
}
