#include "Board.h"

#include <iostream>
#include <sstream>
#include <vector>

#include "../helpers/bit_helpers.h"

Board::Board() : piece_bitboards(12), white_occupancy(0), black_occupancy(0), occupied_squares(0), side_to_move(0),
                 castling_rights(0), ep_square(no_sq), halfmove_clock(0), fullmove_counter(0)
{

}

// function to pretty print the bitboard, for debugging
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

void Board::parse_FEN(std::string FEN_string)
{
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
                set_bit(piece_bitboards[piece], square);
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
    side_to_move = fields[1] == "w" ? 0 : 1;

    // parse castling rights
    if (fields[2] == "-")
    {
        castling_rights = 0;
    }
    else
    {
        for (auto c: fields[2])
        {
            if (c == 'K') castling_rights |= (1 << 3);
            if (c == 'Q') castling_rights |= (1 << 2);
            if (c == 'k') castling_rights |= (1 << 1);
            if (c == 'q') castling_rights |= 1;
        }
    }

    // parse en passant square
    ep_square = char_to_sq[fields[3]];

    // parse halfmove clock
    halfmove_clock = std::stoi(fields[4]);

    // parse fullmove counter
    fullmove_counter = std::stoi(fields[5]);


}