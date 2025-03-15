#include "minesweeper.h"
#include <random>
#include <unordered_set>

Minesweeper::Minesweeper(size_t dimension, float density)
{
    map_dim = dimension;
    map_size = map_dim * map_dim;
    is_first_click = true;
    p_loc.x = 0x0;
    p_loc.y = 0x0;
    mine_amount = density * (map_size);
    current_flagged = 0;
}

void Minesweeper::upd_player_loc_mouse(int x, int y)
{
    if(x < 0 || x > 255 || y < 0 || y > 255)
        return;
    p_loc.x = static_cast<uint8_t>(x);
    p_loc.y = static_cast<uint8_t>(y);
}

void Minesweeper::gen_map()
{
    is_first_click = false;
    //Random number generation (for bombs)
    std::random_device rand_seed;
    std::mt19937 rand_gen(rand_seed());
    std::uniform_int_distribution<int> rand_range(0, map_size);
    //Ensure no duplicates
    std::unordered_set<int> unique_num;
    while (unique_num.size() < mine_amount)
    {
        int rand_num = rand_range(rand_gen);
        unique_num.insert(rand_num);
    }
    
    //Fill squares in map
    ms_tile_info tile {0, 0, 0, 0, 0};
    for (int i = 0; i < map_size; i++)
    {
        map.push_back(tile);
        tile.id++;
    }

    //Set generated bomb tiles
    for (const auto& bomb_id : unique_num)
        map[bomb_id].is_bomb = 0x1;
}

int Minesweeper::edgecase_check(uint16_t map_pos)
{
    int temp = 0;
    if (map_pos % map_dim == 0) 
        temp = 1; //left
    if (map_pos % map_dim == map_dim - 1)
        temp = 2; //right
    if (map_pos < map_dim)
        temp = 3; //top
    if (map_pos >= (map_dim - 1) * map_dim)
        temp = 4; //down
    if (!temp)
        return 0;

    //Check for corner case
    if (map_pos == 0)
        return 5; //Top left
    if (map_pos == (map_size - 1))
        return 6; //Bottom right
    if (map_pos == (map_size - map_dim))
        return 7; //Bottom left
    if (map_pos == (map_dim - 1))
        return 8; //Top right

    return temp;
}

