#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

struct inode{
  int num;
  int mode;
  int links;
};

int num_blocks=0, num_inodes=0,block_size=0,inode_size=0,non_rnode=0,free_i=0,free_b=0,b_map=0,i_map=0,inodes_block=0;

vector<int> bfree,ifree,dir_inodes;

vector<inode> inodes;




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
	  inode in;
	  string i1_buff="",i2_buff="",i3_buff="";
	  i++;
	  while(line[i]!=',')
	    {
	      i1_buff+=line[i];
	      i++;
	    }
	  in.num=atoi(i1_buff.c_str());
	  //super_buff="";
	  i++;

	  while(line[i]!=',')
	    i++;

	  i++;
	  
	  while(line[i]!=',')
	    {
	      i2_buff+=line[i];
	      i++;
	    }
	  in.mode=atoi(i2_buff.c_str());
	  i++;

	  while(line[i]!=',')
	    i++;
	  i++;

	  while(line[i]!=',')
	    i++;
	  i++;

	  while(line[i]!=',')
	    {
	      i3_buff+=line[i];
	      i++;
	    }
	  in.links=atoi(i3_buff.c_str());

	  inodes.push_back(in);
	  
	}
      else if(head=="DIRENT")
	{

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
	  dir_inodes.push_back(atoi(super_buff.c_str()));
	  super_buff="";
	  i++;

	}
	  
	  
	  


    }
}


void audit_inodes()
{
  vector<inode>::iterator  it;
  int t1;
  
  for(it=inodes.begin(); it!=inodes.end(); ++it)
    {
      t1=(*it).num;
 
      if ( find(ifree.begin(), ifree.end(), t1) != ifree.end() )
	cout<<"ALLOCATED INODE "<<t1<<" ON FREELIST\n";
    }

  int t=0;
  for(int i=non_rnode;i<=num_inodes;i++)
    {
      for(it=inodes.begin(); it!=inodes.end(); ++it)
	{
	  int t2=(*it).num;
	  if(t2==i)
	    {
	      t=1;
	      break;
	    }
	}

      if(t==0)
	{
	  if ( find(ifree.begin(), ifree.end(), i) == ifree.end() )
	    {
	      cout<<"UNALLOCATED INODE "<<i<<" NOT ON FREELIST\n";
	      continue;
	    }
	}
      t=0;
    }
	 

      
}

void audit_dirs()
{
  int ctr=0;
  vector<inode>::iterator it;
  vector<int>::iterator it1;
  
  for(it=inodes.begin(); it!=inodes.end(); ++it)
    {
      for(it1=dir_inodes.begin(); it1!=dir_inodes.end(); ++it1)
	{
	  if((*it).num==*it1)
	    {
	      ctr++;
	    }
	}
      if(ctr!=(*it).links)
	{
	  cout<<"INODE "<<(*it).num<<" HAS "<<ctr<<" LINKS BUT LINKCOUNT IS "<<(*it).links<<endl;
	}
      ctr=0;

      
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

  audit_dirs();
 
  
 


  /*********************READ SUPERBLOCK****************************/

  return 0;

}
  
