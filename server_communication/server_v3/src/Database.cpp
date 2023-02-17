#include "Database.h"
#include "util.h"

Database::Database(){
    MYSQL *p1 = mysql_init(&mysql_conn);
    errif(p1 == nullptr, "database create error");

    mysql_real_connect(p1, "127.0.0.1", "root",
                       "123456", "chat_server", 3306, NULL, 0);
    errif(p1 == nullptr, "database connection error");
}
/*
id      user        passwd
1     duanshuai      hello
*/
int Database::regist_new(string name, string passwd){
    string str_sql = "insert into log_table(user, passwd) values('"
                  + name + "','" + passwd + "')";
    const char* sql = str_sql.data();
    mysql_query(&mysql_conn, sql);
    //返回id
    string sql1 = "select max(id) from log_table";
    //int res = 
    return 0;
}

string Database::log_validation(int id, string passwd){
    string str_sql = "select * from log_table where id = " + std::to_string(id);
    const char* sql = str_sql.data();

    if(mysql_query(&mysql_conn, sql)!=0)
    {
        cout<<"寄*1"<<endl;
    }
    MYSQL_RES *res=mysql_store_result(&mysql_conn);//获取结果,结果集
    if(res == NULL){
        cout<<"寄*2"<<endl;
    }
    int num = mysql_num_rows(res);//一共有多少行，通过结果集获取
    printf("查询到%d条记录\n",num);
    if(num == 1){
        MYSQL_ROW r=mysql_fetch_row(res);
        if(r[2] == passwd){
            string name = r[1];
            mysql_free_result(res);
            return name;
        }
    }
    return "";
}