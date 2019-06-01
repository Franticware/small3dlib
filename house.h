#ifndef HOUSE_H
#define HOUSE_H

#include "houseTexture.h"

const S3L_Unit houseVertices[] = {
    904,   862,   343,        // 0
    904,   862,   556,        // 3
    817,   387,   457,        // 6
    817,   378,   316,        // 9
    736,   862,   556,        // 12
    677,   410,   457,        // 15
   1035,   864,   698,        // 18
    744,   397,   316,        // 21
    829,   862,   343,        // 24
    781,   602,  -291,        // 27
    829,   862,  -519,        // 30
    781,   604,   108,        // 33
     94,   899,   736,        // 36
    736,   862,   455,        // 39
    677,   430,   364,        // 42
    620,  1636,   -88,        // 45
     34,  1587,   -88,        // 48
    822,   -28,   318,        // 51
    765,   -28,   318,        // 54
    656,   -28,   382,        // 57
    656,   -28,   502,        // 60
    822,   -28,   502,        // 63
    301,   407,   364,        // 66
    269,   -28,   384,        // 69
    318,   720,   441,        // 72
    269,   -26,   468,        // 75
    318,   719,   562,        // 78
    232,   628,   554,        // 81
    186,   -26,   474,        // 84
    232,   629,   422,        // 87
    186,   -28,   374,        // 90
    656,   604,   108,        // 93
    744,   408,   102,        // 96
    656,   266,    85,        // 99
    656,   266,  -260,        // 102
    656,   602,  -291,        // 105
    754,   266,    85,        // 108
    754,   266,  -260,        // 111
    765,   -28,  -494,        // 114
   -836,   862,   343,        // 117
   -749,   375,   316,        // 120
   -749,   375,   457,        // 123
   -836,   862,   556,        // 126
   -610,   380,   457,        // 129
   -668,   862,   556,        // 132
   -954,   873,   714,        // 135
   -761,   862,   343,        // 138
   -676,   407,   316,        // 141
   -713,   606,  -247,        // 144
   -661,   359,  -499,        // 147
   -676,   409,  -251,        // 150
   -610,   391,   364,        // 153
   -668,   862,   455,        // 156
  -1223,  1696,  -139,        // 159
   -936,   873,  -856,        // 162
   -696,   -28,   318,        // 165
   -763,   -28,   318,        // 168
   -588,   -28,   480,        // 171
   -588,   -28,   382,        // 174
   -763,   -28,   480,        // 177
   -201,   -28,   384,        // 180
   -221,   407,   364,        // 183
   -238,   705,   441,        // 186
   -201,   -26,   468,        // 189
   -238,   703,   562,        // 192
   -117,   -26,   474,        // 195
   -152,   613,   554,        // 198
   -117,   -28,   374,        // 201
   -152,   614,   422,        // 204
   -836,   862,  -519,        // 207
   -587,   606,    71,        // 210
   -587,   210,    72,        // 213
   -676,   409,    75,        // 216
   -713,   606,    71,        // 219
   -685,   210,    72,        // 222
   -761,   862,  -519,        // 225
    904,   862,  -519,        // 228
    817,   423,  -492,        // 231
    817,   407,  -633,        // 234
    904,   847,  -733,        // 237
   1225,  1817,  -113,        // 240
   -668,   862,  -631,        // 243
    123,   890,  -763,        // 246
    736,   862,  -631,        // 249
    686,   426,  -633,        // 252
    736,   862,  -733,        // 255
   1021,   829,  -826,        // 258
    744,   423,  -492,        // 261
    686,   426,  -540,        // 264
   -551,  1636,   -88,        // 267
   -643,  1479,    71,        // 270
    822,   -28,  -494,        // 273
    656,   -28,  -656,        // 276
    656,   -28,  -558,        // 279
    822,   -28,  -656,        // 282
    744,   409,  -277,        // 285
   -836,   862,  -733,        // 288
   -734,   359,  -633,        // 291
   -734,   359,  -499,        // 294
   -668,   862,  -733,        // 297
   -611,   359,  -633,        // 300
   -587,   606,  -247,        // 303
   -587,   210,  -248,        // 306
   -611,   359,  -540,        // 309
   -753,   -28,  -494,        // 312
   -696,   -28,  -494,        // 315
   -588,   -28,  -558,        // 318
   -588,   -28,  -656,        // 321
   -753,   -28,  -656,        // 324
   -685,   210,  -248,        // 327
   -702,  1389,   161,        // 330
   -628,  1724,   100,        // 333
   -721,  1712,   269,        // 336
   -856,  1741,   112,        // 339
   -794,  1472,    85,        // 342
    904,   862,   556,        // 345
    886,   862,  -641,        // 348
    886,   862,  -641,        // 351
   -836,   862,   556,        // 354
   -818,   862,  -641,        // 357
   -818,   862,  -641,        // 360
    904,   862,  -733,        // 363
    886,   862,   465,        // 366
    886,   862,   465,        // 369
   -836,   862,  -733,        // 372
   -818,   862,   465,        // 375
   -818,   862,   465         // 378
}; // houseVertices

