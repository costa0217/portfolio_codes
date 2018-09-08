Texture2D           gtxtDepth				: register(t0);
Texture2D           gtxtNormal				: register(t2);

//Texture2D<float> DepthTex : register(t0);
//Texture2D<float3> NormalsTex : register(t1);
RWStructuredBuffer<float4> MiniDepthRW : register(u0);

cbuffer stCSDynamicData : register(b9) {
	float4x4 ViewMatrix: packoffset(c0);//camera
};
cbuffer stCSStaticData : register(b10) {
	float4 ProjParams: packoffset(c0);   // camera
	float MaxDepth : packoffset(c1.x); ;//camera
};
cbuffer Static : register(b11)
{
	float2 ResRcp  : packoffset(c0.x);
	float OffsetRadius : packoffset(c0.z);
	float Radius : packoffset(c0.w);
}
cbuffer stCSGlobalBufferData : register(b13) {
	uint nWidth : packoffset(c0);
	uint nHeight: packoffset(c0.y);
	uint nHalfWidth: packoffset(c0.z);
	uint nHalfHeight: packoffset(c0.w);;
	uint nQuadWidth: packoffset(c1.x);
	uint nQuadHeight: packoffset(c1.y);
};
float ConvertDepthToLinear(float depth){
	float linearDepth = ProjParams.z / (depth + ProjParams.w);
	return linearDepth;
}

//총 픽셀 수를 1024로 나눈 만큼의 x스레드 그룹을 다운 스케일 한 컴퓨트 세이더에 적용
[numthreads(1024, 1, 1)]
void main( uint3 dispatchThreadId : SV_DispatchThreadID ){
	uint3 CurPixel = uint3(dispatchThreadId.x % nHalfWidth, dispatchThreadId.x / nHalfWidth, 0);
	if (CurPixel.y < nHalfHeight)
	{
		float minDepth = 1.0;
		float3 avgNormalWorld = float3(0.0, 0.0, 0.0);
		uint3 FullResPixel = CurPixel * 2;

		[unroll]
		for (int i = 0; i < 2; i++)
		{
			[unroll]
			for (int j = 0; j < 2; j++)
			{
				//가장 작은 depth를 저장한다.
				float curDepth = gtxtDepth.Load(FullResPixel, int2(j, i));
				minDepth = min(curDepth, minDepth);

				//평균 normal을 저장한다.
				float3 normalWorld = gtxtNormal.Load(FullResPixel, int2(j, i));
				avgNormalWorld += normalize(normalWorld * 2.0 - 1.0);
			}
		}

		float3 avgNormalView = mul(avgNormalWorld * 0.25, (float3x3)ViewMatrix);
		MiniDepthRW[dispatchThreadId.x].x = ConvertDepthToLinear(minDepth);
		MiniDepthRW[dispatchThreadId.x].yzw = avgNormalView;
		//MiniDepthRW[CurPixel.xy] = float4(avgNormalView, ConvertDepthToLinear(minDepth));
	}
}