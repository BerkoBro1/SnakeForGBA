const u32 alphabet_image[27][8] = {
        {0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000}, //null
        {0x00000000, 0x00111000, 0x01000100, 0x01000100, 0x01111100, 0x01000100, 0x01000100, 0x01000100}, //A
        {0x00000000, 0x00011110, 0x00100010, 0x00100010, 0x00011110, 0x00100010, 0x00100010, 0x00011110}, //B
        {0x00000000, 0x00111100, 0x00000110, 0x00000010, 0x00000010, 0x00000010, 0x00000110, 0x00111100}, //C
        {0x00000000, 0x00011110, 0x00100010, 0x00100010, 0x00100010, 0x00100010, 0x00100010, 0x00011110}, //D
        {0x00000000, 0x00111110, 0x00000010, 0x00000010, 0x00001110, 0x00000010, 0x00000010, 0x00111110}, //E
        {0x00000000, 0x00111110, 0x00000010, 0x00000010, 0x00001110, 0x00000010, 0x00000010, 0x00000010}, //F
        {0x00000000, 0x00111100, 0x00000110, 0x00000010, 0x01110010, 0x01000010, 0x01000110, 0x00111100}, //G
        {0x00000000, 0x01000100, 0x01000100, 0x01000100, 0x01111100, 0x01000100, 0x01000100, 0x01000100}, //H
        {0x00000000, 0x00111110, 0x00001000, 0x00001000, 0x00001000, 0x00001000, 0x00001000, 0x00111110}, //I
        {0x00000000, 0x00111110, 0x00001000, 0x00001000, 0x00001000, 0x00001000, 0x00001010, 0x00001110}, //J
        {0x00000000, 0x00010010, 0x00010010, 0x00001010, 0x00000110, 0x00001010, 0x00010010, 0x00010010}, //K
        {0x00000000, 0x00000010, 0x00000010, 0x00000010, 0x00000010, 0x00000010, 0x00000010, 0x00011110}, //L
        {0x00000000, 0x10000010, 0x11000110, 0x10101010, 0x10010010, 0x10000010, 0x10000010, 0x10000010}, //M
        {0x00000000, 0x01000010, 0x01000010, 0x01000110, 0x01001010, 0x01010010, 0x01100010, 0x01000010}, //N
        {0x00000000, 0x00111100, 0x01000010, 0x01000010, 0x01000010, 0x01000010, 0x01000010, 0x00111100}, //O
        {0x00000000, 0x00011110, 0x00100010, 0x00100010, 0x00011110, 0x00000010, 0x00000010, 0x00000010}, //P
        {0x00000000, 0x00111100, 0x01000010, 0x01000010, 0x01000010, 0x01000010, 0x01100010, 0x11111100}, //Q
        {0x00000000, 0x00011110, 0x00100010, 0x00100010, 0x00011110, 0x00001010, 0x00010010, 0x00100010}, //R
        {0x00000000, 0x00111100, 0x00000010, 0x00000010, 0x00111100, 0x01000000, 0x01000000, 0x00111100}, //S
        {0x00000000, 0x00111110, 0x00001000, 0x00001000, 0x00001000, 0x00001000, 0x00001000, 0x00001000}, //T
        {0x00000000, 0x01000010, 0x01000010, 0x01000010, 0x01000010, 0x01000010, 0x01000010, 0x00111100}, //U
        {0x00000000, 0x00100010, 0x00100010, 0x00100010, 0x00100010, 0x00100010, 0x00010100, 0x00001000}, //V
        {0x00000000, 0x00100010, 0x00100010, 0x00100010, 0x00100010, 0x00101010, 0x00101010, 0x00011100}, //W
        {0x00000000, 0x01000010, 0x00100100, 0x00100100, 0x00011000, 0x00011000, 0x00100100, 0x01000010}, //X
        {0x00000000, 0x00100010, 0x00100010, 0x00100010, 0x00010100, 0x00001000, 0x00001000, 0x00001000}, //Y
        {0x00000000, 0x01111110, 0x01000000, 0x00100000, 0x00011000, 0x00000100, 0x00000010, 0x01111110}, //Z
};