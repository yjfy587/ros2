#include<string>
#include "rclcpp/rclcpp.hpp"

class PersonNode:public rclcpp::Node
{
private:
    std::string name_;
    int age_;
public:
    PersonNode(const std::string &node_name,
        const std::string &name,
        const int &age):Node(node_name))
    {
        this->name_ = name;
        this->age_ = age;
    };
    void eat(const std::string &food_name)
    {
        RCLCPP_INFO(this->get_logger(), "我是%s，今年%d岁，正在吃%s",
            this->name_.c_str(),this->age_, food_name.c_str());
    };
};
int main(int argc, char **argv)
{
    rclcpp::init(argc,argv);
    auto node= std::make_shared<PersonNode>("cpp_node","张三",20);
    node->eat("面包");
    rclcpp::spin(node);
    rclcpp::shutdown();
}