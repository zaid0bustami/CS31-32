// The file commands.txt should contain a sequence of lines of the form
//   {                    which requests a call to enterScope
//   }                    which requests a call to exitScope
//   identifier number    which requests a call to declare(identifier,number)
//   identifier           which requests a call to find(identifier)

#include "NameTable.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <cassert>
using namespace std;

const char* COMMAND_FILE_NAME = "commands.txt";

class SlowNameTable
{
public:
    void enterScope();
    bool exitScope();
    bool declare(const string& id, int lineNum);
    int find(const string& id) const;
private:
    vector<string> m_ids;
    vector<int> m_lines;
};

struct Command
{
    static Command* create(string line, int lineno);
    Command(string line, int lineno) : m_line(line), m_lineno(lineno) {}
    virtual ~Command() {}
    virtual void execute(NameTable& nt) const = 0;
    virtual bool executeAndCheck(NameTable& nt, SlowNameTable& snt) const = 0;
    string m_line;
    int m_lineno;
};

void extractCommands(istream& dataf, vector<Command*>& commands);
string testCorrectness(const vector<Command*>& commands);
void testPerformance(const vector<Command*>& commands);

void testHashTable()
{
    NameTable t;
    t.enterScope();
    t.declare("hello", 5);
    t.declare("testing", 7);
    t.enterScope();
    t.declare("hello", 9);
    t.declare("testing", 10);
    assert(t.find("hello") == 9);
    assert(t.find("testing") == 10);
    t.exitScope();
    assert(t.find("hello") == 5);
    assert(t.find("testing") == 7);
    assert(t.declare("hello", 20) == false);
    assert(t.find("oooo") == -1);
    t.exitScope();
    assert(t.exitScope() == false);

    NameTable nt;
    assert(nt.declare("alpha", 1));
    assert(nt.declare("beta", 2));
    assert(nt.declare("p1", 3));
    assert(nt.find("alpha") == 1);   // the alpha declared in line 1
    assert(nt.declare("p2", 4));
    assert(nt.find("beta") == 2);    // the beta declared in line 2
    assert(nt.declare("p3", 5));
    assert(nt.find("gamma") == -1);  // Error!  gamma hasn't been declared
    assert(nt.declare("f", 6));
    nt.enterScope();
    assert(nt.declare("beta", 7));
    assert(nt.declare("gamma", 8));
    assert(nt.find("alpha") == 1);   // the alpha declared in line 1
    assert(nt.find("beta") == 7);    // the beta declared in line 7
    assert(nt.find("gamma") == 8);   // the gamma declared in line 8
    nt.enterScope();
    assert(nt.declare("alpha", 13));
    assert(nt.declare("beta", 14));
    assert(!nt.declare("beta", 15)); // Error! beta already declared
    assert(nt.find("alpha") == 13);  // the alpha declared in line 13
    assert(nt.exitScope());
    assert(nt.find("alpha") == 1);   // the alpha declared in line 1
    assert(nt.find("beta") == 7);    // the beta declared in line 7
    nt.enterScope();
    assert(nt.declare("beta", 21));
    assert(nt.find("beta") == 21);   // the beta declared in line 21
    assert(nt.exitScope());
    assert(nt.exitScope());
    assert(nt.declare("p4", 25));
    assert(nt.find("alpha") == 1);   // the alpha declared in line 1
    assert(nt.declare("p5", 26));
    assert(nt.find("beta") == 2);    // the beta declared in line 2
    assert(nt.declare("p6", 27));
    assert(nt.find("gamma") == -1); // Error! gamma is not in scope
    assert(nt.declare("main", 28));
    nt.enterScope();
    assert(nt.declare("beta", 29));
    assert(nt.find("beta") == 29);   // the beta declared in line 29
    assert(nt.find("f") == 6);       // the f declared in line 6
    assert(nt.exitScope());


}

int main()
{
    testHashTable();
    vector<Command*> commands;

    // Basic correctness test

    // Notice that this test string contains an excess "}" command at the
    // end.  That tests whether you correctly return false if you call
    // exitScope more times than you call enterScope.

    istringstream basicf(
        "alpha 1\n"
        "beta 2\n"
        "p1 3\n"
        "alpha\n"
        "p2 4\n"
        "beta\n"
        "p3 5\n"
        "gamma\n"
        "f 6\n"
        "{\n"
        "beta 7\n"
        "gamma 8\n"
        "alpha\n"
        "beta\n"
        "gamma\n"
        "{\n"
        "alpha 13\n"
        "beta 14\n"
        "beta 15\n"
        "alpha\n"
        "}\n"
        "alpha\n"
        "beta\n"
        "{\n"
        "beta 21\n"
        "beta\n"
        "}\n"
        "}\n"
        "p4 25\n"
        "alpha\n"
        "p5 26\n"
        "beta\n"
        "p6 27\n"
        "gamma\n"
        "main 28\n"
        "{\n"
        "beta 29\n"
        "beta\n"
        "f\n"
        "}\n"
        "}\n"
    );
    extractCommands(basicf, commands);

    cout << "Basic correctness test: " << flush;
    cout << testCorrectness(commands) << endl;

    for (size_t k = 0; k < commands.size(); k++)
        delete commands[k];
    commands.clear();

    // Thorough correctness and performance tests

    ifstream thoroughf(COMMAND_FILE_NAME);
    if (!thoroughf)
    {
        cout << "Cannot open " << COMMAND_FILE_NAME
            << ", so cannot do thorough correctness or performance tests!"
            << endl;
        return 1;
    }
    extractCommands(thoroughf, commands);

    cout << "Thorough correctness test: " << flush;
    cout << testCorrectness(commands) << endl;

    cout << "Performance test on " << commands.size() << " commands: " << flush;
    testPerformance(commands);

    for (size_t k = 0; k < commands.size(); k++)
        delete commands[k];
}

