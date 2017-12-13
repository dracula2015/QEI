#include "VectorMatrix.h"

Vector3f OMRS_controller(Vector3f qd, Vector3f dqd, Vector3f ddqd, Vector3f q, Vector3f dq)
{
	Vector3f uavc;
	Parameter P;
	Matrix Ravc(cos(q.z), -sin(q.z), 0, sin(q.z), cos(q.z), 0, 0, 0, 1);
	Matrix DRavc(-sin(q.z), -cos(q.z), 0, cos(q.z), -sin(q.z), 0, 0, 0, 0);
	Matrix M2avc(1.5*P.beta0 + P.m, 0, 0, 0, 1.5 * P.beta0 + P.m, 0, 0, 0, 3 * P.beta0*pow(P.La, 2) + P.Iv);
	Matrix C2avc(1.5*P.beta1, -P.m*dq.z, 0, P.m*dq.z, 1.5*P.beta1, 0, 0, 0, 3 * P.beta1*pow(P.La, 2));
	Matrix RavcRev(cos(q.z), sin(q.z), 0, -sin(q.z), cos(q.z), 0, 0, 0, 1);
	Matrix Mavc = M2avc*RavcRev;
	Matrix Cavc = C2avc*RavcRev - M2avc*RavcRev*DRavc*RavcRev;
	Matrix BavcOri(-0.5, -0.5, 1, 0.866, -0.866, 0, P.La, P.La, P.La);
	Matrix Bavc = BavcOri*P.beta2;
	Matrix Kp(6, 0, 0, 0, 6, 0, 0, 0, 6);
	Matrix Kd(10, 0, 0, 0, 10, 0, 0, 0, 10);
	uavc = Bavc.Inverse()*Mavc*(ddqd- Kd*(dq-dqd)-Kp*(q-qd))+ Bavc.Inverse()*Cavc*dq;
	return uavc;
}