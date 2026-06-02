"""
Tic Tac Toe class + game play implementation by Kylie Ying
YouTube Kylie Ying: https://www.youtube.com/ycubed
Twitch KylieYing: https://www.twitch.tv/kylieying
Twitter @kylieyying: https://twitter.com/kylieyying
Instagram @kylieyying: https://www.instagram.com/kylieyying/
Website: https://www.kylieying.com
Github: https://www.github.com/kying18
Programmer Beast Mode Spotify playlist: https://open.spotify.com/playlist/4Akns5EUb3gzmlXIdsJkPs?si=qGc4ubKRRYmPHAJAIrCxVQ
"""

import math
import time
from player import HumanPlayer, RandomComputerPlayer, SmartComputerPlayer


class TicTacToe():
    def __init__(self):
        self.board = self.make_board()
        self.current_winner = None

    @staticmethod
    def make_board():
        return [' ' for _ in range(9)]

    def print_board(self):
        for row in [self.board[i*3:(i+1) * 3] for i in range(3)]:
            print('| ' + ' | '.join(row) + ' |')

    @staticmethod
    def print_board_nums():

        number_board = [[str(i) for i in range(j*3, (j+1)*3)] for j in range(3)]
        for row in number_board:
            print('| ' + ' | '.join(row) + ' |')

    def make_move(self, square, letter):
        if self.board[square] == ' ':
            self.board[square] = letter
            if self.winner(square, letter):
                self.current_winner = letter
            return True
        return False

    def winner(self, square, letter):

        row_ind = math.floor(square / 3)
        row = self.board[row_ind*3:(row_ind+1)*3]

        if all([s == letter for s in row]):
            return True
        col_ind = square % 3
        column = [self.board[col_ind+i*3] for i in range(3)]

        if all([s == letter for s in column]):
            return True
        if square % 2 == 0:
            diagonal1 = [self.board[i] for i in [0, 4, 8]]

            if all([s == letter for s in diagonal1]):
                return True
            diagonal2 = [self.board[i] for i in [2, 4, 6]]

            if all([s == letter for s in diagonal2]):
                return True
        return False

    def empty_squares(self):
        return ' ' in self.board

    def num_empty_squares(self):
        return self.board.count(' ')

    def available_moves(self):
        return [i for i, x in enumerate(self.board) if x == " "]


def play(game, x_player, o_player, print_game=True):

    letter = 'O'
    while game.empty_squares():
        if letter == 'O':
            square = o_player.get_move(game)
        else:
            square = x_player.get_move(game)
        if game.make_move(square, letter):

            if game.current_winner:
                return letter
            letter = 'O' if letter == 'X' else 'X'



    if print_game:
        print('It\'s a tie!')
    return 'Tie'

def main(num_games):
    x_wins = 0
    o_wins = 0
    ties = 0

    for _ in range(num_games):
        game = TicTacToe()
        x_player = SmartComputerPlayer('X')
        o_player = RandomComputerPlayer('O')
        result = play(game, x_player, o_player, print_game=False)

        if result == 'X':
            x_wins += 1
        elif result == 'O':
            o_wins += 1
        else:
            ties += 1

    print(f"After {num_games} games:")
    print(f"Player X wins: {x_wins/num_games*100} %")
    print(f"Player O wins: {o_wins/num_games*100} %")
    print(f"Ties: {ties/num_games*100} %")

if __name__ == '__main__':
    num_games = 50000
    main(num_games)
