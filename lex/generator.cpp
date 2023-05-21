#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>
#include "analizator/e_nka.hpp"
#include "analizator/stringext.hpp"
#include <filesystem>
#define LEX_STANJE_PX "%X"
#define LEX_JEDINKE_LX "%L"

inline void concat_regex(const std::string& pre, std::string& ins, const std::string& post, std::string& regex, const std::map<std::string, std::string>& regdef){
    for(auto x: regdef){
        if(x.first == ins){
            ins = "(" + x.second + ")";
            break;
        }
    }
    regex = pre + ins + post;
}

inline void reference_swap(std::string& regex, const std::map<std::string, std::string>& regdef){     // zamjena referenci reg definicija s reg izrazima
    int refr_ind = 0, refr_end;
    std::string pre = "", ins, post;
    if((regex.find('{') != std::string::npos)){
        refr_ind = regex.find('{');
        refr_end = regex.find('}', refr_ind);
        
        post = regex.substr(refr_end+1, regex.length()-1);
        ins = regex.substr(refr_ind, refr_end+1-refr_ind);
        if(refr_ind == 0){
            concat_regex(pre, ins, post, regex, regdef);
            reference_swap(regex, regdef);
        } else if(is_operator(regex,refr_ind)) {  //provjerava je li "{" operator       
            pre = regex.substr(0, refr_ind);
            concat_regex(pre, ins, post, regex, regdef);
            reference_swap(regex, regdef);
        }
        return;
    }  
}

//vrlo potrebna funkcija
void construct_automaton(std::string regex, eNKA& automat){
    convert_regex(regex, automat);
    return;
}


int main(){
    std::string tmp_input;
    std::map<std::string, std::string> regularne_definicije;
    std::vector<std::string> stanja_automata, imena_leksickih_jedinki;
    std::map<std::string, std::map<std::string, std::vector<std::string>>> akcije;
    std::vector<eNKA> automaton_list;
    //std::ifstream instream;
    std::ofstream outstream;
    //instream.open("tekst.txt");

    //cita regularne definicije dok ne naide na %X (LEX_STANJE_PX)
    std::getline(std::cin, tmp_input);
    //std::getline(instream, tmp_input);
    while(tmp_input.substr(0,2) != LEX_STANJE_PX ){
        int razmak_index = tmp_input.find(" ");
        std::string reg_def = tmp_input.substr(0, razmak_index);
        std::string reg_exp = tmp_input.substr(razmak_index+1, tmp_input.length()-1);
        reference_swap(reg_exp, regularne_definicije);
        regularne_definicije.insert({reg_def, reg_exp});
        std::getline(std::cin, tmp_input);
        //std::getline(instream, tmp_input);
    }

    //cita stanja automata i imena leksickih jedinki
    stanja_automata = str_split(tmp_input.substr(3, tmp_input.length() - 2), " ");
    std::getline(std::cin, tmp_input);
    //std::getline(instream, tmp_input);
    imena_leksickih_jedinki = str_split(tmp_input.substr(3, tmp_input.length() - 2), " ");

    
    for(auto x: stanja_automata){
        std::map<std::string, std::vector<std::string>> map;
        akcije.insert({x, map});
    }

    //sam algoritam generatora leksickog analizatora, pretvara reference u reg. definicijama u reg. izraze, te konstruira automate
    int counter = 0;
    while(std::getline(std::cin, tmp_input)){           /*std::getline(instream, tmp_input)*/
            if(tmp_input.at(0) == '<'){
                int index = tmp_input.find('>');
                eNKA automat1;
                std::vector<std::string> tmp_actions;
                std::string regex_substring = tmp_input.substr(index+1, tmp_input.length());
                reference_swap(regex_substring, regularne_definicije);
                //std::cout << std::to_string(counter) + " " + regex_substring << std::endl;
                construct_automaton(regex_substring, automat1);
                automaton_list.push_back(automat1);
                std::string stanje = tmp_input.substr(1, index-1);
                std::getline(std::cin, tmp_input);       //procita {
                std::getline(std::cin, tmp_input);       //procita prvu akciju
                //std::getline(instream, tmp_input);
                //std::getline(instream, tmp_input);
                do{
                    tmp_actions.push_back(tmp_input);
                    std::getline(std::cin, tmp_input);   //procita svaku iducu
                    //std::getline(instream, tmp_input);
                } while (tmp_input != "}");
                std::map<std::string, std::map<std::string, std::vector<std::string>>>::iterator it = akcije.find(stanje); 
                if (it != akcije.end()){
                    it->second.insert({std::to_string(counter++)/*+regex_substring*/, tmp_actions});
                }
            }
    }


    //instream.close();
    //konstrukcija datoteka za spremanje generiranih automata i pravila leksickog analizatora
    outstream.open("analizator/akcije.txt", std::fstream::out);
    if(!outstream.is_open()){
        std::cout << std::filesystem::current_path() << std::endl;
        std::cout << "generator-akcije" << std::endl;
        return -1;
    }
    for(auto x: akcije){
        for(auto y : x.second){
            outstream << (x.first + " " + y.first + " " + str_concat(y.second)) << std::endl;
        }
        outstream << std::endl;
    }
    outstream.close();

    int counter2 = 0;
    outstream.open("analizator/tablica.txt", std::fstream::out);
    if(!outstream.is_open()){
        std::cout << std::filesystem::current_path() << std::endl;
        std::cout << "generator-tablica" << std::endl;
        return -1;
    }
    for(auto x : automaton_list){
        outstream << (std::to_string(counter2++) + " " +std::to_string(x.pocetno_stanje) +" "+ std::to_string(x.prihvatljivo_stanje) +" "+ std::to_string(x.prijelazi.size())) << std::endl;
        for(auto y: x.prijelazi){
            outstream << (std::to_string(y.first) + "," + str_concat(y.second))<<std::endl;
        }
        outstream << std::endl;
    }
    outstream.close();

    outstream.open("analizator/postavke.txt", std::fstream::out);
    if(!outstream.is_open()){
        std::cout << std::filesystem::current_path() << std::endl;
        std::cout << "generator-postavke" << std::endl;
        return -1;
    }
    outstream << str_concat(stanja_automata) << std::endl;
    outstream << str_concat(imena_leksickih_jedinki) << std::endl;
    outstream.close();

    return 0;
}