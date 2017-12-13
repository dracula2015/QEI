#include "VectorMatrix.h"

Vector3f OMRS_model(Vector3f u, Vector3f q, Vector3f dq)
{
	Vector3f DDQ;
	Parameter P;
	Matrix Rav(cos(q.z), -sin(q.z), 0, sin(q.z), cos(q.z), 0, 0, 0, 1);
	Matrix DRav(-sin(q.z), -cos(q.z), 0, cos(q.z), -sin(q.z), 0, 0, 0, 0);
	Matrix M2av(1.5*P.beta0 + P.m, 0, 0, 0, 1.5 * P.beta0 + P.m, 0, 0, 0, 3 * P.beta0*pow(P.La, 2) + P.Iv);
	Matrix C2av(1.5*P.beta1, -P.m*dq.z, 0, P.m*dq.z, 1.5*P.beta1, 0, 0, 0, 3 * P.beta1*pow(P.La, 2));
	Matrix RavRev(cos(q.z), sin(q.z), 0, -sin(q.z), cos(q.z), 0, 0, 0, 1);
	Matrix Mav = M2av*RavRev;
	Matrix Cav = C2av*RavRev - M2av*RavRev*DRav*RavRev;
	Matrix BavOri(-0.5, -0.5, 1, 0.866, -0.866, 0, P.La, P.La, P.La);
	Matrix Bav = BavOri*P.beta2;
	DDQ = Mav.Inverse()*(Bav*u-Cav*dq);
	return DDQ;
}