#include "mainwindow.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

string const SOLS_FILE_NAME = "ouR.txt";
string const TEMP_FILE_NAME = "tmp.txt";
string const STAT_FILE_NAME = "saR.txt";
string const TBL_FILE_NAME = "stE.txt";
string const SCALE_FILE_NAME = "enQ.txt";
string const REF_FILE_NAME = "enU.txt";

int readOUR (int & nF, int & nM, QString & name) {
    name = QString::fromStdString(SOLS_FILE_NAME);
    ifstream fin;
    fin.open(SOLS_FILE_NAME, ios_base::in);
    if (!fin.is_open())
        return 1;
    fin >> nF >> nM;

    fin.close();
    return 0;


}

int readSAR (int & nF, int & nM, QString & name) {
    name = QString::fromStdString(STAT_FILE_NAME);
    ifstream fin;
    fin.open(STAT_FILE_NAME, ios_base::in);
    if (!fin.is_open())
        return 1;
    fin >> nF >> nM;

    fin.close();
    return 0;

}

int readSTE (int & nM, QString & name) {
    name = QString::fromStdString(TBL_FILE_NAME);
    ifstream fin;
    fin.open(TBL_FILE_NAME, ios_base::in);
    if (!fin.is_open())
        return 1;
    fin >> nM;

    fin.close();
    return 0;

}

int readENQ (QString & name) {
    name = QString::fromStdString(SCALE_FILE_NAME);
    ifstream fin;
    fin.open(SCALE_FILE_NAME, ios_base::in);
    if (!fin.is_open())
        return 1;
    fin.close();
    return 0;

}

int readENU (QString & name) {
    name = QString::fromStdString(REF_FILE_NAME);
    ifstream fin;
    fin.open(REF_FILE_NAME, ios_base::in);
    if (!fin.is_open())
        return 1;
    fin.close();
    return 0;

}