//Average code block moment
void Minesweeper::rev_sel_tile_recurse(uint16_t p_map_pos)
{
    map[p_map_pos].is_rev = 1;
    switch(edgecase_check(p_map_pos))
    {
        case 0: //none
            if (!map[MINESWEEPER_TILE_ABOVE(p_map_pos)].is_rev && !map[MINESWEEPER_TILE_ABOVE(p_map_pos)].is_bomb && !map[MINESWEEPER_TILE_ABOVE(p_map_pos)].is_flag)
                rev_sel_tile_recurse(MINESWEEPER_TILE_ABOVE(p_map_pos));
            if (!map[MINESWEEPER_TILE_DOWN(p_map_pos)].is_rev && !map[MINESWEEPER_TILE_DOWN(p_map_pos)].is_bomb && !map[MINESWEEPER_TILE_DOWN(p_map_pos)].is_flag)
                rev_sel_tile_recurse(MINESWEEPER_TILE_DOWN(p_map_pos));
            if (!map[MINESWEEPER_TILE_LEFT(p_map_pos)].is_rev && !map[MINESWEEPER_TILE_LEFT(p_map_pos)].is_bomb && !map[MINESWEEPER_TILE_LEFT(p_map_pos)].is_flag)
                rev_sel_tile_recurse(MINESWEEPER_TILE_LEFT(p_map_pos));
            if (!map[MINESWEEPER_TILE_RIGHT(p_map_pos)].is_rev && !map[MINESWEEPER_TILE_RIGHT(p_map_pos)].is_bomb && !map[MINESWEEPER_TILE_RIGHT(p_map_pos)].is_flag)
                rev_sel_tile_recurse(MINESWEEPER_TILE_RIGHT(p_map_pos));           
            break;
        case 1: //left
            if (!map[MINESWEEPER_TILE_ABOVE(p_map_pos)].is_rev && !map[MINESWEEPER_TILE_ABOVE(p_map_pos)].is_bomb && !map[MINESWEEPER_TILE_ABOVE(p_map_pos)].is_flag)
                rev_sel_tile_recurse(MINESWEEPER_TILE_ABOVE(p_map_pos));
            if (!map[MINESWEEPER_TILE_DOWN(p_map_pos)].is_rev && !map[MINESWEEPER_TILE_DOWN(p_map_pos)].is_bomb && !map[MINESWEEPER_TILE_DOWN(p_map_pos)].is_flag)
                rev_sel_tile_recurse(MINESWEEPER_TILE_DOWN(p_map_pos));
            if (!map[MINESWEEPER_TILE_RIGHT(p_map_pos)].is_rev && !map[MINESWEEPER_TILE_RIGHT(p_map_pos)].is_bomb && !map[MINESWEEPER_TILE_RIGHT(p_map_pos)].is_flag)
                rev_sel_tile_recurse(MINESWEEPER_TILE_RIGHT(p_map_pos));  
            break;
        case 2: //right
            if (!map[MINESWEEPER_TILE_ABOVE(p_map_pos)].is_rev && !map[MINESWEEPER_TILE_ABOVE(p_map_pos)].is_bomb && !map[MINESWEEPER_TILE_ABOVE(p_map_pos)].is_flag)
                rev_sel_tile_recurse(MINESWEEPER_TILE_ABOVE(p_map_pos));
            if (!map[MINESWEEPER_TILE_DOWN(p_map_pos)].is_rev && !map[MINESWEEPER_TILE_DOWN(p_map_pos)].is_bomb && !map[MINESWEEPER_TILE_DOWN(p_map_pos)].is_flag)
                rev_sel_tile_recurse(MINESWEEPER_TILE_DOWN(p_map_pos));
            if (!map[MINESWEEPER_TILE_LEFT(p_map_pos)].is_rev && !map[MINESWEEPER_TILE_LEFT(p_map_pos)].is_bomb && !map[MINESWEEPER_TILE_LEFT(p_map_pos)].is_flag)
                rev_sel_tile_recurse(MINESWEEPER_TILE_LEFT(p_map_pos));
            break;
        case 3: //top
            if (!map[MINESWEEPER_TILE_DOWN(p_map_pos)].is_rev && !map[MINESWEEPER_TILE_DOWN(p_map_pos)].is_bomb && !map[MINESWEEPER_TILE_DOWN(p_map_pos)].is_flag)
                rev_sel_tile_recurse(MINESWEEPER_TILE_DOWN(p_map_pos));
            if (!map[MINESWEEPER_TILE_LEFT(p_map_pos)].is_rev && !map[MINESWEEPER_TILE_LEFT(p_map_pos)].is_bomb && !map[MINESWEEPER_TILE_LEFT(p_map_pos)].is_flag)
                rev_sel_tile_recurse(MINESWEEPER_TILE_LEFT(p_map_pos));
            if (!map[MINESWEEPER_TILE_RIGHT(p_map_pos)].is_rev && !map[MINESWEEPER_TILE_RIGHT(p_map_pos)].is_bomb && !map[MINESWEEPER_TILE_RIGHT(p_map_pos)].is_flag)
                rev_sel_tile_recurse(MINESWEEPER_TILE_RIGHT(p_map_pos)); 
            break;
        case 4: //down
            if (!map[MINESWEEPER_TILE_ABOVE(p_map_pos)].is_rev && !map[MINESWEEPER_TILE_ABOVE(p_map_pos)].is_bomb && !map[MINESWEEPER_TILE_ABOVE(p_map_pos)].is_flag)
                rev_sel_tile_recurse(MINESWEEPER_TILE_ABOVE(p_map_pos));
            if (!map[MINESWEEPER_TILE_LEFT(p_map_pos)].is_rev && !map[MINESWEEPER_TILE_LEFT(p_map_pos)].is_bomb && !map[MINESWEEPER_TILE_LEFT(p_map_pos)].is_flag)
                rev_sel_tile_recurse(MINESWEEPER_TILE_LEFT(p_map_pos));
            if (!map[MINESWEEPER_TILE_RIGHT(p_map_pos)].is_rev && !map[MINESWEEPER_TILE_RIGHT(p_map_pos)].is_bomb && !map[MINESWEEPER_TILE_RIGHT(p_map_pos)].is_flag)
                rev_sel_tile_recurse(MINESWEEPER_TILE_RIGHT(p_map_pos));   
            break;
        case 5: //top left
            if (!map[MINESWEEPER_TILE_RIGHT(p_map_pos)].is_rev && !map[MINESWEEPER_TILE_RIGHT(p_map_pos)].is_bomb && !map[MINESWEEPER_TILE_RIGHT(p_map_pos)].is_flag)
                rev_sel_tile_recurse(MINESWEEPER_TILE_RIGHT(p_map_pos)); 
            if (!map[MINESWEEPER_TILE_DOWN(p_map_pos)].is_rev && !map[MINESWEEPER_TILE_DOWN(p_map_pos)].is_bomb && !map[MINESWEEPER_TILE_DOWN(p_map_pos)].is_flag)
                rev_sel_tile_recurse(MINESWEEPER_TILE_DOWN(p_map_pos));
            break;
        case 6: //bottom right
            if (!map[MINESWEEPER_TILE_ABOVE(p_map_pos)].is_rev && !map[MINESWEEPER_TILE_ABOVE(p_map_pos)].is_bomb && !map[MINESWEEPER_TILE_ABOVE(p_map_pos)].is_flag)
                rev_sel_tile_recurse(MINESWEEPER_TILE_ABOVE(p_map_pos));
            if (!map[MINESWEEPER_TILE_LEFT(p_map_pos)].is_rev && !map[MINESWEEPER_TILE_LEFT(p_map_pos)].is_bomb && !map[MINESWEEPER_TILE_LEFT(p_map_pos)].is_flag)
                rev_sel_tile_recurse(MINESWEEPER_TILE_LEFT(p_map_pos));
            break;
        case 7: //bottom left
            if (!map[MINESWEEPER_TILE_ABOVE(p_map_pos)].is_rev && !map[MINESWEEPER_TILE_ABOVE(p_map_pos)].is_bomb && !map[MINESWEEPER_TILE_ABOVE(p_map_pos)].is_flag)
                rev_sel_tile_recurse(MINESWEEPER_TILE_ABOVE(p_map_pos));
            if (!map[MINESWEEPER_TILE_RIGHT(p_map_pos)].is_rev && !map[MINESWEEPER_TILE_RIGHT(p_map_pos)].is_bomb && !map[MINESWEEPER_TILE_RIGHT(p_map_pos)].is_flag)
                rev_sel_tile_recurse(MINESWEEPER_TILE_RIGHT(p_map_pos)); 
            break;
        case 8: //top right
            if (!map[MINESWEEPER_TILE_DOWN(p_map_pos)].is_rev && !map[MINESWEEPER_TILE_DOWN(p_map_pos)].is_bomb && !map[MINESWEEPER_TILE_DOWN(p_map_pos)].is_flag)
                rev_sel_tile_recurse(MINESWEEPER_TILE_DOWN(p_map_pos));
            if (!map[MINESWEEPER_TILE_LEFT(p_map_pos)].is_rev && !map[MINESWEEPER_TILE_LEFT(p_map_pos)].is_bomb && !map[MINESWEEPER_TILE_LEFT(p_map_pos)].is_flag)
                rev_sel_tile_recurse(MINESWEEPER_TILE_LEFT(p_map_pos));
            break;
    }
    
}

