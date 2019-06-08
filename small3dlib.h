#ifndef SMALL3DLIB_H
#define SMALL3DLIB_H

/*
  WIP

  Simple realtime 3D software rasterization renderer. It is fast, focused on
  resource-limited computers, located in a single C header file, with no
  dependencies, using only integer arithmetics.

  author: Miloslav Ciz
  license: CC0 1.0 + additional waiver of all IP

  --------------------

  This work's goal is to never be encumbered by any exclusive intellectual
  property rights. The work is therefore provided under CC0 1.0 + additional
  WAIVER OF ALL INTELLECTUAL PROPERTY RIGHTS that waives the rest of
  intellectual property rights not already waived by CC0 1.0. The WAIVER OF ALL
  INTELLECTUAL PROPERTY RGHTS is as follows:

  Each contributor to this work agrees that they waive any exclusive rights,
  including but not limited to copyright, patents, trademark, trade dress,
  industrial design, plant varieties and trade secrets, to any and all ideas,
  concepts, processes, discoveries, improvements and inventions conceived,
  discovered, made, designed, researched or developed by the contributor either
  solely or jointly with others, which relate to this work or result from this
  work. Should any waiver of such right be judged legally invalid or
  ineffective under applicable law, the contributor hereby grants to each
  affected person a royalty-free, non transferable, non sublicensable, non
  exclusive, irrevocable and unconditional license to this right.

  --------------------

  CONVENTIONS:

  This library should never draw pixels outside the specified screen
  boundaries, so you don't have to check this!

  You can safely assume that triangles are rasterized one by one and from top
  down, left to right (so you can utilize e.g. various caches), and if sorting
  is disabled the order of rasterization will be that specified in the scene
  structure (of course, some triangles and models may be skipped).

  Angles are in S3L_Units, a full angle (2 pi) is S3L_FRACTIONS_PER_UNITs.

  We use row vectors.

  In 3D space, a left-handed coord. system is used. One spatial unit is split
  into S3L_FRACTIONS_PER_UNIT fractions (fixed point arithmetic).

     y ^
       |   _ 
       |   /| z
       |  /
       | /
  [0,0,0]-------> x

  Untransformed camera is placed at [0,0,0], looking forward along +z axis. The
  projection plane is centered at [0,0,0], stretrinch from
  -S3L_FRACTIONS_PER_UNIT to S3L_FRACTIONS_PER_UNIT horizontally (x),
  vertical size (y) depends on the camera aspect ratio. Camera FOV is defined
  by focal length.

           y ^
             |  _
             |  /| z
         ____|_/__
        |    |/   |
     -----[0,0,0]-|-----> x
        |____|____|
             |    
             |

  Rotations use Euler angles and are generally in the extinsic Euler angles in
  ZXY order (by Z, then by X, then by Y).

  Coordinates of pixels on screen start typically at the top left, from [0,0].

  There is NO subpixel accuracy (screen coordinates are only integer).

  Triangle rasterization rules are these (mostly same as OpenGL, D3D etc.):

  - Let's define:
    - left side:
      - not exactly horizontal, and on the left side of triangle
      - exactly horizontal and above the topmost
      (in other words: its normal points at least a little to the left or
       completely up)
    - right side: not left side
  - Pixel centers are at integer coordinates and triangle for drawing are
    specified with integer coordinates of pixel centers.
  - A pixel is rasterized:
    - if its center is inside the triangle OR
    - if its center is exactly on the triangle side which is left and at the
      same time is not on the side that's right (case of a triangle that's on
      a single line) OR
    - if its center is exactly on the triangle corner of sides neither of which
      is right.

  These rules imply among others:

  - Adjacent triangles don't have any overlapping pixels, nor gaps between.
  - Triangles of points that lie on a single line are NOT rasterized.
  - A single "long" triangle CAN be rasterized as non-continuous.
  - Transforming (e.g. mirroring, rotating by 90 degrees etc.) a result of
    rasterizing triangle A is NOT generally equal to applying the same
    transformation to triangle A first and then rasterizing it. Even the number
    of rasterized pixels is usually different.
  - If specifying a triangle with integer coordinates, then:
    - The bottom-most corner (or side) of a triangle is never rasterized
      (because it is connected to a right side).
    - The top-most corner can only be rasterized on completely horizontal side
      (otherwise it is connected to a right side).
    - Vertically middle corner is rasterized if and only if it is on the left
      of the triangle and at the same time is also not the bottom-most corner.
*/

#include <stdint.h>

/* === PRESETS ===
   These can be used to quickly set a predefined library behavior. */

// TODO

// ---------------

#ifndef S3L_RESOLUTION_X
#define S3L_RESOLUTION_X 640 ///< Redefine to screen x resolution.
#endif

#ifndef S3L_RESOLUTION_Y
#define S3L_RESOLUTION_Y 480 ///< Redefine to screen y resolution.
#endif

/** Units of measurement in 3D space. There is S3L_FRACTIONS_PER_UNIT in one
spatial unit. By dividing the unit into fractions we effectively achieve a
fixed point arithmetic. The number of fractions is a constant that serves as
1.0 in floating point arithmetic (normalization etc.). */

typedef int32_t S3L_Unit;    

/** How many fractions a spatial unit is split into. This is NOT SUPPOSED TO
BE REDEFINED, so rather don't do it (otherwise things may overflow etc.). */

#define S3L_FRACTIONS_PER_UNIT 512 

typedef int16_t S3L_ScreenCoord;
typedef uint16_t S3L_Index;

#ifndef S3L_STRICT_NEAR_CULLING
  /** If on, any triangle that only partially intersects the near plane will be
  culled. This can prevent errorneous rendering and  artifacts, but also makes
  triangles close to the camera disappear. */

  #define S3L_STRICT_NEAR_CULLING 1 
#endif

#ifndef S3L_FLAT
  /** If on, disables computation of per-pixel values such as barycentric
  coordinates and depth -- these will still be available but will be the same
  for the whole triangle. This can be used to create flat-shaded renders and
  will be a lot faster. With this option on you will probably want to use
  sorting instead of z-buffer. */

  #define S3L_FLAT 0           
#endif

#if S3L_FLAT
  #define S3L_COMPUTE_DEPTH 0
  #define S3L_PERSPECTIVE_CORRECTION 0
  #define S3L_Z_BUFFER 0
#endif

#ifndef S3L_PERSPECTIVE_CORRECTION
  /** Specifies what type of perspective correction (PC) to use. Remember this
  is an expensive operation! Possible values:
 
  - 0: No perspective correction. Fastest, ugly.
  - 1: Per-pixel perspective correction, nice but very expensive.*/

  #define S3L_PERSPECTIVE_CORRECTION 0 
#endif

#if S3L_PERSPECTIVE_CORRECTION
#define S3L_COMPUTE_DEPTH 1  // PC inevitably computes depth, so enable it
#endif

#ifndef S3L_COMPUTE_DEPTH
  /** Whether to compute depth for each pixel (fragment). Some other options
  may turn this on automatically. */
  #define S3L_COMPUTE_DEPTH 1
#endif

#ifndef S3L_Z_BUFFER
  /** What type of z-buffer (depth buffer) to use for visibility determination.
  Possible values:

  - 0: Don't use z-buffer. This saves a lot of memory, but visibility checking
       won't be pixel-accurate and has to mostly be done by other means
       (typically sorting).
  - 1: Use full z-buffer (of S3L_Units) for visibiltiy determination. This is
       the most accurate option (and also a fast one), but  requires a big
       amount of memory.
  - 2: Use reduced-size z-buffer (of bytes). This is fast and somewhat
       accurate, but inaccuracies can occur and a considerable amount of memory
       is needed. */

  #define S3L_Z_BUFFER 0  
#endif

#ifndef S3L_STENCIL_BUFFER
  /** Whether to use stencil buffer for drawing -- with this a pixel that would
  be resterized over an already rasterized pixel will be discarded. This is
  mostly for front-to-back sorted drawing. */

  #define S3L_STENCIL_BUFFER 0 
#endif

#ifndef S3L_SORT
  /** Defines how to sort triangles before drawing a frame. This can be used to
  solve visibility in case z-buffer is not used, to prevent overwrting already
  rasterized pixels, implement transparency etc. Note that for simplicity and
  performance a relatively simple sorting is used which doesn't work completely
  correctly, so mistakes can occur (even the best sorting wouldn't be able to
  solve e.g. intersecting triangles). Possible values:

  - 0: Don't sort triangles. This is fastest.
  - 1: Sort triangles from back to front. This can in most cases solve 
       visibility without requiring almost any extra memory compared to 
       z-buffer.
  - 2: Sort triangles from front to back. This can be faster than back to
       front, because we prevent computing pixels that will be overwritten by
       nearer ones, but we need a 1b stencil buffer for this (enable
       S3L_STENCIL_BUFFER), so a bit more memory is needed. */

  #define S3L_SORT 0
#endif

#ifndef S3L_MAX_TRIANGES_DRAWN
  /** Maximum number of triangles that can be drawn in sorted modes. This
  affects the size of the cache used for triangle sorting. */

  #define S3L_MAX_TRIANGES_DRAWN 128 
#endif

#ifndef S3L_NEAR
  /** Distance of the near clipping plane. Points in front or EXATLY ON this
  plane are considered outside the frustum. This must be >= 0. */

  #define S3L_NEAR (S3L_FRACTIONS_PER_UNIT / 4) 
#endif

#if S3L_NEAR <= 0
#define S3L_NEAR 1 // Can't be <= 0.
#endif

#ifndef S3L_FAST_LERP_QUALITY
  /** Quality (scaling) of SOME (stepped) linear interpolations. 0 will most
  likely be a tiny bit faster, but artifacts can occur for bigger tris, while
  higher values can fix this -- in theory all higher values will have the same
  speed (it is a shift value), but it mustn't be too high to prevent
  overflow. */

  #define S3L_FAST_LERP_QUALITY 8 
#endif

/** Vector that consists of four scalars and can represent homogenous
  coordinates, but is generally also used as Vec3 and Vec2. */
typedef struct
{
  S3L_Unit x;
  S3L_Unit y;
  S3L_Unit z;
  S3L_Unit w;
} S3L_Vec4;

static inline void S3L_initVec4(S3L_Vec4 *v);
static inline void S3L_vec3Add(S3L_Vec4 *result, S3L_Vec4 added);
static inline void S3L_vec3Sub(S3L_Vec4 *result, S3L_Vec4 substracted);
S3L_Unit S3L_vec3Length(S3L_Vec4 v);
void S3L_normalizeVec3(S3L_Vec4 *v);
S3L_Unit S3L_vec2Length(S3L_Vec4 v);
void S3L_normalizeVec2(S3L_Vec4 *v);
void S3L_crossProduct(S3L_Vec4 a, S3L_Vec4 b, S3L_Vec4 *result);
static inline S3L_Unit S3L_dotProductVec3(S3L_Vec4 a, S3L_Vec4 b);

