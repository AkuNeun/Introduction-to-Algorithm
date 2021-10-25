#include <iostream>
#include<stack>
#include<cmath>
#include<map>
//建两棵OS-Tree
//nil.size=0 新插入结点z.size=1
using namespace std;
#define RED 0
#define BLACK 1
#define pi acos(-1)

struct Node //OS-Tree结点
{
    double key; //角度[0,2pi)
    int size;   //内部结点数
    bool color;  //颜色 红0 黑1
    Node* left;  //左孩子
    Node* right;   //右孩子
    Node* p;  //父节点
    Node() //* 默认构造为nil
    {
        key = -1;
        size = 0;
        color = BLACK;
        left = nullptr;
        right = nullptr;
        p = nullptr;
    }
    Node(const double& a, const int& k, const bool& c = RED, Node* l = nullptr, Node* r = nullptr, Node* _p = nullptr) :
        key(a), size(k), color(c), left(l), right(r), p(_p) {}
};
struct RBTree
{
    Node* root;   //树根
    Node* nil;    //外部节点
    RBTree()
    {
        nil = new Node(-1, 0, BLACK, nil, nil, nil);
        root = nil;
    }
    //Node* Root() { return root; } //返回树根
    void left_rotate(Node* x);  //左旋
    void right_rotate(Node* x);   //右旋
    /*插入*/
    void insert(double val);  //插入值为val的结点
    void insert_fixup(Node* z);  //插入调整
    /*删除*/
    Node* find_min(Node* r);  //r的树上的最小值
    void transplant(Node* u, Node* v); //交换结点u和v，不涉及子树的交换
    void Delete(double val); //删除第一个值为val的结点
    void delnode(Node* z);
    void delete_fixup(Node* z);  //删除调整

    int OS_rank(Node* x);   //求结点的秩
    Node* OS_select(Node* x, int i); //查找第i小的结点

    int height(Node* x);   //树高
    Node* search(Node* m, double val);  //查找树m上值为val的结点
};
void RBTree::left_rotate(Node* x)   //左旋
{
    Node* y = x->right;
    x->right = y->left;
    if (y->left != nil)
    {
        y->left->p = x;
    }
    y->p = x->p;
    if (x->p == nil)
        root = y;
    else if (x->p->left == x)
        x->p->left = y;
    else
        x->p->right = y;
    y->left = x;
    x->p = y;
    //维护size
    y->size = x->size;
    x->size = x->left->size + x->right->size + 1;
}
void RBTree::right_rotate(Node* x)  //右旋
{
    Node* y = x->left;
    x->left = y->right;
    if (y->right != nil)
    {
        y->right->p = x;
    }
    y->p = x->p;
    if (x->p == nil)
        root = y;
    else if (x->p->left == x)
        x->p->left = y;
    else
        x->p->right = y;
    y->right = x;
    x->p = y;
    //维护size
    y->size = x->size;
    x->size = x->left->size + x->right->size + 1;
}
void RBTree::insert(double val) { //升序插入值为val的结点 
    Node* z = new Node(val, 1, RED, nil, nil, nil); //待插入结点
    if (root == nil)  //空树 
    {
        z->color = BLACK; //根染黑
        root = z;
        //root->size = 1;
        root->p = nil;
        root->left = nil;
        root->right = nil;
        return;
    }
    Node* x = root;
    Node* y = nil;
    while (x != nil) //找到插入位置
    {
        x->size++;  //维护size
        y = x;

        if (z->key < x->key)
        {
            x = x->left;
        }
        else
        {
            x = x->right;
        }
    }
    /*将z插入*/
    z->p = y;
    if (z->key < y->key)
    {
        y->left = z;
    }
    else
    {
        y->right = z;
    }
    z->left = nil;
    z->right = nil;
    Node* temp = z;
    insert_fixup(temp); //调整平衡
};
void RBTree::insert_fixup(Node* z) //调整平衡
{
    while (z->p->color == RED)
    {
        if (z->p == z->p->p->left)
        {
            Node* y = z->p->p->right;
            if (y->color == RED)
            {
                z->p->color = BLACK;
                y->color = BLACK;
                z->p->p->color = RED;
                z = z->p->p;
            }
            else
            {
                if (z == z->p->right)
                {
                    z = z->p;
                    left_rotate(z);
                }
                z->p->color = BLACK;
                z->p->p->color = RED;
                right_rotate(z->p->p);
            }
        }
        else
        {
            Node* y = z->p->p->left;
            if (y->color == RED)
            {
                z->p->color = BLACK;
                y->color = BLACK;
                z->p->p->color = RED;
                z = z->p->p;
            }
            else
            {
                if (z == z->p->left)
                {
                    z = z->p;
                    right_rotate(z);
                }
                z->p->color = BLACK;
                z->p->p->color = RED;
                left_rotate(z->p->p);
            }
        }
    } //while
    root->color = BLACK; //根染黑
}
/*删除结点*/
Node* RBTree::find_min(Node* r) //找r的子树上的最小值
{
    Node* p = r;
    while (r != nil)
    {
        p = r;
        p->size--; //*维护size!!!!!!!!
        r = r->left;
    }
    return p;
}
void RBTree::transplant(Node* u, Node* v)  //交换两个结点*不涉及对子树的操作 //需要对size进行维护?
{
    if (u->p == nil)
        root = v;
    else if (u == u->p->left)
        u->p->left = v;
    else
        u->p->right = v;
    v->p = u->p;
}

