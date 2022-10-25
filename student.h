#ifndef STUDENT_H 
#define STUDENT_H
#include <iostream>
#include <string>
#include "vector.h"
#include "strlib.h"
using std::string;

class Student {
public:
    /*
     * The default constructor.
     */
    Student();

    /*
     * The main constrctor which take in two vectors 
     * one contains the elementry information about the student 
     * the second contains the student's score.
     */
    Student(Vector<string> infos, Vector<int> scores);

    /* 
     * The default destructor for the student class
     */
    ~Student();

    /*
     * The method to update the grade of the student.
     */
    void changeGrade(int index, int grade);

    /*
     * The method to modify the basic information of the student.
     */
    void changeInfo(int index, string str);

    /*
     * Get name from the student class.
     */
    string getName();

    /*
     * Get nickname from the student class.
     */
    string getNick();

    /*
     * A method to change the student class to a one line string
     * in order to write back to the file.
     */
    string toString() const;
    
    /*
     * You overload the << operator to generate the output information.
     */
    friend std::ostream& operator<<(std::ostream& oss, const Student& stu);


private:
    /* 
     * The vector contains the basic information about a student
     */
    Vector<string> infos;

    /*
     * The vector contains the scores of the student
     */
    Vector<int> scores;
};
    
Student::Student(Vector<string> infos, Vector<int> scores) :
    infos(infos),
    scores(scores) {}

Student::~Student() {}

Student::Student() {}

void Student::changeInfo(int index, string str) {
    infos[index] = str;
}

void Student::changeGrade(int index, int grade) {
    scores[index-4] = grade;
}

string Student::getName() {
    return infos[1];
}

string Student::getNick() {
    return infos[2];
}

string Student::toString() const {
    string str;
    for (string s : infos) {
        str += s + " ";
    }
    for (int i = 0; i < 2; i++) {
        str += integerToString(scores[i]) + " ";
    }
    str += integerToString(scores[2]) + "\n";
    return str;
}

std::ostream& operator<<(std::ostream& oss, const Student& stu) {
    oss << "informations: " << stu.infos << '\n';
    oss << "Grades: " << stu.scores;
    return oss;
}

#endif
