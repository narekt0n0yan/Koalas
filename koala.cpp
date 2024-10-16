#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm> 
#include <iomanip>
#include <map>
#include <string>


void printvector(std::vector<std::string> V){
    for (std::string elem : V){
        std::cout << std::setw(8) << elem << " ,"; 
    }
    std::cout<< std::endl;
}

void printmatrix(std::vector<std::vector<std::string>> M){
    for (const auto row : M) {
        for (const auto value : row) {      
            std::cout << std::setw(8) << value ;
        }
        std::cout << std::endl;  
    }
}

void printmatrixpointer(std::vector<std::vector<std::string*>> M){
    for (const auto row : M) {
        for (const auto value : row) {
            std::cout << std::setw(16) << *value;
        }
        std::cout << std::endl;    
    }
}

bool is_in(std::vector<std::string> V, std::string S){
    for (int i = 0; i < V.size(); i++){ 
        if (S == V[i]){
            return true;
        }
    }
    return false;
}

bool is_string(std::string s){
    char m = s[0]; 
    if (m - 'a' <= 29 && m -'a' >= 0) {
        return true;
    }
    else if (m - 'A' <= 29 && m - 'A' >= 0) {
        return true;
    }
    else{
        return false;
    }
}


void printMap(std::map<std::string, std::vector<int>> MadeFeatureMap) {
        for (const auto& pair : MadeFeatureMap) {
            std::cout << "Feature:" << pair.first << "-->";
            for (int value : pair.second) {
                std::cout << value << " ";
            }
            std::cout << std::endl;
        }
    }

class Koala{
public:
    std::string m_DATAfileName;
    std::vector<std::vector<std::string>> m_DATAmatrixasword;
    std::vector<std::vector<std::string*>> m_DATAmatrixasFeature;
    std::vector<std::string>  m_FeaturesVector;
  

    Koala(std::string DATAfileName) {
        m_DATAmatrixasFeature.reserve(100);
        m_DATAmatrixasword.reserve(100);
        m_FeaturesVector.reserve(100);
        m_DATAfileName = DATAfileName;
    
        std::ifstream myCSVfile(m_DATAfileName);
        if (!myCSVfile.is_open()) {
            std::cout << "Error: Could not open the file!" << std::endl;
        }

        std::string mystringCSV;
        std::vector<std::string> DATAmatrixaslineStr;

        while (getline(myCSVfile,mystringCSV) ) {
            DATAmatrixaslineStr.push_back(mystringCSV);
        }

        myCSVfile.close();

        std::istringstream tt(DATAmatrixaslineStr[0]);
        std::string touch;
        while (getline(tt,touch,',')) {
            m_FeaturesVector.push_back(touch);
        }
    
        for (int i = 1; i < DATAmatrixaslineStr.size(); ++i) {
            std::istringstream ss(DATAmatrixaslineStr[i]); 
            std::string token;
 
            std::vector<std::string> row;
            row.reserve(100);
            while (getline(ss,token,',')) {
                row.push_back(token);
            }
            m_DATAmatrixasword.push_back(row);
        }
        DATAmatrixaslineStr.clear();

        if (!m_DATAmatrixasword.empty()) {
            for (int i = 0; i < m_DATAmatrixasword[0].size(); ++i) {
                std::vector<std::string*> row;
                row.reserve(100);
                for (int j = 0; j < m_DATAmatrixasword.size(); ++j) {
                    row.push_back(&(m_DATAmatrixasword[j][i]));
                }
                m_DATAmatrixasFeature.push_back(row);
            }
        } 
        else {
            std::cout << "empty vector" <<std::endl;
        } 
    }

    void add_feature(std::string new_feature) {
        m_FeaturesVector.push_back(new_feature);
        std::vector<std::string*> row;
        row.reserve(100);
        for (int i = 0; i < m_DATAmatrixasword.size(); ++i){
            m_DATAmatrixasword[i].push_back("0");
            row.push_back(&(m_DATAmatrixasword[i].back()));
        }
        m_DATAmatrixasFeature.push_back(row);
    }

    void insert(int i, int j , std::string value) {
        m_DATAmatrixasword[i][j] = value;
    }

    void remove(std::string feature){
        int m = 0;
        for (std::string elem : m_FeaturesVector){
            if (elem == feature){
                m_FeaturesVector.erase(std::remove(m_FeaturesVector.begin(), m_FeaturesVector.end(), feature), m_FeaturesVector.end());
                break;
            }
            else{
                m++;
            }
        }
        for (int i = 0; i <m_DATAmatrixasword.size(); ++i){
            m_DATAmatrixasword[i].erase(m_DATAmatrixasword[i].begin() + m);
        }
        m_DATAmatrixasFeature.erase(m_DATAmatrixasFeature.begin() + m);
    }

    void OneHotencoding(){
        std::vector<std::string> featurerow;
        int featureindex = 0;

        for (int i = 0; i < m_DATAmatrixasFeature.size(); i++){
            for (int j = 0; j<m_DATAmatrixasFeature[i].size(); j++){
                if (is_string(*(m_DATAmatrixasFeature[i][j])) && !(is_in(featurerow, *(m_DATAmatrixasFeature[i][j])))){
                    featurerow.push_back(*(m_DATAmatrixasFeature[i][j]));
                    featureindex = i;
                }
            }
        }

        for(int k = 0; k<featurerow.size(); k++){ 
            add_feature(m_FeaturesVector[featureindex] + "_" + featurerow[k]);
            for(int g = 0; g < m_DATAmatrixasword.size(); g++){
                if (m_DATAmatrixasword[g][featureindex] == featurerow[k]){
                    insert(g, (m_DATAmatrixasword[g].size() - 1), "1");
                }
            }
        }
        remove(m_FeaturesVector[featureindex]);
    }
};    

int main(){
    Koala Data("winequality-red (1).csv");
    

    printvector(Data.m_FeaturesVector);
    std::cout << std::endl;
    std::cout << std::endl;
    printmatrix(Data.m_DATAmatrixasword);
    std::cout << std::endl;
    std::cout << std::endl;
    printmatrixpointer(Data.m_DATAmatrixasFeature);
    std::cout << std::endl;
    std::cout << std::endl;

    Data.OneHotencoding();
    printvector(Data.m_FeaturesVector);
    printmatrix(Data.m_DATAmatrixasword);
    

    return 0;
};




