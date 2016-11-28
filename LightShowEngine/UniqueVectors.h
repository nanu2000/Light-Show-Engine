#ifndef _UNIQUE_VECTORS_H
#define _UNIQUE_VECTORS_H

template<typename T> class UniqueVec2
{

	public:
		
		union
		{

			T vec2[2];

			struct {
				T x;
				T y;
			};

		};
		
		inline UniqueVec2 operator+(UniqueVec2 &v)
		{
			return UniqueVec2(v.x + x, v.y + y);
		}


		inline UniqueVec2 operator-(UniqueVec2 &v)
		{
			return UniqueVec2(v.x - x, v.y - y);
		}

		inline UniqueVec2 operator*(UniqueVec2 &v)
		{
			return UniqueVec2(v.x * x, v.y * y);
		}

		inline UniqueVec2 operator*(T &v)
		{
			return UniqueVec2(v * x, v * y);
		}

		UniqueVec2(T X, T Y)
		{
			x = X;
			y = Y;
		}

		UniqueVec2(T XY)
		{
			x = XY;
			y = XY;
		}

		UniqueVec2()
		{
			x = 0;
			y = 0;
		}

};


template<typename T> class UniqueVec3
{

public:

	union 
	{
		
		T vec3[3];

		struct {
			T x;
			T y;
			T z;
		};

	};

	inline UniqueVec3 operator+(UniqueVec3 &v)
	{
		return UniqueVec3(v.x + x, v.y + y, v.z + z);
	}


	inline UniqueVec3 operator-(UniqueVec3 &v)
	{
		return UniqueVec3(v.x - x, v.y - y, v.z - z);
	}

	inline UniqueVec3 operator*(UniqueVec3 &v)
	{
		return UniqueVec3(v.x * x, v.y * y, v.z * z);
	}

	inline UniqueVec3 operator*(T &v)
	{
		return UniqueVec3(v * x, v * y, v * z);
	}

	UniqueVec3(T X, T Y, T Z)
	{
		x = X;
		y = Y;
		z = Z;
	}

	UniqueVec3(T XYZ)
	{
		x = XYZ;
		y = XYZ;
		z = XYZ;
	}
	UniqueVec3()
	{
		x = 0;
		y = 0;
		z = 0;
	}
};




template<typename T> class UniqueVec4
{

public:


	union
	{

		T vec4[4];

		struct {
			T x;
			T y;
			T z;
			T w;
		};

	};

	inline UniqueVec4 operator+(UniqueVec4 &v)
	{
		return UniqueVec4(v.x + x, v.y + y, v.z + z, v.w + w);
	}


	inline UniqueVec4 operator-(UniqueVec4 &v)
	{
		return UniqueVec4(v.x - x, v.y - y, v.z - z, v.w - w);
	}

	inline UniqueVec4 operator*(UniqueVec4 &v)
	{
		return UniqueVec4(v.x * x, v.y * y, v.z * z, v.w * w);
	}

	inline UniqueVec4 operator*(T &v)
	{
		return UniqueVec4(v * x, v * y, v * z, v * w);
	}


	UniqueVec4(T X, T Y, T Z, T W)
	{
		x = X;
		y = Y;
		z = Z;
		w = W;
	}
	UniqueVec4(T XYZW)
	{
		x = XYZW;
		y = XYZW;
		z = XYZW;
		w = XYZW;
	}
	UniqueVec4()
	{
		x = 0;
		y = 0;
		z = 0;
		w = 0;
	}
};



#endif