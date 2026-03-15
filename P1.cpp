#include ".\universal.hpp"
struct film
{
    long long int id;
    std::string nazwa;
    int ocena;
    friend bool operator<(const film& f1, const film& f2){
        return f1.ocena<f2.ocena;
    }
    friend bool operator>(const film& f1, const film& f2){
        return f1.ocena>f2.ocena;
    }
    friend bool operator>=(const film& f1, const film& f2){
        return f1.ocena>=f2.ocena;
    }
};
std::ostream& operator<<(std::ostream& os, const film& f){
        return os<<"Id:"<<f.id<<"Nazwa:"<<f.nazwa<<"Ocena: "<<f.ocena<<"\n";
    }
int size;
film *films;
void print_films(film *filmy, int size){
    for(int i=0;i<size;i++){
        std::cout<<filmy[i];
    }
    std::cout<<"\n";
}
template<typename T>
void quicksort(T* filmy, int rozmiar){
    int tmptab[3] = {filmy[0].ocena-(filmy[rozmiar/2].ocena+filmy[rozmiar/2+1].ocena)/2, filmy[1].ocena-(filmy[rozmiar/2-1].ocena+filmy[rozmiar/2+1].ocena)/2, filmy[2].ocena-(filmy[rozmiar/2-1].ocena+filmy[rozmiar/2].ocena)/2};
    int pp = rozmiar/2;
    if(tmptab[0]<tmptab[1]&&tmptab[0]<tmptab[2]){
        pp = rozmiar/2-1;
    }
    else if(tmptab[2]<tmptab[0]&&tmptab[2]<tmptab[1]){
        pp = rozmiar/2+1;
        
    }
    T *pivot = &filmy[rozmiar/2];
    T *i = filmy;
    T *j = &filmy[rozmiar-1];
    T ftmp;
    if (rozmiar==2)
    {
        if (*i>*j){
            ftmp = *i;
            *i = *j;
            *j=ftmp;
        }
    }
    if(rozmiar==3){
        if(*i>*j){
            ftmp = *i;
            *i = *j;
            *j=ftmp;
        }
        if(*i>*pivot){
            ftmp = *i;
            *i = *pivot;
            *pivot=ftmp;
        }
        if(*j<*pivot){
            ftmp = *j;
            *j = *pivot;
            *pivot=ftmp;
        }
    }
    if(rozmiar>3){
        while(i!=j){
            while(*i<*pivot&&i<pivot){
                i++;
            }
            while(*j>=*pivot&&j>pivot){
                j--;
            }
            if(j>pivot&&i<pivot){
                ftmp = *i;
                *i=*j;
                *j=ftmp;
                i++;
                j--;
            }
            else if(i==pivot&&i!=j){
                ftmp = *pivot;
                *pivot = *(pivot+1);
                pivot++;
                *pivot = ftmp;
            }
            else if(j==pivot&&i!=j){
                ftmp = *pivot;
                *pivot = *(pivot-1);
                pivot--;
                *pivot = ftmp;
            }

        }
        quicksort(filmy, (pivot-filmy));
        quicksort(pivot+1, rozmiar-(pivot-filmy)-1);
    }
}
int main(int argc, char *argv[]){
    std::srand(std::time(0));
    size = atoi(argv[1]);
    films = new film[size];
    std::ifstream plik("projekt1_dane.csv", std::ios::in);
    std::string record;
    std::string strtmp;
    film ftmp;
    std::getline(plik, record);
    for(int i=0;i<size;++i){
        do{
        std::getline(plik, record);
        }while(record[record.size()-1]!='0');
        std::stringstream strstr(record);
        std::getline(strstr, strtmp,',');
        if(strtmp[0]=='"'){
            strtmp = strtmp.substr(1,strtmp.size()-1);
        }
        ftmp.id = std::stoll(strtmp);
        std::getline(strstr, ftmp.nazwa,',');
        std::getline(strstr, strtmp, ',');
        while(strtmp[strtmp.size()-2]!='.'||strtmp[strtmp.size()-1]!='0'){
            ftmp.nazwa += ","+strtmp;
            std::getline(strstr, strtmp, ',');
        }
        ftmp.ocena = std::stoi(strtmp);
        films[i] = ftmp;
    }
    print_films(films, size);
    tic
    quicksort(films, size);
    toc
    print_films(films, size);
    std::cout<<time;
}