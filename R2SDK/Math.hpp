#pragma once

namespace Math
{
	inline float DotProduct(Vector& v1, float* v2)
	{
		return v1.x * v2[0] + v1.y * v2[1] + v1.z * v2[2];
	}

	inline void VectorTransform(Vector& in1, matrix3x4_t& in2, Vector& out)
	{
		out.x = DotProduct(in1, in2[0]) + in2[0][3];
		out.y = DotProduct(in1, in2[1]) + in2[1][3];
		out.z = DotProduct(in1, in2[2]) + in2[2][3];
	}

	inline void VectorSubtract(const Vector& a, const Vector& b, Vector& c)
	{
		CHECK_VALID(a);
		CHECK_VALID(b);
		c.x = a.x - b.x;
		c.y = a.y - b.y;
		c.z = a.z - b.z;
	}

	inline void VectorClear(Vector& a)
	{
		a.x = a.y = a.z = 0.0f;
	}

	inline void VectorCopy(const Vector& src, Vector& dst)
	{
		CHECK_VALID(src);
		dst.x = src.x;
		dst.y = src.y;
		dst.z = src.z;
	}

	inline Vector CalcAngle(const Vector& source, const Vector& destination)
	{
		Vector angles = Vector(0.0f, 0.0f, 0.0f);
		Vector delta = (source - destination);
		float fHyp = sqrt((delta.x * delta.x) + (delta.y * delta.y));

		angles.x = (atanf(delta.z / fHyp) * M_RADPI);
		angles.y = (atanf(delta.y / delta.x) * M_RADPI);
		angles.z = 0.0f;

		if (delta.x >= 0.0f)
			angles.y += 180.0f;

		return angles;
	}

	inline Vector ClampAngles(Vector Angle)
	{
		if (Angle.x < -89.f)
			Angle.x = -89.f;

		if (Angle.x > 89.f)
			Angle.x = 89.f;

		while (Angle.y < -180.f)
			Angle.y += 360.f;

		while (Angle.y > 180.f)
			Angle.y -= 360.f;

		Angle.z = 0.f;

		return Angle;
	}

	inline float CalcFov(const Vector& src, const Vector& dst)
	{
		Vector v_src = Vector();
		AngleVectors(src, &v_src);

		Vector v_dst = Vector();
		AngleVectors(dst, &v_dst);

		float result = RAD2DEG(acos(v_dst.Dot(v_src) / v_dst.LengthSqr()));

		//if (!isfinite(result) || isinf(result) || isnan(result))
			//result = 0.0f;

		return result;
	}
}