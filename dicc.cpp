#include "mainwindow.h"
#include <conio.h>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <direct.h>
#include <string>
#include <set>
#include <vector>
#include <ctime>
#include <algorithm>
#include <QProgressBar>
using namespace std;

int DICC(int N, int M[], QProgressBar * pb) {
    setlocale(LC_ALL, "rus");
    FILE *input,  *cfg, *tmp, *wrds, *wl;
    const string NIF = "ouR.txt"; //файл с решениями
    const string IWF = "inV.txt"; //файл с покрытием
    const string CGF = "enU.txt"; //файл с эталоном
    const string DIC = "dictionaryStr"; //каталог со словарем
    const string NOF = "outputMIN.txt"; //вывод
    const string TMF = "tmp.txt"; // temporary file
    const int LA = 31;// макс длина алфавита
    const int LW = 40;// макс длина слова



    struct Struct { //структура слова
        char abc[LW] = ""; //структура
        int nWords = 0; // количество слов структуры
        int first = 0; //первое слово структуры
    };

    struct Word { //слово
        char abc[LW] = ""; //слово
        unsigned int freq = 0; //частота
        int next = 0; //следующее в списке
    };

    struct Dict { //длина в словаре
        void reserve() {
            structs.reserve(nStructs);
            words.reserve(nWords);
        }
        bool isThere = 0;
        unsigned int nStructs{};
        unsigned int nWords{};
        vector<Struct> structs;
        vector<Word> words;
    };

    struct Context {
        Context() {
            lens.reserve(50);
            words.reserve(100);
        }
        vector<int> lens;
        vector<Word> words;
    };

    struct Solution { // решение
        char abc[LA] = ""; //код
        unsigned int n = 0; // номер в списке
        unsigned int nT = 0; // номер в дереве
        char met[6] {}; // метод
        double score = 0; // оценка
    };

    struct Nw {
        vector<char> ch [LA];
    };

    struct f {
        Nw checkCont(Context cover, Context cont, string sol) {
            Nw nws;
            for (vector<int>::size_type i = 0; i != cover.words.size(); i++) {
                for (int j = 0; j < cover.lens[i]; j++) {
                    int ism = sol.find_first_of(cover.words[i].abc[j]);

                    if (find(nws.ch[ism].begin(), nws.ch[ism].end(), cont.words[i].abc[j]) == nws.ch[ism].end()) {
                        nws.ch[ism].push_back(cont.words[i].abc[j]);
                    }

                }


            }
            return nws;
        }
        void readDictionary(FILE *file, Dict& dictionary) {


            dictionary.isThere = 1;
            dictionary.nStructs = 0;
            dictionary.nWords = 0;

            fscanf_s(file, "%d", &dictionary.nWords);
            fscanf_s(file, "%d", &dictionary.nStructs);
            dictionary.reserve();

            // read structures
            Struct strbf;
            for (int i = 0; i < dictionary.nStructs; i++) {
                fscanf_s(file, "%s", strbf.abc, (unsigned)_countof(strbf.abc));
                fscanf_s(file, "%d", &strbf.nWords);
                fscanf_s(file, "%d", &strbf.first);
                dictionary.structs.emplace_back(strbf);
            }

            // read words
            Word wrdbf;
            for (int i = 0; i < dictionary.nWords; i++) {
                fscanf_s(file, "%s", wrdbf.abc, (unsigned)_countof(wrdbf.abc));
                fscanf_s(file, "%d", &wrdbf.freq);
                fscanf_s(file, "%d", &wrdbf.next);
                dictionary.words.emplace_back(wrdbf);
            }


        }
        void printSol(FILE *output, Solution sl, int nSym) { //вывод
            for (int ic = 0; ic < nSym; ic++) {
                fprintf_s(output, "%c", sl.abc[ic]);
            }
            fprintf_s(output, " %i\t%i\t", sl.n, sl.nT);
            for (int ic = 0; ic < 6; ic++) {
                fprintf_s(output, "%c", sl.met[ic]);
            }
            //fprintf(output, " %lf", sl.score);
            fprintf_s(output, "\n");
        }

        void decode(char* openWord, char* closeWord, Solution sl, char* stand, int len) { //декодирование
            int ind = 0;
            for (int i = 0; i < len; i++) {
                int ist = 0;
                while (closeWord[i] != sl.abc[ist]) {
                    ist++;
                }
                openWord[i] = stand[ist];
            }
        }
        void toStruct(char* str, char* word, int len) { //переход к структуре слова
            int k = 1;
            char c = word[0];
            int io = 0;
            str[io] = (char)-64;
            io++;
            for (int i = 1; i < len; i++) {
                char c = word[i];
                bool b = false;
                for (int j = 0; j < i; j++) {
                    if (word[j] == c) {
                        b = true;
                        str[io] = str[j];
                        io++;
                        break;
                    }
                }
                if (!b) {
                    str[io] = (char)-64 + k;
                    io++;
                    k++;
                }
            }
        }
        void countLetters(int* cnts, char* word, char* stand, int len, int nSym) { //подсчет вхождений букв в слово
            for (int i = 0; i < nSym; i++) {
                cnts[i] = count(word, word + strlen(word), stand[i]);
            }
        }
        void compLetters(int* inc, char* word, char* stand, int len, int nSym) { //проверка присутствия букв в слове
            for (int i = 0; i < nSym; i++) {
                if (strchr(word, stand[i]) != NULL) inc[i] = 1;
                else inc[i] = 0;
            }
        }

        double compWords(char* opword, char* dicword, char* stand, int *tru, int len, int nSym) {//сравнение слова со словарным (0-1) 1 - полностью совпадает
            double eq = 0;
            for (int i = 0; i < nSym; i++) tru[i] = 0;
            for (int i = 0; i < len; i++) {
                if (opword[i] == dicword[i]) {
                    eq += 1;
                    tru[strchr(stand, opword[i]) - stand] = 1;
                }
            }
            eq /= len;
            return eq;
        }
        bool comStr(char* first, char* second, int len) {
            bool eq = true;
            for (int i = 0; i < len; i++) {
                if (first[i] != second[i]) {
                    eq = false;
                    break;
                }
            }
            return eq;
        }

    };



    f f; //init functions
    errno_t err;
    //char buffer[1048576];
    char buffer[1024];
    int nFrag = 0, nSol = 0;
    set<int> nums;
    Dict dictionary[LW];



    if ((err = fopen_s(&cfg, CGF.c_str(), "r")) != 0) {
        cout << "Cannot open enU file.\n";
        return 1;
    }
    if ((err = fopen_s(&input, NIF.c_str(), "r")) != 0) {
        cout << "Cannot open ouR file.\n";
        return 1;
    }


    //считываем эталон
    int nLetters = 0;
    fscanf_s(cfg, "%i\t%*i\n", &nLetters); //nLetters - количество букв в алфавите
    char standart[LA]; //строка с эталоном
    for (int k = 0; k < nLetters; k++) {
        char c;
        fscanf_s(cfg, "%c\t", &c, 1);
        standart[k] = c;
    }
    if (N == 0) {
        nums.insert(1);
    } else
        for (int k = 0; k < N; k++) {
            nums.insert(M[k]); // множество с номерами решений, которые нужно проверить
        }

    //проверка решений
    //fopen_s(&output, NOF.c_str(), "w");
    fopen_s(&tmp, TMF.c_str(), "w");
    fscanf_s(input, "%i %i\n", &nFrag, &nSol); //считываем количество фрагментов и кол-во решений
    fprintf_s(tmp, "%i %i\n", nFrag, nSol + 1);
    pb->setRange(0,nFrag-1);
    pb->setValue(0);
    for (int frag = 0; frag < nFrag; frag++) {

        pb->setValue(frag);
        vector<Solution> uniqSols;
        int nfrg = 0;
        fscanf_s(input, "_%i_\n", &nfrg);
        fprintf_s(tmp, "_%i_\n", nfrg);
        //if (nfrg != frag) { frag++; fprintf_s(tmp, "_%i_\n", nfrg);	}
        int cntUS = 0;
        for (int s = 0; s < nSol; s++) {
            //считывание решения
            char ch;
            Solution currSol;
            for (int ic = 0; ic < nLetters; ic++) {
                ch = fgetc(input);
                currSol.abc[ic] = ch;
            }
            ch = fgetc(input);
            fscanf_s(input, "%i\t%i", &currSol.n, &currSol.nT);
            ch = fgetc(input);
            for (int ic = 0; ic < 6; ic++) {
                ch = fgetc(input);
                currSol.met[ic] = ch;
            }
            ch = fgetc(input);
            f.printSol(tmp, currSol, nLetters);
            //
            //поиск и удаление одинаковых решений (остается первое попавшееся)
            if (nums.count(currSol.n) != 0) { //есть ли решение с таким номером в списке на проверку
                bool isEqual = false;
                int inS = 0;
                while (!isEqual && cntUS != 0 && inS < cntUS) {
                    for (int ic = 0; ic < nLetters; ic++) {
                        if (currSol.abc[ic] != uniqSols[inS].abc[ic]) {
                            break;
                        }
                        if (ic == nLetters - 1) isEqual = true;
                    }
                    inS++;
                }
                if (!isEqual) {
                    uniqSols.push_back(currSol);
                    cntUS++;
                } else isEqual = false;
            }
        }
        Solution min;
        Context minCont;
        Context minCover;
        //на данном этапе сформирован массив из решений, далее оценка каждого
        for (int iSol = 0; iSol < cntUS; iSol++) {
            Context cont;
            Context cov;
            // считывание словарной формы
            if ((err = fopen_s(&wrds, IWF.c_str(), "r")) != 0) {
                cout << "Cannot open inW file.\n";
                return 1;
            }
            int nFragW;
            fscanf_s(wrds, "%d\n", &nFragW);
            if (nFragW > nFrag) nFrag = nFragW; //сравнение количества фрагментов в словарной форме и решениях
            fscanf_s(wrds, "%*s\n");
            int nWords = 0;
            int nLens = 0;
            fscanf_s(wrds, "%*d %d %d %*d\n", &nWords, &nLens); //количество слов и длин во фрагменте
            for (int iL = 0; iL < nLens; iL++) { //цикл длин
                int currLn = 0;
                int nwLn = 0;
                fscanf_s(wrds, "%d %d %*d %*d\n", &currLn, &nwLn); //длина, кол-во слов
                for (int iW = 0; iW < nwLn; iW++) { //цикл слов в длине
                    char closeWord[LW];
                    char c;
                    int iwc = 0;
                    double points = 0;
                    while ((c = fgetc(wrds)) != ' ') {
                        closeWord[iwc] = c;    //слово
                        iwc++;
                    }
                    while ((c = fgetc(wrds)) != ' ') {}//частота (не нужна)
                    //вывод
                    Word openWord;
                    f.decode(openWord.abc, closeWord, uniqSols[iSol], standart, currLn); //декодирование с помошью решения
                    char strWord[LW] = "";
                    f.toStruct(strWord, openWord.abc, currLn); //нахождение структуры
                    int cntLet[LA];
                    f.countLetters(cntLet, openWord.abc, standart, currLn, nLetters); //количество каждой буквы

                    //поиск слова в словаре
                    string DICWF = DIC + "\\w" + to_string(currLn) + ".txt"; //открытие файла нужной длины
                    if ((err = fopen_s(&wl, DICWF.c_str(), "r")) != 0) {
                        //если такой длины нет в словаре
                        points = 0;
                    } else {
                        if (!dictionary[currLn].isThere) {
                            f.readDictionary(wl, dictionary[currLn]);
                        }

                        bool found = false;
                        Struct currStr;
                        for (int iStr = 0; iStr < dictionary[currLn].nStructs; iStr++) { //поиск структуры в словаре
                            //если структуры совпадают
                            if (f.comStr(dictionary[currLn].structs[iStr].abc, strWord, currLn)) {
                                currStr = dictionary[currLn].structs[iStr];
                                found = true;
                                break;
                            }
                        }
                        if (found) { //если структура найдена
                            int cmpLet[LA];
                            Word bestWord;
                            double maxMatch = 0;
                            int nxt = currStr.first;
                            for (int iW = 0; iW < dictionary[currLn].nWords; iW++) { //ищем в словах лучшее совпадение
                                if (iW == nxt) {
                                    nxt = dictionary[currLn].words[iW].next;
                                    if (maxMatch != 1) {
                                        int cLet[LA];
                                        double sc = f.compWords(openWord.abc, dictionary[currLn].words[iW].abc, standart, cLet, currLn, nLetters);
                                        if (sc > maxMatch) {
                                            maxMatch = sc;
                                            bestWord = dictionary[currLn].words[iW];
                                            memcpy(cmpLet, cLet, sizeof(cLet));
                                        }
                                    }

                                }
                            }


                            //cmpLet = f.compLetters(bestWord.abc, standart, currLn, nLetters); //присутствие каждой буквы в лучшем слове
                            //подсчет очков для слова
                            for (int i = 0; i < nLetters; i++) {
                                points += cntLet[i] * cmpLet[i]; // сумма
                            }
                            points /= currLn; // оценка решения для слова
                            cont.words.push_back(bestWord); // добавление слова в контекст
                            cov.words.push_back(openWord);
                            cont.lens.push_back(currLn);
                            cov.lens.push_back(currLn);
                            /*
                            for (int ic = 0; ic < nLetters; ic++) { fprintf_s(output, "%i ", cntLet[ic]); }
                            fprintf_s(output, "\n");
                            for (int ic = 0; ic < nLetters; ic++) { fprintf_s(output, "%i ", cmpLet[ic]); }
                            fprintf_s(output, "\n");
                            for (int ic = 0; ic < nLetters; ic++) { fprintf_s(output, "%c ", standart[ic]); }
                            fprintf_s(output, "\n");

                            for (int ic = 0; ic < currLn; ic++) { fprintf_s(output, "%c ", bestWord.abc[ic]); }
                            fprintf_s(output, "\n");

                            for (int ic = 0; ic < currLn; ic++) { fprintf_s(output, "%c ", openWord.abc[ic]); }
                            fprintf_s(output, "\n");

                            fprintf_s(output, " %lf - points", points);
                            fprintf_s(output, "\n");
                            */

                        } else {
                            /*

                            fprintf_s(output, "Слов с такой структурой нет\n");
                            for (int ic = 0; ic < currLn; ic++) { fprintf_s(output, "%c", strWord[ic]); }
                            fprintf_s(output, "\n"); */

                            points = 0;
                        }


                    }
                    fclose(wl);
                    //uniqSols[iSol].cont = cont;
                    uniqSols[iSol].score += points; //суммарное количество очков для метода
                    if (uniqSols[iSol].score > min.score) {
                        min = uniqSols[iSol];
                        minCont = cont;
                        minCover = cov;
                    }
                }
            }
            fclose(wrds);
        }



        min.met[0] = ' ';
        min.met[1] = '[';
        min.met[2] = 'D';
        min.met[3] = 'I';
        min.met[4] = 'C';
        min.met[5] = ']';
        min.n = nSol + 1;
        min.nT = 0;

        //fprintf_s(output, "Solution score = %lf \n", min.score);
        //оценка контекста
        /*
        for (auto const& wrd : minCont.words) {
            fprintf_s(output, "%s ", wrd.abc);
        }

        fprintf_s(output, "\n");
        for (auto const& wrd : minCover.words) {
            fprintf_s(output, "%s ", wrd.abc);
        }
        */

        Nw nw = f.checkCont(minCover, minCont, min.abc);
        /*
        fprintf_s(output, "\n");
        for (int i = 0; i < LA; i++) {
            fprintf_s(output, "%i ", nw.ch[i].size());
        }
        fprintf_s(output, "\n");
        for (int i = 0; i < LA; i++) {
            if (nw.ch[i].size() != 0) fprintf_s(output, "%c ", nw.ch[i][0]);
        }
        */


        int k2 = 0;
        for (int i = 0; i < LA; i++) {
            k2 += nw.ch[i].size();
        }
        k2 /= LA;

        if (k2 == 1) {
            for (int i = 0; i < LA; i++) {
                min.abc[i] = nw.ch[i][0];
            }
        }

        //вывод
        /*
        fprintf_s(output, "\n");
        fprintf_s(output, "_%i_\n", frag + 1);
        fprintf_s(output, "%i\n", cntUS);
        for (int ic = 0; ic < nLetters; ic++) { fprintf_s(output, "%c", standart[ic]); }
        fprintf_s(output, "\n");

        for (int s = 0; s < cntUS; s++) {
            f.printSol(output, uniqSols[s], nLetters);
        }
        f.printSol(output, min, nLetters);
        */

        f.printSol(tmp, min, nLetters);

    }

    fclose(input);
    //fclose(output);
    fclose(cfg);
    fclose(tmp);
    //перенос данных из временного файла

    fopen_s(&tmp, TMF.c_str(), "r");
    fopen_s(&input, NIF.c_str(), "w");
    char cc[256];
    while (!feof(tmp)) {
        fgets(cc, sizeof(cc), tmp);
        fputs(cc, input);
    }
    fclose(input);
    fclose(tmp);
    remove(TMF.c_str());

    return 0;
}
int dicCont(QProgressBar * pb) {
    int M[20] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
    DICC(20, M, pb);
    return 0;
}
