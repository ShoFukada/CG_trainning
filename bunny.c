#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <GL/gl.h>
/* 初期バッファサイズ */
#define DEF_BUF_SIZE 2
typedef struct {
    float *vertices;
    int num_vertices;
} model;

/* OBJファイルの読み込み */
void load_obj(model *m, const char *file_name);
/* 浮動小数点数バッファ */
typedef struct {
    int buf_size;
    int current_index;
    float *buf;
} float_buffer;

/* 整数バッファ */
typedef struct {
    int buf_size;
    int current_index;
    int *buf;
} int_buffer;

/* 頂点座標の読み込み */
void read_vertices(const char *line, float_buffer *vs);

/* インデックスの読み込み */
void read_indices(const char *line, int_buffer *fs);

/* モデルの作成 */
void create_model(model *m, float_buffer *vs, int_buffer *fs);

/* 浮動小数点数バッファの操作 */
float_buffer *alloc_float_buffer(void);
void free_float_buffer(float_buffer *fbuf);
void add_float(float_buffer *fbuf, float value);
float get_float(float_buffer *fbuf, int index);

/* 整数バッファの操作 */
int_buffer *alloc_int_buffer(void);
void free_int_buffer(int_buffer *ibuf);
void add_int(int_buffer *ibuf, int value);
int get_int(int_buffer *ibuf, int index);

/*==============================*
** OBJファイルの読み込み
**==============================*/
void load_obj(model *m, const char *file_name)
{
    FILE *fp;
    char line[1024];
    float_buffer *vs;
    int_buffer *fs;

    fp = fopen(file_name, "r");
    if (!fp) {
        fprintf(stderr, "Cannot open %s.\n", file_name);
        exit(EXIT_FAILURE);
    }

    vs = alloc_float_buffer();//バッファの初期化、メモリ割り当て
    fs = alloc_int_buffer();

    while (!feof(fp)) {//ファイルポインタの位置がEOFか判定する
        fgets(line, sizeof(line), fp);
        if (line[0] == 'v' && line[1] == ' ') {//頂点座標
            read_vertices(line, vs);//内容をバッファに入れる
        }
        else if (line[0] == 'f' && line[1] == ' ') {//モデルを構成する三角形
            read_indices(line, fs);
        }
    }

    create_model(m, vs, fs);

    free_float_buffer(vs);
    free_int_buffer(fs);

    fclose(fp);
}

/*------------------------------*
** 頂点座標の読み込み
**------------------------------*/
void read_vertices(const char *line, float_buffer *vs)
{
    float x, y, z;
    int count;

    count = sscanf(line, "%*s%f%f%f", &x, &y, &z);

    if (count == 3) {
        add_float(vs, x);
        add_float(vs, y);
        add_float(vs, z);
    }
}

/*------------------------------*
** インデックスの読み込み
**------------------------------*/
void read_indices(const char *line, int_buffer *fs)
{
    int v1, v2, v3;
    int count;

    count = sscanf(line, "%*s%d%d%d", &v1, &v2, &v3);

    if (count == 3) {
        add_int(fs, v1);
        add_int(fs, v2);
        add_int(fs, v3);
    }
}

/*------------------------------*
** モデルの作成
**------------------------------*/
void create_model(model *m, float_buffer *vs, int_buffer *fs)
{
    int i, j;

    m->num_vertices = fs->current_index * 3;
    m->vertices = malloc(sizeof(float) * m->num_vertices);

    if (!m->vertices) {
        fprintf(stderr, "Memory allocation error.\n");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < fs->current_index; i++) {
        int idx = fs->buf[i] - 1;
        for (j = 0; j < 3; j++) {
            m->vertices[i * 3 + j] = vs->buf[idx * 3 + j];
        }
    }
}

