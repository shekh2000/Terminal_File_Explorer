#include<bits/stdc++.h>
#include<termios.h>
//#include<conio.h>
//#include <conio.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include<iostream>
#include<unistd.h>
#include<dirent.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fstream>
#include <sys/ioctl.h>
#include <iomanip>
#include <sys/wait.h>
#include<pwd.h>
#include<grp.h>
#include <fcntl.h>
using namespace std;

int up=1;
int dow=0;
int j=0;
int adjust=0;
int flag=0;
int flag1=0;
int rows=0;
int cols=0;
int i;
vector<string> v;
vector<int> df;
stack<string>s1;
stack<string>s2;
string d_i="";
string r_f;
int r_k=0;
string inp_comma="";
string dummy_co;
int flag2=0;
struct termios orig_termios;



/*char editorReadKey()
{
   int nread;
   char c;
   while ((nread = read(STDIN_FILENO, &c, 1)) != 1) 
   {
      //if (nread == -1 && errno != EAGAIN) die("read");
   }
   return c;
}*/

// function  to clear the screen and position the cursor at the upper left corner
void editorRefreshScreen()
{
    write(STDOUT_FILENO, "\x1b[2J", 4);
    write(STDOUT_FILENO, "\x1b[H", 3);
}

// function to disable the raw mode when exiting
void disableRawMode()
{
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

//function to enter into the raw mode so whatever we type it is not visible
void enableRawMode()
{
    tcgetattr(STDIN_FILENO,&orig_termios);
    struct termios raw=orig_termios;

    raw.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO,TCSAFLUSH,&raw);
}

//this function is used to get the details of directory in normal mode
// vector is used to store the name of file in a current directory
void directory_details(string f)
{
    v.clear();
    df.clear();
    DIR *dr;
    struct dirent *en;
    dr = opendir(f.c_str());
     if(dr)
    {
       // vector<string>v;
        int x=0;
        while ((en = readdir(dr)) != NULL)
        {
            v.push_back(en->d_name);
        }
        sort(v.begin(),v.end());
        int j_k=0;

        for(i=adjust;i<v.size();i++)
        {
                    r_k++;
                    if(j==i)
                    cout<<"-->";
                    else
                    cout<<"   ";
		    struct stat info_file;
		    stat((f+"/"+v[i]).c_str(),&info_file);
		    
		    cout<<left<<setw(15)<<v[i]<<left<<setw(15)<<info_file.st_size<<left<<setw(15);
                       if( S_ISDIR(info_file.st_mode) )
                       {
                           df.push_back(0);
                       }
                       else
                       {
                           df.push_back(1);
                       }
                       struct passwd *pw = getpwuid(info_file.st_uid);
		struct group  *gr = getgrgid(info_file.st_gid);
		cout<<pw->pw_name;
		cout <<setw(10);
		cout<<gr->gr_name;
		cout <<setw(10);
                   // cout<<df[j_k]<<left<<setw(15);
		    cout<<( (S_ISDIR(info_file.st_mode)) ? "d" : "-");
		    cout<<( (info_file.st_mode & S_IRUSR) ? "r" : "-");
		    cout<<( (info_file.st_mode & S_IWUSR) ? "w" : "-");
		    cout<<( (info_file.st_mode & S_IXUSR) ? "x" : "-");
		    cout<<( (info_file.st_mode & S_IRGRP) ? "r" : "-");
		    cout<<( (info_file.st_mode & S_IWGRP) ? "w" : "-");
		    cout<<( (info_file.st_mode & S_IXGRP) ? "x" : "-");
		    cout<<( (info_file.st_mode & S_IROTH) ? "r" : "-");
		    cout<<( (info_file.st_mode & S_IWOTH) ? "w" : "-");
		    cout<<( (info_file.st_mode & S_IXOTH) ? "x" : "-");
		    cout<<"                  "<<ctime(&info_file.st_mtime);
                    j_k++;
                    if(i!=0)
                    {
                       if(i%(rows-5)==0)
                       break;
                    }
	}
        int x_o=rows-j_k-2;
        while(x_o)
        {
            cout<<endl;
            x_o--;
        }        
       // "NORMAL MODE:  "<<f;
        string d_f="NORMAL MODE: "+f;
        int f_j=d_f.length();
        write(STDOUT_FILENO, d_f.c_str(), f_j);
    }
  /*  if(v.size()>(rows-5) && i==(row-5))
    {
          cout<<endl;
          cout<<endl;
          cout<<endl;
          cout<<"NORMAL MODE :"<<f<<endl;
    }
    else if(v.size()<=(row-5))
    {
          int f_d=row-v.size();
          while(f_d!=1)
          cout<<"\n";
          cout<<"NORMAL MODE :"<<"   "<<f<<endl;
    }*/
    closedir(dr);
}