//Returns false if alive and true if died
bool Minesweeper::rev_sel_tile()
{
    //Lose condition
    if (map[MINESWEEPER_P_TO_MAP_TRANSFER].is_bomb && !map[MINESWEEPER_P_TO_MAP_TRANSFER].is_flag)
        return true;
    
    //Begin reveal
    if (is_first_click)
        Minesweeper::gen_map();
    rev_sel_tile_recurse(MINESWEEPER_P_TO_MAP_TRANSFER);
    return false;
}
//Up = 0, Down = 1, Left = 2, Right = 3
void Minesweeper::upd_player_loc_kbd(int direction)
{
    switch(MINESWEEPER_P_TO_MAP_TRANSFER)
    {
        case 0: //none
            kbd_loc_upd_logic(direction);
            break;
        case 1: //left
            if (direction != 2)
                kbd_loc_upd_logic(direction);
            break;
        case 2: //right
            if (direction != 3)
                kbd_loc_upd_logic(direction);
            break;
        case 3: //top
            if (direction != 0)
                kbd_loc_upd_logic(direction);
            break;
        case 4: //down
            if (direction != 1)
                kbd_loc_upd_logic(direction);
            break;
        case 5: //top left
            if ((direction != 0) && (direction != 2))
                kbd_loc_upd_logic(direction);
            break;
        case 6: //bottom right
            if ((direction != 1) && (direction != 3))
                kbd_loc_upd_logic(direction);
            break;
        case 7: //bottom left
            if ((direction != 1) && (direction != 2))
                kbd_loc_upd_logic(direction);
            break;
        case 8: //top right
            if ((direction != 0) && (direction != 3))
                kbd_loc_upd_logic(direction);
            break;
    }
}

void Minesweeper::kbd_loc_upd_logic(int direction)
{
    switch (direction)
    {
        case 0: //up
            p_loc.x -= 1;
            break;
        case 1: //down
            p_loc.x += 1;
            break;
        case 2: //left
            p_loc.y -= 1;
            break;
        case 3: //right
            p_loc.y += 1;
            break;
    }
}

void Minesweeper::flag_sel_tile()
{
    uint16_t p_map_pos = MINESWEEPER_P_TO_MAP_TRANSFER;
    if (!map[p_map_pos].is_flag)
    {
        if (map[p_map_pos].is_rev)
            return;
        if (map[p_map_pos].is_bomb)
        {
            map[p_map_pos].is_flag = 1;
            current_flagged++;
        }
    } else 
    {
        if (map[p_map_pos].is_bomb)
            current_flagged--;
        map[p_map_pos].is_flag = 0;
    }
}

bool Minesweeper::did_win()
{
    if (current_flagged == mine_amount)
        return true;
    return false;
}

const std::vector<ms_tile_info>& Minesweeper::get_map()
{
    return map;
}