// 20213058_이주엽_BST에서 변경점
// 1. inorder recursive로 구현(map 버림)
// 2. insertBST, deleteBST의 return type은 bool
// 3. inorder 및 잘못된 입력 메시지를 insertBST, deleteBST내부가 아닌 main에서 출력
// bf 계산 원활히 하기 위해 height 초기값 1로 변경
//

// 고칠것
// 현재 root의 bf 이상하다. pdf77대로 BF계산파트 바꾸기(height의존 x).
// 체크밸런스 리턴 a, f 두개 되게 => 이중포인터
// 현재는 height의존임 
// LL LR RL RR 0 1 2 3 으로 바꾸기.
// pdf 77참고해서 b로 코드 간결화하기.

// 최초삽입일시 처리?
// updateFactor 가독성
// 이름 다 AVL로 치환
// 코드 전체 쭉 훑기



// height, bf 전체적인 재조정.
#include <iostream>
#include <stack>

using namespace std;

enum {NO, LL, LR, RL, RR};

class Node{
public:
    Node() : left(nullptr), right(nullptr), height(1), bf(0) {};
    int key;
    Node* left;
    Node* right;
    int height;
    int bf;
};

class AVL{
public:
    AVL() : root(nullptr), rotate(NO) {};
    void inorder(Node* T);
    Node* getNodeBST();
    bool insertBST(Node* T, int newKey);
    Node* eraseBST(Node* T, int deleteKey);
    void clear(Node* &T);
    int height(Node* T);
    int size(Node* T);
    Node* minNode(Node* T);
    Node* maxNode(Node* T);
    bool insertAVL(Node* T, int newKey);
    bool eraseAVL(Node* T, int deleteKey);
    void checkBalance(Node* T, int newKey);
    void rotateTree(Node* T);
    void updateFactor(Node* T);
    void test(Node* T, int testKey);
public:
    Node* root;
    int rotate;         
    Node* x;
    Node* f;
};

// struct Result {
//     Node* value1;
//     Node* value2;
// };

void AVL::test(Node* T, int testKey){
    Node* p = T;
    while(p->key != testKey){
        if(testKey < p->key){p = p->left;}
        else{p = p->right;}
    }
    if(p == nullptr){return;}
    cout << "key: " << p->key << " " << "height: " << p->height << ' ' << "bf: " << p->bf << ' ' << "size: " << size(p) << endl;
}

void AVL::inorder(Node* T){
    Node* cur = T;
    if(cur == nullptr){return;}
    cout << '<';
    if(cur->left != nullptr){inorder(cur->left);}
    cout << " " << cur->key << " ";
    if(cur->right != nullptr){inorder(cur->right);}
    cout << '>';

    return;
}

Node* AVL::getNodeBST(){
    return new Node();
}

bool AVL::insertBST(Node* T, int newKey){
    Node* p = T;
    Node* q = nullptr;
    stack<Node*> stack;
    while(p != nullptr){
        if(newKey == p->key){               // 이미 있는 값이면 false 뱉음.
            return false;
        }       
        q = p;
        stack.push(q);
        if(newKey < p->key){p = p->left;}
        else{p = p->right;}
    }
    Node* newNode = getNodeBST();
    newNode->key = newKey;

    if(T == nullptr){root = newNode;} // 최초 삽입이면 root로 설정.
    else if(newKey < q->key){q->left = newNode;}
    else if(newKey > q->key){q->right = newNode;}

    // insertBST로 바뀐 height 및 BF 업데이트
    while(!stack.empty()){
        q = stack.top();
        stack.pop();
        if(q->left != nullptr && q->right != nullptr){
            if(q->left->height >= q->right->height){q->height = 1 + q->left->height;}
            else{q->height = 1 + q->right->height;}
        }
        else if(q->left == nullptr){q->height = 1 + q->right->height;}
        else if(q->right == nullptr){q->height = 1 + q->left->height;}

        // bf업데이트
        if (q->left != nullptr && q->right != nullptr) {q->bf = q->left->height - q->right->height;}
        else if (q->left == nullptr && q->right != nullptr) {q->bf = -q->right->height;}
        else if (q->left != nullptr && q->right == nullptr) {q->bf = q->left->height;}
        else {q->bf = 0;}
    }

    return true;
}

