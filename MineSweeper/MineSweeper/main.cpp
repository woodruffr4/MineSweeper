
//
// Disclaimer:
// ----------
//
// This code will work only if you selected window, graphics and audio.
//
// Note that the "Run Script" build phase will copy the required frameworks
// or dylibs to your application bundle so you can execute it on any OS X
// computer.
//
// Your resource files (images, sounds, fonts, ...) are also copied to your
// application bundle. To get the path to these resources, use the helper
// function `resourcePath()` from ResourcePath.hpp
//

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include <ctime>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <fstream>
#include "ResourcePath.hpp"
#include "Tile.hpp"
using namespace std;

mt19937 random_mt;
int r, c;
int numFlagsAvailable;

int dx[] = {0,0,1,1,1,-1,-1,-1};
int dy[] = {1,-1,0,1,-1,0,1,-1};

//int dx2[] = {0,0,1,-1};
//int dy2[] = {1,-1,0,0};

int dx2[] = {0,0,1,1,1,-1,-1,-1};
int dy2[] = {1,-1,0,1,-1,0,1,-1};

int Random(int min, int max)
{
    uniform_int_distribution<int> dist(min, max);
    return dist(random_mt);
}

void toggleAllMines(Tile* (&grid)[16][25]) {
    for(int i=0;i<r;i++) {
        for(int j=0;j<c;j++) {
            if(grid[i][j]->isMine()) {
                grid[i][j]->toggleHidden();
            }
        }
    }
}

void endGame(Tile* (&grid)[16][25]) {
    toggleAllMines(grid);
}

void resetGame(Tile* (&grid)[16][25]) {
    
    //Create randomized set of 50 bombs
    unordered_set<int> mineLoc;
    while(mineLoc.size()!=50) {
        mineLoc.insert(Random(0,399));
    }
    
    for(int i=0;i<r;i++) {
        for(int j=0;j<c;j++) {
            int num = 25*i+j;
            if(mineLoc.find(num)!=mineLoc.end()) {
                if(grid[i][j]!=nullptr) {
                    delete grid[i][j];
                }
                Tile* newTile = new Tile(0,true,false,true);
                grid[i][j] = newTile;
            } else {
                if(grid[i][j]!=nullptr) {
                    delete grid[i][j];
                }
                Tile* newTile = new Tile(0,true,false,false);
                grid[i][j] = newTile;
            }
        }
    }
    
    for(int i=0;i<r;i++) {
        for(int j=0;j<c;j++) {
            Tile* tile = grid[i][j];
            if(tile->isMine()) {
                for(int k=0;k<8;k++) {
                    int newr = i+dx[k];
                    int newc = j+dy[k];
                    if(newr >= 0 && newr < r && newc >= 0 && newc < c) {
                        Tile* add = grid[newr][newc];
                        if(!add->isMine()) {
                            add->add();
                        }
                    }
                }
            }
        }
    }
    
    numFlagsAvailable = 50;
}

bool didWin(Tile* (&grid)[16][25]) {
    for(int i=0;i<r;i++) {
        for(int j=0;j<c;j++) {
            if(!grid[i][j]->isHidden()) continue;
            if(grid[i][j]->isMine()) continue;
            else return false;
        }
    }
    
    //Set all mines with flags
    for(int i=0;i<r;i++) {
        for(int j=0;j<c;j++) {
            if(grid[i][j]->isMine()) {
                if(grid[i][j]->isFlagged()) continue;
                else grid[i][j]->toggleFlagged();
            }
        }
    }
    
    //set Flag counter to 0
    numFlagsAvailable = 0;
    
    return true;
}

void resetGameFromFile(Tile* (&grid)[16][25], string filename) {
    ifstream file(resourcePath()+filename);
    string line;
    numFlagsAvailable = 0;
    for(int i=0;i<16;i++) {
        getline(file, line);
        for(int j=0;j<25;j++) {
            if(line[j]=='0') {
                if(grid[i][j]!=nullptr) {
                    delete grid[i][j];
                }
                grid[i][j]=new Tile(0,true,false,false);
            } else {
                numFlagsAvailable++;
                if(grid[i][j]!=nullptr) {
                    delete grid[i][j];
                }
                grid[i][j]=new Tile(0,true,false,true);
            }
        }
    }
    
    for(int i=0;i<r;i++) {
        for(int j=0;j<c;j++) {
            Tile* tile = grid[i][j];
            if(tile->isMine()) {
                for(int k=0;k<8;k++) {
                    int newr = i+dx[k];
                    int newc = j+dy[k];
                    if(newr >= 0 && newr < r && newc >= 0 && newc < c) {
                        Tile* add = grid[newr][newc];
                        if(!add->isMine()) {
                            add->add();
                        }
                    }
                }
            }
        }
    }
}