#define S3L_logVec4(v)\
  printf("Vec4: %d %d %d %d\n",((v).x),((v).y),((v).z),((v).w))

typedef struct
{
  S3L_Vec4 translation;
  S3L_Vec4 rotation; /**< Euler angles. Rortation is applied in this order:
                          1. z = by z (roll) CW looking along z+
                          2. x = by x (pitch) CW looking along x+
                          3. y = by y (yaw) CW looking along y+ */
  S3L_Vec4 scale;
} S3L_Transform3D;

#define S3L_logTransform3D(t)\
  printf("Transform3D: T = [%d %d %d], R = [%d %d %d], S = [%d %d %d]\n",\
    (t).translation.x,(t).translation.y,(t).translation.z,\
    (t).rotation.x,(t).rotation.y,(t).rotation.z,\
    (t).scale.x,(t).scale.y,(t).scale.z)

static inline void S3L_initTransoform3D(S3L_Transform3D *t);

void S3L_lookAt(S3L_Vec4 pointFrom, S3L_Vec4 pointTo, S3L_Transform3D *t);

void S3L_setTransform3D(
  S3L_Unit tx,
  S3L_Unit ty,
  S3L_Unit tz,
  S3L_Unit rx,
  S3L_Unit ry,
  S3L_Unit rz,
  S3L_Unit sx,
  S3L_Unit sy,
  S3L_Unit sz,
  S3L_Transform3D *t);

/** Converts rotation transformation to three direction vectors of given length
  (any one can be NULL, in which case it won't be computed). */
void S3L_rotationToDirections(
  S3L_Vec4 rotation,
  S3L_Unit length,
  S3L_Vec4 *forw, 
  S3L_Vec4 *right,
  S3L_Vec4 *up);


/** 4x4 matrix, used mostly for 3D transforms. The indexing is this:
    matrix[column][row]. */
typedef S3L_Unit S3L_Mat4[4][4]; 

#define S3L_logMat4(m)\
  printf("Mat4:\n  %d %d %d %d\n  %d %d %d %d\n  %d %d %d %d\n  %d %d %d %d\n"\
   ,(m)[0][0],(m)[1][0],(m)[2][0],(m)[3][0],\
    (m)[0][1],(m)[1][1],(m)[2][1],(m)[3][1],\
    (m)[0][2],(m)[1][2],(m)[2][2],(m)[3][2],\
    (m)[0][3],(m)[1][3],(m)[2][3],(m)[3][3])

/** Initializes a 4x4 matrix to identity. */
static inline void S3L_initMat4(S3L_Mat4 *m);

void S3L_transposeMat4(S3L_Mat4 *m);

void S3L_makeTranslationMat(
  S3L_Unit offsetX,
  S3L_Unit offsetY,
  S3L_Unit offsetZ,
  S3L_Mat4 *m);

/** Makes a scaling matrix. DON'T FORGET: scale of 1.0 is set with
  S3L_FRACTIONS_PER_UNIT! */
void S3L_makeScaleMatrix(
  S3L_Unit scaleX,
  S3L_Unit scaleY,
  S3L_Unit scaleZ,
  S3L_Mat4 *m);

/** Makes a matrixfor rotation in the ZXY order. */
void S3L_makeRotationMatrixZXY(
  S3L_Unit byX,
  S3L_Unit byY,
  S3L_Unit byZ,
  S3L_Mat4 *m);

void S3L_makeWorldMatrix(S3L_Transform3D worldTransform, S3L_Mat4 *m);
void S3L_makeCameraMatrix(S3L_Transform3D cameraTransform, S3L_Mat4 *m);

/** Multiplies a vector by a matrix with normalization by
  S3L_FRACTIONS_PER_UNIT. Result is stored in the input vector. */
void S3L_vec4Xmat4(S3L_Vec4 *v, S3L_Mat4 *m);

/** Same as S3L_vec4Xmat4 but faster, because this version doesn't compute the
  W component of the result, which is usually not needed. */
void S3L_vec3Xmat4(S3L_Vec4 *v, S3L_Mat4 *m);

/** Multiplies two matrices with normalization by S3L_FRACTIONS_PER_UNIT.
  Result is stored in the first matrix. The result represents a transformation
  that has the same effect as applying the transformation represented by m1 and
  then m2 (in that order). */
void S3L_mat4Xmat4(S3L_Mat4 *m1, S3L_Mat4 *m2);

typedef struct
{
  S3L_Unit focalLength;       ///< Defines the field of view (FOV).
  S3L_Transform3D transform;
} S3L_Camera;

void S3L_initCamera(S3L_Camera *camera);

typedef struct
{
  uint8_t backfaceCulling;    /**< What backface culling to use. Possible
                                   values:
                                   - 0 none
                                   - 1 clock-wise
                                   - 2 counter clock-wise */
  int8_t visible;             /**< Can be used to easily hide the model. */
} S3L_DrawConfig;

void S3L_initDrawConfig(S3L_DrawConfig *config);

typedef struct
{
  const S3L_Unit *vertices;
  S3L_Index vertexCount;
  const S3L_Index *triangles;
  S3L_Index triangleCount;
  S3L_Transform3D transform;
  S3L_DrawConfig config;
} S3L_Model3D;                ///< Represents a 3D model.

void S3L_initModel3D(
  S3L_Unit *vertices,
  S3L_Unit vertexCount,
  S3L_Index *triangles,
  S3L_Index triangleCount,
  S3L_Model3D *model);

typedef struct
{
  S3L_Model3D *models;
  S3L_Index modelCount;
  S3L_Camera camera;
} S3L_Scene;                  ///< Represent the 3D scene to be rendered.

void S3L_initScene(
  S3L_Model3D *models,
  S3L_Index modelCount,
  S3L_Scene *scene);

typedef struct
{
  S3L_ScreenCoord x;          ///< Screen X coordinate.
  S3L_ScreenCoord y;          ///< Screen Y coordinate.

  S3L_Unit barycentric[3]; /**< Barycentric coords corresponds to the three
                              vertices. These serve to locate the pixel on a
                              triangle and interpolate values between it's
                              three points. The sum of the three coordinates
                              will always be exactly S3L_FRACTIONS_PER_UNIT. */
  S3L_Index triangleIndex;  ///< Triangle index.
  S3L_Index modelIndex;
  S3L_Unit depth;         ///< Depth (only if depth is turned on).
  S3L_Unit previousZ;     /**< Z-buffer value (not necessarily world depth in
                               S3L_Units!) that was in the z-buffer on the
                               pixels position before this pixel was
                               rasterized. This can be used to set the value
                               back, e.g. for transparency. */
  S3L_ScreenCoord triangleSize[2]; /**< Rasterized triangle width and height,
                              can be used e.g. for MIP mapping. */
} S3L_PixelInfo;         /**< Used to pass the info about a rasterized pixel
                              (fragment) to the user-defined drawing func. */

static inline void S3L_initPixelInfo(S3L_PixelInfo *p);

// general helper functions
static inline S3L_Unit S3L_abs(S3L_Unit value);
static inline S3L_Unit S3L_min(S3L_Unit v1, S3L_Unit v2);
static inline S3L_Unit S3L_max(S3L_Unit v1, S3L_Unit v2);
static inline S3L_Unit S3L_clamp(S3L_Unit v, S3L_Unit v1, S3L_Unit v2);
static inline S3L_Unit S3L_wrap(S3L_Unit value, S3L_Unit mod);
static inline S3L_Unit S3L_nonZero(S3L_Unit value);

S3L_Unit S3L_sin(S3L_Unit x);
S3L_Unit S3L_asin(S3L_Unit x);
static inline S3L_Unit S3L_cos(S3L_Unit x);

S3L_Unit S3L_vec3Length(S3L_Vec4 v);
S3L_Unit S3L_sqrt(S3L_Unit value);

/** Computes a normalized normal of given triangle. */
void S3L_triangleNormal(S3L_Vec4 t0, S3L_Vec4 t1, S3L_Vec4 t2,
  S3L_Vec4 *n);

/** Computes a normalized normal for every vertex of given model (this is
  relatively slow and SHOUDN'T be done each frame). The dst array must have a
  sufficient size preallocated! The size is: number of model vertices * 3 *
  sizeof(S3L_Unit). Note that for advanced allowing sharp edges it is not
  sufficient to have per-vertex normals, but must be per-triangle. This
  function doesn't support this. */

void S3L_computeModelNormals(S3L_Model3D model, S3L_Unit *dst,
  int8_t transformNormals);

/** Interpolated between two values, v1 and v2, in the same ratio as t is to
  tMax. Does NOT prevent zero division. */
static inline S3L_Unit S3L_interpolate(
  S3L_Unit v1,
  S3L_Unit v2,
  S3L_Unit t,
  S3L_Unit tMax);

/** Same as S3L_interpolate but with v1 == 0. Should be faster. */
static inline S3L_Unit S3L_interpolateFrom0(
  S3L_Unit v2,
  S3L_Unit t,
  S3L_Unit tMax);

/** Like S3L_interpolate, but uses a parameter that goes from 0 to
  S3L_FRACTIONS_PER_UNIT - 1, which can be faster. */
static inline S3L_Unit S3L_interpolateByUnit(
  S3L_Unit v1,
  S3L_Unit v2,
  S3L_Unit t);

/** Same as S3L_interpolateByUnit but with v1 == 0. Should be faster. */
static inline S3L_Unit S3L_interpolateByUnitFrom0(
  S3L_Unit v2,
  S3L_Unit t);

static inline S3L_Unit S3L_distanceManhattan(S3L_Vec4 a, S3L_Vec4 b);

/** Returns a value interpolated between the three triangle vertices based on
  barycentric coordinates. */
static inline S3L_Unit S3L_interpolateBarycentric(
  S3L_Unit value0,
  S3L_Unit value1,
  S3L_Unit value2,
  S3L_Unit barycentric0,
  S3L_Unit barycentric1,
  S3L_Unit barycentric2);

static inline void S3L_mapProjectionPlaneToScreen(
  S3L_Vec4 point,
  S3L_ScreenCoord *screenX,
  S3L_ScreenCoord *screenY);

/** Draws a triangle according to given config. The vertices are specified in
  projection-plane space (NOT screen space!) -- they wll be mapped to screen
  space by thies function. If perspective correction is enabled, each vertex
  has to have a depth (Z position in camera space) specified in the Z
  component. */
void S3L_drawTriangle(
  S3L_Vec4 point0,
  S3L_Vec4 point1,
  S3L_Vec4 point2,
  const S3L_DrawConfig *config,
  const S3L_Camera *camera,
  S3L_Index modelIndex,
  S3L_Index triangleIndex);

/** This should be called before rendering each frame. The function clears
  buffers and does potentially other things needed for the frame. */
void S3L_newFrame();

void S3L_zBufferClear();
void S3L_stencilBufferClear();

void S3L_zBufferWrite(S3L_ScreenCoord x, S3L_ScreenCoord y, S3L_Unit value);
S3L_Unit S3L_zBufferRead(S3L_ScreenCoord x, S3L_ScreenCoord y);

