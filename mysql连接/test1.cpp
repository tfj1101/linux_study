#include <mysql/mysql.h>
#include <iostream>
#include <cstdio>
#include <string>
using namespace std;

const char *const host = "192.168.200.1";
const char *const user = "root";
const char *const password = "666666tfj";
const char *const db_name = "user";
const unsigned int port = 3306;

int main()
{
    MYSQL *mysql = mysql_init(nullptr);

    cout << "mysql version:" << mysql_get_client_info() << endl;

    if (mysql_real_connect(mysql, host, user, password, db_name, port, nullptr, 0) == nullptr)
    {
        cerr << "connect failed!" << endl;
        return -1;
    }
    else
    {
        mysql_set_character_set(mysql, "utf8");

        string sql = "select * from gameuser";
        int code = mysql_query(mysql, sql.c_str());
        cout << "execute:" << sql << "success" << endl;

        MYSQL_RES *res = mysql_store_result(mysql);
        int row = mysql_num_rows(res);
        int col = mysql_num_fields(res);

        MYSQL_FIELD *field = mysql_fetch_field(res);
        for (int i = 0; i < col; i++)
        {
            cout << field[i].name << "\t";
        }
        cout << endl;
        for (int i = 0; i < row; i++)
        {
            MYSQL_ROW line = mysql_fetch_row(res);
            for (int j = 0; j < col; j++)
            {
                cout << line[j] << "\t";
            }
            cout << endl;
        }
    }
    mysql_close(mysql);
    return 0;
}