#include<map>
#include<functional>
#include<iostream>

using namespace std;

float add(float a, float b)
{
    return a + b;
}
struct substract
{
    float operator()(float a, float b)
    {
        return a - b;
    }
};

int main()
{
    map<char,function<double(double,double)>> calculator{
        {'+',add},
        {'-',substract()},
        {'*',[](double a,double b){return a*b;}}
    };

    cout<<calculator['+'](1,2)<<endl;
    cout<<calculator['-'](13.0,6.2)<<endl;
    cout<<calculator['*'](3.0,4.0)<<endl;
}