#include <array>
#include <cassert>
#include <ios>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <cstdint>
#include <sstream>

// definitions and globals

// Board defs
using Bitboard = unsigned long long;

enum Piece
{
    K, Q, R, B, N, P, k, q, r, b, n, p,
};

enum Square
{
    a1, b1, c1, d1, e1, f1, g1, h1,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a8, b8, c8, d8, e8, f8, g8, h8, no_sq
};

enum Colour
{
    white, black,
};

std::map<char, int> char_to_piece{
    {'K', K},
    {'Q', Q},
    {'R', R},
    {'B', B},
    {'N', N},
    {'P', P},
    {'k', k},
    {'q', q},
    {'r', r},
    {'b', b},
    {'n', n},
    {'p', p}
};

std::map<int, char> piece_to_char{
    {K, 'K'},
    {Q, 'Q'},
    {R, 'R'},
    {B, 'B'},
    {N, 'N'},
    {P, 'P'},
    {k, 'k'},
    {q, 'q'},
    {r, 'r'},
    {b, 'b'},
    {n, 'n'},
    {p, 'p'}
};

std::map<std::string, int> char_to_sq{
    {"a1", a1}, {"b1", b1}, {"c1", c1}, {"d1", d1}, {"e1", e1}, {"f1", f1}, {"g1", g1}, {"h1", h1},
    {"a2", a2}, {"b2", b2}, {"c2", c2}, {"d2", d2}, {"e2", e2}, {"f2", f2}, {"g2", g2}, {"h2", h2},
    {"a3", a3}, {"b3", b3}, {"c3", c3}, {"d3", d3}, {"e3", e3}, {"f3", f3}, {"g3", g3}, {"h3", h3},
    {"a4", a4}, {"b4", b4}, {"c4", c4}, {"d4", d4}, {"e4", e4}, {"f4", f4}, {"g4", g4}, {"h4", h4},
    {"a5", a5}, {"b5", b5}, {"c5", c5}, {"d5", d5}, {"e5", e5}, {"f5", f5}, {"g5", g5}, {"h5", h5},
    {"a6", a6}, {"b6", b6}, {"c6", c6}, {"d6", d6}, {"e6", e6}, {"f6", f6}, {"g6", g6}, {"h6", h6},
    {"a7", a7}, {"b7", b7}, {"c7", c7}, {"d7", d7}, {"e7", e7}, {"f7", f7}, {"g7", g7}, {"h7", h7},
    {"a8", a8}, {"b8", b8}, {"c8", c8}, {"d8", d8}, {"e8", e8}, {"f8", f8}, {"g8", g8}, {"h8", h8}, {"-", no_sq}
};

std::map<int, std::string> sq_to_char{
    {a1, "a1"}, {b1, "b1"}, {c1, "c1"}, {d1, "d1"}, {e1, "e1"}, {f1, "f1"}, {g1, "g1"}, {h1, "h1"},
    {a2, "a2"}, {b2, "b2"}, {c2, "c2"}, {d2, "d2"}, {e2, "e2"}, {f2, "f2"}, {g2, "g2"}, {h2, "h2"},
    {a3, "a3"}, {b3, "b3"}, {c3, "c3"}, {d3, "d3"}, {e3, "e3"}, {f3, "f3"}, {g3, "g3"}, {h3, "h3"},
    {a4, "a4"}, {b4, "b4"}, {c4, "c4"}, {d4, "d4"}, {e4, "e4"}, {f4, "f4"}, {g4, "g4"}, {h4, "h4"},
    {a5, "a5"}, {b5, "b5"}, {c5, "c5"}, {d5, "d5"}, {e5, "e5"}, {f5, "f5"}, {g5, "g5"}, {h5, "h5"},
    {a6, "a6"}, {b6, "b6"}, {c6, "c6"}, {d6, "d6"}, {e6, "e6"}, {f6, "f6"}, {g6, "g6"}, {h6, "h6"},
    {a7, "a7"}, {b7, "b7"}, {c7, "c7"}, {d7, "d7"}, {e7, "e7"}, {f7, "f7"}, {g7, "g7"}, {h7, "h7"},
    {a8, "a8"}, {b8, "b8"}, {c8, "c8"}, {d8, "d8"}, {e8, "e8"}, {f8, "f8"}, {g8, "g8"}, {h8, "h8"},
};

std::array<Bitboard, 64> ROOK_MAGICS = {
    0x8a80104000800020ULL, 0x140002000100040ULL, 0x2801880a0017001ULL, 0x100081001000420ULL, 0x200020010080420ULL,
    0x3001c0002010008ULL, 0x8480008002000100ULL, 0x2080088004402900ULL, 0x800098204000ULL, 0x2024401000200040ULL,
    0x100802000801000ULL, 0x120800800801000ULL, 0x208808088000400ULL, 0x2802200800400ULL, 0x2200800100020080ULL,
    0x801000060821100ULL, 0x80044006422000ULL, 0x100808020004000ULL, 0x12108a0010204200ULL, 0x140848010000802ULL,
    0x481828014002800ULL, 0x8094004002004100ULL, 0x4010040010010802ULL, 0x20008806104ULL, 0x100400080208000ULL,
    0x2040002120081000ULL, 0x21200680100081ULL, 0x20100080080080ULL, 0x2000a00200410ULL, 0x20080800400ULL,
    0x80088400100102ULL, 0x80004600042881ULL, 0x4040008040800020ULL, 0x440003000200801ULL, 0x4200011004500ULL,
    0x188020010100100ULL, 0x14800401802800ULL, 0x2080040080800200ULL, 0x124080204001001ULL, 0x200046502000484ULL,
    0x480400080088020ULL, 0x1000422010034000ULL, 0x30200100110040ULL, 0x100021010009ULL, 0x2002080100110004ULL,
    0x202008004008002ULL, 0x20020004010100ULL, 0x2048440040820001ULL, 0x101002200408200ULL, 0x40802000401080ULL,
    0x4008142004410100ULL, 0x2060820c0120200ULL, 0x1001004080100ULL, 0x20c020080040080ULL, 0x2935610830022400ULL,
    0x44440041009200ULL, 0x280001040802101ULL, 0x2100190040002085ULL, 0x80c0084100102001ULL, 0x4024081001000421ULL,
    0x20030a0244872ULL, 0x12001008414402ULL, 0x2006104900a0804ULL, 0x1004081002402ULL,
};

