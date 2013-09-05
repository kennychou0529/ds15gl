//����http://blog.csdn.net/yushuai007008/article/details/7096726
// MyQueue.cpp : �������̨Ӧ�ó������ڵ㡣
//ʵ����ʽ���У�queue��������һ��ͷ��㡣���в��������ڶ�ͷ���ӣ�pop�����ڶ�β��ӣ�push����
//ȡ�ö�ͷԪ�أ�front_element)��ȡ�ö�βԪ�أ�back_element��������Ԫ�ظ�����size����
//�����Ƿ�Ϊ�գ�empty����
//#include "stdafx.h"
#include <iostream>

using namespace std;

//������еĽڵ�ṹ
template <class T>
struct NODE {
    NODE<T>* next;
    T data;
};

template <class T>
class DSQueue {
public:
    DSQueue() {
        NODE<T>* p = new NODE<T>;
        if (NULL == p) {
            cout << "Failed to malloc the node." << endl;
        }
        p->data = NULL;
        p->next = NULL;
        front = p;
        rear = p;
    }

    //�ڶ�β���
    void push(T e) {
        NODE<T>* p = new NODE<T>;
        if (NULL == p) {
            cout << "Failed to malloc the node." << endl;
        }
        p->data = e;
        p->next = NULL;
        rear->next = p;
        rear = p;
    }

    //�ڶ�ͷ����
    T pop() {
        T e;

        if (front == rear) {
            cout << "The queue is empty." << endl;
            return NULL;
        } else {
            NODE<T>* p = front->next;
            front->next = p->next;
            e = p->data;
            //ע���жϵ�ֻ��һ��Ԫ�أ���ɾ����֮��rearָ���node��ɾ��
            //Ӧ����ָ��ͷ���
            if (rear == p) {
                rear = front;
            }
            delete p;
            p = NULL;
            return e;
        }
    }

    //ȡ�ö�ͷԪ��
    T front_element() {
        if (front == rear) {
            cout << "The queue is empty." << endl;
            return NULL;
        } else {
            NODE<T>* p = front->next;
            return p->data;
        }
    }

    T back_element() {
        if (front == rear) {
            cout << "The queue is empty." << endl;
            return NULL;
        } else {
            return rear->data;
        }
    }

    //ȡ�ö���Ԫ�ظ���
    int size() {
        int count(0);

        NODE<T>* p = front;

        while (p != rear) {
            p = p->next;
            count++;
        }
        return count;
    }

    //�ж϶����Ƿ�Ϊ��
    bool empty() {
        if (front == rear) {
            return true;
        } else {
            return false;
        }
    }

private:
    NODE<T>* front; //ָ��ͷ����ָ�롣 front->next->data�Ƕ�ͷ��һ��Ԫ�ء�
    NODE<T>* rear;//ָ���β�������ӵ�һ��Ԫ�أ���ָ��
};
//
//int _tmain(int argc, _TCHAR* argv[])
//{
//  DSQueue<int> myqueue;
//  cout << myqueue.size() << endl;
//  myqueue.push(10);
//  myqueue.push(20);
//  myqueue.push(30);
//  cout << myqueue.front_element() << endl;
//  cout << myqueue.back_element() << endl;
//  myqueue.pop();
//  if (myqueue.empty())
//  {
//      cout << "The queue is empty now." << endl;
//  }
//  else
//  {
//      cout << "The queue has " << myqueue.size() << " elements now." << endl;
//  }
//  myqueue.pop();
//  myqueue.pop();
//  if (myqueue.empty())
//  {
//      cout << "The queue is empty now." << endl;
//  }
//  else
//  {
//      cout << "The queue has " << myqueue.size() << " elements now." << endl;
//  }
//  return 0;
//}