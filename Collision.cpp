#include "Collision.h"
#include"calc.h"
#include"MyVector3.h"
#include<cmath>

bool Collision::IsCollision(const Sphere& s1, const Sphere& s2) {
	float length = Calc::MakeLength(s1.center_, s2.center_);

	//スケールはいじるな！お願いします。
	if (length <= s1.radius_ + s2.radius_) {
		return true;
	}
	return false;
}

bool Collision::IsCollision(const Sphere& sphere, const Plane& plane) {
	float length = Calc::MakeLength(sphere, plane);

	if (length <= sphere.radius_) {
		if (plane.isLimit) {
			MyVector3 center = plane.normal;
			center *= plane.distance;

			MyVector3 perpendiculars[4];

			perpendiculars[0] = Calc::Normalize(Calc::Perpendicular(plane.normal));
			perpendiculars[1] = { -perpendiculars[0].x,-perpendiculars[0].y, -perpendiculars[0].z };
			perpendiculars[2] = Calc::Cross(plane.normal, perpendiculars[0]);
			perpendiculars[3] = { -perpendiculars[2].x,-perpendiculars[2].y,-perpendiculars[2].z };

			for (int32_t index = 0; index < 4; index++) {
				perpendiculars[index] = center + perpendiculars[index] * 2.0f;
			}

			MyVector3 pos = Calc::Project(center - sphere.center_, plane.normal) + sphere.center_;
			float radius = sqrtf(powf(sphere.radius_, 2.0f) - powf(length, 2.0f));

			float len;
			len = Calc::MakeLength(pos, perpendiculars[0], perpendiculars[2]);
			if (len <= radius) {
				return true;
			}
			len = Calc::MakeLength(pos, perpendiculars[0], perpendiculars[3]);
			if (len <= radius) {
				return true;
			}
			len = Calc::MakeLength(pos, perpendiculars[1], perpendiculars[2]);
			if (len <= radius) {
				return true;
			}
			len = Calc::MakeLength(pos, perpendiculars[1], perpendiculars[2]);
			if (len <= radius) {
				return true;
			}

			float dot[4];

			dot[0] = Calc::Dot(plane.normal, Calc::Cross(pos - perpendiculars[2], perpendiculars[0] - pos));
			dot[1] = Calc::Dot(plane.normal, Calc::Cross(pos - perpendiculars[1], perpendiculars[2] - pos));
			dot[2] = Calc::Dot(plane.normal, Calc::Cross(pos - perpendiculars[3], perpendiculars[1] - pos));
			dot[3] = Calc::Dot(plane.normal, Calc::Cross(pos - perpendiculars[0], perpendiculars[3] - pos));

			if ((dot[0] > 0 && dot[1] > 0 && dot[2] > 0 && dot[3] > 0) || (dot[0] < 0 && dot[1] < 0 && dot[2] < 0 && dot[3] < 0)) {
				return true;
			}
		}
		else {
			return true;
		}
	}
	return false;
}

