#include <iostream>
#include <fstream>
#include <algorithm>

#include "reader.hpp"


vector<string> split (string s, string delimiter){
    /*
    Splits a string based on the delimiter of choice
    */
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    string token;
    vector<string> res;

    while ((pos_end = s.find (delimiter, pos_start)) != string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        if (token[0] != *""){
            res.push_back (token);
        }
    }

    res.push_back (s.substr (pos_start));
    return res;
}

Params ReadFile(string FileName){

    /*
    Reads the input file and stores its options
    */
    
    Params myParams; // Initialize output struct for parameters
    vector<vector<string>> contents {}; // Initialize vector to store info
    
    // Initialize file and open
    ifstream myFile; 
    myFile.open(FileName, ios::in);
    string delim = " ";

    //Read file
    if (myFile.is_open()){
        string line;
        while (myFile){
            getline(myFile, line);
            if (line[0] == *";" || line[0] == *""){
                continue;
            }
            vector<string> words = split(line, delim);
            contents.push_back(words);
        }
    }
    else{
        std::cout << "\nCOULD NOT READ INPUT FILE\nTERMINATING" << std::endl;
        exit(0);
    }

    // Iterate over read lines and store their content
    for (const auto &line : contents){
        
        int size = line.size();
        myParams.NKernels = size - 2;
        string Type = line[0];

        std::transform(Type.begin(), Type.end(), Type.begin(), ::toupper);

        if (Type == "NSTEPS"){
            for (int i = 2; i < size; i++){
                if (line[i] == "" || line[i] == ";" || line[i][0] == *";")
                break;
                else
                myParams.NSTEPS = std::stoi(line[i]);
            }
        }

        else if (Type == "NOUT"){
            for (int i = 2; i < size; i++){
                if (line[i] == "" || line[i] == ";" || line[i][0] == *";")
                break;
                else
                myParams.NOUT = std::stoi(line[i]);
            }
        }

        else if (Type == "DIMENSION"){
            for (int i = 2; i < size; i++){
                if (line[i] == "" || line[i] == ";" || line[i][0] == *";")
                break;
                else
                myParams.DIMENSION = std::stoi(line[i]);
            }
        }

        else if (Type == "COURANT"){
            for (int i = 2; i < size; i++){
                if (line[i] == "" || line[i] == ";" || line[i][0] == *";")
                break;
                else
                myParams.COURANT = std::stof(line[i]);
            }
        }

        else if (Type == "BOUNDARY"){
            for (int i = 2; i < size; i++){
                if (line[i] == "" || line[i] == ";" || line[i][0] == *";")
                break;
                else{
                    string bound = line[i];
                    std::transform(bound.begin(), bound.end(), bound.begin(), ::toupper);
                    myParams.BOUNDARY = bound;
                }
                
            }
        }

        else if (Type == "KERNELS"){
            for (int i = 2; i < size; i++){
                if (line[i] == "" || line[i] == ";" || line[i][0] == *";")
                break;
                else{
                    string kernel = line[i];
                    std::transform(kernel.begin(), kernel.end(), kernel.begin(), ::toupper);
                    myParams.Kernels.push_back(kernel);
                }
            }
        }

        else if (Type == "X0"){
            for (int i = 2; i < size; i++){
                if (line[i] == "" || line[i] == ";" || line[i][0] == *";")
                break;
                else
                myParams.X0.push_back(std::stof(line[i]));
            }
        }

        else if (Type == "Y0"){
            for (int i = 2; i < size; i++){
                if (line[i] == "" || line[i] == ";" || line[i][0] == *";")
                break;
                else
                myParams.Y0.push_back(std::stof(line[i]));
            }
        }

        else if (Type == "AMPLITUDE"){
            for (int i = 2; i < size; i++){
                if (line[i] == "" || line[i] == ";" || line[i][0] == *";")
                break;
                else
                myParams.AMPLITUDE.push_back(std::stof(line[i]));
            }
        }

        else if (Type == "FREQUENCY"){
            for (int i = 2; i < size; i++){
                if (line[i] == "" || line[i] == ";" || line[i][0] == *";")
                break;
                else
                myParams.FREQUENCY.push_back(std::stof(line[i]));
            }
        }

        else if (Type == "DECAY"){
            for (int i = 2; i < size; i++){
                if (line[i] == "" || line[i] == ";" || line[i][0] == *";")
                break;
                else
                myParams.DECAY.push_back(std::stof(line[i]));
            }
        }
    }
    myParams.NKernels = myParams.Kernels.size();
    return myParams;
}