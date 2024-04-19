#include <iostream>
#include <cstdio>
#include <string>
#include <mysql/mysql.h>
#include<pthread.h>
using namespace std;
const char *host = "192.168.0.106";
const char *name = "root";
const char *password = "666666tfj";
const char *db = "user";
const unsigned int port = 3306;

int main()
{
    MYSQL *mysql = mysql_init(nullptr);

    if (mysql_real_connect(mysql, host, name, password, db, port, nullptr, 0) == nullptr)
    {
        cout << "connect failed" << endl;
        return -1;
    }

    cout << mysql_get_host_info(mysql) << endl;
    mysql_set_character_set(mysql, "utf8");
    string sql = "select * from nb";
    // 执行sql语句
    int code = mysql_query(mysql, sql.c_str());
    // 获取结果集
    MYSQL_RES *res = mysql_store_result(mysql);
    // 获取字段名（属性名）
    MYSQL_FIELD *field = mysql_fetch_field(res);
    int col = mysql_num_fields(res);
    int row = mysql_num_rows(res);
    for (int i = 0; i < col; i++)
    {
        cout << field[i].name << "\t";
    }
    cout << endl;
    for (int i = 0; i < row; i++)
    {
        // 获取查询结果中的一行数据   mysql_fetch_row()
        MYSQL_ROW line = mysql_fetch_row(res);
        for (int j = 0; j < col; j++)
        {
            cout << line[j] << "\t";
        }
        cout << endl;
    }
    mysql_free_result(res);
    mysql_close(mysql);
    return 0;
}
