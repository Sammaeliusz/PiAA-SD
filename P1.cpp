#include ".\universal.hpp"
#include "P1-SD.hpp"
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
    friend bool operator<=(const film& f1, const film& f2){
        return f1.ocena<=f2.ocena;
    }
    friend bool operator!=(const film& f1, const film& f2){
        return f1.ocena!=f2.ocena;
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
template<typename T>
bool is_sorted(T* data, int size){
    for(int i=0;i<size-1;i++){
        if(data[i]>data[i+1]){
            return false;
        }
    }
    return true;
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
        if(*pivot>*j){
            ftmp = *pivot;
            *pivot = *j;
            *j=ftmp;
        }
    }
    if(rozmiar>3){
        bool equal = true;
        while(i<j){
            print_tab(data, rozmiar);
            printf("Left size: %d, Right size: %d\n", pivot-data, rozmiar-(pivot-data));
            printf("Pivot: %d\n", *pivot);
            printf("i: %d, j: %d, pivot: %d\n", i-data, j-data, pivot-data);
            printf("Lvalue: %d\n", *i);
            printf("Rvalue: %d\n \n", *j);
            if(*i!=*pivot||*j!=*pivot){
                    equal = false;
            }
            if(*i<*pivot&&i<pivot){
                i++;
                if(*i!=*pivot){
                    equal = false;
                }
            }
            if(*j>*pivot&&j>pivot){
                j--;
                if(*j!=*pivot){
                    equal = false;
                }
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
                if(j<&data[rozmiar-1]){
                    j++;
                }
                *pivot = ftmp;
            }
            else if(j==pivot&&i!=j){
                ftmp = *pivot;
                *pivot = *(pivot-1);
                pivot--;
                if(i>data){
                    i--;
                }
                *pivot = ftmp;
            }
        }
        if(equal){
            return;
        }
        if(rozmiar<=10){
            if(is_sorted(data, rozmiar)){
                return;
            }
        }
        quicksort(pivot, rozmiar-(pivot-data));
        quicksort(data, pivot-data);
        
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
template <typename T>
void bucketsort(T* data, int rozmiar){
        int min = 1;
        int max = 10;
    T out[rozmiar];
    T* end = out;
    for(int j=0; j<rozmiar; j++){
        dynamic_array<T>* bucket = new dynamic_array<T>;
        printf("%d %d %d\n", bucket->start, bucket->size, bucket->capacity);
        for(int i=min; i<=max; i++){
            if(data[j].ocena==i){
                push_back(bucket, data[j]);
            }
            
        }
        print_tab(bucket->start, bucket->capacity);
        for(int j=0; j<bucket->capacity; j++){
            *end = bucket->start[j];
            end++;
        }
    } 
    print_tab(out, rozmiar);
    for(int i=0; i<rozmiar; i++){
        data[i] = out[i];
    }
}
int main(int argc, char *argv[]){
    size = atoi(argv[1]);
    int offset = atoi(argv[2]);
    films = new film[size];
    std::ifstream plik("projekt1_dane.csv", std::ios::in);
    std::string record;
    std::string strtmp;
    film ftmp;
    std::getline(plik, record);
    for(int i=0;i<offset;++i){
        std::getline(plik, record);
    }
    for(int i=0;i<size;++i){
        std::getline(plik, record);
        if(record[record.size()-1]==','){
            continue;;
        }
        std::stringstream strstr(record);
        std::getline(strstr, strtmp,',');
        if(strtmp[0]=='"'){
            strtmp = strtmp.substr(1,strtmp.size()-1);
        }
        ftmp.id = std::stoll(strtmp);
        std::getline(strstr, ftmp.nazwa);
        if(ftmp.nazwa[ftmp.nazwa.size()-3]=='0'){
            ftmp.nazwa = ftmp.nazwa.substr(0,ftmp.nazwa.size()-4);
            ftmp.ocena = 10;
        }
        else{
            ftmp.ocena = (int)ftmp.nazwa[ftmp.nazwa.size()-3]-(int)'0';
            ftmp.nazwa = ftmp.nazwa.substr(0,ftmp.nazwa.size()-3);
        }
        films[i] = ftmp;
    }
    srand(time(0));
    int testsize = 20;
    int test[testsize];
    for(int i=testsize; i>0; i--){
        test[testsize-i]=rand()%(testsize-5)+1;
    }
    int a[5] = {10,10,10,14,11};
    print_tab(films, size);
    tic
    quicksort(films, size);
    toc
    print_tab(films, size);
    printf("Is sorted: %d\n", is_sorted(films, size));
    std::cout<<time;
}