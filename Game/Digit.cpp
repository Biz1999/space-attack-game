#include "Digit.h"


bool Digit::Initialize(float digitPositionX, float digitPositionY) {

	if (
		led1.Initialize(900.0f + digitPositionX, 50.0f + digitPositionY, 10.0f, 2.50f) &&
		led2.Initialize(912.5f + digitPositionX, 50.0f + digitPositionY, 10.0f, 2.50f) &&
		led3.Initialize(900.0f + digitPositionX, 62.5f + digitPositionY, 10.0f, 2.50f) &&
		led4.Initialize(912.5f + digitPositionX, 62.5f + digitPositionY, 10.0f, 2.50f) &&
		led5.Initialize(902.5f + digitPositionX, 47.5f + digitPositionY, 2.50f, 10.0f) &&
		led6.Initialize(902.5f + digitPositionX, 60.0f + digitPositionY, 2.50f, 10.0f) &&
		led7.Initialize(902.5f + digitPositionX, 73.0f + digitPositionY, 2.50f, 10.0f)
		)
		return true;

	return false;

}

vector<Led> Digit::checkDrawnLeds(int num)
{

	switch (num)
	{
	case 0:
		return renderNumber0();
	case 1:
		return renderNumber1();
	case 2:
		return renderNumber2();
	case 3:
		return renderNumber3();
	case 4:
		return renderNumber4();
	case 5:
		return renderNumber5();
	case 6:
		return renderNumber6();
	case 7:
		return renderNumber7();
	case 8:
		return renderNumber8();
	case 9:
		return renderNumber9();
	default:
		break;
	}


}

vector<Led> Digit::renderNumber0()
{
	return { led1, led2, led3, led4, led5, led7 };
}

vector<Led> Digit::renderNumber1()
{
	return { led2, led4 };
}

vector<Led> Digit::renderNumber2()
{
	return { led2, led3, led5, led6, led7 };
}

vector<Led> Digit::renderNumber3()
{
	return { led2, led4, led5, led6, led7 };
}

vector<Led> Digit::renderNumber4()
{
	return { led1, led2, led4, led6 };
}

vector<Led> Digit::renderNumber5()
{
	return { led1, led4, led5, led6, led7 };
}

vector<Led> Digit::renderNumber6()
{
	return { led1, led3, led4, led5, led6, led7 };
}

vector<Led> Digit::renderNumber7()
{
	return { led2,  led4, led5 };
}

vector<Led> Digit::renderNumber8()
{
	return{ led1, led2, led3, led4, led5,led6, led7 };
}

vector<Led> Digit::renderNumber9()
{
	return { led1, led2, led4, led5, led6 };
}