/*------------------------------*
** 浮動小数点数バッファの割り当て
**------------------------------*/
float_buffer *alloc_float_buffer(void)
{
    float_buffer *fbuf;

    fbuf = malloc(sizeof(float_buffer));
    if (!fbuf) {
        fprintf(stderr, "Memory allocation error.\n");
        exit(EXIT_FAILURE);
    }

    fbuf->buf_size = DEF_BUF_SIZE;
    fbuf->current_index = 0;
    fbuf->buf = malloc(sizeof(float) * fbuf->buf_size);

    return fbuf;
}

/*------------------------------*
** 浮動小数点数バッファの解放
**------------------------------*/
void free_float_buffer(float_buffer *fbuf)
{
    free(fbuf->buf);
    free(fbuf);
}

/*------------------------------*
** バッファに浮動小数点数を追加
**------------------------------*/
void add_float(float_buffer *fbuf, float value)
{
    fbuf->buf[fbuf->current_index] = value;
    fbuf->current_index++;
    if (fbuf->current_index >= fbuf->buf_size) {
        fbuf->buf_size *= 2;
        fbuf->buf = realloc(fbuf->buf, sizeof(float) * fbuf->buf_size);//メモリを指定した大きさに拡大する
        if (!fbuf->buf) {
            fprintf(stderr, "Memory allocation error.\n");
            exit(EXIT_FAILURE);
        }
    }
}

/*------------------------------*
** バッファから浮動小数点数を取り出し
**------------------------------*/
float get_float(float_buffer *fbuf, int index)
{
    return fbuf->buf[index];
}

/*------------------------------*
** 整数バッファの割り当て
**------------------------------*/
int_buffer *alloc_int_buffer(void)
{
    int_buffer *ibuf;

    ibuf = malloc(sizeof(int_buffer));
    if (!ibuf) {
        fprintf(stderr, "Memory allocation error.\n");
        exit(EXIT_FAILURE);
    }

    ibuf->buf_size = DEF_BUF_SIZE;
    ibuf->current_index = 0;
    ibuf->buf = malloc(sizeof(int) * ibuf->buf_size);

    return ibuf;
}

/*------------------------------*
** 整数バッファの解放
**------------------------------*/
void free_int_buffer(int_buffer *ibuf)
{
    free(ibuf->buf);
    free(ibuf);
}

/*------------------------------*
** バッファに整数を追加
**------------------------------*/
void add_int(int_buffer *ibuf, int value)
{
    ibuf->buf[ibuf->current_index] = value;
    ibuf->current_index++;
    if (ibuf->current_index >= ibuf->buf_size) {
        ibuf->buf_size *= 2;
        ibuf->buf = realloc(ibuf->buf, sizeof(int) * ibuf->buf_size);
        if (!ibuf->buf) {
            fprintf(stderr, "Memory allocation error.\n");
            exit(EXIT_FAILURE);
        }
    }
}

/*------------------------------*
** バッファから整数を取り出し
**------------------------------*/
int get_int(int_buffer *ibuf, int index)
{
    return ibuf->buf[index];
}


model bunny;
const char *file_name = "bunny.obj";


void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0,0.8,1.0);
    glBegin(GL_TRIANGLES);
        for(int i=0;i < bunny.num_vertices/3;i++) {
            glVertex3f(bunny.vertices[3*i],bunny.vertices[3*i+1],bunny.vertices[3*i+2]);
        }
    glEnd();
    glFlush();
}

void init() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);//次の変換が投影行列に適用される
    glLoadIdentity();
    gluPerspective(20.0, 1.0, 1.0, 10.0);
    gluLookAt(1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    // glFrontFace(GL_CCW);
    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_BACK);今回はいらないっぽい

}

int main(int argc, char** argv) {
    load_obj(&bunny, file_name);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB);
    glutCreateWindow(argv[0]);

    init();
    glutDisplayFunc(display);
    // for(int i=0;i<pyramid.num_vertices;i++) {
    //     printf("%f\n",pyramid.vertices[i]);
    // }
    glutMainLoop();
    free(bunny.vertices);
    return 0;
}