void loadTextures(map<string, sf::Texture> &textures) {
    sf::Texture debug;
    if (!debug.loadFromFile(resourcePath() + "debug.png")) {
        return EXIT_FAILURE;
    }
    textures["debug"] = debug;
    
    sf::Texture digits;
    if (!digits.loadFromFile(resourcePath() + "digits.png")) {
        return EXIT_FAILURE;
    }
    textures["digits"] = digits;
    
    sf::Texture face_happy;
    if (!face_happy.loadFromFile(resourcePath() + "face_happy.png")) {
        return EXIT_FAILURE;
    }
    textures["face_happy"] = face_happy;
    
    sf::Texture face_lose;
    if (!face_lose.loadFromFile(resourcePath() + "face_lose.png")) {
        return EXIT_FAILURE;
    }
    textures["face_lose"] = face_lose;
    
    sf::Texture face_win;
    if (!face_win.loadFromFile(resourcePath() + "face_win.png")) {
        return EXIT_FAILURE;
    }
    textures["face_win"] = face_win;
    
    sf::Texture flag;
    if (!flag.loadFromFile(resourcePath() + "flag.png")) {
        return EXIT_FAILURE;
    }
    textures["flag"] = flag;
    
    sf::Texture mine;
    if (!mine.loadFromFile(resourcePath() + "mine.png")) {
        return EXIT_FAILURE;
    }
    textures["mine"] = mine;
    
    sf::Texture number_1;
    if (!number_1.loadFromFile(resourcePath() + "number_1.png")) {
        return EXIT_FAILURE;
    }
    textures["number_1"] = number_1;
    
    sf::Texture number_2;
    if (!number_2.loadFromFile(resourcePath() + "number_2.png")) {
        return EXIT_FAILURE;
    }
    textures["number_2"] = number_2;
    
    sf::Texture number_3;
    if (!number_3.loadFromFile(resourcePath() + "number_3.png")) {
        return EXIT_FAILURE;
    }
    textures["number_3"] = number_3;
    
    sf::Texture number_4;
    if (!number_4.loadFromFile(resourcePath() + "number_4.png")) {
        return EXIT_FAILURE;
    }
    textures["number_4"] = number_4;
    
    sf::Texture number_5;
    if (!number_5.loadFromFile(resourcePath() + "number_5.png")) {
        return EXIT_FAILURE;
    }
    textures["number_5"] = number_5;
    
    sf::Texture number_6;
    if (!number_6.loadFromFile(resourcePath() + "number_6.png")) {
        return EXIT_FAILURE;
    }
    textures["number_6"] = number_6;
    
    sf::Texture number_7;
    if (!number_7.loadFromFile(resourcePath() + "number_7.png")) {
        return EXIT_FAILURE;
    }
    textures["number_7"] = number_7;
    
    sf::Texture number_8;
    if (!number_8.loadFromFile(resourcePath() + "number_8.png")) {
        return EXIT_FAILURE;
    }
    textures["number_8"] = number_8;
    
    sf::Texture test_1;
    if (!test_1.loadFromFile(resourcePath() + "test_1.png")) {
        return EXIT_FAILURE;
    }
    textures["test_1"] = test_1;
    
    sf::Texture test_2;
    if (!test_2.loadFromFile(resourcePath() + "test_2.png")) {
        return EXIT_FAILURE;
    }
    textures["test_2"] = test_2;
    
    sf::Texture tile_hidden;
    if (!tile_hidden.loadFromFile(resourcePath() + "tile_hidden.png")) {
        return EXIT_FAILURE;
    }
    textures["tile_hidden"] = tile_hidden;
    
    sf::Texture tile_revealed;
    if (!tile_revealed.loadFromFile(resourcePath() + "tile_revealed.png")) {
        return EXIT_FAILURE;
    }
    textures["tile_revealed"] = tile_revealed;
    
    for(int i=0;i<=9;i++) {
        sf::Texture digit;
        if(!digit.loadFromFile(resourcePath() + "digits.png", sf::IntRect(21*i,0,21,32))) {
            return EXIT_FAILURE;
        }
        string name = "digit_"+to_string(i);
        textures[name] = digit;
    }
}