void AVL::checkBalance(Node* T, int checkedKey){
    Node* p = T;                    // q찾는 cur역할
    Node* q = nullptr;              // newKey의 삽입 위치
    x = nullptr;                    // x와 f 초기화
    f = nullptr;

    // 1. checkedKey 위치 q 및 q에 가장 가까운 불균형 노드 x 찾기.
    while(p != nullptr){
        if(p->bf < -1 || p->bf > 1){x = p; f = q;}              // x, f 업데이트

        if(checkedKey < p->key){q = p; p = p->left;}
        else if(checkedKey > p->key){q = p; p = p->right;}
        else{q = p; break;}
    }


    // 2. 트리의 균형여부 검사 및 회전 유형 결정
    rotate = NO;
    // 삽입과 삭제를 모두 고려한 회전 타입 지정 방법.
    if(x != nullptr){                   // 불균형 노드가 없다면 회전타입은 NO
        if (1 < x->bf) {                // x의 좌 서브트리가 높음
			if (x->left->bf < 0) { rotate = LR;}
			else { rotate = LL;}
		}
		else if( x->bf < -1 ){          // x의 우 서브트리가 높음
			// right�� bf�� ������ RL
			if (x->right->bf > 0) { rotate = RL;}
			// right�� bf�� ���� �Ǵ� 0�̶��
			else { rotate = RR;}
		}
    }
    
    return;
}

void AVL::rotateTree(Node* T){
    Node* a;
    Node* b;
    Node* c;
    if(rotate == NO){return;}

    if(rotate == LL){
        a = x;
        b = x->left;
        if(f != nullptr){
            if(f->left == a){f->left = b;}
            else if(f->right == a){f->right = b;}
        }
        else if(f == nullptr){root = b;}  // x가 root였다면(f가 nullptr이면) b가 root가 된다.
        if(x == root){root = x->left;}
        a->left = b->right;
        b->right = a;
        a->bf = 0;
        b->bf = 0;
    }
    else if(rotate == RR){
        a = x;
        b = x->right;
        if(f != nullptr){
            if(f->left == a){f->left = b;}
            else if(f->right == a){f->right = b;}
        }
        else if(f == nullptr){root = b;}  // x가 root였다면(f가 nullptr이면) b가 root가 된다.
        a->right = b->left;
        b->left = a;
        a->bf = 0;
        b->bf = 0;
    }
    else if(rotate == LR){
        a = x;
        b = a->left;
        c = b->right;
        if(f != nullptr){
            if(f->left == a){f->left = c;}
            else if(f->right == a){f->right = c;}
        }
        else if(f == nullptr){root = c;}
        b->right = c->left;
        a->left = c->right;
        c->left = b;
        c->right = a;
        switch(c->bf){
            case 0: { b->bf = 0; a->bf = 0; break;}
            case 1:{ a->bf = -1; b->bf = 0; break;}
            case -1:{ b->bf = 1; a->bf = 0; break;}
        }
        c->bf = 0;
        f = c;              // 특이케이스 커버
    }
    else if(rotate == RL){
        a = x;
        b = a->right;
        c = b->left;
        if(f != nullptr){
            if(f->left == a){f->left = c;}
            else if(f->right == a){f->right = c;}
        }
        else if(f == nullptr){root = c;}
        b->left = c->right;
        a->right = c->left;
        c->left = a;
        c->right = b;
        switch (c->bf) {
            case 0: { b->bf = 0; a->bf = 0; break; }
            case 1: { a->bf = 1; b->bf = 0; break; }
            case -1: { b->bf = -1; a->bf = 0; break; }
		}
        c->bf = 0;
        f = c;              // 특이케이스 커버
    }
}

void AVL::updateFactor(Node* T){
    if (T==nullptr){return;} // null 노드의 높이는 0
    // cout << '2' << endl;
    

    updateFactor(T->left);
    updateFactor(T->right);

    if(T->left != nullptr && T->right != nullptr){
            if(T->left->height >= T->right->height){T->height = 1 + T->left->height;}
            else{T->height = 1 + T->right->height;}
    }
    else if(T->left == nullptr && T->right == nullptr){T->height = 1;}
    else if(T->left == nullptr){T->height = 1 + T->right->height;}
    else if(T->right == nullptr){T->height = 1 + T->left->height;}

    if (T->left != nullptr && T->right != nullptr) {T->bf = T->left->height - T->right->height;}
    else if(T->left == nullptr && T->right == nullptr){T->bf = 0;}
    else if (T->left == nullptr && T->right != nullptr) {T->bf = -T->right->height;}
    else if (T->left != nullptr && T->right == nullptr) {T->bf = T->left->height;}
    else {T->bf = 0;}

    return;
}

