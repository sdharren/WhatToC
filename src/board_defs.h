#ifndef BOARD_DEFS_H
#define BOARD_DEFS_H
#include <cstdint>
#include <map>
#include <string>

// Our standardised Bitboard type
using Bitboard = uint64_t;

// Board enumerations
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

// Enum maps
inline const std::map<char, int> char_to_piece{
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

inline const std::map<int, char> piece_to_char{
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

inline const std::map<std::string, int> char_to_sq{
    {"a1", a1}, {"b1", b1}, {"c1", c1}, {"d1", d1}, {"e1", e1}, {"f1", f1}, {"g1", g1}, {"h1", h1},
    {"a2", a2}, {"b2", b2}, {"c2", c2}, {"d2", d2}, {"e2", e2}, {"f2", f2}, {"g2", g2}, {"h2", h2},
    {"a3", a3}, {"b3", b3}, {"c3", c3}, {"d3", d3}, {"e3", e3}, {"f3", f3}, {"g3", g3}, {"h3", h3},
    {"a4", a4}, {"b4", b4}, {"c4", c4}, {"d4", d4}, {"e4", e4}, {"f4", f4}, {"g4", g4}, {"h4", h4},
    {"a5", a5}, {"b5", b5}, {"c5", c5}, {"d5", d5}, {"e5", e5}, {"f5", f5}, {"g5", g5}, {"h5", h5},
    {"a6", a6}, {"b6", b6}, {"c6", c6}, {"d6", d6}, {"e6", e6}, {"f6", f6}, {"g6", g6}, {"h6", h6},
    {"a7", a7}, {"b7", b7}, {"c7", c7}, {"d7", d7}, {"e7", e7}, {"f7", f7}, {"g7", g7}, {"h7", h7},
    {"a8", a8}, {"b8", b8}, {"c8", c8}, {"d8", d8}, {"e8", e8}, {"f8", f8}, {"g8", g8}, {"h8", h8}, {"-", no_sq}
};

inline const std::map<int, std::string> sq_to_char{
    {a1, "a1"}, {b1, "b1"}, {c1, "c1"}, {d1, "d1"}, {e1, "e1"}, {f1, "f1"}, {g1, "g1"}, {h1, "h1"},
    {a2, "a2"}, {b2, "b2"}, {c2, "c2"}, {d2, "d2"}, {e2, "e2"}, {f2, "f2"}, {g2, "g2"}, {h2, "h2"},
    {a3, "a3"}, {b3, "b3"}, {c3, "c3"}, {d3, "d3"}, {e3, "e3"}, {f3, "f3"}, {g3, "g3"}, {h3, "h3"},
    {a4, "a4"}, {b4, "b4"}, {c4, "c4"}, {d4, "d4"}, {e4, "e4"}, {f4, "f4"}, {g4, "g4"}, {h4, "h4"},
    {a5, "a5"}, {b5, "b5"}, {c5, "c5"}, {d5, "d5"}, {e5, "e5"}, {f5, "f5"}, {g5, "g5"}, {h5, "h5"},
    {a6, "a6"}, {b6, "b6"}, {c6, "c6"}, {d6, "d6"}, {e6, "e6"}, {f6, "f6"}, {g6, "g6"}, {h6, "h6"},
    {a7, "a7"}, {b7, "b7"}, {c7, "c7"}, {d7, "d7"}, {e7, "e7"}, {f7, "f7"}, {g7, "g7"}, {h7, "h7"},
    {a8, "a8"}, {b8, "b8"}, {c8, "c8"}, {d8, "d8"}, {e8, "e8"}, {f8, "f8"}, {g8, "g8"}, {h8, "h8"},
};

// Board masks, for files and ranks
inline constexpr Bitboard A_FILE_MASK = 0x0101010101010101;
inline constexpr Bitboard H_FILE_MASK = 0x8080808080808080;
inline constexpr Bitboard AB_FILE_MASK = 0x303030303030303;
inline constexpr Bitboard GH_FILE_MASK = 0xc0c0c0c0c0c0c0c0;
inline constexpr Bitboard RANK_8_MASK = 0xff00000000000000;
inline constexpr Bitboard RANK_1_MASK = 0xff;
inline constexpr Bitboard RANK_2_MASK = 0xff00;
inline constexpr Bitboard RANK_7_MASK = 0xff000000000000;

// Bitboard bit manipulation
constexpr Bitboard get_bit(Bitboard bitboard, int square) {return bitboard & (1ULL << square);};
constexpr void set_bit(Bitboard& bitboard, int square) {bitboard |= (1ULL << square);};
constexpr void reset_bit(Bitboard& bitboard, int square) {bitboard &= (~(1ULL << square));};
int count_bits(Bitboard bitboard);
// DEPRECATED
int get_LS1B(Bitboard bitboard);

void print(Bitboard bitboard);

#endif //BOARD_DEFS_H
