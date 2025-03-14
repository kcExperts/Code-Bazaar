#include <cstdint>
#include <vector>

//Stores player location in hex digits
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

class Minesweeper
{
    private:
        std::vector<ms_tile_info> map; //2d map represented in linear vector
        ms_player_loc p_loc; 
        size_t map_size;
        size_t map_dim;
        bool is_first_click;
        void gen_map();
        //mode bool
    public:
        Minesweeper(size_t size);
        void upd_player_loc(int x, int y);
        void rev_sel_tile();
        ~Minesweeper();
};