1- source ROS2 installation
(1.5 - this builds the package (from root-folder) on my computer: $ colcon build --merge-install --event-handlers console_direct+ --cmake-args -DPython3_ROOT_DIR=C:/Python38/python.exe -DPython3_FIND_STRATEGY=LOCATION -DPython3_FIND_REGISTRY=NEVER -DPython3_EXECUTABLE=C:/Python38/python.exe -DPYTHON_EXECUTABLE=C:/Python38/python.exe)
	- remember to source visual studio ($ call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x86_amd64)
	-this should supposedly enforce that python 3.8 is used.
2- go to ws root and source the package ($ call install\setup.bat)
3- (in another terminal source ROS2 and $ ros2 run turtlesim turtlesim_node)
4- run the node: ros2 run cpp_inputter input_publisher
5- to run with preset argument for period: $ ros2 run cpp_inputter input_publisher --ros-args -p period:=<param_value>