int main(int, char const**)
{
    bool gameOver = false;
    bool won = false;
    numFlagsAvailable = 50;
    random_mt.seed(time(0));
    
    
    
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");

    // Set the Icon
    sf::Image icon;
    if (!icon.loadFromFile(resourcePath() + "icon.png")) {
        return EXIT_FAILURE;
    }
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    
    
    //Read in all textures
    map<string, sf::Texture> textures;
    loadTextures(textures);
    
    Tile* grid[16][25];
    r = 16;
    c = 25;
    for(int i=0;i<r;i++) {
        for(int j=0;j<c;j++) {
            grid[i][j]=nullptr;
        }
    }
    
    resetGame(grid);
   
    // Start the game loop
    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        
        sf::Vector2i flag;
        bool flagged = false;
        
        sf::Vector2i click;
        bool clicked = false;
        
        sf::Sprite restart(textures["face_happy"]);
        restart.setPosition(11.5*32.0, 16*32.0);
        
        sf::Sprite debug(textures["debug"]);
        debug.setPosition(11.5*32.0+128, 16*32.0);
        
        sf::Sprite test1(textures["test_1"]);
        test1.setPosition(11.5*32.0+128+64, 16*32.0);
        
        sf::Sprite test2(textures["test_2"]);
        test2.setPosition(11.5*32.0+128+64+64, 16*32.0);
        
        sf::Sprite digit1(textures["digit_0"]);
        sf::Sprite digit2(textures["digit_0"]);
        sf::Sprite digit3(textures["digit_0"]);
        
        digit1.setPosition(0, 16*32.0);
        digit2.setPosition(21.0, 16*32.0);
        digit3.setPosition(42.0, 16*32.0);
        
        if(numFlagsAvailable<0) {
            
        }
        else if(numFlagsAvailable/10 == 0) {
            //1 digits
            string name = "digit_"+to_string(numFlagsAvailable%10);
            digit3.setTexture(textures[name]);
        }
        else if(numFlagsAvailable/100 == 0) {
            //2 digits
            string name1 = "digit_"+to_string(numFlagsAvailable/10);
            string name2 = "digit_"+to_string(numFlagsAvailable%10);
            digit2.setTexture(textures[name1]);
            digit3.setTexture(textures[name2]);
        }
        else {
            //MAX 3 digits
            string name1 = "digit_"+to_string(numFlagsAvailable/10);
            string name2 = "digit_"+to_string(numFlagsAvailable%10);
            string name3 = "digit_"+to_string(numFlagsAvailable/100);
            digit2.setTexture(textures[name1]);
            digit3.setTexture(textures[name2]);
            digit1.setTexture(textures[name3]);
        }
        
        if(didWin(grid)) {
            won = true;
            restart.setTexture(textures["face_win"]);
        }
        if(gameOver) {
            restart.setTexture(textures["face_lose"]);
        }
        
        while (window.pollEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // Escape pressed: exit
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                window.close();
            }
            
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                click = sf::Mouse::getPosition(window);
                clicked = true;
            }
            
            if(sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
                flag = sf::Mouse::getPosition(window);
                flagged = true;
            }
        }
        
        //Restart?
        if((won || gameOver) && clicked && click.x>=(11.5*32.0) && click.x<(13.5*32.0) && click.y>=(16*32.0) && click.y<(18*32.0)) {
            clicked = false;
            resetGame(grid);
            gameOver = false;
            won = false;
        }
        
        //Debug?
        if(!won && !gameOver && clicked && click.x>=(11.5*32.0+128) && click.x<(13.5*32.0+128) && click.y>=(16*32.0) && click.y<(18*32.0)) {
            clicked = false;
            toggleAllMines(grid);
        }
        
        //Load test 1
        if(!won && !gameOver && clicked && click.x>=(11.5*32.0+128+64) && click.x<(13.5*32.0+128+64) && click.y>=(16*32.0) && click.y<(18*32.0)) {
            resetGameFromFile(grid, "testboard.brd");
            gameOver = false;
            won = false;
            clicked = false;
        }
        
        //Load test 2
        if(!won && !gameOver && clicked && click.x>=(11.5*32.0+128+64+64) && click.x<(13.5*32.0+128+64+64) && click.y>=(16*32.0) && click.y<(18*32.0)) {
            resetGameFromFile(grid, "testboard2.brd");
            gameOver = false;
            won = false;
            clicked = false;
        }
        
        
    
        
        for(int i=0;i<r;i++) {
            for(int j=0;j<c;j++) {
                float topx = j*32.0;
                float topy = i*32.0;
                float nextx = (j+1)*32.0;
                float nexty = (i+1)*32.0;
                if(!won && !gameOver && clicked && click.x>=topx && click.x<nextx && click.y>=topy && click.y<nexty) {
                    Tile* tile = grid[i][j];
                    if(tile->isFlagged()) {
                        clicked = false;
                        continue;
                    }
                    if(tile->isHidden()) {
                        //Reveal it
                        if(tile->isMine()) {
                            // LOSE
                            gameOver = true;
                            endGame(grid);
                        } else {
                            int number = tile->getNumber();
                            if(number==0) {
                                
                                //BFS
                                queue<int> qx;
                                queue<int> qy;
                                qx.push(i);
                                qy.push(j);
                                while(qx.size()!=0) {
                                    int x = qx.front();
                                    int y = qy.front();
                                    qx.pop();
                                    qy.pop();
                                    Tile* t = grid[x][y];
                                    if(!t->isHidden()) continue;
                                    if(t->isMine()) continue;
                                    if(t->isFlagged()) {
                                        //Idk what to do here
                                        continue;
                                        //numFlagsAvailable++;
                                        // Subtract from counter
                                    }
                                    t->toggleHidden();
                                    if(t->getNumber()!=0) {
                                        continue;
                                    } else {
                                        for(int k=0;k<8;k++) {
                                            int newx = x+dx2[k];
                                            int newy = y+dy2[k];
                                            if(newx>=0 && newx<r && newy>=0 && newy<c) {
                                                qx.push(newx);
                                                qy.push(newy);
                                            }
                                        }
                                    }
                                }
                                
                                
                                
                            } else {
                                tile->toggleHidden();
                            }
                        }
                    } else {
                        continue;
                    }
                    clicked = false;
                }
                else if(!won && !gameOver && flagged && flag.x>=topx && flag.x<nextx && flag.y>=topy && flag.y<nexty) {
                    Tile* tile = grid[i][j];
                    tile->toggleFlagged();
                    if(tile->isFlagged()) numFlagsAvailable--;
                    else numFlagsAvailable++;
                    flagged = false;
                }
            }
        }

        // Clear screen
        window.clear();
        
        window.draw(restart);
        
        window.draw(debug);
        
        window.draw(test1);
        
        window.draw(test2);
        
        window.draw(digit1);
        window.draw(digit2);
        window.draw(digit3);

        // Draw the sprite
        //window.draw(sprite);
        
        for(int i=0;i<16;i++) {
            for(int j=0;j<25;j++) {
                int num = i*25+j;
                
                Tile* tile = grid[i][j];
                if(tile->isHidden()) {
                    sf::Sprite hidden(textures["tile_hidden"]);
                    hidden.setPosition((float)j*32.0, (float)i*32.0);
                    window.draw(hidden);
                    if(tile->isFlagged()) {
                        sf::Sprite flagged(textures["flag"]);
                        flagged.setPosition((float)j*32.0, (float)i*32.0);
                        window.draw(flagged);
                    }
                } else {
                    sf::Sprite revealed(textures["tile_revealed"]);
                    revealed.setPosition((float)j*32.0, (float)i*32.0);
                    window.draw(revealed);
                    if(tile->isMine()) {
                        sf::Sprite mine(textures["mine"]);
                        mine.setPosition((float)j*32.0, (float)i*32.0);
                        window.draw(mine);
                    } else {
                        int number = tile->getNumber();
                        if(number!=0) {
                            string file = "number_"+to_string(number);
                            sf::Sprite showNum(textures[file]);
                            showNum.setPosition((float)j*32.0, (float)i*32.0);
                            window.draw(showNum);
                        }
                    }
                }
                
            }
        }
        
        
        // Draw the string
        //window.draw(text);

        // Update the window
        window.display();
    }
    
    for(int i=0;i<r;i++) {
        for(int j=0;j<c;j++) {
            delete grid[i][j];
        }
    }
    
    return EXIT_SUCCESS;
}
