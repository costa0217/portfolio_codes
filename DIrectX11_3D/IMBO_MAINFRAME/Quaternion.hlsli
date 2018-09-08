
float4 QuaternionMul(float4 q1, float4 q2) {
	float x = q2.x*q1.w - q2.y*q1.z + q2.z*q1.y + q2.w*q1.x;
	float y = q2.x*q1.z + q2.y*q1.w - q2.z*q1.x + q2.w*q1.y;
	float z = -q2.x*q1.y + q2.y*q1.x + q2.z*q1.w + q2.w*q1.z;
	float w = -q2.x*q1.x - q2.y*q1.y - q2.z*q1.z + q2.w*q1.w;

	return float4(x, y, z, w);
}
float4 QuaternionNorm(float4 q) {
	return normalize(q);
}
float4 QuaternionConjugate(float4 q) {
	return float4(-q.x, -q.y, -q.z, q.w);
}
float3 QuaternionRotation(float3 v, float4 q) {
	float4 p = float4(v, 0.f);//ȸ���� ���͸� ���� ���ʹϾ����� ��ȯ
	q = QuaternionNorm(q);//���ʹϾ� ����ȭ
	float4 cq = QuaternionConjugate(q);//�ӷ� ���ʹϾ� ����
	return QuaternionMul(QuaternionMul(q, p), cq).xyz;//ȸ��
}