#include "minesweeper.h"

Minesweeper::Minesweeper(size_t dimension)
{
    map_dim = dimension;
    map_size = map_dim * map_dim;
    is_first_click = true;
    p_loc.x = 0x0;
    p_loc.y = 0x0;
}

void Minesweeper::upd_player_loc(int x, int y)
{
    if(x < 0 || x > 255 || y < 0 || y > 255)
        return;
    p_loc.x = static_cast<uint8_t>(x);
    p_loc.y = static_cast<uint8_t>(y);
}

void Minesweeper::gen_map()
{
    is_first_click = false;
}

void Minesweeper::rev_sel_tile()
{
    
}