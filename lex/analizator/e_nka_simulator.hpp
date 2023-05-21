#include <string>
#include <vector>
#include "e_nka.hpp"
#include <stack>

//klasa simulatora automata, nasljeduje klasu eNKA te prosiruje sa skupom vektora X i Y te stoga kojim se simuliraju prijelazi i epsilon prijelazi 
class enkaSimulator : public eNKA
{
public:
    std::vector<bool> X, Y;
    std::stack<int> stog;
    int duljina;

    //funkcija za postavljanja i resetiranje automata, stanje 0 (SET) ili 1 (RESET)
    void initialize(int stanje){
        if (stanje == 0){
            for (int i = 0; i < br_stanja; i++){
                X.push_back(0);
                Y.push_back(0);
            }
            X.at(0) = 1;
            duljina = 0;
        }
        else{
            for (int i = 0; i < br_stanja; i++){
                X.at(i) = 0;
                Y.at(i) = 0;
                if(!stog.empty()){
                    stog.pop();
                }
            }
            X.at(0) = 1;
            duljina = 0;
        }
    }
};

void epsilon_jedan(enkaSimulator &automat){
    //simulacija epsilon prijelaza (racunanja epsilon okruzenja)
    while(!automat.stog.empty()){
        int t = automat.stog.top();
        automat.stog.pop();
        for(auto x: automat.prijelazi.at(t)){
            for(auto y: str_split(x, " ")){
                if (y != ""){
                    int state = std::stoi(y.substr(y.find("->") + 2, std::string::npos));
                    if (x.substr(0, 1) == "$" && automat.Y.at(state) != 1)
                    {
                        automat.Y.at(state) = 1;
                        automat.stog.push(state);
                    }
                }
            }
        }
    }
    automat.X = automat.Y;
    for(auto y : automat.Y){
        y = 0;
    }
}

int simulacija_stog(enkaSimulator &automat, std::string input){
    automat.initialize(1);
    int moved = 0;

    //pocetni epsilon prijelaz, stavlja se pocetno stanje (uvijek 0 radi implementacije enka automata) na stog
    automat.stog.push(0);
    epsilon_jedan(automat);

    //simulacija enka, za svaki procitani znak stavlja se stanje u koje prelazi na stog te naknadno se ponovo skup stanja ureduje epsilon prijelazima
    for (int j = 0; j < input.size(); j++){
        if(j < input.size()){
            moved = 0;
        }  
        std::string tmp_input = input.substr(j,1);
        if(tmp_input == "\n") tmp_input = "\\n";
        if(tmp_input == "\t") tmp_input = "\\t";  
        for (int i = 0; i < automat.X.size(); i++){
            if (automat.X.at(i) == 1){
                for (auto y : automat.prijelazi.at(i)){
                    if (y.substr(0, y.find("->")) == tmp_input){
                        int state = std::stoi(y.substr(y.find("->") + 2, std::string::npos));
                        //if(automat.Y.at(state) == 0){
                            automat.stog.push(state);
                            moved++;
                        //}
                        automat.Y.at(state) = 1;
                    }
                }
            }
        }
        for(auto y: automat.Y){
            y = 0;
        }
        epsilon_jedan(automat);
    }
    for (auto x : automat.Y){
        x = 0;
    }
    //ako se dogodio pomak znaci da se automat i dalje vrti, ako se nije dogodio pomak i automat je u prihvatljivom stanju onda automat prihvaca niz inace ne prihvaca
    if (moved == 0){
        return -1;
    }
    else if(automat.X.at(automat.prihvatljivo_stanje) == 1){
        return 1;
    }
    return 0;
}