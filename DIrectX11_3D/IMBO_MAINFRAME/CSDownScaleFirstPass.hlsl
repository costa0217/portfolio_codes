cbuffer Static : register(b12) {
	//�ٿ���� �̹����� �� �ȼ� ��
	uint Domain : packoffset(c0.x);
	//ù��° pass�� ����� �׷� ��
	uint GroupSize : packoffset(c0.y);
	//bloom �Ӱ谪 ����
	float fBloomThreshold : packoffset(c0.z);
}
cbuffer stCSGlobalBufferData : register(b13) {
	uint nWidth : packoffset(c0);
	uint nHeight: packoffset(c0.y);
	uint nHalfWidth: packoffset(c0.z);
	uint nHalfHeight: packoffset(c0.w);;
	uint nQuadWidth: packoffset(c1.x);
	uint nQuadHeight: packoffset(c1.y);
};
cbuffer Dynamic : register(b0) {
	//������
	float Adaptation : packoffset(c0);
}


//������ ������ �ؽ���
Texture2D HDRTex : register(t0);

//��� group���� ��ŭ�� AverageLum
RWStructuredBuffer<float> AverageLum : register(u1);
//bloom down scale texture ����
RWTexture2D<float4> HDRDownScale : register(u0);

groupshared float SharedPositions[1024];

static const float4 LUM_FACTOR = float4(0.299, 0.587, 0.114, 0);

float DownScale4x4(uint2 CurPixel, uint groupThreadId) {
	float avgLum = 0.0;

	//Skip out of boud pixels
	if (CurPixel.y < nQuadHeight) {
		//Sum a group of 4x4 pixels
		int3 nFullResPos = int3(CurPixel * 4, 0);
		float4 downScaled = float4(0.0, 0.0, 0.0, 0.0);
		[unroll]
		for (int i = 0; i < 4; ++i) {
			[unroll]
			for (int j = 0; j < 4; ++j) {
				float4 curColor = HDRTex.Load(nFullResPos, int2(j, i));
				if (all(curColor) == false/*black*/) downScaled += float4(0.5, 0.5, 0.5, 1.0);
				else downScaled += curColor;
			}
		}
		downScaled /= 16.0;//���
						   //bloom
		HDRDownScale[CurPixel.xy] = downScaled;//1/4�̹��� �ػ� ����

											   //Calculate the lumenace value for this pixel
		avgLum = dot(downScaled, LUM_FACTOR);

		//Write the result to the shared memory
		SharedPositions[groupThreadId] = avgLum;
	}

	//Synchronize before next step
	GroupMemoryBarrierWithGroupSync();

	return avgLum;
}

float DownScale1024to4(uint dispatchThreadId, uint groupThreadId, float avgLum) {
	[unroll]
	for (uint groupSize = 4, step1 = 1, step2 = 2, step3 = 3; groupSize < 1024; groupSize *= 4, step1 *= 4, step2 *= 4, step3 *= 4) {
		//Skip out of boud pixels
		if (groupThreadId % groupSize == 0) {
			//Calculate the luminance sum for this step
			float stepAvgLum = avgLum;
			stepAvgLum += dispatchThreadId + step1 < Domain ? SharedPositions[groupThreadId + step1] : avgLum;
			stepAvgLum += dispatchThreadId + step2 < Domain ? SharedPositions[groupThreadId + step2] : avgLum;
			stepAvgLum += dispatchThreadId + step3 < Domain ? SharedPositions[groupThreadId + step3] : avgLum;

			//Store the results
			avgLum = stepAvgLum;
			SharedPositions[groupThreadId] = stepAvgLum;
		}

		//Syncronize 
		GroupMemoryBarrierWithGroupSync();
	}

	return avgLum;
}

void DownScale4to1(uint dispatchThreadId, uint groupThreadId, uint groupId, float avgLum) {
	if (groupThreadId == 0) {
		//Calculate the average lumenance for this thread group
		float fFinalAvgLum = avgLum;
		fFinalAvgLum += dispatchThreadId + 256 < Domain ? SharedPositions[groupThreadId + 256] : avgLum;
		fFinalAvgLum += dispatchThreadId + 256 < Domain ? SharedPositions[groupThreadId + 512] : avgLum;
		fFinalAvgLum += dispatchThreadId + 256 < Domain ? SharedPositions[groupThreadId + 768] : avgLum;
		fFinalAvgLum /= 1024.0;

		AverageLum[groupId] = saturate(fFinalAvgLum);
	}
}

[numthreads(1024, 1, 1)]
void main(uint3 dispatchThreadId: SV_DispatchThreadID, uint3 groupThreadId : SV_GroupThreadID, uint groupId : SV_GroupID) {
	uint2 CurPixel = uint2(dispatchThreadId.x % nQuadWidth, dispatchThreadId.x / nQuadWidth);

	float avgLum = DownScale4x4(CurPixel, groupThreadId.x);

	avgLum = DownScale1024to4(dispatchThreadId.x, groupThreadId.x, avgLum);

	DownScale4to1(dispatchThreadId.x, groupThreadId.x, groupId.x, avgLum);
}