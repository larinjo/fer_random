#include <vector>
#include <string>
#include <algorithm>
#pragma once

//vraca vektor stringova koji su nastali dijeljenjem pocetnog stringa sa substringom temp
inline std::vector<std::string> str_split(std::string in_str, const std::string temp){
    std::vector<std::string> result_str;
    while(true){
        int index;
        if(in_str.find(temp) == std::string::npos){
            result_str.push_back(in_str);
            break;
        }
        index = in_str.find(temp);
        result_str.push_back(in_str.substr(0, index));
        in_str = in_str.substr(index+1, in_str.length() - in_str.substr(0, index).length());
    }
    return result_str;
}

//pretvara string u vector tipa string
inline std::vector<std::string> string_to_vector(const std::string str){
    std::vector<std::string> result_vector;
    result_vector.push_back(str);
    return result_vector;
}

//spaja stringove u vektoru u veliki string u odredenom formatu
inline std::string str_concat(std::vector<std::string> in_vector){
    std::string result_str = "";
    for(int i = 0; i < in_vector.size(); i++){
        if(i == 0){
            if((in_vector.at(i).find("\n") == 0)){
                result_str += ("\\n"+in_vector.at(i).substr(1, std::string::npos));
            } else if((in_vector.at(i).find("\t") == 0)){
                result_str += ("\\t"+in_vector.at(i).substr(1, std::string::npos));
            } else {
                result_str += in_vector.at(i);
            }
        } else {
            if((in_vector.at(i).find("\n") == 0)){
                result_str += (" \\n" + in_vector.at(i).substr(1,std::string::npos));
            } else if((in_vector.at(i).find("\t") == 0)){
                result_str += (" \\t" + in_vector.at(i).substr(1,std::string::npos));
            } else {
                result_str += (" " + in_vector.at(i));
            }
        }
    }
    return result_str;
}
