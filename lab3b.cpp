#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdlib>

using namespace std;
int num_blocks=0, num_inodes=0,block_size=0,inode_size=0,non_rnode=0,free_i=0,free_b=0,b_map=0,i_map=0,inodes_block=0;

//IDEAS: Have a shell script that replaces comma(',') with space maybe(' ')

void read_file(ifstream& fin)
{
   int i=0;
  string super_buff="",head="",line;

  while(getline(fin,line))
    {
      i=0,head="";
      while (line[i]!=',')
	{
	  head+=line[i];
	  i++;
	}
      
      if(head=="SUPERBLOCK")
	{
	  i++;
	  
	  while(line[i]!=',')
	    {
	      super_buff+=line[i];
	      i++;
	    }
	  num_blocks=atoi(super_buff.c_str());
	  super_buff="";
	  i++;
	  
	  while(line[i]!=',')
	    {
	      super_buff+=line[i];
	      i++;
	    }
	  num_inodes=atoi(super_buff.c_str());
	  super_buff="";
	  i++;
	  
	  while(line[i]!=',')
	    {
	      super_buff+=line[i];
	      i++;
	    }
	  block_size=atoi(super_buff.c_str());
	  super_buff="";
	  i++;
	  
	  while(line[i]!=',')
	    {
	      super_buff+=line[i];
	      i++;
	    }
	  inode_size=atoi(super_buff.c_str());
	  super_buff="";
	  i++;

	   while(line[i]!=',')
	     i++;
	   i++;
	   while(line[i]!=',')
	     i++;
	   i++;
	   
	   while(line[i]!='\0')//|| line[i]!='\n')
	     {
	       super_buff+=line[i];
	       i++;
	     }
	   non_rnode=atoi(super_buff.c_str());
	

	   cout<<"Blocks: "<<num_blocks<<" Nodes: "<<num_inodes<<" Bsize: "<<block_size<<" Inode size: "<<inode_size<<" Non-reserved: "<<non_rnode;
	   cout<<endl;
	}
      else if(head=="GROUP")
	{
	  super_buff="";
	  i++;
	  while(line[i]!=',')
	    i++;
	  i++;
	  while(line[i]!=',')
	    i++;
	  i++;
	  while(line[i]!=',')
	    i++;
	  i++;

	  while(line[i]!=',')
	    {
	      super_buff+=line[i];
	      i++;
	    }
	  free_b=atoi(super_buff.c_str());
	  super_buff="";
	  i++;

	  while(line[i]!=',')
	    {
	      super_buff+=line[i];
	      i++;
	    }
	  free_i=atoi(super_buff.c_str());
	  super_buff="";
	  i++;
	  
	  while(line[i]!=',')
	    {
	      super_buff+=line[i];
	      i++;
	    }
	  b_map=atoi(super_buff.c_str());
	  super_buff="";
	  i++;
	  
	  while(line[i]!=',')
	    {
	      super_buff+=line[i];
	      i++;
	    }
	  i_map=atoi(super_buff.c_str());
	  super_buff="";
	  i++;

	  while(line[i]!='\0')
	    {
	      super_buff+=line[i];
	      i++;
	    }
	  inodes_block=atoi(super_buff.c_str());
	  super_buff="";
	  i++;

	  cout<<free_b<<" "<<free_i<<" "<<b_map<<" "<<i_map<<" "<<inodes_block<<endl;
	}
	  
	  
	  
	  


    }
}


int main(int argc, char** argv)
{
 
  //Getting the csv file:
  if(argc !=2)
    {
      cout<<"Error: Wrong number of arguments\n";
      return 1;
    }
 
 
  ifstream fin(argv[1],ifstream::in);
 
  //Reading file here:
  read_file(fin);

 


  /*********************READ SUPERBLOCK****************************/

  return 0;

}
  
