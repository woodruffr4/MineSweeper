//
//  Tile.cpp
//  MineSweeper
//
//  Created by Ricky Woodruff on 11/30/18.
//  Copyright © 2018 Ricky Woodruff. All rights reserved.
//

#include "Tile.hpp"

Tile::Tile() {
    
}

Tile::Tile(int number, bool hidden, bool flagged, bool mine) {
    this->number = number;
    this->flagged = flagged;
    this->mine = mine;
    this->hidden = hidden;
}
Tile& Tile::operator=(const Tile &t) {
    this->number = t.number;
    this->flagged = t.flagged;
    this->mine = t.mine;
    this->hidden = t.hidden;
}
Tile::~Tile() {
    
}

bool Tile::isHidden() const {
    return hidden;
}
void Tile::toggleHidden() {
    hidden = !hidden;
}
bool Tile::isMine() const {
    return mine;
}
void Tile::setMine() {
    mine = true;
}
int Tile::getNumber() const {
    return number;
}
void Tile::add() {
    number++;
}
bool Tile::isFlagged() const {
    return flagged;
}
void Tile::toggleFlagged() {
    flagged = !flagged;
}
