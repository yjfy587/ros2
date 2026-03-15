#include <QApplication>
#include <QLabel>
#include <QString>
#include "rclcpp/rclcpp.hpp"
#include "status_interfaces/msg/system_status.hpp"

using SystemStatus = status_interfaces::msg::SystemStatus;

class SysStatusDisplay : public rclcpp::Node {
public:
    SysStatusDisplay() : Node("sys_status_display") {
        subscription_ = this->create_subscription<SystemStatus>(
            "sys_status", 10, [&](const SystemStatus::SharedPtr msg) -> void {
                label_->setText(get_qstr_from_msg(msg));
            });
        label_ = new QLabel(get_qstr_from_msg(std::make_shared<SystemStatus>()));
        label_->show();
    }

    QString get_qstr_from_msg(const SystemStatus::SharedPtr msg) {
        std::stringstream show_str;
        show_str
            << "=================系统状态可视化显示工具=================\n"
            << "数据时间" << msg->stamp.sec << "\ts\n"
            << "用户名称" << msg->host_name << "\t\n"
            << "CPU使用率" << msg->cpu_percent << "\t%\n"
            << "内存使用率" << msg->memory_percent << "\t%\n"
            << "内存总量" << msg->memory_total << "\tGB\n"
            << "内存可用量" << msg->memory_avaliable << "\tGB\n"
            << "网络发送速率" << msg->net_sent << "\tMB/s\n"
            << "网络接收速率" << msg->net_recv << "\tMB/s\n";
        return QString::fromStdString(show_str.str());
    }

private:
    rclcpp::Subscription<SystemStatus>::SharedPtr subscription_;
    QLabel* label_;
};

int main(int argc, char** argv) {
    rclcpp::init(argc, argv);
    QApplication app(argc, argv);
    auto node = std::make_shared<SysStatusDisplay>();
    std::thread spin_thread([&]() -> void {
        rclcpp::spin(node);
    });
    spin_thread.detach();
    app.exec();
    rclcpp::shutdown();
    return 0;
}