#include <cstdint>
#include <vector>

//Stores player location in hex digits (0 indexed)
struct ms_player_loc
{
    uint8_t x;
    uint8_t y;
};

//Store critical tile information
struct ms_tile_info
{
    uint8_t is_flag;
    uint8_t is_bomb;
    uint8_t is_rev;
    uint8_t num;
    uint16_t id;
};

#define MINESWEEPER_P_TO_MAP_TRANSFER (map_dim * p_loc.y) + p_loc.x //0 indexed
#define MINESWEEPER_TILE_LEFT(current_pos) (current_pos - 1) //0 indexed
#define MINESWEEPER_TILE_RIGHT(current_pos) (current_pos + 1) //0 indexed
#define MINESWEEPER_TILE_ABOVE(current_pos) (current_pos - map_dim) //0 indexed
#define MINESWEEPER_TILE_DOWN(current_pos) (current_pos + map_dim) //0 indexed


class Minesweeper
{
    private:
        std::vector<ms_tile_info> map; //2d map represented in linear vector
        ms_player_loc p_loc; 
        size_t map_size;
        size_t map_dim;
        size_t mine_amount;
        size_t current_flagged;
        bool is_first_click;
        void gen_map();
        int edgecase_check(uint16_t map_pos);
        void rev_sel_tile_recurse(uint16_t p_map_pos);
        void kbd_loc_upd_logic(int direction);
        //mode bool
    public:
        Minesweeper(size_t size, float density);
        void upd_player_loc_mouse(int x, int y); //0 indexed
        void upd_player_loc_kbd(int direction); //0 indexed
        bool rev_sel_tile(); //0 indexed
        void flag_sel_tile(); //0 indexed
        bool did_win();
        const std::vector<ms_tile_info>& get_map();
};