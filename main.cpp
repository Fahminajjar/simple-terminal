#include <iostream>
#include <pthread.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <fcntl.h>
#include <unistd.h>
#include <string>
#include <vector>

using namespace std;

// the root for this terminal is Desktop of the machine!
string root_dir;
string current_dir;
string username;

vector<string> split(string str);
string getcmd(string cmd);
void routing(string cmd);
string cookArg(string arg);
void initializeUser();

void cat(string filepath);
void ls(string option = " ", string path = current_dir);
void cp(string src, string dest, string option = " ");
void mv(string src, string dest, string option = " ");
void mkdir(string dir);
void pwd();
void cd(string dir = root_dir);
void help();
void whoami();

vector<string> split(string str){
    vector<string> words;
    words.push_back("");
    
    int x = 0;
    for(int i = 0; i < str.size(); i++){
        if(str[i] == ' '){
            x++;
            words.push_back("");
        }
        else
            words[x].push_back(str[i]);
    }
    return words;
}


string getcmd(string cmd){
    vector<string> words = split(cmd);
    return words[0];
}


void routing(string cmd){
    vector<string> argv = split(cmd);

    for(int i = 1; i < argv.size(); i++){
        if(argv[i][0] != '-')
            argv[i] = cookArg(argv[i]);
    }
    
    
    if(argv[0] == "ls"){
        if(argv.size() == 1)
            ls();
        else if(argv.size() > 1 && argv[1][0] == '-'){
            if(argv.size() == 2)
                ls(argv[1]);
            else if(argv.size() == 3)
                ls(argv[1], argv[2]);
            else{
                cout << "Wrong number of arguments!";
                return;
            }
        }
        else
            ls(" ", argv[1]);
        
        return;
    }
    
    else if(argv[0] == "cd"){
        if(argv.size() == 1)
            cd();
        else if(argv.size() == 2)
            cd(argv[1]);
        else
            cout << "Wrong number of arguments!";
        
        return;
    }
    
    else if(argv[0] == "cp"){
        if(argv.size() == 3){
            cp(argv[1], argv[2]);
        }
        else if(argv.size() == 4 && argv[1][0] == '-'){
            cp(argv[2], argv[3], argv[1]);
        }
        else{
            cout << "Wrong number of arguments!";
            return;
        }
        return;
    }
    
    
    else if(argv[0] == "mv"){
        if(argv.size() == 3){
            mv(argv[1], argv[2]);
        }
        else if(argv.size() == 4 && argv[1][0] == '-'){
            mv(argv[2], argv[3], argv[1]);
        }
        else{
            cout << "Wrong number of arguments!";
            return;
        }
        return;
    }
    else if(argv[0] == "pwd"){
        pwd();
        return;
    }
    else if(argv[0] == "help"){
        help();
        return;
    }
    else if(argv[0] == "whoami"){
        whoami();
        return;
    }
    
    else if(argv[0] == "exit")
        exit(0);
    
    
    if(argv.size() == 0)
        return;
    else if(argv.size() == 1){
        cout << "Command not found";
        return;
    }
    
    if(argv[0] == "cat")
        cat(argv[1]);
    
    else if(argv[0] == "mkdir")
        mkdir(argv[1]);

    else
        cout << "Command not found!";
}


string cookArg(string arg){
    
    if(arg == "..")
        arg = current_dir + "..";
    else if(arg[0] == '.')
        arg = current_dir;
    else if(arg[0] != '/')
        arg = current_dir + arg;
    
    return arg;
}


void initializeUser(){
    cout << "Welcome to myTerminal!" << endl;
    cout << "Please enter username: ";
    
    cin >> username;
    
    root_dir = "/Users/" + username + "/";
    current_dir = root_dir;
    cout << endl;
}


// ___________________________________________  //


void cat(string filepath){
    int status;
    const char * arg = filepath.c_str();
    
    pid_t p  = fork();
    if(p == 0){
        execl("/bin/cat", "cat", arg, 0);
    }else{
        wait(&status);
        sleep(1);
    }
}