const S3L_Index houseTriangleIndices[] = {
      0,     2,     3,        // 0
      4,     2,     1,        // 3
      4,     1,     6,        // 6
      0,     6,     1,        // 9
      3,     8,     0,        // 12
     10,    11,     9,        // 15
      8,     7,    11,        // 18
     12,     4,     6,        // 21
      5,    13,    14,        // 24
     15,    16,    12,        // 27
      3,    18,     7,        // 30
      5,    19,    20,        // 33
      2,    20,    21,        // 36
      2,    17,     3,        // 39
     22,    19,    14,        // 42
     24,    14,    13,        // 45
     25,    24,    26,        // 48
     23,    22,    24,        // 51
     25,    27,    28,        // 54
     28,    29,    30,        // 57
     31,    32,    33,        // 60
     32,    31,    11,        // 63
     31,    34,    35,        // 66
     36,    33,    32,        // 69
     37,    18,    38,        // 72
     18,    36,     7,        // 75
     39,    41,    42,        // 78
     41,    44,    42,        // 81
     44,    45,    42,        // 84
     39,    42,    45,        // 87
     39,    47,    40,        // 90
     48,    49,    50,        // 93
     12,    45,    44,        // 96
     43,    52,    44,        // 99
     53,    54,    45,        // 102
     55,    40,    47,        // 105
     43,    58,    51,        // 108
     41,    57,    43,        // 111
     56,    41,    40,        // 114
     58,    61,    51,        // 117
     51,    62,    52,        // 120
     63,    62,    60,        // 123
     60,    62,    61,        // 126
     63,    66,    64,        // 129
     65,    68,    66,        // 132
     45,    69,    39,        // 135
     70,    71,    72,        // 138
     72,    73,    70,        // 141
     74,    72,    71,        // 144
     75,    49,    48,        // 147
     76,    78,    79,        // 150
     15,     6,    80,        // 153
     81,    82,    83,        // 156
     78,    85,    79,        // 159
     85,    86,    79,        // 162
     76,    79,    86,        // 165
     10,    77,    76,        // 168
      0,    10,    76,        // 171
      7,    32,    11,        // 174
     82,    86,    85,        // 177
     84,    83,    85,        // 180
     16,    89,    90,        // 183
     15,    82,    16,        // 186
     38,    77,    87,        // 189
     84,    93,    88,        // 192
     78,    92,    84,        // 195
     91,    78,    77,        // 198
     35,    34,    95,        // 201
     95,     9,    35,        // 204
     11,    35,     9,        // 207
     37,    95,    34,        // 210
     33,    37,    34,        // 213
      7,    36,    32,        // 216
     69,    97,    98,        // 219
     89,    54,    53,        // 222
     99,    82,    81,        // 225
     99,    97,    96,        // 228
     99,    96,    54,        // 231
     69,    54,    96,        // 234
     98,    75,    69,        // 237
    101,    71,    70,        // 240
     46,    73,    47,        // 243
     82,    99,    54,        // 246
    100,    81,   103,        // 249
     15,    86,    82,        // 252
     89,    16,    82,        // 255
     73,    75,    48,        // 258
     98,   105,    49,        // 261
    100,   106,   107,        // 264
    100,   108,    97,        // 267
     97,   104,    98,        // 270
    103,    93,   106,        // 273
     81,    88,   103,        // 276
    109,    55,    74,        // 279
    101,    50,   102,        // 282
     50,   101,    48,        // 285
    102,    74,    71,        // 288
    109,   102,    50,        // 291
     48,    70,    73,        // 294
     73,    72,    47,        // 297
     69,    46,    39,        // 300
     86,     0,    76,        // 303
     80,     6,    86,        // 306
     30,    68,    67,        // 309
     27,    68,    29,        // 312
     27,    64,    66,        // 315
     62,    26,    24,        // 318
     24,    52,    62,        // 321
     12,    52,    13,        // 324
      4,    12,    13,        // 327
     12,    44,    52,        // 330
     10,     9,    87,        // 333
     87,     9,    95,        // 336
     87,    95,    37,        // 339
     38,    87,    37,        // 342
    109,    50,    49,        // 345
     49,   105,   109,        // 348
     74,    47,    72,        // 351
     47,    74,    55,        // 354
     90,   112,   110,        // 357
    112,   114,   110,        // 360
    114,   111,    90,        // 363
    113,   112,   111,        // 366
     45,   114,    53,        // 369
    110,   114,    45,        // 372
     12,    16,    90,        // 375
    110,    12,    90,        // 378
    110,    45,    12,        // 381
     90,    89,   114,        // 384
     89,    53,   114,        // 387
     82,    85,    83,        // 390
      0,     1,     2,        // 393
      4,     5,     2,        // 396
      3,     7,     8,        // 399
     10,     8,    11,        // 402
      5,     4,    13,        // 405
      3,    17,    18,        // 408
      5,    14,    19,        // 411
      2,     5,    20,        // 414
      2,    21,    17,        // 417
     22,    23,    19,        // 420
     24,    22,    14,        // 423
     25,    23,    24,        // 426
     25,    26,    27,        // 429
     28,    27,    29,        // 432
     31,    33,    34,        // 435
     37,    36,    18,        // 438
     39,    40,    41,        // 441
     41,    43,    44,        // 444
     39,    46,    47,        // 447
     43,    51,    52,        // 450
     55,    56,    40,        // 453
     43,    57,    58,        // 456
     41,    59,    57,        // 459
     56,    59,    41,        // 462
     58,    60,    61,        // 465
     51,    61,    62,        // 468
     63,    64,    62,        // 471
     63,    65,    66,        // 474
     65,    67,    68,        // 477
     45,    54,    69,        // 480
     76,    77,    78,        // 483
     15,    12,     6,        // 486
     78,    84,    85,        // 489
     10,    87,    77,        // 492
      0,     8,    10,        // 495
     84,    88,    83,        // 498
     38,    91,    77,        // 501
     84,    92,    93,        // 504
     78,    94,    92,        // 507
     91,    94,    78,        // 510
     11,    31,    35,        // 513
     33,    36,    37,        // 516
     69,    96,    97,        // 519
     89,    82,    54,        // 522
     99,   100,    97,        // 525
     98,    49,    75,        // 528
    101,   102,    71,        // 531
    100,    99,    81,        // 534
     15,    80,    86,        // 537
     73,    46,    75,        // 540
     98,   104,   105,        // 543
    100,   103,   106,        // 546
    100,   107,   108,        // 549
     97,   108,   104,        // 552
    103,    88,    93,        // 555
     81,    83,    88,        // 558
    109,   105,    55,        // 561
    102,   109,    74,        // 564
     48,   101,    70,        // 567
     69,    75,    46,        // 570
     86,     6,     0,        // 573
     30,    29,    68,        // 576
     27,    66,    68,        // 579
     27,    26,    64,        // 582
     62,    64,    26,        // 585
     24,    13,    52,        // 588
     90,   111,   112,        // 591
    112,   113,   114,        // 594
    114,   113,   111         // 597
}; // houseTriangleIndices

