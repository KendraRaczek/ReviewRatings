////////////////////////////////////////////////////////////////////////////////
// File Name:      a3.cpp
//
// Author:         Kendra Raczek
// CS email:       raczek@cs.wisc.edu
//
// Description:    The source file for a3.
//
// IMPORTANT NOTE: THIS IS THE ONLY FILE THAT YOU SHOULD MODIFY FOR A3.
//                 You SHOULD NOT MODIFY any of the following:
//                   1. Name of the functions.
//                   2. The number and type of parameters of the functions.
//                   3. Return type of the functions.
//                   4. Import statements.
//
// Sources:        GeeksforGeeks
//
// URL(s) of sources:
//                 http://www.geeksforgeeks.org/
////////////////////////////////////////////////////////////////////////////////

#include "a3.hpp"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <fstream>

/**
 * @brief Cleans data file by remving common words from input file and outputs
 * result to a file
 *
 * @param inFile Input file
 * @param outFile Output file
 * @param commonWords Set of common words
 */
void cleanData(std::istream &inFile, std::ostream &outFile,
               std::unordered_set<std::string> &commonWords) {

    for( std::string line; getline( inFile, line ); ) {
        std::vector<std::string> wordsInLine;
        splitLine(line, wordsInLine);
        removeCommonWords(wordsInLine,commonWords);

        for (auto itr = wordsInLine.begin(); itr != wordsInLine.end();
             ++itr) {

            if (itr == wordsInLine.end() - 1){
                outFile << *itr << std::endl;
            }
            else {
                outFile << *itr << " ";
            }
        }
    }
}

/**
 * @brief Extracts information from given file and creates a dictionary, whose
 * keys are words and value is a pair representing total rating and total count
 *
 * @param apples newInFile Input file to be read from
 * @param dict Unordered set that contains each word and its associated value
 * pair
 */
void fillDictionary(std::istream &newInFile,
                    std::unordered_map<std::string, std::pair<long, long>> &dict) {
    // list of review rating words that will be added to dictionary
    std::vector<std::string> cleanReviewRatings;
    for( std::string line; getline( newInFile, line ); ) {
        splitLine(line, cleanReviewRatings);
    }
    long currentRating;
    // iterate through vector and add to map
    for (auto itr = cleanReviewRatings.begin(); itr != cleanReviewRatings.end();
         ++itr) {
        if ( ((*itr) == "0") ||
             ((*itr) == "1") ||
             ((*itr) == "2") ||
             ((*itr) == "3") ||
             ((*itr) == "4")) {

            long i = std::stol(*itr);
            currentRating = i;
        }
        else {
            // if word hasn't been added to dictionary
            if (dict.count(*itr) == 0) {
                dict.emplace(*itr, (std::make_pair(currentRating, 1)));
            }
            // if word has already been added to dictionary
            else {
                // update total rating and total count
                dict[*itr].first += currentRating;
                dict[*itr].second += 1;
            }
        }
    }
}

/**
 * @brief Extracts common words from the file of common words and adds them to
 * an unordered set
 *
 * @param inFile File that holds common words
 * @param commonwords Unordered set that will contain common words
 */
void fillCommonWords(std::istream &inFile,
                     std::unordered_set<std::string> &commonWords) {
    for( std::string line; getline( inFile, line ); ) {
        commonWords.insert(line);
    }
}

/**
 * @brief Combines information of each word's rating in a review and evaluates a
 * rating for each review as a whole based on the total rating and total count
 * of each word
 *
 * @param testFile Input file of reviews
 * @param dict Unordered set that contains each word and its associated total
 * rating and total count
 * @param ratingsFile Output file to write final ratings of each review
 */
void rateReviews(std::istream &testFile,
                 std::unordered_map<std::string, std::pair<long, long>> &dict,
                 std::ostream &ratingsFile) {

    std::ifstream commonWordsFile("commonwords.txt");
    std::unordered_set<std::string> commonWords;
    fillCommonWords(commonWordsFile, commonWords);

    // ratings of each review line
    std::vector<double> reviewRatings;
    for( std::string line; getline( testFile, line ); ) {
        std::vector<std::string> wordsInLine;
        splitLine(line, wordsInLine);

        // rating for a single word
        double wordRating;
        // ratings for each word in a line
        std::vector<double> wordRatings;
        // iterate through vector
        for (auto itr = wordsInLine.begin(); itr != wordsInLine.end(); ++itr) {
            if (dict.count(*itr) == 1) {
                wordRating = ((double)((dict[(*itr)]).first) / (double)((dict[(*itr)]).second));
            }
            else {
                wordRating = 2.00;
            }
            wordRatings.push_back(wordRating);
        }

        // rating of a single review line
        double reviewRating = 0.0;
        // calculate rating for review line
        for (auto itr = wordRatings.begin(); itr != wordRatings.end(); ++itr) {
            reviewRating += *itr;
        }
        reviewRating = reviewRating / wordRatings.size();
        reviewRatings.push_back(reviewRating);
    }
    // write to ratings output
    for (auto itr = reviewRatings.begin(); itr != reviewRatings.end(); ++itr) {
        ratingsFile << std::fixed;
        ratingsFile << std::setprecision(2) << *itr << std:: endl;
    }
}

/**
 * @brief Scans through vector, removing any instances of a common word
 *
 * @param tokens Input list of words to be searched through
 * @param commonwords Unordered set of common words that should be removed from
 * list
 */
void removeCommonWords(std::vector<std::string> &tokens,
                       std::unordered_set<std::string> &commonWords) {

    for (auto itr = commonWords.begin(); itr != commonWords.end(); ++itr) {
        tokens.erase(std::remove(tokens.begin(), tokens.end(), *itr),
                       tokens.end());
    }
}

/**
 * @brief splits string line into a vector list of individual words, skipping
 * whitespace
 *
 * @param line The string line of words to be split
 * @param words Vector list of words after they have been split individually
 */
void splitLine(std::string &line, std::vector<std::string> &words) {
    // This code is provided for you and is correct.
    // You should NOT make any changes to this function!
    std::stringstream ss;
    ss.str(line);
    std::string token;
    while (std::getline(ss, token, ' ')) {
        if (token.length() != 0) {
            words.push_back(token);
        }
    }
}