std::array<Bitboard, 64> ROOK_ATTACK_MASKS = {
    282578800148862ULL, 565157600297596ULL, 1130315200595066ULL, 2260630401190006ULL, 4521260802379886ULL,
    9042521604759646ULL, 18085043209519166ULL, 36170086419038334ULL, 282578800180736ULL, 565157600328704ULL,
    1130315200625152ULL, 2260630401218048ULL, 4521260802403840ULL, 9042521604775424ULL, 18085043209518592ULL,
    36170086419037696ULL, 282578808340736ULL, 565157608292864ULL, 1130315208328192ULL, 2260630408398848ULL,
    4521260808540160ULL, 9042521608822784ULL, 18085043209388032ULL, 36170086418907136ULL, 282580897300736ULL,
    565159647117824ULL, 1130317180306432ULL, 2260632246683648ULL, 4521262379438080ULL, 9042522644946944ULL,
    18085043175964672ULL, 36170086385483776ULL, 283115671060736ULL, 565681586307584ULL, 1130822006735872ULL,
    2261102847592448ULL, 4521664529305600ULL, 9042787892731904ULL, 18085034619584512ULL, 36170077829103616ULL,
    420017753620736ULL, 699298018886144ULL, 1260057572672512ULL, 2381576680245248ULL, 4624614895390720ULL,
    9110691325681664ULL, 18082844186263552ULL, 36167887395782656ULL, 35466950888980736ULL, 34905104758997504ULL,
    34344362452452352ULL, 33222877839362048ULL, 30979908613181440ULL, 26493970160820224ULL, 17522093256097792ULL,
    35607136465616896ULL, 9079539427579068672ULL, 8935706818303361536ULL, 8792156787827803136ULL,
    8505056726876686336ULL, 7930856604974452736ULL, 6782456361169985536ULL, 4485655873561051136ULL,
    9115426935197958144ULL,
};

std::array<int, 64> ROOK_SHIFT_BITS = {
    12, 11, 11, 11, 11, 11, 11, 12,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    12, 11, 11, 11, 11, 11, 11, 12,
};

std::array<Bitboard, 64> BISHOP_MAGICS = {
    0x40040844404084ULL, 0x2004208a004208ULL, 0x10190041080202ULL, 0x108060845042010ULL, 0x581104180800210ULL,
    0x2112080446200010ULL, 0x1080820820060210ULL, 0x3c0808410220200ULL, 0x4050404440404ULL, 0x21001420088ULL,
    0x24d0080801082102ULL, 0x1020a0a020400ULL, 0x40308200402ULL, 0x4011002100800ULL, 0x401484104104005ULL,
    0x801010402020200ULL, 0x400210c3880100ULL, 0x404022024108200ULL, 0x810018200204102ULL, 0x4002801a02003ULL,
    0x85040820080400ULL, 0x810102c808880400ULL, 0xe900410884800ULL, 0x8002020480840102ULL, 0x220200865090201ULL,
    0x2010100a02021202ULL, 0x152048408022401ULL, 0x20080002081110ULL, 0x4001001021004000ULL, 0x800040400a011002ULL,
    0xe4004081011002ULL, 0x1c004001012080ULL, 0x8004200962a00220ULL, 0x8422100208500202ULL, 0x2000402200300c08ULL,
    0x8646020080080080ULL, 0x80020a0200100808ULL, 0x2010004880111000ULL, 0x623000a080011400ULL, 0x42008c0340209202ULL,
    0x209188240001000ULL, 0x400408a884001800ULL, 0x110400a6080400ULL, 0x1840060a44020800ULL, 0x90080104000041ULL,
    0x201011000808101ULL, 0x1a2208080504f080ULL, 0x8012020600211212ULL, 0x500861011240000ULL, 0x180806108200800ULL,
    0x4000020e01040044ULL, 0x300000261044000aULL, 0x802241102020002ULL, 0x20906061210001ULL, 0x5a84841004010310ULL,
    0x4010801011c04ULL, 0xa010109502200ULL, 0x4a02012000ULL, 0x500201010098b028ULL, 0x8040002811040900ULL,
    0x28000010020204ULL, 0x6000020202d0240ULL, 0x8918844842082200ULL, 0x4010011029020020ULL,
};

std::array<Bitboard, 64> BISHOP_ATTACK_MASKS = {
    18049651735527936ULL, 70506452091904ULL, 275415828992ULL, 1075975168ULL, 38021120ULL, 8657588224ULL,
    2216338399232ULL, 567382630219776ULL, 9024825867763712ULL, 18049651735527424ULL, 70506452221952ULL,
    275449643008ULL, 9733406720ULL, 2216342585344ULL, 567382630203392ULL, 1134765260406784ULL, 4512412933816832ULL,
    9024825867633664ULL, 18049651768822272ULL, 70515108615168ULL, 2491752130560ULL, 567383701868544ULL,
    1134765256220672ULL, 2269530512441344ULL, 2256206450263040ULL, 4512412900526080ULL, 9024834391117824ULL,
    18051867805491712ULL, 637888545440768ULL, 1135039602493440ULL, 2269529440784384ULL, 4539058881568768ULL,
    1128098963916800ULL, 2256197927833600ULL, 4514594912477184ULL, 9592139778506752ULL, 19184279556981248ULL,
    2339762086609920ULL, 4538784537380864ULL, 9077569074761728ULL, 562958610993152ULL, 1125917221986304ULL,
    2814792987328512ULL, 5629586008178688ULL, 11259172008099840ULL, 22518341868716544ULL, 9007336962655232ULL,
    18014673925310464ULL, 2216338399232ULL, 4432676798464ULL, 11064376819712ULL, 22137335185408ULL, 44272556441600ULL,
    87995357200384ULL, 35253226045952ULL, 70506452091904ULL, 567382630219776ULL, 1134765260406784ULL,
    2832480465846272ULL, 5667157807464448ULL, 11333774449049600ULL, 22526811443298304ULL, 9024825867763712ULL,
    18049651735527936ULL,
};

std::array<int, 64> BISHOP_SHIFT_BITS = {
    6, 5, 5, 5, 5, 5, 5, 6,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    6, 5, 5, 5, 5, 5, 5, 6,
};

Bitboard A_FILE_MASK = 0x0101010101010101;
Bitboard H_FILE_MASK = 0x8080808080808080;
Bitboard AB_FILE_MASK = 0x303030303030303;
Bitboard GH_FILE_MASK = 0xc0c0c0c0c0c0c0c0;
Bitboard RANK_8_MASK = 0xff00000000000000;
Bitboard RANK_1_MASK = 0xff;
Bitboard RANK_2_MASK = 0xff00;
Bitboard RANK_7_MASK = 0xff000000000000;

