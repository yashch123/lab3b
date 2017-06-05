#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;
int num_blocks=0, num_inodes=0,block_size=0,inode_size=0,non_rnode=0,free_i=0,free_b=0,b_map=0,i_map=0,inodes_block=0;

vector<int> bfree,ifree,all_inode_num,inode_mode,inode_alloc,inode_unalloc;

void read_file(ifstream& fin)
{
  int i=0,j=0,k=0;
  string super_buff="",temp_buff="",head="",line;
   
  
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
      else if(head=="BFREE")
	{
	  i++;
	  while(line[i]!='\0')
	    {
	      super_buff+=line[i];
	      i++;
	    }
	  bfree.push_back(atoi(super_buff.c_str()));
	  j++;
	  super_buff="";
	}
			  
      else if(head=="IFREE")
	{
	  i++;
	  while(line[i]!='\0')
	    {
	      super_buff+=line[i];
	      i++;
	    }
	  ifree.push_back(atoi(super_buff.c_str()));
	  super_buff="";
	}
      else if(head=="INODE")
	{
	  i++;
	  while(line[i]!=',')
	    {
	      super_buff+=line[i];
	      i++;
	    }
	  all_inode_num.push_back(atoi(super_buff.c_str()));   //Get allocated inodes num
	  //super_buff="";
	  i++;

	  while(line[i]!=',')
	    i++;

	  i++;
	  
	  while(line[i]!=',')
	    {
	      temp_buff+=line[i];
	      i++;
	    }
	  if(atoi(temp_buff.c_str())==0)
	    inode_unalloc.push_back(atoi(super_buff.c_str()));
	  else
	    inode_alloc.push_back(atoi(super_buff.c_str()));
	  
	  inode_mode.push_back(atoi(temp_buff.c_str()));
	  super_buff="";
	  temp_buff="";
	  i++;
	}
	  
	  


    }
}


void audit_inodes()
{
  vector<int>::iterator  it;
  
  for(it=inode_alloc.begin(); it!=inode_alloc.end(); it++)
    {
      if ( find(ifree.begin(), ifree.end(), *it) != ifree.end() )
	cout<<"ALLOCATED INODE "<<*it<<" ON FREELIST\n";
    }
  for(it=inode_unalloc.begin(); it!=inode_unalloc.end(); it++)
    {
      if ( find(ifree.begin(), ifree.end(), *it) == ifree.end() )
	cout<<"UNALLOCATED INODE "<<*it<<" NOT ON FREELIST\n";
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

  audit_inodes();


 
  
 


  /*********************READ SUPERBLOCK****************************/

  return 0;

}
  