const S3L_Unit houseUVs[] = {
    279,   174,         // 0
    166,   140,         // 2
    166,   174,         // 4
    279,   106,         // 6
    279,   140,         // 8
    281,   238,         // 10
    307,   238,         // 12
    327,   260,         // 14
    307,   206,         // 16
    279,   204,         // 18
    484,    15,         // 20
    393,    53,         // 22
    447,    53,         // 24
    355,    15,         // 26
    360,    83,         // 28
    183,   266,         // 30
    166,   106,         // 32
    279,    74,         // 34
    166,    74,         // 36
    263,   397,         // 38
    203,   397,         // 40
    203,   505,         // 42
     68,   204,         // 44
    166,   204,         // 46
     68,    74,         // 48
     68,   106,         // 50
     68,   140,         // 52
     68,   174,         // 54
    454,   338,         // 56
    494,   392,         // 58
    496,   335,         // 60
    451,   298,         // 62
    504,   280,         // 64
    440,   382,         // 66
    445,   304,         // 68
    445,   392,         // 70
    436,   314,         // 72
    431,   382,         // 74
    432,   318,         // 76
    428,   373,         // 78
    399,    58,         // 80
    392,    82,         // 82
    401,   102,         // 84
    439,   102,         // 86
    441,    58,         // 88
    395,   107,         // 90
    444,   107,         // 92
    359,   147,         // 94
    480,   147,         // 96
    279,   106,         // 98
    166,   140,         // 100
    279,   140,         // 102
    279,   174,         // 104
     66,   238,         // 106
     22,   263,         // 108
     41,   238,         // 110
     41,   206,         // 112
    166,    74,         // 114
    166,   106,         // 116
    447,    53,         // 118
    480,    85,         // 120
    448,    82,         // 122
    166,   174,         // 124
    279,   204,         // 126
     86,   397,         // 128
      0,   449,         // 130
     86,   505,         // 132
     68,    74,         // 134
     68,   204,         // 136
    166,   204,         // 138
     68,   174,         // 140
     68,   106,         // 142
    337,   392,         // 144
    378,   338,         // 146
    334,   340,         // 148
    381,   300,         // 150
    327,   280,         // 152
    391,   382,         // 154
    385,   392,         // 156
    396,   315,         // 158
    387,   306,         // 160
    399,   382,         // 162
    400,   319,         // 164
     41,    74,         // 166
    399,    58,         // 168
    401,   102,         // 170
    392,    82,         // 172
    393,    53,         // 174
    395,   107,         // 176
    484,    15,         // 178
    279,   106,         // 180
    166,   140,         // 182
    279,   140,         // 184
    314,   505,         // 186
    314,   397,         // 188
     66,    57,         // 190
    187,    36,         // 192
    281,    57,         // 194
    279,   174,         // 196
    281,    41,         // 198
    325,    27,         // 200
    307,    41,         // 202
    307,    74,         // 204
    279,    74,         // 206
    166,   106,         // 208
    295,    74,         // 210
    166,   174,         // 212
    279,   204,         // 214
    144,   397,         // 216
    136,   420,         // 218
    203,   291,         // 220
     68,    74,         // 222
    166,    74,         // 224
     68,   204,         // 226
    166,   204,         // 228
     68,   174,         // 230
     68,   106,         // 232
    448,    82,         // 234
    279,   174,         // 236
    166,   140,         // 238
    166,   174,         // 240
     86,   291,         // 242
     66,    41,         // 244
    279,   106,         // 246
    279,   140,         // 248
     41,    41,         // 250
     25,    22,         // 252
    279,   204,         // 254
    441,    58,         // 256
    355,    15,         // 258
    360,    83,         // 260
    166,   106,         // 262
    279,    74,         // 264
    166,    74,         // 266
    314,   291,         // 268
     68,   204,         // 270
    166,   204,         // 272
     68,    74,         // 274
     68,   106,         // 276
     68,   140,         // 278
     68,   174,         // 280
    495,   218,         // 282
    345,   264,         // 284
    492,   264,         // 286
    502,   158,         // 288
    342,   210,         // 290
    444,   107,         // 292
    359,   147,         // 294
    439,   102,         // 296
     52,   206,         // 298
    402,   449,         // 300
    403,   373,         // 302
     66,   223,         // 304
    281,   223,         // 306
    480,    85,         // 308
    480,   147,         // 310
    492,   407,         // 312
    425,   437,         // 314
    492,   437,         // 316
    492,   467,         // 318
    425,   500,         // 320
    492,   500,         // 322
    425,   467,         // 324
    398,   484,         // 326
    121,   423,         // 328
    131,   433,         // 330
    279,    74,         // 332
     68,   140,         // 334
    295,   206,         // 336
     68,   140,         // 338
    336,   158,         // 340
     52,    74,         // 342
    425,   407          // 344
}; // houseUVs

