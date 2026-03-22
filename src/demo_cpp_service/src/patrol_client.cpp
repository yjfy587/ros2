#include<cstdlib>
#include<ctime>
#include"rclcpp/rclcpp.hpp"
#include"chapt4_interfaces/srv/patrol.hpp"
#include<chrono>
#include"rcl_interfaces/msg/parameter.hpp"
#include"rcl_interfaces/msg/parameter_value.hpp"
#include"rcl_interfaces/msg/parameter_type.hpp"
#include"rcl_interfaces/srv/set_parameters.hpp"
using SetP=rcl_interfaces::srv::SetParameters;
using namespace std::chrono_literals;
using Patrol=chapt4_interfaces::srv::Patrol;
class patrolClient:public rclcpp::Node{
    public:
        patrolClient():Node("patrol_client"){
            patrol_client=this->create_client<Patrol>("patrol");
            timer_=this->create_wall_timer(10s,std::bind(&patrolClient::time_callback,this));
            srand(time(NULL));
        }

        void update_server_param_k(double k){
            auto param=rcl_interfaces::msg::Parameter();
            param.name="k";
            //
            auto param_value=rcl_interfaces::msg::ParameterValue();
            param_value.type=rcl_interfaces::msg::ParameterType::PARAMETER_DOUBLE;
            param_value.double_value=k;
            param.value=param_value;
            //
            auto response= call_set_parameters(param);
            if(response==nullptr){
                RCLCPP_INFO(this->get_logger(),"参数修改失败");
                return;
            }else{
                for(auto result:response->results){
                    if(result.successful){
                        RCLCPP_INFO(this->get_logger(),"参数k已经修改为%f",k);
                    }
                    else{
                        RCLCPP_WARN(this->get_logger(),"参数k失败原因：%s",result.reason.c_str());
                    }
                }
            }
        }
        void time_callback(){
            //1.等待服务端上线
            while(!patrol_client->wait_for_service(std::chrono::seconds(1))){
                if(!rclcpp::ok()){
                    RCLCPP_ERROR(this->get_logger(),"等待服务的过程中被打断……");
                    return;
                }
                RCLCPP_INFO(this->get_logger(),"等待服务端上线");
            }
            //2.构造请求的对象
            auto request=std::make_shared<Patrol::Request>();
            request->target_x=rand()%15;
            request->target_y=rand()%15;
            RCLCPP_INFO(this->get_logger(),"请求巡逻：（%f,%f)",request->target_x,request->target_y);
            //3.发送异步请求
            patrol_client->async_send_request(request,std::bind(&patrolClient::request1,this,std::placeholders::_1));
        }
    
    private:
        rclcpp::TimerBase::SharedPtr timer_;
        rclcpp::Client<Patrol>::SharedPtr patrol_client;
        
    private:
        void request1(rclcpp::Client<Patrol>::SharedFuture result_future){
            auto response=result_future.get();
            if(response->result==Patrol::Response::SUCCESS){
                RCLCPP_INFO(this->get_logger(),"目标点处理成功");
            }
            else if(response->result==Patrol::Response::FAIL){
                RCLCPP_INFO(this->get_logger(),"目标点处理失败");
            }
        }
        std::shared_ptr<SetP::Response> call_set_parameters(rcl_interfaces::msg::Parameter &parameter){
            auto param_client=this->create_client<SetP>("/turtle_controller/set_parameters");
            while(!param_client->wait_for_service(std::chrono::seconds(1)))
            {
                if(!rclcpp::ok()){
                    RCLCPP_ERROR(this->get_logger(),"等待的过程中被打断");
                    return nullptr;
                }
                    RCLCPP_INFO(this->get_logger(),"等待参数设置上线");
            }
            auto request=std::make_shared<SetP::Request>();
            request->parameters.push_back(parameter);
            auto future=param_client->async_send_request(request);
            rclcpp::spin_until_future_complete(this->get_node_base_interface(),future);
            auto response=future.get();
            return response;
        }
};
int main(int argc,char **argv){
    rclcpp::init(argc,argv);
    auto node=std::make_shared<patrolClient>();
    node->update_server_param_k(1.5);
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