void ls(string option, string path){
    int status;
    const char * arg = path.c_str();
    const char * opt = option.c_str();
    
    pid_t p = fork();
    if(p == 0){
        if(option != " ")
            execl("/bin/ls", "ls", opt, arg, 0);
        else
            execl("/bin/ls", "ls", arg, 0);
    }else{
        wait(&status);
        sleep(1);
    }
}


void cp(string src, string dest, string option){
    const char * sr = src.c_str();
    const char * ds = dest.c_str();
    const char * opt = option.c_str();
    
    int status;
    
    pid_t p = fork();
    if(p == 0){
        if(option != " ")
            execl("/bin/cp", "cp", opt, sr, ds, 0);
        else
            execl("/bin/cp", "cp", sr, ds, 0);
    }else{
        wait(&status);
        sleep(1);
    }
}


void mv(string src, string dest, string option){
    const char * sr = src.c_str();
    const char * ds = dest.c_str();
    const char * opt = option.c_str();

    int status;
    
    pid_t p = fork();
    if(p == 0){
        if(option != " ")
            execl("/bin/mv", "mv", opt, sr, ds, 0);
        else
            execl("/bin/mv", "mv", sr, ds, 0);
    }else{
        wait(&status);
        sleep(1);
    }
}


void mkdir(string dir){
    const char * d = dir.c_str();
    
    int status;
    
    pid_t p = fork();
    if(p == 0){
        execl("/bin/mkdir", "mkdir", d, 0);
    }else{
        wait(&status);
        sleep(1);
    }
}


void pwd(){
    cout << current_dir;
}


void cd(string dir){
    if(dir[dir.size() - 1] != '/')
        dir += "/";
    
    const char * d = dir.c_str();
    current_dir = d;
}


void whoami(){
    cout << username;
}


void help(){
    cout << "\ncommand name: cat";
    cout << endl;
    cout << endl;
    cout << "void cat(string filepath)";
    cout << endl;
    cout << "cat command displays the content of the path file followed by the command";
    cout << endl;
    cout << "-------------------------------------------------------";
    
    cout << "\ncommand name: ls";
    cout << endl;
    cout << endl;
    cout << "void ls(string path = cuurent directory, string option[optional] )";
    cout << endl;
    cout << "ls command displays the content of the current directory";
    cout << endl;
    cout << "-------------------------------------------------------";
    
    cout << "\ncommand name: cp";
    cout << endl;
    cout << endl;
    cout << "void cp(string src, string dest, string option[optional] )";
    cout << endl;
    cout << "copies a file from the given destination to the diven source";
    cout << endl;
    cout << "-------------------------------------------------------";
    
    cout <<"\ncommand name: mv";
    cout<<endl;
    cout<<endl;
    cout<<"void mv(string src, string dest, string option[optional] )";
    cout<<endl;
    cout<<"moves a file from the given source to the given destination";
    cout << endl;
    cout<<"-------------------------------------------------------";
    
    cout <<"\ncommand name: mkdir";
    cout<<endl;
    cout<<endl;
    cout<<"void mkdir(string dir)";
    cout<<endl;
    cout<<"creates a directory if not exists";
    cout << endl;
    cout<<"-------------------------------------------------------";
    
    cout <<"\ncommand name: pwd";
    cout<<endl;
    cout<<endl;
    cout<<"void pwd()";
    cout<<endl;
    cout<<"displays current working directory";
    cout << endl;
    cout<<"-------------------------------------------------------";
    
    cout <<"\ncommand name: cd";
    cout<<endl;
    cout<<endl;
    cout<<"void cd(string dir = root_dir [optional] )";
    cout<<endl;
    cout<<"changes current directory";
    cout << endl;
    cout<<"-------------------------------------------------------";
    
    cout <<"\ncommand name: whoami";
    cout<<endl;
    cout<<endl;
    cout<<"void whoami()";
    cout<<endl;
    cout<<"displays the current user";
    cout << endl;
    cout<<"-------------------------------------------------------";

    
}


int main(){
    
    initializeUser();
    
    string cmd;
    while(1){
        cout << current_dir + ": " + username + "$ ";
        getline(cin, cmd);
        routing(cmd);
        cout << endl;
    }

    
    return 0;
}
