#include <iostream>
#include <random>
#include <vector>

using namespace std;

struct Point {
    int x;
    int y;
};

bool isSnakeAt(const vector<Point>& snake, int x, int y) {
    for (const Point& segment : snake) {
        if (segment.x == x && segment.y == y) {
            return true;
        }
    }
    return false;
}

bool placeFood(Point& food, const vector<Point>& snake, int width, int height,
               mt19937& randomGenerator) {
    vector<Point> emptyCells;
    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < width - 1; x++) {
            if (!isSnakeAt(snake, x, y)) {
                emptyCells.push_back({x, y});
            }
        }
    }

    if (emptyCells.empty()) {
        return false;
    }

    uniform_int_distribution<size_t> chooseCell(0, emptyCells.size() - 1);
    food = emptyCells[chooseCell(randomGenerator)];
    return true;
}

int main() {
    const int width = 20;
    const int height = 12;

    vector<Point> snake = {{5, 5}};
    Point food = {10, 7};
    int score = 0;
    mt19937 randomGenerator(random_device{}());

    while (true) {
        cout << "\033[2J\033[H";
        cout << "Рахунок: " << score << "\n";

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                if (x == 0 || x == width - 1 || y == 0 || y == height - 1) {
                    cout << '#';
                } else {
                    bool printedSnake = false;
                    for (size_t i = 0; i < snake.size(); i++) {
                        if (snake[i].x == x && snake[i].y == y) {
                            cout << (i == 0 ? 'O' : 'o');
                            printedSnake = true;
                            break;
                        }
                    }
                    if (!printedSnake) {
                        cout << (x == food.x && y == food.y ? '*' : ' ');
                    }
                }
            }
            cout << '\n';
        }

        cout << "W/A/S/D - рух, Q - вихід: " << flush;
        char command;
        if (!(cin >> command)) {
            break;
        }

        if (command == 'q' || command == 'Q') {
            break;
        }

        Point nextHead = snake.front();

        switch (command) {
            case 'w': case 'W': nextHead.y--; break;
            case 's': case 'S': nextHead.y++; break;
            case 'a': case 'A': nextHead.x--; break;
            case 'd': case 'D': nextHead.x++; break;
            default: continue;
        }

        if (nextHead.x == 0 || nextHead.x == width - 1 ||
            nextHead.y == 0 || nextHead.y == height - 1) {
            cout << "\033[2J\033[H";
            cout << "Гру завершено: змійка врізалася у стіну!\n";
            return 0;
        }

        const bool eating = nextHead.x == food.x && nextHead.y == food.y;
        const size_t segmentsToCheck = snake.size() - (eating ? 0 : 1);
        for (size_t i = 0; i < segmentsToCheck; i++) {
            if (snake[i].x == nextHead.x && snake[i].y == nextHead.y) {
                cout << "\033[2J\033[H";
                cout << "Гру завершено: змійка врізалася у власний хвіст!\n";
                return 0;
            }
        }

        snake.insert(snake.begin(), nextHead);
        if (eating) {
            score++;
            if (!placeFood(food, snake, width, height, randomGenerator)) {
                cout << "\033[2J\033[H";
                cout << "Вітаю, ти заповнив усе поле!\n";
                return 0;
            }
        } else {
            snake.pop_back();
        }
    }

    return 0;
}