static inline void S3L_rotate2DPoint(S3L_Unit *x, S3L_Unit *y, S3L_Unit angle);

/** Predefined vertices of a cube to simply insert in an array. These come with
    S3L_CUBE_TRIANGLES and S3L_CUBE_TEXCOORDS. */
#define S3L_CUBE_VERTICES(m)\
 /* 0 front, bottom, right */\
 m/2, -m/2, -m/2,\
 /* 1 front, bottom, left */\
-m/2, -m/2, -m/2,\
 /* 2 front, top,    right */\
 m/2,  m/2, -m/2,\
 /* 3 front, top,    left */\
-m/2,  m/2, -m/2,\
 /* 4 back,  bottom, right */\
 m/2, -m/2,  m/2,\
 /* 5 back,  bottom, left */\
-m/2, -m/2,  m/2,\
 /* 6 back,  top,    right */\
 m/2,  m/2,  m/2,\
 /* 7 back,  top,    left */\
-m/2,  m/2,  m/2

#define S3L_CUBE_VERTEX_COUNT 8

/** Predefined triangle indices of a cube, to be used with S3L_CUBE_VERTICES
    and S3L_CUBE_TEXCOORDS. */
#define S3L_CUBE_TRIANGLES\
  3, 0, 2, /* front  */\
  1, 0, 3,\
  0, 4, 2, /* right  */\
  2, 4, 6,\
  4, 5, 6, /* back   */\
  7, 6, 5,\
  3, 7, 1, /* left   */\
  1, 7, 5,\
  6, 3, 2, /* top    */\
  7, 3, 6,\
  1, 4, 0, /* bottom */\
  5, 4, 1

#define S3L_CUBE_TRIANGLE_COUNT 12

/** Predefined texture coordinates of a cube, corresponding to triangles (NOT
    vertices), to be used with S3L_CUBE_VERTICES and S3L_CUBE_TRIANGLES. */
#define S3L_CUBE_TEXCOORDS(m)\
  0,0,  m,m,  m,0,\
  0,m,  m,m,  0,0,\
  m,m,  m,0,  0,m,\
  0,m,  m,0,  0,0,\
  m,0,  0,0,  m,m,\
  0,m,  m,m,  0,0,\
  0,0,  0,m,  m,0,\
  m,0,  0,m,  m,m,\
  0,0,  m,m,  m,0,\
  0,m,  m,m,  0,0,\
  m,0,  0,m,  m,m,\
  0,0,  0,m,  m,0

//=============================================================================
// privates

#define S3L_HALF_RESOLUTION_X (S3L_RESOLUTION_X >> 1)
#define S3L_HALF_RESOLUTION_Y (S3L_RESOLUTION_Y >> 1)

#define S3L_PROJECTION_PLANE_HEIGHT\
  ((S3L_RESOLUTION_Y * S3L_FRACTIONS_PER_UNIT * 2) / S3L_RESOLUTION_X)

#if S3L_Z_BUFFER == 1
  #define S3L_COMPUTE_DEPTH 1
  #define S3L_MAX_DEPTH 2147483647
  S3L_Unit S3L_zBuffer[S3L_RESOLUTION_X * S3L_RESOLUTION_Y];
  #define S3L_zBufferFormat(depth) (depth)
#elif S3L_Z_BUFFER == 2
  #define S3L_COMPUTE_DEPTH 1
  #define S3L_MAX_DEPTH 255
  uint8_t S3L_zBuffer[S3L_RESOLUTION_X * S3L_RESOLUTION_Y];
  #define S3L_zBufferFormat(depth) (((depth) >> 5) & 0x000000FF)
#endif

#if S3L_Z_BUFFER
static inline int8_t S3L_zTest(
  S3L_ScreenCoord x,
  S3L_ScreenCoord y,
  S3L_Unit depth)
{
  uint32_t index = y * S3L_RESOLUTION_X + x;

  depth = S3L_zBufferFormat(depth);

  if (depth < S3L_zBuffer[index])
  {
    S3L_zBuffer[index] = depth;
    return 1;
  }

  return 0;
}
#endif

S3L_Unit S3L_zBufferRead(S3L_ScreenCoord x, S3L_ScreenCoord y)
{
#if S3L_Z_BUFFER
  return S3L_zBuffer[y * S3L_RESOLUTION_X + x];
#else
  return 0;
#endif
}

void S3L_zBufferWrite(S3L_ScreenCoord x, S3L_ScreenCoord y, S3L_Unit value)
{
#if S3L_Z_BUFFER
  S3L_zBuffer[y * S3L_RESOLUTION_X + x] = value;
#endif
}

#if S3L_STENCIL_BUFFER
  #define S3L_STENCIL_BUFFER_SIZE\
    ((S3L_RESOLUTION_X * S3L_RESOLUTION_Y - 1) / 8 + 1)

uint8_t S3L_stencilBuffer[S3L_STENCIL_BUFFER_SIZE];

static inline int8_t S3L_stencilTest(
  S3L_ScreenCoord x,
  S3L_ScreenCoord y)
{
  uint32_t index = y * S3L_RESOLUTION_X + x;
  uint32_t bit = (index & 0x00000007);
  index = index >> 3;

  uint8_t val = S3L_stencilBuffer[index];

  if ((val >> bit) & 0x1)
    return 0;
  
  S3L_stencilBuffer[index] = val | (0x1 << bit);

  return 1;
}
#endif

#define S3L_COMPUTE_LERP_DEPTH\
  (S3L_COMPUTE_DEPTH && (S3L_PERSPECTIVE_CORRECTION != 1))

#define S3L_SIN_TABLE_LENGTH 128

static const S3L_Unit S3L_sinTable[S3L_SIN_TABLE_LENGTH] =
{
  /* 511 was chosen here as a highest number that doesn't overflow during
     compilation for S3L_FRACTIONS_PER_UNIT == 1024 */

  (0*S3L_FRACTIONS_PER_UNIT)/511, (6*S3L_FRACTIONS_PER_UNIT)/511, 
  (12*S3L_FRACTIONS_PER_UNIT)/511, (18*S3L_FRACTIONS_PER_UNIT)/511, 
  (25*S3L_FRACTIONS_PER_UNIT)/511, (31*S3L_FRACTIONS_PER_UNIT)/511, 
  (37*S3L_FRACTIONS_PER_UNIT)/511, (43*S3L_FRACTIONS_PER_UNIT)/511, 
  (50*S3L_FRACTIONS_PER_UNIT)/511, (56*S3L_FRACTIONS_PER_UNIT)/511, 
  (62*S3L_FRACTIONS_PER_UNIT)/511, (68*S3L_FRACTIONS_PER_UNIT)/511, 
  (74*S3L_FRACTIONS_PER_UNIT)/511, (81*S3L_FRACTIONS_PER_UNIT)/511, 
  (87*S3L_FRACTIONS_PER_UNIT)/511, (93*S3L_FRACTIONS_PER_UNIT)/511, 
  (99*S3L_FRACTIONS_PER_UNIT)/511, (105*S3L_FRACTIONS_PER_UNIT)/511, 
  (111*S3L_FRACTIONS_PER_UNIT)/511, (118*S3L_FRACTIONS_PER_UNIT)/511, 
  (124*S3L_FRACTIONS_PER_UNIT)/511, (130*S3L_FRACTIONS_PER_UNIT)/511, 
  (136*S3L_FRACTIONS_PER_UNIT)/511, (142*S3L_FRACTIONS_PER_UNIT)/511, 
  (148*S3L_FRACTIONS_PER_UNIT)/511, (154*S3L_FRACTIONS_PER_UNIT)/511, 
  (160*S3L_FRACTIONS_PER_UNIT)/511, (166*S3L_FRACTIONS_PER_UNIT)/511, 
  (172*S3L_FRACTIONS_PER_UNIT)/511, (178*S3L_FRACTIONS_PER_UNIT)/511, 
  (183*S3L_FRACTIONS_PER_UNIT)/511, (189*S3L_FRACTIONS_PER_UNIT)/511, 
  (195*S3L_FRACTIONS_PER_UNIT)/511, (201*S3L_FRACTIONS_PER_UNIT)/511, 
  (207*S3L_FRACTIONS_PER_UNIT)/511, (212*S3L_FRACTIONS_PER_UNIT)/511, 
  (218*S3L_FRACTIONS_PER_UNIT)/511, (224*S3L_FRACTIONS_PER_UNIT)/511, 
  (229*S3L_FRACTIONS_PER_UNIT)/511, (235*S3L_FRACTIONS_PER_UNIT)/511, 
  (240*S3L_FRACTIONS_PER_UNIT)/511, (246*S3L_FRACTIONS_PER_UNIT)/511, 
  (251*S3L_FRACTIONS_PER_UNIT)/511, (257*S3L_FRACTIONS_PER_UNIT)/511, 
  (262*S3L_FRACTIONS_PER_UNIT)/511, (268*S3L_FRACTIONS_PER_UNIT)/511, 
  (273*S3L_FRACTIONS_PER_UNIT)/511, (278*S3L_FRACTIONS_PER_UNIT)/511, 
  (283*S3L_FRACTIONS_PER_UNIT)/511, (289*S3L_FRACTIONS_PER_UNIT)/511, 
  (294*S3L_FRACTIONS_PER_UNIT)/511, (299*S3L_FRACTIONS_PER_UNIT)/511, 
  (304*S3L_FRACTIONS_PER_UNIT)/511, (309*S3L_FRACTIONS_PER_UNIT)/511, 
  (314*S3L_FRACTIONS_PER_UNIT)/511, (319*S3L_FRACTIONS_PER_UNIT)/511, 
  (324*S3L_FRACTIONS_PER_UNIT)/511, (328*S3L_FRACTIONS_PER_UNIT)/511, 
  (333*S3L_FRACTIONS_PER_UNIT)/511, (338*S3L_FRACTIONS_PER_UNIT)/511, 
  (343*S3L_FRACTIONS_PER_UNIT)/511, (347*S3L_FRACTIONS_PER_UNIT)/511, 
  (352*S3L_FRACTIONS_PER_UNIT)/511, (356*S3L_FRACTIONS_PER_UNIT)/511, 
  (361*S3L_FRACTIONS_PER_UNIT)/511, (365*S3L_FRACTIONS_PER_UNIT)/511, 
  (370*S3L_FRACTIONS_PER_UNIT)/511, (374*S3L_FRACTIONS_PER_UNIT)/511, 
  (378*S3L_FRACTIONS_PER_UNIT)/511, (382*S3L_FRACTIONS_PER_UNIT)/511, 
  (386*S3L_FRACTIONS_PER_UNIT)/511, (391*S3L_FRACTIONS_PER_UNIT)/511, 
  (395*S3L_FRACTIONS_PER_UNIT)/511, (398*S3L_FRACTIONS_PER_UNIT)/511, 
  (402*S3L_FRACTIONS_PER_UNIT)/511, (406*S3L_FRACTIONS_PER_UNIT)/511, 
  (410*S3L_FRACTIONS_PER_UNIT)/511, (414*S3L_FRACTIONS_PER_UNIT)/511, 
  (417*S3L_FRACTIONS_PER_UNIT)/511, (421*S3L_FRACTIONS_PER_UNIT)/511, 
  (424*S3L_FRACTIONS_PER_UNIT)/511, (428*S3L_FRACTIONS_PER_UNIT)/511, 
  (431*S3L_FRACTIONS_PER_UNIT)/511, (435*S3L_FRACTIONS_PER_UNIT)/511, 
  (438*S3L_FRACTIONS_PER_UNIT)/511, (441*S3L_FRACTIONS_PER_UNIT)/511, 
  (444*S3L_FRACTIONS_PER_UNIT)/511, (447*S3L_FRACTIONS_PER_UNIT)/511, 
  (450*S3L_FRACTIONS_PER_UNIT)/511, (453*S3L_FRACTIONS_PER_UNIT)/511, 
  (456*S3L_FRACTIONS_PER_UNIT)/511, (459*S3L_FRACTIONS_PER_UNIT)/511, 
  (461*S3L_FRACTIONS_PER_UNIT)/511, (464*S3L_FRACTIONS_PER_UNIT)/511, 
  (467*S3L_FRACTIONS_PER_UNIT)/511, (469*S3L_FRACTIONS_PER_UNIT)/511, 
  (472*S3L_FRACTIONS_PER_UNIT)/511, (474*S3L_FRACTIONS_PER_UNIT)/511, 
  (476*S3L_FRACTIONS_PER_UNIT)/511, (478*S3L_FRACTIONS_PER_UNIT)/511, 
  (481*S3L_FRACTIONS_PER_UNIT)/511, (483*S3L_FRACTIONS_PER_UNIT)/511, 
  (485*S3L_FRACTIONS_PER_UNIT)/511, (487*S3L_FRACTIONS_PER_UNIT)/511, 
  (488*S3L_FRACTIONS_PER_UNIT)/511, (490*S3L_FRACTIONS_PER_UNIT)/511, 
  (492*S3L_FRACTIONS_PER_UNIT)/511, (494*S3L_FRACTIONS_PER_UNIT)/511, 
  (495*S3L_FRACTIONS_PER_UNIT)/511, (497*S3L_FRACTIONS_PER_UNIT)/511, 
  (498*S3L_FRACTIONS_PER_UNIT)/511, (499*S3L_FRACTIONS_PER_UNIT)/511, 
  (501*S3L_FRACTIONS_PER_UNIT)/511, (502*S3L_FRACTIONS_PER_UNIT)/511, 
  (503*S3L_FRACTIONS_PER_UNIT)/511, (504*S3L_FRACTIONS_PER_UNIT)/511, 
  (505*S3L_FRACTIONS_PER_UNIT)/511, (506*S3L_FRACTIONS_PER_UNIT)/511, 
  (507*S3L_FRACTIONS_PER_UNIT)/511, (507*S3L_FRACTIONS_PER_UNIT)/511, 
  (508*S3L_FRACTIONS_PER_UNIT)/511, (509*S3L_FRACTIONS_PER_UNIT)/511, 
  (509*S3L_FRACTIONS_PER_UNIT)/511, (510*S3L_FRACTIONS_PER_UNIT)/511, 
  (510*S3L_FRACTIONS_PER_UNIT)/511, (510*S3L_FRACTIONS_PER_UNIT)/511, 
  (510*S3L_FRACTIONS_PER_UNIT)/511, (510*S3L_FRACTIONS_PER_UNIT)/511
};

