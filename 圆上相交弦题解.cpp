#include <iostream>
#include<stack>
#include<cmath>
#include<map>
//������OS-Tree
//nil.size=0 �²�����z.size=1
using namespace std;
#define RED 0
#define BLACK 1
#define pi acos(-1)

struct Node //OS-Tree���
{
    double key; //�Ƕ�[0,2pi)
    int size;   //�ڲ������
    bool color;  //��ɫ ��0 ��1
    Node* left;  //����
    Node* right;   //�Һ���
    Node* p;  //���ڵ�
    Node() //* Ĭ�Ϲ���Ϊnil
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
    Node* root;   //����
    Node* nil;    //�ⲿ�ڵ�
    RBTree()
    {
        nil = new Node(-1, 0, BLACK, nil, nil, nil);
        root = nil;
    }
    //Node* Root() { return root; } //��������
    void left_rotate(Node* x);  //����
    void right_rotate(Node* x);   //����
    /*����*/
    void insert(double val);  //����ֵΪval�Ľ��
    void insert_fixup(Node* z);  //�������
    /*ɾ��*/
    Node* find_min(Node* r);  //r�����ϵ���Сֵ
    void transplant(Node* u, Node* v); //�������u��v�����漰�����Ľ���
    void Delete(double val); //ɾ����һ��ֵΪval�Ľ��
    void delnode(Node* z);
    void delete_fixup(Node* z);  //ɾ������

    int OS_rank(Node* x);   //�������
    Node* OS_select(Node* x, int i); //���ҵ�iС�Ľ��

    int height(Node* x);   //����
    Node* search(Node* m, double val);  //������m��ֵΪval�Ľ��
};
void RBTree::left_rotate(Node* x)   //����
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
    //ά��size
    y->size = x->size;
    x->size = x->left->size + x->right->size + 1;
}
void RBTree::right_rotate(Node* x)  //����
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
    //ά��size
    y->size = x->size;
    x->size = x->left->size + x->right->size + 1;
}
void RBTree::insert(double val) { //�������ֵΪval�Ľ�� 
    Node* z = new Node(val, 1, RED, nil, nil, nil); //��������
    if (root == nil)  //���� 
    {
        z->color = BLACK; //��Ⱦ��
        root = z;
        //root->size = 1;
        root->p = nil;
        root->left = nil;
        root->right = nil;
        return;
    }
    Node* x = root;
    Node* y = nil;
    while (x != nil) //�ҵ�����λ��
    {
        x->size++;  //ά��size
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
    /*��z����*/
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
    insert_fixup(temp); //����ƽ��
};
void RBTree::insert_fixup(Node* z) //����ƽ��
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
    root->color = BLACK; //��Ⱦ��
}
/*ɾ�����*/
Node* RBTree::find_min(Node* r) //��r�������ϵ���Сֵ
{
    Node* p = r;
    while (r != nil)
    {
        p = r;
        p->size--; //*ά��size!!!!!!!!
        r = r->left;
    }
    return p;
}
void RBTree::transplant(Node* u, Node* v)  //�����������*���漰�������Ĳ��� //��Ҫ��size����ά��?
{
    if (u->p == nil)
        root = v;
    else if (u == u->p->left)
        u->p->left = v;
    else
        u->p->right = v;
    v->p = u->p;
}

void RBTree::Delete(double val)  //ɾ��ֵΪval�Ľ��
{
    if (root == nil) //����
        return;
    Node* z = root;
    while (z != nil)
    {
        if (val == z->key) //�ҵ�
            break;
        if (val < z->key)
            z = z->left;
        else if (val > z->key)
            z = z->right;
    }
    if (z == nil) //��㲻����
    {
        //cout << "not found" << endl;
        return;
    }
    delnode(z);
}
void RBTree::delnode(Node* z) //ɾ�����  ��debug
{
    Node* y = z, * x = z;
    bool ori_color = y->color; //yԭ������ɫ
    /*ά��size*/
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
    else  //��Ҫά��size��debug:��Ҫ��find_min����ά����
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
            //ά��size
            y->right->size = y->right->right->size + y->right->left->size + 1;
            y->size = y->right->size + y->left->size + 1;
        }
        transplant(z, y);
        y->left = z->left;
        y->left->p = y;
        y->color = z->color;
        //ά��size
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
        else  //�Գ����
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

int RBTree::height(Node* x)//������
{
    if (x == nil)
        return 0; //����
    int hl = height(x->left);   //�������߶�
    int hr = height(x->right);    //�������߶�
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
    //if (x == nil) return nil; //*������
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
    //��rΪ��xΪ����������key[x]����
    int r = x->left->size + 1;
    Node* y = x;
    while (y != root)
    {
        //��y��p[y]���Һ��ӣ�p[y]��p[y]�����������н��ǰ��x
        if (y == y->p->right)
            r = r + y->p->left->size + 1;
        y = y->p;
    }
    return r;
}

double getAngle(double x, double y)//������y���������˳ʱ��н�
{
    double ang;
    if (x >= 0)
    {
        if (y >= 0) //��һ����
            ang = atan(x / y);
        else   //�ڶ�����
            ang = atan(-y / x) + pi / 2;  //x=0������п��ܳ�����
    }
    else
    {
        if (y < 0) //��������
            ang = atan(-x / -y) + pi;
        else //��������
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

map<double, double> R; //�������˵�֮�佨������
int main()   //��debug��ɾ��ʱ��sizeά������
{
    int n; //n����
    cin >> n;
    RBTree t1, t2; //������OStree��t1�����ʼ�Ƕ� t2��Ž����Ƕ�
    double x1, y1, x2, y2; //�ҵĶ˵�����
    double a1, a2; //��y���������˳ʱ��н�
    for (int i = 0; i < n; i++)
    {
        cin >> x1 >> y1 >> x2 >> y2;
        a1 = getAngle(x1, y1); //��ʼ�Ƕ�
        a2 = getAngle(x2, y2);  //�����Ƕ�
        if (a1 > a2)
            swap(a1, a2);
        t1.insert(a1);
        t2.insert(a2);
        R.insert(pair<double, double>(a2, a1)); //����һ���ҵ������˵�R��end-start
    }

    long long cnt = 0; //�ཻ�Ҷ���
    Node* end, * temp, * tmp;
    double start;  //�ҵ���ʼ�Ƕ�
    long long r;
    long long rank_2; //�����ж�
    for (int i = 0; i < n; i++) //���ǿ�
    {
        end = t2.OS_select(t2.root, 1); //ȡ��С���յ㣬��t2��rank=1�Ľ�� //��С��a2
        start = R[end->key]; //a1
        temp = t1.search(t1.root, start);  //�ҵ�����Ӧ�Ľ�� a1
        //�ж��ж������ĽǶȱ�start��
        /*�޸�*/
        t1.insert(end->key);
        tmp = t1.search(t1.root, end->key);
        rank_2 = t1.OS_rank(tmp); //a2��rank //��������󡪡�ɾ��ʱsizeά������
        r = t1.OS_rank(temp); //a1��rank
        cnt += rank_2 - r - 1;
        t1.delnode(tmp);
        t2.delnode(end);
        t1.Delete(start);
    }
    cout << cnt;


    /*�ཻ����:a1<b1<a2<b2
    *˼·1��ȡa1��size-rank= a2<b2������ ��Ҫ�ж�b1<a2
    *ԭ����˼·��ȡ��a2��end����size-a1.rank=b1>a1����������Ҫ�ж�b1(start)<a2(end)*/

    return 0;
}