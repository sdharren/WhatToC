#include "PERFT.h"

#include <assert.h>
#include <iostream>

void perft_tests()
{
    PERFT perft = PERFT();

    // regular position
    perft.engine.board.parse_FEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    perft.perft_driver(1);
    assert(perft.nodes == 20);
    perft.nodes = 0;
    perft.perft_driver(2);
    assert(perft.nodes == 400);
    perft.nodes = 0;
    perft.perft_driver(3);
    assert(perft.nodes == 8902);
    perft.nodes = 0;
    perft.perft_driver(4);
    assert(perft.nodes == 197281);
    perft.nodes = 0;
    perft.perft_driver(5);
    assert(perft.nodes == 4865609);
    perft.nodes = 0;

    // position 2
    perft = PERFT();
    perft.engine.board.parse_FEN("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
    perft.perft_driver(1);
    assert(perft.nodes == 48);
    perft.nodes = 0;
    perft.perft_driver(2);
    assert(perft.nodes == 2039);
    perft.nodes = 0;
    perft.perft_driver(3);
    assert(perft.nodes == 97862);
    perft.nodes = 0;
    perft.perft_driver(4);
    assert(perft.nodes == 4085603);
    perft.nodes = 0;

    // position 5
    perft = PERFT();
    perft.engine.board.parse_FEN("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");
    perft.perft_driver(1);
    assert(perft.nodes == 44);
    perft.nodes = 0;
    perft.perft_driver(2);
    assert(perft.nodes == 1486);
    perft.nodes = 0;
    perft.perft_driver(3);
    assert(perft.nodes == 62379);
    perft.nodes = 0;
    perft.perft_driver(4);
    assert(perft.nodes == 2103487);
    perft.nodes = 0;
    perft.perft_driver(5);
    assert(perft.nodes == 89941194);
    perft.nodes = 0;

    std::cout << "tests pass" <<  std::endl;
}
