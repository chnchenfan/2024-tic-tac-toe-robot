'''
    1、配置配好
    如果视觉收到串口传回来的开始检测数据。几种数据：模式切换的数据。
        模式一前三种问题：返回坐标；
        模式二：人先出：视觉部分每收到一次数据，检测一次，返回给核心算法一次全局数据【第一次是返回坐标和全局数据，后面是只有全局数据】，算法计算结果【说明核心算法是一直开启的，只需要订阅话题即可，说明只有回调函数是一直在监听的
            ，所以全局点的初始化可以放在init内，后面都是回调函数在变化】）
                算法得到全局变量后，比对与上次的区别，存在区别的唯一点即为本次所走的位置，如果全局存在“1”，则则说明还有未走完的位置，则将全局情况传给核心算法，使得其能够进行解算，得到AI需要下棋的位置
                随后，将得到位置值保存至self.point中，更新全局数据，并且将对应的AI所走的位置的坐标通过串口下发。
        模式三：AI先出
'''

import math
import threading
import rospy
from std_msgs.msg import Int32MultiArray,String,Float32MultiArray












class Player():
    def __init__(self, letter):
        self.letter = letter

    def get_move(self, game):
        pass

class SmartComputerPlayer(Player):
    def __init__(self, letter):
        super().__init__(letter)

    def get_move(self, game):
        rospy.loginfo("AI is thinking ")
        if len(game.available_moves()) == 9:
            square = 4
        elif game.point.count('O') == 1 and game.point[4] == ' ':
            square = 4
        else:
            square = self.minimax(game, self.letter, 0)['position']
        return square

    def minimax(self, state, player, depth):
        if depth == 0:
            rospy.loginfo("enter minimax")
        max_player = self.letter
        other_player = 'O' if player == 'X' else 'X'

        if state.current_winner == other_player:
            return {'position': None, 'score': 1 * (state.num_empty_squares() + 1) if other_player == max_player else -1 * (state.num_empty_squares() + 1)}
        elif not state.empty_squares():
            return {'position': None, 'score': 0}

        if player == max_player:
            best = {'position': None, 'score': -math.inf}
        else:
            best = {'position': None, 'score': math.inf}

        for possible_move in state.available_moves():
            state.make_move(possible_move, player)
            sim_score = self.minimax(state, other_player, depth + 1)
            state.point[possible_move] = ' '
            state.current_winner = None
            sim_score['position'] = possible_move

            if player == max_player:
                if sim_score['score'] > best['score']:
                    best = sim_score
            else:
                if sim_score['score'] < best['score']:
                    best = sim_score

        return best


class TicTacToe():
    def __init__(self):
        self.current_winner = None


    def init_data(self,point):

        self.point = point

    def make_move(self, square, letter):
        if self.point[square] == ' ':
            self.point[square] = letter
            self.winner_define = False
        if self.winner(square, letter):
            self.current_winner = letter
            self.winner_define = True


    def winner(self, square, letter):

        row_ind = math.floor(square / 3)
        row = self.point[row_ind*3:(row_ind+1)*3]

        if all([s == letter for s in row]):
            return True
        col_ind = square % 3
        column = [self.point[col_ind+i*3] for i in range(3)]

        if all([s == letter for s in column]):
            return True
        if square % 2 == 0:
            diagonal1 = [self.point[i] for i in [0, 4, 8]]

            if all([s == letter for s in diagonal1]):
                return True
            diagonal2 = [self.point[i] for i in [2, 4, 6]]

            if all([s == letter for s in diagonal2]):
                return True
        return False

    def empty_squares(self):
        return ' ' in self.point

    def num_empty_squares(self):
        return self.point.count(' ')

    def available_moves(self):
        return [i for i, x in enumerate(self.point) if x == ' ']

class TicTacToeMasterNode:
    def __init__(self):
        self.init_data()
        self.init_ros()
        self.t = TicTacToe()
        self.point = [' '] * 9
        self.quanjupoint = None
        self.globalpoint = None
        self.condition = threading.Condition()
        self.playstate = 0

    def init_data(self):
        rospy.loginfo("正在初始化......")
        self.data_dict = {}
        self.personpoint = 0
        self.counst = 0

    def init_ros(self):
        rospy.loginfo("订阅中心点数据......")
        self.sub1 = rospy.Subscriber("/chessboard_centers", Int32MultiArray, self.std_zuobiaocallback, queue_size=10)
        rospy.loginfo("订阅棋子状态数据......")
        self.sub2 = rospy.Subscriber("/chessboard_states", String, self.std_pointcallback, queue_size=10)
        rospy.loginfo("订阅模式......")
        self.sub3 = rospy.Subscriber("DATA", Int32MultiArray, self.std_DATA, queue_size=10)
        rospy.loginfo("发布目标坐标数据......")
        self.centers_pub = rospy.Publisher("/mubiaopoint_t6", Int32MultiArray, queue_size=1)
        rospy.loginfo("发布目标数据......")
        self.centers_pub1 = rospy.Publisher("/target_point_t6", Int32MultiArray, queue_size=1)

    def std_zuobiaocallback(self, msg):
        if len(msg.data) != 18:
            rospy.logwarn("接收到的数据长度不正确，预期为18，实际为%d", len(msg.data))
            return

        self.data_dict.clear()

        x_values = msg.data[:9]
        y_values = msg.data[9:]

        for i in range(9):
            self.data_dict[i + 1] = [x_values[i], y_values[i]]

    def std_pointcallback(self, msg):

        self.quanjupoint = list(map(str, msg.data))

        node.play_game()








    def std_DATA(self, msg):
        rospy.loginfo("mode 4")


    def play_game(self):
        rospy.loginfo("game")
        with self.condition:
            if self.quanjupoint is None:
                rospy.loginfo("等待棋子状态数据...")
                self.condition.wait()

        self.point = self.quanjupoint[:]

        x_player = SmartComputerPlayer('O')
        self.t.init_data(self.point)

        if self.t.empty_squares():
            rospy.loginfo("现在开始博弈")

            square = x_player.get_move(self.t)

            if self.t.winner(square, 'O'):
                rospy.loginfo(f"胜利者为: {self.t.current_winner}")
            elif self.point[square] == ' ':
                self.update_data_dict(square)


    def update_data_dict(self, square):
        square_map = {0: 3, 1: 6, 2: 9, 3: 2, 4: 5, 5: 8, 6: 1, 7: 4, 8: 7}
        square = square_map.get(square, square)

        if square in self.data_dict:
            coords = self.data_dict[square]
            rospy.loginfo(f"AI选择的位置: {square} 对应的坐标为: {coords}")
            msg = Int32MultiArray(data=coords)
            self.centers_pub.publish(msg)
            point_square = [square] + [square] * 17
            square_msg = Int32MultiArray(data=point_square)
            self.centers_pub1.publish(square_msg)
        else:
            rospy.logwarn(f"位置 {square + 1} 没有对应的坐标在 data_dict 中")

if __name__ == '__main__':
    rospy.init_node('tic_tac_toe_master', anonymous=True)
    node = TicTacToeMasterNode()
    rate = rospy.Rate(10)



    rate.sleep()
    rospy.spin()

