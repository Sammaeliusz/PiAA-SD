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
        return os<<"Id:"<<f.id<<" Nazwa:"<<f.nazwa<<" Ocena: "<<f.ocena<<"\n";
    }
int size;
film *films;
template <typename T>
void print_tab(T* data, int size){
    for(int i=0;i<size;i++){
        std::cout<<data[i]<<" ";
    }
    std::cout<<"\n";
}
template <typename T>
T* copy_subtable(T* data, int start, int end){
    T* copy = new T[end-start+1];
    for(int i=0;i<end-start+1;i++){
        copy[i] = data[start+i];
    }
    return copy;
}
template <typename T>
void quicksort(T* data, int rozmiar){
    T *pivot = &data[rozmiar/2];
    T *i = data;
    T *j = &data[rozmiar-1];
    T ftmp;
    //print_tab(data, rozmiar);
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
            //printf("l size %d\n", (pivot-data));
            //printf("r size %d\n", rozmiar-((pivot-data)));
            //printf("pivot %d\n", *pivot);
            //printf("data %d \n",*data);
            //printf("p-d %d \n",pivot-data);
            print_tab(data, rozmiar);
            quicksort(pivot+1, (pivot-data));
            quicksort(data, rozmiar-((pivot-data)));
        }
    }
}
template <typename T>
void mergesort(T* data, int rozmiar){
    int cell_size = 1;
    int lb, rb, le, re;
    while(cell_size<rozmiar){
        for(int i=0; i<rozmiar-1; i+=2*cell_size){
            lb = i;
            le = std::min(i+cell_size-1, rozmiar-1);
            rb = le+1;
            re = std::min(i+2*cell_size-1, rozmiar-1);
            if(rb < rozmiar){
                T* lewa = copy_subtable(data, lb, le);
                T* prawa = copy_subtable(data, rb, re);
                int j = 0, k=0, m=lb;
                while(j<le-lb+1&&k<re-rb+1){
                    if(lewa[j]<prawa[k]){
                        data[m] = lewa[j];
                        j++;
                    }
                    else{
                        data[m] = prawa[k];
                        k++;
                    }
                    m++;
                }
                while(j<le-lb+1){
                    data[m] = lewa[j];
                    j++;
                    m++;
                }
                while(k<re-rb+1){
                    data[m] = prawa[k];
                    k++;
                    m++;
                }
            }
        }
        cell_size*=2;
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
    int test[16] = {6,5,3,1,8,7,2,4, 9, 15, 12, 11, 10, 14, 13, 16};
    int a[8] = {1,2,3,4,5,6,7,8};
    print_tab(a, 8);
    tic
    quicksort(films, size);
    toc
    print_tab(a, 8);
    std::cout<<time;
}