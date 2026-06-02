# 2024 电赛井字棋项目

本项目是一个基于 ROS 上位机和 STM32 下位机的井字棋自动落子系统。上位机完成摄像头图像采集、棋盘定位、棋子识别、极大极小算法决策和串口数据下发；STM32 接收数据后解析目标格与坐标，完成运动平台控制、取子、移动和落子。

## 项目结构

```text
.
├── 识别/                         # ROS + OpenCV 棋盘/棋子识别
│   └── color_ws/
├── 博弈算法-极大极小算法/          # 井字棋极大极小算法 ROS 工作空间
│   └── tic_tac_toe_ws/
├── 通信/                         # ROS 与 STM32 串口通信
│   └── communication_ws01/
├── STM32/                        # STM32 下位机工程
├── 需要启动的文件                  # 原始启动命令记录
├── 项目介绍.md                    # 项目介绍稿
├── minimax_simulation.py          # 极大极小算法仿真脚本
└── README.md
```

## 项目介绍

系统整体信息流如下：

1. USB 摄像头发布 `/usb_cam/image_raw` 图像。

2. 视觉节点 `color_dectednew.py` 基于 OpenCV 进行棋盘定位和棋子识别。

3. 视觉节点发布 `/chessboard_centers` 和 `/chessboard_states`。

4. 博弈节点 `game.py` 订阅棋盘坐标与棋子状态，基于极大极小算法计算 AI 下一步落子。

5. 博弈节点发布目标点坐标和目标格编号。

6. 串口节点 `publish_node.cpp` 将 ROS 消息封装为串口数据帧发送给 STM32。

7. STM32 通过 USART1 接收数据，解析目标位置并输出 PWM/方向控制信号，驱动执行机构完成取子、移动、落子和复位。

### 视觉识别

视觉部分位于 `识别/color_ws/src/color_dected/scripts/color_dectednew.py`。程序先对图像进行灰度化、高斯滤波、Canny 边缘检测、膨胀和腐蚀，再通过轮廓提取与四边形拟合定位棋盘外框。得到棋盘四角点后，计算九宫格中心点，形成 9 个格子的中心坐标。

棋子识别使用 HSV 阈值和面积占比判断：

* 黑棋阈值：`[0, 0, 0]` 到 `[180, 255, 89]`

* 白棋阈值：`[0, 0, 176]` 到 `[180, 28, 255]`

* 黑色面积占比大于 `0.15` 判定为黑棋 `X`

* 白色面积占比大于 `0.1` 判定为白棋 `O`

* 否则判定为空位

视觉节点输出：

* `/chessboard_centers`：18 个 `int`，前 9 个为 x 坐标，后 9 个为 y 坐标。

* `/chessboard_states`：长度为 9 的字符串，`X` 表示黑棋，`O` 表示白棋，空格表示空位。

### 博弈算法

博弈部分位于：

* `博弈算法-极大极小算法/tic_tac_toe_ws/src/test_pkg_t5/scripts/game.py`

* `博弈算法-极大极小算法/tic_tac_toe_ws/src/test_pkg_t6/scripts/game.py`

算法基于极大极小搜索。AI 会递归遍历后续所有可能落子，并按胜负进行评分：AI 胜为正分，对手胜为负分，平局为 0 分；AI 回合取最大分支，对手回合取最小分支。计算结果会映射到机械执行使用的格号，并发布目标格坐标和目标格编号。

仿真数据：

| 口径     | 场景    |  对局数 | AI 胜 |  平局 | AI 负 |  AI 胜率 |  AI 不败率 |
| ------ | ----- | ---: | ---: | --: | ---: | -----: | ------: |
| 全路径穷举  | AI 先手 |   92 |   85 |   7 |    0 | 92.39% | 100.00% |
| 全路径穷举  | AI 后手 |  565 |  381 | 164 |   20 | 67.43% |  96.46% |
| 随机对手仿真 | AI 先手 | 5000 | 4924 |  76 |    0 | 98.48% | 100.00% |
| 随机对手仿真 | AI 后手 | 5000 | 4045 | 854 |  101 | 80.90% |  97.98% |

