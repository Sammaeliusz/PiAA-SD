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
    friend bool operator==(const film& f1, const film& f2){
        return f1.ocena==f2.ocena;
    }
    friend bool operator==(const film& f1, const int& i){
        return f1.ocena==i;
    }
};
std::ostream& operator<<(std::ostream& os, const film& f){
        return os<<"Id:"<<f.id<<" Nazwa:"<<f.nazwa<<" Ocena: "<<f.ocena<<"\n";
    }
std::ofstream logi("logi.txt", std::ios::out);
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
            std::cout<< "Not sorted at index " << i << ": " << data[i] << " > " << data[i+1] << std::endl;
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
bool quicksort(T* data, int rozmiar){
    bool equal = true;
    
    //printf("\n\n");
    T pivot = data[rozmiar/2];
    T *i = data;
    T *j = &data[rozmiar-2];
    T ftmp;
    //print_tab(data, rozmiar);
    if (rozmiar==2)
    {
        if (*data>*(data+1)){
            swap(data, data+1);
        }
        //print_tab(data, rozmiar);
        return true;
    }
    if(rozmiar==3){
        if(*data>*(data+1)){
            swap(data, data+1);
        }
        if(*data>*(data+2)){
            swap(data, data+2);
        }
        if(*(data+1)>*(data+2)){
            swap(data+1, data+2);
        }
        //print_tab(data, rozmiar);
        return true;
    }
    if(rozmiar>3){
        swap(&pivot, &data[rozmiar-1]);
        pivot = data[rozmiar-1];
        while(i<j){
            //print_tab(data, rozmiar);
            //printf("Left size: %d, Right size: %d\n", pivot-data, rozmiar-(pivot-data));
            //logi<< "i: " << i-data << ", j: " << j-data << ", pivot: " << pivot-data << "\n";
            //logi<< "Equal: " << equal << "\n";
            if(*i<pivot){
                i++;
                if(*i!=pivot){
                    equal = false;
                }
            }
            if(*j>=pivot){
                j--;
                if(*j!=pivot){
                    equal = false;
                }
            }
            if(i<j&&*i>=pivot&&*j<pivot){
                swap(i, j);
                i++;
                j--;
            }
        }
        if(rozmiar<=10){
            if(is_sorted(data, rozmiar)){
                return true;
            }
        }
        //printf("i: %d, j: %d, pivot: %d\n", i-data, j-data, pivot-data);
        //printf("Left size: %d, Right size: %d\n", i-data, rozmiar-(i-data)-1);
        //print_tab(data, rozmiar);
        if(equal){
            return true;
        }
        if(i==data){
            quicksort(data, rozmiar-1);
        }
        else{
            quicksort(i, rozmiar-(i-data));
            quicksort(data, i-data+1);
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
template <typename T>
void bucketsort(T* data, int rozmiar){
    int min = 1;
    int max = 10;
    T* end = data;
    dynamic_array<T>* buckets = new dynamic_array<T>[max-min+1];
    for(int j=0; j<rozmiar; j++){
        //
        for(int i=min; i<=max; i++){
            //printf("%d %d %d\n", buckets[i-min].start, buckets[i-min].size, buckets[i-min].capacity);
            if(data[j]==i){
                //printf("Pushing %d to bucket of size %d\n", data[j].id, buckets[i-min].capacity);
                push_begin(buckets+i-min, data[j]);
                break;
            }
            
        }
    } 
    for(int i=0; i<max-min+1; i++){
        //print_tab(buckets[i].start, buckets[i].capacity);
        while(buckets[i].capacity>0){
            *end = *pop_begin(buckets+i);
            end++;
        }
    }
    delete [] buckets;
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
            continue;
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
        if(ftmp.ocena<1||ftmp.ocena>10){
            i--;
        }
    }
    srand(time(0));
    int testsize = 100;
    int test[testsize];
    long long int tot_time = 0;
    film* copy= new film[size];
    for(int i=testsize; i>0; i--){
        test[testsize-i]=rand()%10+1;
    }
    int a[5] = {10,10,10,10,10};
    //print_tab(films, size);
    int avg = 10;
    //for(int i=0; i<avg; i++){
        printf("Test %d\n", 0);
        for(int j=0; j<size; j++){
            copy[j] = films[j];
        }
        print_tab(films, size);
        tic
        quicksort(films, size);
        toc
        tot_time += time;
        print_tab(films, size);
        printf("Is sorted: %d\n", is_sorted(films, size));
    //}
    
    //print_tab(copy, size);
    
    std::cout<<tot_time/avg<<" microseconds\n";
}