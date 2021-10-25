#include<iostream>
#include<string>
#include<math.h>
#include<vector>
using namespace std;
int main()
{
	cout<<"Enter the original message\n";
	string s,t,a;
	cin>>s;
	int r,m=s.length();
	for(r=0;pow(2,r)<m+r+1;r++);
	cout<<"r="<<r<<endl;
	for(int i=0;i<r+m;i++)
		a+=' ';
	for(int i=0;i<r;i++)
		a[pow(2,i)-1]='x';
	for(int i=0,j=0;i<r+m;i++)
	{
		if(a[i]!='x')
		{
			a[i]=s[j++];
		}
	}
	vector<vector<int>> v;
	for(int i=0;i<r;i++)
	{
		vector<int> v1;
		for(int j=0;j<=r+m;j++)
		{
			if(j&(1<<(i)))
			{
				v1.push_back(j);
			}
		}
		v.push_back(v1);
	}
	for(int i=0;i<r;i++)
	{
		int c=0;
		for(int j=1;j<v[i].size();j++)
		{
			if(a[v[i][j]-1]=='1')
				c++;
		}
		if(c&1)
			a[pow(2,i)-1]='1';
		else
			a[pow(2,i)-1]='0';
	}
	cout<<a<<endl;
	cout<<"Enter received message"<<endl;
	cin>>t;
	int p=0;
	for(int i=0;i<r;i++)
	{
		int c=0;
		for(int j=0;j<v[i].size();j++)
		{
			if(t[v[i][j]-1]=='1')
				c++;
		}
		if(c&1)
			p+=pow(2,i);
	}
	cout<<"p="<<p<<endl;
	if(p!=0)
	{
		if(t[p-1]=='0')
			t[p-1]='1';
		else
			t[p-1]='0';
	}
	cout<<t<<endl;
	int j=0;
	string t1;
	for(int i=0;i<r+m;i++)
	{
		if(i==v[j][0]-1)
		{
			j++;
			if(j==r)
				j=0;
			continue;
		}
		t1+=t[i];
	}
	cout<<t1<<endl;
	return 0;
}