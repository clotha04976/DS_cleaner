
#include<iostream>
#include <typeinfo>
#include <iomanip>
#include <fstream>
#include<stack>
#include<queue>
#include<math.h>
using namespace std;
ifstream test("floor.data");
ofstream result("final.path");
#define INFINITY 9999999
int rows, cols, power;
char** map;

class point {
public:
	int row, col;
	point() :row(0), col(0) {}
	point(int _row, int _col) :row(_row), col(_col) {}
	point operator+(const point& p) {
		point tmp(row + p.row, col + p.col);
	}
	point operator-(const point& p) {
		point tmp(row - p.row, col - p.col);
	}

	friend istream& operator>> (istream &s, point p) {
		s >> p.col >> p.row;
		return s;
	}
	friend ostream& operator<< (ostream &s, point p) {
		//s << "(" << setw(3) << p.row << ", " << setw(3) << p.col << " )";
		s <<  p.row << " " <<  p.col ;
		return s;
	}

	bool operator==(const point& p) {
		return ((row == p.row) && (col == p.col));
	}
	bool operator!=(const point& p) {
		return !((row == p.row) && (col == p.col));
	}
};

template <class T>
void showMap(T **G, int padding = 6) {
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			cout << setw(padding) << G[i][j];
		}
		cout << endl;
	}
}
template <class T>
void wrtMap(T **G, int padding = 6) {
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			result << setw(padding) << G[i][j];
		}
		result << endl;
	}
}
void detectArround(point& center, int **distance,  point** pred, char** prev, queue<point>& q) { // at center, look around, and calculate the distance from center ( center + 1)
	int cost = 1;
	if (center.col + 1 < cols){
		if (map[center.row][center.col + 1] != '1') { // right  NOTE 0 is  chat, not int
			if (distance[center.row][center.col + 1] > distance[center.row][center.col] + cost){
				distance[center.row][center.col + 1] = distance[center.row][center.col] + cost; // cost = 1
				pred[center.row][center.col + 1] = center;
				prev[center.row][center.col + 1] = '<';
				q.push(point(center.row, center.col+1));
			}
		}
	}
	if (center.row + 1 < rows) {
		if (map[center.row + 1][center.col] != '1') { // down
			if (distance[center.row + 1][center.col] > distance[center.row][center.col] + cost){
				distance[center.row + 1][center.col] = distance[center.row][center.col] + cost;
				pred[center.row + 1][center.col] = center;
				prev[center.row + 1][center.col] = '^';
				q.push(point(center.row+1, center.col));
			}
		}
	}
	if (center.col - 1 >= 0)
		if (map[center.row][center.col - 1] != '1') { //left
			if(distance[center.row][center.col - 1] > distance[center.row][center.col] + cost){
				distance[center.row][center.col - 1]= distance[center.row][center.col] + cost;
				pred[center.row][center.col - 1] = center;
				prev[center.row][center.col - 1] = '>';
				q.push(point(center.row, center.col-1));
			}
		}
	if (center.row - 1 >= 0) {
		if (map[center.row - 1][center.col] != '1') { //up
			if (distance[center.row - 1][center.col] > distance[center.row][center.col] + cost){
				distance[center.row - 1][center.col] = distance[center.row][center.col] + cost;
				pred[center.row - 1][center.col] = center;
				prev[center.row - 1][center.col] = 'V';
				q.push(point(center.row-1, center.col));
			}
		}
	}
}
point choose(int **distance, int**visited) {
	point nextChoose;
	int  mindistance = INFINITY;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			if (distance[i][j] < mindistance && visited[i][j] == 0) {
				mindistance = distance[i][j];
				nextChoose = point(i, j);
			}
		}
	}
	return nextChoose;
};

void dijkstra(char** map, point start, int** distance,point** pred  ) {
	int cost = 1;   //cost[max][max] cost all is 1
	char** prev;  // Goal: show a "comming from" map
	int** visited; // 0 is unvisited,  visit 1 times and so on, and build a visited map
	visited = new int*[rows];
	prev = new char*[rows];
    for (int i = 0; i < rows; i++){
	   visited[i] = new int[cols];
	   prev[i] = new char[cols];
    }

	int count=0, i, j;
	for (i = 0; i < rows; i++) {
		for (j = 0; j < cols; j++) {
			distance[i][j] = INFINITY;
			if (map[i][j] == '1') {
				visited[i][j] = 9;
				prev[i][j] = 'O';
			}
			else {
				visited[i][j] = 0;
				prev[i][j] = '_';
				count++;
			}
			pred[i][j] = point(-1, -1);
		}
	}

	////Round Initial ////
	queue<point> q;
	distance[start.row][start.col] = 0;
	visited[start.row][start.col] = 1;
	prev[start.row][start.col] = 'R';
	count--;
	detectArround(start, distance, pred, prev, q);

	point v = start;
   while(count--) { //should be all visited
	  //Find min (BFS)
      point u = q.front();
      q.pop();
	  //RELEX
	  visited[u.row][u.col]++;
	  detectArround(u, distance, pred, prev, q);
   }
}

