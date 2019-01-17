//
//  Tile.hpp
//  MineSweeper
//
//  Created by Ricky Woodruff on 11/30/18.
//  Copyright © 2018 Ricky Woodruff. All rights reserved.
//

#pragma once

#include <stdio.h>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

class Tile {
public:
    Tile();
    Tile(int number, bool hidden, bool flagged, bool mine);
    ~Tile();
    Tile& operator=(const Tile &t);
    
    int number;
    bool flagged;
    bool mine;
    bool hidden;
    
    bool isHidden() const;
    void toggleHidden();
    bool isMine() const;
    void setMine();
    int getNumber() const;
    void add();
    bool isFlagged() const;
    void toggleFlagged();
};