void RBTree::Delete(double val)  //删除值为val的结点
{
    if (root == nil) //空树
        return;
    Node* z = root;
    while (z != nil)
    {
        if (val == z->key) //找到
            break;
        if (val < z->key)
            z = z->left;
        else if (val > z->key)
            z = z->right;
    }
    if (z == nil) //结点不存在
    {
        //cout << "not found" << endl;
        return;
    }
    delnode(z);
}
void RBTree::delnode(Node* z) //删除结点  待debug
{
    Node* y = z, * x = z;
    bool ori_color = y->color; //y原来的颜色
    /*维护size*/
    Node* t = z->p;
    while (t != nil)
    {
        t->size--;
        t = t->p;
    }
    if (z->left == nil)
    {
        x = z->right;
        transplant(z, z->right);
    }
    else if (z->right == nil)
    {
        x = z->left;
        transplant(z, z->left);
    }
    else  //需要维护size（debug:需要在find_min里面维护）
    {
        y = find_min(z->right);
        ori_color = y->color;   //
        x = y->right;
        if (y->p == z)
        {
            x->p = y;
        }
        else
        {
            transplant(y, y->right);
            y->right = z->right;
            y->right->p = y;
            //维护size
            y->right->size = y->right->right->size + y->right->left->size + 1;
            y->size = y->right->size + y->left->size + 1;
        }
        transplant(z, y);
        y->left = z->left;
        y->left->p = y;
        y->color = z->color;
        //维护size
        y->left->size = y->left->left->size + y->left->right->size + 1;
        y->size = y->right->size + y->left->size + 1;
        if (y->p != nil)
        {
            y->p->size = y->p->left->size + y->p->right->size + 1;
        }
    }
    delete z; //*
    if (ori_color == BLACK)
    {
        delete_fixup(x);
    }
}
void RBTree::delete_fixup(Node* x)
{
    while (x != root && x->color == BLACK)
    {
        if (x == x->p->left)
        {
            Node* w = x->p->right;
            if (w->color == RED)         //CASE 1
            {
                w->color = BLACK;
                x->p->color = RED;
                left_rotate(x->p);
                w = x->p->right;
            }
            if (w->left->color == BLACK && w->right->color == BLACK)    //CASE 2
            {
                w->color = RED;
                x = x->p;
            }
            else
            {
                if (w->right->color == BLACK)    //CASE 3
                {
                    w->left->color = BLACK;
                    w->color = RED;
                    right_rotate(w);
                    w = x->p->right;
                }
                w->color = x->p->color;
                x->p->color = BLACK;
                w->right->color = BLACK;
                left_rotate(x->p);
                x = root;
            }
        }// if
        else  //对称情况
        {
            Node* w = x->p->left;
            if (w->color == RED)   //CASE 1
            {
                w->color = BLACK;
                x->p->color = RED;
                right_rotate(x->p);
                w = x->p->left;
            }
            if (w->right->color == BLACK && w->left->color == BLACK)     //CASE 2
            {
                w->color = RED;
                x = x->p;
            }
            else
            {
                if (w->left->color == BLACK)      //CASE 3
                {
                    w->right->color = BLACK;
                    w->color = RED;
                    left_rotate(w);
                    w = x->p->left;
                }
                w->color = x->p->color;
                x->p->color = BLACK;
                w->left->color = BLACK;
                right_rotate(x->p);
                x = root;
            }
        }
    }
    x->color = BLACK;
}

