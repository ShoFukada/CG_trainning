#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>
#include <GL/gl.h>
/* 初期バッファサイズ */
#define DEF_BUF_SIZE 2
typedef struct {
    float *vertices;
    int num_vertices;
    float *normals;
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
void create_model(model *m, float_buffer *vs, float_buffer *vn, int_buffer *fs);

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
    float_buffer *vn;
    int_buffer *fs;

    fp = fopen(file_name, "r");
    if (!fp) {
        fprintf(stderr, "Cannot open %s.\n", file_name);
        exit(EXIT_FAILURE);
    }

    vs = alloc_float_buffer();//バッファの初期化、メモリ割り当て
    vn = alloc_float_buffer();
    fs = alloc_int_buffer();

    while (!feof(fp)) {//ファイルポインタの位置がEOFか判定する
        fgets(line, sizeof(line), fp);
        if (line[0] == 'v' && line[1] == ' ') {//頂点座標
            read_vertices(line, vs);//内容をバッファに入れる
        }
        else if (line[0] == 'f' && line[1] == ' ') {//モデルを構成する三角形
            read_indices(line, fs);
        }
        else if (line[0] == 'v' && line[1] == 'n') {
            read_vertices(line, vn);
        }
    }

    create_model(m, vs, vn, fs);

    free_float_buffer(vs);
    free_float_buffer(vn);
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
void create_model(model *m, float_buffer *vs, float_buffer *vn, int_buffer *fs)
{
    int i, j;

    m->num_vertices = fs->current_index * 3;
    m->vertices = malloc(sizeof(float) * m->num_vertices);
    m->normals = malloc(sizeof(float) * m->num_vertices);
    if (!m->vertices) {
        fprintf(stderr, "Memory allocation error.\n");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < fs->current_index; i++) {
        int idx = fs->buf[i] - 1;
        for (j = 0; j < 3; j++) {
            m->vertices[i * 3 + j] = vs->buf[idx * 3 + j];
            m->normals[i * 3 + j] = vn->buf[idx * 3 + j];
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
//以下ベクトル演算


void vec_add(double *result, const double *a, const double *b) {
    result[0] = a[0] + b[0];
    result[1] = a[1] + b[1];
    result[2] = a[2] + b[2];
}
void vec_sub(double *result, const double *a, const double *b) {
    // printf("%f,%f,%f\n",b[0],b[1],b[2]);
    // printf("%f,%f,%f\n",a[0],a[1],a[2]);
    result[0] = a[0] - b[0];
    result[1] = a[1] - b[1];
    result[2] = a[2] - b[2];
}
void vec_scalar_mul(double *result, const double *a, double k) {
    result[0] = k * a[0];
    result[1] = k * a[1];
    result[2] = k * a[2];
}
double vec_length(const double *a) {
    double len = sqrt(a[0]*a[0]+a[1]*a[1]+a[2]*a[2]);
    return len;
}
void vec_normalize(double *result, const double *a) {
    double len = vec_length(a);
    result[0] = a[0]/len;
    result[1] = a[1]/len;
    result[2] = a[2]/len;
}
double vec_dot(const double *a, const double *b) {
    double ans = a[0]*b[0]+a[1]*b[1]+a[2]*b[2];
    return ans;
}
void vec_cross(double *result, const double *a, const double *b) {
    result[0] = a[1]*b[2]-a[2]*b[1];
    result[1] = a[2]*b[0]-a[0]*b[2];
    result[2] = a[0]*b[1]-a[1]*b[0];
}

//以下四元数演算

typedef struct{
    double a;
    double b;//i成分
    double c;//j成分
    double d;//k成分
} Qua;

void showq(Qua v){
    printf("%+8.3f    %+8.3f %+8.3f %+8.3f \n",v.a,v.b,v.c,v.d);
}

Qua qua_add(Qua q1,Qua q2){
    Qua ret;
    ret.a = q1.a + q2.a;
    ret.b = q1.b + q2.b;
    ret.c = q1.c + q2.c;
    ret.d = q1.d + q2.d;
    return ret;
}
//共役の計算
Qua bar(Qua q){
    Qua ret;
    ret.a =  q.a;
    ret.b = -q.b;
    ret.c = -q.c;
    ret.d = -q.d;
    return ret;
}

Qua qua_mul(Qua q1,Qua q2){
    Qua ret;
    double a1=q1.a;     double a2=q2.a;
    double b1=q1.b;     double b2=q2.b;
    double c1=q1.c;     double c2=q2.c;
    double d1=q1.d;     double d2=q2.d;
    ret.a = a1*a2 - b1*b2 - c1*c2 - d1*d2;// 1成分
    ret.b = b1*a2 + a1*b2 - d1*c2 + c1*d2;// i成分
    ret.c = c1*a2 + d1*b2 + a1*c2 - b1*d2;// j成分
    ret.d = d1*a2 - c1*b2 + b1*c2 + a1*d2;// k成分
    return ret;
}
double norm(Qua q){
    double a=q.a;
    double b=q.b;
    double c=q.c;
    double d=q.d;
    return sqrt(a*a+b*b+c*c+d*d);
}
Qua normalize(Qua q){
    double n = norm(q);
    if(n<1e-10){ puts("Error: normalize()  norm is almost 0"); exit(-1); }
    double inv = 1.0/n;
    q.a *= inv;    q.b *= inv;    q.c *= inv;    q.d *= inv;
    return q;
}
//qを計算(回転四元数)
Qua getq(double rad, Qua u){
    Qua ret;
    ret.a = cos(rad/2);
    ret.b = sin(rad/2)*u.b;
    ret.c = sin(rad/2)*u.c;
    ret.d = sin(rad/2)*u.d;
    return ret;
}

//原点を通る任意軸回転
void rot0(double *p0, double *axis, double rad, double *p1){// u:回転軸の方向ベクタ
    Qua u,src;
    u.a  =0;   u.b  =axis[0]; u.c  =axis[1]; u.d  =axis[2];
    src.a=0;   src.b=p0[0];   src.c=p0[1];   src.d=p0[2];
    u = normalize(u);
    Qua q   = getq(rad,u);
    Qua qa  = qua_mul(q,src);
    Qua dst = qua_mul(qa,bar(q));
    p1[0]=dst.b;
    p1[1]=dst.c;
    p1[2]=dst.d;
}

//任意軸回転
void rot(double *p0, double *axis, double *ofs, double rad, double *p1){
    // ofsにはeyeの座標を持ってきて原点に並行移動してから回転し、その後にもどす
    double org[3];    org[0]=p0[0];    org[1]=p0[1];     org[2]=p0[2];
    //平行移動（往路）
    org[0]-=ofs[0];   org[1]-=ofs[1];   org[2]-=ofs[2];
    //原点を通る軸で回転
    rot0(org,axis,rad, p1);
    //平行移動（復路）
    p1[0]+=ofs[0];    p1[1]+=ofs[1];    p1[2]+=ofs[2];
}


model bunny;
const char *file_name = "bunny_smooth.obj";
double aspect;
GLfloat light0pos[] = { 0.0, 3.0, 5.0, 1.0 };//光源の設定(x,y,z,?)
GLfloat light1pos[] = { 0.0, 3.0, 0.0, 1.0 };

GLfloat green[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat red[] = { 1.0, 0.8, 1.0, 1.0 };

double eye[3] = {0.0,0.0,1.0};//カメラ位置の座標
double pov[3] = {0.0,0.0,0.0};//注視点の座標
double up[3] = {0.0,1.0,0.0};//カメラの上向き方向ベクトル(forwardと垂直とは限らない)
double fovh = M_PI / 3;//縦方向視野角
double fovv = M_PI / 3;//横方向視野角　
int mouseButton = -1;
int prevMouseX, prevMouseY;

void updateCamera() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fovh * 180.0 / M_PI, aspect, 0.1, 1000.0);
    gluLookAt(eye[0], eye[1], eye[2], pov[0], pov[1], pov[2], up[0], up[1], up[2]);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, red);//面の表裏、拡散反射係数鏡面反射係数、色
    glBegin(GL_TRIANGLES);
        for(int i=0;i < bunny.num_vertices/3;i++) {
            glNormal3f(bunny.normals[3*i],bunny.normals[3*i+1],bunny.normals[3*i+2]);
            glVertex3f(bunny.vertices[3*i],bunny.vertices[3*i+1],bunny.vertices[3*i+2]);
        }
    glEnd();
    glFlush();
}
void resize(int w, int h)
{
  glViewport(0, 0, w, h);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  aspect = (double)w / (double)h;
  gluPerspective(fovh * 180.0 / M_PI, aspect, 0.1, 1000.0);
  updateCamera();
}



void init() {
    glEnable(GL_DEPTH_TEST);//描画順序を自動的に管理させる、正しい深度関係になる

    glClearColor(0.0, 0.0, 0.0, 1.0);
    updateCamera();

    glFrontFace(GL_CCW);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);//今回はいらないっぽい
    glEnable(GL_LIGHTING);//陰影付を有効にする
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glLightfv(GL_LIGHT0, GL_POSITION, light0pos);
    glLightfv(GL_LIGHT1, GL_POSITION, light1pos);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, green);
    glLightfv(GL_LIGHT1, GL_SPECULAR, green);
}

