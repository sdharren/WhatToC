#include "Board.h"

#include <iostream>
#include <sstream>

Board::Board() : piece_bitboards(12), white_occupancy(0), black_occupancy(0), occupied_squares(0), piece_list(64),
                 game_state(GameState()), zobrist_randoms(ZobristRandoms())
{

}

/* ---------------------------------------------------------------------------------------------------------------------
 * Board utility functions
 */

void Board::parse_FEN(std::string FEN_string)
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
                int piece = char_to_piece.at(c);
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
    game_state.ep_square = char_to_sq.at(fields[3]);
    game_state.zobrist_key ^= zobrist_randoms.ep_square_randoms[game_state.ep_square];

    // parse halfmove clock
    game_state.halfmove_clock = std::stoi(fields[4]);

    // parse fullmove counter
    game_state.fullmove_counter = std::stoi(fields[5]);
}

void Board::put_piece(int piece, int square, bool is_unmake)
{
    // set piece on bitboard
    set_bit(piece_bitboards[piece], square);

    // update zobrist key
    if (!is_unmake)
    {
        game_state.zobrist_key ^= zobrist_randoms.piece_randoms[piece][square];
    }

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

void Board::remove_piece(int piece, int square, bool is_unmake)
{
    // remove piece on bitboard
    reset_bit(piece_bitboards[piece], square);

    // update zobrist key
    if (!is_unmake)
    {
        game_state.zobrist_key ^= zobrist_randoms.piece_randoms[piece][square];
    }

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

void Board::move_piece(int piece, int start_square, int target_square, bool is_unmake)
{
    // remove piece
    remove_piece(piece, start_square, is_unmake);

    // put piece
    put_piece(piece, target_square, is_unmake);
}

void Board::set_ep_square(int square)
{
    // remove current ep from zobrist
    game_state.zobrist_key ^= zobrist_randoms.ep_square_randoms[game_state.ep_square];

    // set eq square
    game_state.ep_square = square;

    // add new ep to zobrist
    game_state.zobrist_key ^= zobrist_randoms.ep_square_randoms[game_state.ep_square];
}

void Board::swap_side()
{
    // remove from zobrist
    game_state.zobrist_key ^= zobrist_randoms.side_to_move_randoms[game_state.side_to_move];

    // change side
    game_state.side_to_move ^= 1;

    // update zobrist
    game_state.zobrist_key ^= zobrist_randoms.side_to_move_randoms[game_state.side_to_move];
}

void Board::update_castling_rights(int start_square, int target_square)
{
    // remove from zobrist
    game_state.zobrist_key ^= zobrist_randoms.castling_rights_randoms[game_state.castling_rights];

    // update castling rights
    game_state.castling_rights &= CASTLING_RIGHTS[start_square];
    game_state.castling_rights &= CASTLING_RIGHTS[target_square];

    // update zobrist
    game_state.zobrist_key ^= zobrist_randoms.castling_rights_randoms[game_state.castling_rights];
}

/* ---------------------------------------------------------------------------------------------------------------------
 * Debugging functions
 */

void Board::print_piece_list()
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
                x = piece_to_char.at(temp);
            }
            std::cout << x << ' ';;
        }
        std::cout << std::endl;
    }
}

bool Board::operator==(const Board& b2)
{
    if (this->piece_bitboards != b2.piece_bitboards)
    {
        std::cout << "bitboards" << std::endl;
    }

    if (this->white_occupancy != b2.white_occupancy)
    {
        std::cout << "white" << std::endl;
    }

    if (this->black_occupancy != b2.black_occupancy)
    {
        std::cout << "black" << std::endl;
    }

    if (this->piece_list != b2.piece_list)
    {
        std::cout << "piece list" << std::endl;
    }

    if (this->game_state != b2.game_state)
    {
        std::cout << "game state" << std::endl;
    }
    return this->piece_bitboards == b2.piece_bitboards && this->white_occupancy == b2.white_occupancy && this->black_occupancy == b2.black_occupancy && this->piece_list == b2.piece_list && this->game_state == b2.game_state;
}

bool GameState::operator==(const GameState& gs2)
{
    return this->side_to_move == gs2.side_to_move && this->castling_rights == gs2.castling_rights && this->ep_square == gs2.ep_square && this->halfmove_clock == gs2.halfmove_clock && this->fullmove_counter == gs2.fullmove_counter;
}