Texture2D<float> DepthTex : register(t0);
RWTexture2D<unorm float> OcclussionRW : register(u0);

cbuffer stCSGlobalBufferData : register(b13) {
	uint nWidth : packoffset(c0);
	uint nHeight: packoffset(c0.y);
	uint nHalfWidth: packoffset(c0.z);
	uint nHalfHeight: packoffset(c0.w);;
	uint nQuadWidth: packoffset(c1.x);
	uint nQuadHeight: packoffset(c1.y);
};

[numthreads(1024, 1, 1)]
void main( uint3 dispatchThreadId : SV_DispatchThreadID ){
	uint3 CurPixel = uint3(dispatchThreadId.x % nHalfWidth, dispatchThreadId.x / nHalfWidth, 0);

	if (CurPixel.y < nHalfHeight){
		// Get the depth
		float curDepth = DepthTex.Load(CurPixel);
	
		// sky box¸é 1 return
		OcclussionRW[CurPixel.xy] = (curDepth > 0.99);
	}
}