全路径穷举用于验证所有可能对局下的最坏情况；5000 局随机对手仿真用于统计随机对手下的平均表现。

### 串口通信

通信部分位于：

* `通信/communication_ws01/src/serial_test/src/serial_test.cpp`

* `通信/communication_ws01/src/topic_example/src/publish_node.cpp`

* `通信/communication_ws01/src/topic_example/src/mbot_linux_serial.cpp`

串口参数：

* 设备：`/dev/ttyUSB0`

* 波特率：`115200`

* 数据位：8

* 校验：无

* 停止位：1

通信分为两类数据帧：

* 普通棋盘坐标：帧头 `0x7B`，帧尾 `0x7D`，中间为 18 个整型数据的低/高字节。

* 目标格数据：帧头 `0xFF`，帧尾 `0xFE`，中间为 18 个整型数据的低/高字节。

### STM32 执行控制

STM32 下位机工程位于 `STM32/1227 (2)`。核心文件包括：

* `USER/main.c`

* `SYSTEM/usart/usart.c`

* `HARDWARE/CONTROL/control.c`

STM32 使用 USART1 接收上位机数据，通过中断识别 `0x7B...0x7D` 和 `0xFF...0xFE` 两类数据帧。`parseData` 会将低/高字节恢复为 18 个整型数据。随后程序将视觉像素坐标转换为运动延时量：

```c
x_3 = (points_3[i].x - 150) * 70;
y_3 = (points_3[i].y + 74) * 14;
y_3 = 8355 - (points_3[i].y + 74) * 14;
```

下位机通过 `Set_Pwm` 设置电机方向引脚和 PWM 输出，并结合 `MMGG1` 到 `MMGG4`、`xitie` 等 GPIO 状态控制吸附/落子相关机构。如果硬件连接的是 42 步进电机，可表述为 STM32 根据目标格位置输出 PWM/方向控制信号驱动 42 步进电机，并配合电磁吸附机构完成棋子搬运和落子。

## 环境配置

推荐环境：

* Ubuntu 20.04

* ROS Noetic

* Python 3

* OpenCV / `cv_bridge`

* Catkin

* USB 摄像头

* USB 串口设备 `/dev/ttyUSB0`

安装 ROS 依赖示例：

```bash
sudo apt update
sudo apt install -y ros-noetic-desktop-full
sudo apt install -y python3-rosdep python3-catkin-tools python3-opencv
sudo apt install -y ros-noetic-cv-bridge ros-noetic-image-transport
sudo apt install -y ros-noetic-usb-cam ros-noetic-serial
```

初始化 ROS 环境：

```bash
source /opt/ros/noetic/setup.bash
```

如果串口权限不足：

```bash
sudo usermod -aG dialout $USER
```

执行后重新登录系统，或临时修改串口权限：

```bash
sudo chmod 666 /dev/ttyUSB0
```

分别编译三个 ROS 工作空间：

```bash
cd 识别/color_ws
catkin_make
source devel/setup.bash
```

```bash
cd 博弈算法-极大极小算法/tic_tac_toe_ws
catkin_make
source devel/setup.bash
```

```bash
cd 通信/communication_ws01
catkin_make
source devel/setup.bash
```

## 需要启动的文件

以下命令按模块分终端启动。每个 ROS 工作空间在运行前都需要先 `source devel/setup.bash`。

### 1. 启动 ROS master

```bash
roscore
```

### 2. 启动串口接收节点

```bash
cd 通信/communication_ws01
catkin_make
source devel/setup.bash
rosrun serial_test serial_test
```

该节点从 STM32 接收模式触发数据，并发布 `mode_data`、`DATA` 等 ROS 话题。

### 3. 启动串口发送节点