int RBTree::height(Node* x)//求树高
{
    if (x == nil)
        return 0; //空树
    int hl = height(x->left);   //左子树高度
    int hr = height(x->right);    //右子树高度
    return (max(hl, hr) + 1);
}
Node* RBTree::search(Node* m, double val)
{
    if (m == nil)
        return nil;
    Node* z = m;
    while (z != nil)
    {
        if (val == z->key)
            return z;
        if (val < z->key)
            z = z->left;
        else if (val > z->key)
            z = z->right;
    }
    return nil;
}
Node* RBTree::OS_select(Node* x, int i)
{
    //if (x == nil) return nil; //*有误吗？
    int r = x->left->size + 1;
    if (i == r)
        return x;
    else if (i < r)
    {
        return OS_select(x->left, i);
    }
    else
    {
        return OS_select(x->right, i - r);
    }
}
int RBTree::OS_rank(Node* x)  //
{
    if (x == nil)
        return 0;
    //置r为以x为根的子树中key[x]的秩
    int r = x->left->size + 1;
    Node* y = x;
    while (y != root)
    {
        //若y是p[y]的右孩子，p[y]和p[y]左子树中所有结点前于x
        if (y == y->p->right)
            r = r + y->p->left->size + 1;
        y = y->p;
    }
    return r;
}

double getAngle(double x, double y)//计算与y轴正方向的顺时针夹角
{
    double ang;
    if (x >= 0)
    {
        if (y >= 0) //第一象限
            ang = atan(x / y);
        else   //第二象限
            ang = atan(-y / x) + pi / 2;  //x=0的情况有可能出问题
    }
    else
    {
        if (y < 0) //第三象限
            ang = atan(-x / -y) + pi;
        else //第四象限
            ang = atan(y / -x) + 1.5 * pi;
    }
    return ang;
}
void swap(double& a, double& b)
{
    double temp = a;
    a = b;
    b = temp;
}

map<double, double> R; //两棵树端点之间建立关联
int main()   //待debug：删除时的size维护出错
{
    int n; //n条弦
    cin >> n;
    RBTree t1, t2; //建两棵OStree，t1存放起始角度 t2存放结束角度
    double x1, y1, x2, y2; //弦的端点坐标
    double a1, a2; //与y轴正方向的顺时针夹角
    for (int i = 0; i < n; i++)
    {
        cin >> x1 >> y1 >> x2 >> y2;
        a1 = getAngle(x1, y1); //起始角度
        a2 = getAngle(x2, y2);  //结束角度
        if (a1 > a2)
            swap(a1, a2);
        t1.insert(a1);
        t2.insert(a2);
        R.insert(pair<double, double>(a2, a1)); //关联一条弦的两个端点R：end-start
    }

    long long cnt = 0; //相交弦对数
    Node* end, * temp, * tmp;
    double start;  //弦的起始角度
    long long r;
    long long rank_2; //用于判断
    for (int i = 0; i < n; i++) //树非空
    {
        end = t2.OS_select(t2.root, 1); //取最小的终点，即t2中rank=1的结点 //最小的a2
        start = R[end->key]; //a1
        temp = t1.search(t1.root, start);  //找到起点对应的结点 a1
        //判断有多少起点的角度比start大
        /*修改*/
        t1.insert(end->key);
        tmp = t1.search(t1.root, end->key);
        rank_2 = t1.OS_rank(tmp); //a2的rank //求出来有误——删除时size维护出错
        r = t1.OS_rank(temp); //a1的rank
        cnt += rank_2 - r - 1;
        t1.delnode(tmp);
        t2.delnode(end);
        t1.Delete(start);
    }
    cout << cnt;


    /*相交条件:a1<b1<a2<b2
    *思路1：取a1，size-rank= a2<b2的数量 还要判断b1<a2
    *原来的思路：取出a2（end），size-a1.rank=b1>a1的数量，还要判断b1(start)<a2(end)*/

    return 0;
}