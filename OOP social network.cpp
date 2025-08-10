#include<iostream>
#include<fstream>
using namespace std;
int strLen(char* str)
{
	int len = 0;
	for (int i = 0; str[i] != '\0'; i++, len++);
	return len;
}
char* buffertostr(char* buffer)

{
	int size = strLen(buffer);
	char* str = new char[size + 1];
	for (int i = 0; i < size; str[i] = buffer[i], i++);
	str[size] = '\0';
	return str;
}
class person
{
protected:
	char* firstname;
	char* lastname;
	char* id;
public:
	
	person()
	{
		firstname = nullptr;
		id = nullptr;
		lastname = nullptr;
	}
	void setid(char* id1)
	{
		char* temp = new char[10];
		
		for (int i = 0; id1[i] != '\0'; i++)
		{
			int count = 0;
			temp[i] = id1[i];
			count++;
		}

		id = buffertostr(id1);
		delete[]temp;
		temp = nullptr;


	}
	void setfirstname(char* name1)
	{
		
	char* temp=new char[10];
	int count = 0;
		for (int i = 0; name1[i] != '\0'; i++)
		{
			temp[i] = name1[i];
			count++;
		}
		firstname = buffertostr(name1);
		
			for (int i = 0; name1[i] != '\0'; i++)
			{
				firstname[i] = name1[i];
				
			}
		
		delete[]temp;
		temp = nullptr;
		
	}
	void setlastname(char* lastname)
	{
		for (int i = 0; lastname[i] != '\0'; i++)
		{
			this->lastname[i] = lastname[i];
		}
	}
	void returnname()
	{
		
		cout << firstname<<endl;
	}
	void returnid()
	{
		cout << id<<endl;
	}
	

};
class user :public person
{
private:
	char** skill;
	int experience;


public:

};
class page :public person
{
public:
	void readpagedata(char*title,char*pageid)
	{
		this->id = pageid;
	
		this->firstname = title;
	}
	void printpage()
	{
	cout << "page id :" << id<<endl;
		cout << "Page title :" << firstname<<endl;
	
	
		
	}

};
class company :public person
{
private:
	char* location;
public:
	void readcompanydata(char* id,char*companylocation)
	{
		this->id = id;
		location = companylocation;
	}
	void printcompany()
	{
		
		cout << "Company id :" << this->id<<endl;
		cout << "Company's Location " << location<<endl;
	}
	char* returnid()
	{
		return id;
	}

};
class education
{
private:
	char* degree;
	char* ins;
public:

};
class comment
{
private:

	char* id;
	char* caption;
	char* userid;
	char* cname;
public:
	comment()
	{
		id = new char;
		caption = new char;
		
		
	}
	person* p=new person;
	void setdata(char* id, char* caption, char* firstname1, char* id1)
	{
	
		this->id = id;
		this->caption = caption;
		
		
		p->setfirstname(firstname1);
		p->setid(id1);
		
	}
	void print()
	{
	
			cout << "The comment id is :" << id << endl;
		
		cout << "The name of the post is : "; p->returnname();
		cout << "THe id of the user who posted the comment is :";p->returnid();
		cout << "The comment is :" << caption << endl;
		
	}
};
class date
{
private:

	const char* currentdate = "14/5/2022";
	char* postdate;
public:
};
class post
{
private:

	int id;
	char* description;
	comment* c;
	date* d;
};
class activity :public post
{
private:
	int id;
	char* description;
public:

};
class socialnetwork
{
private:
	comment* c;
	int totalcomments;
	page* p;
	int totalpages;
	company* co;
	int totalcomapies;

public:

	char* name = new char;
	char* cid = new char;
	char* pid = new char;
	char* comments = new char;

	void readcommentfile(ifstream& filename)
	{
		filename >> totalcomments;
		c = new comment[totalcomments];
		for (int i = 0; i < totalcomments; i++)
		{
			char* temp = new char[100];

			filename >> cid;
			filename >> name;
			filename >> pid;
			filename.getline(temp, 100, '+');
			comments = buffertostr(temp);
			delete[]temp;
			temp = nullptr;
			c[i].setdata(cid, comments, name, pid);

		}

	}
	void printcomment()
	{
		for (int i = 0; i < totalcomments; i++)
		{
			c[i].print();
		}
	}char* pageid = new char;
	char* title = new char;
	void readpages(ifstream& filename)
	{

		filename >> totalpages;
		p = new page[totalpages];
		for (int i = 0; i < totalpages; i++)
		{
			char* temp = new char[100];
			filename >> pageid;
			filename.getline(temp, 100, '+');
			title = buffertostr(temp);
			delete[]temp;
			temp = nullptr;
			p[i].readpagedata(title, pageid);
		}

	}
	void printpages()
	{
		for (int i = 0; i < totalpages; i++)
		{
			p[i].printpage();
			
		}
	}
	char* companyid=new char;
	char* companylocation=new char;
	void readcompanydata(ifstream&filename)
	{
		filename >> totalcomapies;
		co = new company[totalcomapies];
		for (int i = 0; i < totalcomapies; i++)
		{
			filename >> companyid;
			char* temp = new char[50];
			filename.getline(temp, 50, '+');
			companylocation = buffertostr(temp);
			delete[]temp;
			temp = nullptr;
			co[i].readcompanydata(companyid, companylocation);
		}
	}
	void printcompanydata()
	{
		for (int i = 0; i < totalcomapies; i++)
		{
			co[i].printcompany();
		

		}
	

	}
};

int main()
{socialnetwork s;
	/*ifstream file("comments.txt");
	s.readcommentfile(file);
	s.printcomment();
	ifstream file1("pages.txt");
	s.readpages(file1);
	s.printpages();*/
ifstream file2("company.txt");
s.readcompanydata(file2);
s.printcompanydata();
}
