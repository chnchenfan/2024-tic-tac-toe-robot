#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""Run minimax simulations for the tic-tac-toe AI used in this project."""

import argparse
import math
import random
from dataclasses import dataclass


class TicTacToe:
    def __init__(self, point=None):
        self.point = [" "] * 9 if point is None else point[:]
        self.current_winner = None

    def make_move(self, square, letter):
        if self.point[square] != " ":
            return False
        self.point[square] = letter
        if self.winner(square, letter):
            self.current_winner = letter
        return True

    def winner(self, square, letter):
        row_ind = square // 3
        row = self.point[row_ind * 3 : (row_ind + 1) * 3]
        if all(s == letter for s in row):
            return True

        col_ind = square % 3
        column = [self.point[col_ind + i * 3] for i in range(3)]
        if all(s == letter for s in column):
            return True

        if square % 2 == 0:
            diagonal1 = [self.point[i] for i in [0, 4, 8]]
            if all(s == letter for s in diagonal1):
                return True
            diagonal2 = [self.point[i] for i in [2, 4, 6]]
            if all(s == letter for s in diagonal2):
                return True
        return False

    def empty_squares(self):
        return " " in self.point

    def num_empty_squares(self):
        return self.point.count(" ")

    def available_moves(self):
        return [i for i, x in enumerate(self.point) if x == " "]


class SmartComputerPlayer:
    def __init__(self, letter):
        self.letter = letter
        self._cache = {}

    def get_move(self, game):
        if len(game.available_moves()) == 9:
            return 4
        if game.point.count("O") == 1 and game.point[4] == " ":
            return 4
        return self.minimax(game, self.letter)["position"]

    def minimax(self, state, player):
        key = (tuple(state.point), state.current_winner, player)
        cached = self._cache.get(key)
        if cached is not None:
            return dict(cached)

        max_player = self.letter
        other_player = "O" if player == "X" else "X"

        if state.current_winner == other_player:
            score = state.num_empty_squares() + 1
            result = {
                "position": None,
                "score": score if other_player == max_player else -score,
            }
            self._cache[key] = result
            return dict(result)
        if not state.empty_squares():
            result = {"position": None, "score": 0}
            self._cache[key] = result
            return dict(result)

        best = {
            "position": None,
            "score": -math.inf if player == max_player else math.inf,
        }

        for possible_move in state.available_moves():
            state.make_move(possible_move, player)
            sim_score = self.minimax(state, other_player)

            state.point[possible_move] = " "
            state.current_winner = None
            sim_score = dict(sim_score)
            sim_score["position"] = possible_move

            if player == max_player:
                if sim_score["score"] > best["score"]:
                    best = sim_score
            elif sim_score["score"] < best["score"]:
                best = sim_score
        self._cache[key] = best
        return best


@dataclass
class Result:
    ai_win: int = 0
    human_win: int = 0
    draw: int = 0

    @property
    def games(self):
        return self.ai_win + self.human_win + self.draw

    def record(self, winner, ai_letter):
        if winner == ai_letter:
            self.ai_win += 1
        elif winner is None:
            self.draw += 1
        else:
            self.human_win += 1

    def line(self):
        games = self.games
        return (
            f"games={games}, "
            f"AI_win={self.ai_win} ({self.ai_win / games:.2%}), "
            f"draw={self.draw} ({self.draw / games:.2%}), "
            f"AI_loss={self.human_win} ({self.human_win / games:.2%}), "
            f"AI_unbeaten={(self.ai_win + self.draw) / games:.2%}"
        )


def play_random_game(ai_first, rng, ai):
    ai_letter = "O"
    human_letter = "X"
    game = TicTacToe()
    turn = ai_letter if ai_first else human_letter

    while game.empty_squares() and game.current_winner is None:
        if turn == ai_letter:
            move = ai.get_move(game)
        else:
            move = rng.choice(game.available_moves())
        game.make_move(move, turn)
        turn = human_letter if turn == ai_letter else ai_letter
    return game.current_winner, ai_letter


def monte_carlo(ai_first, games, seed):
    rng = random.Random(seed + (1 if ai_first else 2))
    ai = SmartComputerPlayer("O")
    result = Result()
    for _ in range(games):
        winner, ai_letter = play_random_game(ai_first, rng, ai)
        result.record(winner, ai_letter)
    return result


def exhaustive(ai_first):
    ai_letter = "O"
    human_letter = "X"
    ai = SmartComputerPlayer(ai_letter)
    result = Result()

    def rec(game, turn):
        if game.current_winner is not None:
            result.record(game.current_winner, ai_letter)
            return
        if not game.empty_squares():
            result.record(None, ai_letter)
            return

        if turn == ai_letter:
            move = ai.get_move(game)
            next_game = TicTacToe(game.point)
            next_game.current_winner = game.current_winner
            next_game.make_move(move, ai_letter)
            rec(next_game, human_letter)
        else:
            for move in game.available_moves():
                next_game = TicTacToe(game.point)
                next_game.current_winner = game.current_winner
                next_game.make_move(move, human_letter)
                rec(next_game, ai_letter)

    rec(TicTacToe(), ai_letter if ai_first else human_letter)
    return result


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--games", type=int, default=5000, help="random games per side")
    parser.add_argument("--seed", type=int, default=20260602)
    args = parser.parse_args()

    print("Random opponent simulation")
    print("AI first :", monte_carlo(True, args.games, args.seed).line())
    print("AI second:", monte_carlo(False, args.games, args.seed).line())
    print()
    print("Exhaustive human move branches")
    print("AI first :", exhaustive(True).line())
    print("AI second:", exhaustive(False).line())


if __name__ == "__main__":
    main()