void mouse(int button, int state, int x, int y) {//押されたときまたは離されたときに実行される
    if(state == GLUT_DOWN) {
        mouseButton = button;
        prevMouseX = x;
        prevMouseY = y;
    } else {
        mouseButton = -1;
    }
}

void motion(int x, int y) {
    int dx = x - prevMouseX;
    int dy = y - prevMouseY;

    if (mouseButton == GLUT_LEFT_BUTTON) {
        double right_norm[3],up_norm[3],forward[3],translation_right[3],translation_up[3];
        vec_normalize(up_norm,up);//上方向ベクトルを正規化
        vec_sub(forward,pov,eye);//カメラから注視点が前方ベクトル
        vec_cross(right_norm,forward,up_norm);
        vec_normalize(right_norm,right_norm);
        vec_scalar_mul(translation_right,right_norm,(double)dx/1000);
        vec_scalar_mul(translation_up,up,(double)dy/1000);
        vec_sub(eye,eye,translation_right);
        vec_sub(pov,pov,translation_right);//カメラ方向と逆に物体が動くためsubにする
        vec_add(eye,eye,translation_up);
        vec_add(pov,pov,translation_up);
    } else if (mouseButton == GLUT_MIDDLE_BUTTON) {
        double forward[3],forward_len,zoom;
        vec_sub(forward,pov,eye);
        forward_len = vec_length(forward);
        zoom = 1 + forward_len*(double)dy/1000;
        vec_scalar_mul(forward,forward,zoom);
        vec_sub(eye,pov,forward);
    } else if (mouseButton == GLUT_RIGHT_BUTTON) {
        double forward[3],up_norm[3],right_norm[3],rad_x,rad_y;
        vec_normalize(up_norm,up);
        vec_sub(forward,pov,eye);
        vec_cross(right_norm,forward,up_norm);
        vec_normalize(right_norm,right_norm);
        rad_x = -(double)dx/1000.0*M_PI/2;
        rad_y = -(double)dy/1000.0*M_PI/2;
        rot(forward,right_norm,eye,rad_y,forward);
        rot(forward,up_norm,eye,rad_x,forward);
        vec_sub(eye,pov,forward);
    }
    updateCamera();
    glutPostRedisplay();//更新した条件で再度描画
    prevMouseX = x;
    prevMouseY = y;

}
//GLUTはイベント駆動形
int main(int argc, char** argv) {
    load_obj(&bunny, file_name);//プログラムの最初に実行
    // FILE *fp;
    // fp = fopen("vn_test.txt", "w");
    // for(int i=0;i<bunny.num_vertices/3;i++) {
    //     fprintf(fp, "vn %f %f %f\n",bunny.normals[3*i],bunny.normals[3*i+1],bunny.normals[3*i+2]);
    // }
    // fclose(fp);
    glutInit(&argc, argv);//GLUTの内部データ構造を初期化(ウィンドウが表示される前)
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH);//ウィンドウの表示モード変更(ウィンドウが表示される前)
    glutCreateWindow(argv[0]);//ウィンドウを作成し表示(その後以下の関数がイベントに応じて実行される)

    init();//ウィンドウ作成後に１度だけ実行される
    glutDisplayFunc(display);//ウィンドウが表示されたりリサイズされたときとglutPostRedisplayで実行
    glutMouseFunc(mouse);//マウスボタンが押されたり離されたりするときに実行
    glutMotionFunc(motion);//マウスがドラッグされたときに実行
    glutReshapeFunc(resize);//ウィンドウサイズが変更されたときに実行される
    glutMainLoop();
    free(bunny.vertices);
    return 0;
}