point goArround(point current, int** distance, int& power, point** pred){
    if (current.col + 1 < cols && distance[current.row][current.col + 1] < power && map[current.row][current.col + 1] != 'C') {
        map[current.row][current.col + 1] = 'C';
        current = point(current.row, current.col + 1);
    //	cout << current << endl;
    }
    else if (current.row + 1 < rows && distance[current.row + 1][current.col] < power && map[current.row + 1][current.col] != 'C') {
        map[current.row + 1][current.col] = 'C';
        current = point(current.row + 1, current.col);
    //	cout << current << endl;
    }
    else if (current.col - 1 >=0 && distance[current.row][current.col - 1] < power && map[current.row][current.col - 1] != 'C') {
        map[current.row][current.col - 1] = 'C';
        current = point(current.row, current.col - 1);
    //	cout << current << endl;

    }
    else if (current.row - 1 >= 0 &&  distance[current.row - 1][current.col] < power && map[current.row - 1][current.col] != 'C') {
        map[current.row - 1][current.col] = 'C';
        current = point(current.row - 1, current.col);
    //	cout << current << endl;
    }
    else{
        current = pred[current.row][current.col];
    }
/*
    else if (current.col + 1 < cols && distance[current.row][current.col + 1] < power) {
        current = point(current.row, current.col + 1);
//		cout << current << endl;
    }
    else if (current.row + 1 < rows &&  distance[current.row +1][current.col] < power) {
        current = point(current.row+1, current.col);
//		cout << current << endl;
    }
    else if (current.col > 0 && distance[current.row][current.col - 1] < power) {
        current = point(current.row, current.col - 1);
//		cout << current << endl;
    }
    else if (current.row > 0 &&  distance[current.row - 1][current.col] < power) {
        current = point(current.row - 1, current.col);
//		cout << current << endl;
    }*/
    return current;

}



bool allClean(){
    for (int i = 0 ; i < rows; i ++){
        for(int j = 0 ; j < cols; j ++){
            if(map[i][j] == '0')
                return false;
        }
    }
    return true;
}
int main()
{
	//input
	point R;
	test >> rows >> cols >> power;
	int FullPower = power;
	int** distance;
	point** pred;
	map = new char*[rows];
	distance = new int*[rows];
	pred = new point*[rows];
	queue<point> outputResult;

	int countResult = 0 ;
	for (int i = 0; i < rows; i++) {
		map[i] = new char[cols];
		distance[i] = new int[cols];
		pred[i] = new point[cols];
	}

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			test >> map[i][j];
			if (map[i][j] == 'R') {
				R.row = i;
				R.col = j;
			}
		}
	}

	dijkstra(map, R, distance, pred);
	//showMap<char>(map);
	//cout << "main distance" << endl;
	//showMap<int>(distance);
	//cout << "main pred" << endl;
	//showMap<point>(pred);

	point longestPoint;
	int longest = 0;
	int i, j;
	bool allCleaned = false;
	outputResult.push(R);
	while(!allClean()){
		int longest = 0;
		//Pickup longest Point,
		for (i = 0; i < rows; i++) {
			for (j = 0; j < cols; j++) {
				if (distance[i][j]!=INFINITY && distance[i][j] > longest && map[i][j] !='C') {
					longest = distance[i][j];
					longestPoint = point(i, j);
				}
			}
		}
		//cout << "main distance" << endl;
		//showMap<int>(distance);
        //cout << "longest Point: " << longestPoint << " long : " << longest << endl;
		// Go to longest point
		stack<point> s;
		point current(longestPoint.row, longestPoint.col);

		int powerPredict = distance[longestPoint.row][longestPoint.col];
		if (power/2 > distance[longestPoint.row][longestPoint.col]){
            powerPredict = power - distance[longestPoint.row][longestPoint.col];
		}
		//cout <<"powerPredict " << powerPredict <<endl;
		do {
			s.push(current);
			if (map[pred[current.row][current.col].row][pred[current.row][current.col].col] != 'C'){
                current = pred[current.row][current.col];
                powerPredict--;
			}
			else{
                current = goArround(current, distance, powerPredict, pred );
                powerPredict--;
			}
			//cout << current << " ";
		} while (current != R);
        //power++; // Actually, power is decreased by distance, but I use the point to count power which is wrong. However, x point = x-1 distance
		while (!s.empty()) {
		//	cout << s.top() <<endl;
			outputResult.push(s.top());
			map[s.top().row][s.top().col] = 'C'; //表示已經Clean 過了

			power--;
			countResult++;
			s.pop();
		}

		//cout << "Longest back" << endl;
		//showMap<char>(map);
		//DFS
		current = longestPoint;
		while (current != R) {

			current = goArround(current, distance, power, pred );
            outputResult.push(current);
            power--;
        //    cout << " power :" << power << endl;
            countResult++;
		}
		power = FullPower;
	}
	//showMap<char>(map);

	// */
	result << countResult << endl;
	int cntOne=0;
	//cout <<"go back " <<endl;
	while (!outputResult.empty()) {
		result << outputResult.front() << endl;
		outputResult.pop();
		cntOne++;
	}
	//cout << "dif= " << cntOne - countResult -1 << endl;
    return 0;
}

