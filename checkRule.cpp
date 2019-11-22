#include<iostream>
#include <typeinfo>
#include <iomanip>
#include <fstream>
#include<stack>
#include<math.h>
#include<string>
using namespace std;
int rows, cols;
long long int power;
char** map;
bool debug = false;
ifstream testData("floor.data");
ifstream result("final.path");
ofstream rule("checkResult.txt");

int absolute(int a){
    if (a<0) return -a;
    else return a;
}
class point {
public:
	int row, col;
	point() :row(0), col(0) {}
	point(int _row, int _col) :row(_row), col(_col) {}
	point operator+(const point& p) {
		point tmp(row + p.row, col + p.col);
		return tmp;
	}
	point operator-(const point& p) {
		point tmp(row - p.row, col - p.col);
		return tmp;
	}

	friend istream& operator>> (istream &s, point p) {
		s >> p.col >> p.row;
		return s;
	}
	friend ostream& operator<< (ostream &s, point p) {
		s << "(" << setw(3) << p.row << ", " << setw(3) << p.col << " )";
		//s <<  p.row << " " <<  p.col ;
		return s;
	}

	bool operator==(const point& p) {
		return ((row == p.row) && (col == p.col));
	}
	bool operator!=(const point& p) {
		return !((row == p.row) && (col == p.col));
	}
};

int main(int argc, char* argv[]){
    int failure = 0;
	//load floor
	point R;

	testData >> rows >> cols >> power;
	if (power >2147483647 || power <0 ){
        if(debug){
            cout << "Power out of Range" << endl;
		}
		rule << "Power out of Range" << endl;
		failure = 1;
	}

	int FullPower = power;
	map = new char*[rows];
	char** checkMap = new char*[rows];
	int countResult = 0 ;

	for (int i = 0; i < rows; i++) {
		map[i] = new char[cols];
		checkMap[i] = new char[cols];
	}

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			testData >> map[i][j];
			checkMap[i][j] = map[i][j];
			if (map[i][j] == 'R') {
				R.row = i;
				R.col = j;
			}
		}
	}
	// load result
	int dataNum;
	if (!(result >> dataNum)){
        rule << "Empty final path file" ;
        return 1;
	}
	//rule << dataNum <<endl;
	point path ;
	point pathPrev ;
	int pathRow, pathCol;


	result >> pathRow >> pathCol;
	path = point(pathRow, pathCol);

	//check Rule a: Always starts from the cell marked “R”
	if(path != R){
		if(debug){
            cout << "Not start from R" << endl;
		}
		rule << "Not start from R" << endl;
		failure = 1;
	}
	else{
		if(debug)cout << "Start from R" << endl;

	}
	//check Rule b. Needs to clean every free cell “0” on the floor
	for(int i=1 ; i <= dataNum; i++){
		pathPrev = path;
		if(result >> pathRow >> pathCol){
            path = point(pathRow, pathCol);
            if( checkMap[path.row][path.col] =='0'){
                checkMap[path.row][path.col] ='C';
            }
            //else if ( checkMap[path[i].row][path[i].col] !='C' || checkMap[path[i].row][path[i].col] !='0'){
            //check Rule c. Cannot move into or cross any obstacle cell “1” or move outside the floor boundary.
            else if ( checkMap[path.row][path.col] =='1' || path.row > rows || path.row<0 || path.col > cols || path.col <0 ){
                if(debug){
                    cout << "Out of boundary: " << path  << endl;
                }
                rule << "Out of boundary: " << path  << endl;
                failure = 1;
            }

            //check Rule d. Can only move up, down, left and right.
            if (absolute(path.row - pathPrev.row)+ absolute(path.col - pathPrev.col)>1){
                if(debug){
                    cout << "Point " << i << " Jump from " << pathPrev << " to " << path << endl;
                }
                rule << "Point " << i << " Jump from " << pathPrev << " to " << path<< endl;
                failure = 1;
            }
            else if(absolute(path.row - pathPrev.row)+ absolute(path.col - pathPrev.col) ==0){
                if(debug){
                    cout << "Point " << i << " Keep Stay at " << path << endl;
                }
                rule  << "Point " << i << " Keep Stay at " << path << endl;

            }
            power--;
            if ( checkMap[path.row][path.col] =='R'){
                power = FullPower;
            }
            else if (power <=0){
                if(debug){
                    cout << "Point " << i << " Out of power" << endl;
                }
                rule  << "Point " << i << " Out of power" << endl;
                failure = 1;
            }
        }
        else{
            if(debug){
                cout << "Point " << i << " No data" << endl;
            }
            rule << "Point " << i << "  No data" << endl;
            failure = 1;
        }
	}
	bool cleanAll = true;
	for(int i= 0 ; i < rows; i++){
        for(int j = 0 ; j < cols; j++){
            if (checkMap[i][j] == '0'){
                cleanAll = false;
            }
        }
	}
	if (!cleanAll){
        if(debug){
            cout << "Haven't clean all" << endl;
        }
        rule << "Haven't clean all" << endl;
        failure = 1;
	}
	else{
        if(debug){
            cout << "Clean all" << endl;
            rule << "Clean all" << endl;
        }

	}

    if(path!= R){
		if(debug){
                cout << "Not end in R:" << R << " end point : " << path  << endl;
		}
		rule << "Not end in R:" << R << " end point : " << path << endl;
		failure = 1;
	}
	else{
		if(debug)cout << "End in R" << endl;
	}


    return failure;
}