const S3L_Index houseUVIndices[] = {
      0,     1,     2,        // 0
      3,     1,     4,        // 3
      5,     6,     7,        // 6
      8,     7,     6,        // 9
      2,     9,     0,        // 12
     10,    11,    12,        // 15
     13,    14,    11,        // 18
     15,     5,     7,        // 21
     16,    17,    18,        // 24
     19,    20,    21,        // 27
      2,    22,    23,        // 30
     16,    24,    25,        // 33
      1,    25,    26,        // 36
      1,    27,     2,        // 39
     28,    29,    30,        // 42
     31,    30,    32,        // 45
     33,    31,    34,        // 48
     35,    28,    31,        // 51
     33,    36,    37,        // 54
     37,    38,    39,        // 57
     40,    41,    42,        // 60
     41,    40,    11,        // 63
     40,    43,    44,        // 66
     45,    42,    41,        // 69
     46,    47,    48,        // 72
     47,    45,    14,        // 75
     49,    50,    51,        // 78
     50,    52,    51,        // 81
     53,    54,    55,        // 84
     56,    55,    54,        // 87
     49,    57,    58,        // 90
     59,    60,    61,        // 93
     15,    54,    53,        // 96
     62,    63,    52,        // 99
     64,    65,    66,        // 102
     67,    58,    57,        // 105
     62,    68,    69,        // 108
     50,    70,    62,        // 111
     71,    50,    58,        // 114
     72,    73,    74,        // 117
     74,    75,    76,        // 120
     77,    75,    78,        // 123
     78,    75,    73,        // 126
     77,    79,    80,        // 129
     81,    82,    79,        // 132
     54,    83,    56,        // 135
     84,    85,    86,        // 138
     86,    87,    84,        // 141
     88,    86,    85,        // 144
     89,    60,    59,        // 147
     90,    91,    92,        // 150
     19,    93,    94,        // 153
     95,    96,    97,        // 156
     91,    98,    92,        // 159
     99,   100,   101,        // 162
    102,   101,   100,        // 165
    103,   104,    90,        // 168
      8,   105,   102,        // 171
     14,    41,    11,        // 174
     96,   100,    99,        // 177
    106,   107,    98,        // 180
     20,   108,   109,        // 183
     19,   110,    20,        // 186
    111,   104,   112,        // 189
    106,   113,   114,        // 192
     91,   115,   106,        // 195
    116,    91,   104,        // 198
     44,    43,   117,        // 201
    117,    12,    44,        // 204
     11,    44,    12,        // 207
     46,   117,    43,        // 210
     42,    46,    43,        // 213
     14,    45,    41,        // 216
    118,   119,   120,        // 219
    108,   121,    64,        // 222
    122,    96,    95,        // 225
    123,   119,   124,        // 228
    122,   125,   126,        // 231
     83,   126,   125,        // 234
    120,   127,   118,        // 237
    128,    85,    84,        // 240
    129,    87,   130,        // 243
     96,   122,   126,        // 246
    131,   132,   133,        // 249
     19,   134,   110,        // 252
    108,    20,   110,        // 255
     87,    89,    59,        // 258
    120,   135,   136,        // 261
    131,   137,   138,        // 264
    131,   139,   119,        // 267
    119,   140,   120,        // 270
    141,   142,   143,        // 273
    144,   145,   141,        // 276
    146,   147,    88,        // 279
    128,    61,   148,        // 282
     61,   128,    59,        // 285
    148,    88,    85,        // 288
    146,   148,    61,        // 291
     59,    84,    87,        // 294
     87,    86,   130,        // 297
     83,   149,    56,        // 300
    100,     8,   102,        // 303
     94,    93,   150,        // 306
     39,    82,   151,        // 309
     36,    82,    38,        // 312
     36,    80,    79,        // 315
     75,    34,    31,        // 318
     31,    76,    75,        // 321
     15,   152,   153,        // 324
      5,    15,   153,        // 327
     15,    53,   152,        // 330
     10,    12,   154,        // 333
    154,    12,   117,        // 336
    154,   117,    46,        // 339
     48,   154,    46,        // 342
    146,    61,    60,        // 345
     60,   155,   146,        // 348
     88,   130,    86,        // 351
    130,    88,   147,        // 354
    156,   157,   158,        // 357
    157,   159,   158,        // 360
    159,   160,   161,        // 363
    162,   163,   160,        // 366
     66,   164,    64,        // 369
    165,   164,    66,        // 372
     21,    20,   109,        // 375
    165,    21,   109,        // 378
    165,    66,    21,        // 381
    109,   108,   164,        // 384
    108,    64,   164,        // 387
     96,    99,    97,        // 390
      0,     4,     1,        // 393
      3,    16,     1,        // 396
      2,    23,     9,        // 399
     10,    13,    11,        // 402
     16,     3,    17,        // 405
      2,    27,    22,        // 408
     16,    18,    24,        // 411
      1,    16,    25,        // 414
      1,    26,    27,        // 417
     28,    35,    29,        // 420
     31,    28,    30,        // 423
     33,    35,    31,        // 426
     33,    34,    36,        // 429
     37,    36,    38,        // 432
     40,    42,    43,        // 435
     46,    45,    47,        // 438
     49,    58,    50,        // 441
     50,    62,    52,        // 444
     49,   166,    57,        // 447
     62,    69,    63,        // 450
     67,    71,    58,        // 453
     62,    70,    68,        // 456
     50,   167,    70,        // 459
     71,   167,    50,        // 462
     72,    78,    73,        // 465
     74,    73,    75,        // 468
     77,    80,    75,        // 471
     77,    81,    79,        // 474
     81,   151,    82,        // 477
     54,   126,    83,        // 480
     90,   104,    91,        // 483
     19,    21,    93,        // 486
     91,   106,    98,        // 489
    103,   112,   104,        // 492
      8,   168,   105,        // 495
    106,   114,   107,        // 498
    111,   116,   104,        // 501
    106,   115,   113,        // 504
     91,   169,   115,        // 507
    116,   169,    91,        // 510
     11,    40,    44,        // 513
     42,    45,    46,        // 516
    118,   124,   119,        // 519
    108,   110,   121,        // 522
    123,   131,   119,        // 525
    120,   136,   127,        // 528
    128,   148,    85,        // 531
    131,   123,   132,        // 534
     19,    94,   134,        // 537
     87,   129,    89,        // 540
    120,   140,   135,        // 543
    131,   133,   137,        // 546
    131,   138,   139,        // 549
    119,   139,   140,        // 552
    141,   145,   142,        // 555
    144,   170,   145,        // 558
    146,   155,   147,        // 561
    148,   146,    88,        // 564
     59,   128,    84,        // 567
     83,   171,   149,        // 570
    100,     7,     8,        // 573
     39,    38,    82,        // 576
     36,    79,    82,        // 579
     36,    34,    80,        // 582
     75,    80,    34,        // 585
     31,    32,    76,        // 588
    156,   172,   157,        // 591
    157,   162,   159,        // 594
    159,   162,   160         // 597
}; // houseUVIndices

S3L_Model3D house = 
{
  .vertices = houseVertices,
  .vertexCount = 127,
  .triangles = houseTriangleIndices,
  .triangleCount = 200
};

#endif // guard
