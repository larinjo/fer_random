#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include "stringext.hpp"
#include "e_nka.hpp"
#include "e_nka_simulator.hpp"
#include <sstream>
#include <ostream>
#include <algorithm>
#include <numeric>
#include <filesystem>
#include <stdio.h>
#define SET 0
#define RESET 1


int main(){
    std::vector<std::string> stanja_automata, identifikatori_leks_jedinki;
    std::ifstream in_stream;   
    std::string tmp_input; 
    std::vector<std::string> tmp_vector;
    std::vector<enkaSimulator> automaton_list;
    std::map<std::string, std::map<int, std::vector<std::string>>> akcije;
    std::string trenutno_stanje;
    std::vector<std::string> vector_print;

    //ucitavanje postavki analizatora iz datoteka generiranih od generatora
    in_stream.open("postavke.txt", std::fstream::in);
    if(!in_stream.is_open()){
        std::cout << std::filesystem::current_path() << std::endl;
        std::cout << "postavke" << std::endl;
        return -1;
    }
    std::getline(in_stream, tmp_input);
    stanja_automata = str_split(tmp_input, " ");
    std::getline(in_stream, tmp_input);
    identifikatori_leks_jedinki = str_split(tmp_input, " ");
    in_stream.close();

    for(auto x: stanja_automata){
        std::map<int, std::vector<std::string>> mapa;
        akcije.insert({x, mapa});
    }
    
    in_stream.open("akcije.txt", std::fstream::in);
    if(!in_stream.is_open()){
        std::cout << std::filesystem::current_path() << std::endl;
        std::cout << "akcije" << std::endl;
        return -1;
    }
    while(std::getline(in_stream, tmp_input)){
        if(tmp_input != ""){
            tmp_vector = str_split(tmp_input, " ");
            auto it = akcije.find(tmp_vector.at(0)); 
            if ((it != akcije.end() && it->second.size() > 0) && ((it->second.find(std::stoi(tmp_vector.at(1))) != it->second.end()))){
                it->second.at(std::stoi(tmp_vector.at(1))).push_back(str_concat({tmp_vector.begin()+2, tmp_vector.end()}));
            } else {
                it->second.insert({std::stoi(tmp_vector.at(1)), {tmp_vector.begin()+2, tmp_vector.end()}}); 
            }
        }
    }
    in_stream.close();

    in_stream.open("tablica.txt", std::fstream::in);
    if(!in_stream.is_open()){
        std::cout << std::filesystem::current_path() << std::endl;
        std::cout << "tablica" << std::endl;
        return -1;
    }

    //izgradnja enka automata (enkaSimulator extenda enka)
    while(std::getline(in_stream, tmp_input)){
        if(tmp_input != ""){
            enkaSimulator automat;
            tmp_vector = str_split(tmp_input, " ");
            automat.pocetno_stanje = std::stoi(tmp_vector.at(1));
            automat.prihvatljivo_stanje = std::stoi(tmp_vector.at(2));
            automat.br_stanja = std::stoi(tmp_vector.at(3))+1;
            for(int i = 0; i < automat.br_stanja-1; i++){
                std::getline(in_stream, tmp_input);    
                if(i == 0){
                    automat.prijelazi.insert({1, string_to_vector("")});
                }
                automat.prijelazi.insert({std::stoi(tmp_input.substr(0, tmp_input.find(","))), string_to_vector(tmp_input.substr(tmp_input.find(",")+1, std::string::npos))});
            }       
            automat.initialize(SET);
            automaton_list.push_back(automat);
        }
    }
    in_stream.close();
   
    //ucitava cijeli ulazni stream u string
    std::string program_input(std::istreambuf_iterator<char>(std::cin), {});


    //algoritam leksickog analizatora
    int row_count = 1;
    trenutno_stanje = stanja_automata.at(0);
    int sim_state, pocetak = 1, posljednji = 0, zavrsetak = 0;
    std::vector<int> trenutni_automati, prihvatili, zadnji, vrte_se, broke;
    while(zavrsetak < program_input.size()-1){
        int tmp_poc, tmp_zav, tmp_poslj;
        tmp_poc = pocetak; tmp_zav = zavrsetak; tmp_poslj = posljednji;
        int izraz = -1, duljina = 0;

        //incijalizacija svih listi
        /*
        trenutni_automati - "bitset" vektor koji cuva za svaki automat vrijednost je li trenutno moze nastaviti parsirati niz
        prihvatili - "bitset" vektor koji cuva za svaki automat vrijednost prihvaca li trenutni niz znakova
        zadnji - "length" vektor koji cuva duljinu najduzeg niza za svaki automat koji je ili je bio trenutni za ovu trenutnni niz
        vrte_se - "bitset" vektor koji služi za dobivanje nizova maksimalne duljine
        */
        trenutni_automati.clear();
        prihvatili.clear();
        zadnji.clear();
        vrte_se.clear();
        broke.clear();
        
        for(int i = 0; i < automaton_list.size(); i++){
            trenutni_automati.push_back(0);
            prihvatili.push_back(0);
            automaton_list.at(i).initialize(RESET);  
            zadnji.push_back(0);
            vrte_se.push_back(0);
            broke.push_back(0);
        }

        for(auto x : akcije.at(trenutno_stanje)){
            trenutni_automati.at(x.first) = 1;
        }
  
        /*
        u svaki trenutni automat se salje sve duzi niz znakova dok svi automati ne prihvate najduzi niz
        prekid ako zadnji procitani znak u nizu znakova je $, postize se npr ab$c -> abc 
        prekid ako pokazaljka tmp_zav dosegne kraj ulaznog stringa
        */
        int count_prihvatili, count_trenutni, count_vrti_se;
        std::string lex_unit = "";
        do{
            tmp_input = program_input.substr(tmp_zav, 1);
            if(tmp_input == "$") break; //idem logikom da je prazan niz "$"
            lex_unit += tmp_input;

            for(int i = 0; i < automaton_list.size(); i++){
                if(trenutni_automati.at(i) == 1){
                    sim_state = simulacija_stog(automaton_list.at(i), lex_unit); //lex_unit
                    if(sim_state == -1){
                        trenutni_automati.at(i) = 0;
                        prihvatili.at(i) = 0; // zadnji bi trebao sacuvat stanje
                        vrte_se.at(i) = 0;
                        broke.at(i) = tmp_zav - tmp_poslj + 1;
                        //alternativa.at(i) = 1;
                    } else if(sim_state == 1){
                        prihvatili.at(i) = 1;
                        zadnji.at(i) = tmp_zav - tmp_poslj + 1;
                        vrte_se.at(i) = 1;
                    } else {
                        vrte_se.at(i) = 1;
                    }
                }
            }
            count_trenutni = 0, count_prihvatili = 0, count_vrti_se = 0;
            for(int i = 0; i < trenutni_automati.size(); i++){
                if(trenutni_automati.at(i) == 1){
                    count_trenutni++;
                    if(prihvatili.at(i) == 1){
                        count_prihvatili++;
                    }
                }
                if(vrte_se.at(i) ==1){
                    count_vrti_se++;
                }
            }

            tmp_zav++;
            if(tmp_zav == program_input.size()){
                break;
            }
        } while (count_trenutni != count_prihvatili || count_vrti_se > 0);

        //nalazi najduži niz pa onda prvi po redu od svih jednake (maksimalne) duljine
        for(int i = 0; i < zadnji.size();i++){
            if(zadnji.at(i) > 0 ){
                if(zadnji.at(i) > duljina){
                    duljina = zadnji.at(i);
                    izraz = i;
                }
            }
        }

        //isto kao gore ali za greške
        int broke_duljina = -1, broke_izraz = -1;
        for(int i = 0; i < broke.size();i++){
            if(broke.at(i) > 0 ){
                if(broke.at(i) > broke_duljina){
                    broke_duljina = broke.at(i);
                    broke_izraz = i;
                }
            }
        }

        trenutni_automati.clear();
        ///
        //prihvacanje ili odbacivanje leksicke jedinke i oporavak od greske
        if(izraz == -1){
            //std::cerr << program_input.substr(posljednji, 1); //ispis znaka kod odbacivanja greske
            zavrsetak = pocetak;
            pocetak++;
            posljednji++;
            //ako je leksicka se 
            if(vector_print.size() > 0){
                //za specificne slucajeve kada se prethodna leksicka jedinka treba odbaciti jer je greska otkrivena naknadno a istog je -tipa-
                if(lex_unit.size() > 1 && str_split(vector_print.at(vector_print.size()-1), " ").at(0) == akcije.at(trenutno_stanje).at(broke_izraz).at(0)){
                    if(str_split(vector_print.at(vector_print.size()-1), " ").at(2).size() == lex_unit.size()-1){
                        vector_print.pop_back();
                    }
                }
            }
        } else {
            int index_vrati_se = posljednji;
            posljednji = posljednji + duljina;
            pocetak = posljednji+1;
            zavrsetak = posljednji;
            tmp_vector = akcije.at(trenutno_stanje).at(izraz);
            int sizevec = tmp_vector.size();
            int tmp_rows = row_count;
            for(int i = 0; i < sizevec; i++){
                if(tmp_vector.at(i).substr(0, 8) == "VRATI_SE"){
                    int vrati_se = std::stoi(tmp_vector.at(i+1));
                    duljina = vrati_se;
                    zavrsetak = index_vrati_se+vrati_se;
                    pocetak = pocetak+vrati_se+1;
                    posljednji = index_vrati_se+vrati_se;
                    i++;
                }
            }
            for(int i = 0; i < sizevec; i++){
                if(tmp_vector.at(i).substr(0, 13) == "UDJI_U_STANJE"){
                    trenutno_stanje = tmp_vector.at(i+1);
                    i++;
                }
            }
            for(int i = 0; i < sizevec; i++){
                if(tmp_vector.at(i) == "-"){
                    //do nothing
                } else {
                    if(tmp_vector.at(i) == "NOVI_REDAK"){
                        row_count++;
                    } else if(std::find(identifikatori_leks_jedinki.begin(), identifikatori_leks_jedinki.end(), tmp_vector.at(i)) != identifikatori_leks_jedinki.end()){
                        std::string lex_jedinka = program_input.substr(posljednji-duljina, duljina);
                        vector_print.push_back(tmp_vector.at(i) + " " + std::to_string(tmp_rows) + " " + lex_jedinka);
                    }
                }
            }
            izraz = 0;
        }
    }
    //ispis leksickih leksickih jedinki, broja retka te dio ulaznog programa koji odgovara leksickoj jedinki
    for(auto x: vector_print){
        std::cout << x << std::endl;
    }
    
    return 0;
}
