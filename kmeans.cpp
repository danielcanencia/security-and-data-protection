#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <filesystem>
#include <cstring>

using namespace std;
namespace fs = std::filesystem;

class any_type
{
public:
   virtual ~any_type() {}
   virtual void print() = 0;
};

template <class T>
class concrete_type : public any_type
{
private:
   T value_;
public:
   concrete_type(const T& value)
   {
	value_ = value;
   }

   virtual void print()
   {
      std::cout << value_ << '\n';
   }
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
		values.emplace_back(value);
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


int main(int argc, char** argv) {

	/*Record x(2, "line");
   	x.push_back_value(new concrete_type<int>(99));
   	x.push_back_value(new concrete_type<string>("Bottles of Beer"));

	for(size_t i = 0; i < 2; ++i)
  	{
     		x.print_values(i);	
    	}

	// Obtain current working dir
	string path = "Current working path: " + fs::current_path().u8string() + "/inputs" + '\n';
	cout << path;
	*/

	// Read CSV Input files
	if (argc != 2) {
		cout << "No file given. Use ./kmeans [filename]" << endl;
		return -1;
	}
	string file = argv[0];
	char *ptr = strtok(argv[1], ".");
	cout << ptr << endl;
	if (ptr != "csv") {
		return -1;
	}
	// Parse CSV Input file
	

	return 0;
}