bool AVL::insertAVL(Node* T, int newKey){
    // 최초삽입일 시 처리?

    if(!insertBST(T, newKey)){return false;}        // bst삽입. 삽입 불가 시 바로 false 뱉고 종료

    checkBalance(root, newKey);
    if(x == nullptr){return true;}          // bf 문제 없으면 삽입은 했으니 true 뱉고 종료.

    rotateTree(root);                          

    // 회전 후 높이, BF재조정.
    updateFactor(root);


    return true;
}

Node* AVL::eraseBST(Node* T, int deleteKey){
    Node* p = T;
    Node* q = nullptr;
    Node* ret;
    stack<Node*> heightUpdate;
    while( (p != nullptr) && (deleteKey != p->key) ){
        q = p;
        heightUpdate.push(q);
        if(deleteKey < p->key){p = p->left;}
        else{p = p->right;}
    }
    if(p == nullptr){return nullptr;}

    // 케이스 0. 자식이 둘(차수가 2)일 때.
    // p의 키를 좌측서브트리의 최대키 혹은 우측 서브트리의 최소키로 바꾸기.
    // p를 좌측 서브트리의 최대키노드 혹은 우측 서브트리의 최소키노드로 바꾸고
    // 차수가 0이거나 1인 경우로 넘어가 r(경우에 따라 좌측서브최대키 or 우측노드최소키) 삭제. 
    if( ((p->left != nullptr) && ( p->right != nullptr)) ){
        Node* r;
        if( (p->left->height < p->right->height) 
        || ((p->left->height == p->right->height) && (size(p->left) < size(p->right)))){
            r = minNode(p->right);
            p->key = r->key;
            heightUpdate.push(p);
            p = p->right;
            while(p->key != r->key){
                heightUpdate.push(p);
                p = p->left;
            }
        }
        else if(p->left->height >= p->right->height){
            r = maxNode(p->left);
            p->key = r->key;
            heightUpdate.push(p);
            p = p->left;
            while(p->key != r->key){
                heightUpdate.push(p);
                p = p->right;
            }
        }
        q = heightUpdate.top();
    }
    // AVL++ : 
    ret = q;

    // 케이스 1. 리프노드일 경우. 단순삭제.
    if( (p->left == nullptr) && ( p->right == nullptr) ){
        if(q == nullptr){root = nullptr;}                          // p가 root노드인 경우 처리.
        else if(q->left == p){q->left = nullptr;}
        else{q->right = nullptr;}
        delete p;
    }
    // 케이스 2. 자식이 하나(차수가 1)일 때. 그 자식노드를 삭제할 노드 위치로 변경(포인터 변경).
    else if( ((p->left != nullptr) && ( p->right == nullptr)) ){
        if(q == nullptr){root = p->left;}                          // p가 root노드인 경우 처리.
        else if(q->left == p){q->left = p->left;}
        else{q->right = p->left;}
        delete p;
    }
    else if( ((p->left == nullptr) && ( p->right != nullptr)) ){
        if(q == nullptr){root = p->right;}                          // p가 root노드인 경우 처리.
        else if(q->left == p){q->left = p->right;}
        else{q->right = p->right;}
        delete p;
    }

    // eraseBST로 바뀐 height 및 BF 업데이트.
    updateFactor(root);
    // cout << 'a' << endl;
    // while(!heightUpdate.empty()){
    //     q = heightUpdate.top();
    //     heightUpdate.pop();
    //     // case를 나눠주는 이유는 nullptr->height하면 오류나서.
    //     if(q->left != nullptr && q->right != nullptr){
    //         if(q->left->height >= q->right->height){q->height = 1 + q->left->height;}
    //         else{q->height = 1 + q->right->height;}
    //     }
    //     else if(q->left == nullptr && q->right == nullptr){q->height = 0;}  //delete는 q가 리프가 될 수 있음.
    //     else if(q->left == nullptr){q->height = 1 + q->right->height;}
    //     else if(q->right == nullptr){q->height = 1 + q->left->height;}

    //     // bf업데이트
    //     if (q->left != nullptr && q->right != nullptr) {q->bf = q->left->height - q->right->height;}
    //     else if (q->left == nullptr && q->right != nullptr) {q->bf = -q->right->height;}
    //     else if (q->left != nullptr && q->right == nullptr) {q->bf = q->left->height;}
    //     else {q->bf = 0;}
    // }
    return ret;
}