#define S3L_SIN_TABLE_UNIT_STEP\
  (S3L_FRACTIONS_PER_UNIT / (S3L_SIN_TABLE_LENGTH * 4))

void S3L_initVec4(S3L_Vec4 *v)
{
  v->x = 0; v->y = 0; v->z = 0; v->w = S3L_FRACTIONS_PER_UNIT;
}

void S3L_vec3Add(S3L_Vec4 *result, S3L_Vec4 added)
{
  result->x += added.x;
  result->y += added.y;
  result->z += added.z;
}

void S3L_vec3Sub(S3L_Vec4 *result, S3L_Vec4 substracted)
{
  result->x -= substracted.x;
  result->y -= substracted.y;
  result->z -= substracted.z;
}

void S3L_initMat4(S3L_Mat4 *m)
{
  #define M(x,y) (*m)[x][y]
  #define S S3L_FRACTIONS_PER_UNIT

  M(0,0) = S; M(1,0) = 0; M(2,0) = 0; M(3,0) = 0; 
  M(0,1) = 0; M(1,1) = S; M(2,1) = 0; M(3,1) = 0; 
  M(0,2) = 0; M(1,2) = 0; M(2,2) = S; M(3,2) = 0; 
  M(0,3) = 0; M(1,3) = 0; M(2,3) = 0; M(3,3) = S; 

  #undef M
  #undef S
}

S3L_Unit S3L_dotProductVec3(S3L_Vec4 a, S3L_Vec4 b)
{
  return (a.x * b.x + a.y * b.y + a.z * b.z) / S3L_FRACTIONS_PER_UNIT;
}

void S3L_crossProduct(S3L_Vec4 a, S3L_Vec4 b, S3L_Vec4 *result)
{
  result->x = a.y * b.z - a.z * b.y;
  result->y = a.z * b.x - a.x * b.z;
  result->z = a.x * b.y - a.y * b.x;
}

void S3L_triangleNormal(S3L_Vec4 t0, S3L_Vec4 t1, S3L_Vec4 t2,
  S3L_Vec4 *n)
{
  #define antiOverflow 32

  t1.x = (t1.x - t0.x) / antiOverflow;
  t1.y = (t1.y - t0.y) / antiOverflow;
  t1.z = (t1.z - t0.z) / antiOverflow;

  t2.x = (t2.x - t0.x) / antiOverflow;
  t2.y = (t2.y - t0.y) / antiOverflow;
  t2.z = (t2.z - t0.z) / antiOverflow;

  #undef antiOverflow

  S3L_crossProduct(t1,t2,n);
  S3L_normalizeVec3(n);
}

void S3L_computeModelNormals(S3L_Model3D model, S3L_Unit *dst,
  int8_t transformNormals)
{
  S3L_Index vPos = 0;

  S3L_Vec4 n;

  n.w = 0;

  #define MAX_NORMALS 6

  S3L_Vec4 ns[MAX_NORMALS];
  S3L_Index normalCount;

  for (S3L_Index i = 0; i < model.vertexCount; ++i)
  {
    normalCount = 0;

    for (S3L_Index j = 0; j < model.triangleCount * 3; j += 3)
    {
      if (
        (model.triangles[j] == i) ||
        (model.triangles[j + 1] == i) ||
        (model.triangles[j + 2] == i))
      {    
        S3L_Vec4 t0, t1, t2;
        S3L_Index vIndex;

        #define getVertex(n)\
          vIndex = model.triangles[j + n] * 3;\
          t##n.x = model.vertices[vIndex];\
          vIndex++;\
          t##n.y = model.vertices[vIndex];\
          vIndex++;\
          t##n.z = model.vertices[vIndex];

        getVertex(0)
        getVertex(1)
        getVertex(2)

        #undef getVertex
        
        S3L_triangleNormal(t0,t1,t2,&(ns[normalCount]));    

        normalCount++;

        if (normalCount >= MAX_NORMALS)
          break;
      }
    }
      
    n.x = S3L_FRACTIONS_PER_UNIT;
    n.y = 0;
    n.z = 0;

    if (normalCount != 0)
    {
      // compute average

      n.x = 0;

      for (uint8_t i = 0; i < MAX_NORMALS; ++i)
      {
        n.x += ns[i].x;
        n.y += ns[i].y;
        n.z += ns[i].z;
      }

      n.x /= normalCount;
      n.y /= normalCount;
      n.z /= normalCount;

      S3L_normalizeVec3(&n);
    }

    dst[vPos] = n.x;
    vPos++;

    dst[vPos] = n.y;
    vPos++;

    dst[vPos] = n.z;
    vPos++;
  }

  #undef MAX_NORMALS
    
  S3L_Mat4 m;

  S3L_makeWorldMatrix(model.transform,&m);

  if (transformNormals)
    for (S3L_Index i = 0; i < model.vertexCount * 3; i += 3)
    {
      n.x = dst[i];
      n.y = dst[i + 1];
      n.z = dst[i + 2];

      S3L_vec4Xmat4(&n,&m);

      dst[i] = n.x;
      dst[i + 1] = n.y;
      dst[i + 2] = n.z;
    }
}

void S3L_vec4Xmat4(S3L_Vec4 *v, S3L_Mat4 *m)
{
  S3L_Vec4 vBackup;

  vBackup.x = v->x;  
  vBackup.y = v->y;  
  vBackup.z = v->z;  
  vBackup.w = v->w;  

  // TODO: try alternative operation orders to optimize

  #define dotCol(col)\
    ((vBackup.x * (*m)[col][0]) +\
     (vBackup.y * (*m)[col][1]) +\
     (vBackup.z * (*m)[col][2]) +\
     (vBackup.w * (*m)[col][3])) / S3L_FRACTIONS_PER_UNIT

  v->x = dotCol(0);
  v->y = dotCol(1);
  v->z = dotCol(2);
  v->w = dotCol(3);
}

void S3L_vec3Xmat4(S3L_Vec4 *v, S3L_Mat4 *m)
{
  S3L_Vec4 vBackup;

  #undef dotCol
  #define dotCol(col)\
    (vBackup.x * (*m)[col][0]) / S3L_FRACTIONS_PER_UNIT +\
    (vBackup.y * (*m)[col][1]) / S3L_FRACTIONS_PER_UNIT +\
    (vBackup.z * (*m)[col][2]) / S3L_FRACTIONS_PER_UNIT +\
    (*m)[col][3]

  vBackup.x = v->x;  
  vBackup.y = v->y;  
  vBackup.z = v->z;  
  vBackup.w = v->w;  

  v->x = dotCol(0);
  v->y = dotCol(1);
  v->z = dotCol(2);
  v->w = S3L_FRACTIONS_PER_UNIT;
}

#undef dotCol

S3L_Unit S3L_abs(S3L_Unit value)
{
  return value >= 0 ? value : -1 * value;
}

S3L_Unit S3L_min(S3L_Unit v1, S3L_Unit v2)
{
  return v1 >= v2 ? v2 : v1;
}

S3L_Unit S3L_max(S3L_Unit v1, S3L_Unit v2)
{
  return v1 >= v2 ? v1 : v2;
}

S3L_Unit S3L_clamp(S3L_Unit v, S3L_Unit v1, S3L_Unit v2)
{
  return v >= v1 ? (v <= v2 ? v : v2) : v1;
}

