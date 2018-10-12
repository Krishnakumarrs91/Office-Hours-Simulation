//
//  main.cpp
//  HW02
//
//  Created by Rohan Nair on 4/17/17.
//  Copyright © 2017 Rohan Nair. All rights reserved.
//

#include <iostream>
#include <queue>
#include <stdlib.h>
#include <time.h>
#include <ctime>
#include <cstdlib>
#include <map>
#include <iterator>

using namespace std;

class Student{
public:
    Student();
    int id;
    int arrivalTime;
    int helpTime;
    int waitTime;
    int urgency;
    string helpTopic;
    string chapters[6] = {"queues", "stacks", "binary trees", "b-trees", "hashmaps", "sorting"};
};

bool arrival();

bool operator < (const Student& a, const Student& b);

int main(){
    multimap<int, string> finalReport;
    srand(time(NULL)); //seed random # generator
    priority_queue<Student> input_queue;
    queue<Student> output_queue;
    int finalTotalWait = 0;
    int finalTotalStudents = 0;
    int finalTotalExtraTime = 0;
    //conducting simulation 100 times
    for (int i = 0; i < 100; ++i){
        //clear queues to be reused
        while (!input_queue.empty()){
            input_queue.pop();
        }
        while (!output_queue.empty()){
            output_queue.pop();
        }
        for (int curr_minute = 0; curr_minute < 60; curr_minute++){
            if (arrival()){
                Student tmp;
                finalReport.insert(pair<int,string>(tmp.id, tmp.helpTopic));
                tmp.arrivalTime = curr_minute;
                input_queue.push(tmp);
            }
        }
        //if no students come to office hours
        if (input_queue.empty()){
            cout << "no students in queue" << endl;
            exit(1);
        }
        Student tmp = input_queue.top();
        int waitTotal = 0;
        int totalStudents = 0;
        int totalExtraTime = 0;
        //start current minute when first student arrives
        int curr_minute = tmp.arrivalTime;
        while (!input_queue.empty()){
            tmp = input_queue.top();
            input_queue.pop();
            //if a student arrives while professor idle
            if (curr_minute - tmp.arrivalTime < 0){
                curr_minute = tmp.arrivalTime;
                tmp.waitTime = 0;
            }
            //wait time is current time - arrival time
            else{
                tmp.waitTime = curr_minute - tmp.arrivalTime;
            }
            //advance time until student is helped
            curr_minute += tmp.helpTime;
            output_queue.push(tmp);
            totalStudents++;
            waitTotal += tmp.waitTime;
        }
        int averageWait = waitTotal/totalStudents;
        finalTotalWait += averageWait;
        finalTotalStudents +=totalStudents;
        //calculate last student's departure time
        Student tmp2 = output_queue.back();
        int  lastStudentDepart= tmp2.arrivalTime + tmp2.waitTime + tmp2.helpTime;
        //if departure time is less than 0, professor leaves at 1 hr
        totalExtraTime = (lastStudentDepart <60) ?  0: lastStudentDepart-60 ;
        finalTotalExtraTime += totalExtraTime;
    }
    cout << "average wait time: " << finalTotalWait/100 << endl;
    cout << "students helped on average: " << finalTotalStudents/100 << endl;
    cout << "extra time stayed on average: " << finalTotalExtraTime/100 << endl;
//    multimap<int, string>::iterator it;
//    multimap<int,string>::iterator s_it;
//    for (it = finalReport.begin(); it != finalReport.end(); ++it){
//        int tempKey = (*it).first;
//        int count = 0;
//        cout << "Student " << tempKey << endl;
//        for (s_it = finalReport.begin(); s_it !=finalReport.end(); ++s_it){
//            if ((*s_it).first == tempKey){
//                ++ count;
//                cout << (*s_it).second << endl;
//            }
//        }
//        cout << "times visited: " << count << endl;
//        cout << endl;
//        //cout << "Student" << (*it).first << " " << (*it).second << endl;
//        finalReport.erase(tempKey);
//    }
    cout << endl;
    cout << "final report:\n";
    for (int i= 1; i <= 100; ++i){
        int count = 0;
        pair<multimap<int,string>::iterator, multimap<int,string>::iterator> ret;
        ret = finalReport.equal_range(i);
        cout << "Student " << i << ": ";
        cout << endl;
        for (multimap<int,string>::iterator it=ret.first; it!=ret.second; ++it){
            cout << '\t' << it->second << endl;
            ++count;
        }
        cout  << "\ttimes visited: " << count;
        cout << '\n';
    }
    
    return 0;
}

Student::Student(){
    helpTime = rand() % 10 + 1;
    urgency = rand() % 3 + 1;
    id = rand() % 100 + 1;
    helpTopic = chapters[rand() % 6];
}

//10% probability
bool arrival(){
    int x = rand() % 10 + 1;
    if (x <= 1){
        return true;
    }
    else{
        return false;
    }
}

bool operator < (const Student& a, const Student& b){
    return (a.urgency < b.urgency ? true : false);
}




