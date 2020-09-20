#include "Transaction.h"

Transaction::Transaction(){
    _amount = -1;
    _date= -1;
    //emin değilim
}
Transaction::Transaction(double amount, time_t date){
    _amount=amount;
    _date=date;
}
Transaction::Transaction(const Transaction& rhs){
    _amount=rhs._amount;
    _date=rhs._date;

}
bool Transaction::operator<(const Transaction& rhs) const{
    if(_date<rhs._date){
        return true;
    }
    else{
        return false; 
    }
}
bool Transaction::operator>(const Transaction& rhs) const{
    if(_date>rhs._date){
        return true;
    }
    else{
        return false; 
    }
}
bool Transaction::operator<(const time_t date) const{
    if(_date<date){
        return true;
    }
    else{
        return false;
    }
}
bool Transaction::operator>(const time_t date) const{
    if(_date>date){
        return true;
    }
    else{
        return false;
    }
}
double Transaction::operator+(const Transaction& rhs){
    double sum=_amount+rhs._amount;
    return sum;
}
double Transaction::operator+(const double add){
    double sum =_amount+add;
    return sum;
}
Transaction& Transaction::operator=(const Transaction& rhs){
    _amount=rhs._amount;
    _date=rhs._date;
    return *this;
}
std::ostream& operator<<(std::ostream& os, const Transaction& transaction){
    double tmpamount = transaction._amount;
    time_t tmp = transaction._date;
    int hour,min,sec,day,mon,year;
    tm *tmptime = localtime(&tmp);
    hour= tmptime->tm_hour;
    min = tmptime->tm_min;
    sec = tmptime->tm_sec;
    day = tmptime->tm_mday;
    mon = (tmptime->tm_mon)+1;
    year = 1900+(tmptime->tm_year);
    os<< tmpamount << "\t" << "-" << "\t"<<hour<<":"<<min<<":"<<sec<<"-"<<day<<"/"<<mon<<"/"<<year <<"\n";
    return os;
}