S3L_Unit S3L_wrap(S3L_Unit value, S3L_Unit mod)
{
  return value >= 0 ? (value % mod) : (mod + (value % mod) - 1);
}

S3L_Unit S3L_nonZero(S3L_Unit value)
{
  return value != 0 ? value : 1;
}

S3L_Unit S3L_interpolate(S3L_Unit v1, S3L_Unit v2, S3L_Unit t, S3L_Unit tMax)
{
  return v1 + ((v2 - v1) * t) / tMax;
}

S3L_Unit S3L_interpolateByUnit(S3L_Unit v1, S3L_Unit v2, S3L_Unit t)
{
  return v1 + ((v2 - v1) * t) / S3L_FRACTIONS_PER_UNIT;
}

S3L_Unit S3L_interpolateByUnitFrom0(S3L_Unit v2, S3L_Unit t)
{
  return (v2 * t) / S3L_FRACTIONS_PER_UNIT;
}

S3L_Unit S3L_interpolateFrom0(S3L_Unit v2, S3L_Unit t, S3L_Unit tMax)
{
  return (v2 * t) / tMax;
}

S3L_Unit S3L_distanceManhattan(S3L_Vec4 a, S3L_Vec4 b)
{
  return
    S3L_abs(a.x - b.x) +
    S3L_abs(a.y - b.y) +
    S3L_abs(a.z - b.z);
}

void S3L_mat4Xmat4(S3L_Mat4 *m1, S3L_Mat4 *m2)
{
  S3L_Mat4 mat1;

  for (uint16_t row = 0; row < 4; ++row)
    for (uint16_t col = 0; col < 4; ++col)
      mat1[col][row] = (*m1)[col][row];

  for (uint16_t row = 0; row < 4; ++row)
    for (uint16_t col = 0; col < 4; ++col)
    {
      (*m1)[col][row] = 0;

      for (uint16_t i = 0; i < 4; ++i)
        (*m1)[col][row] +=
          (mat1[i][row] * (*m2)[col][i]) / S3L_FRACTIONS_PER_UNIT;
    }
}

S3L_Unit S3L_sin(S3L_Unit x)
{
  x = S3L_wrap(x / S3L_SIN_TABLE_UNIT_STEP,S3L_SIN_TABLE_LENGTH * 4);
  int8_t positive = 1;

  if (x < S3L_SIN_TABLE_LENGTH)
    x = x;
  else if (x < S3L_SIN_TABLE_LENGTH * 2)
    x = S3L_SIN_TABLE_LENGTH * 2 - x - 1;
  else if (x < S3L_SIN_TABLE_LENGTH * 3)
  {
    x = x - S3L_SIN_TABLE_LENGTH * 2;
    positive = 0;
  }
  else
  {
    x = S3L_SIN_TABLE_LENGTH - (x - S3L_SIN_TABLE_LENGTH * 3) - 1;
    positive = 0;
  }

  return positive ? S3L_sinTable[x] : -1 * S3L_sinTable[x];
}

S3L_Unit S3L_asin(S3L_Unit x)
{
  x = S3L_clamp(x,-S3L_FRACTIONS_PER_UNIT,S3L_FRACTIONS_PER_UNIT);

  int8_t sign = 1;

  if (x < 0)
  {
    sign = -1;
    x *= -1;
  }

  int16_t low = 0;
  int16_t high = S3L_SIN_TABLE_LENGTH -1;
  int16_t middle;

  while (low <= high) // binary search
  {
    middle = (low + high) / 2;

    S3L_Unit v = S3L_sinTable[middle];

    if (v > x)
      high = middle - 1;
    else if (v < x)
      low = middle + 1;
    else
      break;
  }

  middle *= S3L_SIN_TABLE_UNIT_STEP;

  return sign * middle;
}

S3L_Unit S3L_cos(S3L_Unit x)
{
  return S3L_sin(x - S3L_FRACTIONS_PER_UNIT / 4);
}

void S3L_makeTranslationMat(
  S3L_Unit offsetX,
  S3L_Unit offsetY,
  S3L_Unit offsetZ,
  S3L_Mat4 *m)
{
  #define M(x,y) (*m)[x][y]
  #define S S3L_FRACTIONS_PER_UNIT

  M(0,0) = S; M(1,0) = 0; M(2,0) = 0; M(3,0) = 0; 
  M(0,1) = 0; M(1,1) = S; M(2,1) = 0; M(3,1) = 0; 
  M(0,2) = 0; M(1,2) = 0; M(2,2) = S; M(3,2) = 0; 
  M(0,3) = offsetX; M(1,3) = offsetY; M(2,3) = offsetZ; M(3,3) = S;

  #undef M
  #undef S
}

void S3L_makeScaleMatrix(
  S3L_Unit scaleX,
  S3L_Unit scaleY,
  S3L_Unit scaleZ,
  S3L_Mat4 *m)
{
  #define M(x,y) (*m)[x][y]

  M(0,0) = scaleX; M(1,0) = 0;      M(2,0) = 0;     M(3,0) = 0; 
  M(0,1) = 0;      M(1,1) = scaleY; M(2,1) = 0; M(3,1) = 0; 
  M(0,2) = 0;      M(1,2) = 0;      M(2,2) = scaleZ; M(3,2) = 0; 
  M(0,3) = 0;      M(1,3) = 0;     M(2,3) = 0; M(3,3) = S3L_FRACTIONS_PER_UNIT; 

  #undef M
}

void S3L_makeRotationMatrixZXY(
  S3L_Unit byX,
  S3L_Unit byY,
  S3L_Unit byZ,
  S3L_Mat4 *m)
{
  S3L_Unit sx = S3L_sin(byX);
  S3L_Unit sy = S3L_sin(byY);
  S3L_Unit sz = S3L_sin(byZ);

  S3L_Unit cx = S3L_cos(byX);
  S3L_Unit cy = S3L_cos(byY);
  S3L_Unit cz = S3L_cos(byZ);

  #define M(x,y) (*m)[x][y]
  #define S S3L_FRACTIONS_PER_UNIT

  M(0,0) = (cy * cz) / S + (sy * sx * sz) / (S * S);
  M(1,0) = (cx * sz) / S;
  M(2,0) = (cy * sx * sz) / (S * S) - (cz * sy) / S;
  M(3,0) = 0;

  M(0,1) = (cz * sy * sx) / (S * S) - (cy * sz) / S;
  M(1,1) = (cx * cz) / S;
  M(2,1) = (cy * cz * sx) / (S * S) + (sy * sz) / S;
  M(3,1) = 0;

  M(0,2) = (cx * sy) / S;
  M(1,2) = -1 * sx;
  M(2,2) = (cy * cx) / S;
  M(3,2) = 0;

  M(0,3) = 0;
  M(1,3) = 0;
  M(2,3) = 0;
  M(3,3) = S3L_FRACTIONS_PER_UNIT;

  #undef M
  #undef S 
}

S3L_Unit S3L_sqrt(S3L_Unit value)
{
  int8_t sign = 1;

  if (value < 0)
  {
    sign = -1;
    value *= -1;
  }

  uint32_t result = 0;
  uint32_t a = value;
  uint32_t b = 1u << 30;

  while (b > a)
    b >>= 2;

  while (b != 0)
  {
    if (a >= result + b)
    {
      a -= result + b;
      result = result +  2 * b;
    }

    b >>= 2;
    result >>= 1;
  }

  return result * sign;
}

S3L_Unit S3L_vec3Length(S3L_Vec4 v)
{
  return S3L_sqrt(v.x * v.x + v.y * v.y + v.z * v.z);  
}

S3L_Unit S3L_vec2Length(S3L_Vec4 v)
{
  return S3L_sqrt(v.x * v.x + v.y * v.y);  
}

void S3L_normalizeVec3(S3L_Vec4 *v)
{
  S3L_Unit l = S3L_vec3Length(*v);

  if (l == 0)
    return;

  v->x = (v->x * S3L_FRACTIONS_PER_UNIT) / l;
  v->y = (v->y * S3L_FRACTIONS_PER_UNIT) / l;
  v->z = (v->z * S3L_FRACTIONS_PER_UNIT) / l;
}

void S3L_normalizeVec2(S3L_Vec4 *v)
{
  S3L_Unit l = S3L_vec2Length(*v);

  if (l == 0)
    return;

  v->x = (v->x * S3L_FRACTIONS_PER_UNIT) / l;
  v->y = (v->y * S3L_FRACTIONS_PER_UNIT) / l;
}

void S3L_initTransoform3D(S3L_Transform3D *t)
{
  S3L_initVec4(&(t->translation));
  S3L_initVec4(&(t->rotation));
  t->scale.x = S3L_FRACTIONS_PER_UNIT;
  t->scale.y = S3L_FRACTIONS_PER_UNIT;
  t->scale.z = S3L_FRACTIONS_PER_UNIT;
}

void S3L_lookAt(S3L_Vec4 pointFrom, S3L_Vec4 pointTo, S3L_Transform3D *t)
{
  S3L_Vec4 v;

  v.x = pointTo.x - pointFrom.x;
  v.y = pointTo.z - pointFrom.z;

  S3L_Unit dx = v.x;
  S3L_Unit l = S3L_vec2Length(v);

  dx = (v.x * S3L_FRACTIONS_PER_UNIT) / S3L_nonZero(l); // normalize

  t->rotation.y = S3L_asin(dx);

  if (v.y < 0)
    t->rotation.y = S3L_FRACTIONS_PER_UNIT / 2 - t->rotation.y;

  v.x = pointTo.y - pointFrom.y;
  v.y = l;
 
  l = S3L_vec2Length(v);
 
  dx = (v.x * S3L_FRACTIONS_PER_UNIT) / S3L_nonZero(l);

  t->rotation.x = S3L_asin(dx);
}

void S3L_setTransform3D(
  S3L_Unit tx,
  S3L_Unit ty,
  S3L_Unit tz,
  S3L_Unit rx,
  S3L_Unit ry,
  S3L_Unit rz,
  S3L_Unit sx,
  S3L_Unit sy,
  S3L_Unit sz,
  S3L_Transform3D *t)
{
  t->translation.x = tx;
  t->translation.y = ty;
  t->translation.z = tz;

  t->rotation.x = rx;
  t->rotation.y = ry;
  t->rotation.z = rz;

  t->scale.x = sx;
  t->scale.y = sy;
  t->scale.z = sz;
}

void S3L_initCamera(S3L_Camera *camera)
{
  camera->focalLength = S3L_FRACTIONS_PER_UNIT;
  S3L_initTransoform3D(&(camera->transform));
}

void S3L_rotationToDirections(
  S3L_Vec4 rotation,
  S3L_Unit length,
  S3L_Vec4 *forw, 
  S3L_Vec4 *right,
  S3L_Vec4 *up)
{
  S3L_Mat4 m;

  S3L_makeRotationMatrixZXY(-1 * rotation.x,-1 * rotation.y,-1 * rotation.z,&m);

  if (forw != 0)
  {
    forw->x = 0;
    forw->y = 0;
    forw->z = length;
    S3L_vec3Xmat4(forw,&m);
  }

  if (right != 0)
  {
    right->x = length;
    right->y = 0;
    right->z = 0;
    S3L_vec3Xmat4(right,&m);
  }

  if (up != 0)
  {
    up->x = 0;
    up->y = length;
    up->z = 0;
    S3L_vec3Xmat4(up,&m);
  }
}

