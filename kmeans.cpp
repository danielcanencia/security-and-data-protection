#include <iostream>
#include <vector>
#include <string>
#include <memory>

#include <algorithm>
#include <iostream>


using namespace std;


class any_type
{
public:
   virtual ~any_type() {}
   virtual void print() = 0;
};

template <class T>
class concrete_type : public any_type
{
public:
   concrete_type(const T& value) : value_(value)
   {}

   virtual void print()
   {
      std::cout << value_ << '\n';
   }
private:
   T value_;
};


class Record {
private:
	int rid, gid;
	vector<unique_ptr<any_type>> values;

public:
	Record(int _rid, string record) {
		rid = _rid;
	}

	template <class T>
	void push_back_value(concrete_type<T>* value) {
		//value->print();	
		//values[0].reset(value);
		//values.push_back((unique_ptr<T>)value);
		values.emplace_back(value);
		//values.emplace(values.begin(), value);
		//values.at(0)->print();

		/*std::vector<unique_ptr<any_type>>::iterator itr = std::find(values.begin(), values.end(), value); 

		if (itr != values.cend()) {
			std::cout << "Element present at index " << std::distance(values.begin(), itr);
		}*/
	}

	void print_values(int i) {
		values.at(i)->print();
	}
};


class Group {
private:
	int gid;
	vector<any_type> centroid;
	vector<any_type> records;
public:
	Group(int _gid, Record _centroid) {
		gid = _gid;
	}

};


int main() {
	//vector<unique_ptr<any_type>> v(2);
	Record x(2, "line");

	concrete_type<int>* con = new concrete_type<int>(70);
	con->print();

   	x.push_back_value(new concrete_type<int>(99));
   	x.push_back_value(new concrete_type<string>("Bottles of Beer"));

	for(size_t i = 0; i < 2; ++i)
  	{
     		x.print_values(i);	
    	}

	// Read CSV Input file

	// Parse CSV Input file
	
	return 0;
}