//this function is used to get the size of our terminal window
int getWindowSize(int *rows, int *cols) {
  struct winsize ws;
  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
    return -1;
  } else {
    *cols = ws.ws_col;
    *rows = ws.ws_row;
    return 0;
  }
}

// function to read the key input
char readkey()
{   int nk;
    char c;
    while ((nk=read(STDIN_FILENO, &c, 1))!=1)
    {
       getWindowSize(&rows, &cols);
       editorRefreshScreen();
    }
    return c;
}

//function to specify whether it is a directory or a file
//for directory return 1
//for file return 0
bool isDir(string path){
	struct stat st;
	stat(path.c_str(), &st);
	string ch = ( (S_ISDIR(st.st_mode)) ? "d" : "-");
	return ch[0] == 'd';
	
}

//function to get the directory details in command mode
void directory_det(string f)
{
    v.clear();
    df.clear();
    DIR *dr;
    struct dirent *en;
    dr = opendir(f.c_str());
     if(dr)
    {
       // vector<string>v;
        int x=0;
        while ((en = readdir(dr)) != NULL)
        {
            v.push_back(en->d_name);
        }
        sort(v.begin(),v.end());
        int j_k=0;

        for(i=0;i<v.size();i++)
        {
                    r_k++;
                    //if(j==i)
                    //cout<<"-->";
                    //else
                    //cout<<"   ";
		    struct stat info_file;
		    stat((f+"/"+v[i]).c_str(),&info_file);
		    
		    cout<<left<<setw(25)<<v[i]<<left<<setw(15)<<info_file.st_size<<write(STDOUT_FILENO,"B",1)<<left<<setw(15);
                       if( S_ISDIR(info_file.st_mode) )
                       {
                           df.push_back(0);
                       }
                       else
                       {
                           df.push_back(1);
                       }
                       struct passwd *pw = getpwuid(info_file.st_uid);
		struct group  *gr = getgrgid(info_file.st_gid);
		cout<<pw->pw_name;
		cout <<setw(10);
		cout<<gr->gr_name;
		cout <<setw(10);
                    //cout<<df[j_k]<<left<<setw(15);
		    cout<<( (S_ISDIR(info_file.st_mode)) ? "d" : "-");
		    cout<<( (info_file.st_mode & S_IRUSR) ? "r" : "-");
		    cout<<( (info_file.st_mode & S_IWUSR) ? "w" : "-");
		    cout<<( (info_file.st_mode & S_IXUSR) ? "x" : "-");
		    cout<<( (info_file.st_mode & S_IRGRP) ? "r" : "-");
		    cout<<( (info_file.st_mode & S_IWGRP) ? "w" : "-");
		    cout<<( (info_file.st_mode & S_IXGRP) ? "x" : "-");
		    cout<<( (info_file.st_mode & S_IROTH) ? "r" : "-");
		    cout<<( (info_file.st_mode & S_IWOTH) ? "w" : "-");
		    cout<<( (info_file.st_mode & S_IXOTH) ? "x" : "-");
		    cout<<"                  "<<ctime(&info_file.st_mtime);
                    j_k++;
                    /*if(i!=0)
                    {
                       if(i%(rows-5)==0)
                       break;
                    }*/
	}
        int x_o=rows-j_k-2;
        while(x_o)
        {
            cout<<endl;
            x_o--;
        }        
       // "NORMAL MODE:  "<<f;
        string d_f="COMMAND MODE: ";
        int f_j=d_f.length();
        write(STDOUT_FILENO, d_f.c_str(), f_j);
    }
}

