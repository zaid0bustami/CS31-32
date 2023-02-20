#include "NameTable.h"
#include <functional>
#include <string>
#include <stack>
#include <forward_list>
using namespace std;

// This class does the real work of the implementation.

class HashTable
{
private:
    struct Item
    {
        string m_id;
        int m_line;
        int m_scope;
    };
    forward_list<Item> m_table[6007] = {};
    stack<Item> m_items;
public:
    int nBuckets() const { return 6007; }
    int nItems() const { return m_items.size(); }
    //int loadFactor() const { return m_nItems / nBuckets(); }
    unsigned int hash(const string& id) const { return std::hash<string>()(id) % nBuckets(); }

    int find(const string& id, const int& scope) const;
    void insert(const string& id, const int& line, const int& scope);
    void remove(const int& scope);
    //forward_list<Item>::iterator remove(const string& id, const int& scope);
};

int HashTable::find(const string& id, const int& scope) const
{
    int bucket = hash(id);
    forward_list<Item>::const_iterator item = m_table[bucket].begin();

    while (item != m_table[bucket].end())
    {
        if ((scope == -1 || item->m_scope == scope) && item->m_id == id) //either diregard the scope and just find an id match or find a scope and id match
        {
            return item->m_line;
        }
        item++;
    }
    return -1;
}
void HashTable::insert(const string& id, const int& line, const int& scope)
{
    Item item = { id, line, scope };
    int bucket = hash(id);

    m_table[bucket].push_front(item);
    m_items.push(item);
}
void HashTable::remove(const int& scope)
{
    while (!m_items.empty() && m_items.top().m_scope == scope)
    {
        int bucket = hash(m_items.top().m_id);
        m_table[bucket].pop_front();
        m_items.pop();
    }
    
    /*for (int i = 0; i != nBuckets(); i++) //this takes a LOT of time
    {
        while (!m_table[i].m_items.empty() && m_table[i].m_items.begin()->m_scope == scope) //while there's still an item with the scope of interest, remove it
        {
            m_table[i].m_items.pop_front();
            m_nItems--;
        }
    }*/

    /*int bucket = hash(id); //hashing each time could be eating up time
    bool b = false;

    forward_list<Item>::const_iterator i = find(id, scope, b);
    if (i == m_table[bucket].m_items.end())
        return m_table[bucket].m_items.end();
    forward_list<Item>::iterator j = m_table[bucket].m_items.before_begin();
    forward_list<Item>::iterator k = j;
    k++;
    for (; k != i; k++, j++) {} //moves j so that the iterator after it (k) points to the item we'd like to remove
    m_nItems--;
    return m_table[bucket].m_items.erase_after(j);*/
}

class NameTableImpl
{
public:
    void enterScope();
    bool exitScope();
    bool declare(const string& id, int lineNum);
    int find(const string& id) const;
private:
    HashTable m_hash;
    int m_scope = 0;
};

void NameTableImpl::enterScope()
{
    m_scope++;
}

bool NameTableImpl::exitScope()
{
    if (m_scope > 0)
    {
        m_hash.remove(m_scope);
        m_scope--;
        return true;
    }
    else
    {
        return false;
    }
}

bool NameTableImpl::declare(const string& id, int lineNum)
{
    int i = m_hash.find(id, m_scope); //run a find strict to scope
    if (i == -1 && id != "")
    {
        m_hash.insert(id, lineNum, m_scope);
        return true;
    }
    return false;
}

int NameTableImpl::find(const string& id) const
{
    if (m_hash.nItems() != 0)
        return m_hash.find(id, -1); //run a find not strict to scope
    return -1;
}

//*********** NameTable functions **************

// For the most part, these functions simply delegate to NameTableImpl's
// functions.

NameTable::NameTable()
{
    m_impl = new NameTableImpl;
}

NameTable::~NameTable()
{
    delete m_impl;
}

void NameTable::enterScope()
{
    m_impl->enterScope();
}

bool NameTable::exitScope()
{
    return m_impl->exitScope();
}

bool NameTable::declare(const string& id, int lineNum)
{
    return m_impl->declare(id, lineNum);
}

int NameTable::find(const string& id) const
{
    return m_impl->find(id);
}