bool AVL::eraseAVL(Node* T, int deleteKey){
    Node* eAVL = eraseBST(T, deleteKey);      // bst삭제. 삭제 불가 시 바로 false 뱉고 종료.
    if( eAVL == nullptr){return false;}
    // cout << root->right->right->key << endl;
    Node* cur = eAVL;
    // while (cur != nullptr) {
    //     checkBalance(T, cur->key);
    //     rotateTree(root);
    //     updateFactor(T);
    //     cur = f;
    // }
    checkBalance(root, cur->key);
    // cout << root->right->right->key << endl;
    // cout << root->right->right->bf << endl;
    // cout << root->right->right->height << endl;
    // cout << rotate << endl;
    // if(x == nullptr){return true;}
    rotateTree(root);
    // cout << root->right->right->key << endl;

    updateFactor(root);

    
    // checkBalance(T, deleteKey);
    // if(x == nullptr){return true;}          // bf 문제 없으면 삽입은 했으니 true 뱉고 종료.

    // rotateTree(root);

    return true;
}
int AVL::height(Node* T){
    return T->height;
}
int AVL::size(Node* T){
    if(T == nullptr){return 0;}
    stack<Node*> stack;
    Node* cur = T;
    int size = 0;
    while((!stack.empty()) || cur != nullptr){
        while(cur != nullptr){
            stack.push(cur);
            size += 1;              // inorder에서 stack에 노드 들어갈 때마다 size += 1;
            cur = cur->left;
        }
        cur = stack.top();
        stack.pop();
        cur = cur->right;
    }
    return size;
}
Node* AVL::minNode(Node* T){
    Node* cur = T;
    while(cur->left != nullptr){cur = cur->left;}
    return cur;
}
Node* AVL::maxNode(Node* T){
    Node* cur = T;
    while(cur->right != nullptr){cur = cur->right;}
    return cur;
}
void AVL::clear(Node* &T){
    if(T == nullptr){return;}
    clear(T->left);
    clear(T->right);
    delete T;
    T = nullptr;
    // stack<Node*> nodes;     // inorder용 스택과 별개로 방문한 모든 노드 저장할 스택 추가.
    // stack<Node*> stack;
    // Node* cur = T;
    // while((!stack.empty()) || cur != nullptr){
    //     while(cur != nullptr){
    //         stack.push(cur);
    //         nodes.push(cur);
    //         cur = cur->left;
    //     }
    //     cur = stack.top();
    //     stack.pop();
    //     cur = cur->right;
    // }
    // while(!nodes.empty()){              // stack에서 꺼내며 delete 
    //     cur = nodes.top();
    //     nodes.pop();
    //     delete cur;
    // }
    
    return;
}



int main(){
    AVL avl;
    char order;
    int key;

    while(cin >> order >> key){
        switch(order){
        case 'i':
            if(avl.insertAVL(avl.root, key)){
                avl.inorder(avl.root);
                // cout << bst.root->key << ' ' << bst.root->bf;
                cout << endl;
            }
            else{cout << "i " << key << ": The key already exists" << endl;}
            break;
        case 'd':
            if (avl.root != nullptr && key == avl.root->key) {
                avl.eraseAVL(avl.root, key);
                avl.inorder(avl.root);
                cout << endl;
            }
            else if(avl.eraseAVL(avl.root, key)){
                avl.inorder(avl.root);
                // cout << bst.root->key;
                // cout << bst.root->key << ' ' << bst.root->bf << bst.root->right->key << ' ' << bst.root->right->right->key << ' ' << bst.root->right->right->height;
                cout << endl;
            }
            else{cout << "d " << key << ": The key does not exist" << endl;}
            break;
        case 't':
            avl.test(avl.root, key);
            break;
        case 'r':
            cout << avl.root->key << endl;
        }


        if(cin.eof()) break;
    }

    avl.clear(avl.root);
    return 0;
}

// CHECKEDBALANCE 초기 회전타입 지정 폼
// if(x != nullptr){                        // 트리가 불균형이면 회전 타입 지정해야함.
    //     if(x->key > checkedKey){
    //         if(x->left->key > checkedKey){rotate = "LL";}
    //         else{rotate = "LR";}
    //     }
    //     else if(x->key < checkedKey){
    //         if(x->right->key > checkedKey){rotate = "RL";}
    //         else{rotate = "RR";}
    //     }
    // }
