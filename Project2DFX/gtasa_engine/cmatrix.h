#ifndef _CMATRIX
#define _CMATRIX

class CMatrix
{
public:
	RwMatrix	matrix;
	RwMatrix*	pMatrix;
	int		    haveRwMatrix;

public:
	inline CMatrix()
		: pMatrix(nullptr), haveRwMatrix(FALSE)
	{}

	inline CMatrix(RwMatrix* pMatrix, bool bHasMatrix=false)
		: pMatrix(nullptr)
	{ Attach(pMatrix, bHasMatrix); }

	inline CMatrix(const CMatrix& theMatrix)
		: pMatrix(nullptr), haveRwMatrix(FALSE), matrix(theMatrix.matrix)
	{}

	inline CMatrix(const CVector& vecRight, const CVector& vecUp, const CVector& vecAt, const CVector& vecPos)
	{
		matrix.right.x = vecRight.x;
		matrix.right.y = vecRight.y;
		matrix.right.z = vecRight.z;

		matrix.up.x = vecUp.x;
		matrix.up.y = vecUp.y;
		matrix.up.z = vecUp.z;

		matrix.at.x = vecAt.x;
		matrix.at.y = vecAt.y;
		matrix.at.z = vecAt.z;

		matrix.pos.x = vecPos.x;
		matrix.pos.y = vecPos.y;
		matrix.pos.z = vecPos.z;
	}

	int(__cdecl *RwMatrixDestroy)(RwMatrix* mpMat) = (int(__cdecl *)(RwMatrix* mpMat))0x7F2A20;

	inline ~CMatrix()
		{	if ( haveRwMatrix && pMatrix ) 
				RwMatrixDestroy(pMatrix); }

	friend inline CMatrix operator*(const CMatrix& Rot1, const CMatrix& Rot2)
		{ return CMatrix(	CVector(Rot1.matrix.right.x * Rot2.matrix.right.x + Rot1.matrix.right.y * Rot2.matrix.up.x + Rot1.matrix.right.z * Rot2.matrix.at.x + Rot2.matrix.pos.x,
								Rot1.matrix.right.x * Rot2.matrix.right.y + Rot1.matrix.right.y * Rot2.matrix.up.y + Rot1.matrix.right.z * Rot2.matrix.at.y + Rot2.matrix.pos.y,
								Rot1.matrix.right.x * Rot2.matrix.right.z + Rot1.matrix.right.y * Rot2.matrix.up.z + Rot1.matrix.right.z * Rot2.matrix.at.z + Rot2.matrix.pos.z),
						CVector(Rot1.matrix.up.x * Rot2.matrix.right.x + Rot1.matrix.up.y * Rot2.matrix.up.x + Rot1.matrix.up.z * Rot2.matrix.at.x + Rot2.matrix.pos.x,
								Rot1.matrix.up.x * Rot2.matrix.right.y + Rot1.matrix.up.y * Rot2.matrix.up.y + Rot1.matrix.up.z * Rot2.matrix.at.y + Rot2.matrix.pos.y,
								Rot1.matrix.up.x * Rot2.matrix.right.z + Rot1.matrix.up.y * Rot2.matrix.up.z + Rot1.matrix.up.z * Rot2.matrix.at.z + Rot2.matrix.pos.z),
						CVector(Rot1.matrix.at.x * Rot2.matrix.right.x + Rot1.matrix.at.y * Rot2.matrix.up.x + Rot1.matrix.at.z * Rot2.matrix.at.x + Rot2.matrix.pos.x,
								Rot1.matrix.at.x * Rot2.matrix.right.y + Rot1.matrix.at.y * Rot2.matrix.up.y + Rot1.matrix.at.z * Rot2.matrix.at.y + Rot2.matrix.pos.y,
								Rot1.matrix.at.x * Rot2.matrix.right.z + Rot1.matrix.at.y * Rot2.matrix.up.z + Rot1.matrix.at.z * Rot2.matrix.at.z + Rot2.matrix.pos.z),
						CVector(Rot1.matrix.pos.x * Rot2.matrix.right.x + Rot1.matrix.pos.y * Rot2.matrix.up.x + Rot1.matrix.pos.z * Rot2.matrix.at.x + Rot2.matrix.pos.x,
								Rot1.matrix.pos.x * Rot2.matrix.right.y + Rot1.matrix.pos.y * Rot2.matrix.up.y + Rot1.matrix.pos.z * Rot2.matrix.at.y + Rot2.matrix.pos.y,
								Rot1.matrix.pos.x * Rot2.matrix.right.z + Rot1.matrix.pos.y * Rot2.matrix.up.z + Rot1.matrix.pos.z * Rot2.matrix.at.z + Rot2.matrix.pos.z)); };

