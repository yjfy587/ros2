#include"rclcpp/rclcpp.hpp"
#include"geometry_msgs/msg/twist.hpp"
#include"turtlesim/msg/pose.hpp"
#include"chapt4_interfaces/srv/patrol.hpp"
#include"rcl_interfaces/msg/set_parameters_result.hpp"
using SetParametersResult=rcl_interfaces::msg::SetParametersResult;
using Patrol=chapt4_interfaces::srv::Patrol;
class Turtlecontroller:public rclcpp::Node{
public:
    Turtlecontroller():Node("turtle_controller"){
        this->declare_parameter("k",1.0);
        this->declare_parameter("max_speed",1.0);
        this->get_parameter("max_speed",max_speed_);
        this->get_parameter("k",k_);
        velocity_publisher=this->create_publisher<geometry_msgs::msg::Twist>("/turtle1/cmd_vel",10);//订阅，串口Twist
        pose_subscription=this->create_subscription<turtlesim::msg::Pose>("/turtle1/pose",10,std::bind(&Turtlecontroller::on_pose_received_,this,std::placeholders::_1));
        patrol_server_=this->create_service<Patrol>("patrol",[&](const std::shared_ptr<Patrol::Request> request,std::shared_ptr<Patrol::Response> response) -> void{
            if((0<request->target_x&&request->target_x<12.0f)&&(0<request->target_x&&request->target_x<12.0f)){
                target_x=request->target_x;
                target_y=request->target_y;
                response->result=Patrol::Response::SUCCESS;
            }
            else{
                response->result=Patrol::Response::FAIL;
            }
        });
        parameters_callback_handle_=this->add_on_set_parameters_callback([&](const std::vector<rclcpp::Parameter> &params){
            for(auto param:params){
                RCLCPP_INFO(this->get_logger(),"更新参数%s值为：%f",param.get_name().c_str(),param.as_double());
                if(param.get_name()=="k"){
                    k_=param.as_double();
                }
                else if(param.get_name()=="max_speed"){
                    max_speed_=param.as_double();
                }
            }
            auto result=SetParametersResult();
            result.successful=true;
            return result;
        });
    }
private:
    void on_pose_received_(const turtlesim::msg::Pose::SharedPtr pose){
        auto message=geometry_msgs::msg::Twist();
        double current_x=pose->x;
        double current_y=pose->y;
        RCLCPP_INFO(this->get_logger(),"当前位置 x:%f,y:%f",current_x,current_y);
        double distance=std::sqrt((target_x-current_x)*(target_x-current_x)+(target_y-current_y)*(target_y-current_y));
        double angle=std::atan2(target_y-current_y,target_x-current_x)-pose->theta;
        if(distance>0.1){
            if(fabs(angle)>0.2)
            {
                message.angular.z=fabs(angle);
            }
            else{
                message.linear.x=k_*distance;
            }
        }
        if(message.linear.x>max_speed_){
            message.linear.x=max_speed_;
        }
        velocity_publisher->publish(message);
    }
private:
    rclcpp::Subscription<turtlesim::msg::Pose>::SharedPtr pose_subscription;
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr velocity_publisher;
    rclcpp::Service<Patrol>::SharedPtr patrol_server_;
    OnSetParametersCallbackHandle::SharedPtr parameters_callback_handle_;
    double target_x{1.0};
    double target_y{1.0};
    double k_{1.0};
    double max_speed_{5.0};
};
int main(int argc,char **argv){
    rclcpp::init(argc,argv);
    auto node=std::make_shared<Turtlecontroller>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}