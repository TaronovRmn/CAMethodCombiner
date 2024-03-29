// FreqMethod1.cpp: определяет точку входа для консольного приложения.
//
#include "mainwindow.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <QListWidget>
#include <QProgressBar>
namespace fr1 {
using namespace std;

string const SOLS_FILE_NAME = "ouR.txt";
string const TEMP_FILE_NAME = "tmp.txt";

string METHOD_NAME = "[FR1]";

int PRIORITY_METHOD = 10;


struct Letter {
    Letter(char & c, bool b) : letter(c), priority(b) {}
    char letter;
    int count = 1;
    bool priority = false;
};

class LettersSet {
  public:
    bool addLetter(char & c) {
        if (letters_.find(c) == string::npos) {
            letters_ += c;
            return true;
        }
        return false;
    }
    char getLetter() {
        char c;
        //if (letters_.length() != 0) { c = letters_[0]; letters_.erase(letters_.begin()); }    //Last-In-First-Out
        if (letters_.length() != 0) {
            c = letters_.back();    //First-In-First-Out
            letters_.pop_back();
        } else c = '#';
        return c;
    }
    bool removeLetter(char & c) {
        auto const pos = letters_.find(c);
        if (pos != string::npos) {
            letters_.erase(letters_.begin() + pos);
            return true;
        }
        return false;
    }
  private:
    string letters_;
};

class LetterRating {
  public:
    LetterRating(int length) {
        letters_.reserve(length);
    }

    void addLetter(Letter & l) {
        const auto it = find_if(letters_.begin(), letters_.end(),
        [l](const Letter & current) {
            return current.letter == l.letter;
        });
        if (it != letters_.end()) {
            it->count++;
            if (l.priority == true) it->priority = true;
        } else {
            letters_.push_back(l);
        }

    }

    Letter findBestLetter() {
        int c = 0;
        const auto bestLetter = max_element(letters_.begin(), letters_.end(),
        [](const Letter & a, const Letter & b) {
            if (a.count == b.count) {
                if (a.priority) return false;
                if (b.priority) return true;
            }
            return a.count < b.count;
        });
        return Letter(bestLetter->letter,bestLetter->priority);
    }
    int getSize() const {
        return letters_.size();
    }
  private:
    vector<Letter> letters_;
};
class Solution { //solution with: letters, name, index
  public:
    void setLetters(string & s) {
        letters_ = s;
    }

    void setName(string & name) {
        if (name.length() == 6) name_ = name;
        else if (name.length() == 5) name_ = name.insert(0, " ");
    }
    void setIndex(int i) {
        index_ = i;
    }
    void setTreePosition(int i) {
        treePosition_ = i;
    }
    int getSize() const {
        return letters_.length();
    }
    int getIndex() const {
        return index_;
    }
    int getTreePosition() const {
        return treePosition_;
    }
    string getLetters() const {
        return letters_;
    }
    string getName() const {
        return name_;
    }
    void setPriority(bool b) {
        priority_ = b;
    }
    bool getPriority() const {
        return priority_;
    }

