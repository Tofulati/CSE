
// Integration by: Albert, Patrick, Alan, Addy, Michael

#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <iostream>
#include "Graph.h"
#include "Vertex.h"
#include <climits>

struct Vec{
    int x;
    int y;

    Vec(){
        x = 0;
        y = 0;
    }

    Vec(int x, int y){
        this->x = x;
        this->y = y;
    }

    void set(int x, int y){
        this->x = x;
        this->y = y;
    }
};

std::ostream& operator<<(std::ostream& os, const Vec& v){
    os << "(" << v.x << ", " << v.y << ")";

    return os;
}

struct GameState;
std::ostream& operator<<(std::ostream& os, const GameState& state);

struct GameState{
    int** grid;
    bool currentTurn;
    int size;
    int turnCount;

    bool done;
    Vec lastMove;

    GameState(){
        size = 3;
        currentTurn = 0;
        turnCount = 0;
        done = false;

        lastMove.set(-1, -1);

        grid = new int*[size];

        for (int i = 0; i < size; i++){
            grid[i] = new int[size];
            for (int j = 0; j < size; j++){
                grid[i][j] = -1;
            }
        }
    }

    GameState(int size){
        this->size = size;
        currentTurn = 0;
        turnCount = 0;
        done = false;

        lastMove.set(-1, -1);

        grid = new int*[size];

        for (int i = 0; i < size; i++){
            grid[i] = new int[size];
            for (int j = 0; j < size; j++){
                grid[i][j] = -1;
            }
        }
    }

    GameState(const GameState& other){
        size = other.size;
        currentTurn = other.currentTurn;
        turnCount = other.turnCount;
        done = other.done;
        lastMove = other.lastMove;

        grid = new int*[size];

        for (int i = 0; i < size; i++){
            grid[i] = new int[size];
            for (int j = 0; j < size; j++){
                grid[i][j] = other.grid[i][j];
            }
        }
    }

    bool operator==(const GameState& other){
        bool sizeMatch = size == other.size;
        bool currentTurnMatch = currentTurn == other.currentTurn;
        bool turnCountMatch = turnCount == other.turnCount;
        bool doneMatch = done == other.done;
        bool lastMoveMatch = lastMove.x == other.lastMove.x && lastMove.y == other.lastMove.y;
        if (sizeMatch && currentTurnMatch && turnCountMatch && doneMatch && lastMoveMatch){
            for (int i = 0; i < size; i++){
                for (int j = 0; j < size; j++){
                    if (grid[i][j] != other.grid[i][j]){
                        return false;
                    }
                }
            }
            
            return true;
        }
        else{
            return false;
        }
    }

    GameState& operator=(const GameState& other){
        currentTurn = other.currentTurn;
        turnCount = other.turnCount;
        done = other.done;
        lastMove = other.lastMove;

        if (size == other.size){
            for (int i = 0; i < size; i++){
                for (int j = 0; j < size; j++){
                    grid[i][j] = other.grid[i][j];
                }
            }
        }
        else{
            
            for (int i = 0; i < size; i++){
                delete[] grid[i];
            }
            delete[] grid;

            size = other.size;

            grid = new int*[size];

            for (int i = 0; i < size; i++){
                grid[i] = new int[size];
                for (int j = 0; j < size; j++){
                    grid[i][j] = other.grid[i][j];
                }
            }
        }

        return *this;
    }

    // Integration by: Albert

    bool validInput(int x, int y) {
        if ((x == 0 || x == 1 || x == 2) && (y == 0 || y == 1 || y == 2)) {
            return true;
        } else {
            return false;
        }
    }

    bool hasWon(int player){
        for (int i = 0; i < size; i++){
            int count = 0;
            for (int j = 0; j < size; j++){
                if (grid[i][j] == player){
                    count++;
                }
            }
            if (count == size){
                return true;
            }
        }
        for (int i = 0; i < size; i++){
            int count = 0;
            for (int j = 0; j < size; j++){
                if (grid[j][i] == player){
                    count++;
                }
            }
            if (count == size){
                return true;
            }
        }
        int mainDiagCount = 0;
        for (int i = 0; i < size; i++){
            if (grid[i][i] == player){
                mainDiagCount++;
            }
        }
        if (mainDiagCount == size){
            return true;
        }

        int secondaryDiagCount = 0;
        for (int i = 0; i < size; i++){
            if (grid[i][size-1-i] == player){
                secondaryDiagCount++;
            }
        }
        if (secondaryDiagCount == size){
            return true;
        }
        

        return false;
    }

    bool play(int x, int y){
        if (grid[x][y] != -1){
            return false;
        }

        grid[x][y] = currentTurn;
        currentTurn = !currentTurn;
        turnCount++;
        lastMove.set(x, y);

        if (turnCount == size * size){
            done = true;
        }
        else if (hasWon(0) || hasWon(1)){
            done = true;
        }

        return true;
    }

