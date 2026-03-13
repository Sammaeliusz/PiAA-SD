#include ".\..\universal.hpp"
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
void quicksort(film* filmy, int rozmiar){
    film *pivot = &filmy[rozmiar/2];
    film *i = filmy;
    film *j = &filmy[rozmiar-1];
    film ftmp;
    //print_films(filmy, rozmiar);
    if (rozmiar==2)
    {
        if (*i>*j){
            ftmp = *i;
            *i = *j;
            *j=ftmp;
        }
    }
    if(rozmiar>2){
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
        if(rozmiar>3){
            quicksort(pivot, (pivot-filmy)/sizeof(film));
            quicksort(filmy, rozmiar-((pivot-filmy)/sizeof(film))-1);
        }
    }
}
int main(int argc, char *argv[]){
    size = atoi(argv[1]);
    films = new film[size];
    std::ifstream plik("projekt1_dane.csv", std::ios::in);
    std::string record;
    std::string strtmp;
    film ftmp;
    std::getline(plik, record);
    for(int i=0;i<size;++i){
        std::getline(plik, record);
        std::stringstream strstr(record);
        std::getline(strstr, strtmp,',');
        if(strtmp[0]=='"'){
            strtmp = strtmp.substr(1,strtmp.size()-1);
        }
        ftmp.id = std::stoll(strtmp);
        std::getline(strstr, ftmp.nazwa,',');
        std::getline(strstr, strtmp, ',');
        while((strtmp[0]<48||strtmp[0]>57)){
            ftmp.nazwa += ","+strtmp;
            std::getline(strstr, strtmp, ',');
        }
        ftmp.ocena = std::stoi(strtmp);
        films[i] = ftmp;
    }
    print_films(films, size);
    tic
    //quicksort(films, size);
    toc
    //print_films(films, size);
    std::cout<<time;
}