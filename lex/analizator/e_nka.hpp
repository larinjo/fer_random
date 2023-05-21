#include <vector>
#include <string>
#include <map>
#include "stringext.hpp"
#pragma once

class eNKA{
    public:
        int br_stanja, pocetno_stanje, prihvatljivo_stanje;
        std::map<int, std::vector<std::string>> prijelazi;

        eNKA(){
            br_stanja = 0;
            pocetno_stanje = 0;
            prihvatljivo_stanje = 0;
        };
};


void add_epsilon_transition(eNKA &automat, int lijevo, int desno){
    std::map<int, std::vector<std::string>>::iterator it = automat.prijelazi.find(lijevo); 
        if(it != automat.prijelazi.end()){
            it->second.push_back("$->" + std::to_string(desno));
        } else {
            std::vector<std::string> vector;
            vector.push_back("$->" + std::to_string(desno));
            automat.prijelazi.insert({lijevo, vector});
        }
}   

void add_transition(eNKA &automat, int lijevo, int desno, std::string prijelazni_znak){
    std::map<int, std::vector<std::string>>::iterator it = automat.prijelazi.find(lijevo); 
        if(it != automat.prijelazi.end()){
            it->second.push_back(prijelazni_znak +"->" + std::to_string(desno));
        } else {
            std::vector<std::string> vector;
            vector.push_back(prijelazni_znak +"->" + std::to_string(desno)); 
            automat.prijelazi.insert({lijevo, vector});
        }
}

int new_state(eNKA &automat){
    automat.br_stanja++;
    return automat.br_stanja - 1;
}

bool is_operator(std::string izraz, int i){
    int br = 0;
    while(i-1>=0 && izraz.at(i-1)=='\\'){
        br++;
        i--;
    }
    return (br%2 == 0);
}

std::pair<int, int> convert_regex(const std::string izraz, eNKA &automat){
    std::vector<std::string> izbori;
    std::string ostatak = izraz;
    std::pair<int, int> rezultat;
    int br_zagrada = 0;
    for(int i=0, cnt = 0; i < izraz.length(); i++){
        if(izraz.at(i) == '(' && is_operator(izraz, i)){
            br_zagrada++;
        } else if(izraz.at(i) == ')' && is_operator(izraz, i)){
            br_zagrada--;
        } else if(br_zagrada == 0 && izraz.at(i) == '|' && is_operator(izraz, i)){
            izbori.push_back(izraz.substr(cnt, i-cnt));
            cnt = i+1;
            ostatak = izraz.substr(i+1, std::string::npos);
        }
    }
    if(izbori.size() > 0){ //izraz.find('|') != std::string::npos ili izbori.size() > 0
        izbori.push_back(ostatak);
    }

    int lijevo_stanje = new_state(automat);
    int desno_stanje = new_state(automat);
    if(izbori.size() > 0){ //izraz.find('|') != std::string::npos
        for(int i = 0; i < izbori.size(); i++){
            std::pair<int, int> privremeno = convert_regex(izbori[i], automat);
            add_epsilon_transition(automat, lijevo_stanje, privremeno.first);
            add_epsilon_transition(automat, privremeno.second, desno_stanje);
        }
    } else {
        bool prefiksirano = false;
        int zadnje_stanje = lijevo_stanje;
        for(int i = 0; i < izraz.length(); i++){
            int a, b;
            if(prefiksirano){
                prefiksirano = false;
                std::string prijelazni_znak;
                if(izraz.at(i) == 't'){
                    prijelazni_znak = "\t";
                } else if(izraz.at(i) == 'n'){
                    prijelazni_znak = "\n";
                } else if(izraz.at(i) == '_'){
                    prijelazni_znak = " ";
                } else {
                    prijelazni_znak = izraz.at(i);
                }
                a = new_state(automat);
                b = new_state(automat);
                add_transition(automat, a, b, prijelazni_znak);
            } else {
                if(izraz.at(i) == '\\'){
                    std::string tmp_znak;
                    tmp_znak = izraz.at(i);
                    prefiksirano = true;
                    continue;
                } 
                if(izraz.at(i) != '('){
                    //debug
                    //
                    a = new_state(automat);
                    b = new_state(automat);
                    if(izraz.at(i) == '$'){
                        add_epsilon_transition(automat, a, b);
                    } else {
                        std::string tmp_znak;
                        tmp_znak = izraz.at(i);
                        add_transition(automat, a, b, tmp_znak);
                    }
                } else {
                    std::string tmp_znak;
                    tmp_znak = izraz.at(i);
                    //
                    int tmp_i = i;
                    int j = 0;
                    /*
                    while(true){
                        int index_zag = izraz.find(')', tmp_i);
                        if(index_zag != std::string::npos && is_operator(izraz, index_zag)){
                            j = index_zag;
                            break;
                        } else {
                            tmp_i++;
                        }
                    }
                    */
                    int br_zagrada2 = 0;
                    for(j=i; j < izraz.length(); j++){
                        if(izraz.at(j) == '(' && is_operator(izraz, j)){
                            br_zagrada2++;
                        } else if(izraz.at(j) == ')' && is_operator(izraz, j)){
                            br_zagrada2--;
                        } if(br_zagrada2 == 0 && is_operator(izraz, j)){
                            break;
                        }
                    }
                    std::pair<int, int> privremeno = convert_regex(izraz.substr(i+1, j-i-1), automat);
                    a = privremeno.first;
                    b = privremeno.second;
                    i = j;
                }
            }
            if(izraz.length() > i + 1 && izraz.at(i+1) == '*'){
                int x = a;
                int y = b;
                a = new_state(automat);
                b = new_state(automat);
                add_epsilon_transition(automat, a, x);
                add_epsilon_transition(automat, y, b);
                add_epsilon_transition(automat, a, b);
                add_epsilon_transition(automat, y, x);
                i++;
            }
            add_epsilon_transition(automat, zadnje_stanje, a);
            zadnje_stanje = b;
        }
        add_epsilon_transition(automat, zadnje_stanje, desno_stanje);
    }
    rezultat.first = lijevo_stanje;
    automat.pocetno_stanje = lijevo_stanje;
    rezultat.second = desno_stanje;
    automat.prihvatljivo_stanje = desno_stanje;
    return rezultat;
} 

