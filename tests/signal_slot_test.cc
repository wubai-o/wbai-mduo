#include"../study/signal_slot.h"
#include<functional>

using namespace std;

void hello()
{
	cout << "hello" << endl;
}
void print(int i)
{
	cout << "print " << i << endl;
}
void test()
{
	{
		study::Signal<void(void)> sig;
		study::Slot slot1 = sig.connect(&hello);
		sig.call();
	}
	study::Signal<void(int)> sig1;
	study::Slot slot1 = sig1.connect(&print);
	study::Slot slot2 = sig1.connect(std::bind(&print, std::placeholders::_1));
	std::function<void(int)> func1(std::bind(&print, std::placeholders::_1));
	study::Slot slot3 = sig1.connect(std::move(func1));
	{
		study::Slot slot4 = sig1.connect(std::bind(&print, 666));
		sig1.call(4);
	}
	sig1.call(4);
}
 
int main()
{
	test();
	char c; cin >> c;
}
