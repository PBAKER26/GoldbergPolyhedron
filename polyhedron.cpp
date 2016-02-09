#include "goldberg.h"
#include "polyhedron.h"
#include <algorithm>

bool Point::operator<(const Point &other) const {
	if (distFromOrigin > other.distFromOrigin) return true;
	else if (distFromOrigin < other.distFromOrigin) return false;
	else if (y < other.y) return true;
	else if (y > other.y) return false;
	else if (z < other.z) return true;
	else if (z > other.z) return false;
	else if (x < other.x) return true;
	else return false;
}

bool Point::operator>(const Point &other) const {
	if (distFromOrigin < other.distFromOrigin) return true;
	else if (distFromOrigin > other.distFromOrigin) return false;
	else if (y > other.y) return true;
	else if (y < other.y) return false;
	else if (z > other.z) return true;
	else if (z < other.z) return false;
	else if (x > other.x) return true;
	else return false;
}

void Point::setDistFromOrigin() {
	distFromOrigin = hypot(hypot(x - origin->x, y - origin->y), z - origin->z);
}

bool SORT_BY_PROXIMITY(Point * a, Point * b) {
	return a->distFromOrigin < b->distFromOrigin;
}

void Polyhedron::initPolyhedron(int _m, int _n) {
	origin = new Point(0, 3, 0);
	Vector3f farpoint[20][3];										//corners of a triangle that encompass all points of ico triangle
	std::vector<Triangle> subTriangles;
	std::vector<Point> basepoint;								//points on an abstract triangle that coincide with actual points on the real triangle. x = A, y = B, z = C
	std::vector<Point *> pPoints;

	if (_m < 0 || _n < 0) return;
	if (_n > _m) { m = _n; n = _m; }
	else { m = _m; n = _n; }

	//debug
	unsigned int currentTime = SDL_GetTicks(), newTime;
	std::cout << m << ", " << n << '\n';

	float L = m + n * 2 - 3;

	if (n == 0) {
		L = m;
		for (int i = 0; i < 20; i++) {
			farpoint[i][0] = icoTriangles[i].a;
			farpoint[i][1] = icoTriangles[i].b;
			farpoint[i][2] = icoTriangles[i].c;
		}
		//populate basepoint array
		for (int A = 0; A < m + 1; A++) {
			for (int B = L - A; B < m + 1; B++) {
				basepoint.push_back(Vector3f(A, B, m * 2 - A - B));
			}
		}
	}

	if (n > 0) {
		float length = sqrt(pow(m + n / 2, 2) + pow(tri_height*n, 2));				//length of the side of the triangle

		//Begin by finding 3 points that encompass all possible points within the triangle
		
		float angle = atan((tri_height * m) / (n + m / 2));
		float x = n / 2;
		float y = tan(angle) * x;
		float h = y / sin(angle);													//hypotneuse

		float s_length = length - h;
		float l = tri_height * (n - 2);
		float w = (m * 2 + n * 4 - 6) * tri_height / 3;
		float q = w - y - l;

		for (int i = 0; i < 20; i++) {
			Vector3f Z = icoTriangles[i].center;
			Vector3f D = (icoTriangles[i].a * (s_length)+icoTriangles[i].b * (h)) / length;
			Vector3f E = (icoTriangles[i].b * (s_length)+icoTriangles[i].c * (h)) / length;
			Vector3f F = (icoTriangles[i].c * (s_length)+icoTriangles[i].a * (h)) / length;

			Vector3f D_diff = Z - D;
			Vector3f E_diff = Z - E;
			Vector3f F_diff = Z - F;

			farpoint[i][0] = D - D_diff * ((w - q) / q);
			farpoint[i][1] = E - E_diff * ((w - q) / q);
			farpoint[i][2] = F - F_diff * ((w - q) / q);
		}

		//populate basepoint array
		Vector2f bA = { 0, L };
		Vector2f bB = { L, 0 };
		Vector2f bC = { L, L };
		Vector2f rA = { n - 2, m + n * 2 - 2 };
		Vector2f rB = { m + n - 2, n - 2 };
		Vector2f rC = { m + n * 2 - 2, m + n - 2 };
		//slopes of line of triangle
		float mAB = (rB.y - rA.y) / (rB.x - rA.x);
		float mBC = (rC.y - rB.y) / (rC.x - rB.x);
		float mAC = (rC.y - rA.y) / (rC.x - rA.x);
		//now find every point within the triangle, excluding ones outside the triangle
		for (int A = 0; A < L + 1; A++) {
			for (int B = L - A; B < L + 1; B++) {
				int C = L * 2 - A - B;
				if (rA.y - mAB * rA.x <= B - mAB * A) { if (rB.y - mBC * rB.x <= B - mBC * A) { if (rC.y - mAC * rC.x >= B - mAC * A) {
							basepoint.push_back(Vector3f(A, B, C));
							//std::cout << A << ", " << B << ", " << C << "\n";
				} } }
			}
		}
	}

	points.reserve(basepoint.size() * 20 + 13);		//this makes sure the vectors memory is never reallocated, which would invalidate any pointers to its contents

	//fill in known neighbours now, its faster this way
	std::vector<Point>::iterator nearPointsTemp[6];
	std::vector<std::vector<int>> nearPoints(basepoint.size());
	int index = 0;
	for (std::vector<Point>::iterator i = basepoint.begin(); i != basepoint.end(); i++) {
		nearPointsTemp[0] = std::find(basepoint.begin(), i, *i + Vector3f(-1, 0, 1));
		nearPointsTemp[1] = std::find(basepoint.begin(), i, *i + Vector3f(-1, 1, 0));
		if (index > 0 && *std::prev(i,1) == *i + Vector3f(0, -1, 1)) {
			nearPointsTemp[2] = std::prev(i,1);
		}
		else {
			nearPointsTemp[2] = basepoint.end();
		}
		if (index < basepoint.size() - 1 && *std::next(i,1) == *i + Vector3f(0, 1, -1)) {
			nearPointsTemp[3] = std::next(i,1);
		}
		else {
			nearPointsTemp[3] = basepoint.end();
		}
		nearPointsTemp[4] = std::find(i, basepoint.end(), *i + Vector3f(1, -1, 0));
		nearPointsTemp[5] = std::find(i, basepoint.end(), *i + Vector3f(1, 0, -1));
		for (int j = 0; j < 6; j++) {
			if (nearPointsTemp[j] != basepoint.end() && nearPointsTemp[j] != i) {
				basepoint[index].neighbour.push_back(&*nearPointsTemp[j]);
				nearPoints[index].push_back(nearPointsTemp[j] - basepoint.begin());
			}
		}
		index++;
	}

	//take the basepoints and use them as a template to populate all 20 sides of the icosahedron.
	if (m == 1 && n == 1) L = 1;				//otherwise L would be 0 and would cause a divide by zero error
	Point temp_Point;
	std::vector<Point>::iterator cPoint;
	for (int i = 0; i < 20; i++) {
		std::vector<Point *> facePoints;
		//place the actual points
		for (int j = 0; j < basepoint.size(); j++) {
			temp_Point = (farpoint[i][0] * (L - basepoint[j].x) + farpoint[i][1] * (L - basepoint[j].y) + farpoint[i][2] * (L - basepoint[j].z)) / L;
			cPoint = std::find(points.begin(), points.end(), temp_Point);
			if (cPoint == points.end()) {
				points.push_back(temp_Point);
				facePoints.push_back(&points.back());
			} else {
				facePoints.push_back(&*cPoint);
			}
		}
		//now link the known neighbours together
		for (int j = 0; j < basepoint.size(); j++) {
			for (int k = 0; k < basepoint[j].neighbour.size(); k++) {
				facePoints[j]->neighbour.push_back(facePoints[nearPoints[j][k]]);
			}
		}
	}

	//Make sure that the pentagons only get 5 neighbours. The first 12 points should be the pentagons as they were added in first
	unsigned vSize = points.size();
	for (unsigned i = 0; i < 12; i++) {
		points[i].neighbours = 5;
	}
	
	//Normalize the points to form a sphere
	for (unsigned i = 0; i < vSize; i++) {
		points[i].Normalize();
	}
	
	//now figure out all the missing neighbours for each point
	std::vector<Point *> vecNeighbour;
	std::vector<Point *>::iterator it;
	//First add each point that doesnt have all its neighbours to a vector of pointers
	for (unsigned i = 0; i < vSize; i++) {
			if (points[i].neighbour.size() != points[i].neighbours) vecNeighbour.push_back(&points[i]);
	}
	do {
		origin = vecNeighbour.back();
		vecNeighbour.pop_back();
		//sort by distance to the point being neighboured, origin, and then fill in the missing neighbours
		vSize = vecNeighbour.size();
		for (unsigned i = 0; i < vSize; i++) {
			vecNeighbour[i]->setDistFromOrigin();
		}
		std::sort(vecNeighbour.begin(), vecNeighbour.end(), SORT_BY_PROXIMITY);
		int limit = origin->neighbours;
		if (limit > vSize) limit = vSize;
		for (unsigned i = 0; i < limit; i++) {
			if (std::find(origin->neighbour.begin(), origin->neighbour.end(), vecNeighbour[i]) == origin->neighbour.end()) {
				origin->neighbour.push_back(vecNeighbour[i]);
				vecNeighbour[i]->neighbour.push_back(origin);
			}
			if (origin->neighbour.size() == origin->neighbours) break;
		}
		//this loop is VERY slow
		/*for (vSize = points.size() - 1; vSize > 0; vSize--) {
			origin = &points[vSize];
			tempSet.clear();
			for (unsigned i = 0; i < vSize; i++) tempSet.insert( points[i] );		//orders every point by distance to origin
			it = tempSet.end();
			for (unsigned i = points[vSize].neighbour.size(); i < points[vSize].neighbours; i++) {
			it--;
			points[vSize].neighbour.push_back(it->original);
			it->original->neighbour.push_back(&points[vSize]);
			}
		} */

		vSize = points.size();
		vecNeighbour.clear();
		for (unsigned i = 0; i < vSize; i++) {
			if (points[i].neighbour.size() != points[i].neighbours) vecNeighbour.push_back(&points[i]);
		}
	} while (vecNeighbour.size() > 0);
	vSize = points.size();

	//Assign a number to each point, starting from the top and working down in a spiral.
	//first establish the first and second point
	Point * tempPoint = &points[0];
	for (unsigned i = 1; i < vSize; i++) { if (points[i].y > tempPoint->y) tempPoint = &points[i]; }			//get the highest point by y value
	tempPoint->ID = 0;
	tempPoint = tempPoint->neighbour[0];
	tempPoint->ID = 1;
	for (unsigned i = 2; i < vSize; i++) {
		int least[6] = { tempPoint->ID, tempPoint->ID, tempPoint->ID, tempPoint->ID, tempPoint->ID, tempPoint->ID };
		for (unsigned j = 0; j < tempPoint->neighbours; j++) {
			if (tempPoint->neighbour[j]->ID == -1) {
				for (unsigned h = 0; h < tempPoint->neighbour[j]->neighbours; h++) {
					if (tempPoint->neighbour[j]->neighbour[h]->ID != -1 && tempPoint->neighbour[j]->neighbour[h]->ID < least[j]) {
						least[j] = tempPoint->neighbour[j]->neighbour[h]->ID;
					}
				}
			}
		}
		int next = 0;
		for (unsigned j = 1; j < tempPoint->neighbours; j++) {
			if (least[j] < least[next]) next = j;
		}
		tempPoint = tempPoint->neighbour[next];
		tempPoint->ID = i;
	}

	//Put the points into order then populate the hexagons and pentagons
	for (unsigned i = 0; i < vSize; i++) {
		int least = 0; int least2 = 0;
		std::vector<Point *> tempHolder; std::vector<Point *> tempHolder2;
		
		std::sort(points[i].neighbour.begin(), points[i].neighbour.end(), [](Point* a, Point* b) { return a->y < b->y; });
		tempHolder.push_back(points[i].neighbour[0]);						//start with the lowest ID'd neighbour. I might later change it to highest y value neighbour
		//then find the next neighbour that borders the first. There will be two, find both and use the one with the lowest ID
		for (unsigned h = 1; h < points[i].neighbours; h++) {
			if(std::find(points[i].neighbour[h]->neighbour.begin(), points[i].neighbour[h]->neighbour.end(), tempHolder[0]) != points[i].neighbour[h]->neighbour.end()) {
				tempHolder2.push_back(points[i].neighbour[h]);
			}
		}

		int xSign = 1; int zSign = 1;
		if (points[i].x < 0) xSign = -1;
		if (points[i].z < 0) zSign = -1;
		if (points[i].z * zSign < points[i].x * xSign) {
			if (tempHolder2[0]->z * xSign < tempHolder2[1]->z * xSign) tempHolder.push_back(tempHolder2[1]);
			else tempHolder.push_back(tempHolder2[0]);
		}
		else {
			if (tempHolder2[0]->x * zSign > tempHolder2[1]->x * zSign) tempHolder.push_back(tempHolder2[1]);
			else tempHolder.push_back(tempHolder2[0]);
		}

		//now find the rest of them
		for (unsigned j = 2; j < points[i].neighbours; j++) {
			for (unsigned h = 1; h < points[i].neighbours; h++) {
				if(std::find(tempHolder.begin(), tempHolder.end(), points[i].neighbour[h]) == tempHolder.end()
				&& std::find(points[i].neighbour[h]->neighbour.begin(), points[i].neighbour[h]->neighbour.end(), tempHolder[j - 1]) != points[i].neighbour[h]->neighbour.end()) {
					tempHolder.push_back(points[i].neighbour[h]);
					h = points[i].neighbours;					//break this loop
				}
			}
		}
		points[i].neighbour = tempHolder;

		faces.push_back(Face(points[i].neighbours, points[i].ID));		//then create the hexagon or pentagon
		Vector3f temp3f;
		faces[i].corners.push_back((points[i] + *points[i].neighbour[0] + *points[i].neighbour[points[i].neighbours - 1]) / 3);
		temp3f += (points[i] + *points[i].neighbour[0] + *points[i].neighbour[points[i].neighbours - 1]) / 3;
		for (unsigned j = 1; j < points[i].neighbours; j++) {
			faces[i].corners.push_back((points[i] + *points[i].neighbour[j] + *points[i].neighbour[j - 1]) / 3);
			temp3f += (points[i] + *points[i].neighbour[j] + *points[i].neighbour[j - 1]) / 3;
		}
		faces[i].center = temp3f / faces[i].n;
	}

	std::sort(faces.begin(), faces.end(), [](Face a, Face b) { return a.ID > b.ID; });		//now sort the faces by ID for testing purposes. This will ruin pointers but its okay for now as they are not currently needed
	
	//Now lets make the masks. First increase the dimensions so that they just barely overtake the faces
	masks = faces;
	for (unsigned int i = 0; i < vSize; i++) {
		masks[i].center *= 1.0001;
		for (unsigned int j = 0; j < faces[i].n; j++) {
			masks[i].corners[j] *= 1.0001;
		}
	}

	std::vector<Vertex> _VBO;
	radius = 2 / (faces[1].center - faces[0].center).length();
	for (unsigned i = 0; i < faces.size(); i++) {
		if (faces[i].ID != -1) {
			int j;
			for (j = faces[i].ID; j > 100; j -= 100) {}
			if (faces[i].n == 6) {
				_VBO.push_back(Vertex(faces[i].corners[5] * radius, Vector2f(0.0f, 0.075f) + textureGrid[j], faces[i].center));
				_VBO.push_back(Vertex(faces[i].corners[4] * radius, Vector2f(0.05f, 0.1f) + textureGrid[j], faces[i].center));
				_VBO.push_back(Vertex(faces[i].corners[3] * radius, Vector2f(0.1f, 0.075f) + textureGrid[j], faces[i].center));

				_VBO.push_back(Vertex(faces[i].corners[5] * radius, Vector2f(0.0f, 0.075f) + textureGrid[j], faces[i].center));
				_VBO.push_back(Vertex(faces[i].corners[3] * radius, Vector2f(0.1f, 0.075f) + textureGrid[j], faces[i].center));
				_VBO.push_back(Vertex(faces[i].corners[2] * radius, Vector2f(0.1f, 0.025f) + textureGrid[j], faces[i].center));

				_VBO.push_back(Vertex(faces[i].corners[0] * radius, Vector2f(0.0f, 0.025f) + textureGrid[j], faces[i].center));
				_VBO.push_back(Vertex(faces[i].corners[5] * radius, Vector2f(0.0f, 0.075f) + textureGrid[j], faces[i].center));
				_VBO.push_back(Vertex(faces[i].corners[2] * radius, Vector2f(0.1f, 0.025f) + textureGrid[j], faces[i].center));

				_VBO.push_back(Vertex(faces[i].corners[1] * radius, Vector2f(0.05f, 0.0f) + textureGrid[j], faces[i].center));
				_VBO.push_back(Vertex(faces[i].corners[0] * radius, Vector2f(0.0f, 0.025f) + textureGrid[j], faces[i].center));
				_VBO.push_back(Vertex(faces[i].corners[2] * radius, Vector2f(0.1f, 0.025f) + textureGrid[j], faces[i].center));
			}
			if (faces[i].n == 5) {
				_VBO.push_back(Vertex(faces[i].corners[4] * radius, Vector2f(0.0f, 0.062f) + textureGrid[j], faces[i].center));
				_VBO.push_back(Vertex(faces[i].corners[3] * radius, Vector2f(0.05f, 0.1f) + textureGrid[j], faces[i].center));
				_VBO.push_back(Vertex(faces[i].corners[2] * radius, Vector2f(0.1f, 0.062f) + textureGrid[j], faces[i].center));

				_VBO.push_back(Vertex(faces[i].corners[0] * radius, Vector2f(0.02f, 0.0f) + textureGrid[j], faces[i].center));
				_VBO.push_back(Vertex(faces[i].corners[4] * radius, Vector2f(0.0f, 0.062f) + textureGrid[j], faces[i].center));
				_VBO.push_back(Vertex(faces[i].corners[2] * radius, Vector2f(0.1f, 0.062f) + textureGrid[j], faces[i].center));

				_VBO.push_back(Vertex(faces[i].corners[1] * radius, Vector2f(0.08f, 0.0f) + textureGrid[j], faces[i].center));
				_VBO.push_back(Vertex(faces[i].corners[0] * radius, Vector2f(0.02f, 0.0f) + textureGrid[j], faces[i].center));
				_VBO.push_back(Vertex(faces[i].corners[2] * radius, Vector2f(0.1f, 0.062f) + textureGrid[j], faces[i].center));
			}
		}
	}
	glGenBuffers(1, &VBO);
	numV = _VBO.size();
	roundNormals(&_VBO[0], numV);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, numV * sizeof(Vertex), &_VBO[0], GL_STATIC_DRAW);
	//end of test code

	//debug ===============================================================================================================
	newTime = SDL_GetTicks();
	std::cout << (newTime - currentTime) << '\n';
	currentTime = SDL_GetTicks();
}
