#include<iostream>
#include<fstream>
#include<string>
#include<cstring>
#include<vector>
#include<cmath>
#include<map>

using namespace std;

int main(int argc, char** argv) {
    fstream file;
    string data;
    
    if (argc == 1) {
        cout << "Not enough parameters\n";
        exit(1);
    }
    if (argc > 2) {
        cout << "Too many parameters passed\n";
        for (int i = 1; i < argc; i++) {
            cout << *argv[i] << "\n";
        }
    }
    else {
        if (*argv[1] == '1') {
            cout << "Compression\n";
            file.open("message.txt");
            if (file.is_open()) {
                char mes[50], copy[50];
                file >> mes;
                strcpy(copy, mes);
                int length = strlen(mes);
                vector<char> letters;
                vector<double> prob;
                double counter;

                for (int i = 0; i < length; i++) {
                    if (mes[i] != '-') {
                        letters.push_back(mes[i]);
                        counter = 1.0;
                        for (int j = i + 1; j < length; j++) {
                            if (mes[i] == mes[j]) {
                                counter++;
                                mes[j] = '-';
                            }
                        }
                        prob.push_back(counter / length);
                    }
                }

                int position = 0, max, maxindex;
                vector<long> result;
                //bubble sort

                double temp, temp1;
                for (int i = 0; i < prob.size() - 1; i++) {
                    for (int j = 0; j < prob.size() - i - 1; j++) {
                        if (prob[j + 1] > prob[j]) {
                            temp = prob[j];
                            temp1 = letters[j];
                            prob[j] = prob[j + 1];
                            letters[j] = letters[j + 1];
                            prob[j + 1] = temp;
                            letters[j + 1] = temp1;
                        }
                    }
                }

                result.push_back(0);
                for (int i = 1; i < letters.size() - 1; i++) {
                    result.push_back((result[i - 1] + 1) * 10);
                }
                result.push_back(result[letters.size() - 2] + 1);

                file.close();

                ofstream write;
                write.open("message.txt", ios::trunc);

                if (!write.is_open()) cout << "no file\n";
                for (int i = 0; i < strlen(copy); i++) {
                    for (int j = 0; j < letters.size(); j++) {
                        if (copy[i] == letters[j]) {
                            write << result[j];
                        }
                    }
                }

                int xp, c123, totallength = 1, ln;
                double cmpratio, original, newsize, entropy = 0, efficiency, avglength = 0;
                int* avgl;
                avgl = new int[letters.size()];
                double val = 0;

                for (int i = 0; i < letters.size(); i++) {
                    val = prob[i] * log2(prob[i]);
                    entropy += val;
                }
                entropy *= -1;

                for (int i = 0; i < letters.size(); i++) {
                    c123 = 1;
                    xp = result[i];
                    while (xp > 0) {
                        ln = xp % 10;
                        totallength++;
                        if (ln == 1) c123++;
                        xp = xp / 10;
                    }
                    avgl[i] = c123;
                }

                for (int i = 0; i < prob.size(); i++) {
                    avglength += prob[i] * avgl[i];
                }

                newsize = letters.size() * 8.0 + totallength;
                original = strlen(copy) * 8.0;
                cmpratio = (1 - (newsize / original)) * 100;
                cout << "The message compression ratio is " << cmpratio << "\n";
                efficiency = 100 * (entropy / avglength);
                cout << "The coding efficiency is " << efficiency << "\n";

                write << endl;
                for (int i = 0; i < letters.size(); i++) {
                    write << letters[i] << "\n" << result[i] << "\n";
                }

                write.close();

            }
            else {
                cout << "File opening failed\n";
                exit(1);
            }
        }
        else {
            if (*argv[1] == '0') {
                cout << "Decompression\n";
                ifstream compressedFile("message.txt");
                vector<char> chars;
                vector<string> codes;
                int size = 0;
                bool flag = false;
                map <string, string> mp;

                string codedMessage;
                string tmpMsg;
                if (compressedFile.is_open ()) {
                    getline(compressedFile, codedMessage);
                    while (!compressedFile.eof ()) {
                        if (flag) {
                            getline(compressedFile, tmpMsg);
                            std::cout << tmpMsg << std::endl;
                            codes.push_back(tmpMsg);
                            flag = false;
                            size++;
                        } else {
                            getline(compressedFile, tmpMsg);
                            std::cout << "msg input: " << tmpMsg << std::endl;
                            chars.push_back(tmpMsg[0]);
                            flag = true;
                        }
                    }
                }

                for (int i = 0; i < size; i++) {
                    mp[codes.at(i)] = chars.at(i);
                }

                ofstream outputFile("output.txt");
                bool found = false;
                int counter = size;
                
                string tmp = "";
                for (int i = 0; i < codedMessage.length(); i++) {
                    tmp = tmp + codedMessage[i];

                    for (int j = 0; j < size; j++) {
                        if (!found) {
                            if (tmp == codes.at(j)) {
                                outputFile << mp[tmp];
                                tmp = "";
                                found = true;
                            }
                        }
                    }

                    found = false;
                }

                for (int i = 0; i < 4; i++) {
                    std::cout << chars.at(i) << " " << codes.at(i) << std::endl;
                }
            }
            else {
                cout << "Invalid input\n";
            }
        }
    }

    return 0;
}
