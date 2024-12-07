#pragma once

class Camera
{
public:

	Camera(void)
		: m_vOrigin(VECTOR3::O),
		  m_vAngles(VECTOR3::O),
		  m_vForward(VECTOR3::OZ),
		  m_vRight(VECTOR3::OX),
		  m_vUp(VECTOR3::OY)
	{
	}

	Camera(const VECTOR3& origin, const VECTOR3& angles)
		: m_vOrigin(origin),
		  m_vAngles(angles)
	{
		BuildMatrix();
		m_vUp = VECTOR3::OY;
	}

	void Move(float accel)
	{
		m_vOrigin += m_vForward*accel;
	}

	void Strafe(float accel)
	{
		m_vOrigin += m_vRight*accel;
	}

	// calc forward-right-up vectors
	void BuildMatrix()
	{
		// TODO: instead finding forward and right moving vectors
		// possible take these given from projecting matrix;
		// (note that in this case `m_viewPn` must be negate!)
		float sina, cosa, sinb, cosb;
		::sincos(DEG2RADF(m_vAngles[0]), &sina, &cosa);
		::sincos(DEG2RADF(m_vAngles[1]), &sinb, &cosb);
		m_vForward(cosa*sinb, sina, cosa*cosb);
		m_vRight(cosb, 0, -sinb);
		//m_vUp = m_vForward ^ m_vRight;	// `up` always {0,1,0}
	}

//private:
	VECTOR3 m_vOrigin, m_vAngles,
			m_vForward, m_vRight, m_vUp;

public:
	static Camera Global[16];		// up to 16 global world cameras
	static Camera* pCurrent;
};