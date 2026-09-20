# In-Memory-C-Search-Engine-Mini-Google-
# 🧠 Mini C++ Search Engine

A lightweight, in-memory search engine built in C++ that tokenizes and indexes `.txt` documents, processes search queries using Boolean AND logic, and ranks results using TF-IDF scoring.

---

## 📁 Features

* Document loading and preprocessing from a folder
* Tokenization and case normalization
* Inverted index generation
* AND-based query processing
* TF-IDF based relevance ranking
* Command-line interface for user interaction

---

## 🛠️ Getting Started

### 🔧 Prerequisites

* C++17 compiler (e.g., `g++`, `clang++`)
* A directory with `.txt` files for indexing

### 🧪 Compile

```bash
g++ -std=c++17 search_engine.cpp -o search_engine
```

### ▶️ Run

```bash
./search_engine
```

### 🗂️ Example Input

When prompted:

```
Enter folder path of .txt documents: ./data
Search Engine Ready. Enter search query: artificial intelligence
```

---

## 📄 Project Structure

```
📦 mini-search-engine
├── search_engine.cpp   # Main source code
├── README.md           # This file
└── data/               # Folder containing .txt documents
```

---

## 🚀 Features to Add (Future Work)

* OR / NOT / phrase queries
* Fuzzy search (Levenshtein distance)
* Multi-threaded indexing
* BM25 ranking
* Web frontend with REST API (using cpp-httplib)

---