void S3L_initPixelInfo(S3L_PixelInfo *p) // TODO: maybe non-pointer for p
{                                        // could be faster?
  p->x = 0;
  p->y = 0;
  p->barycentric[0] = S3L_FRACTIONS_PER_UNIT;
  p->barycentric[1] = 0;
  p->barycentric[2] = 0;
  p->triangleIndex = 0;
  p->depth = 0;
  p->previousZ = 0;
}

void S3L_initModel3D(
  S3L_Unit *vertices,
  S3L_Unit vertexCount,
  S3L_Index *triangles,
  S3L_Index triangleCount,
  S3L_Model3D *model)
{
  model->vertices = vertices;
  model->vertexCount = vertexCount;
  model->triangles = triangles;
  model->triangleCount = triangleCount;
  
  S3L_initTransoform3D(&(model->transform));
  S3L_initDrawConfig(&(model->config));
}

void S3L_initScene(
  S3L_Model3D *models,
  S3L_Index modelCount,
  S3L_Scene *scene)
{
  scene->models = models;
  scene->modelCount = modelCount;
  S3L_initCamera(&(scene->camera));
}

void S3L_initDrawConfig(S3L_DrawConfig *config)
{
  config->backfaceCulling = 2;
  config->visible = 1;
}

static inline void S3L_PIXEL_FUNCTION(S3L_PixelInfo *pixel); // forward decl

typedef struct
{
  int16_t steps;
  int16_t err;
  S3L_ScreenCoord x;
  S3L_ScreenCoord y;

  int16_t *majorCoord;
  int16_t *minorCoord;
  int16_t majorIncrement;
  int16_t minorIncrement;
  int16_t majorDiff;
  int16_t minorDiff; 
} S3L_BresenhamState; ///< State of drawing a line with Bresenham algorithm.

typedef struct
{
  S3L_Unit valueScaled;
  S3L_Unit stepScaled;
} S3L_FastLerpState;

#define S3L_getFastLerpValue(state)\
  (state.valueScaled >> S3L_FAST_LERP_QUALITY)

#define S3L_stepFastLerp(state)\
  state.valueScaled += state.stepScaled

static inline S3L_Unit S3L_interpolateBarycentric(
  S3L_Unit value0,
  S3L_Unit value1,
  S3L_Unit value2,
  S3L_Unit barycentric0,
  S3L_Unit barycentric1,
  S3L_Unit barycentric2)
{
  return
    (
      (value0 * barycentric0) +
      (value1 * barycentric1) +
      (value2 * barycentric2)
    ) / S3L_FRACTIONS_PER_UNIT;
}

void S3L_bresenhamInit(S3L_BresenhamState *state, int16_t x0, int16_t y0,
  int16_t x1, int16_t y1)
{
  int16_t dx = x1 - x0;
  int16_t dy = y1 - y0;

  int16_t absDx = S3L_abs(dx);
  int16_t absDy = S3L_abs(dy);

  if (absDx >= absDy)
  {
    state->majorCoord = &(state->x);
    state->minorCoord = &(state->y);

    state->minorDiff = 2 * absDy;
    state->majorDiff = 2 * absDx;
    state->err = 2 * dy - dx;
  
    state->majorIncrement = dx >= 0 ? 1 : -1;
    state->minorIncrement = dy >= 0 ? 1 : -1;

    state->steps = absDx;
  }
  else
  {
    state->majorCoord = &(state->y);
    state->minorCoord = &(state->x);

    state->minorDiff = 2 * absDx;
    state->majorDiff = 2 * absDy;
    state->err = 2 * dx - dy;

    state->majorIncrement = dy >= 0 ? 1 : -1;
    state->minorIncrement = dx >= 0 ? 1 : -1;

    state->steps = absDy;
  }

  state->x = x0;
  state->y = y0;
}

int S3L_bresenhamStep(S3L_BresenhamState *state)
{
  state->steps--;

  (*state->majorCoord) += state->majorIncrement;

  if (state->err > 0)
  {
    (*state->minorCoord) += state->minorIncrement;
    state->err -= state->majorDiff;
  }

  state->err += state->minorDiff;

  return state->steps >= 0;
}

void S3L_mapProjectionPlaneToScreen(
  S3L_Vec4 point,
  S3L_ScreenCoord *screenX,
  S3L_ScreenCoord *screenY)
{
  *screenX = 
    S3L_HALF_RESOLUTION_X +
    (point.x * S3L_HALF_RESOLUTION_X) / S3L_FRACTIONS_PER_UNIT;

  *screenY = 
    S3L_HALF_RESOLUTION_Y -
    (point.y * S3L_HALF_RESOLUTION_X) / S3L_FRACTIONS_PER_UNIT;
}

void S3L_zBufferClear()
{
#if S3L_Z_BUFFER
  for (uint32_t i = 0; i < S3L_RESOLUTION_X * S3L_RESOLUTION_Y; ++i)
    S3L_zBuffer[i] = S3L_MAX_DEPTH;
#endif
}

void S3L_stencilBufferClear()
{
#if S3L_STENCIL_BUFFER
  for (uint32_t i = 0; i < S3L_STENCIL_BUFFER_SIZE; ++i)
    S3L_stencilBuffer[i] = 0;
#endif
}

void S3L_newFrame()
{
  S3L_zBufferClear();
  S3L_stencilBufferClear();
}

