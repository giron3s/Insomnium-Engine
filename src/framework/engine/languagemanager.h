/*******************************************************************************
 *  Author      : giron3s
 *  Copyright   : Copyright (C) 2019 Marc Girones Dezsenyi - All Rights Reserved
 *                Unauthorized copying of this file, via any medium is strictly prohibited
 *                Proprietary and confidential
 *******************************************************************************/


#pragma once
#include <vector>
#include "core/utils.h"
#include "basemanager.h"

typedef std::string VocabString;
typedef std::stringstream VocabStream;

typedef std::string Language;
typedef std::string VolabularyID;
typedef std::string VolabularyText;

namespace Framework
{
    class LanguageManager : public Framework::BaseManager
    {
    public:

        struct Vocabularies
        {
            Language mLanguage;
            std::map<VolabularyID, VolabularyText> mVocabularies;
        };

        LanguageManager();
        ~LanguageManager() override;

        void                        Initialize() override;
        void                        DeInitialize() override;

        std::vector<VocabString>    Tokenize(VocabString aString, char aDelimiter);
        void                        SetLanguage(Language aLanguage);

        bool                        CheckID(VolabularyID aID);
        VolabularyText              GetString(VolabularyID aID);

    private:

        std::map<Language, std::shared_ptr<Vocabularies>>   mLanguageVocabularies;
        std::weak_ptr<Vocabularies>                         mActiveVocabularies;
    };
}