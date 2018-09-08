cbuffer Static : register(b12) {
	//다운스케일 이미지의 총 픽셀 수
	uint Domain : packoffset(c0.x);
	//첫번째 pass에 적용된 그룹 수
	uint GroupSize : packoffset(c0.y);
	//bloom 임계값 비율
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
	//적응값
	float Adaptation : packoffset(c0);
}


Texture2D<float4> HDRDownScaleTex : register(t0);
StructuredBuffer<float> AvgLum : register(t1);
RWTexture2D<float4> Bloom : register(u0);

static const float4 LUM_FACTOR = float4(0.299, 0.587, 0.114, 0);

#define groupthreads 128

[numthreads(1024, 1, 1)]
void main( uint3 dispatchThreadId : SV_DispatchThreadID ){
	uint2 CurPixel = uint2(dispatchThreadId.x % nQuadWidth, dispatchThreadId.x / nQuadWidth);

	if (CurPixel.y < nQuadHeight) {
		float4 color = HDRDownScaleTex.Load(int3(CurPixel, 0));
		float Lum = dot(color, LUM_FACTOR);
		float avgLum = AvgLum[0];

		//Find the color scale
		float colorScale = saturate(Lum - avgLum * fBloomThreshold);

		Bloom[CurPixel.xy] = color * colorScale;
		//Bloom[CurPixel.xy] = float4(AvgLum[0], AvgLum[0], AvgLum[0], 1.0);
	}
}