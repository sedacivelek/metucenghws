#include "Account.h"
using namespace std;

void sort(Transaction* &arr,int n){
    Transaction temp;
    for(int i=0;i<n;i++){
        for(int j=i+1;j<n;j++){
            if(arr[j]<arr[i]){
                temp = arr[i];
                arr[i]=arr[j];
                arr[j]=temp;
            }
        }
    }
}
Account::Account(){
    _id = -1;
    _activity = nullptr;
    _monthly_activity_frequency= nullptr;
}
Account::Account(int id, Transaction** const activity, int* monthly_activity_frequency){
    _id= id;
    _activity = new Transaction*[12];
    _monthly_activity_frequency = new int[12];
    for(int i=0;i<12;i++){
        _monthly_activity_frequency[i]=monthly_activity_frequency[i];
        int temp = monthly_activity_frequency[i];
        _activity[i] = new Transaction[monthly_activity_frequency[i]];
        for(int j=0;j<temp ;j++){
            _activity[i][j]=activity[i][j];
        }
        sort(_activity[i],_monthly_activity_frequency[i]);
    }
}
Account::~Account(){
    if(_activity!=nullptr){
        for(int i=0;i<12;i++){
        delete []_activity[i];
    }
    delete  []_activity;
    }
    if(_monthly_activity_frequency!=nullptr)
    delete  []_monthly_activity_frequency;
}
Account::Account(const Account& rhs){
    _id= rhs._id;
    _activity = new Transaction*[12];
    _monthly_activity_frequency = new int[12];
    for(int i=0;i<12;i++){
        _monthly_activity_frequency[i]=rhs._monthly_activity_frequency[i];
        int temp = rhs._monthly_activity_frequency[i];
        _activity[i] = new Transaction[rhs._monthly_activity_frequency[i]];
        for(int j=0;j<temp ;j++){
            _activity[i][j]=rhs._activity[i][j];
        }
    }
}
Account::Account(const Account& rhs, time_t start_date, time_t end_date){
    _id = rhs._id;
    _activity = new Transaction*[12];
    _monthly_activity_frequency = new int[12];
    for(int i=0;i<12;i++){
        _monthly_activity_frequency[i]=0;
    }
    for(int i=0;i<12;i++){
        for(int j=0;j<rhs._monthly_activity_frequency[i];j++){
            if(rhs._activity[i][j]>start_date && rhs._activity[i][j]<end_date){
                _monthly_activity_frequency[i]++;
            }
        }
    }
    
    for(int i=0;i<12;i++){
        _activity[i]=new Transaction[_monthly_activity_frequency[i]];
        for(int k=0;k<_monthly_activity_frequency[i];){
        for(int j=0;j<rhs._monthly_activity_frequency[i];j++){
                if(rhs._activity[i][j]>start_date && rhs._activity[i][j]<end_date){
                    _activity[i][k]=rhs._activity[i][j];
                    k++;
                }
            }
        }
    }
}
Account::Account(Account&& rhs){
    _id = rhs._id;
    _monthly_activity_frequency = rhs._monthly_activity_frequency;
    _activity=rhs._activity;
    rhs._activity=nullptr;
    rhs._monthly_activity_frequency=nullptr;
}
Account& Account::operator=(Account&& rhs){
    if(this!=&rhs){
        _id=rhs._id;
        _activity=rhs._activity;
        _monthly_activity_frequency=rhs._monthly_activity_frequency;
        rhs._activity=nullptr;
        rhs._monthly_activity_frequency=nullptr;
    }
    return *this;
}
Account& Account::operator=(const Account& rhs){
    if(this != &rhs){
        if(_activity!=nullptr){
            for(int i=0;i<12;i++){
                delete [] _activity[i];
            }
            delete [] _activity;
        }
    if(_monthly_activity_frequency!=nullptr)
    delete [] _monthly_activity_frequency;
    
    _id= rhs._id;
    _activity = new Transaction*[12];
    _monthly_activity_frequency = new int[12];
    for(int i=0;i<12;i++){
        _monthly_activity_frequency[i]=rhs._monthly_activity_frequency[i];
        
        _activity[i] = new Transaction[rhs._monthly_activity_frequency[i]];
        for(int j=0;j<rhs._monthly_activity_frequency[i];j++){
            _activity[i][j]=rhs._activity[i][j];
        }
        
    }
    }
    
    return *this;
}
bool Account::operator==(const Account& rhs) const{
    return (_id==rhs._id) ? true : false;
}
bool Account::operator==(int id) const{
    return (_id==id) ? true : false;
}
Account& Account::operator+=(const Account& rhs){
    int * tempfreq = new int[12];
    for(int i=0;i<12;i++){
        tempfreq[i]=_monthly_activity_frequency[i]+rhs._monthly_activity_frequency[i];
    }
    Transaction ** newac = new Transaction*[12];
    for(int i=0;i<12;i++){
        newac[i] = new Transaction[tempfreq[i]];
        int j=0;
        for(;j<_monthly_activity_frequency[i];j++){
            newac[i][j]=_activity[i][j];
        }
        for(int k=0;k<rhs._monthly_activity_frequency[i];k++){
            newac[i][j]=rhs._activity[i][k];
            j++;
        }
        sort(newac[i],_monthly_activity_frequency[i]);
    }
    for(int i=0;i<12;i++){
        delete [] this->_activity[i];
    }
    delete [] this -> _activity;
    delete [] this->_monthly_activity_frequency;
    _activity=newac;
    _monthly_activity_frequency=tempfreq;
    return *this;
}
double Account::balance(){
    double sum =0;
    for(int i=0;i<12;i++){
        for(int j=0;j<_monthly_activity_frequency[i];j++){
            sum=_activity[i][j]+sum;
        }
    }
    return sum;
}
double Account::balance(time_t end_date){
    double sum=0;
    for(int i=0;i<12;i++){
        for(int j=0;j<_monthly_activity_frequency[i];j++){
            if(_activity[i][j]<end_date)
            {
                sum=_activity[i][j]+sum;
            }
            
        }
    }
    return sum;
}
double Account::balance(time_t start_date, time_t end_date){
    double sum=0;
    for(int i=0;i<12;i++){
        for(int j=0;j<_monthly_activity_frequency[i];j++){
            if(_activity[i][j]<end_date && _activity[i][j]>start_date)
            {
                sum=_activity[i][j]+sum;
            }
            
        }
    }
    return sum;
}
std::ostream& operator<<(std::ostream& os, const Account& account){
    if(account._activity==nullptr || account._monthly_activity_frequency==nullptr){
        os<<"-1"<<endl;
    }
    else{
        os<< account._id << "\n";
        for(int i=0;i<12;i++){
            for(int j=0;j<account._monthly_activity_frequency[i];j++){ 
                os<< account._activity[i][j]; 
            }
        }
    }
    return os;
}