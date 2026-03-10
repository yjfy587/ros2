#include<iostream>
#include"rclcpp/rclcpp.hpp"
using namespace std;
class PersonNode:public rclcpp::Node
{
private:
    string name
    int age;
public:
    PersonNode(const string &name,int age):Node("person_node")
    {
        this->name_=name;
        this->age_=age;
        RCLCPP_INFO(this->get_logger(),"我是%s，今年%d岁了",name_.c_str(),age_)
    }
    void eat(const string &food_name)
    {
        RCLCPP_INFO(this->get_logger(),"%s正在吃%s",name_.c_str(),food_name.c_str());
    }
}
int main(int argc,char **argv)
{
    rclcpp::init(argc, argv);
    auto node=std::make_shared<PersonNode>("小明",18);
    node->eat("面包");
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}