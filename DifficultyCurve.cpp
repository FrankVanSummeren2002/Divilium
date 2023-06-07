#include "DifficultyCurve/DifficultyCurve.h"
#include "Serialize/Serialize.h"
#include "Core/Math.h"
using namespace divil;


DifficultyCurve::DifficultyCurve()
{
}
// Find the roots for a cubic polynomial with bernstein coefficients
// {pa, pb, pc, pd}. The function will first convert those to the
// standard polynomial coefficients, and then run through Cardano's
// formula for finding the roots of a depressed cubic curve.



//New bezier Functions
void divil::DifficultyCurve::AddBezierPoint(Point aPoint,Point aGrad)
{
	BezierPoint bez;
	bez.mBegin = aPoint;
	bez.mGradient = aGrad;

	mBezierList.Add(bez);
	BezierReorderList();
}

BezierPoint* divil::DifficultyCurve::GetBezierPoint(int index)
{
	return &mBezierList[index];
}

bool divil::DifficultyCurve::GetBezierValue(float xValue, float& yValue)
{
	Point Begin;
	Point End;
	Point Grad;

	if(!FindBezierPoints(xValue, Begin, End, Grad))
		return false;

	if (Begin.x == xValue)
	{
		yValue = Begin.y;
		return true;
	}
	if (End.x == xValue)
	{
		yValue = End.y;
		return true;
	}
	//make it a qaudratic function
	//function to get Bezier curve C(t)=(1−t)2P0+2t(1−t)P1+t2P2
	//rewriten x(t)=(x0−2x1+x2)t2+2(x1−x0)t+x0
	// rewrite it so 0 = (x0−2x1+x2)t2+2(x1−x0)t+x0 - x(t)
	// fill them in
	double a = static_cast<double>(Begin.x - 2 * Grad.x + End.x);
	double b = 2.0 * static_cast<double>(Grad.x - Begin.x);
	double c = static_cast<double>(Begin.x - xValue);

	//solve the quadratic function
	double val1;
	double val2;
	if (!SolveQuadratic(a, b, c, val1, val2))
		return false;

	//find which of the two is the correct one
	double temp = 0;
	if (val1 >= 0 && val1 <= 1)
		temp = val1;
	else if (val2 >= 0 && val2 <= 1)
		temp = val2;

	//returns the y value
	float L3 = Lerp(Begin.y, Grad.y, (float)temp);
	float L4 = Lerp(Grad.y, End.y, (float)temp);
	yValue = Lerp(L3, L4, (float)temp);

	return true;
}

void divil::DifficultyCurve::BezierReorderList()
{
	int i;
	int j;

	for (i = 0; i < mBezierList.Count() - 1; i++)
	{
		for (j = 0; j < mBezierList.Count() - i - 1; j++)
		{
			if (mBezierList[j].mBegin.x > mBezierList[j + 1].mBegin.x)
			{
				BezierPoint temp = mBezierList[j];
				mBezierList[j] = mBezierList[j + 1];
				mBezierList[j + 1] = temp;
				
				if (j > 0)
				{

				}
			}
		}
	}
}

int divil::DifficultyCurve::GetBezierSize()
{
	return mBezierList.Count();
}

void divil::DifficultyCurve::RemovePoint(int index)
{
	mBezierList.RemoveOrdered(index);
}

bool divil::DifficultyCurve::FindBezierPoints(float value, Point& aBegin, Point& aEnd, Point& Grad)
{
	for (int i = 0; i < mBezierList.Count()-1; i++)
	{
		if (mBezierList[i].mBegin.x <= value && mBezierList[i+1].mBegin.x >= value)
		{
			aBegin = mBezierList[i].mBegin;
			aEnd = mBezierList[i + 1].mBegin;
			Grad = aBegin + mBezierList[i].mGradient * (aEnd - aBegin);
			return true;
		}
	}
	return false;
}

void divil::Serialize(Serializer& ser, BezierPoint& datum)
{
	assert(ser.IsValid());

	if (ser.IsWriting())
	{
		fwrite(&datum.mBegin.x, sizeof(float), 1, ser.mFile);
		fwrite(&datum.mBegin.y, sizeof(float), 1, ser.mFile);
		fwrite(&datum.mGradient.x, sizeof(float), 1, ser.mFile);
		fwrite(&datum.mGradient.y, sizeof(float), 1, ser.mFile);
	}
	else
	{
		fread(&datum.mBegin.x, sizeof(float), 1, ser.mFile);
		fread(&datum.mBegin.y, sizeof(float), 1, ser.mFile);
		fread(&datum.mGradient.x, sizeof(float), 1, ser.mFile);
		fread(&datum.mGradient.y, sizeof(float), 1, ser.mFile);
	}
}

void divil::Serialize(Serializer& ser, DifficultyCurve& datum)
{
	assert(ser.IsValid());
	Serialize(ser, datum.mBezierList);
}
