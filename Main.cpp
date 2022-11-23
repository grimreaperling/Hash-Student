#include <iostream>
#include <fstream>
#include <stdint.h>
#include "hashmap.h"
#include "xxhash32.h"
#include "City.h"
#include "student.h"
#include "vector.h"
#include "console.h"
#include "simpio.h"
#include "filelib.h"
#include "strlib.h"
using std::cout;     using std::endl;
using std::ifstream; using std::string;

template <typename T>
void mapInit(T&, int index=2);

template <typename T, typename M>
void mapInit(T&, M&);                                                       //The function to read in the data from the file and store it in the hash map.

void search(HashMap<string, Student>&, HashMap<string, Student>&, int);    //The function to start the serach and modify on a particular hash map.
void add(HashMap<string, Student>&, HashMap<string, Student>&);      //The function to add a new entry.
void writeBack(const HashMap<string, Student>&);                     //The function to write the hash map back to the file.
void testHash();

int main() {
    HashMap<string, Student> nameHash;
    HashMap<string, Student> nickHash;
    mapInit<HashMap<string, Student>, HashMap<string, Student>>(nameHash, nickHash);
    int time = 1;
    
    /* Into a infinite loop and wait for the user input.*/
    while (true) {
        cout << "Phase " << time << endl;
        cout << "\n" << endl;
        time++;
        if (getYesOrNo("Input yes to start and no to quit.", "You must input a yes or no!")) {
            string prompt = "Input 1 to search in the name, 2 in nickname, 3 to add a new entry, 0 to test the hash function";
            prompt += " other integer to show the hash map!";
            int flag = getInteger(prompt);
            if (flag == 1 or flag == 2) {
                try {
                    search(nameHash, nickHash, flag);
                } catch (std::out_of_range &e) {
                    cout << "Entry not exsit!!" << "\n" << endl;
                    continue;
                }
            } else if (flag == 3) {
                add(nameHash, nickHash);
            } else if (flag == 0) {
                testHash();
            } else {
                nameHash.debug();
            }
            cout << "\n" << endl;
        } else {
            cout << "Bye!" << endl;
            cout << "\n" << endl;
            writeBack(nickHash);
            return 0;
        }
    }
    return 0;
}

/*
 * A little function to change XXHash the way we can use.
 */
size_t xxhash(const string& input) {
    uint64_t size = static_cast<uint64_t>(input.size());
    const char* s = input.c_str();
    return static_cast<size_t>(XXHash32::hash(s, size, 0));
}

/*
 * A little function to change the farm hash interface to adapt it
 * into the hash map class.
 */
size_t cityhash(const string& input) {
    size_t size = input.size();
    const char* s = input.c_str();
    return CityHash64(s, size);
}

/*
 * Little function to test the hash function.
 */
void testHash() {
    /* Create and initialize the hashmap.*/
    HashMap<string, Student> stdhashmap;
    HashMap<string, Student, size_t (*)(const string&)> xxhashmap(10, xxhash);
    HashMap<string, Student, size_t (*)(const string&)> cityhashmap(10, cityhash);

    mapInit<HashMap<string, Student, size_t (*)(const string&)>>(xxhashmap);
    mapInit<HashMap<string, Student, size_t (*)(const string&)>>(cityhashmap);
    mapInit<HashMap<string, Student>>(stdhashmap);

    /* Read the file and insert the entry into the hashmap.*/
    int total_lenth1, total_lenth2, total_lenth3;
    total_lenth3 = total_lenth2 = total_lenth1 = 0;
    int count = 0;
    string filename = "test.txt";
    ifstream ifs;
    if (!openFile(ifs, filename)) {
        cout << "Can't open the file!" << endl;
    }
    Vector<string> names;
    readEntireFile(ifs, names);
    for (string name : names) {
        total_lenth1 += xxhashmap.findLenth(name);
        total_lenth2 += cityhashmap.findLenth(name);
        total_lenth3 += stdhashmap.findLenth(name);
        count++;
    }

    /*Output the test information.*/
    cout << "End test of the XXHash function the average find lenth is: ";
    cout << std::setprecision(4) << static_cast<float>(total_lenth1) / count << endl;
    cout << "End test of the CityHash function the average find lenth is: ";
    cout << std::setprecision(4) << static_cast<float>(total_lenth2) / count << endl;
    cout << "End test of the STLHash function the average find lenth is: ";
    cout << std::setprecision(4) << static_cast<float>(total_lenth3) / count << endl;
}

/* Initialize the hashmap and insert the entry from the file into the hashmap.*/
template <typename T>
void mapInit(T& hash, int index) {
    string filename = "students.txt";
    ifstream ifs;

    /* Handle the exception.*/
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
        hash.insert({words.get(index), stu});

        while (hash.load_factor() > 10) {                  //We rehash the hashmap if the load factor is greater than 10.
            hash.rehash(hash.bucket_count() * 2);
        }
    }
}

template <typename T, typename M>
void mapInit(T& nameHash, M& nickHash) {
    mapInit<T>(nameHash, 1);
    mapInit<M>(nickHash);
}

/*The function to handle the user's search.*/
void search(HashMap<string, Student>& nameHash, HashMap<string, Student>& nickHash, int flag) {
    string name = getLine("Please input the key: ");
    Student stu;
    if (flag == 1) {
        stu = nameHash.at(name);
    } else {
        stu = nickHash.at(name);
    }
    cout << stu << endl;
    
    /* This part is responsible for the user edit.*/
    if (getYesOrNo("Input yes to modidy this entry, no to continue: ", "You must input a yes or no!")) {

        cout << "Please input the number to continue modify!" << endl;

        int i = getInteger("1: number, 2: classno, 3: grade1, 4: grade2, 5: grade3, other integer to delete this entry.");

        if (i > 0 and i < 3) {
            string str = getLine("Input what you wanna change to:");
            stu.changeInfo(3 * (i - 1), str);
            nameHash.at(stu.getName()) = stu;
            nickHash.at(stu.getNick()) = stu;
        } else if (i > 2 and i < 6) {
            int grade = getInteger("Input the grade you wanna change to:");
            stu.changeGrade(i+1, grade);
            nameHash.at(stu.getName()) = stu;
            nickHash.at(stu.getNick()) = stu;
        } else {
            nameHash.erase(stu.getName());
            nickHash.erase(stu.getNick());
        }
    }
}

/* Add a entry into the hashmap.
 * Call from the main function
 * and get the information from the console input.
 */
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

/* Before end up the program write back the changes into the file.*/
void writeBack(const HashMap<string, Student>& map) {
    string filename = "students.txt";
    auto iter = map.begin();
    writeEntireFile(filename, iter->second.toString());           // For the first write we flush the original content and for the rest we append the file.
    iter++;
    for (; iter != map.end(); iter++) {
        writeEntireFile(filename, iter->second.toString(), true);
    }
}