```bash
cd 通信/communication_ws01
catkin_make
source devel/setup.bash
rosrun topic_example publish_node
```

该节点订阅视觉和算法话题，并将数据打包发送给 STM32。

### 4. 启动摄像头

仓库中包含以下 launch 文件：

* `识别/color_ws/src/color_dected/launch/usb_cam.launch`

* `识别/color_ws/src/color_dected/launch/usb_cam_with_calibration.launch`

* `识别/color_ws/src/color_dected/launch/usb_cam_with_calibration1.launch`

可按实际摄像头配置选择其一，例如：

```bash
cd 识别/color_ws
catkin_make
source devel/setup.bash
roslaunch color_dected usb_cam.launch
```

如果本机安装的 `usb_cam` 包提供 `usb_cam-test.launch`，也可以使用：

```bash
roslaunch usb_cam usb_cam-test.launch
```

### 5. 启动视觉识别节点

```bash
cd 识别/color_ws
catkin_make
source devel/setup.bash
rosrun color_dected color_dectednew.py
```

查看视觉输出：

```bash
rqt_image_view
rostopic echo /chessboard_centers
rostopic echo /chessboard_states
```

### 6. 启动博弈算法节点

模式 4：

```bash
cd 博弈算法-极大极小算法/tic_tac_toe_ws
catkin_make
source devel/setup.bash
rosrun test_pkg game.py
```

查看输出：

```bash
rostopic echo /mubiaopoint
rostopic echo /target_point
```

模式 5：

```bash
cd 博弈算法-极大极小算法/tic_tac_toe_ws
catkin_make
source devel/setup.bash
rosrun test_pkg_t5 game.py
```

查看输出：

```bash
rostopic echo /mubiaopoint_t5
rostopic echo /target_point_t5
```

模式 6：

```bash
cd 博弈算法-极大极小算法/tic_tac_toe_ws
catkin_make
source devel/setup.bash
rosrun test_pkg_t6 game.py
```

查看输出：

```bash
rostopic echo /mubiaopoint_t6
rostopic echo /target_point_t6
```

## 极大极小算法仿真运行命令

仓库根目录提供了独立仿真脚本 `minimax_simulation.py`，不依赖 ROS，可直接用 Python 运行：

```bash
python3 minimax_simulation.py
```

默认会分别输出：

* AI 先手 5000 局随机对手仿真

* AI 后手 5000 局随机对手仿真

* AI 先手全路径穷举

* AI 后手全路径穷举

指定随机仿真局数：

```bash
python3 minimax_simulation.py --games 5000
```

固定随机种子：

```bash
python3 minimax_simulation.py --games 5000 --seed 20260602
```

参考输出：

```text
Random opponent simulation
AI first : games=5000, AI_win=4924 (98.48%), draw=76 (1.52%), AI_loss=0 (0.00%), AI_unbeaten=100.00%
AI second: games=5000, AI_win=4045 (80.90%), draw=854 (17.08%), AI_loss=101 (2.02%), AI_unbeaten=97.98%

Exhaustive human move branches
AI first : games=92, AI_win=85 (92.39%), draw=7 (7.61%), AI_loss=0 (0.00%), AI_unbeaten=100.00%
AI second: games=565, AI_win=381 (67.43%), draw=164 (29.03%), AI_loss=20 (3.54%), AI_unbeaten=96.46%
```

## 注意事项

* 项目中的部分原始说明文件存在编码乱码，以README文件、源码行为和 ROS topic 信息流为准。

* 启动 ROS 节点时，建议每个工作空间单独打开终端并执行对应的 `source devel/setup.bash`。

* 视觉识别效果受相机位置、光照、棋盘边框和棋子颜色影响，正式测试前需要重新标定 HSV 阈值和摄像头参数。

* 串口节点默认使用 `/dev/ttyUSB0`，如果设备号变化，需要同步修改通信代码或创建 udev 规则。