    // Integration by: Albert, Patrick, Alan, Addy, Michael (Tried to get algorithm)

    // int getReward(Vertex<GameState>* successor, int player) {
    //     int minVal, maxVal;

    //     if (successor->data.done) {
    //         if (successor->data.hasWon(1)) {
    //             return 10;
    //         } else if (successor->data.hasWon(0)) {
    //             return -10;
    //         } else {
    //             return 0;
    //         }
    //     }

    //     if (player == 1) {
    //         maxVal = INT_MIN;
    //         for (int i = 0; i < successor->data.size; i++) {
    //             for (int j = 0; j < successor->data.size; j++) {
    //                 if (successor->data.grid[i][j] == -1) {
    //                     successor->data.grid[i][j] = 1;
    //                     maxVal = max(maxVal, getReward(successor, 0));
    //                     successor->data.grid[i][j] = -1;
    //                 }
    //             }
    //         }
    //         return maxVal;
    //     } else {
    //         minVal = INT_MAX;
    //         for (int i = 0; i < successor->data.size; i++) {
    //             for (int j = 0; j < successor->data.size; j++) {
    //                 if (successor->data.grid[i][j] == -1) {
    //                     successor->data.grid[i][j] = 0;
    //                     minVal = min(minVal, getReward(successor, 1));
    //                     successor->data.grid[i][j] = -1;
    //                 }
    //             }
    //         }
    //         return minVal;
    //     }
    // }


    // Integration by: Albert, Patrick, Alan, Addy, Michael

    int getReward(Vertex<GameState>* start, int player, int depth) {
        if (start->neighbors.size() == 0) {
            if (start->data.hasWon(player)) {
                return 100 - depth; 
            }
            else if (start->data.hasWon(!player)) {
                return -100 + depth; 
            }
            else {
                return 50;
            }
        }
        else {
            int reward = getReward(start->neighbors[0]->location, player, depth + 1);
            for (int i = 1; i < start->neighbors.size(); i++) {
                int curr = getReward(start->neighbors[i]->location, player, depth + 1);
                if (start->data.currentTurn == player) {
                    if (curr > reward) {
                        reward = curr;
                    }
                }
                else {
                    if (curr < reward) {
                        reward = curr;
                    }
                }
            }
            return reward;
        }
    }

    Vec validMove(GameState game){
        for (int i = 0; i < game.size; i++){
            for (int j = 0; j < game.size; j++){
                if (game.grid[i][j] == -1){
                    return Vec(i, j);
                }
            }
        }
        return Vec(0,0);
    }


    // Integration by: Albert, Patrick, Alan, Addy, Michael

    Vec findBestMove(GameState game){
        Graph<GameState> stateSpace;
        Vertex<GameState>* start = new Vertex<GameState>(game);
        Vec bestMove = { -1, -1};
        int maxVal = INT_MIN;

        stateSpace.addVertex(start);

        Queue<Vertex<GameState>*> frontier;
        frontier.enqueue(start);

        while (!frontier.isEmpty()){
            Vertex<GameState>* curr = frontier.dequeue();
            if (!curr->data.done){
                for(int i = 0; i < game.size; i++){
                    for (int j = 0; j < game.size; j++){
                        if(curr->data.grid[i][j] == -1){
                            GameState next = curr->data;
                            next.play(i, j);
                            Vertex<GameState>* successor = new Vertex<GameState>(next);
                            stateSpace.addVertex(successor);
                            stateSpace.addDirectedEdge(curr, successor);
                            if (!successor->data.done){
                                frontier.enqueue(successor);
                            }
                        }
                    }
                }
            }
        }

        // Evaluate each move from the current state
        for (size_t i = 0; i < start->neighbors.size(); ++i) {
            auto& neighbor = start->neighbors[i];
            int moveVal = getReward(neighbor->location, game.currentTurn, 0);
            
            // If the move is more favorable than the current best, update bestMove and maxVal
            if (moveVal > maxVal) {
                std::cout << moveVal << std::endl;
                maxVal = moveVal;
                bestMove.x = neighbor->location->data.lastMove.x;
                bestMove.y = neighbor->location->data.lastMove.y;
            }
        }


        return bestMove;
    }

    // Integration by: Albert, Alan

    void begBot(){
        GameState game;

        while(!game.done){

            std::cout << game << std::endl;

            int x, y;

            if (game.currentTurn){
                Vec move = validMove(game);
                x = move.x;
                y = move.y;
            } else {
                std::cout << std::endl;
                std::cout << "Enter move for (" << (!game.currentTurn ? "X" : "O") << "): ";
                std::cin >> x >> y;

                while (!validInput(x, y)) {
                    std:: cout << game << std::endl;

                    std::cout << std::endl;
                    std::cout << "Invalid move. Enter move for (" << (!game.currentTurn ? "X" : "O") << "): ";
                    std::cin >> x >> y;
                }
            }
            game.play(x, y);
        }

        system("clear");
        std::cout << game << std::endl;
        std::cout << std::endl;
        if (game.hasWon(0)){
            std::cout << "Player X has won" << std::endl;
        }
        else if (game.hasWon(1)){
            std::cout << "Player O has won" << std::endl;
        }
        else {
            std::cout << "It's a tie" << std::endl;
        }
        std::cout << std::endl;
        std::cout << "Press [enter] to return to main menu" << std::endl;
        std::cin.ignore(9999999, '\n');
        std::cin.get();
    }

