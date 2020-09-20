#include "Bank.h"
using namespace std;
Bank::Bank(){
    _bank_name="not_defined";
    _user_count=0;
    _users=nullptr;
}
Bank::Bank(std::string bank_name, Account* const users, int user_count){
    _bank_name=bank_name;
    _user_count=user_count;
    _users = new Account[user_count];
    for(int i=0;i<user_count;i++){
        _users[i]=users[i];
    }
}
Bank::~Bank(){
    if(_users!=nullptr){
        delete []_users;
    }
}
Bank::Bank(const Bank& rhs){
    _bank_name=rhs._bank_name;
    _user_count=rhs._user_count;
    _users = new Account[rhs._user_count];
    for(int i=0;i<_user_count;i++){
        _users[i]=rhs._users[i];
    }
}
int isin(Account* source,int n,Account & acc){
    for(int i=0;i<n;i++)
    {
        if(source[i]==acc)
        return i;
    }
    return -1;
}
Bank& Bank::operator+=(const Bank& rhs){
    int flag=0;
    for(int i=0;i<_user_count;i++){
        for(int j=0;j<rhs._user_count;j++){
            if(_users[i]==rhs._users[j]){
                flag++;
                break;
            }
        }
    }
    int nuser = _user_count+rhs._user_count-flag;
    Account * tempus = new Account[nuser];
    int i=0;
    for(i=0;i<_user_count;i++){
        tempus[i]=_users[i];
    }
    for(int j=0;j<rhs._user_count;j++){
       int a= isin(_users,_user_count,rhs._users[j]);
        if(a!=-1){
            tempus[a]+=rhs._users[j];
        }
        else{
            tempus[i]=rhs._users[j];
            i++;
        }
        
    }
    delete []_users;
    _users = tempus;
    _user_count=nuser;
    return *this;


}
Bank& Bank::operator+=(const Account& new_acc){
    bool flag=false;
    int merged;
    for(int i=0;i<_user_count;i++){
        if(_users[i]==new_acc){
            flag=true;
            merged=i;
            break;
        }
    }
    if(flag){
        _users[merged]+=new_acc;
    }
    else{
        int i;
        Account * tempus = new Account[_user_count+1];
        for(i=0;i<_user_count;i++){
            tempus[i]=_users[i];
        }
        tempus[i]=new_acc;
        delete []_users;
        _users=tempus;
        _user_count++;
    }
    return *this;
}
Account& Bank::operator[](int account_id){
    bool flag;
    int ret;
    for(int i=0;i<_user_count;i++){
        if(_users[i]==account_id){
            flag=true;
            return _users[i];
        }
    }
    return _users[0];
}
std::ostream& operator<<(std::ostream& os, const Bank& bank){
    int sum =0;
    double balance[bank._user_count][12];
    for(int i=0;i<bank._user_count;i++){
        sum+=bank._users[i].balance();
    }
    for(int i=0;i<bank._user_count;i++){
        struct tm date1={0};
        date1.tm_year=119; date1.tm_mon=1; date1.tm_mday=1;
        date1.tm_hour=0;date1.tm_min=0;date1.tm_sec=0;
        time_t t1 = mktime(&date1);
        balance[i][0]=bank._users[i].balance(t1);

        struct tm date2={0};
        date2.tm_year=119; date2.tm_mon=2; date2.tm_mday=1;
        date2.tm_hour=0;date2.tm_min=0;date2.tm_sec=0;
        time_t t2 = mktime(&date2);
        balance[i][1]=bank._users[i].balance(t2)-balance[i][0];

        struct tm date3={0};
        date3.tm_year=119; date3.tm_mon=3; date3.tm_mday=1;
        date3.tm_hour=0;date3.tm_min=0;date3.tm_sec=0;
        time_t t3 = mktime(&date3);
        balance[i][2]=bank._users[i].balance(t3)-balance[i][1];

        struct tm date4={0};
        date4.tm_year=119; date4.tm_mon=4; date4.tm_mday=1;
        date4.tm_hour=0;date4.tm_min=0;date4.tm_sec=0;
        time_t t4 = mktime(&date4);
        balance[i][3]=bank._users[i].balance(t4)-balance[i][2];

        struct tm date5={0};
        date5.tm_year=119; date5.tm_mon=5; date5.tm_mday=1;
        date5.tm_hour=0;date5.tm_min=0;date5.tm_sec=0;
        time_t t5 = mktime(&date5);
        balance[i][4]=bank._users[i].balance(t5)-balance[i][3];

        struct tm date6={0};
        date6.tm_year=119; date6.tm_mon=6; date6.tm_mday=1;
        date6.tm_hour=0;date6.tm_min=0;date6.tm_sec=0;
        time_t t6 = mktime(&date6);
        balance[i][5]=bank._users[i].balance(t6)-balance[i][4];

        struct tm date7={0};
        date7.tm_year=119; date7.tm_mon=7; date7.tm_mday=1;
        date7.tm_hour=0;date7.tm_min=0;date7.tm_sec=0;
        time_t t7 = mktime(&date7);
        balance[i][6]=bank._users[i].balance(t7)-balance[i][5];

        struct tm date8={0};
        date8.tm_year=119; date8.tm_mon=8; date8.tm_mday=1;
        date8.tm_hour=0;date8.tm_min=0;date8.tm_sec=0;
        time_t t8 = mktime(&date8);
        balance[i][7]=bank._users[i].balance(t8)-balance[i][6];

        struct tm date9={0};
        date9.tm_year=119; date9.tm_mon=9; date9.tm_mday=1;
        date9.tm_hour=0;date9.tm_min=0;date9.tm_sec=0;
        time_t t9 = mktime(&date9);
        balance[i][8]=bank._users[i].balance(t9)-balance[i][7];

        struct tm date10={0};
        date10.tm_year=119; date10.tm_mon=10; date10.tm_mday=1;
        date10.tm_hour=0;date10.tm_min=0;date10.tm_sec=0;
        time_t t10 = mktime(&date10);
        balance[i][9]=bank._users[i].balance(t10)-balance[i][8];
        
        struct tm date11={0};
        date11.tm_year=119; date11.tm_mon=11; date11.tm_mday=1;
        date11.tm_hour=0;date11.tm_min=0;date11.tm_sec=0;
        time_t t11 = mktime(&date11);
        balance[i][10]=bank._users[i].balance(t11)-balance[i][9];

        struct tm date12={0};
        date12.tm_year=120; date12.tm_mon=0; date12.tm_mday=1;
        date12.tm_hour=0;date12.tm_min=0;date12.tm_sec=0;
        time_t t12 = mktime(&date12);
        balance[i][11]=bank._users[i].balance(t12)-balance[i][10];
    }
    bool flag=false;
    int elig=bank._user_count;
    for(int i=0;i<bank._user_count;i++){
        for(int j=0;j<11;j++){
            if(balance[i][j]<0&&balance[i][j+1]<0){
                flag=true;
                break;
            }
        }
        if(flag){
            elig--;
        }
        flag=false;
    }
    
    os<<bank._bank_name<<"\t"<<elig<<"\t"<<sum<<endl;
    return os;
}