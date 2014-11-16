#ifndef SHAPES_HEADER
#define SHAPES_HEADER

const int SHAPESIZE = 4;

enum SHAPE_NUM{
I_SHAPE = 0,
J_SHAPE,
L_SHAPE,
O_SHAPE,
S_SHAPE,
T_SHAPE,
Z_SHAPE,
TOTAL_SHAPES
};

struct PSHAPE{
    int shape[SHAPESIZE][SHAPESIZE];
    int xl_offset;
    int xr_offset;
    int yt_offset;
    int yb_offset;
};

#endif // SHAPES_HEADER
