#include "octominx.h"

Move intToMove(int i) { return {static_cast<Face>(i / 2), static_cast<Direction>(i % 2)}; };