	friend inline CVector operator*(const CMatrix& matrix, const CVector& vec)
			{ return CVector(matrix.matrix.up.x * vec.y + matrix.matrix.right.x * vec.x + matrix.matrix.at.x * vec.z + matrix.matrix.pos.x,
								matrix.matrix.up.y * vec.y + matrix.matrix.right.y * vec.x + matrix.matrix.at.y * vec.z + matrix.matrix.pos.y,
								matrix.matrix.up.z * vec.y + matrix.matrix.right.z * vec.x + matrix.matrix.at.z * vec.z + matrix.matrix.pos.z); };

	friend inline CMatrix operator+(const CMatrix& Rot1, const CMatrix& Rot2)
	{ return CMatrix(	CVector(Rot1.matrix.right.x + Rot2.matrix.right.x, Rot1.matrix.right.y + Rot2.matrix.right.y, Rot1.matrix.right.z + Rot2.matrix.right.z),
						CVector(Rot1.matrix.up.x + Rot2.matrix.up.x, Rot1.matrix.up.y + Rot2.matrix.up.y, Rot1.matrix.up.z + Rot2.matrix.up.z),
						CVector(Rot1.matrix.at.x + Rot2.matrix.at.x, Rot1.matrix.at.y + Rot2.matrix.at.y, Rot1.matrix.at.z + Rot2.matrix.at.z),
						CVector(Rot1.matrix.pos.x + Rot2.matrix.pos.x, Rot1.matrix.pos.y + Rot2.matrix.pos.y, Rot1.matrix.pos.z + Rot2.matrix.pos.z)); }

	inline CMatrix& operator=(const CMatrix& mat)
	{
		matrix = mat.matrix;
		if ( this->pMatrix )
			UpdateRwMatrix(pMatrix);
		return *this;
	}

	inline CMatrix& operator+=(const CMatrix& mat)
	{
		matrix.right.x += mat.matrix.right.x;
		matrix.right.y += mat.matrix.right.y;
		matrix.right.z += mat.matrix.right.z;

		matrix.up.x += mat.matrix.up.x;
		matrix.up.y += mat.matrix.up.y;
		matrix.up.z += mat.matrix.up.z;

		matrix.at.x += mat.matrix.at.x;
		matrix.at.y += mat.matrix.at.y;
		matrix.at.z += mat.matrix.at.z;

		matrix.pos.x += mat.matrix.pos.x;
		matrix.pos.y += mat.matrix.pos.y;
		matrix.pos.z += mat.matrix.pos.z;

		return *this;
	}

	friend inline CMatrix& Invert(const CMatrix& src, CMatrix& dst)
	{
		dst.matrix.right.x = src.matrix.right.x;
		dst.matrix.right.y = src.matrix.up.x;
		dst.matrix.right.z = src.matrix.at.x;

		dst.matrix.up.x = src.matrix.right.y;
		dst.matrix.up.y = src.matrix.up.y;
		dst.matrix.up.z = src.matrix.at.y;

		dst.matrix.at.x = src.matrix.right.z;
		dst.matrix.at.y = src.matrix.up.z;
		dst.matrix.at.z = src.matrix.at.z;

		dst.matrix.pos.x = dst.matrix.right.x * src.matrix.pos.x;
		dst.matrix.pos.y = dst.matrix.right.y * src.matrix.pos.x;
		dst.matrix.pos.z = dst.matrix.right.z * src.matrix.pos.x;

		dst.matrix.pos.x += dst.matrix.up.x * src.matrix.pos.y;
		dst.matrix.pos.y += dst.matrix.up.y * src.matrix.pos.y;
		dst.matrix.pos.z += dst.matrix.up.z * src.matrix.pos.y;

		dst.matrix.pos.x += dst.matrix.at.x * src.matrix.pos.z;
		dst.matrix.pos.y += dst.matrix.at.y * src.matrix.pos.z;
		dst.matrix.pos.z += dst.matrix.at.z * src.matrix.pos.z;

		dst.matrix.pos.x = -dst.matrix.pos.x;
		dst.matrix.pos.y = -dst.matrix.pos.y;
		dst.matrix.pos.z = -dst.matrix.pos.z;

		return dst;
	}

	friend inline CMatrix Invert(const CMatrix& src)
	{
		CMatrix		NewMatrix;
		return CMatrix(Invert(src, NewMatrix));
	}

	friend inline CVector Multiply3x3(const CMatrix& matrix, const CVector& vec)
			{ return CVector(matrix.matrix.up.x * vec.y + matrix.matrix.right.x * vec.x + matrix.matrix.at.x * vec.z,
								matrix.matrix.up.y * vec.y + matrix.matrix.right.y * vec.x + matrix.matrix.at.y * vec.z,
								matrix.matrix.up.z * vec.y + matrix.matrix.right.z * vec.x + matrix.matrix.at.z * vec.z); };

