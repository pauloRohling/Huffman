#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <fstream>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <LinkedList.h>

using namespace std;

class FileManager {
public:
    static string GetTextFromFile(string fileName) {
        string line, finalText = "";
        ifstream file;
        file.open(fileName.c_str(), ios::binary);

        while(getline(file, line))
            if(line != "")
                finalText += line + "\n";
        if(finalText.size() == 0)
            throw "Erro ao ler o arquivo.";

        finalText.erase(finalText.size()-1);
        file.close();
        return finalText;
    }

    static LinkedList<string>* GetCharsFromFile(string fileName) {
        ifstream file;
        file.open(fileName.c_str(), ios::binary);
        LinkedList<string>* list = new LinkedList<string>();
        string line;

        for(int i = 0; getline(file, line); i++)
            list->AddEnd(line);

        return list;
    }

    static void CreateOutputFile(string* text, int size, string fileName) {
        ofstream file;
        file.open((fileName).c_str());

        for(int i = 0; i < size; i++) {
            file << text[i];
            if(i != size-1)
                file << "\n";
        }

        file.close();
    }

    static void RegisterLine(string line, string fileName) {
        fstream file;
        file.open(fileName.c_str(), ios::app);
        file << line << "\n";
        file.close();
    }

    static void EraseFile(string fileName) {
        ofstream file;
        file.open(fileName.c_str());
        file.close();
    }

    static LinkedList<double>* GetTimes(string fileName) {
        LinkedList<double>* list = new LinkedList<double>();
        string line;
        ifstream file;
        file.open(fileName.c_str(), ios::binary);

        while(getline(file, line))
            if(line != "")
                list->AddEnd(stod(line));

        file.close();
        return list;
    }
};



#endif // FILEMANAGER_H