//function to go to a particular directory specified by goto command in command mode
void goto_co(string f)
{
    r_f="";
    int i=0;
    while(f[i+7]!='>')
    {
       r_f=r_f+f[i+7];
       i++;
    }
    //directory_det(r_f);
}

//function to create a file when path is given and name of file is given in command mode
void create_file(string f)
{
    string file_name;
    int i=0;
    while(f[i+14]!='>')
    {
       file_name=file_name+f[i+14];
       i++;
    }
    i++;
    i++;
    string dest_path="";
    while(f[i+14]!='>')
    {
       dest_path=dest_path+f[i+14];
       i++;
    }
    file_name=dest_path+"/"+file_name;
    int c=open(file_name.c_str(),O_CREAT,S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
   /* if(c>0)
    {
       string res="success";
       int re_l=res.length();
       write(STDOUT_FILENO,res.c_str(),re_l);
    }*/
}

//function to create a new directory when path is given in command mode
void create_dir(string f)
{
    string dire_name;
    int i=0;
    while(f[i+13]!='>')
    {
       dire_name=dire_name+f[i+13];
       i++;
    }
    i++;
    i++;
    string dest_path="";
    while(f[i+13]!='>')
    {
       dest_path=dest_path+f[i+13];
       i++;
    }
    dire_name=dest_path+"/"+dire_name;
    int c=mkdir(dire_name.c_str(),S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
}


void copy(string f)
{
   string direct_path;
   int x=f.length()-1;
   while(f[x]!='~')
   {
      direct_path=f[x]+direct_path;
      x--;
   }
   string file_pt;
   x--;
   while(f[x]!=' ')
   {
      file_pt=f[x]+file_pt;
      x--;
   }
   int rea_f_re;
   int wri_f_re;
   
}

//function to delete a file when path is given in command mode
void delet_fil(string f)
{
   string pat;
   int i=0;
   while(f[i+14]!='\0')
   {
      pat=pat+f[i+14];
      i++;
   }
   remove(pat.c_str());
}

//function to rename a file when path is given in command mode
void rename(string f)
{
   string oldname;
   string newname;
  
   int i=f.length()-1;
   while(f[i]!=' ')
   {
       newname=f[i]+newname;
       i--;
   }
   i--;
   while(f[i]!=' ')
   {
       oldname=f[i]+oldname;
       i--;
   }
   rename(oldname.c_str(),newname.c_str());
}

//funciton for command mode
//return true for quitting from a command mode
//return false for continuing in normal
bool command_mode()
{
    while(1)
    {
       editorRefreshScreen();
       directory_det(r_f);
       int str_length=inp_comma.length();
       write(STDOUT_FILENO, inp_comma.c_str(),str_length);
       char c;
       c=readkey();
       if(c==27)
       {
           return false;
           //break;
       }
       else if(c=='\n')
       {
           dummy_co=inp_comma;
           inp_comma="";
           if(dummy_co[2]=='g' && dummy_co[3]=='o' && dummy_co[4]=='t' && dummy_co[5]=='o')
           {
               goto_co(dummy_co);
           }
           if(dummy_co[2]=='c' && dummy_co[3]=='r' && dummy_co[4]=='e' && dummy_co[9]=='f')
           {
               create_file(dummy_co);
           }
           if(dummy_co[2]=='c' && dummy_co[3]=='r' && dummy_co[4]=='e' && dummy_co[9]=='d')
           {
               create_dir(dummy_co);
           }
           if(dummy_co[2]=='c' && dummy_co[3]=='o' && dummy_co[4]=='p' && dummy_co[5]=='y')
           {
               copy(dummy_co);
           }
           if(dummy_co[9]=='f' && dummy_co[10]=='i' && dummy_co[11]=='l' && dummy_co[12]=='e')
           {
               delet_fil(dummy_co);
           }
           if(dummy_co[2]=='r' && dummy_co[3]=='e' && dummy_co[4]=='n' && dummy_co[5]=='a')
           {
               rename(dummy_co);
           }
           if(dummy_co[2]=='q' && dummy_co[3]=='u' && dummy_co[4]=='i' && dummy_co[5]=='t')
           {
               return true;
           }
       }
       else
       {
           inp_comma=inp_comma+c;
       }
   }
}
int main()
{
    enableRawMode();
    editorRefreshScreen();
    struct passwd *pw=getpwuid(getuid());
    r_f=pw->pw_dir;
    s1.push(r_f);  
    //string r_f=".";  
    getWindowSize(&rows, &cols);   
        while(1)
        {
                editorRefreshScreen();
                //getWindowSize(&rows, &cols);
		directory_details(r_f);
      
		char c,d,e;
		//read(STDIN_FILENO,&c,1);
		c=readkey();
                if(c=='q')
                {
                  break;
                }
                if(c==';')
                {
                  if(command_mode())
                  {
                     flag2=1;
                  }
                  flag1=1;
                }
                if(flag2==1)
                break;
                if(c=='\n')
                {
                   string temp=r_f;
                   r_f=r_f+"/"+v[j];
                  // s1.push(r_f);
                   if(isDir(r_f))
                   {
                        adjust=0;
                        //directory_details(r_f);
                        s1.push(r_f);
                        j=0;
                   }
                   else
                   {
                      
                      int pid = fork();
                      if (pid == 0)
                      {
                           execl("/usr/bin/vi", "vi", r_f.c_str(), (char *)0);
                           exit(1);
                      }
                      else
                      {
				int wv;
				int st=0;
				while((wv=waitpid(pid, &st, 0))!=pid){
					if(wv==-1) exit(1);
				}
                                r_f=temp;
                                j=0;
                                adjust=0;
		      }
                   }
                   flag1=1;
                   //j=0;
                }
                if(c==127)
                {
                   struct passwd *pw=getpwuid(getuid());
                   string y_u=pw->pw_dir;
                   if(r_f==y_u)
                   {
                      d_i=r_f;
                      int x_i=d_i.length()-1;
                      while(d_i[x_i]!='/')
                      {
                         d_i.pop_back();
                         x_i--;
                      }
                      d_i.pop_back();
                      s2.push(s1.top());
                      s1.pop();
                      s1.push(d_i);
                      r_f=s1.top();
                   }
                   else if(r_f==d_i)
                   {
                      s2.push(s1.top());
                      s1.pop();
                      s1.push("/");
                      r_f=s1.top();
                   }
                   /*if(!s1.empty() && s1.size()>1)
                   {
                      s2.push(s1.top());
                      s1.pop();
                   }*/
                     //d_i=s1.top();
                   else
                   {
                      if(r_f!="/")
                      {
                        s2.push(s1.top());
                        s1.pop();
                        r_f=s1.top();
                      }
                      //d_i=r_f;
                      /*if(r_f!="/")
                      {
                         int x_i=r_f.length()-1;
                         while(r_f[x_i]!='/')
                         {
                            r_f.pop_back();
                            x_i--;
                         }
                         r_f.pop_back();
                      }*/
                    }
                    flag1=1;
                    j=0;
                    adjust=0;
                }
                if(c==104)
                {
                   //cout<<"123"<<endl;
                   struct passwd *pw=getpwuid(getuid());
                   r_f=pw->pw_dir;
                   s1.push(r_f);
                   flag1=1;
                }
                if(flag1==0)
                {
                   cin>>d;
                   cin>>e;
                }
	        if((c==27)&&(d==91))
	        {
			if (e==66)
		        {
		            j=j+1;
		            if(j==(v.size()))
		            j=v.size()-1;
		            if(j%(rows-4)==0)
		            {
		               //j=0;
		               adjust=j;
		               //editorRefreshScreen();
		            }
		        }
			if (e==65) 
		        { 
		            j=j-1; 
		            if(j==-1)
		            j=0;
		            if(j==(adjust-1))
		            {
                                j=0;
		                adjust=0;
		            }
		        }
		        if (e==68)
		        {
		           if(!s1.empty() && s1.size()>1)
		           {
		             s2.push(s1.top());
		             s1.pop();
                             r_f=s1.top();
                             j=0;
                             adjust=0;
		           }
		        }
		        if (e==67)
		        {
		           if(!s2.empty())
		           {
		              s1.push(s2.top());
		              s2.pop();
                              r_f=s1.top();
                              j=0;
                              adjust=0;
		           }
		        }
      	        }
                flag1=0;
        }
        disableRawMode();
           
    return 0;
}










