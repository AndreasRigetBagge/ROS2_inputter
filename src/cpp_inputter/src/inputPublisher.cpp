//The imports from other class for now
//could maybe just copy as default -> should be no name clashes beyond original? 
//(and should link up if in original folder?)

/** TODO:
 * -Consider making this a hpp-file instead (with some defs in cpp-file). -> depending on size and use?!
 * -Maybe add some delegating constructors (one that postfixes name with nothing vs. postfixes with more things?)
 *    -For instance, probably one for intervals, exe-time buffers (adding 500 ms timeout etc.) etc.
 * -Maybe add more member-functions?
 * -constexpr variables might (also) become member-variables
 * -Find out if constexpr could be problem?
 * -make class more template based / or member types in order to have generalize it, message-wise etc.
 * -Some dependencies commented out for now.
 *    -> to make parameterizable, it is a good question, whether that should happen outside or inside class
 * -Consider using static member-variable for id'ing topic-names etc.
 * -Consider also listening to pose-channel for outputs or do it in a separate class/node?
 * -The variables in the C++-file should probably be made into ROS2-paramters!!!!!
 * -Create parameters, and generate the yaml-file from python-script that will be used
 *    to launch this node.
 * -Consider using timer-class from crono (see chap. 18 in learncpp), and make sure not in debug-config
 * -maybe add benchmarkable extention to messages (such as ID and timestamp),
 *    or use topic-statistics in rclcpp (only per topic basis?)
 * -Can we assume sequential arrival-order of callbacks (1st sent will be 1st in end)?
 * -addressing shortcomings in modeling some delays may be contribution in itself?
 * -YAETS maybe promising tracing-system (https://github.com/fmrico/yaets), as asynchronously
 * -Otherwise maybe ROS2's own topic statistics might be of use. 
 *      -> lots of applications only concerned with average-latency rather than max-latency (important in real-time, though)
 * -For more fine-grained, it might be possible to scale times (requires that we know relative duration?)
 */

//#include "turtlesim/turtle.hpp"

//#include <QColor>
//#include <QRgb>

#include <cmath>
#include <functional>
#include <string>
#include <chrono>
#include <vector>

#include "geometry_msgs/msg/twist.hpp"
#include "rclcpp/rclcpp.hpp"

//#include "turtlesim_msgs/action/rotate_absolute.hpp"
//#include "turtlesim_msgs/msg/pose.hpp"
#include <turtlesim/msg/pose.hpp>
//#include "turtlesim_msgs/msg/color.hpp"
//#include "turtlesim_msgs/srv/set_pen.hpp"
//#include "turtlesim_msgs/srv/teleport_absolute.hpp"
//#include "turtlesim_msgs/srv/teleport_relative.hpp"
//#include "turtlesim/qos.hpp"


//from publisher-class in ROS-tutorial

#include <memory>



#include "std_msgs/msg/string.hpp"

//TODO: make some const parameters that will be the correct topic-names, qos, etc. and publish simple
//message into those (maybe just read from type? -> could be required in header and create our "using" of it?)

/*A qos object:
For now, hardcoded the QoS-object seemingly used in the included header*/
/*If this should be used in model, maybe relevant. Otherwise maybe higher.
Also relevant to check for limit/bound as to how much computer can publish
not constexpr, as no constexpr constructor seems to exist?*/
const rclcpp::QoS QoS_param = rclcpp::QoS(rclcpp::KeepLast(7)).reliable();

/*Topic name: 
(for now, hardcoded)
Can't be constexpr in C++ 14*/
const std::string topicName = "turtle1/cmd_vel";
/*Type of publishing (e.g. periodic or sporadically?)*/

/*execution-time? (if not just from the original system
    -> otherwise we could maybe make function/use existing to find seeming bound?)
    -> execution-time might depend on other ones? -> constrain it not to?
        -> or otherwise execution-time should only denote time excluding blocking time (and then model communications?)*/

/*Default-message: */

/*times: here an enum for deciding on the kind of timestamps*/
/*Could later make if-statement that creates publisher accordingly*/
enum class Timing_Behavior{
    fixed,
    non_fixed,
};

constexpr Timing_Behavior timing = Timing_Behavior::fixed;


using namespace std::chrono_literals;


/*Try to maybe find way to convert integral-param to the ms-one?

the following illustrates one possible way to convert from int to the ms-interval:
  int timeInt{ 100 };
  unsigned long long timeFromInt{ static_cast<unsigned long long>(timeInt) };
  std::chrono::milliseconds period2(timeFromInt);
*/
/*fixed time interval (if periodic)*/
constexpr auto interval = 500ms;


/*timestamps (if sporadic)*/
//TODO: Add the using-clause above this


/*The publisher: 
(for now more or less using the one from ROS2-tutorial with some injections?)*/



/* This example creates a subclass of Node and uses std::bind() to register a
* member function as a callback from the timer. */

class MinimalPublisher : public rclcpp::Node
{
  //declared first (for now) as it has auto-return-type
  private: 
    auto create_param_timer()
    {
      auto period_int = get_parameter("period").get_value<int>();
      unsigned long long timeFromInt{ static_cast<unsigned long long>(period_int) };
      std::chrono::milliseconds period_param(timeFromInt);
      return this->create_wall_timer(
      period_param, std::bind(&MinimalPublisher::timer_callback, this));
    }
  public:
    MinimalPublisher() //constructor
    : Node("minimal_publisher"), count_(0) //class member-initializations
    { //Insert the topic-name here
      //create parameters
      this->declare_parameter("period", 1000);
      this->declare_parameter("topic_name", "turtle1/cmd_vel");
      this->declare_parameter("buffer-size", 7);
      //TODO: message-type inserted here (used geometry_msgs here) (could be less hardcoded?)
      //QoS_param also just inserted
      publisher_ = this->create_publisher<geometry_msgs::msg::Twist>(topicName, QoS_param); //further inits (because of pointers?)?


      //timer_ = this->create_wall_timer(
      //interval, std::bind(&MinimalPublisher::timer_callback, this));
      timer_ = create_param_timer();

    }

  private:
    //class function-member (the callback function)
    void timer_callback()
    { //TODO: This should probably be parameterized:
      
      //default left-spin-message
      geometry_msgs::msg::Twist twist;
      twist.angular.z = 2.0; 
      twist.linear.x = 0.0;
      
      //print message and publish message
      RCLCPP_INFO(this->get_logger(), "Publishing: a message no `%s`!:-)", std::to_string(++count_));
      publisher_->publish(twist);
    }
    
    //Class data-members 
    rclcpp::TimerBase::SharedPtr timer_; 
    //we must change type here depending on msg-type? (unless we convert inside callback??)
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_;
    size_t count_;
};


////TODO: If in class, we can maybe also dynamically allocate an array (and delete it in destructor -> see chap. 19)
////template function for taking an array indicating release-times
////(We might end up preferring range-based loop instead etc.?)
//template <typename T>
//void sporadicCallback(const std::vector<T>& arr)
//{
//    std::size_t length { arr.size() };
//    //TODO: implement
//     for (std::size_t i{ 0 }; index < length; ++i){
//        //arr.data()[i];
//     }
//}

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<MinimalPublisher>());
  rclcpp::shutdown();
  return 0;
}
