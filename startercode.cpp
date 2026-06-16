// Mini Search Engine in C++ (Starter Code)

#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <filesystem>
#include <algorithm>
#include <cmath>

namespace fs = std::filesystem;

using namespace std;

// Utility: Tokenizer (very basic)
vector<string> tokenize(const string &text) {
    vector<string> tokens;
    string word;
    for (char ch : text) {
        if (isalnum(ch)) {
            word += tolower(ch);
        } else if (!word.empty()) {
            tokens.push_back(word);
            word.clear();
        }
    }
    if (!word.empty()) tokens.push_back(word);
    return tokens;
}

// Document Manager
class DocumentManager {
public:
    unordered_map<int, string> docPaths;
    unordered_map<int, unordered_map<string, int>> docWordFreqs;
    int totalDocs = 0;

    void loadDocuments(const string &folderPath) {
        int docID = 0;
        for (const auto &entry : fs::directory_iterator(folderPath)) {
            if (entry.path().extension() == ".txt") {
                string path = entry.path().string();
                docPaths[docID] = path;

                ifstream file(path);
                stringstream buffer;
                buffer << file.rdbuf();

                vector<string> words = tokenize(buffer.str());
                for (const string &w : words) {
                    docWordFreqs[docID][w]++;
                }
                docID++;
            }
        }
        totalDocs = docID;
    }
};

// Inverted Index
class InvertedIndex {
public:
    unordered_map<string, unordered_map<int, int>> index;

    void build(DocumentManager &docMgr) {
        for (const auto &[docID, freqs] : docMgr.docWordFreqs) {
            for (const auto &[word, count] : freqs) {
                index[word][docID] = count;
            }
        }
    }

    unordered_set<int> searchAND(const vector<string> &terms) {
        unordered_set<int> result;
        if (terms.empty()) return result;

        // Start with first term
        if (index.count(terms[0]) == 0) return result;
        for (auto &[docID, _] : index[terms[0]]) {
            result.insert(docID);
        }

        for (size_t i = 1; i < terms.size(); ++i) {
            if (index.count(terms[i]) == 0) {
                result.clear();
                return result;
            }
            unordered_set<int> temp;
            for (auto &[docID, _] : index[terms[i]]) {
                if (result.count(docID)) temp.insert(docID);
            }
            result = temp;
        }
        return result;
    }
};

// TF-IDF Ranker
class Ranker {
public:
    static double computeScore(const unordered_map<string, int> &docFreq,
                               const unordered_map<string, unordered_map<int, int>> &index,
                               const vector<string> &queryTerms,
                               int totalDocs) {
        double score = 0.0;
        int docLength = 0;
        for (const auto &[_, freq] : docFreq) docLength += freq;

        for (const string &term : queryTerms) {
            if (docFreq.count(term)) {
                double tf = (double)docFreq.at(term) / docLength;
                double idf = log((double)totalDocs / (1 + index.at(term).size()));
                score += tf * idf;
            }
        }
        return score;
    }
};

int main() {
    DocumentManager docMgr;
    InvertedIndex index;

    string folder;
    cout << "Enter folder path of .txt documents: ";
    getline(cin, folder);

    docMgr.loadDocuments(folder);
    index.build(docMgr);

    string query;
    cout << "\nSearch Engine Ready. Enter search query: ";
    while (getline(cin, query)) {
        vector<string> terms = tokenize(query);
        unordered_set<int> results = index.searchAND(terms);

        vector<pair<double, int>> ranked;
        for (int docID : results) {
            double score = Ranker::computeScore(docMgr.docWordFreqs[docID], index.index, terms, docMgr.totalDocs);
            ranked.push_back({score, docID});
        }

        sort(ranked.rbegin(), ranked.rend());
        cout << "\nTop Results:\n";
        for (auto &[score, docID] : ranked) {
            cout << "[Score: " << score << "] " << docMgr.docPaths[docID] << endl;
        }

        cout << "\nSearch again: ";
    }

    return 0;
}
