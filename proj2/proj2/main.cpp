#include <string>
#include <cassert>
#include <iostream>
#include "Sequence.h"
using namespace std;

#define TEST_WITH_STRING

#ifdef TEST_WITH_STRING
const ItemType VAL[13] = { "testing", "hello", "why", "there", "zebra", "yawning", "UCLA", "b", "a", "asdf", "woah", "oo", ""};
#else // assume unsigned long
const ItemType VAL[13] = { 9, 8, 23, 20, 26, 25, 21, 3, 1, 2, 54, 45, 34 };
#endif

int main()
{
    //object 1
    Sequence s; //default constructor
    assert(s.empty() == true); //test empty
    assert(s.size() == 0); //test size
    assert(s.insert(0, VAL[0]) == 0); //test insert for empty
    assert(s.size() == 1); //test size increase
    assert(s.insert(1, VAL[1]) == 1); //test insert for one-element
    assert(s.size() == 2); //test size increase
    assert(s.insert(3, VAL[1]) == -1); //test insert failure
    assert(s.insert(1, VAL[2]) == 1); //test insert for larger seq
    assert(s.size() == 3); //test size increase
    assert(s.insert(VAL[3]) == 1); //test insert without pos for multi-element
    assert(s.find(VAL[3]) == 1); //test find for multi-element
    s.dump(); //verify the seq
    assert(s.erase(0) == true);
    assert(s.size() == 3);
    s.dump();
    ItemType x;
    assert(s.get(0, x) == true && x == VAL[3]); //test get for multi-element
    assert(s.set(1, x) == true); //test set for multi-element
    s.dump();
    assert(s.get(1, x) == true && x == VAL[3]); 
    cerr << endl;

    //object 2
    Sequence t;
    assert(t.erase(0) == false); //test erase for empty
    assert(t.remove(VAL[0]) == 0); //test remove for empty
    assert(t.get(0, x) == false); //test get for empty
    assert(t.set(0, x) == false); //test set for empty
    assert(t.find(VAL[0]) == -1); //test find for emtpy
    assert(t.insert(VAL[0]) == 0); //test insert without pos for empty
    assert(t.find(VAL[0]) == 0); //test find for one-element
    assert(t.insert(VAL[1]) == 0); //test insert without pos for one-element
    assert(t.find(VAL[1]) == 0); //test find for multi-element
    assert(t.find(VAL[5]) == -1); //test find for no results
    t.dump();
    assert(t.erase(1) == true); //test erase
    t.dump();
    assert(t.insert(VAL[1]) == 0); //test insert
    assert(t.insert(VAL[1]) == 0); //test insert
    assert(t.insert(VAL[1]) == 0); //test insert
    assert(t.insert(VAL[1]) == 0); //test insert
    assert(t.insert(3, VAL[4]) == 3); //test insert
    t.dump();
    assert(t.remove(VAL[1]) == 5); //test remove for multi-element
    assert(t.size() == 1);
    t.dump();
    assert(t.get(0, x) == true && x == VAL[4]); //test get for one-element
    assert(t.set(0, VAL[5]) == true); //test set for one-element
    t.dump();
    assert(t.set(0, x) == true);
    t.dump();
    assert(t.remove(VAL[4]) == 1); //test remove for one-element
    assert(t.size() == 0);
    t.dump();
    cerr << endl;

    //object 3
    Sequence ss;
    ss.insert(0, VAL[1]);
    ss.insert(1, VAL[2]);
    ss.insert(2, VAL[3]);
    ItemType y = VAL[4];
    assert(!ss.get(3, y) && y == VAL[4]);  // x is unchanged
    assert(ss.get(1, y) && y == VAL[2]);
    ss.dump();
    cerr << endl;

    //copy, swap tests
    Sequence tt(ss); //test copy constructor for multi-element
    tt.dump();
    tt.erase(0);
    tt.erase(0);
    tt.dump();
    Sequence u(t); //test copy constructor for empty
    u.dump();
    tt.swap(u); //test swap
    tt.dump();
    u.dump();
    Sequence v(tt); //test copy constructor for one-element
    v.dump();
    v.swap(s); //test swap
    v.dump();
    cerr << endl;

    //assigment tests
    v.dump();
    t.dump();
    t = v; //test assignment for one empty and one non-empty list
    v.dump();
    t.dump();
    cerr << endl;

    //subsequence tests
    Sequence s1, s2;
    for (int i = 0; i < 5; i++)
        s1.insert(i, VAL[i]);
    assert(subsequence(s1, s2) == -1);
    s2.insert(0, VAL[2]);
    assert(subsequence(s1, s2) == 2);
    s2.insert(1, VAL[3]);
    assert(subsequence(s1, s2) == 2);
    s2.insert(2, VAL[4]);
    assert(subsequence(s1, s2) == 2);
    s2.insert(0, VAL[1]);
    assert(subsequence(s1, s2) == 1);
    s2.insert(0, VAL[0]);
    assert(subsequence(s1, s2) == 0);
    s2.set(2, VAL[5]);
    assert(subsequence(s1, s2) == -1);
    s2.set(2, VAL[2]);
    s2.insert(5, VAL[5]);
    assert(subsequence(s1, s2) == -1);
    s1.dump();
    cerr << endl;

    //interleave cases
    Sequence result;
    interleave(ss, t, result); //test interleave when s1.size() == s2.size()
    ss.dump();
    t.dump();
    result.dump();
    Sequence result2;
    Sequence empty;
    Sequence empty2;
    interleave(result, ss, result2); //test interleave when s1.size() > s2.size()
    result2.dump();
    interleave(ss, result, result2); //test interleave when s1.size() < s2.size()
    result2.dump();
    interleave(result, empty, result2); //test interleave when s2 empty
    result2.dump();
    interleave(empty, result, result2); //test interleave when s1 empty
    result2.dump();
    interleave(empty, empty2, result2); //test interleave when both empty
    result2.dump();
    interleave(result, result, result2); //test interleave when s1 and s2 are the same
    result2.dump();
    interleave(result, ss, result); //test interleave when s1 and result are the same
    result.dump();
    interleave(ss, result, result); //test interleave when s2 and result are the same
    result.dump();
    interleave(ss, ss, ss); //test interleave when s1 s2 and result are the same
    ss.dump();

    cerr << "Passed all tests" << endl;
}