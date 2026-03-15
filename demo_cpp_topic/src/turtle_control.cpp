#include "geometry_msgs/msg/twist.hpp"
#include "rclcpp/rclcpp.hpp"
#include "turtlesim/msg/pose.hpp"

class TurtleController :public rclcpp::Node{
public:
    TurtleController():Node("turtle_controller"){
        velocity_publisher_=this->create_publisher<geometry_msgs::msg::Twist>("/turtle1/cmd_vel",10);
        pose_subscription_=this->create_subscription<turtlesim::msg::Pose>("/turtle1/pose",10,std::bind(&TurtleController::on_pose_received,this,std::placeholders::_1));
    }
private:
    void on_pose_received(const turtlesim::msg::Pose::SharedPtr pose){
        auto message=geometry_msgs::msg::Twist();
        double current_x=pose->x;
        double current_y=pose->y;
        RCLCPP_INFO(this->get_logger(),"当前位置:(x=%f,y=%f)",current_x,current_y);

        double distance=std::sqrt((target_x_-current_x)*(target_x_-current_x)+(target_y_-current_y)*(target_y_-current_y));
        double angle=std::atan2(target_y_-current_y,target_x_-current_x)-pose->theta;
        if(distance>0.01){
            if(fabs(angle)>0.1)
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
        velocity_publisher_->publish(message);
    }
private:
    rclcpp::Subscription<turtlesim::msg::Pose>::SharedPtr pose_subscription_;
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr velocity_publisher_;
    double target_x_{1.0};
    double target_y_{1.0};
    double k_{1.0};
    double max_speed_{3.0};
};
int main(int argc,char **argv){
    rclcpp::init(argc,argv);
    auto node= std::make_shared<TurtleController>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}