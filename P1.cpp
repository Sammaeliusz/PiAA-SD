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
void quicksort(T* data, int rozmiar){
    if (rozmiar<=1){
        return;
    }
    if (rozmiar==2)
    {
        if (*data>*(data+1)){
            swap(data, data+1);
        }
    }
    if(rozmiar>2){
        T pivot = data[rozmiar/2];
        T *i = data;
        T *j = &data[rozmiar-1];
        T ftmp;
        while(i<=j){
            while(*i<pivot){
                i++;
            }
            while(*j>pivot){
                j--;
            }
            if(i<=j){
                swap(i, j);
                i++;
                j--;
            }
        }
        quicksort(i, rozmiar-(i-data));
        quicksort(data, j-data+1);
    }
}
template <typename T>
void mergesort(T* data, int rozmiar){
    int cell_size = 1;
    int lb, rb, le, re;
    T* lewa;
    T* prawa;
    while(cell_size<rozmiar){
        for(int i=0; i<rozmiar-1; i+=2*cell_size){
            lb = i;
            le = min(i+cell_size-1, rozmiar-1);
            rb = le+1;
            re = min(i+2*cell_size-1, rozmiar-1);
            if(rb < rozmiar){
                lewa = copy_subtable(data, lb, le);
                prawa = copy_subtable(data, rb, re);
                int j = 0, k=0, m=lb;
                //Scalanie
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
                //Dociągnięcie drugiej części
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
                delete[] lewa;
                delete[] prawa;
            }
        }
        cell_size*=2;
    }
}
template <typename T>
void bucketsort(T* data, int rozmiar, int min, int max){
    T* end = data;
    dynamic_array<T>* buckets = new dynamic_array<T>[max-min+1];
    for(int j=0; j<rozmiar; j++){
        int bucket_index = data[j].ocena - min;
        push_back(buckets+bucket_index, data[j]);
    } 
    for(int i=0; i<max-min+1; i++){
        while(buckets[i].size>0){
            *end = pop_back(buckets+i);
            end++;
        }
    }
    for(int i=0; i<max-min+1; i++){
        free(buckets[i].start);
        buckets[i].size = 0;
        buckets[i].capacity = 0;
    }
    delete[] buckets;
}
int main(int argc, char *argv[]){
    size = atoi(argv[1]);
    int offset = atoi(argv[2]);
    films = new film[size];
    std::ifstream plik("./../projekt1_dane.csv", std::ios::in);
    std::string record;
    std::string strtmp;
    film ftmp;
    std::getline(plik, record);
    for(int i=0;i<offset;++i){
        std::getline(plik, record);
    }
    int i=0;
    while(i<size && std::getline(plik, record)){
        std::stringstream strstr(record);
        std::getline(strstr, strtmp,',');
        //std::cout<<"Id string: "<<strtmp<<"\n";
        ftmp.id = std::stoll(strtmp);
        std::getline(strstr, ftmp.nazwa);
        if(ftmp.nazwa[0]=='\"'){
            ftmp.nazwa.erase(0,1);
            int last_quote = ftmp.nazwa.find_last_of('\"');
            std::string ocena_str = ftmp.nazwa.substr(last_quote+2, ftmp.nazwa.size()-last_quote-2);
            ftmp.nazwa.erase(last_quote, ftmp.nazwa.size()-last_quote);
            if(!ocena_str.empty()){
                ftmp.ocena = std::stoll(ocena_str);
                films[i] = ftmp;
                i++;
            }
        }
        else{
            int last_comma = ftmp.nazwa.find_last_of(',');
            std::string ocena_str = ftmp.nazwa.substr(last_comma+1, ftmp.nazwa.size()-last_comma-1);
            ftmp.nazwa.erase(last_comma, ftmp.nazwa.size()-last_comma);
            if(!ocena_str.empty()){
                ftmp.ocena = std::stoll(ocena_str);
                films[i] = ftmp;
                i++;
            }
        }
    }
    size = i-1;
    film* copy= new film[size];
    long long int tot_time = 0;
    //print_tab(films, size);
    for(int i=0; i<size; i++){
        if(films[i].ocena<1||films[i].ocena>10){
            std::cout<<"Error at index "<<i<<": "<<films[i]<<"\n";
            return -1;
        }
    }
    int avg = 50;
    for(int i=0; i<avg; i++){
        printf("Test %d\n", i);
        for(int j=0; j<size; j++){
            copy[j] = films[j];
        }
        //print_tab(copy, size);
        tic
        quicksort(copy, size);
        //mergesort(copy, size);
        //bucketsort(copy, size, 1, 10);
        toc
        tot_time += time;
        printf("%d is sorted: %d\n",i, is_sorted(copy, size));
        //print_tab(copy, size);
        
    }
    printf("Moda: %d\n", (copy[size/2].ocena+copy[size/2+1].ocena)/2);
    float avg_tab = 0;
    for(int i=0; i<size; i++){
        avg_tab += copy[i].ocena;
    }
    printf("Average: %f\n", avg_tab/size);
    
    std::cout<<tot_time/avg<<" microseconds\n";
}