	friend inline CVector Multiply3x3(const CVector& vec, const CMatrix& matrix)
			{ return CVector(matrix.matrix.right.x * vec.x + matrix.matrix.right.y * vec.y + matrix.matrix.right.z * vec.z,
								matrix.matrix.up.x * vec.x + matrix.matrix.up.y * vec.y + matrix.matrix.up.z * vec.z,
								matrix.matrix.at.x * vec.x + matrix.matrix.at.y * vec.y + matrix.matrix.at.z * vec.z); };

	inline CVector*	GetRight()
		{ return reinterpret_cast<CVector*>(&matrix.right); }
	inline CVector*	GetUp()
		{ return reinterpret_cast<CVector*>(&matrix.up); }
	inline CVector*	GetAt()
		{ return reinterpret_cast<CVector*>(&matrix.at); }
	inline CVector* GetPos()
		{ return reinterpret_cast<CVector*>(&matrix.pos); }

	inline void		SetTranslateOnly(float fX, float fY, float fZ)
		{ matrix.pos.x = fX; matrix.pos.y = fY; matrix.pos.z = fZ; }
	
	inline void		SetRotateX(float fAngle)
		{ SetRotateXOnly(fAngle); matrix.pos.x = 0.0f; matrix.pos.y = 0.0f; matrix.pos.z = 0.0f; }
	inline void		SetRotateY(float fAngle)
		{ SetRotateYOnly(fAngle); matrix.pos.x = 0.0f; matrix.pos.y = 0.0f; matrix.pos.z = 0.0f; }
	inline void		SetRotateZ(float fAngle)
		{ SetRotateZOnly(fAngle); matrix.pos.x = 0.0f; matrix.pos.y = 0.0f; matrix.pos.z = 0.0f; }
	inline void		SetRotate(float fAngleX, float fAngleY, float fAngleZ)
		{ SetRotateOnly(fAngleX, fAngleY, fAngleZ); matrix.pos.x = 0.0f; matrix.pos.y = 0.0f; matrix.pos.z = 0.0f; }
	inline void		SetTranslate(float fX, float fY, float fZ)	
		{	matrix.right.x = 1.0f; matrix.right.y = 0.0f; matrix.right.z = 0.0f;
			matrix.up.x = 0.0f; matrix.up.y = 1.0f; matrix.up.z = 0.0f;
			matrix.at.x = 0.0f; matrix.at.y = 0.0f; matrix.at.z = 1.0f;
			SetTranslateOnly(fX, fY, fZ); }

	inline void		ResetOrientation()
	{	
		matrix.right.x = 1.0f; matrix.right.y = 0.0f; matrix.right.z = 0.0f;
		matrix.up.x = 0.0f; matrix.up.y = 1.0f; matrix.up.z = 0.0f;
		matrix.at.x = 0.0f; matrix.at.y = 0.0f; matrix.at.z = 1.0f;
	}

	inline void		SetUnity()
	{	
		matrix.right.x = 1.0f; matrix.right.y = 0.0f; matrix.right.z = 0.0f;
		matrix.up.x = 0.0f; matrix.up.y = 1.0f; matrix.up.z = 0.0f;
		matrix.at.x = 0.0f; matrix.at.y = 0.0f; matrix.at.z = 1.0f;
		matrix.pos.x = 0.0f; matrix.pos.y = 0.0f; matrix.pos.z = 0.0f;
	}

	inline void		SetScale(float fScale)
	{	
		matrix.right.x = fScale; matrix.right.y = 0.0f; matrix.right.z = 0.0f;
		matrix.up.x = 0.0f; matrix.up.y = fScale; matrix.up.z = 0.0f;
		matrix.at.x = 0.0f; matrix.at.y = 0.0f; matrix.at.z = fScale;
		matrix.pos.x = 0.0f; matrix.pos.y = 0.0f; matrix.pos.z = 0.0f;
	}

	inline void		RotateX(float fAngle)
	{
		CMatrix		RotationMatrix;
		RotationMatrix.SetRotateX(fAngle);
		*this = *this * RotationMatrix;
	}

	inline void		RotateY(float fAngle)
	{
		CMatrix		RotationMatrix;
		RotationMatrix.SetRotateY(fAngle);
		*this = *this * RotationMatrix;
	}

	inline void		RotateZ(float fAngle)
	{
		CMatrix		RotationMatrix;
		RotationMatrix.SetRotateZ(fAngle);
		*this = *this * RotationMatrix;
	}

	inline void		Rotate(float fAngleX, float fAngleY, float fAngleZ)
	{
		CMatrix		RotationMatrix;
		RotationMatrix.SetRotate(fAngleX, fAngleY, fAngleZ);
		*this = *this * RotationMatrix;
	}