// return if a bit is set
Bitboard get_bit(Bitboard bitboard, int square)
{
    return bitboard & (1ULL << square);
}

// set a bit to 1 at a position
void set_bit(Bitboard& bitboard, int square)
{
    bitboard |= (1ULL << square);
}

// set a bit to 0 at a position
void reset_bit(Bitboard& bitboard, int square)
{
    bitboard &= (~(1ULL << square));
}

// count the number of 1s in a bitboard
int count_bits(Bitboard bitboard)
{
    int count = 0;
    while (bitboard)
    {
        count++;
        bitboard = bitboard & (bitboard - 1);
    }

    return count;
}

// get the index of the LS1B
int get_LS1B(Bitboard bitboard)
{
    return count_bits((bitboard & -bitboard) - 1);
}

void print(Bitboard bitboard)
{
    std::cout << "     Bitboard: " << bitboard << std::endl;
    for (int rank = 8; rank > 0; rank--)
    {
        std::cout << rank << "   ";
        for (int file = 0; file < 8; file++)
        {
            int square = (rank - 1) * 8 + file;
            char c = get_bit(bitboard, square) ? 'X' : '.';
            std::cout << c << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "    a b c d e f g h" << std::endl;
}

std::vector<int> CASTLING_RIGHTS = {
    11, 15, 15, 15, 3, 15, 15, 7,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    14, 15, 15, 15, 12, 15, 15, 13
};

// Move defs
using Move = uint16_t;

Move create_move(int start_square, int target_square, int move_flags)
{
    Move move =  0;
    move |= start_square;
    move |= target_square << 6;
    move |= move_flags << 12;

    return move;
}

int get_start_square(Move move)
{
    return move & 0b111111;
}

int get_target_square(Move move)
{
    return (move >> 6) & 0b111111;
}

Move shift_to_flags(Move move)
{
    return move >> 12;
}

int is_quiet_move(Move move)
{
    return shift_to_flags(move) == 0;
}

int is_double_pawn_push_move(Move move)
{
    return shift_to_flags(move) == 1;
}

int is_kingside_castle_move(Move move)
{
    return shift_to_flags(move) == 2;
}

int is_queenside_castle_move(Move move)
{
    return shift_to_flags(move) == 3;
}

int is_capture_move(Move move)
{
    return shift_to_flags(move) == 4;
}

int is_ep_move(Move move)
{
    return shift_to_flags(move) == 5;
}

int is_knight_promo_move(Move move)
{
    return shift_to_flags(move) == 8;
}

int is_bishop_promo_move(Move move)
{
    return shift_to_flags(move) == 9;
}

int is_rook_promo_move(Move move)
{
    return shift_to_flags(move) == 10;
}

int is_queen_promo_move(Move move)
{
    return shift_to_flags(move) == 11;
}

int is_knight_capture_promo_move(Move move)
{
    return shift_to_flags(move) == 12;
}

int is_bishop_capture_promo_move(Move move)
{
    return shift_to_flags(move) == 13;
}

int is_rook_capture_promo_move(Move move)
{
    return shift_to_flags(move) == 14;
}

int is_queen_capture_promo_move(Move move)
{
    return shift_to_flags(move) == 15;
}

void print_move(Move move)
{
    auto ss = get_start_square(move);
    auto ts = get_target_square(move);
    auto x = shift_to_flags(move);
    std::cout << sq_to_char[ss] << "" << sq_to_char[ts] << "-" << x << ": ";
}

// random number generation

unsigned int RANDOM_STATE = 1804289383;

unsigned int get_random_32()
{
    auto number = RANDOM_STATE;

    // XOR shift algorithm
    number ^= number << 13;
    number ^= number >> 17;
    number ^= number << 5;

    // update RANDOM_STATE
    RANDOM_STATE = number;

    return number;
}

unsigned long long get_random_64()
{
    // generate 4 random 32-bit numbers, and get the first 16 bits
    auto n1 = static_cast<unsigned long long>(get_random_32()) & 0xffff;
    auto n2 = static_cast<unsigned long long>(get_random_32()) & 0xffff;
    auto n3 = static_cast<unsigned long long>(get_random_32()) & 0xffff;
    auto n4 = static_cast<unsigned long long>(get_random_32()) & 0xffff;

    // generate the full 64-bit number
    return n1 | (n2 << 16) | (n3 << 32) | (n4 << 48);
}

// classes

class ZobristRandoms
{
public:
    std::vector<std::vector<Bitboard> > piece_randoms;
    std::vector<Bitboard> ep_square_randoms;
    std::vector<Bitboard> castling_rights_randoms;
    std::vector<Bitboard> side_to_move_randoms;

    void init_piece_randoms()
    {
        for (int piece = K; piece <= p; piece++)
        {
            for (int square = 0; square < 64; square++)
            {
                piece_randoms[piece][square] = get_random_64();
            }
        }
    }
    void init_ep_square_randoms()
    {
        for (int square = 0; square < 65; square++)
        {
            ep_square_randoms[square] = get_random_64();
        }
    }
    void init_castling_rights_randoms()
    {
        for (int right = 0; right < 16; right++)
        {
            castling_rights_randoms[right] = get_random_64();
        }
    }
    void init_side_to_move_randoms()
    {
        for (int side = white; side <= black; side++)
        {
            side_to_move_randoms[side] = get_random_64();
        }
    }

    ZobristRandoms() : piece_randoms(12, std::vector<Bitboard>(64)), ep_square_randoms(65),
                                   castling_rights_randoms(16), side_to_move_randoms(2)
    {
        init_piece_randoms();
        init_ep_square_randoms();
        init_castling_rights_randoms();
        init_side_to_move_randoms();
    }

};

struct GameState
{
    int side_to_move;
    int castling_rights;
    int ep_square;
    int halfmove_clock;
    int fullmove_counter;
    Bitboard zobrist_key;
    Move next_move;
};

class Board
{
public:
    std::vector<Bitboard> piece_bitboards;
    Bitboard white_occupancy;
    Bitboard black_occupancy;
    Bitboard occupied_squares;
    std::vector<int> piece_list;

    GameState game_state;
    std::vector<GameState> history;

    ZobristRandoms zobrist_randoms;

    std::vector<std::vector<Bitboard> > PAWN_ATTACK_TABLE;
    std::vector<Bitboard> KNIGHT_ATTACK_TABLE;
    std::vector<Bitboard> KING_ATTACK_TABLE;
    std::vector<std::vector<Bitboard> > ROOK_ATTACK_TABLE;
    std::vector<std::vector<Bitboard> > BISHOP_ATTACK_TABLE;

    void print_piece_list()
    {
        for (int rank = 8; rank > 0; rank--)
        {
            for (int file = 0; file < 8; file++)
            {
                int square = (rank - 1) * 8 + file;
                int temp = piece_list[square];
                char x;
                if (temp == 12)
                {
                    x = '.';
                }
                else
                {
                    x = piece_to_char[temp];
                }
                std::cout << x << ' ';;
            }
            std::cout << std::endl;
        }
    }
    Bitboard generate_king_attack_from_square(int square)
    {
        Bitboard attack_map = 0;
        Bitboard bitboard = 0;
        set_bit(bitboard, square);

        // north
        if (bitboard << 8) attack_map |= bitboard << 8;
        // north east
        if (bitboard << 9 & ~A_FILE_MASK) attack_map |= bitboard << 9;
        // east
        if (bitboard << 1 & ~A_FILE_MASK) attack_map |= bitboard << 1;
        // south east
        if (bitboard >> 7 & ~A_FILE_MASK) attack_map |= bitboard >> 7;
        // south
        if (bitboard >> 8) attack_map |= bitboard >> 8;
        // south west
        if (bitboard >> 9 & ~H_FILE_MASK) attack_map |= bitboard >> 9;
        // west
        if (bitboard >> 1 & ~H_FILE_MASK) attack_map |= bitboard >> 1;
        // north west
        if (bitboard << 7 & ~H_FILE_MASK) attack_map |= bitboard << 7;

        return attack_map;
    }

    Bitboard generate_knight_attack_from_square(int square)
    {
        Bitboard attack_map = 0;
        Bitboard bitboard = 0;
        set_bit(bitboard, square);

        // NNE
        if (bitboard << 17 & ~A_FILE_MASK) attack_map |= bitboard << 17;
        // NEE
        if (bitboard << 10 & ~AB_FILE_MASK) attack_map |= bitboard << 10;
        // SEE
        if (bitboard >> 6 & ~AB_FILE_MASK) attack_map |= bitboard >> 6;
        // SSE
        if (bitboard >> 15 & ~A_FILE_MASK) attack_map |= bitboard >> 15;
        // SSW
        if (bitboard >> 17 & ~H_FILE_MASK) attack_map |= bitboard >> 17;
        // SWW
        if (bitboard >> 10 & ~GH_FILE_MASK) attack_map |= bitboard >> 10;
        // NWW
        if (bitboard << 6 & ~GH_FILE_MASK) attack_map |= bitboard << 6;
        // NNW
        if (bitboard << 15 & ~H_FILE_MASK) attack_map |= bitboard << 15;

        return attack_map;
    }

    Bitboard generate_pawn_attack_from_square(int square, int colour)
    {
        Bitboard attack_map = 0;
        Bitboard bitboard = 0;
        set_bit(bitboard, square);

        if (colour == white)
        {
            if (bitboard << 7 & ~H_FILE_MASK) attack_map |= bitboard << 7;
            if (bitboard << 9 & ~A_FILE_MASK) attack_map |= bitboard << 9;
        }
        else
        {
            if (bitboard >> 7 & ~A_FILE_MASK) attack_map |= bitboard >> 7;
            if (bitboard >> 9 & ~H_FILE_MASK) attack_map |= bitboard >> 9;
        }

        return attack_map;
    }

    void initialise_leaper_piece_attack_tables()
    {
        for (int square = 0; square < 64; square++)
        {
            // generate king attack
            KING_ATTACK_TABLE[square] = generate_king_attack_from_square(square);

            // generate knight attack
            KNIGHT_ATTACK_TABLE[square] = generate_knight_attack_from_square(square);

            // generate pawn attacks
            PAWN_ATTACK_TABLE[white][square] = generate_pawn_attack_from_square(square, white);
            PAWN_ATTACK_TABLE[black][square] = generate_pawn_attack_from_square(square, black);
        }
    }

    Bitboard generate_blocker_board(int index, int num_of_blockers, Bitboard attack_mask)
    {
        Bitboard blocker_board = 0;
        for (int bit = 0; bit < num_of_blockers; bit++)
        {
            int square = get_LS1B(attack_mask);
            attack_mask &= attack_mask - 1;

            if (index & (1 << bit)) blocker_board |= 1ULL << square;
        }

        return blocker_board;
    }

    Bitboard generate_rook_attack_map_with_blockers(int square, Bitboard blocker_board)
    {
        Bitboard attack_map = 0;
        int rank =  square / 8;
        int file = square % 8;
        int newSquare;

        // east
        for (int newFile = file + 1; newFile < 8; newFile++)
        {
            newSquare = rank * 8 + newFile;
            set_bit(attack_map, newSquare);
            if (blocker_board & (1ULL << newSquare)) break;
        }

        // west
        for (int newFile = file - 1; newFile >= 0; newFile--)
        {
            newSquare = rank * 8 + newFile;
            set_bit(attack_map, newSquare);
            if (blocker_board & (1ULL << newSquare)) break;
        }

        // north
        for (int newRank = rank + 1; newRank < 8; newRank++)
        {
            newSquare = newRank * 8 + file;
            set_bit(attack_map, newSquare);
            if (blocker_board & (1ULL << newSquare)) break;
        }

        // south
        for (int newRank = rank - 1; newRank >= 0; newRank--)
        {
            newSquare = newRank * 8 + file;
            set_bit(attack_map, newSquare);
            if (blocker_board & (1ULL << newSquare)) break;
        }

        return attack_map;
    }

    void initialise_rook_attack_table()
    {
        for (int square = 0; square < 64; square++)
        {
            auto mask = ROOK_ATTACK_MASKS[square];
            auto shift_bits = ROOK_SHIFT_BITS[square];
            int num_of_blocker_boards = 1 << shift_bits;

            // generate blocker board and populate table
            for (int i = 0; i < num_of_blocker_boards; i++)
            {
                Bitboard blocker_board = generate_blocker_board(i, shift_bits, mask);
                int magic_index = (blocker_board * ROOK_MAGICS[square]) >> (64 - shift_bits);
                ROOK_ATTACK_TABLE[square][magic_index] = generate_rook_attack_map_with_blockers(square, blocker_board);
            }
        }
    }

    Bitboard generate_bishop_attack_map_with_blockers(int square, Bitboard blocker_board)
    {
        Bitboard attack_map = 0;
        int rank =  square / 8;
        int file = square % 8;
        int newSquare;

        // NE
        for (int newFile = file + 1, newRank = rank + 1; newFile < 8 && newRank < 8; newFile++, newRank++)
        {
            newSquare = newRank * 8 + newFile;
            set_bit(attack_map, newSquare);
            if (blocker_board & (1ULL << newSquare)) break;
        }

        // SE
        for (int newFile = file + 1, newRank = rank - 1; newFile < 8 && newRank >= 0; newFile++, newRank--)
        {
            newSquare = newRank * 8 + newFile;
            set_bit(attack_map, newSquare);
            if (blocker_board & (1ULL << newSquare)) break;
        }

        // SW
        for (int newFile = file - 1, newRank = rank - 1; newFile >= 0 && newRank >= 0; newFile--, newRank--)
        {
            newSquare = newRank * 8 + newFile;
            set_bit(attack_map, newSquare);
            if (blocker_board & (1ULL << newSquare)) break;
        }

        // NW
        for (int newFile = file - 1, newRank = rank + 1; newFile >= 0 && newRank < 8; newFile--, newRank++)
        {
            newSquare = newRank * 8 + newFile;
            set_bit(attack_map, newSquare);
            if (blocker_board & (1ULL << newSquare)) break;
        }

        return attack_map;
    }

    void initialise_bishop_attack_table()
    {
        for (int square = 0; square < 64; square++)
        {
            auto mask = BISHOP_ATTACK_MASKS[square];
            auto shift_bits = BISHOP_SHIFT_BITS[square];
            int num_of_blocker_boards = 1 << shift_bits;

            // generate blocker board and populate table
            for (int i = 0; i < num_of_blocker_boards; i++)
            {
                Bitboard blocker_board = generate_blocker_board(i, shift_bits, mask);
                int magic_index = (blocker_board * BISHOP_MAGICS[square]) >> (64 - shift_bits);
                BISHOP_ATTACK_TABLE[square][magic_index] = generate_bishop_attack_map_with_blockers(square, blocker_board);
            }
        }
    }

    Board() : piece_bitboards(12), white_occupancy(0), black_occupancy(0), occupied_squares(0), piece_list(64),
              game_state(GameState()), zobrist_randoms(ZobristRandoms()),
              PAWN_ATTACK_TABLE(2, std::vector<Bitboard>(64)), KNIGHT_ATTACK_TABLE(64), KING_ATTACK_TABLE(64),
              ROOK_ATTACK_TABLE(64, std::vector<Bitboard>(4096)), BISHOP_ATTACK_TABLE(64, std::vector<Bitboard>(512))
    {
        initialise_leaper_piece_attack_tables();
        initialise_rook_attack_table();
        initialise_bishop_attack_table();
    }

    void parse_FEN(std::string FEN_string)
    {
        std::fill(piece_list.begin(), piece_list.end(), 12);
        // split string by space
        std::string s;
        std::stringstream ss(FEN_string);
        std::vector<std::string> fields;

        while (getline(ss, s, ' '))
        {
            fields.push_back(s);
        }

        // parse board state
        std::vector<std::string> ranks;
        ss.clear();
        ss.str(fields[0]);

        while (getline(ss, s, '/'))
        {
            ranks.push_back(s);
        }
        int rank_index = 7;
        for (auto rank: ranks)
        {
            int file_index = 0;
            for (auto c: rank)
            {
                if (std::isdigit(c))
                {
                    int num = c - '0';
                    file_index = file_index + num;
                }
                else
                {
                    int piece = char_to_piece[c];
                    int square = (rank_index * 8) + file_index;

                    // set piece in piece_list
                    piece_list[square] = piece;

                    // set piece on bitboard
                    set_bit(piece_bitboards[piece], square);

                    // update zobrist key
                    game_state.zobrist_key ^= zobrist_randoms.piece_randoms[piece][square];
                    file_index++;
                }
            }
            rank_index--;
        }
        // set white occupancy
        for (int i = K; i <= P; i++)
        {
            white_occupancy |= piece_bitboards[i];
        }

        // set black occupancy
        for (int i = k; i <= p; i++)
        {
            black_occupancy |= piece_bitboards[i];
        }

        // set occupied squares
        occupied_squares |= white_occupancy;
        occupied_squares |= black_occupancy;

        // parse side to move
        game_state.side_to_move = fields[1] == "w" ? 0 : 1;
        game_state.zobrist_key ^= zobrist_randoms.side_to_move_randoms[game_state.side_to_move];

        // parse castling rights
        if (fields[2] == "-")
        {
            game_state.castling_rights = 0;
        }
        else
        {
            for (auto c: fields[2])
            {
                if (c == 'K') game_state.castling_rights |= (1 << 3);
                if (c == 'Q') game_state.castling_rights |= (1 << 2);
                if (c == 'k') game_state.castling_rights |= (1 << 1);
                if (c == 'q') game_state.castling_rights |= 1;
            }
        }
        game_state.zobrist_key ^= zobrist_randoms.castling_rights_randoms[game_state.castling_rights];

        // parse en passant square
        game_state.ep_square = char_to_sq[fields[3]];
        game_state.zobrist_key ^= zobrist_randoms.ep_square_randoms[game_state.ep_square];

        // parse halfmove clock
        game_state.halfmove_clock = std::stoi(fields[4]);

        // parse fullmove counter
        game_state.fullmove_counter = std::stoi(fields[5]);
    }

    void put_piece(int piece, int square)
    {
        // set piece on bitboard
        set_bit(piece_bitboards[piece], square);

        // update zobrist key
        game_state.zobrist_key ^= zobrist_randoms.piece_randoms[piece][square];

        // update piece list
        piece_list[square] = piece;

        // update occupancy
        if (piece <= 5)
        {
            set_bit(white_occupancy, square);
        }
        else
        {
            set_bit(black_occupancy, square);
        }
        set_bit(occupied_squares, square);

        // update evaluation TODO
    }

    void remove_piece(int piece, int square)
    {
        // remove piece on bitboard
        reset_bit(piece_bitboards[piece], square);

        // update zobrist key
        game_state.zobrist_key ^= zobrist_randoms.piece_randoms[piece][square];

        // update piece list
        piece_list[square] = 12;

        // update occupancy
        if (piece <= 5)
        {
            reset_bit(white_occupancy, square);
        }
        else
        {
            reset_bit(black_occupancy, square);
        }
        reset_bit(occupied_squares, square);

        // update evaluation TODO
    }
    void move_piece(int piece, int start_square, int target_square)
    {
        // remove piece
        remove_piece(piece, start_square);

        // put piece
        put_piece(piece, target_square);
    }

    void set_ep_square(int square)
    {
        // remove current ep from zobrist
        game_state.zobrist_key ^= zobrist_randoms.ep_square_randoms[game_state.ep_square];

        // set eq square
        game_state.ep_square = square;

        // add new ep to zobrist
        game_state.zobrist_key ^= zobrist_randoms.ep_square_randoms[game_state.ep_square];
    }

    void swap_side()
    {
        // remove from zobrist
        game_state.zobrist_key ^= zobrist_randoms.side_to_move_randoms[game_state.side_to_move];

        // change side
        game_state.side_to_move ^= 1;

        // update zobrist
        game_state.zobrist_key ^= zobrist_randoms.side_to_move_randoms[game_state.side_to_move];
    }

    void update_castling_rights(int start_square, int target_square)
    {
        // remove from zobrist
        game_state.zobrist_key ^= zobrist_randoms.castling_rights_randoms[game_state.castling_rights];

        // update castling rights
        game_state.castling_rights &= CASTLING_RIGHTS[start_square];
        game_state.castling_rights &= CASTLING_RIGHTS[target_square];

        // update zobrist
        game_state.zobrist_key ^= zobrist_randoms.castling_rights_randoms[game_state.castling_rights];
    }

    Bitboard generate_rook_attack_from_square(int square, Bitboard occupancy)
    {
        auto mask = ROOK_ATTACK_MASKS[square];
        mask &= occupancy;
        int magic_index = (mask * ROOK_MAGICS[square]) >> (64 - ROOK_SHIFT_BITS[square]);
        return ROOK_ATTACK_TABLE[square][magic_index];
    }

    Bitboard generate_bishop_attack_from_square(int square, Bitboard occupancy)
    {
        auto mask = BISHOP_ATTACK_MASKS[square];
        mask &= occupancy;
        int magic_index = (mask * BISHOP_MAGICS[square]) >> (64 - BISHOP_SHIFT_BITS[square]);
        return BISHOP_ATTACK_TABLE[square][magic_index];
    }

    Bitboard generate_queen_attack_from_square(int square, Bitboard occupancy)
    {
        auto mask = ROOK_ATTACK_MASKS[square];
        mask &= occupancy;
        int magic_index = (mask * ROOK_MAGICS[square]) >> (64 - ROOK_SHIFT_BITS[square]);
        auto queen_attack = ROOK_ATTACK_TABLE[square][magic_index];

        mask = BISHOP_ATTACK_MASKS[square];
        mask &= occupancy;
        magic_index = (mask * BISHOP_MAGICS[square]) >> (64 - BISHOP_SHIFT_BITS[square]);
        return queen_attack | BISHOP_ATTACK_TABLE[square][magic_index];
    }

    bool is_square_attacked_by_colour(int square, int colour)
    {
        Bitboard attack_squares;
        int side = colour == white ? 0 : 6;
        // attacked by rook
        attack_squares = generate_rook_attack_from_square(square, occupied_squares);
        if (attack_squares & piece_bitboards[R + side]) return true;

        // attacked by bishop
        attack_squares = generate_bishop_attack_from_square(square, occupied_squares);
        if (attack_squares & piece_bitboards[B + side]) return true;

        // attacked by queen
        attack_squares = generate_queen_attack_from_square(square, occupied_squares);
        if (attack_squares & piece_bitboards[Q + side]) return true;

        // attacked by knight
        attack_squares = KNIGHT_ATTACK_TABLE[square];
        if (attack_squares & piece_bitboards[N + side]) return true;

        // attacked by pawn
        attack_squares = PAWN_ATTACK_TABLE[colour ^ 1][square];
        if (attack_squares & piece_bitboards[P + side]) return true;

        // attacked by king
        attack_squares = KING_ATTACK_TABLE[square];
        if (attack_squares & piece_bitboards[K + side]) return true;

        return false;
    }

    bool make_move(Move move)
    {
        // copy history
        // auto current_game_state = game_state;
        // current_game_state.next_move = move;
        // history.push_back(current_game_state);
        auto copy_piece_bitboards = piece_bitboards;
        auto copy_white_occupancy = white_occupancy;
        auto copy_black_occupancy = black_occupancy;
        auto copy_occupied_squares = occupied_squares;
        auto copy_piece_list = piece_list;
        auto copy_game_state = game_state;

        // get squares
        int start_square = get_start_square(move);
        int target_square = get_target_square(move);
        int piece = piece_list[start_square];
        int side = game_state.side_to_move == white ? 0 : 6;

        // flags
        int is_quiet = is_quiet_move(move);
        int is_double_pawn_push = is_double_pawn_push_move(move);
        int is_kingside_castle = is_kingside_castle_move(move);
        int is_queenside_castle = is_queenside_castle_move(move);
        int is_capture = is_capture_move(move);
        int is_ep = is_ep_move(move);
        int is_knight_promo = is_knight_promo_move(move);
        int is_bishop_promo = is_bishop_promo_move(move);
        int is_rook_promo = is_rook_promo_move(move);
        int is_queen_promo = is_queen_promo_move(move);
        int is_knight_capture_promo = is_knight_capture_promo_move(move);
        int is_bishop_capture_promo = is_bishop_capture_promo_move(move);
        int is_rook_capture_promo = is_rook_capture_promo_move(move);
        int is_queen_capture_promo = is_queen_capture_promo_move(move);

        // assume move increments halfmove counter
        game_state.halfmove_clock++;

        // reset ep square
        set_ep_square(64);

        if (is_quiet)
        {
            move_piece(piece, start_square, target_square);
            if (piece == P || piece == p)
            {
                game_state.halfmove_clock = 0;
            }
        }

        if (is_double_pawn_push)
        {
            move_piece(piece, start_square, target_square);
            game_state.halfmove_clock = 0;
            int offset = game_state.side_to_move == white ? -1 : 1;
            set_ep_square(target_square + (offset * 8));
        }

        if (is_kingside_castle)
        {
            move_piece(piece, start_square, target_square);
            switch (target_square)
            {
                case g1:
                    move_piece(R, h1, f1);
                    break;
                case g8:
                    move_piece(r, h8, f8);
                    break;
            }
        }

        if (is_queenside_castle)
        {
            move_piece(piece, start_square, target_square);
            switch (target_square)
            {
            case c1:
                move_piece(R, a1, d1);
                break;
            case c8:
                move_piece(r, a8, d8);
                break;
            }
        }

        if (is_capture)
        {
            // std::cout << "we are in correct branch" << std::endl;
            // std::cout << piece << std::endl;
            int target_piece = piece_list[target_square];
            // std::cout << target_piece << std::endl;
            remove_piece(target_piece, target_square);
            move_piece(piece, start_square, target_square);
            game_state.halfmove_clock = 0;
        }

        if (is_ep)
        {
            move_piece(piece, start_square, target_square);
            int offset = game_state.side_to_move == white ? -8 : 8;
            int target_piece = game_state.side_to_move == white ? p : P;
            remove_piece(target_piece, target_square + offset);
            game_state.halfmove_clock = 0;
        }

        if (is_knight_promo)
        {
            remove_piece(piece, start_square);
            put_piece(N + side, target_square);
            game_state.halfmove_clock = 0;
        }

        if (is_bishop_promo)
        {
            remove_piece(piece, start_square);
            put_piece(B + side, target_square);
            game_state.halfmove_clock = 0;
        }

        if (is_rook_promo)
        {
            remove_piece(piece, start_square);
            put_piece(R + side, target_square);
            game_state.halfmove_clock = 0;
        }

        if (is_queen_promo)
        {
            remove_piece(piece, start_square);
            put_piece(Q + side, target_square);
            game_state.halfmove_clock = 0;
        }

        if (is_knight_capture_promo)
        {
            int target_piece = piece_list[target_square];
            remove_piece(piece, start_square);
            remove_piece(target_piece, target_square);
            put_piece(N + side, target_square);
            game_state.halfmove_clock = 0;
        }

        if (is_bishop_capture_promo)
        {
            int target_piece = piece_list[target_square];
            remove_piece(piece, start_square);
            remove_piece(target_piece, target_square);
            put_piece(B + side, target_square);
            game_state.halfmove_clock = 0;
        }

        if (is_rook_capture_promo)
        {
            int target_piece = piece_list[target_square];
            remove_piece(piece, start_square);
            remove_piece(target_piece, target_square);
            put_piece(R + side, target_square);
            game_state.halfmove_clock = 0;
        }

        if (is_queen_capture_promo)
        {
            int target_piece = piece_list[target_square];
            remove_piece(piece, start_square);
            remove_piece(target_piece, target_square);
            put_piece(Q + side, target_square);
            game_state.halfmove_clock = 0;
        }

        // update castling rights
        update_castling_rights(start_square, target_square);

        // increment fullmove counter
        if (game_state.side_to_move == black) game_state.fullmove_counter++;

        // swap side
        swap_side();

        // check if move is legal
        int king = game_state.side_to_move == white ? k : K;
        int k_square = get_LS1B(piece_bitboards[king]);
        if (is_square_attacked_by_colour(k_square, game_state.side_to_move))
        {
            piece_bitboards = copy_piece_bitboards;
            white_occupancy = copy_white_occupancy;
            black_occupancy = copy_black_occupancy;
            occupied_squares = copy_occupied_squares;
            piece_list = copy_piece_list;
            game_state = copy_game_state;

            return false;
        }

        // std::cout << "after the move:" << std::endl;
        // print_move(move);
        // std::cout << std::endl;
        // print_piece_list();

        return true;
    }

    void generate_piece_pseudolegal_moves(std::vector<Move> &move_list, int &move_count, int piece)
    {
        int side = game_state.side_to_move == white ? 0 : 6;
        Bitboard& player_bitboard = game_state.side_to_move == white ? white_occupancy : black_occupancy;
        Bitboard& opponent_bitboard = game_state.side_to_move == white ? black_occupancy : white_occupancy;

        Bitboard bitboard = piece_bitboards[piece + side];
        while (bitboard)
        {
            // get start square
            int start_square = get_LS1B(bitboard);
            Bitboard attack_squares;
            switch (piece)
            {
                case K:
                    attack_squares = KING_ATTACK_TABLE[start_square];
                    break;
                case Q:
                    attack_squares = generate_queen_attack_from_square(start_square, occupied_squares);
                    break;
                case R:
                    attack_squares = generate_rook_attack_from_square(start_square, occupied_squares);
                    break;
                case B:
                    attack_squares = generate_bishop_attack_from_square(start_square, occupied_squares);
                    break;
                case N:
                    attack_squares = KNIGHT_ATTACK_TABLE[start_square];
                    break;
            }
            attack_squares &= ~player_bitboard;
            while (attack_squares)
            {
                // get target square
                int target_square = get_LS1B(attack_squares);

                // capture
                if (get_bit(opponent_bitboard, target_square))
                {
                    move_list[move_count] = create_move(start_square, target_square, 0b0100);
                }
                // quiet
                else
                {
                    move_list[move_count] = create_move(start_square, target_square, 0b0);
                }
                move_count++;

                attack_squares &= attack_squares - 1;
            }

            // pop current bit
            bitboard &= bitboard - 1;
        }

    }

    void generate_castling_pseudolegal_moves(std::vector<Move> &move_list, int &move_count)
    {
        int side = game_state.side_to_move == white ? 0 : 6;
        int opponent = 1 ^ game_state.side_to_move;
        auto rights = game_state.side_to_move == white ? game_state.castling_rights >> 2 : game_state.castling_rights;

        Bitboard bitboard = piece_bitboards[K + side];
        while (bitboard)
        {
            int start_square = get_LS1B(bitboard);
            // kingside
            if (rights & 0b10)
            {
                if (!is_square_attacked_by_colour(start_square, opponent) &&
                    !is_square_attacked_by_colour(start_square + 1, opponent) &&
                    !get_bit(occupied_squares, start_square + 1) &&
                    !get_bit(occupied_squares, start_square + 2))
                {
                    move_list[move_count] = create_move(start_square, start_square + 2, 0b10);
                    move_count++;
                }
            }
            // queenside
            if (rights & 0b1)
            {
                if (!is_square_attacked_by_colour(start_square, opponent) &&
                    !is_square_attacked_by_colour(start_square - 1, opponent) &&
                    !get_bit(occupied_squares, start_square - 1) &&
                    !get_bit(occupied_squares, start_square - 2) &&
                    !get_bit(occupied_squares, start_square - 3))
                {
                    move_list[move_count] = create_move(start_square, start_square - 2, 0b11);
                    move_count++;
                }
            }

            bitboard &= bitboard - 1;
        }
    }

    void generate_pawn_pseudolegal_moves(std::vector<Move>& move_list, int& move_count)
    {
        int side = game_state.side_to_move == white ? 0 : 6;
        int offset = game_state.side_to_move == white ? 1 : -1;
        Bitboard& opponent_bitboard = game_state.side_to_move == white ? black_occupancy : white_occupancy;
        Bitboard promotion_mask = game_state.side_to_move == white ? RANK_8_MASK : RANK_1_MASK;
        Bitboard double_push_mask = game_state.side_to_move == white ? RANK_2_MASK : RANK_7_MASK;

        Bitboard bitboard = piece_bitboards[P + side];

        while (bitboard)
        {
            // get start square
            int start_square = get_LS1B(bitboard);
            Bitboard attack_squares = PAWN_ATTACK_TABLE[game_state.side_to_move][start_square];

            while (attack_squares)
            {
                int target_square = get_LS1B(attack_squares);
                if (get_bit(opponent_bitboard, target_square))
                {
                    // promotion captures
                    if (get_bit(promotion_mask, target_square))
                    {
                        move_list[move_count] = create_move(start_square, target_square, 0b1100);
                        move_count++;
                        move_list[move_count] = create_move(start_square, target_square, 0b1101);
                        move_count++;
                        move_list[move_count] = create_move(start_square, target_square, 0b1110);
                        move_count++;
                        move_list[move_count] = create_move(start_square, target_square, 0b1111);
                        move_count++;
                    }

                    // regular capture
                    else
                    {
                        move_list[move_count] = create_move(start_square, target_square, 0b100);
                        move_count++;
                    }
                }

                // en-passant capture
                if (target_square == game_state.ep_square)
                {
                    move_list[move_count] = create_move(start_square, game_state.ep_square, 0b101);
                    move_count++;
                }
                attack_squares &= attack_squares - 1;
            }

            int single_push = start_square + (offset * 8);
            int double_push = start_square + (offset * 16);

            // double push
            if (get_bit(double_push_mask, start_square))
            {
                if (!get_bit(occupied_squares, single_push) &&
                    !get_bit(occupied_squares, double_push))
                {
                    move_list[move_count] = create_move(start_square, double_push, 0b1);
                    move_count++;
                }
            }

            if (!get_bit(occupied_squares, single_push))
            {
                // single push promotion
                if (get_bit(promotion_mask, single_push))
                {
                    move_list[move_count] = create_move(start_square, single_push, 0b1000);
                    move_count++;
                    move_list[move_count] = create_move(start_square, single_push, 0b1001);
                    move_count++;
                    move_list[move_count] = create_move(start_square, single_push, 0b1010);
                    move_count++;
                    move_list[move_count] = create_move(start_square, single_push, 0b1011);
                    move_count++;
                }

                // single push
                else
                {
                    move_list[move_count] = create_move(start_square, single_push, 0b0);
                    move_count++;
                }
            }

            // pop first bit for next pawn
            bitboard &= bitboard - 1;
        }
    }

    std::pair<std::vector<Move>, int> generate_all_pseudolegal_moves()
    {
        // initialise return variables
        std::vector<Move> move_list(256);
        int move_count = 0;

        // generate rook moves
        generate_piece_pseudolegal_moves(move_list, move_count, R);

        // generate bishop moves
        generate_piece_pseudolegal_moves(move_list, move_count, B);

        // generate queen moves
        generate_piece_pseudolegal_moves(move_list, move_count, Q);

        // generate knight moves
        generate_piece_pseudolegal_moves(move_list, move_count, N);

        // generate king moves
        generate_piece_pseudolegal_moves(move_list, move_count, K);

        // castling
        generate_castling_pseudolegal_moves(move_list, move_count);

        // generate pawn moves
        generate_pawn_pseudolegal_moves(move_list, move_count);

        return std::make_pair(move_list, move_count);
    }
};

class PERFT
{
public:
    Board board;
    int nodes;

    PERFT() : nodes(0), board(Board())
    {

    }

    void perft_driver(int depth)
    {
        if (depth == 0)
        {
            nodes++;
            return;
        }

        auto moves = board.generate_all_pseudolegal_moves();
        for (int i = 0; i < moves.second; i++)
        {
            auto copy_piece_bitboards = board.piece_bitboards;
            auto copy_white_occupancy = board.white_occupancy;
            auto copy_black_occupancy = board.black_occupancy;
            auto copy_occupied_squares = board.occupied_squares;
            auto copy_piece_list = board.piece_list;
            auto copy_game_state = board.game_state;

            if (!board.make_move(moves.first[i])) continue;
            perft_driver(depth - 1);

            board.piece_bitboards = copy_piece_bitboards;
            board.white_occupancy = copy_white_occupancy;
            board.black_occupancy = copy_black_occupancy;
            board.occupied_squares = copy_occupied_squares;
            board.piece_list = copy_piece_list;
            board.game_state = copy_game_state;
        }
    }

    void perft_test_driver(int depth)
    {
        if (depth == 0)
        {
            nodes++;
            return;
        }

        auto moves = board.generate_all_pseudolegal_moves();
        for (int i = 0; i < moves.second; i++)
        {
            auto copy_piece_bitboards = board.piece_bitboards;
            auto copy_white_occupancy = board.white_occupancy;
            auto copy_black_occupancy = board.black_occupancy;
            auto copy_occupied_squares = board.occupied_squares;
            auto copy_piece_list = board.piece_list;
            auto copy_game_state = board.game_state;

            if (!board.make_move(moves.first[i])) continue;

            int curr_nodes = nodes;
            perft_driver(depth - 1);
            int diff = nodes - curr_nodes;

            board.piece_bitboards = copy_piece_bitboards;
            board.white_occupancy = copy_white_occupancy;
            board.black_occupancy = copy_black_occupancy;
            board.occupied_squares = copy_occupied_squares;
            board.piece_list = copy_piece_list;
            board.game_state = copy_game_state;

            print_move(moves.first[i]);
            std::cout << diff << std::endl;
        }
    }
};

int main()
{

    auto perft = PERFT();
    perft.board.parse_FEN("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10");
    perft.perft_driver(5);
    assert(perft.nodes == 164075551);
    std::cout << perft.nodes << std::endl;
    // auto board = Board();
    // board.parse_FEN("8/2p5/3p4/KP5r/5p1k/8/4P1P1/1R6 b - - 0 1");
    // auto move = create_move(h5, b5, 0b100);
    // board.make_move(move);
    // board.print_piece_list();
    // std::cout << "----------------------" << std::endl;

    // board.make_move(create_move(h5, b5, 4));
    // board.print_piece_list();

}