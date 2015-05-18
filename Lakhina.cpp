#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include "bson/bson.h"
#include <stdio.h>
#include "keccak.h"
#include <sstream>

//Main Commit
cout << "Well";

using namespace std;
namespace fs = boost::filesystem;

string test;

struct Fileinfo {
	string path;
	string hash;
	int size;
	string flag;
};

vector<Fileinfo> compare_lists(vector<Fileinfo> newfl, vector<Fileinfo> oldfl) {
	for (vector<Fileinfo>::iterator itnew = newfl.begin(); itnew < newfl.end(); itnew++) {
		for (vector<Fileinfo>::iterator itold = oldfl.begin(); itold < oldfl.end(); itold++) {
			if ((itnew->path == itold->path) && (itnew->hash == itold->hash)) {
				itnew->flag = "UNCHANGED";
				oldfl.erase(itold);
				break;
			}
			if ((itnew->path == itold->path) && (itnew->hash != itold->hash)) {
				itnew->flag = "CHANGED";
				oldfl.erase(itold);
				break;
			}
		}
	}
	for (vector<Fileinfo>::iterator itold = oldfl.begin(); itold < oldfl.end(); itold++) {
		itold->flag = "DELETED";
		newfl.push_back(*itold);
	}
	return newfl;
}



void SaveBson(string filename, vector<Fileinfo> vec_finfo){
	mongo::BSONArrayBuilder array;
	for (Fileinfo it : vec_finfo){
		bson::bob element;
		element.append("path", it.path);
		element.append("size", it.size);
		element.append("hash", it.hash);

		//mongo::BSONArrayBuilder array1;
		array.append(element.obj());
	}

	bson::bo serializedArray = array.arr();

	//Add array into more general object
	bson::bob container;
	container.appendArray("elements", serializedArray);
	bson::bo p = container.obj();
	std::ofstream fbson("./fileout.bson", std::ios::binary | std::ios::out | std::ios::trunc);
	fbson.clear();
	fbson.write(p.objdata(), p.objsize());
	fbson.close();
}

void get_dir_list(fs::directory_iterator iterator, std::vector<Fileinfo> * vec_finfo) {
	Fileinfo finfo;
	for (; iterator != fs::directory_iterator(); ++iterator)
	{
		if (fs::is_directory(iterator->status())) {
			fs::directory_iterator sub_dir(iterator->path());
			get_dir_list(sub_dir, vec_finfo);

		}
		else
		{
			finfo.path = iterator->path().string();
			finfo.size = fs::file_size(iterator->path());
			// SHA3
			stringstream result;
			string a;
			ifstream myfile;
			myfile.open(finfo.path, ios::binary);
			result << myfile.rdbuf();
			a = result.str();
			Keccak Keccak;
			finfo.hash = Keccak(a);
			finfo.flag = "NEW";
			vec_finfo->push_back(finfo);
		}

	}
}

void ReadBson(vector<Fileinfo> & vec_finfo){
	Fileinfo it;
	string path_copy;
	string size_copy;
	string hash_copy;
	std::ofstream fbson("./fileout.bson", std::ios::binary | std::ios::in);
	stringstream buffer;
	buffer << fbson.rdbuf();
	fbson.close();
	auto data = buffer.str();

	bson::bo b(data.c_str());
	auto name = "elements";
	bson::be elements = b.getFieldDottedOrArray(name);

	for (auto & element : elements.Array()) {
		bson::bo data = element.Obj();
		if (data.hasElement("path") && data.hasElement("size") && data.hasElement("hash")) {
			path_copy = data["path"];
			path_copy.erase(0, 7);
			path_copy.erase(path_copy.length() - 1, 1);
			it.path = path_copy;
			size_copy = data["size"];
			size_copy.erase(0, 5);
			it.size = stoi(size_copy);
			hash_copy = data["hash"];
			hash_copy.erase(0, 7);
			hash_copy.erase(hash_copy.length() - 1, 1);
			it.hash = hash_copy;
			vec_finfo.push_back(it);
		}
	}
}

void print(vector<Fileinfo> vec){
	for (Fileinfo it : vec){
		cout << it.path << " : " << it.size << " : " << it.hash << " : " << it.flag << endl;
	}
}

int main(){
	string path, dirpath;
	string checkstatus;

	cout << "Enter path ->" << endl;
	getline(cin, path);


	if (!(fs::exists(path))) {
		cout << "NOT FOUND" << endl;
		checkstatus = "null";
	}

	else {

		cout << "What do you want?" << endl <<
			"1 - save new data" << endl << "2 - check new data" << endl;
		getline(cin, checkstatus);


		if (checkstatus == "1") {
			vector<Fileinfo> vec_finfo;
			fs::directory_iterator home_dir(path);
			get_dir_list(home_dir, &vec_finfo);
			SaveBson("out.bson", vec_finfo);
			print(vec_finfo);
		}

		else {
			if (checkstatus == "2") {
				vector<Fileinfo> vec_finfo;
				fs::directory_iterator home_dir(path);
				get_dir_list(home_dir, &vec_finfo);
				vector<Fileinfo> vec_finfo_old;
				vector<Fileinfo> vec_finfo_new;
				ReadBson(vec_finfo_old);
				vec_finfo_new = compare_lists(vec_finfo, vec_finfo_old);
				print(vec_finfo_new);
			}
			else {
				cout << "Error";
			}
		}
	}

	cin.get();
	return 0;
}