void S3L_drawTriangle(
  S3L_Vec4 point0,
  S3L_Vec4 point1,
  S3L_Vec4 point2,
  const S3L_DrawConfig *config,
  const S3L_Camera *camera,
  S3L_Index modelIndex,
  S3L_Index triangleIndex)
{
  S3L_PixelInfo p;
  S3L_initPixelInfo(&p);
  p.modelIndex = modelIndex;
  p.triangleIndex = triangleIndex;

#if !S3L_STRICT_NEAR_CULLING
  point0.z = point0.z >= S3L_NEAR ? point0.z : S3L_NEAR;
  point1.z = point1.z >= S3L_NEAR ? point1.z : S3L_NEAR;
  point2.z = point2.z >= S3L_NEAR ? point2.z : S3L_NEAR;
#endif

  S3L_Vec4 *tPointPP, *lPointPP, *rPointPP; /* points in projction plane space
                                               (in Units, normalized by
                                               S3L_FRACTIONS_PER_UNIT) */

  S3L_ScreenCoord x0, y0, x1, y1, x2, y2;   /* points in screen space (pixel
                                               coordinates) */

  S3L_mapProjectionPlaneToScreen(point0,&x0,&y0);
  S3L_mapProjectionPlaneToScreen(point1,&x1,&y1);
  S3L_mapProjectionPlaneToScreen(point2,&x2,&y2);

  S3L_ScreenCoord
    tPointSx, tPointSy,   // top point coords, in screen space
    lPointSx, lPointSy,   // left point coords, in screen space
    rPointSx, rPointSy;   // right point coords, in screen space

  S3L_Unit *barycentric0; // bar. coord that gets higher from L to R
  S3L_Unit *barycentric1; // bar. coord that gets higher from R to L
  S3L_Unit *barycentric2; // bar. coord that gets higher from bottom up

  // sort the points:

  #define assignPoints(t,a,b)\
    {\
      tPointSx = x##t;\
      tPointSy = y##t;\
      tPointPP = &point##t;\
      barycentric2 = &(p.barycentric[t]);\
      int32_t aDx = x##a - x##t;\
      int32_t bDx = x##b - x##t;\
      int32_t aDy = S3L_nonZero(y##a - y##t);\
      int32_t bDy = S3L_nonZero(y##b - y##t);\
      if ((aDx << 8) / aDy < (bDx << 8) / bDy)\
      {\
        lPointSx = x##a; lPointSy = y##a;\
        rPointSx = x##b; rPointSy = y##b;\
        lPointPP = &point##a; rPointPP = &point##b;\
        barycentric0 = &(p.barycentric[b]);\
        barycentric1 = &(p.barycentric[a]);\
      }\
      else\
      {\
        lPointSx = x##b; lPointSy = y##b;\
        rPointSx = x##a; rPointSy = y##a;\
        lPointPP = &point##b; rPointPP = &point##a;\
        barycentric0 = &(p.barycentric[a]);\
        barycentric1 = &(p.barycentric[b]);\
      }\
    }

  if (y0 <= y1)
  {
    if (y0 <= y2)
      assignPoints(0,1,2)
    else
      assignPoints(2,0,1)
  }
  else
  {
    if (y1 <= y2)
      assignPoints(1,0,2)
    else
      assignPoints(2,0,1)
  }

  #undef assignPoints

#if S3L_FLAT
  p.depth = (tPointPP->z + lPointPP->z + rPointPP->z) / 3;
  *barycentric0 = S3L_FRACTIONS_PER_UNIT / 3;
  *barycentric1 = S3L_FRACTIONS_PER_UNIT / 3;
  *barycentric2 = S3L_FRACTIONS_PER_UNIT - 2 * (S3L_FRACTIONS_PER_UNIT / 3);
#endif

  p.triangleSize[0] = rPointSx - lPointSx;
  p.triangleSize[1] = (rPointSy > lPointSy ? rPointSy : lPointSy) - tPointSy;

  // now draw the triangle line by line:

  S3L_ScreenCoord splitY; // Y of the vertically middle point of the triangle
  S3L_ScreenCoord endY;   // bottom Y of the whole triangle
  int splitOnLeft;        /* whether splitY is the y coord. of left or right 
                             point */

  if (rPointSy <= lPointSy)
  {
    splitY = rPointSy;
    splitOnLeft = 0;
    endY = lPointSy;
  }
  else
  {
    splitY = lPointSy;
    splitOnLeft = 1;
    endY = rPointSy;
  }

  S3L_ScreenCoord currentY = tPointSy;

  /* We'll be using an algorithm similar to Bresenham line algorithm. The
     specifics of this algorithm are among others:

     - drawing possibly a NON-CONTINUOUS line
     - NOT tracing the line exactly, but rather rasterizing one the right
       side of it, according to the pixel CENTERS, INCLUDING the pixel
       centers
     
     The principle is this:

     - Move vertically by pixels and accumulate the error (abs(dx/dy)).
     - If the error is greater than one (crossed the next pixel center), keep
       moving horizontally and substracting 1 from the error until it is less
       than 1 again.
     - To make this INTEGER ONLY, scale the case so that distance between
       pixels is equal to dy (instead of 1). This way the error becomes
       dx/dy * dy == dx, and we're comparing the error to (and potentially
       substracting) 1 * dy == dy. */

  int16_t
    /* triangle side:
    left     right */
    lX,      rX,       // current x position on the screen
    lDx,     rDx,      // dx (end point - start point)
    lDy,     rDy,      // dy (end point - start point)
    lInc,    rInc,     // direction in which to increment (1 or -1)
    lErr,    rErr,     // current error (Bresenham)
    lErrCmp, rErrCmp,  // helper for deciding comparison (> vs >=)
    lErrAdd, rErrAdd,  // error value to add in each Bresenham cycle
    lErrSub, rErrSub;  // error value to substract when moving in x direction

  S3L_FastLerpState lSideFLS, rSideFLS;

#if S3L_COMPUTE_LERP_DEPTH
  S3L_FastLerpState lDepthFLS, rDepthFLS;

  #define initDepthFLS(s,p1,p2)\
    s##DepthFLS.valueScaled = p1##PointPP->z << S3L_FAST_LERP_QUALITY;\
    s##DepthFLS.stepScaled = ((p2##PointPP->z << S3L_FAST_LERP_QUALITY) -\
      s##DepthFLS.valueScaled) / (s##Dy != 0 ? s##Dy : 1); 
#else
  #define initDepthFLS(s,p1,p2) ;
#endif

  /* init side for the algorithm, params:
     s - which side (l or r)
     p1 - point from (t, l or r)
     p2 - point to (t, l or r)
     down - whether the side coordinate goes top-down or vice versa */
  #define initSide(s,p1,p2,down)\
    s##X = p1##PointSx;\
    s##Dx = p2##PointSx - p1##PointSx;\
    s##Dy = p2##PointSy - p1##PointSy;\
    initDepthFLS(s,p1,p2)\
    s##SideFLS.stepScaled = (S3L_FRACTIONS_PER_UNIT << S3L_FAST_LERP_QUALITY)\
                      / (s##Dy != 0 ? s##Dy : 1);\
    s##SideFLS.valueScaled = 0;\
    if (!down)\
    {\
      s##SideFLS.valueScaled = S3L_FRACTIONS_PER_UNIT << S3L_FAST_LERP_QUALITY;\
      s##SideFLS.stepScaled *= -1;\
    }\
    s##Inc = s##Dx >= 0 ? 1 : -1;\
    if (s##Dx < 0)\
      {s##Err = 0;     s##ErrCmp = 0;}\
    else\
      {s##Err = s##Dy; s##ErrCmp = 1;}\
    s##ErrAdd = S3L_abs(s##Dx);\
    s##ErrSub = s##Dy != 0 ? s##Dy : 1; /* don't allow 0, could lead to an
                                           infinite substracting loop */

  #define stepSide(s)\
    while (s##Err - s##Dy >= s##ErrCmp)\
    {\
      s##X += s##Inc;\
      s##Err -= s##ErrSub;\
    }\
    s##Err += s##ErrAdd;

  initSide(r,t,r,1)
  initSide(l,t,l,1)

#if S3L_PERSPECTIVE_CORRECTION == 1
  /* PC is done by linearly interpolating reciprocals from which the corrected
     velues can be computed. See
     http://www.lysator.liu.se/~mikaelk/doc/perspectivetexture/ */

  S3L_Unit
    tPointRecipZ, lPointRecipZ, rPointRecipZ, /* Reciprocals of the depth of 
                                                 each triangle point. */
    lRecip0, lRecip1, rRecip0, rRecip1;       /* Helper variables for swapping
                                                 the above after split. */

  tPointRecipZ = (S3L_FRACTIONS_PER_UNIT * S3L_FRACTIONS_PER_UNIT)
    / S3L_nonZero(tPointPP->z);

  lPointRecipZ = (S3L_FRACTIONS_PER_UNIT * S3L_FRACTIONS_PER_UNIT)
    / S3L_nonZero(lPointPP->z);

  rPointRecipZ = (S3L_FRACTIONS_PER_UNIT * S3L_FRACTIONS_PER_UNIT)
    / S3L_nonZero(rPointPP->z);

  lRecip0 = tPointRecipZ;
  lRecip1 = lPointRecipZ;
  rRecip0 = tPointRecipZ;
  rRecip1 = rPointRecipZ;

  #define manageSplitPerspective(b0,b1)\
    b1##Recip0 = b0##PointRecipZ;\
    b1##Recip1 = b1##PointRecipZ;\
    b0##Recip0 = b0##PointRecipZ;\
    b0##Recip1 = tPointRecipZ;
#else
  #define manageSplitPerspective(b0,b1) ;
#endif

  // clip to the screen in y dimension:

  endY = S3L_min(endY,S3L_RESOLUTION_Y);

  /* Clipping above the screen (y < 0) can't be easily done here, will be
     handled inside the loop. */

  while (currentY < endY)   /* draw the triangle from top to bottom -- the
                               bottom-most row is left out because, following
                               from the rasterization rules (see top of the
                               source), it is to never be rasterized. */
  {
    if (currentY == splitY) // reached a vertical split of the triangle?
    {
      #define manageSplit(b0,b1,s0,s1)\
        S3L_Unit *tmp = barycentric##b0;\
        barycentric##b0 = barycentric##b1;\
        barycentric##b1 = tmp;\
        s0##SideFLS.valueScaled = (S3L_FRACTIONS_PER_UNIT\
           << S3L_FAST_LERP_QUALITY) - s0##SideFLS.valueScaled;\
        s0##SideFLS.stepScaled *= -1;\
        manageSplitPerspective(s0,s1)

      if (splitOnLeft)
      {
        initSide(l,l,r,0);
        manageSplit(0,2,r,l)
      }
      else
      {
        initSide(r,r,l,0);
        manageSplit(1,2,l,r)
      }
    }

    stepSide(r)
    stepSide(l)

    if (currentY >= 0) /* clipping of pixels whose y < 0 (can't be easily done
                          outside the loop) */
    {                     /* TODO: ^ This is bad though, a single large
                             triangle outside he top of the screen will trigger
                             a long loop. Try to FIX THIS! */
      p.y = currentY;

      // draw the horizontal line

#if !S3L_FLAT
      S3L_Unit rowLength = S3L_nonZero(rX - lX - 1); // prevent zero div

  #if S3L_PERSPECTIVE_CORRECTION == 1
      S3L_Unit lOverZ, lRecipZ, rOverZ, rRecipZ, lT, rT;

      lT = S3L_getFastLerpValue(lSideFLS);
      rT = S3L_getFastLerpValue(rSideFLS);

      lOverZ  = S3L_interpolateByUnitFrom0(lRecip1,lT);
      lRecipZ = S3L_interpolateByUnit(lRecip0,lRecip1,lT);

      rOverZ  = S3L_interpolateByUnitFrom0(rRecip1,rT);
      rRecipZ = S3L_interpolateByUnit(rRecip0,rRecip1,rT);
  #else
      S3L_FastLerpState b0FLS, b1FLS;

    #if S3L_COMPUTE_LERP_DEPTH
      S3L_FastLerpState  depthFLS;

      depthFLS.valueScaled = lDepthFLS.valueScaled;
      depthFLS.stepScaled =
        (rDepthFLS.valueScaled - lDepthFLS.valueScaled) / rowLength;
    #endif

      b0FLS.valueScaled = 0;
      b1FLS.valueScaled = lSideFLS.valueScaled;

      b0FLS.stepScaled = rSideFLS.valueScaled / rowLength;
      b1FLS.stepScaled = -1 * lSideFLS.valueScaled / rowLength;
  #endif
#endif

      // clip to the screen in x dimension:

      S3L_ScreenCoord rXClipped = S3L_min(rX,S3L_RESOLUTION_X),
                      lXClipped = lX;

      if (lXClipped < 0)
      {
        lXClipped = 0;

#if S3L_PERSPECTIVE_CORRECTION != 1 && !S3L_FLAT
        b0FLS.valueScaled -= lX * b0FLS.stepScaled;
        b1FLS.valueScaled -= lX * b1FLS.stepScaled;

  #if S3L_COMPUTE_LERP_DEPTH
        depthFLS.valueScaled -= lX * depthFLS.stepScaled;
  #endif
#endif
      }

#if S3L_PERSPECTIVE_CORRECTION == 1
      S3L_ScreenCoord i = lXClipped - lX;  /* helper var to save one
                                                  substraction in the inner
                                                  loop */
#endif

      if (rXClipped < lXClipped &&
          lXClipped < S3L_RESOLUTION_X && rXClipped >= 0)
      {
        /* This can sometimes happen because of numerical errors in sorting
        left vs right triangle point, which are compared based on SLOPE, not
        x coordinates. Here we swap the values to prevents not drawing the
        triagle. */

        S3L_ScreenCoord tmp = rXClipped;
        rXClipped = lXClipped;
        lXClipped = tmp;
      }

      // draw the row -- inner loop:

      for (S3L_ScreenCoord x = lXClipped; x < rXClipped; ++x)
      {
        int8_t testsPassed = 1;

#if S3L_STENCIL_BUFFER
        if (!S3L_stencilTest(x,p.y))
          testsPassed = 0;
#endif
        p.x = x;

#if S3L_COMPUTE_DEPTH
  #if S3L_PERSPECTIVE_CORRECTION == 1
        p.depth = (S3L_FRACTIONS_PER_UNIT * S3L_FRACTIONS_PER_UNIT) /
          S3L_nonZero(S3L_interpolate(lRecipZ,rRecipZ,i,rowLength));
  #else
        p.depth = S3L_getFastLerpValue(depthFLS);
        S3L_stepFastLerp(depthFLS);
  #endif
#endif

#if S3L_Z_BUFFER
        p.previousZ = S3L_zBuffer[p.y * S3L_RESOLUTION_X + p.x];

        if (!S3L_zTest(p.x,p.y,p.depth))
          testsPassed = 0;        
#endif

        if (testsPassed)
        {
#if !S3L_FLAT
  #if S3L_PERSPECTIVE_CORRECTION == 1
          *barycentric0 =
           ( 
             S3L_interpolateFrom0(rOverZ,i,rowLength)
             * p.depth
           ) / S3L_FRACTIONS_PER_UNIT;

          *barycentric1 =
           ( 
             (lOverZ - S3L_interpolateFrom0(lOverZ,i,rowLength))
             * p.depth
           ) / S3L_FRACTIONS_PER_UNIT;
  #else
          *barycentric0 = S3L_getFastLerpValue(b0FLS);
          *barycentric1 = S3L_getFastLerpValue(b1FLS);
  #endif

          *barycentric2 =
            S3L_FRACTIONS_PER_UNIT - *barycentric0 - *barycentric1;
#endif

          S3L_PIXEL_FUNCTION(&p);
        }

#if !S3L_FLAT
  #if S3L_PERSPECTIVE_CORRECTION == 1
           i++;
  #else
          S3L_stepFastLerp(b0FLS);
          S3L_stepFastLerp(b1FLS);
  #endif
#endif

      }  // inner loop
    }  // y clipping

    S3L_stepFastLerp(lSideFLS);
    S3L_stepFastLerp(rSideFLS);

#if S3L_COMPUTE_LERP_DEPTH
    S3L_stepFastLerp(lDepthFLS);
    S3L_stepFastLerp(rDepthFLS);
#endif

    ++currentY;
  }

  #undef manageSplit
  #undef initPC
  #undef initSide
  #undef stepSide 
}

void S3L_rotate2DPoint(S3L_Unit *x, S3L_Unit *y, S3L_Unit angle)
{
  if (angle < S3L_SIN_TABLE_UNIT_STEP)
    return; // no visible rotation

  S3L_Unit angleSin = S3L_sin(angle);
  S3L_Unit angleCos = S3L_cos(angle);

  S3L_Unit xBackup = *x;

  *x =
    (angleCos * (*x)) / S3L_FRACTIONS_PER_UNIT -
    (angleSin * (*y)) / S3L_FRACTIONS_PER_UNIT;

  *y =
    (angleSin * xBackup) / S3L_FRACTIONS_PER_UNIT +
    (angleCos * (*y)) / S3L_FRACTIONS_PER_UNIT;
}

void S3L_makeWorldMatrix(S3L_Transform3D worldTransform, S3L_Mat4 *m)
{
  S3L_makeScaleMatrix(
    worldTransform.scale.x,
    worldTransform.scale.y,
    worldTransform.scale.z,
    m
  );

  S3L_Mat4 t;

  S3L_makeRotationMatrixZXY(
    worldTransform.rotation.x,
    worldTransform.rotation.y,
    worldTransform.rotation.z,
    &t);

  S3L_mat4Xmat4(m,&t);

  S3L_makeTranslationMat(
    worldTransform.translation.x,
    worldTransform.translation.y,
    worldTransform.translation.z,
    &t);

  S3L_mat4Xmat4(m,&t);
}

void S3L_transposeMat4(S3L_Mat4 *m)
{
  S3L_Unit tmp;

  for (uint8_t y = 0; y < 3; ++y)
    for (uint8_t x = 1 + y; x < 4; ++x)
    {
      tmp = (*m)[x][y];
      (*m)[x][y] = (*m)[y][x];
      (*m)[y][x] = tmp;
    }
}

void S3L_makeCameraMatrix(S3L_Transform3D cameraTransform, S3L_Mat4 *m)
{
  S3L_makeTranslationMat(
    -1 * cameraTransform.translation.x,
    -1 * cameraTransform.translation.y,
    -1 * cameraTransform.translation.z,
    m);

  S3L_Mat4 r;

  S3L_makeRotationMatrixZXY(
    -1 * cameraTransform.rotation.x,
    -1 * cameraTransform.rotation.y,
    -1 * cameraTransform.rotation.z,
    &r);

  S3L_transposeMat4(&r); // transposing creates an inverse transform

  S3L_mat4Xmat4(m,&r);
}

/**
  Performs perspecive division (z-divide). Does NOT check for division by zero.
*/
static inline void S3L_perspectiveDivide(S3L_Vec4 *vector,
  S3L_Unit focalLength)
{
  vector->x = (vector->x * focalLength) / vector->z;
  vector->y = (vector->y * focalLength) / vector->z;
}

/**
  Checks if given triangle (in Projection Plane space) is at least partially
  visible, i.e. returns false if the triangle is either completely outside
  the frustum (left, right, top, bottom, near) or is invisible due to
  backface culling.
*/
static inline int8_t S3L_triangleIsVisible(
  S3L_Vec4 p0,
  S3L_Vec4 p1,
  S3L_Vec4 p2,
  uint8_t backfaceCulling)
{
  #define clipTest(c,cmp,v)\
    (p0.c cmp (v) && p1.c cmp (v) && p2.c cmp (v))

  if ( // outside frustum?

#if S3L_STRICT_NEAR_CULLING
      p0.z <= S3L_NEAR || p1.z <= S3L_NEAR || p2.z <= S3L_NEAR ||
      // ^ partially in front of NEAR?
#else
      clipTest(z,<=,S3L_NEAR) || // completely in front of NEAR?
#endif
      clipTest(x,<,-1 * S3L_FRACTIONS_PER_UNIT) ||
      clipTest(x,>,S3L_FRACTIONS_PER_UNIT) ||
      clipTest(y,<,-1 * S3L_PROJECTION_PLANE_HEIGHT / 2) ||
      clipTest(y,>,S3L_PROJECTION_PLANE_HEIGHT / 2)
    )
    return 0;

  #undef clipTest

  if (backfaceCulling != 0)
  {
    int32_t winding = // determines CW or CCW
      (p1.y - p0.y) * (p2.x - p1.x) - (p1.x - p0.x) * (p2.y - p1.y); 

    if ((backfaceCulling == 1 && winding < 0) ||
        (backfaceCulling == 2 && winding > 0))
      return 0;
  }

  return 1;
}

#if S3L_SORT != 0
typedef struct
{
  uint8_t modelIndex;
  S3L_Index triangleIndex;
  uint16_t sortValue;
} S3L_TriangleToSort;

S3L_TriangleToSort S3L_sortArray[S3L_MAX_TRIANGES_DRAWN];
uint16_t S3L_sortArrayLength;
#endif

void _S3L_projectVertex(
  const S3L_Model3D *model,
  S3L_Index triangleIndex,
  uint8_t vertex,
  S3L_Mat4 *projectionMatrix, 
  S3L_Vec4 *result,
  S3L_Unit focalLength)
{
  S3L_Index vertexIndex = model->triangles[triangleIndex * 3 + vertex] * 3;

  result->x = model->vertices[vertexIndex];
  result->y = model->vertices[vertexIndex + 1];
  result->z = model->vertices[vertexIndex + 2];
  result->w = S3L_FRACTIONS_PER_UNIT; // for translation 
 
  S3L_vec3Xmat4(result,projectionMatrix);
 
  result->z = result->z >= S3L_NEAR ? result->z : S3L_NEAR;
  /* ^ This firstly prevents zero division in the follwoing z-divide and
    secondly "pushes" vertices that are in front of near a little bit forward,
    which makes the behave a bit better. If all three vertices end up exactly
    on NEAR, the triangle will be culled. */ 

  S3L_perspectiveDivide(result,focalLength);
}

void S3L_drawScene(S3L_Scene scene)
{
  S3L_Mat4 matFinal, matCamera;
  S3L_Vec4 transformed0, transformed1, transformed2;
  const S3L_Model3D *model;
  S3L_Index modelIndex, triangleIndex;

  S3L_makeCameraMatrix(scene.camera.transform,&matCamera);

#if S3L_SORT != 0
  uint16_t previousModel = 0;
  S3L_sortArrayLength = 0;
#endif

  for (modelIndex = 0; modelIndex < scene.modelCount; ++modelIndex)
  {
    if (!scene.models[modelIndex].config.visible)
      continue;

#if S3L_SORT != 0
    if (S3L_sortArrayLength >= S3L_MAX_TRIANGES_DRAWN)
      break;

    previousModel = modelIndex;
#endif

    S3L_makeWorldMatrix(scene.models[modelIndex].transform,&matFinal);
    S3L_mat4Xmat4(&matFinal,&matCamera);

    S3L_Index triangleCount = scene.models[modelIndex].triangleCount;

    triangleIndex = 0;
    
    while (triangleIndex < triangleCount)
    {
      model = &(scene.models[modelIndex]);

      /* TODO: maybe create an option that would use a cache here to not
               transform the same point twice? */

      _S3L_projectVertex(model,triangleIndex,0,&matFinal,
        &transformed0,scene.camera.focalLength);

      _S3L_projectVertex(model,triangleIndex,1,&matFinal,
        &transformed1,scene.camera.focalLength);

      _S3L_projectVertex(model,triangleIndex,2,&matFinal,
        &transformed2,scene.camera.focalLength);

      if (S3L_triangleIsVisible(transformed0,transformed1,transformed2,
         model->config.backfaceCulling))
      {
#if S3L_SORT == 0
        // without sorting draw right away
        S3L_drawTriangle(transformed0,transformed1,transformed2,
          &(model->config),&(scene.camera),modelIndex,triangleIndex);
#else
        if (S3L_sortArrayLength >= S3L_MAX_TRIANGES_DRAWN)
          break;

        // with sorting add to a sort list
        S3L_sortArray[S3L_sortArrayLength].modelIndex = modelIndex;
        S3L_sortArray[S3L_sortArrayLength].triangleIndex = triangleIndex;
        S3L_sortArray[S3L_sortArrayLength].sortValue = 
          (transformed0.z + transformed1.z + transformed2.z) >> 2;
        /* ^ As a simple approximation we sort by the triangle center point,
           which is a mean coordinate -- we don't actually have to divide by 3
           (or anything), that is unnecessary for sorting! We shift by 2 just
           as a fast operation to prevent overflow of the sum ver uint_16t. */

        S3L_sortArrayLength++;
#endif
      }

      triangleIndex++;
    }
  }

#if S3L_SORT != 0

  #if S3L_SORT == 1
    #define cmp <
  #else
    #define cmp >
  #endif

  /* Sort the triangles. We use insertion sort, because it has many advantages,
  especially for smaller arrays (better than bubble sort, in-place, stable,
  simple, ...). */

  for (int16_t i = 1; i < S3L_sortArrayLength; ++i)
  {
    S3L_TriangleToSort tmp = S3L_sortArray[i];
 
    int16_t j = i - 1;

    while (j >= 0 && S3L_sortArray[j].sortValue cmp tmp.sortValue)
    {
      S3L_sortArray[j + 1] = S3L_sortArray[j];
      j--;
    }

    S3L_sortArray[j + 1] = tmp;
  }

  #undef cmp

  for (S3L_Index i = 0; i < S3L_sortArrayLength; ++i)
  {
    modelIndex = S3L_sortArray[i].modelIndex;
    triangleIndex = S3L_sortArray[i].triangleIndex;

    model = &(scene.models[modelIndex]);

    if (modelIndex != previousModel)
    {
      // only recompute the matrix when the model has changed
      S3L_makeWorldMatrix(model->transform,&matFinal);
      S3L_mat4Xmat4(&matFinal,&matCamera);
      previousModel = modelIndex;
    }

    /* Here we project the points again, which is redundant and slow as they've
       already been projected above, but saving the projected points would
       require a lot of memory, which for small resolutions could be even
       worse than z-bufer. So this seems to be the best way memory-wise. */
    
    _S3L_projectVertex(model,triangleIndex,0,&matFinal,
      &transformed0,scene.camera.focalLength);

    _S3L_projectVertex(model,triangleIndex,1,&matFinal,
      &transformed1,scene.camera.focalLength);

    _S3L_projectVertex(model,triangleIndex,2,&matFinal,
      &transformed2,scene.camera.focalLength);

    S3L_drawTriangle(transformed0,transformed1,transformed2,
      &(model->config),&(scene.camera),modelIndex,triangleIndex);
  }
#endif
}

#endif