    // Integration by: Addy

    void hvh(){
        GameState game;

        while(!game.done){

            std::cout << game << std::endl;

            int x, y;

            std::cout << std::endl;
            std::cout << "Enter move for (" << (!game.currentTurn ? "X" : "O") << "): ";
            std::cin >> x >> y;

            while (!validInput(x, y)) {
                std:: cout << game << std::endl;

                std::cout << std::endl;
                std::cout << "Invalid move. Enter move for (" << (!game.currentTurn ? "X" : "O") << "): ";
                std::cin >> x >> y;
            }

            game.play(x, y);
        }

        system("clear");
        std::cout << game << std::endl;
        std::cout << std::endl;
        if (game.hasWon(0)){
            std::cout << "Player X has won" << std::endl;
        }
        else if (game.hasWon(1)){
            std::cout << "Player O has won" << std::endl;
        }
        else {
            std::cout << "It's a tie" << std::endl;
        }
        std::cout << std::endl;
        std::cout << "Press [enter] to return to main menu" << std::endl;
        std::cin.ignore(9999999, '\n');
        std::cin.get();
    }


    // Integration by: Patrick, Michael

    void advBot(){
        GameState game;

        while(!game.done){

            std::cout << game << std::endl;

            int x, y;
            if (game.currentTurn){
                Vec move = findBestMove(game);
                x = move.x;
                y = move.y;
            } else{
                std::cout << std::endl;
                std::cout << "Enter move for (" << (!game.currentTurn ? "X" : "O") << "): ";
                std::cin >> x >> y;

                while (!validInput(x, y)) {
                    std:: cout << game << std::endl;

                    std::cout << std::endl;
                    std::cout << "Invalid move. Enter move for (" << (!game.currentTurn ? "X" : "O") << "): ";
                    std::cin >> x >> y;
                }
            }

            game.play(x, y);
        }

        system("clear");
        std::cout << game << std::endl;
        std::cout << std::endl;
        if (game.hasWon(0)){
            std::cout << "Player X has won" << std::endl;
        }
        else if (game.hasWon(1)){
            std::cout << "Player O has won" << std::endl;
        }
        else {
            std::cout << "It's a tie" << std::endl;
        }
        std::cout << std::endl;
        std::cout << "Press [enter] to return to main menu" << std::endl;
        std::cin.ignore(9999999, '\n');
        std::cin.get();
    }


    // Integration by: Albert, Alan

    void start(){
        bool exit = false;
        int option;
        while(!exit){
            system("clear");
            //cout << game << endl;
            std::cout << "=================================" << std::endl;
            std::cout << "            Main Menu                " << std::endl;
            std::cout << "=================================" << std::endl;
            std::cout << "1) Play against human " << std::endl;
            std::cout << "2) Play against beginner bot " << std::endl;
            std::cout << "3) Play against advanced bot " << std::endl;
            std::cout << "4) Leave game." << std::endl;
            std::cout << std::endl;
            std::cout << "Select an option:" << std::endl;
            std::cin >> option;

            if(option == 1){
                system("clear");
                hvh();
            }

            if(option == 2){
                system("clear");
                begBot();
            }

            if(option == 3){
                system("clear");
                advBot();
            }   

            if (option == 4) {
                system("clear");
                exit = true;
            }
        }
    }

    ~GameState(){
        for (int i = 0; i < size; i++){
            delete[] grid[i];
        }
        delete[] grid;
    }
};

std::ostream& operator<<(std::ostream& os, const GameState& state){
    system("clear");
    os << "   ";
    for (int j = 0; j < state.size; j++){
        os << " " << j << "  ";
    }
    os << std::endl;
    os << "   ";
    for (int j = 0; j < state.size; j++){
        os << "--- ";
    }
    os << std::endl;
    for (int i = 0; i < state.size; i++){
        os << i << " ";
        for (int j = 0; j < state.size; j++){
            char c = ' ';
            if (state.grid[i][j] == 0){
                c = 'X';
            }
            else if (state.grid[i][j] == 1){
                c = 'O';
            }
            os << "| " << c << " ";
            if (j == state.size - 1) os << "|";
        }
        os << std::endl << "   ";
        for (int j = 0; j < state.size; j++){
            os << "--- ";
        }
        os << std::endl;
    }

    return os;
}

#endif
