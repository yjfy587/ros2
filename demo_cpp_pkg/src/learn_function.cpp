#include<iostream>
#include<functional>
using namespace std;
void save_with_free_fun(const string &filename)
{
    cout<<"调用了自由函数"<<filename<<endl;
}
class FileSave
{
public:
    void save_with_member_fun(const string &file_name)
    {
        cout<<"调用了成员函数"<<file_name<<endl;
    }
};
int main()
{
    FileSave file_save;
        auto save_with_lambda_fun=[](const string &file_name)->void
    {
        cout<<"调用了lambda表达式"<<file_name<<endl;
    };
    function<void(const string &)> save1=save_with_free_fun;
    function<void(const string &)> save2=save_with_lambda_fun;
    function<void(const string &)> save3=bind(&FileSave::save_with_member_fun,&file_save,placeholders::_1);
    save1("file1.txt");
    save2("file2.txt");
    save3("file3.txt");
    return 0;
}