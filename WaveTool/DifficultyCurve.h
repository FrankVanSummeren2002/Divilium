#pragma once

namespace divil
{
	class Serializer;
	struct Point
	{
		float x = 0;
		float y = 0;
		Point() {}
		Point(float aX, float aY)
		{
			x = aX;
			y = aY;
		}
		Point operator+(const Point& b) { Point a;  a.x = x + b.x;	a.y = y + b.y; return a; }

		Point operator-(const Point& b) { Point a;  a.x = x - b.x;	a.y = y - b.y; return a; }

		Point operator*(const Point& b) { Point a;  a.x = x * b.x;	a.y = y * b.y; return a; }

		Point operator/(const Point& b) { Point a;  a.x = x / b.x;	a.y = y / b.y; return a; }
	};

	//The bezier point holds two points
	//the Begining of a line
	//and the gradient point which should be between 0 and 1
	//the ending of the line will be the beginning of the next
	struct BezierPoint
	{
		Point mBegin;
		Point mGradient;
	};

	//this class is a quadratic Bezier curve
	//The difference with a normal quadratic
	//Bezier curve that this is sorted on the x-Axis
	class DifficultyCurve
	{

		bool FindBezierPoints(float value,Point& aBegin,Point& aEnd, Point& Grad);

	public:
		Array<BezierPoint> mBezierList;

		DifficultyCurve();

		//The gradient should be relative to the begin and ending
		void AddBezierPoint(Point aPoint, Point aGrad = {0,0});
		//Get all of the BezierPoints
		int GetBezierSize();
		//delete a Bezier point at an index
		void RemovePoint(int index);
		//returns a single point of the curve Returns a nullptr if it could not find it
		//if the x value is changed BezierReorderList should be called
		BezierPoint* GetBezierPoint(int index);

		//retuns false if there where not enough points
		//Bezier will search on the X axis
		//the Y value will be the returned value
		bool GetBezierValue(float xValue,float& yValue);
		//Reorder the Bezier list based on the X axis
		void BezierReorderList();
	};
	void Serialize(Serializer& ser, BezierPoint& datum);
	void Serialize(Serializer& ser, DifficultyCurve& datum);
}


