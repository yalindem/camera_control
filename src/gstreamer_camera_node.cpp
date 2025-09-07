#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/opencv.hpp>
#include <image_transport/image_transport.hpp>

class GStreamerCameraNode : public rclcpp::Node
{
public:
    GStreamerCameraNode(std::string node_name)
        : Node(node_name)
    {
        pub_ = this->create_publisher<sensor_msgs::msg::Image>("/camera/image_raw", 10);

        // GStreamer pipeline
        cap_.open("v4l2src ! video/x-raw,format=YUY2,width=640,height=480 ! videoconvert ! appsink", cv::CAP_GSTREAMER);
        if (!cap_.isOpened()) {
            RCLCPP_ERROR(this->get_logger(), "Kamera cannot open!");
            rclcpp::shutdown();
            return;
        }
        timer_ = this->create_wall_timer(
            std::chrono::milliseconds(66),
            std::bind(&GStreamerCameraNode::publish_frame, this));
    }
test
private:
    void publish_frame()
    {
        cv::Mat frame;
        if (!cap_.read(frame)) {
            RCLCPP_WARN(this->get_logger(), "Frame cannot be taken!");
            return;
        }
        //cap_.set(cv::CAP_PROP_AUTO_EXPOSURE, 0.25);
        //cap_.set(cv::CAP_PROP_EXPOSURE, 1000); 

        auto msg = cv_bridge::CvImage(std_msgs::msg::Header(), "bgr8", frame).toImageMsg();
        pub_->publish(*msg);
    }

    rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr pub_;
    cv::VideoCapture cap_;
    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<GStreamerCameraNode>("gstreamer_camera_node");
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}


test1