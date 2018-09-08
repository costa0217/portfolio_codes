#include "Quaternion.hlsli"

cbuffer ViewProjectionConstantBuffer : register(b11)
{
	matrix gmtxViewProjection;
};
struct VS_INS_OUTPUT {
	//	float3 size : SIZE;
	float3 position : POSITION;
	float3 extend : EXTEND;
	float4 quaternion : QUATERNION;
	float3 color : COLOR;
};

struct GS_INS_OUTPUT {
	float4 position : SV_POSITION;
	float3 positionW : POSITION;
	float3 color : COLOR;
};

[maxvertexcount(24)]
void main(point VS_INS_OUTPUT input[1], inout LineStream<GS_INS_OUTPUT> stream) {
	GS_INS_OUTPUT v[8];

	//�� ��ǥ
	float3 position = input[0].position;

	float fExtendW = input[0].extend.x;
	float fExtendH = input[0].extend.y;
	float fExtendZ = input[0].extend.z;

	//���� ��ǥ �� 8�� �̸� ���
	float3 positionW[8];
	//ȸ�� + �̵�
	positionW[0] = QuaternionRotation(float3(0 - fExtendW, 0 - fExtendH, 0 - fExtendZ), input[0].quaternion) + position;
	positionW[1] = QuaternionRotation(float3(0 - fExtendW, 0 + fExtendH, 0 - fExtendZ), input[0].quaternion) + position;
	positionW[2] = QuaternionRotation(float3(0 + fExtendW, 0 - fExtendH, 0 - fExtendZ), input[0].quaternion) + position;
	positionW[3] = QuaternionRotation(float3(0 + fExtendW, 0 + fExtendH, 0 - fExtendZ), input[0].quaternion) + position;
	positionW[4] = QuaternionRotation(float3(0 + fExtendW, 0 - fExtendH, 0 + fExtendZ), input[0].quaternion) + position;
	positionW[5] = QuaternionRotation(float3(0 + fExtendW, 0 + fExtendH, 0 + fExtendZ), input[0].quaternion) + position;
	positionW[6] = QuaternionRotation(float3(0 - fExtendW, 0 - fExtendH, 0 + fExtendZ), input[0].quaternion) + position;
	positionW[7] = QuaternionRotation(float3(0 - fExtendW, 0 + fExtendH, 0 + fExtendZ), input[0].quaternion) + position;

	///////////////////////////////11
	v[0].positionW = positionW[0];
	v[0].position = mul(float4(v[0].positionW, 1.0f), gmtxViewProjection);
	v[0].color = input[0].color;

	v[1].positionW = positionW[1];
	v[1].position = mul(float4(v[1].positionW, 1.0f), gmtxViewProjection);
	v[1].color = input[0].color;

	v[2].positionW = positionW[2];
	v[2].position = mul(float4(v[2].positionW, 1.0f), gmtxViewProjection);
	v[2].color = input[0].color;

	v[3].positionW = positionW[3];
	v[3].position = mul(float4(v[3].positionW, 1.0f), gmtxViewProjection);
	v[3].color = input[0].color;

	///////////////////////////////22
	v[4].positionW = positionW[4];
	v[4].position = mul(float4(v[4].positionW, 1.0f), gmtxViewProjection);
	v[4].color = input[0].color;

	v[5].positionW = positionW[5];
	v[5].position = mul(float4(v[5].positionW, 1.0f), gmtxViewProjection);
	v[5].color = input[0].color;

	v[6].positionW = positionW[6];
	v[6].position = mul(float4(v[6].positionW, 1.0f), gmtxViewProjection);
	v[6].color = input[0].color;

	v[7].positionW = positionW[7];
	v[7].position = mul(float4(v[7].positionW, 1.0f), gmtxViewProjection);
	v[7].color = input[0].color;

	//11
	stream.Append(v[1]);
	stream.Append(v[3]);
	stream.Append(v[5]);
	stream.Append(v[7]);

	//22
	stream.RestartStrip();
	stream.Append(v[0]);
	stream.Append(v[1]);
	stream.Append(v[7]);
	stream.Append(v[6]);

	stream.RestartStrip();
	stream.Append(v[2]);
	stream.Append(v[0]);
	stream.Append(v[6]);
	stream.Append(v[4]);

	stream.RestartStrip();
	stream.Append(v[3]);
	stream.Append(v[2]);
	stream.Append(v[4]);
	stream.Append(v[5]);
}



