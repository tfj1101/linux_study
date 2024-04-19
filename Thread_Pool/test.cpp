#include <iostream>
#include "cpp_thread_pool.h"

using namespace std;

void func1(int a)
{
	cout << "func1() a = " << a << endl;
}

void func2(int a, string b)
{
	cout << "func2() a =" << a << ",b = " << b << endl;
}

void func3()
{
	cout << "func3()" << endl;
}

void test1()
{
	cout << "test() 01" << endl;
	CPP_ThreadPool threadpool;
	threadpool.init(2);
	threadpool.start();

	threadpool.exec(func1, 10);
	threadpool.exec(func2, 20, "lisi");
	threadpool.exec(100, func3);

	threadpool.waitForAllDone();
	threadpool.stop();
}

int func1_furture(int a)
{
	cout << "func1() a = " << a << endl;
	return a;
}

string func2_future(int a, string b)
{
	cout << "func2() a =" << a << ",b = " << b << endl;
	return b;
}
void test02()
{
	cout << "test 02" << endl;
	CPP_ThreadPool threadpool;
	threadpool.init(2);
	threadpool.start();

	future<decltype(func1_furture(0))> ret01 = threadpool.exec(func1_furture, 10);
	future<string> ret02 = threadpool.exec(func2_future, 20, "lisi");
	threadpool.waitForAllDone();
	cout << "ret01 = " << ret01.get() << endl;
	cout << "ret02 = " << ret02.get() << endl;

	threadpool.stop();
}

class Test
{
public:
	int test(int a)
	{
		cout << _name << ":a = " << a << endl;
		return a+1;
	}
	void setName(string name)
	{
		_name = name;
	}
	string _name;
};
void test03()
{
	cout << "test 03" << endl;
	CPP_ThreadPool threadpool;
	threadpool.init(2);
	threadpool.start();
	Test t1;
	Test t2;
	t1.setName("Test 1");
	t2.setName("Test 2");

	auto f1 = threadpool.exec(bind(&Test::test, &t1, placeholders::_1), 10);
	auto f2 = threadpool.exec(bind(&Test::test, &t2, placeholders::_1), 20);
	threadpool.waitForAllDone();
	cout << "f1 = " << f1.get() << endl;
	cout << "f2 = " << f2.get() << endl;

	threadpool.stop();
}
int main()
{
	//简单测试线程池
	test1();
	//测试任务函数返回值
	test02();
	//测试类对象函数的绑定
	test03();

	return 0;
}