	inline void		SetRotateXOnly(float fAngle)
	{
		matrix.right.x = 1.0f;
		matrix.right.y = 0.0f;
		matrix.right.z = 0.0f;

		matrix.up.x = 0.0f;
		matrix.up.y = cos(fAngle);
		matrix.up.z = sin(fAngle);

		matrix.at.x = 0.0f;
		matrix.at.y = -sin(fAngle);
		matrix.at.z = cos(fAngle);
	}

	inline void		SetRotateYOnly(float fAngle)
	{
		matrix.right.x = cos(fAngle);
		matrix.right.y = 0.0f;
		matrix.right.z = sin(fAngle);

		matrix.up.x = 0.0f;
		matrix.up.y = 1.0f;
		matrix.up.z = 0.0f;

		matrix.at.x = -sin(fAngle);
		matrix.at.y = 0.0f;
		matrix.at.z = cos(fAngle);
	}

	inline void		SetRotateZOnly(float fAngle)
	{
		matrix.at.x = 0.0f;
		matrix.at.y = 0.0f;
		matrix.at.z = 1.0f;

		matrix.up.x = -sin(fAngle);
		matrix.up.y = cos(fAngle);
		matrix.up.z = 0.0f;

		matrix.right.x = cos(fAngle);
		matrix.right.y = sin(fAngle);
		matrix.right.z = 0.0f;
	}

	inline void		SetRotateOnly(float fAngleX, float fAngleY, float fAngleZ)
	{
		matrix.right.x = cos(fAngleZ) * cos(fAngleY) - sin(fAngleZ) * sin(fAngleX) * sin(fAngleY);
		matrix.right.y = cos(fAngleZ) * sin(fAngleX) * sin(fAngleY) + sin(fAngleZ) * cos(fAngleY);
		matrix.right.z = -cos(fAngleX) * sin(fAngleY);

		matrix.up.x = -sin(fAngleZ) * cos(fAngleX);
		matrix.up.y = cos(fAngleZ) * cos(fAngleX);
		matrix.up.z = sin(fAngleX);

		matrix.at.x = sin(fAngleZ) * sin(fAngleX) * cos(fAngleY) + cos(fAngleZ) * sin(fAngleY);
		matrix.at.y = sin(fAngleZ) * sin(fAngleY) - cos(fAngleZ) * sin(fAngleX) * cos(fAngleY);
		matrix.at.z = cos(fAngleX) * cos(fAngleY);
	}

	inline void		Attach(RwMatrix* pMatrix, bool bHasMatrix)
	{
		if ( this->pMatrix && haveRwMatrix )
			RwMatrixDestroy(this->pMatrix);

		this->pMatrix = pMatrix;
		haveRwMatrix = bHasMatrix;

		Update();
	}

	inline void		AttachRw(RwMatrix* pMatrix, bool bHasMatrix)
	{
		if ( this->pMatrix && haveRwMatrix )
			RwMatrixDestroy(this->pMatrix);

		this->pMatrix = pMatrix;
		haveRwMatrix = bHasMatrix;

		UpdateRW();
	}

	inline void		Detach()
	{
		if ( pMatrix )
		{
			if ( haveRwMatrix )
				RwMatrixDestroy(pMatrix);
			pMatrix = nullptr;
		}
	}

	inline void		UpdateRW() const
	{
		if ( pMatrix )
			UpdateRwMatrix(pMatrix);
	}

	inline void		Update()
	{
		UpdateMatrix(pMatrix);
	}

	inline void		UpdateMatrix(RwMatrix* pMatrix)
	{
		matrix.right = pMatrix->right;
		matrix.up = pMatrix->up;
		matrix.at = pMatrix->at;
		matrix.pos = pMatrix->pos;
	}

	inline void		UpdateRwMatrix(RwMatrix* pMatrix) const
	{
		pMatrix->right = matrix.right;
		pMatrix->up = matrix.up;
		pMatrix->at = matrix.at;
		pMatrix->pos = matrix.pos;
		pMatrix->flags &= ~(rwMATRIXTYPEMASK | rwMATRIXINTERNALIDENTITY);
	}

	inline void		CopyToRwMatrix(RwMatrix* pMatrix) const
	{
		pMatrix->right = this->pMatrix->right;
		pMatrix->up = this->pMatrix->up;
		pMatrix->at = this->pMatrix->at;
		pMatrix->pos = this->pMatrix->pos;
		pMatrix->flags &= ~(rwMATRIXTYPEMASK | rwMATRIXINTERNALIDENTITY);
	}

	inline void		CopyOnlyMatrix(const CMatrix& from)
	{
		matrix = from.matrix;
	}
};

#endif // _CMATRIX