    void print(ofstream & fout) const {
        fout << letters_ << '\t' << index_ << '\t' << treePosition_ << '\t' << name_ << endl;
    }
    void read(ifstream & fin) {
        fin >> letters_;
        fin >> index_;
        priority_ = index_ == PRIORITY_METHOD;
        fin >> treePosition_;
        string name;
        fin >> name;
        this->setName(name);
    }
  private:
    string letters_;
    int index_ = 0;
    int treePosition_ = 0;
    string name_;
    bool priority_ = false;
};

class Fragment {
  public:
    Fragment( int & k) : numberOfMethods_(k) {
        solutions_.reserve(k);
    }
    void read(ifstream & fin) {
        if (numberOfMethods_ == 0) {
            cout << "trying to input zero methods";
            return;
        }
        int i;
        string s;
        fin >> no_;
        for (int iS = 0; iS < numberOfMethods_; iS++) {
            Solution sol;
            sol.read( fin );
            solutions_.push_back(sol);
        }

    }
    void print(ofstream & fout) {
        fout << no_ << endl;
        for (auto & i : solutions_) {
            i.print(fout);
        }
    }
    vector<Letter> getColumn(int & index) {
        vector<Letter> lv;
        lv.reserve(numberOfMethods_);
        for (auto & s : solutions_) {
            lv.push_back(Letter(s.getLetters()[index], s.getPriority()));
        }
        return lv;

    }
    vector<Solution> getSolutions() const {
        return solutions_;
    }
    void findBestSolution() {
        Solution bestSolution;
        string bsol;
        LettersSet candidates;
        for (int iLetter = 0; iLetter < solutions_[0].getSize(); iLetter++) {
            LetterRating lr(numberOfMethods_);
            vector<Letter> column = getColumn(iLetter);
            char bl;



            for (auto & l : column) {
                if (bsol.find(l.letter) == string::npos) {
                    candidates.addLetter(l.letter);
                    lr.addLetter(l);
                }

            }
            if (lr.getSize() != 0) {
                bl = lr.findBestLetter().letter;
            } else bl = candidates.getLetter();

            candidates.removeLetter(bl);
            bsol += bl;
        }
        //reverse(bsol.begin(), bsol.end()); //for reverse bypass
        bestSolution.setLetters(bsol);
        bestSolution.setIndex(numberOfMethods_ + 1);
        bestSolution.setTreePosition(0);
        bestSolution.setName(METHOD_NAME);


        solutions_.push_back(bestSolution);
    }
    void findBestSolutionV2() {
        Solution bestSolution;
        string bsol;
        LettersSet candidates;
        for (int iLetter = 0; iLetter < solutions_[0].getSize(); iLetter++) {
            LetterRating lr(numberOfMethods_);
            vector<Letter> column = getColumn(iLetter);
            char bl;



            for (auto & l : column) {
                if (bsol.find(l.letter) == string::npos) {
                    candidates.addLetter(l.letter);
                    lr.addLetter(l);
                }

            }
            if (lr.getSize() != 0) {
                bl = lr.findBestLetter().letter;
            } else {
                bl = '?';

            }

            candidates.removeLetter(bl);
            bsol += bl;
        }
        replace(bsol.begin(), bsol.end(), '?', candidates.getLetter());

        bestSolution.setLetters(bsol);
        bestSolution.setIndex(numberOfMethods_ + 1);
        bestSolution.setTreePosition(0);
        bestSolution.setName(METHOD_NAME);


        solutions_.push_back(bestSolution);
    }
  private:
    int numberOfMethods_ = 0;
    string no_;
    vector<Solution> solutions_;
};

void copy_file(ifstream &f, ofstream &t) {
    string const s((istreambuf_iterator<char>(f)),
                   istreambuf_iterator<char>());
    t << s;
}

int freqMethod1(int priorityMethod, QProgressBar * pb) {
    setlocale(LC_ALL, "rus");
    PRIORITY_METHOD = priorityMethod;

    ifstream fin;
    ofstream fout;
    fin.open(SOLS_FILE_NAME, ios_base::in);
    if (!fin.is_open())
        return 1;

    int nFrags, nMethods;

    fin >> nFrags >> nMethods;

    pb->setRange(0,nFrags-1);
    vector<Fragment> fragments;
    fragments.reserve(nFrags);
    for (int iF = 0; iF < nFrags; iF++) {
        pb->setValue(iF);
        Fragment frag(nMethods);
        frag.read(fin);
        frag.findBestSolutionV2();
        fragments.push_back(frag);
    }

    fout.open(TEMP_FILE_NAME, ios_base::out);
    fout << nFrags << " " << nMethods + 1 << endl;
    for (auto & f : fragments) {
        f.print(fout);
    }
    fin.close();
    fout.close();
    ifstream fint(TEMP_FILE_NAME);
    if (!fint.is_open())
        return 1;
    ofstream foutt(SOLS_FILE_NAME);

    copy_file(fint, foutt);

    foutt.close();
    fint.close();
    remove(TEMP_FILE_NAME.c_str());

    return 0;
}
QStringList readMethods() {
    ifstream fin;
    QStringList names;
    fin.open(SOLS_FILE_NAME, ios_base::in);
    if (!fin.is_open())
        return names;

    int nFrags, nMethods;

    fin >> nFrags >> nMethods;


    Fragment frag(nMethods);
    frag.read(fin);
    for (auto & sol : frag.getSolutions()) {
        string fullName;
        if (sol.getTreePosition() == 0)
            fullName = to_string(sol.getIndex()) + " : " + sol.getName();
        else {
            fullName = to_string(sol.getIndex()) + " : " +to_string(sol.getTreePosition()) + sol.getName();
        }
        names.append(QString::fromStdString(fullName));
    }

    fin.close();
    return names;
}
}

