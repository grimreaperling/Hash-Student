#include <iostream>
#include <fstream>
#include "hashmap.h"
#include "student.h"
#include "vector.h"
#include "console.h"
#include "simpio.h"
#include "filelib.h"
#include "strlib.h"
using std::cout;     using std::endl;
using std::ifstream; using std::string;
void mapInit(HashMap<string, Student>&, HashMap<string, Student>&);         //The function to read in the data from the file and store it in the hash map.
void search(HashMap<string, Student>&, HashMap<string, Student>&, int);    //The function to start the serach and modify on a particular hash map.
void add(HashMap<string, Student>&, HashMap<string, Student>&);      //The function to add a new entry.
void writeBack(const HashMap<string, Student>&);                     //The function to write the hash map back to the file.

int main() {
    HashMap<string, Student> nameHash;
    HashMap<string, Student> nickHash;
    mapInit(nameHash, nickHash);
    int time = 1;
    while (true) {
        cout << "Phase " << time << endl;
        cout << "\n" << endl;
        time++;
        if (getYesOrNo("Input yes to start and no to quit.", "You must input a yes or no!")) {
            int flag = getInteger("Input 1 to search in the name, 2 in nickname, 0 to add a new entry, other integer to show the hash map!");
            if (flag == 1 or flag == 2) {
                try {
                    search(nameHash, nickHash, flag);
                } catch (std::out_of_range &e) {
                    cout << "Entry not exsit!!" << "\n" << endl;
                    continue;
                }
            } else if (flag == 0) {
                add(nameHash, nickHash);
            } else {
                nameHash.debug();
            }
            cout << "\n" << endl;
        } else {
            cout << "Bye!" << endl;
            cout << "\n" << endl;
            writeBack(nameHash);
            return 0;
        }
    }
    return 0;
}

void mapInit(HashMap<string, Student>& nameHash, HashMap<string, Student>& nickHash) {
    string filename = "students.txt";
    ifstream ifs;
    if (!openFile(ifs, filename)) {
        cout << "Can't open the file!" << endl;
        exit(1);
    }
    Vector<string> lines;
    readEntireFile(ifs, lines);
    for (string line : lines) {
        Vector<string> words;
        words = stringSplit(line, " ");
        Vector<int> scores;
        for (int i = 4; i < 7; i++) {
            scores += stringToInteger(words[i]);
        }
        Student stu(words.subList(0, 4), scores);
        nameHash.insert({words.get(1), stu});
        nickHash.insert({words.get(2), stu});
        while (nameHash.load_factor() > 10) {                  //We rehash the hashmap if the load factor is greater than 10.
            nameHash.rehash(nameHash.bucket_count() * 2);
        }
        while (nickHash.load_factor() > 10) {
            nickHash.rehash(nickHash.bucket_count() * 2);
        }
    }
}

void search(HashMap<string, Student>& nameHash, HashMap<string, Student>& nickHash, int flag) {
    string name = getLine("Please input the key: ");
    Student stu;
    if (flag == 1) {
        stu = nameHash.at(name);
    } else {
        stu = nickHash.at(name);
    }
    cout << stu << endl;
    if (getYesOrNo("Input yes to modidy this entry, no to continue: ", "You must input a yes or no!")) {
        cout << "Please input the number to continue modify!" << endl;
        int i = getInteger("1: number, 2: name, 3: nickname, 4: classno, 5: grade1, 6: grade2, 7: grade3, other integer to delete this entry.");
        if (i > 0 and i < 5) {
            string str = getLine("Input what you wanna change to:");
            stu.changeInfo(i-1, str);
            nameHash.at(stu.getName()) = stu;
            nickHash.at(stu.getNick()) = stu;
        } else if (i > 4 and i < 8) {
            int grade = getInteger("Input the grade you wanna change to:");
            stu.changeGrade(i-1, grade);
            nameHash.at(stu.getName()) = stu;
            nickHash.at(stu.getNick()) = stu;
        } else {
            nameHash.erase(stu.getName());
            nickHash.erase(stu.getNick());
        }
    }
}

void add(HashMap<string, Student>& nameHash, HashMap<string, Student>& nickHash) {
    Vector<string> infos;
    Vector<int> scores;
    string number = getLine("Input the number: ");
    infos += number;
    string name = getLine("Input the name: ");
    infos += name;
    string nickname = getLine("Input the nickname: ");
    infos += nickname;
    string classno = getLine("Input the class number: ");
    infos += classno;
    for (int i = 1; i <= 3; i++) {
        int number = getInteger("Please input the grade" + integerToString(i) + ": ");
        scores += number;
    }
    Student stu(infos, scores);
    nameHash.insert({infos.get(1), stu});
    nickHash.insert({infos.get(2), stu});
}

void writeBack(const HashMap<string, Student>& map) {
    string filename = "students.txt";
    auto iter = map.begin();
    writeEntireFile(filename, iter->second.toString());           // For the first write we flush the original content and for the rest we append the file.
    iter++;
    for (; iter != map.end(); iter++) {
        writeEntireFile(filename, iter->second.toString(), true);
    }
}