struct EnterScopeCmd : public Command
{
    EnterScopeCmd(string line, int lineno)
        : Command(line, lineno)
    {}
    virtual void execute(NameTable& nt) const
    {
        nt.enterScope();
    }
    virtual bool executeAndCheck(NameTable& nt, SlowNameTable& snt) const
    {
        nt.enterScope();
        snt.enterScope();
        return true;
    }
};

struct ExitScopeCmd : public Command
{
    ExitScopeCmd(string line, int lineno)
        : Command(line, lineno)
    {}
    virtual void execute(NameTable& nt) const
    {
        nt.exitScope();
    }
    virtual bool executeAndCheck(NameTable& nt, SlowNameTable& snt) const
    {
        return nt.exitScope() == snt.exitScope();
    }
};

struct DeclareCmd : public Command
{
    DeclareCmd(string id, int lineNum, string line, int lineno)
        : Command(line, lineno), m_id(id), m_lineNum(lineNum)
    {}
    virtual void execute(NameTable& nt) const
    {
        nt.declare(m_id, m_lineNum);
    }
    virtual bool executeAndCheck(NameTable& nt, SlowNameTable& snt) const
    {
        return nt.declare(m_id, m_lineNum) == snt.declare(m_id, m_lineNum);
    }
    string m_id;
    int m_lineNum;
};

struct FindCmd : public Command
{
    FindCmd(string id, string line, int lineno)
        : Command(line, lineno), m_id(id)
    {}
    virtual void execute(NameTable& nt) const
    {
        nt.find(m_id);
    }
    virtual bool executeAndCheck(NameTable& nt, SlowNameTable& snt) const
    {
        return nt.find(m_id) == snt.find(m_id);
    }
    string m_id;
};

Command* Command::create(string line, int lineno)
{
    istringstream iss(line);
    string field1;
    if (!(iss >> field1))
        return nullptr;
    if (field1 == "{")
        return new EnterScopeCmd(line, lineno);
    if (field1 == "}")
        return new ExitScopeCmd(line, lineno);
    int field2;
    if (!(iss >> field2))
        return new FindCmd(field1, line, lineno);
    return new DeclareCmd(field1, field2, line, lineno);
}

void extractCommands(istream& dataf, vector<Command*>& commands)
{
    string commandLine;
    int commandNumber = 0;
    while (getline(dataf, commandLine))
    {
        commandNumber++;
        Command* cmd = Command::create(commandLine, commandNumber);
        if (cmd != nullptr)
            commands.push_back(cmd);
    }
}

string testCorrectness(const vector<Command*>& commands)
{
    NameTable nt;
    SlowNameTable snt;
    for (size_t k = 0; k < commands.size(); k++)
    {
        // Check if command agrees with our behavior

        if (!commands[k]->executeAndCheck(nt, snt))
        {
            ostringstream msg;
            msg << "*** FAILED *** line " << commands[k]->m_lineno
                << ": \"" << commands[k]->m_line << "\"";
            return msg.str();
        }
    }
    return "Passed";
}

//========================================================================
// Timer t;                 // create a timer and start it
// t.start();               // (re)start the timer
// double d = t.elapsed();  // milliseconds since timer was last started
//========================================================================

#include <chrono>

class Timer
{
public:
    Timer()
    {
        start();
    }
    void start()
    {
        m_time = std::chrono::high_resolution_clock::now();
    }
    double elapsed() const
    {
        std::chrono::duration<double, std::milli> diff =
            std::chrono::high_resolution_clock::now() - m_time;
        return diff.count();
    }
private:
    std::chrono::high_resolution_clock::time_point m_time;
};

void testPerformance(const vector<Command*>& commands)
{
    double endConstruction;
    double endCommands;

    Timer timer;
    {
        NameTable nt;

        endConstruction = timer.elapsed();

        for (size_t k = 0; k < commands.size(); k++)
            commands[k]->execute(nt);

        endCommands = timer.elapsed();
    }

    double end = timer.elapsed();

    cout << end << " milliseconds." << endl
        << "   Construction: " << endConstruction << " msec." << endl
        << "       Commands: " << (endCommands - endConstruction) << " msec." << endl
        << "    Destruction: " << (end - endCommands) << " msec." << endl;
}

void SlowNameTable::enterScope()
{
    // Extend the id vector with an empty string that
    // serves as a scope entry marker.

    m_ids.push_back("");
    m_lines.push_back(0);
}

bool SlowNameTable::exitScope()
{
    // Remove ids back to the last scope entry.

    while (!m_ids.empty() && m_ids.back() != "")
    {
        m_ids.pop_back();
        m_lines.pop_back();
    }
    if (m_ids.empty())
        return false;

    // Remove the scope entry marker itself.

    m_ids.pop_back();
    m_lines.pop_back();
    return true;
}

bool SlowNameTable::declare(const string& id, int lineNum)
{
    if (id.empty())
        return false;

    // Check for another declaration in the same scope.
    // Return if found, break out if encounter the scope
    // entry marker.

    size_t k = m_ids.size();
    while (k > 0)
    {
        k--;
        if (m_ids[k].empty())
            break;
        if (m_ids[k] == id)
            return false;
    }

    // Save the declaration

    m_ids.push_back(id);
    m_lines.push_back(lineNum);
    return true;
}

int SlowNameTable::find(const string& id) const
{
    if (id.empty())
        return -1;

    // Search back for the most recent declaration still
    // available.

    size_t k = m_ids.size();
    while (k > 0)
    {
        k--;
        if (m_ids[k] == id)
            return m_lines[k];
    }
    return -1;
}
