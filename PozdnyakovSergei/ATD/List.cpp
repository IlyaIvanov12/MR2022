#include <iostream>


class Node {
public:
    Node* next;
    double value;
    Node();
    Node(double value = double (), Node *pnext = nullptr);
    Node* get_next();
    void push_next (double value);
    void del_next (double value);
};


typedef Node *Nodeptr;

class List {
private:
    Node *head;
    Nodeptr prev (double value);
public:
    List();
    ~List();

    void push_back(double value);
    void del(double value);
    void push_head(double value);

    void del_head();
    Nodeptr peek_head();
    void clean();
    void print();

    void push_next (double value, double data_next);
    void push_prev (double value, double data_prev);
};


Node:: Node (double value ,Node *pnext){
    this->value = value;
    this->next = pnext;
}


void Node::push_next(double data) {
    Nodeptr node = next;
    Nodeptr new_node = new Node (data);
    node->next = new_node;
    new_node->next = next;
}


void Node::del_next(double data) {
    Node* temp = next;
    next = next->next;
    delete temp;
}


Node* Node:: get_next(){
    return next;
}


List::List(){
    head = nullptr;
}


List:: ~List(){
    clean();
}

Nodeptr List:: peek_head(){
    return head;
}


void List::push_back(double key){
    if ( head == nullptr){
        head = new Node (key);
        return;
    }
    Node *current = head;
    while ( current->next != nullptr){
        current = current->next;
    }
    current->next = new Node (key);
}


void List::push_head(double key){
    head = new Node (key, head);
}


void List::push_next (double data, double data_){
    Nodeptr node_prev = head;
    while (node_prev->value != data_ ){
        node_prev = node_prev->next;
    }
    Nodeptr new_node = new Node (data, node_prev->next);
    node_prev->next = new_node;
}


void List::push_prev(double value, double value_prev) {
    Nodeptr previous = prev(value_prev);
    Nodeptr new_node = new Node (value, previous->next);
    previous->next = new_node;
}


void List:: del_head(){
    Node *current = head;
    head = head->next;
    delete current;
}


void List::clean(){
    while (head != nullptr){
        del_head();
    }
}


void List:: del(double key){
    Node* node_to_del = head;
    while ( node_to_del->value != key)
        node_to_del = node_to_del->next;
    Node *current;
    current = prev(key);
    node_to_del = current->next;
    current->next = node_to_del->next;
    delete node_to_del;
}


void List::print(){
    Node *current = head;
    while (current != nullptr){
        std::cout << current->value << "  ";
        current = current->next;
    }
    std::cout << "\n";
}


Nodeptr List::prev(double data_){
    Node *current = head;
    while ( current->next->value != data_)
        current = current->next;
    return current;
}


int main() {
    List lst;
    for (int i = 0; i < 9; i++){
        lst.push_back(rand() %10);
    }
    lst.print();

    lst.push_head(9.00);
    lst.push_back(5.00);
    lst.print();
    lst.del( 1);
    lst.push_prev(1.00, 0);
    lst.push_next(1.00, 0);

    lst.print();
    lst.clean();
    lst.print();

    return 0;
}