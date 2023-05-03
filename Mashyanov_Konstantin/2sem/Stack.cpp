#include <string>
#include <iostream>
using namespace std;

class Stack {
private:
  int* data;  
  int top;    
  int size;   

public:

  Stack(int n);
  ~Stack();
  void push(int value);  //  Метод для добавления элемента в вершину стека
  void pop();  //  Метод для удаления элемента из вершины стека и возвращения его значения
  int peek();  //  Метод для получения элемента из вершины стека без его удаления
  bool is_empty();  // Метод для проверки, пуст ли стек
  bool is_full();  //  Метод для проверки, полон ли стек
};
  
 
  Stack::Stack(int n) {
    size = n;              
    data = new int[size];  
    top = -1;              
  }

  Stack::~Stack() {
    delete[] data;        
  }

  void Stack::push(int value) {
    if (top >= size - 1) {     
      throw "Stack Overflow";  
    } else {
      top++;                  
      data[top] = value;       
    }
  }

  void Stack::pop() {
    if (top < 0) {            
      throw "Stack Underflow"; 
    } else {
      int value = data[top];  
      top--;                  
    }
  }

  int Stack::peek() {
    if (top < 0) {            
      throw "Stack is Empty"; 
    } else {
      return data[top];       
    }
  }

  bool Stack::is_empty() {
    return top == -1;         
  }

  bool Stack::is_full() {
    return top == size - 1;   
  }
  
  
int main() {
  Stack s(5);
  s.push(10);
  s.push(20);
  s.push(30);
  s.push(40);
  s.push(50);

  while (!s.is_empty()) {
    s.peek();
    s.pop();
  }

  return 0;
}
