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
  vector<int> blocks;
};

struct dir{
  int parent;
  int ref;
  string name;
};

struct indir{
  int block;
};

int num_blocks=0, num_inodes=0,block_size=0,inode_size=0,non_rnode=0,free_i=0,free_b=0,b_map=0,i_map=0,inodes_block=0;

vector<int> bfree,ifree,dir_inodes;

vector<inode> inodes;
vector<dir> dirs;
vector<indir> indirs;


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
	  string i1_buff="",i2_buff="",i3_buff="",i4_buff="";
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
	    i++;
	  i++;

	  while(line[i]!=',')
	    i++;
	  i++;

	  while(line[i]!='\0')
	    {
	      while(line[i]!=',')
		{
		  if(line[i]=='\0')
		    break;
		  i4_buff+=line[i];
		  i++;
		}
	      in.blocks.push_back(atoi(i4_buff.c_str()));
	      i4_buff="";
	      i++;
	    }
	  inodes.push_back(in);
	  
	}
      else if(head=="DIRENT")
	{
	  string d1_buff="",d2_buff="",d3_buff="";
	  dir dr;

	  i++;
	  while(line[i]!=',')
	    {
	      d1_buff+=line[i];
	      i++;
	    }
	  dr.parent=atoi(d1_buff.c_str());
	  i++;

	  while(line[i]!=',')
	    i++;
	  
	  i++;
	  while(line[i]!=',')
	    {
	      d2_buff+=line[i];
	      i++;
	    }
	  dr.ref=atoi(d2_buff.c_str());
	  i++;

	  while(line[i]!=',')
	    i++;
	  
	  i++;

	  while(line[i]!=',')
	    i++;
	  
	  i++;
	  while(line[i]!='\0')
	    {
	      d3_buff+=line[i];
	      i++;
	    }
	  dr.name=d3_buff;
	 
	  i++;
	  dirs.push_back(dr);

	}
      else if(head=="INDIRECT")
	{
	  indir ind;
	  string in_buff="";
	    
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
	    i++;
	  i++;

	  while(line[i]!='\0')
	    {
	      in_buff+=line[i];
	      i++;
	    }
	  ind.block=atoi(in_buff.c_str());
	  in_buff="";
	  
	  indirs.push_back(ind);
	}
	  
	  
	  


    }
}


void audit_blocks()
{
  int firstb,t=0;
  vector<inode>::iterator  it;
  firstb=non_rnode+(num_inodes*inode_size)/block_size;

  for(int i=firstb;i<num_blocks;i++)
    {
      t=0;
      for(it=inodes.begin(); it!=inodes.end(); ++it)
	 {
	   if (find((*it).blocks.begin(), (*it).blocks.end(), i) == (*it).blocks.end())
	     {
	       continue;
	     }
	   else
	     {
	       t=1;
	       break;
	     }
	 }
       if(t==0)
	 {
	   if (find(bfree.begin(), bfree.end(), i) == bfree.end())
	     {
	       cout<<"UNREFERENCED BLOCK "<<i<<endl;
	       continue;
	     }
	 }
	   
    }
}
	  

void audit_inodes()
{
  vector<inode>::iterator  it;
  int t1;
  
  for(it=inodes.begin(); it!=inodes.end(); ++it)  //Iterator to go through all inode entries in csv
    {
      t1=(*it).num;  //saving inode number of allocated inode
 
      if ( find(ifree.begin(), ifree.end(), t1) != ifree.end() )  //checking if the allocated inode is on the free list
	cout<<"ALLOCATED INODE "<<t1<<" ON FREELIST\n";
    }

  int t=0;
  for(int i=non_rnode;i<num_inodes;i++)   //iterates from first non reserved inode to the end of all inodes
    {
      for(it=inodes.begin(); it!=inodes.end(); ++it)  
	{
	  int t2=(*it).num;    //gets allocated inode entire's inode no.
	  if(t2==i)           //checks if an inode has been allocated or not
	    {
	      t=1;
	      break;
	    }
	}

      if(t==0)  //if inode hasn't been allocated
	{
	  if ( find(ifree.begin(), ifree.end(), i) == ifree.end() )   //check is this inode is in free list, if its not - print
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
  vector<inode>::iterator it,it3;
  //vector<int>::iterator it1;
  vector<dir>::iterator it1,it2;
  
  for(it=inodes.begin(); it!=inodes.end(); ++it)  //iterate through inode entries data
    {
      for(it1=dirs.begin(); it1!=dirs.end(); ++it1)  //iterate through directory entries data
	{
	  if((*it).num==(*it1).ref)  //check if the inode number is being referenced by a DIRENT entry
	    {
	      ctr++;               //count how many times ^
	    }
	}
      if(ctr!=(*it).links)   //if this number doesn't match ilinks number from INODE entry - print
	{
	  cout<<"INODE "<<(*it).num<<" HAS "<<ctr<<" LINKS BUT LINKCOUNT IS "<<(*it).links<<endl;
	}
      ctr=0;

      
    }


  int t1=0,t2=0;
  for(it2=dirs.begin(); it2!=dirs.end(); ++it2)       //iterate through DIRENT entries
    {
      t2=0;
      for(int i=1;i<=num_inodes;i++)        //all valid inodes possible
	{
	  if((*it2).ref==i)                  //check if referenced inode is a valid inode 
	    {
	      t2=1;
	      break;
	    }
	}
      
      if(t2==0)              //reference inode is not a valid inode no.
	{
	  cout<<"DIRECTORY INODE "<< (*it2).parent << " NAME "<<(*it2).name <<" INVALID INODE "<<(*it2).ref<<endl;
	  continue;
	}
      
      t1=0;
      for(it3=inodes.begin();it3!=inodes.end();++it3)  //iterate through all INODE entries
	{
	  if ((*it2).ref==(*it3).num)     //check if inode referenced is already allocated
	    {
	      t1=1;
	      break;
	    }
	}

      if(t1==0)      //if ref inode is not allocated, print!
	{
	  cout<<"DIRECTORY INODE "<< (*it2).parent << " NAME "<<(*it2).name <<" UNALLOCATED INODE "<<(*it2).ref<<endl;
	  continue;
	}

      if((*it2).name=="'.'")
	{
	  if((*it2).parent!=(*it2).ref)
	    {
	      cout<<"DIRECTORY INODE "<<(*it2).parent<<" NAME '.' LINK TO INODE "<<(*it2).ref<<" SHOULD BE "<<(*it2).parent<<endl;
	      continue;
	    }
	}
      else if((*it2).name=="'..'")
	{
	  if((*it2).parent==2)
	    {
	      if((*it2).parent!=(*it2).ref)
		{
		  cout<<"DIRECTORY INODE "<<(*it2).parent<<" NAME '..' LINK TO INODE "<<(*it2).ref<<" SHOULD BE "<<(*it2).parent<<endl;
		  continue;
		}
	    }
	}
	  //else part
	    

           
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

  audit_blocks();

  audit_inodes();

  audit_dirs();
 
  
 


  /*********************READ SUPERBLOCK****************************/

  return 0;

}
  
