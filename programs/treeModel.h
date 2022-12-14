#ifndef TREE_MODEL_H
#define TREE_MODEL_H

#define TREE_VERTEX_COUNT 29
const S3L_Unit treeVertices[TREE_VERTEX_COUNT * 3] = {
   -405,   333,   436,        // 0
   -279,   391,  -162,        // 3
   -210,  1196,   -77,        // 6
   -182,  1351,   276,        // 9
    300,  -235,   884,        // 12
    175,   458,   466,        // 15
    323,   456,   -89,        // 18
    799,  -149,  -292,        // 21
   -377,   -35,  -757,        // 24
   -796,  -197,   418,        // 27
    112,  1210,  -203,        // 30
    329,  1710,  -420,        // 33
   -444,  1833,  -237,        // 36
    224,  1289,   169,        // 39
   -129,  2307, -1354,        // 42
    388,  1640,   328,        // 45
   -176,  1684,   369,        // 48
   1428,  1950,  -138,        // 51
  -1254,  2185,   279,        // 54
    237,  1961,  1274,        // 57
    843,  2065, -1083,        // 60
    366,  3590,  1466,        // 63
   1533,  3256,  -257,        // 66
   1256,  2031,   799,        // 69
   -745,  2083,  1179,        // 72
  -1320,  3489,   438,        // 75
  -1176,  2393,  -744,        // 78
   -119,  3625, -1383,        // 81
    242,  4350,   183         // 84
}; // treeVertices

#define TREE_TRIANGLE_COUNT 52
const S3L_Index treeTriangleIndices[TREE_TRIANGLE_COUNT * 3] = {
      0,     2,     3,        // 0
      5,     7,     4,        // 3
      6,     8,     7,        // 6
      1,     9,     8,        // 9
      4,     0,     5,        // 12
     10,    12,     2,        // 15
      5,     3,    13,        // 18
      1,    10,     2,        // 21
      6,    13,    10,        // 24
     12,    11,    14,        // 27
     13,    11,    10,        // 30
      3,    12,    16,        // 33
     13,    16,    15,        // 36
     11,    15,    17,        // 39
     16,    12,    18,        // 42
     16,    19,    15,        // 45
     17,    20,    11,        // 48
     21,    22,    23,        // 51
     19,    23,    15,        // 54
     21,    24,    25,        // 57
     16,    18,    24,        // 60
     25,    26,    27,        // 63
     12,    14,    26,        // 66
     22,    27,    20,        // 69
     27,    22,    28,        // 72
     22,    21,    28,        // 75
     25,    28,    21,        // 78
     27,    28,    25,        // 81
     20,    14,    11,        // 84
     27,    14,    20,        // 87
     26,    14,    27,        // 90
     23,    17,    15,        // 93
     22,    17,    23,        // 96
     20,    17,    22,        // 99
     16,    24,    19,        // 102
     21,    19,    24,        // 105
     23,    19,    21,        // 108
     12,    26,    18,        // 111
     25,    18,    26,        // 114
     24,    18,    25,        // 117
      0,     1,     2,        // 120
      5,     6,     7,        // 123
      6,     1,     8,        // 126
      1,     0,     9,        // 129
      4,     9,     0,        // 132
     10,    11,    12,        // 135
      5,     0,     3,        // 138
      1,     6,    10,        // 141
      6,     5,    13,        // 144
     13,    15,    11,        // 147
      3,     2,    12,        // 150
     13,     3,    16         // 153
}; // treeTriangleIndices

#define TREE_UV_COUNT 35
const S3L_Unit treeUVs[TREE_UV_COUNT * 2] = {
    133,   397,         // 0
    228,   338,         // 2
    125,   335,         // 4
    489,   390,         // 6
    355,   489,         // 8
    493,   487,         // 10
    353,   401,         // 12
    234,   487,         // 14
    230,   399,         // 16
    132,   486,         // 18
     26,   491,         // 20
     26,   389,         // 22
    352,   333,         // 24
    221,   283,         // 26
     23,   327,         // 28
    488,   321,         // 30
    346,   287,         // 32
    260,   212,         // 34
    109,   271,         // 36
     22,   261,         // 38
    490,   270,         // 40
    385,   211,         // 42
    151,   206,         // 44
     17,   170,         // 46
    312,   209,         // 48
    491,    23,         // 50
    359,    85,         // 52
    430,   158,         // 54
    493,   193,         // 56
     23,    25,         // 58
     93,   172,         // 60
    141,    91,         // 62
    201,   199,         // 64
    256,    88,         // 66
    256,    17          // 68
}; // treeUVs

#define TREE_UV_INDEX_COUNT 52
const S3L_Index treeUVIndices[TREE_UV_INDEX_COUNT * 3] = {
      0,     1,     2,        // 0
      3,     4,     5,        // 3
      6,     7,     4,        // 6
      8,     9,     7,        // 9
     10,     0,    11,        // 12
     12,    13,     1,        // 15
     11,     2,    14,        // 18
      8,    12,     1,        // 21
      6,    15,    12,        // 24
     13,    16,    17,        // 27
     15,    16,    12,        // 30
      2,    13,    18,        // 33
     14,    18,    19,        // 36
     16,    20,    21,        // 39
     18,    13,    22,        // 42
     18,    23,    19,        // 45
     21,    24,    16,        // 48
     25,    26,    27,        // 51
     28,    27,    20,        // 54
     29,    30,    31,        // 57
     18,    22,    30,        // 60
     31,    32,    33,        // 63
     13,    17,    32,        // 66
     26,    33,    24,        // 69
     33,    26,    34,        // 72
     26,    25,    34,        // 75
     31,    34,    29,        // 78
     33,    34,    31,        // 81
     24,    17,    16,        // 84
     33,    17,    24,        // 87
     32,    17,    33,        // 90
     27,    21,    20,        // 93
     26,    21,    27,        // 96
     24,    21,    26,        // 99
     18,    30,    23,        // 102
     29,    23,    30,        // 105
     27,    28,    25,        // 108
     13,    32,    22,        // 111
     31,    22,    32,        // 114
     30,    22,    31,        // 117
      0,     8,     1,        // 120
      3,     6,     4,        // 123
      6,     8,     7,        // 126
      8,     0,     9,        // 129
     10,     9,     0,        // 132
     12,    16,    13,        // 135
     11,     0,     2,        // 138
      8,     6,    12,        // 141
      6,     3,    15,        // 144
     15,    20,    16,        // 147
      2,     1,    13,        // 150
     14,     2,    18         // 153
}; // treeUVIndices

S3L_Model3D treeModel;

void treeModelInit()
{
  S3L_model3DInit(
    treeVertices,
    TREE_VERTEX_COUNT,
    treeTriangleIndices,
    TREE_TRIANGLE_COUNT,
    &treeModel);
}

#endif // guard
