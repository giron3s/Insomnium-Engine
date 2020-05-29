/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2019 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *******************************************************************************/

#include "precompiled.h"
#include "engine.h"
#include "languagemanager.h"

#include <sstream>
#include <fstream>
#include <codecvt>

namespace Framework
{
    LanguageManager::LanguageManager()
        : BaseManager(Utils::Demangling(typeid(LanguageManager).name()))
    {

    }

    LanguageManager::~LanguageManager()
    {
        
    }

    void LanguageManager::Initialize()
    {
        //Load the language file
        ifstream lIfStream(Engine::Instance()->Config().GetLanguageFile());
        ASSERT(lIfStream);

        VocabStream lVocabData;
        lVocabData << lIfStream.rdbuf();

        //  Separate each line and save them into a vector for further parsing
        VocabString lTempString;
        vector<VocabString> lLines;
        while (getline(lVocabData, lTempString, '\n'))
            lLines.push_back(lTempString);

        //  First line is for language IDs. Parse language tags and create vocabularies for each language.
       
        vector<VocabString> lLanguages = Tokenize(lLines.at(0), '\t');
        
        vector<VocabString>::iterator lLanguagesIter = lLanguages.begin();
        lLanguagesIter++; // Ignore first entry because it is always ID and does not represent a language.
        while (lLanguagesIter != lLanguages.end())
        {
            shared_ptr<Vocabularies> lVocab = make_shared<Vocabularies>();
            lVocab->mLanguage = *lLanguagesIter;
            mLanguageVocabularies[*lLanguagesIter] = lVocab;
            lLanguagesIter++;
        }

        //  Parse vocabulary IDs and all language words for that ID
        vector<VocabString>::iterator lLinesIter = lLines.begin();
        lLinesIter++; // Ignore first line because it contains languages which have already been parsed.
        while (lLinesIter != lLines.end())
        {
            vector<VocabString> lEntries = Tokenize(*lLinesIter, '\t');
            vector<VocabString>::iterator lEntriesIter = lEntries.begin();
            int lEntryIndex = 1;
            VocabString lID = *lEntriesIter;
            lEntriesIter++;

            while (lEntriesIter != lEntries.end())
            {
                mLanguageVocabularies[lLanguages.at(lEntryIndex)]->mVocabularies[lID] = *lEntriesIter;
                lEntriesIter++;
                lEntryIndex++;
            }

            lLinesIter++;
        }

        lIfStream.close();
        SetLanguage("DEFAULT"); 
    }

    void LanguageManager::DeInitialize()
    {
        mLanguageVocabularies.clear();
    }

    vector<VocabString> LanguageManager::Tokenize(VocabString aString, char aDelimiter)
    {
        VocabStream lVocabStream(aString);
        VocabString lTempString;
        vector<VocabString> lTokens;

        while (getline(lVocabStream, lTempString, aDelimiter))
            lTokens.push_back(lTempString);

        return lTokens;
    }

    void LanguageManager::SetLanguage(Language aLanguage)
    {
        map<Language, shared_ptr<Vocabularies>>::iterator lVocab = mLanguageVocabularies.find(aLanguage);
        ASSERT(lVocab != mLanguageVocabularies.end())
        mActiveVocabularies = lVocab->second;
    }

    //  Checks the format of VocabularyID. ID must follow the rules below
    //  .. Start with @
    //  .. Be all upper-case
    //  .. Contain only alpha-numeric characters
    //  .. Underscore is the only allowed special character
    bool LanguageManager::CheckID(VolabularyID aID)
    {
        string::const_iterator s = aID.begin();

        //The ID has to start with @
        ASSERT(*s == '@'); 
        if (*s != '@')
            return false;

        while (++s != aID.end())
        {
            if (*s == '_')
                continue;

            ASSERT(isalnum(*s))  //Checks whether s is either a decimal digit or an uppercase or lowercase letter.
            if (!isalnum(*s))
                return false;

            ASSERT(!isalpha(*s) || isupper(*s))  //Checks if parameter c is an uppercase alphabetic letter
            if (isalpha(*s) && !isupper(*s))
                return false;
        }
        return true;
    }

    VolabularyText LanguageManager::GetString(VolabularyID aID) 
    {
        if (aID.empty())
            return "";

        if(!CheckID(aID))
            return aID;

        shared_ptr<Vocabularies> lActiveVocabularies = mActiveVocabularies.lock();

        //No active vocabularies
        ASSERT(lActiveVocabularies != nullptr); 
        if (lActiveVocabularies == nullptr)
            return aID;

        map<VolabularyID, VolabularyText>::iterator lValue = lActiveVocabularies->mVocabularies.find(aID);

        //Not find the ID on the vocabulary
        ASSERT(lValue != lActiveVocabularies->mVocabularies.end()); 
        if (lValue == lActiveVocabularies->mVocabularies.end())
            return aID;

        return lValue->second;
    }
}