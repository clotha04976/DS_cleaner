
//#include "stdafx.h"
#include<iostream>
#include <typeinfo>
#include <iomanip>
#include <fstream>
#include<stack>
using namespace std;
ifstream test("floor.data");
ofstream result("final.path");
#define INFINITY 999
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
void goAround(point center, int **distance,  point** pred, char** prev) { // at center, look around, and calculate the distance from center ( center + 1)
	int cost = 1;
	if (center.col + 1 < cols){
		if (map[center.row][center.col + 1] != '1') { // right  NOTE 0 is  chat, not int
			if (distance[center.row][center.col + 1] > distance[center.row][center.col] + cost){
				distance[center.row][center.col + 1] = distance[center.row][center.col] + cost; // cost = 1
				pred[center.row][center.col + 1] = center;
				prev[center.row][center.col + 1] = '<';
			}
		}
	}
	if (center.row + 1 < rows) {
		if (map[center.row + 1][center.col] != '1') { // down
			if (distance[center.row + 1][center.col] > distance[center.row][center.col] + cost){
				distance[center.row + 1][center.col] = distance[center.row][center.col] + cost;
				pred[center.row + 1][center.col] = center;
				prev[center.row + 1][center.col] = '^';
			}
		}
	}
	if (center.col - 1 >= 0)
		if (map[center.row][center.col - 1] != '1') { //left
			if(distance[center.row][center.col - 1] > distance[center.row][center.col] + cost){
				distance[center.row][center.col - 1]= distance[center.row][center.col] + cost;
				pred[center.row][center.col - 1] = center;
				prev[center.row][center.col - 1] = '>';
			}
		}
	if (center.row - 1 >= 0) {
		if (map[center.row - 1][center.col] != '1') { //up
			if (distance[center.row - 1][center.col] > distance[center.row][center.col] + cost){
				distance[center.row - 1][center.col] = distance[center.row][center.col] + cost;
				pred[center.row - 1][center.col] = center;
				prev[center.row - 1][center.col] = 'V';
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

int** dijkstra(char** map, point start) {
	int cost = 1;   //cost[max][max] cost all is 1
	int** distance; // Goal: show a distance map
	point** pred;  // Goal: show a "comming from" map
	char** prev;  // Goal: show a "comming from" map
	int** visited; // 0 is unvisited,  visit 1 times and so on, and build a visited map
	visited = new int*[rows];
    distance = new int*[rows];
    pred = new point*[rows];
	prev = new char*[rows];
    for (int i = 0; i < rows; i++){
	   distance[i] = new int[cols];
	   pred[i] = new point[cols];
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
	distance[start.row][start.col] = 0;
	visited[start.row][start.col] = 1;
	prev[start.row][start.col] = 'R';
	count--;
	goAround(start, distance, pred, prev);

	point v = start;
   while(count--) { //should be all visited
	  //Find min
	  point u = choose(distance, visited); 
      
	  //RELEX
	  visited[u.row][u.col]++;
	  goAround(u, distance, pred, prev);
	
	  /*cout << "distance :" << endl;
	  //showMap<int>(distance);
	  if (count < 2 ){
		  cout << "visited :" << endl;
		  showMap<int>(visited,2);
	  }
	  //cout << "prev :" << endl;
	  //showMap<char>(prev);
	  //cout << "pred :" << endl;
	  //showMap<point>(pred);*/
   }

   /*cout << "-----------------------fin-----------------------------------" << endl;
	cout << "distance :" << endl;
	showMap<int>(distance,4);
	cout << "visited :" << endl;
	showMap<int>(visited,0);
	cout << "prev :" << endl;
	showMap<char>(prev,0);
	//cout << "pred :" << endl;
	//showMap<point>(pred);*/

	return distance;
}

point** dijkstraPath(char** map, point start) {
	int cost = 1;//cost[max][max] cost all is 1
	int** distance; // Goal: show a distance map
	point** pred;  // Goal: show a "comming from" map
	char** prev;  // Goal: show a "comming from" map
	int** visited; // 0 is unvisited,  visit 1 times and so on, and build a visited map
	visited = new int*[rows];
	distance = new int*[rows];
	pred = new point*[rows];
	prev = new char*[rows];
	for (int i = 0; i < rows; i++) {
		distance[i] = new int[cols];
		pred[i] = new point[cols];
		visited[i] = new int[cols];
		prev[i] = new char[cols];
	}
	int count = 0, i, j;
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
	distance[start.row][start.col] = 0;
	visited[start.row][start.col] = 1;
	prev[start.row][start.col] = 'R';
	count--;
	goAround(start, distance, pred, prev);

	point v = start;
	while (count--) { //should be all visited
					  //Find min
		point u = choose(distance, visited);

		//RELEX
		visited[u.row][u.col]++;
		goAround(u, distance, pred, prev);

	}
	return pred;
}
bool allClean() {
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			if (map[i][j] == '0')
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
	map = new char*[rows];
	int** distance;
	point** pred;
	stack<point> outputResult;
	int countResult = 0 ;
	for (int i = 0; i < rows; i++) {
		map[i] = new char[cols];
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
	
	distance = dijkstra(map, R);
	pred = dijkstraPath(map, R);
	showMap<char>(map);
	showMap<int>(distance);
	

	point longestPoint;
	int longest = 0;
	int i, j;

	//cout << "main distance" << endl;
	//showMap<int>(distance);

	//cout << "main pred" << endl;
	//showMap<point>(pred);
	bool allCleaned = false;
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
		cout << "ｌｏｎｇｅｓｔ　ｐｏｉｎｔ: " << longestPoint << endl;
		// Go to longest point
		stack<point> s;
		point current(longestPoint.row, longestPoint.col);
		power++;
		do {
			s.push(current);
			current = pred[current.row][current.col];
			//cout << current << " ";
		} while (current != point(-1, -1));
	
		while (!s.empty()) {
			cout << s.top() <<endl;
			outputResult.push(s.top());
			map[s.top().row][s.top().col] = 'C'; //表示已經Clean 過了
			power--;
			countResult++;
			s.pop();
		}
		
		//cout << "Go to longest" << endl;
		//showMap<char>(map);
		//DFS
		current = longestPoint;
		while (current != R) {
			if (current.col + 1 < cols && distance[current.row][current.col + 1] < power && map[current.row][current.col + 1] != 'C') {
				map[current.row][current.col + 1] = 'C';
				current = point(current.row, current.col + 1);
				cout << current << endl;
				outputResult.push(current);
				power--;
				countResult++;
			}
			else if (current.row + 1 < rows && distance[current.row + 1][current.col] < power && map[current.row + 1][current.col] != 'C') {
				map[current.row + 1][current.col] = 'C';
				current = point(current.row + 1, current.col);
				cout << current << endl;
				outputResult.push(current);
				power--;
				countResult++;
			}
			else if (current.col - 1 >=0 && distance[current.row][current.col - 1] < power && map[current.row][current.col - 1] != 'C') {
				map[current.row][current.col - 1] = 'C';
				current = point(current.row, current.col - 1);
				cout << current << endl;
				outputResult.push(current);
				power--;
				countResult++;
			}
			else if (current.row - 1 >= 0 &&  distance[current.row - 1][current.col] < power && map[current.row - 1][current.col] != 'C') {
				map[current.row - 1][current.col] = 'C';
				current = point(current.row - 1, current.col);
				cout << current << endl;
				outputResult.push(current);
				power--;
				countResult++;
			}

			//else if (distance[current.row][current.col]+1 >= power || allCleaned){ // No more power, must go back
				else if (current.col + 1 < cols && distance[current.row][current.col + 1] < power) {
					current = point(current.row, current.col + 1);
					cout << current << endl;
					outputResult.push(current);
					power--;
					countResult++;
				}
				else if (current.row + 1 < rows &&  distance[current.row +1][current.col] < power) {
					current = point(current.row+1, current.col);
					cout << current << endl;
					outputResult.push(current);
					power--;
					countResult++;
				}
				else if (current.col > 0 && distance[current.row][current.col - 1] < power) {
					current = point(current.row, current.col - 1);
					cout << current << endl;
					outputResult.push(current);
					power--;
					countResult++;
				}
				else if (current.row > 0 &&  distance[current.row - 1][current.col] < power) {
					current = point(current.row - 1, current.col);
					cout << current << endl;
					outputResult.push(current);
					power--;
					countResult++;
				}
				else {
					cout << "error" << endl;
				}
			
			/*
			else { // power enough, but have cleaned around
				int ** distanceFromOther;
				point** predFromOther;
				distanceFromOther = dijkstra(map, current);
				predFromOther = dijkstraPath(map, current);
				cout << "distanceFromOther :" << endl;
				showMap<int>(distanceFromOther);
				point nearestPoint;
				int nearest = INFINITY;
				allCleaned = true;
				for (i = 0; i < rows; i++) {
					for (j = 0; j < cols; j++) {
						if (distanceFromOther[i][j] < nearest && map[i][j] == '0') {
							nearest = distanceFromOther[i][j];
							nearestPoint = point(i, j);
							allCleaned = false;
						}
					}
				}

				if (nearest + distance[nearestPoint.row][nearestPoint.col] <= power) {
					// Go to shortest point
					stack<point> s;
					current = nearestPoint;

					do {
						s.push(current);
						current = predFromOther[current.row][current.col];
						cout << current << " ";
					} while (current != point(-1, -1));
					cout << endl;
					while (!s.empty()) {
						cout << s.top() << endl;
						map[s.top().row][s.top().col] = 'C'; //表示已經Clean 過了
						power--;
						s.pop();
					}
				}

				system("pause");
			}*/
			//showMap<char>(map);
			
		}
		power = FullPower;
	}
	//showMap<char>(map);

	// */
	result << countResult << endl;
	int cntOne=0;
	while (!outputResult.empty()) {
		result << outputResult.top() << endl;
		outputResult.pop();
		cntOne++;
	}
	cout << "dif= " << cntOne - countResult << endl;
	system("pause");
    return 0;
}
 
