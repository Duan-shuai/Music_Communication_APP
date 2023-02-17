# include<mysql.h>
# include<string>
# include<iostream>

using namespace std;
/*
单例模式
*/
class Database{
private:
    MYSQL mysql_conn;
    Database();
public:
    int regist_new(string name, string passwd);
    string log_validation(int id, string passwd);
    
    static Database* GetInstance(){
        static Database db;
        return &db;
    }
};